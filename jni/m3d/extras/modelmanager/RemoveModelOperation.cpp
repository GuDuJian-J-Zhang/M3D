#include "m3d/extras/modelmanager/RemoveModelOperation.h"
#include "m3d/extras/modelmanager/ModelAssemblyHelper.h"

namespace M3D
{
const int RemoveModelOperation::TYPE = 12;
RemoveModelOperation::RemoveModelOperation(View * view,
		Model * model) :
		Operation()
{
     m_commandType = TYPE;
    
	Init(view, model);
}

RemoveModelOperation::~RemoveModelOperation()
{

}

bool RemoveModelOperation::OnExecute()
{
	LOGI(" RemoveModelCommand::OnExecute() BEGIN");
	bool msg = false;

	if(m_view)
	{
		if(m_model)
		{
			LOGI(" RemoveModelCommand::OnExecute() 1");
			msg = ModelAssemblyHelper::DeleteModel(m_view,m_model);
		}
		else
		{
			LOGI(" RemoveModelCommand::OnExecute() ASSAMBLY_DEL_ERR");
			msg =false;
		}
	}
	else
	{
		LOGI(" RemoveModelCommand::OnExecute() ASSAMBLY_DEL_ERR");
		msg = false;
	}
	LOGI(" RemoveModelCommand::OnExecute() END");
	return msg;
}

string RemoveModelOperation::ToJsonString()
{
	string str;
	return str;
}

string RemoveModelOperation::GetPlcPath()
{
	return m_plcPath;
}

void RemoveModelOperation::Init(View* view, Model* model)
{
	m_plcPath = "";
	m_model = model;
	m_view = view;
	if(m_model)
	{
        m_plcPath = PathHelper::GetM3DPath(m_model);
	}
}
}




