#include "ReplaceModelOperation.h"
#include "m3d/extras/modelmanager/ModelAssemblyHelper.h"


namespace M3D
{
	ReplaceModelOperation::ReplaceModelOperation(View * view, Model* oldModel, Model* oldParentModel, string& filePath)
		: Operation()
	{
		m_commandType = TYPE;
		Init(view, oldModel, oldParentModel, filePath);
		m_newModel = NULL;
	}


	ReplaceModelOperation::~ReplaceModelOperation()
	{
	}

	void ReplaceModelOperation::Init(View * view, Model* oldModel, Model* oldParentModel, string& filePath)
	{
		m_view = view;
		m_oldModel = oldModel;
		m_oldParentModel = oldParentModel;
		m_filePath = filePath;
	}

	bool ReplaceModelOperation::OnExecute()
	{
		//替换操作
		//1 打开用来替换的文件
		//2 构建新的Model
		//3 使用原有模型的路径、矩阵等替换新的Model（包含子件的替换）
		//4 删除原有装配
		LOGI(" ReplaceModelCommand::OnExecute() BEGIN");
		bool bRet = false;
		if (!m_view || !m_oldModel || !m_oldParentModel)
			return bRet;
		//打开文件 并创建了新的装配
		m_newModel = ModelAssemblyHelper::AddModel(m_view, m_filePath);
		if (!m_newModel)
			return bRet;
		int iOldId = m_oldModel->GetPlcId();
		//删除原有模型
		ModelAssemblyHelper::DeleteModel(m_view, m_oldModel);
		//插入指定位置 使ID和原装配相同
		bRet = ModelAssemblyHelper::InsertInPos(m_view, m_newModel, iOldId, m_oldParentModel);
		if (!bRet)
		{
			ModelAssemblyHelper::DeleteModel(m_view, m_newModel);
			m_newModel = NULL;
			return bRet;
		}
		//新装配的元素替换 使用原有装配的元素替换新装配
		ReplaceModelData(m_oldModel, m_newModel);
		
		LOGI(" ReplaceModelCommand::OnExecute() END");
		return bRet;
	}

	Model * ReplaceModelOperation::GetNewModel() const
	{
		return m_newModel;
	}

	void ReplaceModelOperation::ReplaceModelData(Model* oldModel, Model* newModel)
	{
		//父模型替换 矩阵
		//递归替换子模型的路径? 需要吗
		bool bRet = false;
		if (!oldModel || !newModel)
			return;
		//配置ID
		newModel->SetPlcId(oldModel->GetPlcId());
		//矩阵
		Matrix3x4* oldMatrix = oldModel->GetPlaceMatrix();
		newModel->SetPlaceMatrix(*oldMatrix);
		bRet = true;
		return;
	}
}

