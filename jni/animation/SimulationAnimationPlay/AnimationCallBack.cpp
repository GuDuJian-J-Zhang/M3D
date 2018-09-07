#include "AnimationCallBack.h"



CAnimationCallBack::CAnimationCallBack()
{
}


CAnimationCallBack::~CAnimationCallBack()
{
	m_vecUpdateViewCB.clear();
	m_vecPlayBeginCB.clear();
}

void CAnimationCallBack::AddUpdateViewCB(UpdateViewCB* pUpdateViewCB)
{
	std::vector<UpdateViewCB*>::iterator itorCB = m_vecUpdateViewCB.begin();
	while (itorCB != m_vecUpdateViewCB.end())
	{
		if (*itorCB == pUpdateViewCB)
			return;
		itorCB++;
	}
	m_vecUpdateViewCB.push_back(pUpdateViewCB);
}

void CAnimationCallBack::RemoveUpdateViewCB(UpdateViewCB* pUpdateViewCB)
{
	std::vector<UpdateViewCB*>::iterator itorCB = m_vecUpdateViewCB.begin();
	while (itorCB != m_vecUpdateViewCB.end())
	{
		if (*itorCB == pUpdateViewCB)
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
		if (m_vecUpdateViewCB[i])
			m_vecUpdateViewCB[i](pBehaviorAction);
	}
}

void CAnimationCallBack::AddPlayBeginCB(PlayBeginCB* pPlayBeginCB)
{
	std::vector<PlayBeginCB*>::iterator itorCB = m_vecPlayBeginCB.begin();
	while (itorCB != m_vecPlayBeginCB.end())
	{
		if (*itorCB == pPlayBeginCB)
			return;
		itorCB++;
	}
	m_vecPlayBeginCB.push_back(pPlayBeginCB);
}

void CAnimationCallBack::RemovePlayBeginCB(PlayBeginCB* pPlayBeginCB)
{
	std::vector<PlayBeginCB*>::iterator itorCB = m_vecPlayBeginCB.begin();
	while (itorCB != m_vecPlayBeginCB.end())
	{
		if (*itorCB == pPlayBeginCB)
		{
			m_vecPlayBeginCB.erase(itorCB);
			break;
		}
		itorCB++;
	}
}

void CAnimationCallBack::InvokePlayBeginCB()
{
	for (int i = 0; i < (int)m_vecPlayBeginCB.size(); i++)
	{
		if (m_vecPlayBeginCB[i])
			m_vecPlayBeginCB[i]();
	}
}

void CAnimationCallBack::AddSetTargetStateCB(SetTargetStateCB* pSetTargetStateCB)
{
	std::vector<SetTargetStateCB*>::iterator itorCB = m_vecSetTargetStateCB.begin();
	while (itorCB != m_vecSetTargetStateCB.end())
	{
		if (*itorCB == pSetTargetStateCB)
			return;
		itorCB++;
	}
	m_vecSetTargetStateCB.push_back(pSetTargetStateCB);
}

void CAnimationCallBack::RemoveSetTargetStateCB(SetTargetStateCB* pSetTargetStateCB)
{
	std::vector<SetTargetStateCB*>::iterator itorCB = m_vecSetTargetStateCB.begin();
	while (itorCB != m_vecSetTargetStateCB.end())
	{
		if (*itorCB == pSetTargetStateCB)
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
		if (m_vecSetTargetStateCB[i])
			m_vecSetTargetStateCB[i](pTargetList);
	}
}