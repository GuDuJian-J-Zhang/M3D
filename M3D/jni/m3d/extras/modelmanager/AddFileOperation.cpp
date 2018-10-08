#include "m3d/extras/modelmanager/AddFileOperation.h"
#include "m3d/extras/modelmanager/ModelAssemblyHelper.h"

namespace M3D
{
const int AddFileOperation::TYPE = 9;
AddFileOperation::AddFileOperation(View * view,string& filePath, Model* desModel) :
		Operation()
{
     m_commandType = TYPE;
	Init( view,filePath,desModel);
}

AddFileOperation::~AddFileOperation()
{
}

string AddFileOperation::ToJsonString()
{
	string str;
	return str;
}

void AddFileOperation::Init(View * view,string& filePath, Model* desModel)
{
	m_filePath = filePath;
	m_desModel = desModel;
	m_view = view;

}

string AddFileOperation::GetUpperPlcPath()
{
	return m_upperPlcPath;
}

string AddFileOperation::GetFilePath()
{
    return m_filePath;
}
    
bool AddFileOperation::OnExecute()
{
	LOGI("AddFileCommand::OnExecute() BEGIN");
	bool ret = false;
	if(m_view)
	{
		Model * parentMdl = m_desModel;
		if(parentMdl)
		{
			m_retModel = ModelAssemblyHelper::AddModel(m_view,m_filePath);

			if(!m_retModel)
			{
				return ret;
			}
            
            if(m_desModel)
            {
                m_upperPlcPath =PathHelper::GetM3DPath(m_desModel);
            }

			LOGI("The model from file ID is %d",m_retModel->GetID());
			ret = ModelAssemblyHelper::InsertBefore(m_view,m_retModel,parentMdl);
			//LOGI("parentMdl Path === %s",GetPlacePath(retModel).c_str());

			if(!ret)
			{
				return ret;
			}

			ModelAssemblyHelper::ComputePlaceMatrix(m_retModel);//设置配置矩阵

			ret = true;
		}
		else
		{
			ret =false;
		}
	}
	else
	{
		ret = false;
	}
	LOGI("AddFileCommand::OnExecute() END");
	return ret;
}

Model* AddFileOperation::GetNewModel() const
{
	return m_retModel;
}

}





