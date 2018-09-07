#include "m3d/action/CallBackAction.h"

namespace M3D
{
CallBackAction::CallBackAction() :
		Action()
{
	 m_pData = NULL;
	 m_pfun= NULL;
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

void CallBackAction::SetActionData(void* data)
{
	this->m_pData = data;
}

void CallBackAction::OnExecute(Model* node)
{
	if (m_pfun != NULL && m_pData)
	{
		m_pfun(m_pData, node);
	}
}

}
