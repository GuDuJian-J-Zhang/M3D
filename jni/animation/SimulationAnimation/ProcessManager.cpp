#include "stdafx.h"
#include "ProcessManager.h"
#include "../SimulationCommon/UtilityXMLParser.h"
#include "../SimulationCommon/vlist.h"
#include "AnimationStepManager.h"
#include "../../Utility.h"
#include <cstdlib>
#include <typeinfo>

SA_NAMESPACE_BEGIN

CProcessManager::CProcessManager(const int ID, const char *name)
{
	m_ID = ID;
	m_ProcessList = new_vlist(malloc, free);
	m_CurProcessID = -1;
	if(!name)
	{
		sprintf(m_Name,"过程%d",ID+1);
#ifndef SVIEW_DESIGNER
#ifdef WIN32
		SA_UTF8 strName(m_Name);
		strcpy(m_Name, (const char*)strName.encodedText());
#else

#endif
#endif

	}
	else
	{
		strcpy(m_Name, name);
	}
	strcpy(m_Desc, "");
	m_pAnimationStepManager = 0;
	m_pParentProcessManager = NULL;
	m_bPlay = true;
	m_ChildProcessManagerList = new_vlist(malloc, free);
	m_ReferenceCount = 0;
}

CProcessManager::~CProcessManager(void)
{
	DeleteAllProcess();
	delete_vlist(m_ProcessList);
	DeleteAllChildProcessManager(false);
	delete_vlist(m_ChildProcessManagerList);
}

void CProcessManager::Reference()
{
	m_ReferenceCount++;
}
void CProcessManager::Release()
{
	m_ReferenceCount--;
	if (m_ReferenceCount <= 0)
	{
		delete this;
	}
}

void CProcessManager::SetName(const char* name)
{
	if(!name)
	{
		return;
	}
	if(strlen(name)>(SA_BUFFER_SIZE_SMALL-1))
	{
		strcpy(m_Name, name);
	}
	else
	{
		strcpy(m_Name,name);
	}

}
void CProcessManager::SetDesc(const char* desc)
{
	if(!desc)
	{
		return;
	}
	if(strlen(desc)>(SA_BUFFER_SIZE_BIG-1))
	{
		strcpy(m_Desc,desc);
	}
	else
	{
		strcpy(m_Desc,desc);
	}

}

void CProcessManager::Serialize(CUtilityXMLGenerator *xmlgen)
{
	CUtilityXMLTag xmlt;
	xmlt.SetTagname("ProcessManager");
	xmlt.AddPropertyInt("ID", m_ID);
	xmlt.AddProperty("Name", m_Name, true);
	xmlt.AddProperty("Desc", m_Desc, true);
	xmlt.AddPropertyInt("CurProcessID", m_CurProcessID);
	xmlt.SetOpen(true);
	xmlgen->AddTag(&xmlt);

	vlist_reset_cursor(m_ProcessList);
	
	while (1)
	{
		CProcess *temp;
		//xmlgenSA.Reset();
		if (!(temp = (CProcess *)vlist_peek_cursor(m_ProcessList)))
			break;
		temp->Serialize(xmlgen);
		
		vlist_advance_cursor(m_ProcessList);	
	}

	vlist_reset_cursor(m_ChildProcessManagerList);
	while (1)
	{
		CProcessManager *temp;
		//xmlgenSA.Reset();
		if (!(temp = (CProcessManager *)vlist_peek_cursor(m_ChildProcessManagerList)))
			break;
		temp->Serialize(xmlgen);

		vlist_advance_cursor(m_ChildProcessManagerList);
	}

	//xmlgen->AddTag(&xmltSA);
	xmlgen->CloseTag(&xmlt);
}

CProcess * CProcessManager::AddProcess( const int ID, const char *name, const bool bAddBehaviorAction)
{
	
	CProcess* pProcess = NULL;
	if((pProcess = FindProcessByID(ID)) && ID > 0)
	{
		pProcess->SetName(name);
		pProcess->DeleteAllTargetObject();

		//清空原来的动画
		if(m_pAnimationStepManager &&
			m_pAnimationStepManager->GetSimulationAnimationManager()&&
			m_pAnimationStepManager->GetSimulationAnimationManager()->FindSimAniByID(pProcess->GetBehaviorActionID()))
		{
			m_pAnimationStepManager->GetSimulationAnimationManager()->FindSimAniByID(pProcess->GetBehaviorActionID())->DeleteAllAnimations();
		}
	}
	else
	{
		//自动创建动画管理器
		CSBehaviorAction* pBehaviorAction = NULL;
		long nBehaviorActionID = 0;
		if(m_pAnimationStepManager &&
			m_pAnimationStepManager->GetSimulationAnimationManager())
		{
			if(bAddBehaviorAction)
			{
				nBehaviorActionID = m_pAnimationStepManager->GetSimulationAnimationManager()->RegisterBehaviorActionID();
				pBehaviorAction = m_pAnimationStepManager->GetSimulationAnimationManager()->AddSimAni(nBehaviorActionID);
			}
			else
			{
				nBehaviorActionID = m_pAnimationStepManager->GetSimulationAnimationManager()->GetCurSAID();
			}
		}

		//创建过程
		if(ID == -1)
		{
			pProcess = new CProcess(RegisterProcessID(), nBehaviorActionID, name);
		}
		else
		{
			pProcess = new CProcess(ID, nBehaviorActionID, name);
		}
		if(pBehaviorAction)
		{
			pBehaviorAction->SetName(pProcess->GetName());
		}
		AddProcess(pProcess);
	}
	
	return pProcess;
}

void CProcessManager::AddProcess( CProcess* pProcess )
{
	
	if(NULL != pProcess &&
		!FindProcessByID(pProcess->GetID()))
	{
		pProcess->Reference();
		vlist_add_last(m_ProcessList, pProcess);
		m_CurProcessID = pProcess->GetID();
		pProcess->SetProcessManager(this);
	}
}

void CProcessManager::AddProcess(CProcess* pProcess, CProcess* pPreProcess)
{
	//在步骤的后边添加指定步骤
	if(NULL != pProcess && NULL != pPreProcess)
	{	
		int nIdx = GetProcessIdxByID(pPreProcess->GetID());
		vlist_nth_item(m_ProcessList,nIdx);
		vlist_add_after_cursor(m_ProcessList, pProcess);
		m_CurProcessID = pProcess->GetID();
		pProcess->SetProcessManager(this);
		pProcess->Reference();
	}
}

void CProcessManager::DeleteProcess( CProcess* pProcess )
{
	if(NULL != pProcess)
	{
		if (!FindProcessByID(pProcess->GetID()))
		{
			return;
		}
		if(pProcess->GetID() == m_CurProcessID)
		{
			CProcess* pNewCurProcess = NULL;
			if((pNewCurProcess = GetNextProcess(pProcess)))
			{
				m_CurProcessID = pNewCurProcess->GetID();
			}
			else if((pNewCurProcess = GetPreProcess(pProcess)))
			{
				m_CurProcessID = pNewCurProcess->GetID();
			}
			else
			{
				m_CurProcessID = -1;
			}

			//删除过程后，设置当前活动的动画管理器与当前过程一致
			if(GetAnimationStepManager() && 
				GetAnimationStepManager()->GetSimulationAnimationManager())
			{
				if(pNewCurProcess)
				{
					GetAnimationStepManager()->GetSimulationAnimationManager()->SetCurSAByID(pNewCurProcess->GetBehaviorActionID());
				}
			}
		}

		vlist_remove(m_ProcessList, pProcess);
		pProcess->SetProcessManager(NULL);
		pProcess->Release();
	}
}

void CProcessManager::EraseProcess(CProcess* pProcess)
{
	if(NULL != pProcess)
	{
		if(pProcess->GetID() == m_CurProcessID)
		{
			CProcess* pNewCurProcess = NULL;
			if((pNewCurProcess = GetNextProcess(pProcess)))
			{
				m_CurProcessID = pNewCurProcess->GetID();
			}
			else if((pNewCurProcess = GetPreProcess(pProcess)))
			{
				m_CurProcessID = pNewCurProcess->GetID();
			}
			else
			{
				m_CurProcessID = -1;
			}

			//删除过程后，设置当前活动的动画管理器与当前过程一致
			if(GetAnimationStepManager() && 
				GetAnimationStepManager()->GetSimulationAnimationManager())
			{
				if(pNewCurProcess)
				{
					GetAnimationStepManager()->GetSimulationAnimationManager()->SetCurSAByID(pNewCurProcess->GetBehaviorActionID());
				}
			}
		}
		vlist_remove(m_ProcessList, pProcess);
		pProcess->SetProcessManager(NULL);
	}
}

void CProcessManager::DeleteAllProcess()
{
	START_LIST_ITERATION(CProcess, m_ProcessList)
		if (temp)
		{
			temp->SetProcessManager(NULL);
			temp->Release();
		}
	END_LIST_ITERATION(m_ProcessList)

	vlist_flush(m_ProcessList);
	m_CurProcessID = -1;
}

CProcess * CProcessManager::FindProcessByID( const int ID )
{
	START_LIST_ITERATION(CProcess, m_ProcessList)
		if(ID == temp->GetID())
		{
			//m_CurProcessID = ID;
			return temp;
		}
		END_LIST_ITERATION(m_ProcessList)
			return NULL;
}

CProcess* CProcessManager::FindProcessByAnimationID(const int ID)
{
	START_LIST_ITERATION(CProcess, m_ProcessList)
		if(ID == temp->GetBehaviorActionID())
		{
			return temp;
		}
		END_LIST_ITERATION(m_ProcessList)
			return NULL;
}

CProcess * CProcessManager::FindProcessByName(const char *name)
{
	START_LIST_ITERATION(CProcess, m_ProcessList)
		if (temp->GetName() && strcmp(temp->GetName(), name) == 0)
			return temp;							
	END_LIST_ITERATION(m_ProcessList)
		return 0;
}

CProcessManager* CProcessManager::FindChildProcessManagerByName(const char *name)
{
	START_LIST_ITERATION(CProcessManager, m_ChildProcessManagerList)
		if (temp->GetName() && strcmp(temp->GetName(), name) == 0)
			return temp;
	END_LIST_ITERATION(m_ChildProcessManagerList)
		return NULL;
}

void CProcessManager::SetCurProcessByID(int ID,bool bUpdateModel/* = true*/, bool bUpdateCam/* = false*/, bool bCamAni/* = false*/)
{
	CSimulationAnimationManager* pSAManager = m_pAnimationStepManager->GetSimulationAnimationManager();
    if(pSAManager == NULL){
        return;
    }
	if(m_CurProcessID != ID)
	{
		m_CurProcessID = ID;
		//设置活动过程时，自动设置活动的过程管理器
		if(GetAnimationStepManager())
		{
			if(GetAnimationStepManager()->GetCurProcessManagerID() != m_ID)
			{
				GetAnimationStepManager()->SetCurProcessManagerByID(m_ID,bUpdateModel,bUpdateCam,bCamAni);
			}
			else
			{
				if( SATools::VersionCompare("2.2",pSAManager->GetVersion()) != 0)
				{
					//根据过程中记录的对象状态，更新视口的显示状态
					CProcess* pCurProcess = GetCurrentProcess();
					if(pCurProcess)
					{
						pCurProcess->UpdateView(bUpdateModel,bUpdateCam,bCamAni);
					}
				}
				else
				{
					//兼容过程不包含对象状态的分段动画演示版本
					if(bUpdateModel)
					{
						//int idx = GetProcessIdxByID(ID);
						//UpdateViewWithProcess(idx);
						int idx = GetAnimationStepManager()->GetProcessManagerIdxByID(m_ID);
						GetAnimationStepManager()->UpdateViewWithProcessManager(idx);
					}
					if(bUpdateCam)
					{
						CProcess* pCurProcess = GetCurrentProcess();
						if(pCurProcess)
						{
							GetAnimationStepManager()->StartChangeCamera(pCurProcess,bCamAni);
						}
					}
				}
			}
		}
	}
	else if(GetAnimationStepManager()->GetCurProcessManagerID() != m_ID)
	{
		GetAnimationStepManager()->SetCurProcessManagerByID(m_ID,bUpdateModel,bUpdateCam,bCamAni);
	}

	CProcess* pProcess = GetCurrentProcess();
	if(pProcess)
	{
		pSAManager->SetCurSAByID(pProcess->GetBehaviorActionID());
	}
}

void CProcessManager::SetCurProcessByIdx(int idx,bool bUpdateModel/* = true*/, bool bUpdateCam/* = false*/, bool bCamAni/* = false*/)
{
	CSimulationAnimationManager* pSAManager = m_pAnimationStepManager->GetSimulationAnimationManager();
	CProcess* pProcess = GetProcessByIdx(idx);
	if(pProcess)
	{
		if(m_CurProcessID != pProcess->GetID())
		{
			m_CurProcessID = pProcess->GetID();
			//设置活动过程时，自动设置活动的过程管理器
			if(GetAnimationStepManager())
			{
				if(GetAnimationStepManager()->GetCurProcessManagerID() != m_ID)
				{
					GetAnimationStepManager()->SetCurProcessManagerByID(m_ID,bUpdateModel,bUpdateCam,bCamAni);
				}
				else
				{
					if( SATools::VersionCompare("2.2",pSAManager->GetVersion()) != 0)
					{
						//根据过程中记录的对象状态，更新视口的显示状态
						CProcess* pCurProcess = GetCurrentProcess();
						if(pCurProcess)
						{
							pCurProcess->UpdateView(bUpdateModel,bUpdateCam,bCamAni);
						}
					}
					else
					{
						//兼容过程不包含对象状态的分段动画演示版本
						if(bUpdateModel)
						{	
							//UpdateViewWithProcess(idx);
							int idxProcessManager = GetAnimationStepManager()->GetProcessManagerIdxByID(m_ID);
							GetAnimationStepManager()->UpdateViewWithProcessManager(idxProcessManager);
						}
						if(bUpdateCam)
						{
							GetAnimationStepManager()->StartChangeCamera(pProcess,bCamAni);
						}
					}
				}
			}
		}
		else if(GetAnimationStepManager()->GetCurProcessManagerID() != m_ID)
		{
			GetAnimationStepManager()->SetCurProcessManagerByID(m_ID,bUpdateModel,bUpdateCam,bCamAni);
		}
		pSAManager->SetCurSAByID(pProcess->GetBehaviorActionID());
	}	
}

void CProcessManager::UpdateViewWithProcess(int nProcessIdx)
{
	CSimulationAnimationManager* pSAManager = m_pAnimationStepManager->GetSimulationAnimationManager();

	//根据过程更新视口时，暂屏蔽播放场景功能，防止场景动画与过程的场景冲突
	bool bPlayCamera = pSAManager->IsCameraPlay();
	pSAManager->SetCameraPlay(false);

	if(!pSAManager->IsPlaying())
	{
		//设置视口的当前状态，要考虑之前和之后过程的影响
		//当前过程之后的过程对应动画播放到最后
		for(int i = 0; i< nProcessIdx; i++)
		{
			CProcess* pTempProcess = GetProcessByIdx(i);
			if(pTempProcess)
			{
				CSBehaviorAction* pBehaviorAction = pTempProcess->GetBehaviorAction();
				if(pBehaviorAction)
				{
					pBehaviorAction->RewindReverse();
				}
			}
		}

		//当前过程之后的过程对应动画复位
		for(int i = GetProcessCount()-1; i> nProcessIdx; i--)
		{
			CProcess* pTempProcess = GetProcessByIdx(i);
			if(pTempProcess)
			{
				CSBehaviorAction* pBehaviorAction = pTempProcess->GetBehaviorAction();
				if(pBehaviorAction)
				{
					pBehaviorAction->Rewind();
				}
			}
		}

		//如果当前处于循环播放时一次的结束状态，根据是否倒播设置当前过程的状态
		CProcess* pTempProcess = GetProcessByIdx(nProcessIdx);
		if(pTempProcess)
		{
			CSBehaviorAction* pBehaviorAction = pTempProcess->GetBehaviorAction();
			if(pBehaviorAction && m_pAnimationStepManager->GetPlayMode() != CAnimationStepManager::PLAY_MODE_NONE)
			{
				if(pSAManager->GetReversePlay())
				{
					pBehaviorAction->RewindReverse();
				}
				else
				{
					pBehaviorAction->Rewind();
				}
			}
		}
	}
	pSAManager->SetCameraPlay(bPlayCamera);
}

CProcess* CProcessManager::GetProcessByIdx(int idx)
{
	CProcess* pProcess = 0;
	if((int)vlist_count(m_ProcessList) >0 && (int)vlist_count(m_ProcessList)>idx)
	{
		pProcess = (CProcess*)vlist_nth_item(m_ProcessList,idx);
	}
	return pProcess;
}

int CProcessManager::GetProcessIdxByID(const int ID)
{
	if(FindProcessByID(ID))
	{
		return m_ProcessList->cursor_index;
	}
	else
	{
		return -1;
	}
}

void * CProcessManager::XMLCallback( CUtilityXMLTag *xt, bool open, void *m_pExtraData )
{
	if(!xt->GetXMLParser()->GetStackData())
	{
		return NULL;
	}
	CAnimationStepManager *pAnimationStepManager = (CAnimationStepManager *)xt->GetXMLParser()->GetStackData();
	CProcessManager *pParentProcessManager = (CProcessManager *)xt->GetXMLParser()->GetStackData();
	CProcessManager *pProcessManager =0;
	/*char*	strSABuffer = NULL;*/
	if(open)
	{
		char Name[SA_BUFFER_SIZE_SMALL] = {0};
		char Desc[SA_BUFFER_SIZE_BIG] = {0};
		int ID = 0;
		int CurProcessID = 0;

		if (!xt->GetProperty("ID", ID))
			ID = 0;
		if (!xt->GetProperty("Name", Name))
			sprintf(Name,"过程%d",ID+1);
		if (!xt->GetProperty("Desc", Desc))
			strcpy(Desc, "");
		if (!xt->GetProperty("CurProcessID", CurProcessID))
			CurProcessID = 0;
		if (typeid(*pAnimationStepManager) == typeid(CAnimationStepManager))
		{
			pProcessManager = pAnimationStepManager->AddProcessManager(ID, Name);
			pProcessManager->SetCurProcessByID(CurProcessID, false);
			pProcessManager->SetDesc(Desc);
		}
		else if (typeid(*pParentProcessManager) == typeid(CProcessManager))
		{
			pAnimationStepManager = pParentProcessManager->GetAnimationStepManager();
			pProcessManager = pAnimationStepManager->AddProcessManager(ID, Name);
			pProcessManager->SetCurProcessByID(CurProcessID, false);
			pProcessManager->SetDesc(Desc);

			pParentProcessManager->AddChildProcessManager(pProcessManager);
			pProcessManager->SetParentProcessManager(pParentProcessManager);
		}

		return pProcessManager;
	}
	else
	{
		pProcessManager =(CProcessManager *)xt->GetXMLParser()->GetStackData();
		int CurProcessID = 0;
		if (!xt->GetProperty("CurProcessID", CurProcessID))
			CurProcessID = 0;
		pProcessManager->SetCurProcessByID(CurProcessID,false);
	}
	return pProcessManager;
}

CProcess* CProcessManager::GetCurrentProcess()
{
	if (GetProcessCount() <= 0)
	{
		for (int i = 0; i < GetChildProcessManagerCount(); i++)
		{
			CProcessManager* pChildProcessManager = GetChildProcessManagerByIdx(i);
			if (pChildProcessManager && 
				pChildProcessManager->GetCurrentProcess())
			{
				return pChildProcessManager->GetCurrentProcess();
			}
		}
	}
	else if(-1 != m_CurProcessID)
	{
		return FindProcessByID(m_CurProcessID);
	}
	return NULL;
}

int CProcessManager::GetMaxProcessID()
{
	int iMaxID = 0;
	int itmpID = 0;
	START_LIST_ITERATION(CProcess, m_ProcessList)
		itmpID = temp->GetID();
	if(itmpID > iMaxID)
		iMaxID = itmpID;
	END_LIST_ITERATION(m_ProcessList)
		return iMaxID;
}

long CProcessManager::RegisterProcessID()
{
	long iRet = 0;
	START_LIST_ITERATION(CProcess, m_ProcessList)
		if(temp->GetID() >= iRet)
		{
			iRet = temp->GetID()+1;
		}
		END_LIST_ITERATION(m_ProcessList)
			return iRet;
}

void CProcessManager::StartListProcessQuery()
{
	vlist_reset_cursor(m_ProcessList);
}

bool CProcessManager::GetNextListProcess(CProcess** opProcess)
{
	void *temp;
	if (!(temp = vlist_peek_cursor(m_ProcessList)))
		return false;
	*opProcess = (CProcess *)temp;
	vlist_advance_cursor(m_ProcessList);	
	return true;
}

bool CProcessManager::HasListProcess()
{
	vlist_reset_cursor(m_ProcessList);
	if (!vlist_peek_cursor(m_ProcessList))
		return false;
	else 
		return true;
}
int CProcessManager::GetProcessCount() 
{ 
	return m_ProcessList->count; 
}


bool CProcessManager::MoveProcess(const int oldIdx,const int newIdx)
{
	return vlist_move_item(m_ProcessList,oldIdx,newIdx);
}

bool CProcessManager::MoveProcess(CProcess* pProcess,CProcess* pTargetProcess)
{
	if (!pProcess || !pTargetProcess)
		return false;
	int oldIdx = GetProcessIdxByID(pProcess->GetID());
	int newIdx = GetProcessIdxByID(pTargetProcess->GetID());
	return vlist_move_item(m_ProcessList,oldIdx,newIdx);
}

bool CProcessManager::MoveProcessToFirst(CProcess* pProcess, bool bUnlink)
{
	bool bRet = false;
	if(NULL != pProcess)
	{
		//断开原有连接
		if(bUnlink)
			vlist_remove(m_ProcessList, pProcess);
		//添加到开始位置
		if(!bUnlink)
		{
			pProcess->SetID(RegisterProcessID());
			//过程间移动 名称的判断
			CProcess* pTmpProcess = FindProcessByName(pProcess->GetName());
			int index = 1;
			string strName = pProcess->GetName();
			//循环查询
			while(pTmpProcess)
			{
				index++;
				strName = pProcess->GetName();
				strName += "(";
				char szIndex[32] = {0};
				sprintf(szIndex,"%d",index);
				strName += szIndex;
				strName += ")";
				pTmpProcess = FindProcessByName(strName.c_str());
			}
			pProcess->SetName(strName.c_str());
		}
		m_CurProcessID = pProcess->GetID();
		pProcess->SetProcessManager(this);
		vlist_add_first(m_ProcessList, pProcess);
		bRet = true;
	}
	return bRet;
}

CProcess* CProcessManager::GetPreProcess(CProcess* pProcess)
{
	CProcess* pPreProcess = NULL;
	int curProcessIdx = GetProcessIdxByID(pProcess->GetID());
	if(curProcessIdx >0)
	{
		pPreProcess = GetProcessByIdx(curProcessIdx-1);
	}
	return pPreProcess;
}

CProcess* CProcessManager::GetNextProcess(CProcess* pProcess)
{
	CProcess* pNextProcess = NULL;
	int curProcessIdx = GetProcessIdxByID(pProcess->GetID());
	if(curProcessIdx < GetProcessCount()-1)
	{
		pNextProcess = GetProcessByIdx(curProcessIdx+1);
	}
	return pNextProcess;
}

void CProcessManager::AddChildProcessManager(CProcessManager* pProcessManager)
{
	if (pProcessManager)
	{
		vlist_add_last(m_ChildProcessManagerList, pProcessManager);
		pProcessManager->SetParentProcessManager(this);
	}
}

void CProcessManager::DeleteChildProcessManager(CProcessManager* pProcessManager,bool bDeleteData/* = true*/)
{
	if ( pProcessManager)
	{
		
		//删除过程后，设置当前活动的动画管理器与当前过程一致
		if (GetAnimationStepManager() && bDeleteData)
		{
			GetAnimationStepManager()->DeleteProcessManager(pProcessManager);
		}

		vlist_remove(m_ChildProcessManagerList, pProcessManager);
	}
}

void CProcessManager::DeleteAllChildProcessManager(bool bDeleteData/* = true*/)
{
	if (GetAnimationStepManager() && bDeleteData)
	{
		START_LIST_ITERATION(CProcessManager, m_ChildProcessManagerList)
			GetAnimationStepManager()->DeleteProcessManager(temp);
		END_LIST_ITERATION(m_ChildProcessManagerList)
	}

	vlist_flush(m_ChildProcessManagerList);
}

CProcessManager* CProcessManager::GetChildProcessManagerByIdx(int idx)
{
	CProcessManager* pProcessManager = 0;
	if (vlist_count(m_ChildProcessManagerList) > 0)
	{
		pProcessManager = (CProcessManager*)vlist_nth_item(m_ChildProcessManagerList, idx);
	}
	return pProcessManager;
}

CProcessManager * CProcessManager::GetChildProcessManagerByID(const int ID)
{
	START_LIST_ITERATION(CProcessManager, m_ChildProcessManagerList)
		if (ID == temp->GetID())
		{
			return temp;
		}
	END_LIST_ITERATION(m_ChildProcessManagerList)
		return NULL;
}

int CProcessManager::GetChildProcessManagerIdxByID(const int ID)
{
	if (GetChildProcessManagerByID(ID))
	{
		return m_ChildProcessManagerList->cursor_index;
	}
	else
	{
		return -1;
	}
}


int CProcessManager::GetChildProcessManagerCount()
{
	return m_ChildProcessManagerList->count;
}

bool CProcessManager::MoveChildProcessManager(const int oldIdx, const int newIdx)
{
	return vlist_move_item(m_ChildProcessManagerList, oldIdx, newIdx);
}

bool CProcessManager::MoveChildProcessManager(CProcessManager* pProcessManager, CProcessManager* pTargetProcessManager)
{
	if (!pProcessManager ||
		!pTargetProcessManager)
		return false;
	int oldIdx = GetChildProcessManagerIdxByID(pProcessManager->GetID());
	int newIdx = GetChildProcessManagerIdxByID(pTargetProcessManager->GetID());
	return vlist_move_item(m_ChildProcessManagerList, oldIdx, newIdx);
}

bool CProcessManager::MoveChildProcessManagerToFirst(CProcessManager* pProcessManager, bool bUnlink)
{
	bool bRet = false;
	if (!GetAnimationStepManager())
	{
		return bRet;
	}
	if (NULL != pProcessManager)
	{
		//断开原有连接
		if (bUnlink)
		{
			vlist_remove(m_ChildProcessManagerList, pProcessManager);
		}
		//添加到开始位置
		else
		{
			//过程间移动 名称的判断
			CProcessManager* pTmpProcessManager = GetAnimationStepManager()->FindProcessManagerByName(pProcessManager->GetName());
			int index = 1;
			string strName = pTmpProcessManager->GetName();
			//循环查询
			while (pTmpProcessManager)
			{
				index++;
				strName = pTmpProcessManager->GetName();
				strName += "(";
				char szIndex[32] = { 0 };
#ifdef WIN32
				_itoa_s(index, szIndex, 10);
#else
                 sprintf(szIndex, "%d", index);
#endif
				strName += szIndex;
				strName += ")";
				pTmpProcessManager = GetAnimationStepManager()->FindProcessManagerByName(strName.c_str());
			}
			pProcessManager->SetName(strName.c_str());
		}
		GetAnimationStepManager()->SetCurProcessManagerByID(pProcessManager->GetID());
		pProcessManager->SetParentProcessManager(this);
		vlist_add_first(m_ChildProcessManagerList, pProcessManager);
		bRet = true;
	}
	return bRet;
}


CProcessManager* CProcessManager::GetPreChildProcessManager(CProcessManager* pProcessManager)
{
	CProcessManager* pPreProcessManager = NULL;
	int curProcessManagerIdx = GetChildProcessManagerIdxByID(pProcessManager->GetID());
	if (curProcessManagerIdx > 0)
	{
		pPreProcessManager = GetChildProcessManagerByIdx(curProcessManagerIdx - 1);
	}
	return pPreProcessManager;
}

CProcessManager* CProcessManager::GetNextChildProcessManager(CProcessManager* pProcessManager)
{
	CProcessManager* pNextProcessManager = NULL;
	int curProcessManagerIdx = GetChildProcessManagerIdxByID(pProcessManager->GetID());
	if (curProcessManagerIdx < GetChildProcessManagerCount() - 1)
	{
		pNextProcessManager = GetChildProcessManagerByIdx(curProcessManagerIdx + 1);
	}
	return pNextProcessManager;
}

CProcessManager * CProcessManager::GetFirstChildProcessManager()
{
	CProcessManager* pFistProcessManager = GetChildProcessManagerByIdx(0);
	return pFistProcessManager;
}

CProcessManager * CProcessManager::GetLastChildProcessManager(bool bStep /*= false*/)
{
	CProcessManager* pLastProcessManager = NULL;
	int childProcessManagerCount = GetChildProcessManagerCount();
	if(childProcessManagerCount>0)
	{
		pLastProcessManager = GetChildProcessManagerByIdx(childProcessManagerCount-1);
		if (bStep && pLastProcessManager &&
			pLastProcessManager->GetChildProcessManagerCount() > 0)
		{
			pLastProcessManager = pLastProcessManager->GetLastChildProcessManager(bStep);
		}
	}
	return pLastProcessManager;
}

CProcess * CProcessManager::GetFirstProcess(bool bStep)
{
	CProcess* pFistProcess = NULL;
	if (GetProcessCount() > 0)
	{
		pFistProcess = GetProcessByIdx(0);
	}
	else if(bStep)
	{
		CProcessManager* pFirstProcessManager = GetChildProcessManagerByIdx(0);
		if (pFirstProcessManager)
		{
			pFistProcess = pFirstProcessManager->GetFirstProcess();
		}
	}
	return pFistProcess;
}

CProcess * CProcessManager::GetLastProcess(bool bStep /*= false*/)
{
	CProcess* pLastProcess = NULL;
	if (bStep)
	{
		CProcessManager* pLastProcessManager = GetLastChildProcessManager(bStep);
		if (pLastProcessManager)
		{
			pLastProcess = pLastProcessManager->GetLastProcess(bStep);
		}
	}
	if (!pLastProcess && GetProcessCount() > 0)
	{
		pLastProcess = GetProcessByIdx(GetProcessCount()-1);
	}
	return pLastProcess;
}

CProcessManager* CProcessManager::Clone(CProcessManager* pParentProcessManager/* = NULL*/)
{
	//克隆功能 实现过程动画的复制
	CProcessManager* pNewProcessManager = NULL;
	if (!m_pAnimationStepManager)
		return pNewProcessManager;
	pNewProcessManager = new CProcessManager(m_pAnimationStepManager->RegisterProcessManagerID(), m_Name);
	if (!pNewProcessManager)
		return pNewProcessManager;
	pNewProcessManager->SetDesc(m_Desc);

	CProcessManager* pChildProcessManager = pParentProcessManager->FindChildProcessManagerByName(m_Name);
	int index = 1;
	string strName = m_Name;
	//循环查询
	while (pChildProcessManager)
	{
		index++;
		strName = m_Name;
		strName += "(";
		char szIndex[32] = { 0 };
#ifdef WIN32
        _itoa_s(index, szIndex, 10);
#else
        sprintf(szIndex, "%d", index);
#endif
		strName += szIndex;
		strName += ")";
		pChildProcessManager = pParentProcessManager->FindChildProcessManagerByName(strName.c_str());
	}

	pNewProcessManager->SetName(strName.c_str());
	m_pAnimationStepManager->AddProcessManager(pNewProcessManager);
	pNewProcessManager->SetAnimationStepManager(m_pAnimationStepManager);
	pNewProcessManager->SetCurProcessByID(m_CurProcessID);
	pNewProcessManager->SetParentProcessManager(pParentProcessManager);
	if (pParentProcessManager)
	{
		pParentProcessManager->AddChildProcessManager(pNewProcessManager);
	}

	START_LIST_ITERATION(CProcess, m_ProcessList)
		CProcess* pNewProcess = temp->Clone(pNewProcessManager);
		if (pNewProcess)
			pNewProcess->SetName(temp->GetName());
	END_LIST_ITERATION(m_ProcessList)

	START_LIST_ITERATION(CProcessManager, m_ChildProcessManagerList)
		CProcessManager* pNewChildProcessManager = temp->Clone(pNewProcessManager);
	END_LIST_ITERATION(m_ChildProcessManagerList)
	return pNewProcessManager;
}
bool CProcessManager::IsChildProcessManager(CProcessManager* pProcessManager, bool bSteps /*= true*/)
{
	bool bRet = false;
	START_LIST_ITERATION(CProcessManager, m_ChildProcessManagerList)
		if (temp == pProcessManager)
		{
			bRet = true;
			break;
		}
		else if (bSteps)
		{
			if (temp->IsChildProcessManager(pProcessManager, bSteps))
			{
				bRet = true;
				break;
			}
		}
	END_LIST_ITERATION(m_ChildProcessManagerList)
	return bRet;
}
SA_NAMESPACE_END


