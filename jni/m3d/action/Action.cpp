#include "m3d/action/Action.h"
#include "m3d/renderer/RenderContext.h"
 
namespace M3D
{
Action::Action()
{
	m_iActionType = KEEPING;
	m_bOnceFinished = false;
	m_bKeepingFinished = false;
	this->m_GLContext = NULL;
}

Action::~Action()
{

}

bool Action::IsFinish()
{
	//to  do
	return this->m_bOnceFinished;
}

void Action::SetFinish(bool state)
{
	//this->bKeepingFinished=true;
	this->m_bOnceFinished = true;
}

int Action::GetType()
{
	return -1;
}

void Action::Apply(SceneNode * node)
{
	this->OnExecute(node);
}

void Action::Apply(Model * node)
{
	this->OnExecute(node);
}

void Action::ApplyOnce()
{
	 
}

void Action::OnExecute(SceneNode* node)
{

}

void Action::OnExecute(Model* node)
{

}

RenderContext* Action::GetGLContext()
{
	return this->m_GLContext;
}

void Action::SetApplyType(int type)
{
	this->m_iActionType = type;
}

int Action::GetApplyType()
{
	return this->m_iActionType;
}

void Action::SetGLContext(RenderContext* gl)
{
	//if (gl != NULL)
	{
		//delete this->m_GLContext;
		this->m_GLContext = gl;
	}
}

}
