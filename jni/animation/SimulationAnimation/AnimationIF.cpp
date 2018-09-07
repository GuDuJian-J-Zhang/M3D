// All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file		AnimationIF.cpp
 *
 *	@brief	    ����������
 *
 *	@par	��ʷ:
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
	*	@brief	  ��ȡ������������
  	*  @Creator qinyp 2016.02.24
	********************************************************************/
	CSimulationAnimationManager* GetSAManager();
	/*****************************************************************//**
	*	@brief	 ��ȡ�ֲ�����������
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
	//��ȡ������������
	CSimulationAnimationManager* pSAManager = NULL;
	CAnimationAPI* pAniAPI = CAnimationAPI::GetInstance();
	assert(pAniAPI);
	if(!pAniAPI) return pSAManager;
	pSAManager = pAniAPI->GetSAManager();
	return pSAManager;
}

CAnimationStepManager* CAnimationIFP::GetAniStepManager()
{
	//��ȡ�ֲ�����������
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
	//����ָ��ID�Ķ���
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

	//������ŵ�������ʱ����һ�β��ź��ٲ��ŵڶ��Σ��޷����ò����ʼ״̬������
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
	//ֹͣ���Ŷ���
	CAnimationStepManager* pAniStepManager = m_pImpl->GetAniStepManager();
	if(!pAniStepManager) return;
	pAniStepManager->Stop();
}

void CAnimationIF::Continue()
{
	//�������Ŷ���
	CSimulationAnimationManager* pSAManager = m_pImpl->GetSAManager();
	if(!pSAManager) return;
	CAnimationStepManager* pAniStepManager = pSAManager->GetAnimationStepManager();
	if(!pAniStepManager) return;
	pAniStepManager->Play(pAniStepManager->GetPlayMode(), pSAManager->GetReversePlay());
}

void CAnimationIF::Pause()
{
	//��ͣ���Ŷ���
	CAnimationStepManager* pAniStepManager = m_pImpl->GetAniStepManager();
	if(!pAniStepManager) return;
	pAniStepManager->Pause();
}

void CAnimationIF::PlaySingleProcess(int nProManagerID, int nProcessID, bool bReverse, bool bChangeProcess)
{
	//��������ָ��ID�����µ�ָ��ID����Ķ���
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
	// ����ָ��ID�����µ�ȫ������
	if(nProManagerID < 0) return;
	CAnimationStepManager* pAniStepManager = m_pImpl->GetAniStepManager();
	if(!pAniStepManager) return;
	pAniStepManager->SetCurProcessManagerByID(nProManagerID);
	pAniStepManager->Play(CAnimationStepManager::PLAY_MODE_PROCESSMANAGER,
		bReverse, bChangeProcess);
}

void CAnimationIF::PlayAllProcessManager(bool bReverse, bool bChangeProcess)
{
	//����ȫ������
	CAnimationStepManager* pAniStepManager = m_pImpl->GetAniStepManager();
	if(!pAniStepManager) return;
	pAniStepManager->Play(CAnimationStepManager::PLAY_MODE_ALL, bReverse, bChangeProcess);
}

void CAnimationIF::PlayFromCurrentProcess(int nProManagerID, int nProcessID, bool bReverse, bool bChangeProcess/* = false*/)
{
	//��ָ�����̵�ָ�����迪ʼ����
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
	// ��ָ�����̿�ʼ����
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