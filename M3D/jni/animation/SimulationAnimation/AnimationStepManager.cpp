#include "stdafx.h"
#include <typeinfo>
#include "AnimationStepManager.h"
#include "../SimulationCommon/UtilityXMLParser.h"
#include "../SimulationCommon/vlist.h"
#include "../SimulationCommon/MatrixOperation.h"
#include "../SimulationCommon/SAUtility.h"
#include "../../Utility.h"
SA_NAMESPACE_BEGIN

CAnimationStepManager::CAnimationStepManager(const char *name)
{
	m_ProcessManagerList = new_vlist( malloc, free );
	m_CurProcessManagerID = -1;
	if(!name)
	{
		strcpy(m_Name, "动画过程管理器");
	}
	else
	{
		strcpy(m_Name, name);
	}
	m_pSA = NULL;
	m_PlayMode = PLAY_MODE_NONE;
	m_pBehaviorActionChgCam = NULL;
	m_nChgCamTickNum = 10;
	m_bIsPause = false;
}

CAnimationStepManager::~CAnimationStepManager(void)
{
	DeleteAllProcessManager();
	delete_vlist(m_ProcessManagerList);

	if(m_pBehaviorActionChgCam)
	{
		CSimulationAnimationManager* pSAManager = m_pBehaviorActionChgCam->GetSimulationAnimationManager();
		if(pSAManager)
		{
			pSAManager->DeleteSimAni(m_pBehaviorActionChgCam);
			if(GetCurrentProcess() &&
				GetCurrentProcess()->GetBehaviorAction())
			{
				pSAManager->SetCurSAByID(GetCurrentProcess()->GetBehaviorAction()->GetID());
			}
		}
		m_pBehaviorActionChgCam = NULL;
	}
}

void CAnimationStepManager::Serialize(CUtilityXMLGenerator *xmlgen)
{
	
	CUtilityXMLTag xmlt;
	xmlt.SetTagname("AnimationStepManager");
	xmlt.AddProperty("Name", m_Name, true);
	xmlt.AddPropertyInt("CurProcessManagerID", m_CurProcessManagerID);
	xmlt.SetOpen(true);
	xmlgen->AddTag(&xmlt);

	vlist_reset_cursor(m_ProcessManagerList);
	
	while (1)
	{
		CProcessManager *temp;
		//xmlgenSA.Reset();
		if (!(temp = (CProcessManager *)vlist_peek_cursor(m_ProcessManagerList)))
			break;
		temp->Serialize(xmlgen);
		
		vlist_advance_cursor(m_ProcessManagerList);	
	}
	//xmlgen->AddTag(&xmltSA);
	xmlgen->CloseTag(&xmlt);
}

CProcessManager * CAnimationStepManager::AddProcessManager( const int ID, const char *name)
{
	
	CProcessManager* pProcessManager = NULL;
	if((pProcessManager = FindProcessManagerByID(ID)) && ID > 0)
	{
		pProcessManager->SetName(name);
		pProcessManager->DeleteAllProcess();
	}
	else
	{
		if(ID == -1)
		{
			pProcessManager = new CProcessManager(RegisterProcessManagerID(), name);
		}
		else
		{
			pProcessManager = new CProcessManager(ID, name);
		}
		AddProcessManager(pProcessManager);
	}
	
	return pProcessManager;
}

void CAnimationStepManager::AddProcessManager( CProcessManager* pProcessManager )
{
	if(NULL != pProcessManager)
	{
		vlist_add_last(m_ProcessManagerList, pProcessManager);
		m_CurProcessManagerID = pProcessManager->GetID();
		pProcessManager->SetAnimationStepManager(this);
	}
}

void CAnimationStepManager::DeleteProcessManager( CProcessManager* pProcessManager )
{
	if(NULL != pProcessManager)
	{
		if(pProcessManager->GetID() == m_CurProcessManagerID)
		{
			CProcessManager* pNewCurProcessManager = NULL;
			if((pNewCurProcessManager = GetNextProcessManager(pProcessManager)))
			{
				m_CurProcessManagerID = pNewCurProcessManager->GetID();
			}
			else if((pNewCurProcessManager = GetPreProcessManager(pProcessManager)))
			{
				m_CurProcessManagerID = pNewCurProcessManager->GetID();
			}
			else
			{
				m_CurProcessManagerID = -1;
			}

			//删除过程管理器后，设置当前活动的动画管理器与当前过程一致
			if(GetSimulationAnimationManager())
			{
				if(pNewCurProcessManager  &&
					pNewCurProcessManager->GetCurrentProcess())
				{
					GetSimulationAnimationManager()->SetCurSAByID(pNewCurProcessManager->GetCurrentProcess()->GetBehaviorActionID());
				}
			}
		}
		vlist_remove(m_ProcessManagerList, pProcessManager);
		delete pProcessManager;
	}
}

void CAnimationStepManager::DeleteAllProcessManager()
{
	START_LIST_ITERATION(CProcessManager, m_ProcessManagerList)
		delete temp;
	END_LIST_ITERATION(m_ProcessManagerList)

	vlist_flush(m_ProcessManagerList);
	m_CurProcessManagerID = -1;
}

CProcessManager * CAnimationStepManager::FindProcessManagerByID( const int ID )
{
	START_LIST_ITERATION(CProcessManager, m_ProcessManagerList)
		if(ID == temp->GetID())
		{
			//m_CurProcessManagerID = ID;
			return temp;
		}
		END_LIST_ITERATION(m_ProcessManagerList)
			return NULL;
}

CProcessManager * CAnimationStepManager::FindProcessManagerByName(const char *name)
{
	START_LIST_ITERATION(CProcessManager, m_ProcessManagerList)
		if (temp->GetName() && strcmp(temp->GetName(), name) == 0)
			return temp;							
	END_LIST_ITERATION(m_ProcessManagerList)
		return 0;
}

void CAnimationStepManager::SetCurProcessManagerByID(int ID,bool bUpdateModel/* = true*/, bool bUpdateCam/* = false*/, bool bCamAni/* = false*/)
{
	if(m_CurProcessManagerID != ID)
	{
		m_CurProcessManagerID = ID;
		if( SATools::VersionCompare("2.2",GetSimulationAnimationManager()->GetVersion()) != 0)
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
			if( bUpdateModel)
			{
				int idx = GetProcessManagerIdxByID(ID);
				UpdateViewWithProcessManager(idx);
			}
		}
	}
	if(m_pSA && GetCurrentProcess())
	{
		m_pSA->SetCurSAByID(GetCurrentProcess()->GetBehaviorActionID());
	}
}

void CAnimationStepManager::SetCurProcessManagerByIdx(int idx,bool bUpdateModel/* = true*/, bool bUpdateCam/* = false*/, bool bCamAni/* = false*/)
{
	CProcessManager* pProcessManager = GetProcessManagerByIdx(idx);
	if(pProcessManager)
	{
		if(m_CurProcessManagerID != pProcessManager->GetID())
		{
			m_CurProcessManagerID = pProcessManager->GetID();
			
			if( SATools::VersionCompare("2.2",GetSimulationAnimationManager()->GetVersion()) != 0)
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
				if( bUpdateModel)
				{
					UpdateViewWithProcessManager(idx);
				}
			}
		}
	}

	if(m_pSA && GetCurrentProcess())
	{
		m_pSA->SetCurSAByID(GetCurrentProcess()->GetBehaviorActionID());
	}
}

void CAnimationStepManager::UpdateViewWithProcessManager(int nProcessManagerIdx)
{
	CSimulationAnimationManager* pSAManager = GetSimulationAnimationManager();

	//根据过程更新视口时，暂屏蔽播放场景功能，防止场景动画与过程的场景冲突
	bool bPlayCamera = pSAManager->IsCameraPlay();
	pSAManager->SetCameraPlay(false);

	if(!pSAManager->IsPlaying())
	{
		//设置视口的当前状态，要考虑之前和之后过程的影响
		//当前过程之后的过程对应动画播放到最后

		for(int i = 0; i< nProcessManagerIdx; i++)
		{
			CProcessManager* pTempProcessManager = GetProcessManagerByIdx(i);
			if(pTempProcessManager)
			{
				for(int j = 0; j< pTempProcessManager->GetProcessCount(); j++)
				{
					CProcess* pTempProcess = pTempProcessManager->GetProcessByIdx(j);
					if(pTempProcess)
					{
						CSBehaviorAction* pBehaviorAction = pTempProcess->GetBehaviorAction();
						if(pBehaviorAction)
						{
							pBehaviorAction->RewindReverse();
						}
					}
				}
			}
		}

		//当前过程之后的过程对应动画复位
		for(int i = GetProcessManagerCount()-1; i> nProcessManagerIdx; i--)
		{
			CProcessManager* pTempProcessManager = GetProcessManagerByIdx(i);
			if(pTempProcessManager)
			{
				for(int j = pTempProcessManager->GetProcessCount()-1; j>=0; j--)
				{
					CProcess* pTempProcess = pTempProcessManager->GetProcessByIdx(j);
					if(pTempProcess)
					{
						CSBehaviorAction* pBehaviorAction = pTempProcess->GetBehaviorAction();
						if(pBehaviorAction)
						{
							pBehaviorAction->Rewind();
						}
					}
				}
			}
		}

		//如果当前处于循环播放时一次的结束状态，根据是否倒播设置当前过程的状态
		CProcessManager* pTempProcessManager = GetProcessManagerByIdx(nProcessManagerIdx);
		if(pTempProcessManager)
		{
			int curProcessIdx = pTempProcessManager->GetProcessIdxByID(pTempProcessManager->GetCurProcessID());
			pTempProcessManager->UpdateViewWithProcess(curProcessIdx);
		}
	}
	pSAManager->SetCameraPlay(bPlayCamera);
}

CProcessManager* CAnimationStepManager::GetProcessManagerByIdx(int idx)
{
	CProcessManager* pProcess = 0;
	if(vlist_count(m_ProcessManagerList) >0)
	{
		pProcess = (CProcessManager*)vlist_nth_item(m_ProcessManagerList,idx);
	}
	return pProcess;
}

int CAnimationStepManager::GetProcessManagerIdxByID(const int ID)
{
	if(FindProcessManagerByID(ID))
	{
		return m_ProcessManagerList->cursor_index;
	}
	else
	{
		return -1;
	}
}

void * CAnimationStepManager::XMLCallback( CUtilityXMLTag *xt, bool open, void *m_pExtraData )
{
	CSimulationAnimationManager * pSAManager = (CSimulationAnimationManager*)xt->GetXMLParser()->GetStackData();
	CAnimationStepManager *pAnimationStepManager = 0;
	/*char*	strSABuffer = NULL;*/
	if(open)
	{
		if(NULL == pSAManager)
		{
			CSimulationAnimationManager *ppSAManager = (CSimulationAnimationManager*) m_pExtraData;
			if(typeid(*ppSAManager) != typeid(CSimulationAnimationManager))
			{
				pAnimationStepManager = new CAnimationStepManager();
				*(CAnimationStepManager**) m_pExtraData = pAnimationStepManager;
			}

		}
		else
		{
			pAnimationStepManager = pSAManager->CreateAnimationStepManager();
		}
		if(pAnimationStepManager)
		{
			if (!xt->GetProperty("Name", pAnimationStepManager->m_Name))
				strcpy(pAnimationStepManager->m_Name, "DefaultName");
			if (!xt->GetProperty("CurProcessManagerID", pAnimationStepManager->m_CurProcessManagerID))
				pAnimationStepManager->m_CurProcessManagerID = 0;
		}

		return pAnimationStepManager;
	}
	else
	{
		int nCurProcessManagerID = 0;
		pAnimationStepManager = (CAnimationStepManager*)xt->GetXMLParser()->GetStackData();
		if (!xt->GetProperty("CurProcessManagerID", nCurProcessManagerID))
			nCurProcessManagerID = 0;

		if(pAnimationStepManager)
		{
			pAnimationStepManager->SetCurProcessManagerByID(nCurProcessManagerID,false);
		}

	}
	return pAnimationStepManager;
}

CProcessManager* CAnimationStepManager::GetCurrentProcessManager()
{
	if(-1 != m_CurProcessManagerID)
	{
		return FindProcessManagerByID(m_CurProcessManagerID);
	}
	return NULL;
}

CProcess* CAnimationStepManager::GetCurrentProcess()
{
	if(-1 != m_CurProcessManagerID)
	{
		CProcessManager* pCurProcessManager = FindProcessManagerByID(m_CurProcessManagerID);
		if(pCurProcessManager)
		{
			return pCurProcessManager->GetCurrentProcess();
		}
	}
	return NULL;
}

int CAnimationStepManager::GetMaxProcessManagerID()
{
	int iMaxID = 0;
	int itmpID = 0;
	START_LIST_ITERATION(CProcessManager, m_ProcessManagerList)
		itmpID = temp->GetID();
	if(itmpID > iMaxID)
		iMaxID = itmpID;
	END_LIST_ITERATION(m_ProcessManagerList)
		return iMaxID;
}

long CAnimationStepManager::RegisterProcessManagerID()
{
	long iRet = 0;
	START_LIST_ITERATION(CProcessManager, m_ProcessManagerList)
		if(temp->GetID() >= iRet)
		{
			iRet = temp->GetID()+1;
		}
		END_LIST_ITERATION(m_ProcessManagerList)
			return iRet;
}

void CAnimationStepManager::StartListProcessManagerQuery()
{
	vlist_reset_cursor(m_ProcessManagerList);
}

bool CAnimationStepManager::GetNextListProcessManager(CProcessManager** opProcessManager)
{
	void *temp;
	if (!(temp = vlist_peek_cursor(m_ProcessManagerList)))
		return false;
	*opProcessManager = (CProcessManager *)temp;
	vlist_advance_cursor(m_ProcessManagerList);	
	return true;
}

bool CAnimationStepManager::HasListProcessManager()
{
	vlist_reset_cursor(m_ProcessManagerList);
	if (!vlist_peek_cursor(m_ProcessManagerList))
		return false;
	else 
		return true;
}
int CAnimationStepManager::GetProcessManagerCount() 
{ 
	return m_ProcessManagerList->count; 
}



bool CAnimationStepManager::MoveProcessManager(const int oldIdx,const int newIdx)
{
	return vlist_move_item(m_ProcessManagerList,oldIdx,newIdx);
}

bool CAnimationStepManager::MoveProcessManager(CProcessManager* pProcessManager,CProcessManager* pTargetProcessManager)
{
	int oldIdx = GetProcessManagerIdxByID(pProcessManager->GetID());
	int newIdx = GetProcessManagerIdxByID(pTargetProcessManager->GetID());
	return vlist_move_item(m_ProcessManagerList,oldIdx,newIdx);
}

CProcessManager* CAnimationStepManager::GetPreProcessManager(CProcessManager* pProcessManager)
{
	CProcessManager* pPreProcessManager = NULL;
	int curProcessManagerIdx = GetProcessManagerIdxByID(pProcessManager->GetID());
	if(curProcessManagerIdx >0)
	{
		pPreProcessManager = GetProcessManagerByIdx(curProcessManagerIdx-1);
	}
	return pPreProcessManager;
}

CProcessManager* CAnimationStepManager::GetNextProcessManager(CProcessManager* pProcessManager)
{
	CProcessManager* pNextProcessManager = NULL;
	int curProcessManagerIdx = GetProcessManagerIdxByID(pProcessManager->GetID());
	if(curProcessManagerIdx < GetProcessManagerCount()-1)
	{
		pNextProcessManager = GetProcessManagerByIdx(curProcessManagerIdx+1);
	}
	return pNextProcessManager;
}

CProcess* CAnimationStepManager::GetPreProcess(CProcess* pProcess)
{
	CProcess* pPreProcess = NULL;
	
	CProcessManager* pProcessManager = GetCurrentProcessManager();
	if(pProcessManager)
	{
		//从当前活动过程管理器中查找上一个
		if(pProcess)
		{
			pPreProcess = pProcessManager->GetPreProcess(pProcess);
		}

		//如果当前过程管理器中找不到，查找前面过程管理器
		while(!pPreProcess && (pProcessManager = GetPreProcessManager(pProcessManager)))
		{
			if(pProcessManager->GetProcessCount()>0)
			{
				pPreProcess = pProcessManager->GetProcessByIdx(pProcessManager->GetProcessCount() - 1);
			}
		}
	}
	
	return pPreProcess;
}

CProcess* CAnimationStepManager::GetNextProcess(CProcess* pProcess)
{
	CProcess* pNextProcess = NULL;

	CProcessManager* pProcessManager = GetCurrentProcessManager();
	if(pProcessManager)
	{
		//从当前活动过程管理器中查找上一个
		if(pProcess)
		{
			pNextProcess = pProcessManager->GetNextProcess(pProcess);
		}

		//如果当前过程管理器中找不到，查找前面过程管理器
		while(!pNextProcess && (pProcessManager = GetNextProcessManager(pProcessManager)))
		{
			if(pProcessManager->GetProcessCount()>0)
			{
				pNextProcess = pProcessManager->GetProcessByIdx(0);
			}
		}
	}

	return pNextProcess;
}

bool CAnimationStepManager::IsAtPlayFirst() 
{
	bool bRet = false;

	CProcessManager* pCurProcessManager = GetCurrentProcessManager();
	CProcess* pCurProcess = GetCurrentProcess();
	if(!pCurProcessManager || !pCurProcess)
	{
		return false;
	}
	CSBehaviorAction* pCurBehaviorAction = pCurProcess->GetBehaviorAction();
	if(!pCurBehaviorAction)
	{
		return false;
	}
	pCurBehaviorAction->ScheduleAllAnimations(true);
	if(pCurBehaviorAction->GetCurrentTick() <= pCurBehaviorAction->GetFirstTick())
	{
		bRet = true;
	}
	else
	{
		bRet = false;
	}

	if(bRet && 
		GetPlayMode() != PLAY_MODE_PROCESS )
	{
		if(!pCurProcessManager->GetPreProcess(pCurProcess))
		{
			bRet = true;
		}
		else
		{
			bRet = false;
		}
	}
	
	if(bRet && 
		(GetPlayMode() == PLAY_MODE_FROM_CURPROCESSMANAGER || GetPlayMode() == PLAY_MODE_ALL))
	{
		if(!GetPreProcessManager(pCurProcessManager))
		{
			bRet = true;
		}
		else
		{
			bRet = false;
		}
	}
	return bRet;
}

bool CAnimationStepManager::IsAtPlayEnd() 
{
	bool bRet = false;

	CProcessManager* pCurProcessManager = GetCurrentProcessManager();
	CProcess* pCurProcess = GetCurrentProcess();
	if(!pCurProcessManager || !pCurProcess)
	{
		return false;
	}
	CSBehaviorAction* pCurBehaviorAction = pCurProcess->GetBehaviorAction();
	if(!pCurBehaviorAction)
	{
		return false;
	}
	pCurBehaviorAction->ScheduleAllAnimations(true);

	if(pCurBehaviorAction->GetCurrentTick() >= pCurBehaviorAction->GetLastTick())
	{
		bRet = true;
	}
	else
	{
		bRet = false;
	}

	if(bRet && 
		GetPlayMode() != PLAY_MODE_PROCESS )
	{
		if(!pCurProcessManager->GetNextProcess(pCurProcess))
		{
			bRet = true;
		}
		else
		{
			bRet = false;
		}
	}

	if(bRet && 
		(GetPlayMode() == PLAY_MODE_FROM_CURPROCESSMANAGER || GetPlayMode() == PLAY_MODE_ALL))
	{
		if(!GetNextProcessManager(pCurProcessManager))
		{
			bRet = true;
		}
		else
		{
			bRet = false;
		}
	}
	return bRet;
}

void CAnimationStepManager::Rewind(AnimationPlayMode playMode,bool bReverse) 
{ 
	if(IsPlaying())
	{
		Stop();
	}
	m_pSA->SetReversePlay(bReverse);
	if(playMode == PLAY_MODE_ALL)
	{
		if(m_pSA->GetReversePlay())
		{
			//将第最后一个过程及过程管理器设置为当前状态，以便从头播放所有动画
			CProcessManager* pProcessManager = GetCurrentProcessManager();
			while(pProcessManager)
			{
				pProcessManager->SetCurProcessByIdx(pProcessManager->GetProcessCount()-1);
				pProcessManager = GetNextProcessManager(pProcessManager);
			}
			SetCurProcessManagerByIdx(GetProcessManagerCount()-1);
		}
		else
		{
			//将第一个过程及过程管理器设置为当前状态，以便从头播放所有动画
			CProcessManager* pProcessManager = GetCurrentProcessManager();
			while(pProcessManager)
			{
				pProcessManager->SetCurProcessByIdx(0);
				pProcessManager = GetPreProcessManager(pProcessManager);
			}
			SetCurProcessManagerByIdx(0);
		}
	}
	else if(playMode == PLAY_MODE_PROCESSMANAGER || 
		playMode == PLAY_MODE_FROM_CURPROCESSMANAGER || 
		playMode == PLAY_MODE_FROM_CURPROCESS)
	{
		//将第一个过程设置为当前状态，以便从头播放当前过程管理器中的动画
		CProcessManager* pCurrentProcessManager = GetCurrentProcessManager();
		if(pCurrentProcessManager)
		{
			if(bReverse)
			{
				pCurrentProcessManager->SetCurProcessByIdx(pCurrentProcessManager->GetProcessCount()-1);
			}
			else
			{
				pCurrentProcessManager->SetCurProcessByIdx(0);
			}
		}
	}
	else if(playMode == PLAY_MODE_PROCESS)
	{
		CProcessManager* pCurrentProcessManager = GetCurrentProcessManager();
		CProcess* pCurrentProcess = 0;
		CSBehaviorAction* pCurrentBehaviorAction = 0;
		if(pCurrentProcessManager)
		{
			pCurrentProcess = pCurrentProcessManager->GetCurrentProcess();
			if(pCurrentProcess ) 
			{
				pCurrentBehaviorAction = pCurrentProcess->GetBehaviorAction();
			}
		}

		if(pCurrentBehaviorAction)
		{
			if(bReverse)
			{
				pCurrentBehaviorAction->RewindReverse();
			}
			else
			{
                if(pCurrentProcessManager){
                    pCurrentProcessManager->SetCurProcessByID(0);
                }
				pCurrentBehaviorAction->Rewind();
			}
		}
	}

	CProcess* pCurProcess = GetCurrentProcess();
	if(pCurProcess)
	{
		pCurProcess->UpdateView(true, m_pSA->IsCameraPlay());
	}
}

void CAnimationStepManager::Play(AnimationPlayMode playMode,bool bReverse,bool bChangeProcess) 
{ 
	if (!m_pSA || ! m_pSA->HasAnimations())
	{
		return;
	}
	if(IsPlaying())
	{
		Stop();
	}
	m_pSA->SetReversePlay(bReverse);
	m_PlayMode = playMode;

	if(m_bIsPause)
	{
		CSBehaviorAction* pBehaviorAction = m_pSA->GetCurrentSA();
		if(pBehaviorAction)
		{
			if(bReverse)
			{
				pBehaviorAction->ContinueReverse();
			}
			else
			{
				pBehaviorAction->Continue();
			}
		}
		m_bIsPause = false;
	}
	else
	{
		bool bPlayEnded = false;
		if(bReverse)
		{
			bPlayEnded = IsAtPlayFirst();
		}
		else
		{
			bPlayEnded = IsAtPlayEnd();
		}
		if(bPlayEnded && !bChangeProcess)
		{
			Rewind(playMode,bReverse);
			m_pSA->SetReversePlay(bReverse);
			m_PlayMode = playMode;
		}
		else
		{
			if(m_PlayMode == PLAY_MODE_ALL)
			{
				if(m_pSA->GetReversePlay())
				{
					//将第最后一个过程及过程管理器设置为当前状态，以便从头播放所有动画
					CProcessManager* pProcessManager = GetCurrentProcessManager();
					while(pProcessManager)
					{
						pProcessManager->SetCurProcessByIdx(pProcessManager->GetProcessCount()-1);
						pProcessManager = GetNextProcessManager(pProcessManager);
					}
					SetCurProcessManagerByIdx(GetProcessManagerCount()-1);
				}
				else
				{
					//将第一个过程及过程管理器设置为当前状态，以便从头播放所有动画
					CProcessManager* pProcessManager = GetCurrentProcessManager();
					while(pProcessManager)
					{
						pProcessManager->SetCurProcessByIdx(0);
						pProcessManager = GetPreProcessManager(pProcessManager);
					}
					SetCurProcessManagerByIdx(0);
				}
			}
			else if(m_PlayMode == PLAY_MODE_PROCESSMANAGER || 
				m_PlayMode == PLAY_MODE_FROM_CURPROCESSMANAGER)
			{
				//将第一个过程设置为当前状态，以便从头播放当前过程管理器中的动画
				CProcessManager* pCurrentProcessManager = GetCurrentProcessManager();
				if(pCurrentProcessManager)
				{
					if(m_pSA->GetReversePlay())
					{
						pCurrentProcessManager->SetCurProcessByIdx(pCurrentProcessManager->GetProcessCount()-1);
					}
					else
					{
						pCurrentProcessManager->SetCurProcessByIdx(0);
					}
				}
			}
		}

		CProcessManager* pCurrentProcessManager = GetCurrentProcessManager();
		CProcess* pCurrentProcess = 0;
		CSBehaviorAction* pCurrentBehaviorAction = 0;
		if(pCurrentProcessManager)
		{
			pCurrentProcess = pCurrentProcessManager->GetCurrentProcess();

			//解决当前过程为空时无法播放的问题。目前具体问题是在播放全部和从当前过程管理器开始播放时，播放到无过程的过程管理器时，自动切换到下一过程管理器
			if(!pCurrentProcess)
			{
				if(m_PlayMode == PLAY_MODE_PROCESSMANAGER || 
					m_PlayMode == PLAY_MODE_FROM_CURPROCESSMANAGER)
				{
					if(m_pSA->GetReversePlay())
					{
						pCurrentProcess = GetPreProcess(pCurrentProcess);
					}
					else
					{
						pCurrentProcess = GetNextProcess(pCurrentProcess);
					}
				}
				if(pCurrentProcess ) 
				{
					pCurrentProcessManager = pCurrentProcess->GetProcessManager();
					pCurrentProcessManager->SetCurProcessByID(pCurrentProcess->GetID(),false);
				}
			}

			if(pCurrentProcess ) 
			{
				pCurrentBehaviorAction = pCurrentProcess->GetBehaviorAction();
			}
		}

		if(pCurrentBehaviorAction)
		{
			pCurrentProcess->UpdateView(true,false,false);
			if(!StartChangeCamera(pCurrentProcess,true))
			{
				if(m_pSA->GetReversePlay())
				{
					pCurrentBehaviorAction->ContinueReverse();
				}
				else
				{
					pCurrentBehaviorAction->Continue();
				}
			}
		}
		else
		{
			m_PlayMode = PLAY_MODE_NONE;
		}
	}
}

bool CAnimationStepManager::IsPlaying() 
{ 
	bool bRet = false;
	if(m_PlayMode != PLAY_MODE_NONE && !m_bIsPause)
	{
		 bRet = true;
	}
	return bRet;
}

void CAnimationStepManager::Stop() 
{ 
	m_PlayMode = PLAY_MODE_NONE;
	if(m_pSA->IsPlaying())
	{
		m_pSA->StopAll();
    }
	m_pSA->SetReversePlay(false);
	//清除场景切换动画
	EndChangeCamera();
}

void CAnimationStepManager::Pause()
{
	m_bIsPause = true;
	if(m_pSA->IsPlaying())
	{
		CSBehaviorAction* pBehaviorAction = m_pSA->GetCurrentSA();
		if(pBehaviorAction)
		{
			pBehaviorAction->Stop();
		}
	}
}

void CAnimationStepManager::PlayFinishCB(CSBehaviorAction* pBehaviorAction)
{
	if (m_PlayMode == PLAY_MODE_RANDOM)
		return;
	CProcessManager* pCurProcessManager = GetCurrentProcessManager();
	if(pCurProcessManager)
	{
		CProcess* pCurProcess = pCurProcessManager->GetCurrentProcess();
		if(pCurProcess)
		{
			if(pBehaviorAction == GetBehaviorActionChgCam())
			{
				//如果是过程间的场景切换临时动画结束，播放过程的实际动画
				if(pCurProcess && pCurProcess->GetBehaviorAction() && m_PlayMode != PLAY_MODE_NONE)
				{
					CSBehaviorAction* pCurBehaviorAction = pCurProcess->GetBehaviorAction();
					if(pCurBehaviorAction)
					{
						if(m_pSA->GetReversePlay())
						{
							//pCurBehaviorAction->RewindReverse();
							pCurBehaviorAction->ContinueReverse();
						}
						else
						{
							//pCurBehaviorAction->Rewind();
							pCurBehaviorAction->Continue();
						}
						m_bIsPause = false;
					}
				}
				else
				{
					//清除场景切换动画
					EndChangeCamera();
				}
			}
			else
			{
				//清除场景切换动画
				EndChangeCamera();
				
				if(m_PlayMode == PLAY_MODE_FROM_CURPROCESS ||
					m_PlayMode == PLAY_MODE_PROCESSMANAGER ||
					m_PlayMode == PLAY_MODE_FROM_CURPROCESSMANAGER ||
					m_PlayMode == PLAY_MODE_ALL)
				{
					//获取要自动播放的下一个过程_START
					if(m_pSA->GetReversePlay())
					{
						pCurProcess = pCurProcessManager->GetPreProcess(pCurProcess);
					}
					else
					{
						pCurProcess = pCurProcessManager->GetNextProcess(pCurProcess);
					}

					if(pCurProcess)
					{
						pCurProcessManager->SetCurProcessByID(pCurProcess->GetID());
					}
					else
					{
						pCurProcess = NULL;
						if(m_PlayMode == PLAY_MODE_FROM_CURPROCESSMANAGER ||
							m_PlayMode == PLAY_MODE_ALL)
						{
							if(m_pSA->GetReversePlay())
							{
								pCurProcessManager = GetPreProcessManager(pCurProcessManager);
							}
							else
							{
								pCurProcessManager = GetNextProcessManager(pCurProcessManager);
							}
							if(pCurProcessManager)
							{
								if(m_pSA->GetReversePlay())
								{
									pCurProcessManager->SetCurProcessByIdx(pCurProcessManager->GetProcessCount()-1);
								}
								else
								{
									pCurProcessManager->SetCurProcessByIdx(0);
								}
								
								pCurProcess = pCurProcessManager->GetCurrentProcess();
							}
							else
							{
								pCurProcessManager = NULL;
							}

							//解决当前过程为空时无法播放的问题。目前具体问题是在播放全部和从当前过程管理器开始播放时，播放到无过程的过程管理器时，自动切换到下一过程管理器
							if(!pCurProcess)
							{
								if(m_pSA->GetReversePlay())
								{
									pCurProcess = GetPreProcess(pCurProcess);
								}
								else
								{
									pCurProcess = GetNextProcess(pCurProcess);
								}
								if(pCurProcess)
								{
									pCurProcessManager = pCurProcess->GetProcessManager();
									pCurProcessManager->SetCurProcessByID(pCurProcess->GetID());
								}
							}
						}
					}
					//获取要自动播放的下一个过程_END

					if(pCurProcess)
					{
						//如果找到下一个过程，开始切换场景，如果场景切换失败，播放当前活动过程的实际动画
						if(!StartChangeCamera(pCurProcess,true))
						{
							CSBehaviorAction* pCurBehaviorAction = pCurProcess->GetBehaviorAction();
							if(pCurBehaviorAction)
							{
								if(m_pSA->GetReversePlay())
								{
									pCurBehaviorAction->RewindReverse();
									pCurBehaviorAction->ContinueReverse();
								}
								else
								{
									pCurBehaviorAction->Rewind();
									pCurBehaviorAction->Continue();
								}
								m_bIsPause = false;
							}
						}
					}
					else
					{
						//如果没有找到下一个过程，播放结束或循环播放
						if(m_pSA->GetContinuousPlay())
						{
							if(m_PlayMode == PLAY_MODE_FROM_CURPROCESS ||
								m_PlayMode == PLAY_MODE_PROCESSMANAGER)
							{
								Play(PLAY_MODE_PROCESSMANAGER,m_pSA->GetReversePlay());
							}
							else
							{
								Play(PLAY_MODE_ALL,m_pSA->GetReversePlay());
							}
						}
						else
						{
							Stop();
						}
					}
				}
				else
				{
					//播放结束或循环播放
					if(m_pSA->GetContinuousPlay())
					{
						Play(m_PlayMode,m_pSA->GetReversePlay());
					}
					else
					{
						Stop();
					}
				}
			}
		}
	}
}


bool CAnimationStepManager::StartChangeCamera(CProcess* pCurProcess,bool bAnimation)
{
	bool bRet = false;
	if(!pCurProcess)
	{
		return bRet;
	}
	if (!m_pSA->IsCameraPlay())
	{
		return bRet;
	}
	
	int nBehaviorActionId  = m_pSA->RegisterBehaviorActionID();
	if(m_pBehaviorActionChgCam)
	{
		m_pBehaviorActionChgCam->DeleteAllAnimations();
	}
	else
	{
		m_pBehaviorActionChgCam = m_pSA->AddSimAni(nBehaviorActionId);
	}

	//录制视口场景
	CHAR strPlcID[SA_BUFFER_SIZE_SMALL];
	CHAR strObjName[SA_BUFFER_SIZE_SMALL];
	CSAnimation *animation = NULL;
	AniPoint pntCenterPnt(0.0,0.0,0.0);

	strcpy(strPlcID, TARGETOBJECTTYPE_CAM);
	strcat(strPlcID, ":SCENE/TARGET");
	strcpy(strObjName, "Camera");

	animation = CSACommonAPI::AddAnimation(m_pBehaviorActionChgCam,strObjName,strPlcID,&pntCenterPnt,NULL);

	if(animation == NULL)
	{
		return false;
	}

	

	//获取当前场景状态
	AnimationPlayApi* pAnimationPlayApi = m_pSA->GetAnimationPlayApi();
	if(pAnimationPlayApi)
	{
		
		AniPoint curPos;
		AniQuat curRotation;
		AniPoint curScale;
		AniPoint newPos;
		AniQuat newRotation;
		AniPoint newScale;

		//如果过程设置了场景，将过程的场景作为结束帧，否则使用初始场景
		//CProcessTargetObject * pTargetObject = pCurProcess->GetCameraTargetObject(false);
		bool bFindCam = pCurProcess->GetCamera(newPos,newRotation,newScale);
		if(!bFindCam)
		{
			strcpy(strPlcID, TARGETOBJECTTYPE_CAM);
			strcat(strPlcID, ":SCENE/TARGET");
			CProcessTargetObject * pTargetObject = m_pSA->FindInitTargetObjectByPath(strPlcID);
			if(pTargetObject)
			{
				newPos = pTargetObject->GetPos();
				newRotation = pTargetObject->GetQuat();
				newScale = pTargetObject->GetScale();
				bFindCam = true;
			}
		}
		
		if(bFindCam)
		{
			//将当前场景的状态作为开始帧
			float fCurScale[3] = {1.0};
			float fCurMtxAA[4][4];
			float fCurTarget[3] = {0};
			pAnimationPlayApi->getCamera(strPlcID,fCurScale, fCurMtxAA, m_pSA->GetView());
			pAnimationPlayApi->getCameraTargetPnt(strPlcID,fCurTarget,m_pSA->GetView());

			fCurMtxAA[3][0] = fCurTarget[0];
			fCurMtxAA[3][1] = fCurTarget[1];
			fCurMtxAA[3][2] = fCurTarget[2];

			float fCurMtxAAInv[4][4];
			MatrixOperation::MatrixCopy((float*)fCurMtxAA,(float*)fCurMtxAAInv,16);
			MatrixOperation::MatrixInversion((float*)fCurMtxAAInv);

			curPos.Set(fCurMtxAAInv[3][0],fCurMtxAAInv[3][1],fCurMtxAAInv[3][2]);
			curRotation = AniQuat::MatrixToQuaternion((float*)fCurMtxAAInv);
			curScale.Set(fCurScale[0],fCurScale[1],fCurScale[2]);
			CSACommonAPI::AddCameraKeyframe(m_pBehaviorActionChgCam,animation,0,AniPoint(0.0f,0.0f,0.0f),curPos,curRotation,curScale,true);


			//解决场景切换时，缩放效果不正确的问题_START
			float fScale = newScale.x;
			if((int)newScale.y > 0 && (int)newScale.z > 0)
			{
				float dx = newScale.y/fCurScale[1];
				float dy = newScale.z/fCurScale[2];
				if(dx > dy)
				{
					fScale = fScale/dx;
				}
				else
				{
					fScale = fScale/dy;
				}
				//fScale = scale[0]/dy;
			}
			newScale.Set(fScale,fCurScale[1],fCurScale[2]);
			//解决场景切换时，缩放效果不正确的问题_END


			float fMtxAA[4][4];
			float fMtxAAInv[4][4];
			newRotation.ToMatrix((float*)fMtxAA);
			fMtxAA[3][0] = newPos.x;
			fMtxAA[3][1] = newPos.y;
			fMtxAA[3][2] = newPos.z;
			MatrixOperation::MatrixCopy((float*)fMtxAA,(float*)fMtxAAInv,16);
			MatrixOperation::MatrixInversion((float*)fMtxAAInv);

			newPos.Set(fMtxAAInv[3][0],fMtxAAInv[3][1],fMtxAAInv[3][2]);
			newRotation = AniQuat::MatrixToQuaternion((float*)fMtxAAInv);
			CSACommonAPI::AddCameraKeyframe(m_pBehaviorActionChgCam,animation,m_nChgCamTickNum,AniPoint(0.0f,0.0f,0.0f),newPos,newRotation,newScale,true);

			if(bAnimation)
			{
				m_pBehaviorActionChgCam->SetCurrentTick(0);
				m_pBehaviorActionChgCam->Continue();
				bRet = true;
				m_bIsPause = false;
			}
			else
			{
				m_pBehaviorActionChgCam->RewindReverse();
				EndChangeCamera();
				bRet = false;
			}
		}
		else
		{
			EndChangeCamera();
			bRet = false;
		}
	}
	return bRet;
}
void CAnimationStepManager::EndChangeCamera()
{
	if(m_pBehaviorActionChgCam)
	{
		CSimulationAnimationManager* pSAManager = m_pBehaviorActionChgCam->GetSimulationAnimationManager();
		if(pSAManager)
		{
			//pSAManager->DeleteSimAni(m_pBehaviorActionChgCam);

			//m_pBehaviorActionChgCam->RewindReverse();//解决过程场景切换连续被中止有时显示不正确的问题
			m_pBehaviorActionChgCam->DeleteAllAnimations();
			if(GetCurrentProcess() &&
				GetCurrentProcess()->GetBehaviorAction())
			{
				pSAManager->SetCurSAByID(GetCurrentProcess()->GetBehaviorAction()->GetID());
			}
		}
		//m_pBehaviorActionChgCam = NULL;
	}
}

void CAnimationStepManager::SetChgCamTime(float fSecond/* = 1.0f*/)
{
	m_nChgCamTickNum = fSecond * DEFAULT_TPS;
}

CProcess* CAnimationStepManager::FindProcessByAnimationID(int nAnimationID)
{
	CProcess* pRetProcess = NULL;
	if(nAnimationID < 0)
		return pRetProcess;
	vlist_s* ProMangerList = GetProcessManagerList();
	for (int iProManager = 0;iProManager < (int)vlist_count(ProMangerList);iProManager++)
	{
		CProcessManager* pProcessManager = (CProcessManager*)vlist_nth_item(ProMangerList, iProManager);
		if(!pProcessManager)
			continue;
		if(pRetProcess = pProcessManager->FindProcessByAnimationID(nAnimationID)) 
			break;
	}
	return pRetProcess;
}

bool CAnimationStepManager::IsPauseState()
{
	return m_bIsPause;
}

SA_NAMESPACE_END


