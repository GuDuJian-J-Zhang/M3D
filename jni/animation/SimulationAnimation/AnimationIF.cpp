// All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file		AnimationIF.cpp
 *
 *	@brief	    任务配置类
 *
 *	@par	历史:
 *		2016/02/24 	created by qinyp
****************************************************************************/
#include "StdAfx.h"
#include "AnimationIF.h"
#include "AnimationAPI.h"
#include "AnimationStepManager.h"
using namespace NS_SimulationAnimation;

class CAnimationIFP
{
public:
	CAnimationIF* publ;
public:
	CAnimationIFP(CAnimationIF* pAniIF);
	~CAnimationIFP();
public:
	/*****************************************************************//**
	*	@brief	  获取动画管理器类
  	*  @Creator qinyp 2016.02.24
	********************************************************************/
	CSimulationAnimationManager* GetSAManager();
	/*****************************************************************//**
	*	@brief	 获取分步动画管理类
  	*  @Creator qinyp 2016.02.24
	********************************************************************/
	CAnimationStepManager* GetAniStepManager();
};

CAnimationIFP::CAnimationIFP(CAnimationIF* pAniIF)
{
	publ = pAniIF;
}

CAnimationIFP::~CAnimationIFP()
{

}

CSimulationAnimationManager* CAnimationIFP::GetSAManager()
{
	//获取动画管理器类
	CSimulationAnimationManager* pSAManager = NULL;
	CAnimationAPI* pAniAPI = CAnimationAPI::GetInstance();
	assert(pAniAPI);
	if(!pAniAPI) return pSAManager;
	pSAManager = pAniAPI->GetSAManager();
	return pSAManager;
}

CAnimationStepManager* CAnimationIFP::GetAniStepManager()
{
	//获取分步动画管理类
	CAnimationStepManager* pAniStepManager = NULL;
	CSimulationAnimationManager* pSAManager = GetSAManager();
	if(!pSAManager) return pAniStepManager;
	pAniStepManager = pSAManager->GetAnimationStepManager();
	return pAniStepManager;
}

CAnimationIF::CAnimationIF(void)
{
	m_pImpl = new CAnimationIFP(this);
}

CAnimationIF::~CAnimationIF(void)
{
	if(m_pImpl)
	{
		delete m_pImpl;
		m_pImpl = NULL;
	}
}

bool CAnimationIF::PlaySingleAnimation(int nAniID, bool bUpdateModel/* = false*/, bool bUpdateCam/* = false*/, bool bCamAni/* = false*/)
{
	//播放指定ID的动画
	bool bPlay = false;
	CSimulationAnimationManager* pSAManager = m_pImpl->GetSAManager();
	if(!pSAManager) return bPlay;
	if(pSAManager->IsPlaying())
		pSAManager->StopAll();
	CSBehaviorAction* pBehaviorAction = NULL;
	pBehaviorAction = pSAManager->FindSimAniByID(nAniID);
	if(!pBehaviorAction) return bPlay;
	CAnimationStepManager* pAnimationStepManager = pSAManager->GetAnimationStepManager();
	if(!pAnimationStepManager)
		return bPlay;
	CProcess* pProcess = pAnimationStepManager->FindProcessByAnimationID(pBehaviorAction->GetID());
	if(!pProcess)
		return bPlay;
	CProcessManager* pProcessManager = pProcess->GetProcessManager();
	if(!pProcessManager)
		return bPlay;
	pAnimationStepManager->SetCurProcessManagerByID(pProcessManager->GetID());

	//解决播放单个动画时，第一次播放后，再播放第二次，无法设置步骤初始状态的问题
	if ((pProcessManager->GetCurProcessID() == pProcess->GetID()) &&
		(bUpdateModel || bUpdateCam || bCamAni))
	{
		CProcess* pCurProcess = pProcessManager->GetCurrentProcess();
		if (pCurProcess)
		{
			pCurProcess->UpdateView(bUpdateModel, bUpdateCam, bCamAni);
		}
	}
	pProcessManager->SetCurProcessByID(pProcess->GetID(), bUpdateModel, bUpdateCam, bCamAni);
	
	pBehaviorAction->Rewind();
	pBehaviorAction->Continue();
	bPlay = true;
	return bPlay;
}

void CAnimationIF::Stop()
{
	//停止播放动画
	CAnimationStepManager* pAniStepManager = m_pImpl->GetAniStepManager();
	if(!pAniStepManager) return;
	pAniStepManager->Stop();
}

void CAnimationIF::Continue()
{
	//继续播放动画
	CSimulationAnimationManager* pSAManager = m_pImpl->GetSAManager();
	if(!pSAManager) return;
	CAnimationStepManager* pAniStepManager = pSAManager->GetAnimationStepManager();
	if(!pAniStepManager) return;
	pAniStepManager->Play(pAniStepManager->GetPlayMode(), pSAManager->GetReversePlay());
}

void CAnimationIF::Pause()
{
	//暂停播放动画
	CAnimationStepManager* pAniStepManager = m_pImpl->GetAniStepManager();
	if(!pAniStepManager) return;
	pAniStepManager->Pause();
}

void CAnimationIF::PlaySingleProcess(int nProManagerID, int nProcessID, bool bReverse, bool bChangeProcess)
{
	//单步播放指定ID过程下的指定ID步骤的动画
	if(nProManagerID < 0 || nProcessID < 0) return;
	CAnimationStepManager* pAniStepManager = m_pImpl->GetAniStepManager();
	if(!pAniStepManager) return;
	CProcessManager* pProManager = pAniStepManager->FindProcessManagerByID(nProManagerID);
	if(!pProManager) return;
	pAniStepManager->SetCurProcessManagerByID(nProManagerID);
	pProManager->SetCurProcessByID(nProcessID);
	pAniStepManager->Play(CAnimationStepManager::PLAY_MODE_PROCESS, bReverse, bChangeProcess);
}

void CAnimationIF::PlaySingleProcessManager(int nProManagerID, bool bReverse, bool bChangeProcess/* = false*/)
{
	// 播放指定ID过程下的全部动画
	if(nProManagerID < 0) return;
	CAnimationStepManager* pAniStepManager = m_pImpl->GetAniStepManager();
	if(!pAniStepManager) return;
	pAniStepManager->SetCurProcessManagerByID(nProManagerID);
	pAniStepManager->Play(CAnimationStepManager::PLAY_MODE_PROCESSMANAGER,
		bReverse, bChangeProcess);
}

void CAnimationIF::PlayAllProcessManager(bool bReverse, bool bChangeProcess)
{
	//播放全部动画
	CAnimationStepManager* pAniStepManager = m_pImpl->GetAniStepManager();
	if(!pAniStepManager) return;
	pAniStepManager->Play(CAnimationStepManager::PLAY_MODE_ALL, bReverse, bChangeProcess);
}

void CAnimationIF::PlayFromCurrentProcess(int nProManagerID, int nProcessID, bool bReverse, bool bChangeProcess/* = false*/)
{
	//从指定过程的指定步骤开始播放
	if(nProcessID < 0 || nProManagerID < 0) return;
	CAnimationStepManager* pAniStepManager = m_pImpl->GetAniStepManager();
	if(!pAniStepManager) return;
	CProcessManager* pProManager = pAniStepManager->FindProcessManagerByID(nProManagerID);
	if(!pProManager) return;
	pAniStepManager->SetCurProcessManagerByID(nProManagerID);
	pProManager->SetCurProcessByID(nProcessID);
	pAniStepManager->Play(CAnimationStepManager::PLAY_MODE_FROM_CURPROCESS, bReverse, bChangeProcess);
}

void CAnimationIF::PlayFromCurrentProcessManager(int nProManagerID, bool bReserve, bool bChangeProcess/* = false*/)
{
	// 从指定过程开始播放
	if(nProManagerID < 0) return;
	CAnimationStepManager* pAniStepManager = m_pImpl->GetAniStepManager();
	if(!pAniStepManager) return;
	pAniStepManager->SetCurProcessManagerByID(nProManagerID);
	pAniStepManager->Play(CAnimationStepManager::PLAY_MODE_FROM_CURPROCESSMANAGER, bReserve, bChangeProcess);
}

void CAnimationIF::RewindAnimationByID(int nAniID)
{
	CSimulationAnimationManager* pSAManager = m_pImpl->GetSAManager();
	if(!pSAManager) return;
	if(pSAManager->IsPlaying())
		pSAManager->StopAll();
	CSBehaviorAction* pBehaviorAction = NULL;
	pBehaviorAction = pSAManager->FindSimAniByID(nAniID);
	if(!pBehaviorAction) return;
	pBehaviorAction->Rewind();
}