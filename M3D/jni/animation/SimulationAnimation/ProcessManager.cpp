#include "stdafx.h"
#include "ProcessManager.h"
#include "../SimulationCommon/UtilityXMLParser.h"
#include "../SimulationCommon/vlist.h"
#include "AnimationStepManager.h"
#include "../../Utility.h"

SA_NAMESPACE_BEGIN

CProcessManager::CProcessManager(const int ID, const char *name)
{
	m_ID = ID;
	m_ProcessList = new_vlist( malloc, free );
	m_CurProcessID = -1;
	if(!name)
	{
		sprintf(m_Name,"过程%d",ID+1);
	}
	else
	{
		strcpy(m_Name, name);
	}
	strcpy(m_Desc, "");
	m_pAnimationStepManager = 0;
}

CProcessManager::~CProcessManager(void)
{
	DeleteAllProcess();
	delete_vlist(m_ProcessList);
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
	
	if(NULL != pProcess)
	{
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
	}
}

void CProcessManager::DeleteProcess( CProcess* pProcess )
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
		delete pProcess;
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
	}
}

void CProcessManager::DeleteAllProcess()
{
	START_LIST_ITERATION(CProcess, m_ProcessList)
		delete temp;
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

void CProcessManager::SetCurProcessByID(int ID,bool bUpdateModel/* = true*/, bool bUpdateCam/* = false*/, bool bCamAni/* = false*/)
{
	CSimulationAnimationManager* pSAManager = m_pAnimationStepManager->GetSimulationAnimationManager();
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
			if(pBehaviorAction)
			{
				if(pSAManager->GetReversePlay() && m_pAnimationStepManager->GetPlayMode() != CAnimationStepManager::PLAY_MODE_NONE)
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

		pProcessManager = pAnimationStepManager->AddProcessManager(ID,Name);
		pProcessManager->SetCurProcessByID(CurProcessID,false);

		pProcessManager->SetDesc(Desc);

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
	if(-1 != m_CurProcessID)
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


SA_NAMESPACE_END


