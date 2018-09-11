#include "AnimationCallBack.h"



CAnimationCallBack::CAnimationCallBack()
{
	
}


CAnimationCallBack::~CAnimationCallBack()
{
	m_vecUpdateViewCB.clear();
	m_vecPlayBeginCB.clear();
	m_vecPlayEndCB.clear();
	m_vecGetModelPlcMtxCB.clear();
	m_vecSetModelPlcMtxCB.clear();
	m_vecGetCameraCB.clear();
	m_vecSetCameraCB.clear();
	m_vecGetCameraTargetPntCB.clear();
	m_vecPlayVisibleCB.clear();
	m_vecPlayColorCB.clear();
	m_vecPlayImageCB.clear();
	m_vecPlayClipPlaneCB.clear();
	m_vecSetTargetStateCB.clear();
	m_vecPlayToolCB.clear();
}

void CAnimationCallBack::AddUpdateViewCB(UpdateViewCB pUpdateViewCB, void* pInstance)
{
	std::vector<AnimationDelegate<UpdateViewCB>>::iterator itorCB = m_vecUpdateViewCB.begin();
	while (itorCB != m_vecUpdateViewCB.end())
	{
		if (*itorCB->m_pCbFun == pUpdateViewCB && 
			itorCB->m_pInstance == pInstance)
			return;
		itorCB++;
	}
	m_vecUpdateViewCB.push_back(AnimationDelegate<UpdateViewCB>(pInstance,pUpdateViewCB));
}

void CAnimationCallBack::RemoveUpdateViewCB(UpdateViewCB pUpdateViewCB, void* pInstance)
{
	std::vector<AnimationDelegate<UpdateViewCB>>::iterator itorCB = m_vecUpdateViewCB.begin();
	while (itorCB != m_vecUpdateViewCB.end())
	{
		if (*itorCB->m_pCbFun == pUpdateViewCB &&
			itorCB->m_pInstance == pInstance)
		{
			m_vecUpdateViewCB.erase(itorCB);
			break;
		}
		itorCB++;
	}
}

void CAnimationCallBack::InvokeUpdateViewCB(void* pBehaviorAction)
{
	for (int i = 0; i < (int)m_vecUpdateViewCB.size(); i++)
	{
		if (m_vecUpdateViewCB[i].m_pCbFun)
			m_vecUpdateViewCB[i].m_pCbFun(pBehaviorAction, m_vecUpdateViewCB[i].m_pInstance);
	}
}

void CAnimationCallBack::AddPlayBeginCB(PlayBeginCB pPlayBeginCB, void* pInstance)
{
	std::vector<AnimationDelegate<PlayBeginCB>>::iterator itorCB = m_vecPlayBeginCB.begin();
	while (itorCB != m_vecPlayBeginCB.end())
	{
		if (*itorCB->m_pCbFun == pPlayBeginCB &&
			itorCB->m_pInstance == pInstance)
			return;
		itorCB++;
	}
	m_vecPlayBeginCB.push_back(AnimationDelegate<PlayBeginCB>(pInstance, pPlayBeginCB));
}

void CAnimationCallBack::RemovePlayBeginCB(PlayBeginCB pPlayBeginCB, void* pInstance)
{
	std::vector<AnimationDelegate<PlayBeginCB>>::iterator itorCB = m_vecPlayBeginCB.begin();
	while (itorCB != m_vecPlayBeginCB.end())
	{
		if (*itorCB->m_pCbFun == pPlayBeginCB &&
			itorCB->m_pInstance == pInstance)
		{
			m_vecPlayBeginCB.erase(itorCB);
			break;
		}
		itorCB++;
	}
}

void CAnimationCallBack::InvokePlayBeginCB(void* pBehaviorAction)
{
	for (int i = 0; i < (int)m_vecPlayBeginCB.size(); i++)
	{
		if (m_vecPlayBeginCB[i].m_pCbFun)
			m_vecPlayBeginCB[i].m_pCbFun(pBehaviorAction, m_vecPlayBeginCB[i].m_pInstance);
	}
}

void CAnimationCallBack::AddPlayEndCB(PlayEndCB pPlayEndCB, void* pInstance)
{
	std::vector<AnimationDelegate<PlayEndCB>>::iterator itorCB = m_vecPlayEndCB.begin();
	while (itorCB != m_vecPlayEndCB.end())
	{
		if (*itorCB->m_pCbFun == pPlayEndCB &&
			itorCB->m_pInstance == pInstance)
		{
			return;
		}
		itorCB++;
	}
	m_vecPlayEndCB.push_back(AnimationDelegate<PlayEndCB>(pInstance, pPlayEndCB));
}

void CAnimationCallBack::RemovePlayEndCB(PlayEndCB pPlayEndCB, void* pInstance)
{
 	std::vector<AnimationDelegate<PlayEndCB>>::iterator itorCB = m_vecPlayEndCB.begin();
 	while(itorCB != m_vecPlayEndCB.end())
 	{
 		if (*itorCB->m_pCbFun == pPlayEndCB &&
 			itorCB->m_pInstance == pInstance)
 		{
 			m_vecPlayEndCB.erase(itorCB);
 			break;
 		}
 		itorCB++;
 	}
}

void CAnimationCallBack::InvokePlayEndCB(void* pBehaviorAction)
{
	for(int i = 0; i < (int)m_vecPlayEndCB.size(); i++)
	{
		if (m_vecPlayEndCB[i].m_pCbFun)
			m_vecPlayEndCB[i].m_pCbFun(pBehaviorAction, m_vecPlayEndCB[i].m_pInstance);
	}
}

void CAnimationCallBack::AddLockViewCB(LockViewCB pLockViewCB, void* pInstance)
{
	std::vector<AnimationDelegate<LockViewCB>>::iterator itorCB = m_vecLockViewCB.begin();
	while (itorCB != m_vecLockViewCB.end())
	{
		if (*itorCB->m_pCbFun == pLockViewCB &&
			itorCB->m_pInstance == pInstance)
		{
			return;
		}
		itorCB++;
	}
	m_vecLockViewCB.push_back(AnimationDelegate<LockViewCB>(pInstance, pLockViewCB));
}

void CAnimationCallBack::RemoveLockViewCB(LockViewCB pLockViewCB, void* pInstance)
{
	std::vector<AnimationDelegate<LockViewCB>>::iterator itorCB = m_vecLockViewCB.begin();
	while (itorCB != m_vecLockViewCB.end())
	{
		if (*itorCB->m_pCbFun == pLockViewCB &&
			itorCB->m_pInstance == pInstance)
		{
			m_vecLockViewCB.erase(itorCB);
			break;
		}
		itorCB++;
	}
}

void CAnimationCallBack::InvokeLockViewCB(bool bLock)
{
	for (int i = 0; i < (int)m_vecLockViewCB.size(); i++)
	{
		if (m_vecLockViewCB[i].m_pCbFun)
			m_vecLockViewCB[i].m_pCbFun(bLock, m_vecLockViewCB[i].m_pInstance);
	}
}

void CAnimationCallBack::AddSetTargetStateCB(SetTargetStateCB pSetTargetStateCB, void* pInstance)
{
	std::vector<AnimationDelegate<SetTargetStateCB>>::iterator itorCB = m_vecSetTargetStateCB.begin();
	while (itorCB != m_vecSetTargetStateCB.end())
	{
		if (*itorCB->m_pCbFun == pSetTargetStateCB &&
			itorCB->m_pInstance == pInstance)
			return;
		itorCB++;
	}
	m_vecSetTargetStateCB.push_back(AnimationDelegate<SetTargetStateCB>(pInstance, pSetTargetStateCB));
}

void CAnimationCallBack::RemoveSetTargetStateCB(SetTargetStateCB pSetTargetStateCB, void* pInstance)
{
	std::vector<AnimationDelegate<SetTargetStateCB>>::iterator itorCB = m_vecSetTargetStateCB.begin();
	while (itorCB != m_vecSetTargetStateCB.end())
	{
		if (*itorCB->m_pCbFun == pSetTargetStateCB &&
			itorCB->m_pInstance == pInstance)
		{
			m_vecSetTargetStateCB.erase(itorCB);
			break;
		}
		itorCB++;
	}
}

void CAnimationCallBack::InvokeSetTargetStateCB(void* pTargetList)
{
	for (int i = 0; i < (int)m_vecSetTargetStateCB.size(); i++)
	{
		if (m_vecSetTargetStateCB[i].m_pCbFun)
			m_vecSetTargetStateCB[i].m_pCbFun(pTargetList, m_vecSetTargetStateCB[i].m_pInstance);
	}
}


void CAnimationCallBack::AddGetModelPlcMtxCB(GetModelPlcMtxCB pGetModelPlcMtxCB, void* pInstance)
{
	std::vector<AnimationDelegate<GetModelPlcMtxCB>>::iterator itorCB = m_vecGetModelPlcMtxCB.begin();
	while(itorCB != m_vecGetModelPlcMtxCB.end())
	{
		if (*itorCB->m_pCbFun == pGetModelPlcMtxCB &&
			itorCB->m_pInstance == pInstance)
		{
			return;
		}
		itorCB++;
	}
	m_vecGetModelPlcMtxCB.push_back(AnimationDelegate<GetModelPlcMtxCB>(pInstance, pGetModelPlcMtxCB));
}

void CAnimationCallBack::RemoveGetModelPlcMtxCB(GetModelPlcMtxCB pGetModelPlcMtxCB, void* pInstance)
{
	std::vector<AnimationDelegate<GetModelPlcMtxCB>>::iterator itorCB = m_vecGetModelPlcMtxCB.begin();
	while(itorCB != m_vecGetModelPlcMtxCB.end())
	{
		if (*itorCB->m_pCbFun == pGetModelPlcMtxCB &&
			itorCB->m_pInstance == pInstance)
		{
			m_vecGetModelPlcMtxCB.erase(itorCB);
			break;
		}
		itorCB++;
	}
}

void CAnimationCallBack::InvokeGetModelPlcMtxCB(const char plcIdPath[SA_BUFFER_SIZE_SMALL],float matrix[4][4])
{
	for(int i = 0; i < (int)m_vecGetModelPlcMtxCB.size(); i++)
	{
		if (m_vecGetModelPlcMtxCB[i].m_pCbFun)
			m_vecGetModelPlcMtxCB[i].m_pCbFun(plcIdPath, matrix, m_vecGetModelPlcMtxCB[i].m_pInstance);
	}
}

void CAnimationCallBack::AddSetModelPlcMtxCB(SetModelPlcMtxCB pSetModelPlcMtxCB, void* pInstance)
{
	std::vector<AnimationDelegate<SetModelPlcMtxCB>>::iterator itorCB = m_vecSetModelPlcMtxCB.begin();
	while(itorCB != m_vecSetModelPlcMtxCB.end())
	{
		if (*itorCB->m_pCbFun == pSetModelPlcMtxCB &&
			itorCB->m_pInstance == pInstance)
		{
			return;
		}
		itorCB++;
	}
	m_vecSetModelPlcMtxCB.push_back(AnimationDelegate<SetModelPlcMtxCB>(pInstance, pSetModelPlcMtxCB));
}

void CAnimationCallBack::RemoveSetModelPlcMtxCB(SetModelPlcMtxCB pSetModelPlcMtxCB, void* pInstance)
{
	std::vector<AnimationDelegate<SetModelPlcMtxCB>>::iterator itorCB = m_vecSetModelPlcMtxCB.begin();
	while(itorCB != m_vecSetModelPlcMtxCB.end())
	{
		if (*itorCB->m_pCbFun == pSetModelPlcMtxCB &&
			itorCB->m_pInstance == pInstance)
		{
			m_vecSetModelPlcMtxCB.erase(itorCB);
			break;
		}
		itorCB++;
	}
}

void CAnimationCallBack::InvokeSetModelPlcMtxCB(const char plcIdPath[SA_BUFFER_SIZE_SMALL],const float matrix[4][4])
{
	for(int i = 0; i < (int)m_vecSetModelPlcMtxCB.size(); i++)
	{
		if (m_vecSetModelPlcMtxCB[i].m_pCbFun)
			m_vecSetModelPlcMtxCB[i].m_pCbFun(plcIdPath, matrix, m_vecSetModelPlcMtxCB[i].m_pInstance);
	}
}



void CAnimationCallBack::AddGetCameraCB(GetCameraCB pGetCameraCB, void* pInstance)
{
	std::vector<AnimationDelegate<GetCameraCB>>::iterator itorCB = m_vecGetCameraCB.begin();
	while (itorCB != m_vecGetCameraCB.end())
	{
		if (*itorCB->m_pCbFun == pGetCameraCB &&
			itorCB->m_pInstance == pInstance)
		{
			return;
		}
		itorCB++;
	}
	m_vecGetCameraCB.push_back(AnimationDelegate<GetCameraCB>(pInstance, pGetCameraCB));
}

void CAnimationCallBack::RemoveGetCameraCB(GetCameraCB pGetCameraCB, void* pInstance)
{
	std::vector<AnimationDelegate<GetCameraCB>>::iterator itorCB = m_vecGetCameraCB.begin();
	while(itorCB != m_vecGetCameraCB.end())
	{
		if (*itorCB->m_pCbFun == pGetCameraCB &&
			itorCB->m_pInstance == pInstance)
		{
			m_vecGetCameraCB.erase(itorCB);
			break;
		}
		itorCB++;
	}
}

void CAnimationCallBack::InvokeGetCameraCB(const char plcIdPath[SA_BUFFER_SIZE_SMALL],float scale[3],float matrix[4][4])
{
	for (int i = 0; i < (int)m_vecGetCameraCB.size(); i++)
	{
		if (m_vecGetCameraCB[i].m_pCbFun)
			m_vecGetCameraCB[i].m_pCbFun(plcIdPath, scale, matrix, m_vecGetCameraCB[i].m_pInstance);
	}
}


void CAnimationCallBack::AddSetCameraCB(SetCameraCB pSetCameraCB, void* pInstance)
{
	std::vector<AnimationDelegate<SetCameraCB>>::iterator itorCB = m_vecSetCameraCB.begin();
	while(itorCB != m_vecSetCameraCB.end())
	{
		if (*itorCB->m_pCbFun == pSetCameraCB &&
			itorCB->m_pInstance == pInstance)
		{
			return;
		}
		itorCB++;
	}
	m_vecSetCameraCB.push_back(AnimationDelegate<SetCameraCB>(pInstance, pSetCameraCB));
}

void CAnimationCallBack::RemoveSetCameraCB(SetCameraCB pSetCameraCB, void* pInstance)
{
	std::vector<AnimationDelegate<SetCameraCB>>::iterator itorCB = m_vecSetCameraCB.begin();
	while(itorCB != m_vecSetCameraCB.end())
	{
		if (*itorCB->m_pCbFun == pSetCameraCB &&
			itorCB->m_pInstance == pInstance)
		{
			m_vecSetCameraCB.erase(itorCB);
			break;
		}
		itorCB++;
	}
}

void CAnimationCallBack::InvokeSetCameraCB(const char plcIdPath[SA_BUFFER_SIZE_SMALL],const float scale[3],const float matrix[4][4])
{
	for(int i = 0; i < (int)m_vecSetCameraCB.size(); i++)
	{
		if (m_vecSetCameraCB[i].m_pCbFun)
			m_vecSetCameraCB[i].m_pCbFun(plcIdPath, scale, matrix, m_vecSetCameraCB[i].m_pInstance);
	}
}


void CAnimationCallBack::AddGetCameraFocalCB(GetCameraFocalCB pGetCameraFocalCB, void* pInstance)
{
	std::vector<AnimationDelegate<GetCameraFocalCB>>::iterator itorCB = m_vecGetCameraFocalCB.begin();
	while (itorCB != m_vecGetCameraFocalCB.end())
	{
		if (*itorCB->m_pCbFun == pGetCameraFocalCB &&
			itorCB->m_pInstance == pInstance)
		{
			return;
		}
		itorCB++;
	}
	m_vecGetCameraFocalCB.push_back(AnimationDelegate<GetCameraFocalCB>(pInstance, pGetCameraFocalCB));
}

void CAnimationCallBack::RemoveGetCameraFocalCB(GetCameraFocalCB pGetCameraFocalCB, void* pInstance)
{
	std::vector<AnimationDelegate<GetCameraFocalCB>>::iterator itorCB = m_vecGetCameraFocalCB.begin();
	while (itorCB != m_vecGetCameraFocalCB.end())
	{
		if (*itorCB->m_pCbFun == pGetCameraFocalCB &&
			itorCB->m_pInstance == pInstance)
		{
			m_vecGetCameraFocalCB.erase(itorCB);
			break;
		}
		itorCB++;
	}
}

void CAnimationCallBack::InvokeGetCameraFocalCB(float& fPosTargetDistance)
{
	for (int i = 0; i < (int)m_vecGetCameraFocalCB.size(); i++)
	{
		if (m_vecGetCameraFocalCB[i].m_pCbFun)
			m_vecGetCameraFocalCB[i].m_pCbFun(fPosTargetDistance, m_vecGetCameraFocalCB[i].m_pInstance);
	}
}

void CAnimationCallBack::AddGetCameraTargetPntCB(GetCameraTargetPntCB pGetCameraTargetPntCB, void* pInstance)
{
	std::vector<AnimationDelegate<GetCameraTargetPntCB>>::iterator itorCB = m_vecGetCameraTargetPntCB.begin();
	while(itorCB != m_vecGetCameraTargetPntCB.end())
	{
		if (*itorCB->m_pCbFun == pGetCameraTargetPntCB &&
			itorCB->m_pInstance == pInstance)
		{
			return;
		}
		itorCB++;
	}
	m_vecGetCameraTargetPntCB.push_back(AnimationDelegate<GetCameraTargetPntCB>(pInstance, pGetCameraTargetPntCB));
}

void CAnimationCallBack::RemoveGetCameraTargetPntCB(GetCameraTargetPntCB pGetCameraTargetPntCB, void* pInstance)
{
	std::vector<AnimationDelegate<GetCameraTargetPntCB>>::iterator itorCB = m_vecGetCameraTargetPntCB.begin();
	while(itorCB != m_vecGetCameraTargetPntCB.end())
	{
		if (*itorCB->m_pCbFun == pGetCameraTargetPntCB &&
			itorCB->m_pInstance == pInstance)
		{
			m_vecGetCameraTargetPntCB.erase(itorCB);
			break;
		}
		itorCB++;
	}
}

void CAnimationCallBack::InvokeGetCameraTargetPntCB(const char plcIdPath[SA_BUFFER_SIZE_SMALL],float targetPnt[3])
{
	for(int i = 0; i < (int)m_vecGetCameraTargetPntCB.size(); i++)
	{
		if (m_vecGetCameraTargetPntCB[i].m_pCbFun)
			m_vecGetCameraTargetPntCB[i].m_pCbFun(plcIdPath, targetPnt, m_vecGetCameraTargetPntCB[i].m_pInstance);
	}
}

void CAnimationCallBack::AddPlayVisibleCB(PlayVisibleCB pPlayVisibleCB, void* pInstance)
{
	std::vector<AnimationDelegate<PlayVisibleCB>>::iterator itorCB = m_vecPlayVisibleCB.begin();
	while(itorCB != m_vecPlayVisibleCB.end())
	{
		if (*itorCB->m_pCbFun == pPlayVisibleCB &&
			itorCB->m_pInstance == pInstance)
		{
			return;
		}
		itorCB++;
	}
	m_vecPlayVisibleCB.push_back(AnimationDelegate<PlayVisibleCB>(pInstance, pPlayVisibleCB));
}

void CAnimationCallBack::RemovePlayVisibleCB(PlayVisibleCB pPlayVisibleCB, void* pInstance)
{
	std::vector<AnimationDelegate<PlayVisibleCB>>::iterator itorCB = m_vecPlayVisibleCB.begin();
	while(itorCB != m_vecPlayVisibleCB.end())
	{
		if (*itorCB->m_pCbFun == pPlayVisibleCB &&
			itorCB->m_pInstance == pInstance)
		{
			m_vecPlayVisibleCB.erase(itorCB);
			break;
		}
		itorCB++;
	}
}

void CAnimationCallBack::InvokePlayVisibleCB(const char plcIdPath[SA_BUFFER_SIZE_SMALL], const char name[SA_BUFFER_SIZE_SMALL], bool bOnOff, float fTran)
{
	for(int i = 0; i < (int)m_vecPlayVisibleCB.size(); i++)
	{
		if (m_vecPlayVisibleCB[i].m_pCbFun)
			m_vecPlayVisibleCB[i].m_pCbFun(plcIdPath, name, bOnOff, fTran, m_vecPlayVisibleCB[i].m_pInstance);
	}
}

void CAnimationCallBack::AddPlayColorCB(PlayColorCB pPlayColorCB, void* pInstance)
{
	std::vector<AnimationDelegate<PlayColorCB>>::iterator itorCB = m_vecPlayColorCB.begin();
	while(itorCB != m_vecPlayColorCB.end())
	{
		if (*itorCB->m_pCbFun == pPlayColorCB &&
			itorCB->m_pInstance == pInstance)
		{
			return;
		}
		itorCB++;
	}
	m_vecPlayColorCB.push_back(AnimationDelegate<PlayColorCB>(pInstance, pPlayColorCB));
}

void CAnimationCallBack::RemovePlayColorCB(PlayColorCB pPlayColorCB, void* pInstance)
{
	std::vector<AnimationDelegate<PlayColorCB>>::iterator itorCB = m_vecPlayColorCB.begin();
	while(itorCB != m_vecPlayColorCB.end())
	{
		if (*itorCB->m_pCbFun == pPlayColorCB &&
			itorCB->m_pInstance == pInstance)
		{
			m_vecPlayColorCB.erase(itorCB);
			break;
		}
		itorCB++;
	}
}

void CAnimationCallBack::InvokePlayColorCB(const char plcIdPath[SA_BUFFER_SIZE_SMALL], const char name[SA_BUFFER_SIZE_SMALL], const float fColor[3])
{
	for(int i = 0; i < (int)m_vecPlayColorCB.size(); i++)
	{
		if (m_vecPlayColorCB[i].m_pCbFun)
			m_vecPlayColorCB[i].m_pCbFun(plcIdPath, name, fColor, m_vecPlayColorCB[i].m_pInstance);
	}
}

void CAnimationCallBack::AddPlayImageCB(PlayImageCB pPlayImageCB, void* pInstance)
{
	std::vector<AnimationDelegate<PlayImageCB>>::iterator itorCB = m_vecPlayImageCB.begin();
	while(itorCB != m_vecPlayImageCB.end())
	{
		if (*itorCB->m_pCbFun == pPlayImageCB &&
			itorCB->m_pInstance == pInstance)
		{
			return;
		}
		itorCB++;
	}
	m_vecPlayImageCB.push_back(AnimationDelegate<PlayImageCB>(pInstance, pPlayImageCB));
}

void CAnimationCallBack::RemovePlayImageCB(PlayImageCB pPlayImageCB, void* pInstance)
{
	std::vector<AnimationDelegate<PlayImageCB>>::iterator itorCB = m_vecPlayImageCB.begin();
	while(itorCB != m_vecPlayImageCB.end())
	{
		if (*itorCB->m_pCbFun == pPlayImageCB &&
			itorCB->m_pInstance == pInstance)
		{
			m_vecPlayImageCB.erase(itorCB);
			break;
		}
		itorCB++;
	}
}

void CAnimationCallBack::InvokePlayImageCB(const char plcIdPath[SA_BUFFER_SIZE_SMALL], const char name[SA_BUFFER_SIZE_SMALL], const float fPos[3], const float fScale[3], bool bOnOffFlg)
{
	for(int i = 0; i < (int)m_vecPlayImageCB.size(); i++)
	{
		if (m_vecPlayImageCB[i].m_pCbFun)
			m_vecPlayImageCB[i].m_pCbFun(plcIdPath, name, fPos, fScale, bOnOffFlg, m_vecPlayImageCB[i].m_pInstance);
	}
}

void CAnimationCallBack::AddPlayClipPlaneCB(PlayClipPlaneCB pPlayClipPlaneCB, void* pInstance)
{
	std::vector<AnimationDelegate<PlayClipPlaneCB>>::iterator itorCB = m_vecPlayClipPlaneCB.begin();
	while(itorCB != m_vecPlayClipPlaneCB.end())
	{
		if (*itorCB->m_pCbFun == pPlayClipPlaneCB &&
			itorCB->m_pInstance == pInstance)
		{
			return;
		}
		itorCB++;
	}
	m_vecPlayClipPlaneCB.push_back(AnimationDelegate<PlayClipPlaneCB>(pInstance, pPlayClipPlaneCB));
}

void CAnimationCallBack::RemovePlayClipPlaneCB(PlayClipPlaneCB pPlayClipPlaneCB, void* pInstance)
{
	std::vector<AnimationDelegate<PlayClipPlaneCB>>::iterator itorCB = m_vecPlayClipPlaneCB.begin();
	while(itorCB != m_vecPlayClipPlaneCB.end())
	{
		if (*itorCB->m_pCbFun == pPlayClipPlaneCB &&
			itorCB->m_pInstance == pInstance)
		{
			m_vecPlayClipPlaneCB.erase(itorCB);
			break;
		}
		itorCB++;
	}
}

void CAnimationCallBack::InvokePlayClipPlaneCB(const char plcIdPath[SA_BUFFER_SIZE_SMALL], const char name[SA_BUFFER_SIZE_SMALL], const float fPos[3], const float fScale[3], bool bOnOffFlg)
{
	for(int i = 0; i < (int)m_vecPlayClipPlaneCB.size(); i++)
	{
		if (m_vecPlayClipPlaneCB[i].m_pCbFun)
			m_vecPlayClipPlaneCB[i].m_pCbFun(plcIdPath, name, fPos, fScale, bOnOffFlg, m_vecPlayClipPlaneCB[i].m_pInstance);
	}
}


void CAnimationCallBack::AddPlayToolCB(PlayToolCB pPlayToolCB, void* pInstance)
{
	std::vector<AnimationDelegate<PlayToolCB>>::iterator itorCB = m_vecPlayToolCB.begin();
	while(itorCB != m_vecPlayToolCB.end())
	{
		if (*itorCB->m_pCbFun == pPlayToolCB &&
			itorCB->m_pInstance == pInstance)
		{
			return;
		}
		itorCB++;
	}
	m_vecPlayToolCB.push_back(AnimationDelegate<PlayToolCB>(pInstance, pPlayToolCB));
}

void CAnimationCallBack::RemovePlayToolCB(PlayToolCB pPlayToolCB, void* pInstance)
{
	std::vector<AnimationDelegate<PlayToolCB>>::iterator itorCB = m_vecPlayToolCB.begin();
	while(itorCB != m_vecPlayToolCB.end())
	{
		if (*itorCB->m_pCbFun == pPlayToolCB &&
			itorCB->m_pInstance == pInstance)
		{
			m_vecPlayToolCB.erase(itorCB);
			break;
		}
		itorCB++;
	}
}

void CAnimationCallBack::InvokePlayToolCB(int nType, const char plcIdPath[SA_BUFFER_SIZE_SMALL], const char strToolPath[SA_BUFFER_SIZE_SMALL], const char strParentPath[SA_BUFFER_SIZE_SMALL])
{
	for(int i = 0; i < (int)m_vecPlayToolCB.size(); i++)
	{
		if (m_vecPlayToolCB[i].m_pCbFun)
			m_vecPlayToolCB[i].m_pCbFun(nType, plcIdPath, strToolPath, strParentPath, m_vecPlayToolCB[i].m_pInstance);
	}
}
