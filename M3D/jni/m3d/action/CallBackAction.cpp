#include "m3d/action/CallBackAction.h"

namespace M3D
{
CallBackAction::CallBackAction() :
		Action()
{
	 m_pData = NULL;
	 m_pfun= NULL;
	 m_pLfun= NULL;
}

CallBackAction::~CallBackAction()
{

}

int CallBackAction::GetType()
{
	return CALLBACK_ACTION;
}

void CallBackAction::SetActionFun(ActionFun fun)
{
	this->m_pfun = fun;
}

void CallBackAction::SetActionLFun(LActionFun fun)
{
	this->m_pLfun = fun;
}

void CallBackAction::SetActionData(void* data)
{
	this->m_pData = data;
}

void CallBackAction::OnExecute(SceneNode* node)
{
	if (m_pfun != NULL && m_pData)
	{
		m_pfun(m_pData, node);
	}
}

void CallBackAction::OnExecute(LSceneNode* node)
{
	if (m_pLfun != NULL && m_pData)
	{
		m_pLfun(m_pData, node);
	}
}

}
