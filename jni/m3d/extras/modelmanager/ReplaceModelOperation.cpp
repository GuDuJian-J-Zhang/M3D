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
		//�滻����
		//1 �������滻���ļ�
		//2 �����µ�Model
		//3 ʹ��ԭ��ģ�͵�·����������滻�µ�Model�������Ӽ����滻��
		//4 ɾ��ԭ��װ��
		LOGI(" ReplaceModelCommand::OnExecute() BEGIN");
		bool bRet = false;
		if (!m_view || !m_oldModel || !m_oldParentModel)
			return bRet;
		//���ļ� ���������µ�װ��
		m_newModel = ModelAssemblyHelper::AddModel(m_view, m_filePath);
		if (!m_newModel)
			return bRet;
		int iOldId = m_oldModel->GetPlcId();
		//ɾ��ԭ��ģ��
		ModelAssemblyHelper::DeleteModel(m_view, m_oldModel);
		//����ָ��λ�� ʹID��ԭװ����ͬ
		bRet = ModelAssemblyHelper::InsertInPos(m_view, m_newModel, iOldId, m_oldParentModel);
		if (!bRet)
		{
			ModelAssemblyHelper::DeleteModel(m_view, m_newModel);
			m_newModel = NULL;
			return bRet;
		}
		//��װ���Ԫ���滻 ʹ��ԭ��װ���Ԫ���滻��װ��
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
		//��ģ���滻 ����
		//�ݹ��滻��ģ�͵�·��? ��Ҫ��
		bool bRet = false;
		if (!oldModel || !newModel)
			return;
		//����ID
		newModel->SetPlcId(oldModel->GetPlcId());
		//����
		Matrix3x4* oldMatrix = oldModel->GetPlaceMatrix();
		newModel->SetPlaceMatrix(*oldMatrix);
		bRet = true;
		return;
	}
}

