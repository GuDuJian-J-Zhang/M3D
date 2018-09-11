#include "m3d/extras/modelmanager/MoveModelOperation.h"
#include "m3d/extras/modelmanager/ModelAssemblyHelper.h"

namespace M3D
{
const int MoveModelOperation::TYPE = 11;
MoveModelOperation::MoveModelOperation(View * view, Model * srcModel,
		Model* desModel) :
		Operation(), m_srcModel(srcModel), m_desModel(desModel)
{
    m_commandType = TYPE;
    
	m_ForwPlcPath = PathHelper::GetM3DPath(m_srcModel);
	m_AfterPlcpath = "";
	m_view = view;
}

MoveModelOperation::~MoveModelOperation()
{

}

string MoveModelOperation::ToJsonString()
{
	string str;

	return str;
}

bool MoveModelOperation::OnExecute()
{
	LOGI("MoveModelCommand::OnExecute() BEGIN)");
	bool msg = false;

	if(m_view)
	{
		Model * sourceMdl = m_srcModel;
		Model * desMdl = m_desModel;
		if(sourceMdl && desMdl)
		{
			sourceMdl->AddRef();

			LOGI("ModelManager::MoveTo 1)");

			Model * parent = (Model*)sourceMdl->GetParent();
			if (parent)
			{
				parent->RemoveSubModel(sourceMdl);
			}

			ModelAssemblyHelper::ComputePlaceMatrix(sourceMdl, desMdl);//设置配置矩阵

			msg = ModelAssemblyHelper::InsertInLast(m_view,sourceMdl,desMdl);

           m_AfterPlcpath = PathHelper::GetM3DPath(desMdl);
			//m_AfterPlcpath = PathHelper::GetM3DPath(sourceMdl);

			sourceMdl->Release();
		}

		msg = true;

	}
	else
	{
		LOGI("MoveModelCommand::OnExecute() ASSAMBLY_MOVTO_ERR)");
		msg = false;
	}
	LOGI("MoveModelCommand::OnExecute() END)");
	return msg;

}

string MoveModelOperation::GetForwPlcPath()
{
	return m_ForwPlcPath;
}
//必须执行完移动后，才能得到正确的结果.
string MoveModelOperation::GetAfterPlcPath()
{
	return m_AfterPlcpath;
}

}

