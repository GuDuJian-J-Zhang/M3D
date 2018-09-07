#include "m3d/extras/Operation.h"
#include "sview/views/View.h"
#include "m3d/SceneManager.h"

namespace M3D
{
const int Operation::TYPE = 0;
Operation::Operation(void) :
		Object()
{
    m_commandType = TYPE;
}

int Operation::GetType()
{
	return m_commandType;
}

Operation::~Operation()
{

}

bool Operation::execute()
{
	bool executeState = false;


	m_view->GetSceneManager()->Lock();
	executeState = OnExecute();
	m_view->GetSceneManager()->UnLock();


	return executeState;
}

bool Operation::Undo()
{
	bool undoState = false;

	return undoState;
}

bool Operation::Redo()
{
	bool redoState = false;

	return redoState;
}

bool Operation::OnExecute()
{
	bool ret = false;

	return ret;
}

string Operation::ToJsonString()
{
	string str;
	return str;
}

View* Operation::GetView()
{
	return this->m_view;
}

void Operation::SetView(View * view)
{
	this->m_view = view;
}
}



