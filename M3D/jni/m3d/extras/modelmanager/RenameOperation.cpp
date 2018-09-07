/*
 * RenameOperation.cpp
 *
 *  Created on: 2016-1-6
 *      Author: CDJ
 */
#include "m3d/extras/modelmanager/RenameOperation.h"
#include "m3d/extras/modelmanager/ModelAssemblyHelper.h"
namespace M3D
{

RenameOperation::RenameOperation(View * view,Model * model,string & name):Operation(),m_newName(name),m_model(model)
{
	m_view = view ;
	m_lastName = model->GetName();

}

RenameOperation::~RenameOperation()
{
	// TODO Auto-generated destructor stub
}


bool RenameOperation::OnExecute()
{
	bool ret = false;
	if (m_view)
	{
		if (m_model)
		{
			m_model->SetName(m_newName);
			ret = true;
		}
		else
		{
			ret = false;
		}
	}
	return ret;
}

}
 /* namespace M3D */
