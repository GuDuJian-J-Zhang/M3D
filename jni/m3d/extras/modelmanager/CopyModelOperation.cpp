#include "m3d/extras/modelmanager/CopyModelOperation.h"
#include "m3d/extras/modelmanager/ModelAssemblyHelper.h"
#include "m3d/SceneManager.h"
#include "m3d/model/ExtendInfoManager.h"

namespace M3D
{
const int CopyModelOperation::TYPE = 10;
CopyModelOperation::CopyModelOperation(View * view, Model * srcModel,
		Model * desModel) :
		Operation(), m_srcModel(srcModel), m_desModel(desModel), m_newModel(
				NULL)
{
     m_commandType = TYPE;
	m_view = view;
}

CopyModelOperation::~CopyModelOperation()
{

}

bool CopyModelOperation::OnExecute()
{
	LOGI(" CopyModelCommand::OnExecute() BEGIN");
	bool msg = false;
//#ifdef WIN32
	if (m_view)
	{
		Model * sourceMdl = m_srcModel;
		Model * desMdl = m_desModel;
		m_newModel = new Model();
		if (sourceMdl && desMdl)
		{
			LOGI(" CopyModelCommand::OnExecute() 1");
			m_newModel->CopyData(sourceMdl);

			//ModelNode * mdlNod = ModelAssemblyHelper::GetModelNode(sourceMdl);
			//ModelNode * pareNod = ModelAssemblyHelper::GetModelNode(desMdl);

			SceneManager * scene = m_view->GetSceneManager();

			//GroupNode * grpNod = scene->CreateModelNodes(m_newModel,
			//		pareNod->GetName(), 0); //将newModel 加入scene

			msg = ModelAssemblyHelper::InsertInLast(m_view, m_newModel, desMdl);
			if (!msg)
			{
				delete m_newModel;
				m_newModel = NULL;
				LOGI(" CopyModelCommand::OnExecute()  ASSAMBLY_INS_BEFO_ERR");
				return msg;
			}
			//scene->ReIndexIDMapAfterAddModel(m_newModel);

			scene->GetExtendInfoManager()->CopyMeshAttr(sourceMdl->GetProtoTypeId(), m_newModel->GetProtoTypeId());
			
			Matrix3x4 sourcePlcMat = ModelAssemblyHelper::GetPlaceMatrix(
					sourceMdl);

			//----------------------------------------复制出的零件方向随机指定--------------
			/*BoundingBox& sceneBox = scene->GetSceneBox();
			Vector3 sceneCenter = sceneBox.Center();
			LOGI("sceneCenter == %s", sceneCenter.Tostring().c_str());
			Vector3 trans = Vector3::LEFT * sceneBox.Length() * 0.8f;

			float angle = Random(10.0f, 350.0);
			Quaternion rot(angle, Vector3::UP);

			trans = rot * trans;*/

			Matrix3x4 tempMat;
			//tempMat.MultiTranslate(trans);
			tempMat = Matrix3x4::IDENTITY;
//----------------------------------------------------------------------------------------
			Matrix3x4 pareWorldMat = ModelAssemblyHelper::GetWorldMatrix(
					desMdl);
			Matrix3x4 sourceWorldMat = ModelAssemblyHelper::GetWorldMatrix(
					sourceMdl);

			sourcePlcMat = pareWorldMat.Inverse() * (tempMat * sourceWorldMat);

			m_newModel->SetOrigPlcMatrix(sourcePlcMat);
			msg = true;
		}
		else
		{
			delete m_newModel;
			m_newModel = NULL;
			msg = false;
		}
	}
	else
	{
		LOGI(" CopyModelCommand::OnExecute() ASSAMBLY_CPYTO_ERR");
		msg = false;
	}
	LOGI(" CopyModelCommand::OnExecute() END");
//#endif
	return msg;
}

string CopyModelOperation::ToJsonString()
{
	string str;
	return str;
}

Model* CopyModelOperation::GetNewMode() const
{
	return m_newModel;
}
    
const string& CopyModelOperation::GetDestPlcPath()
{
    return m_destPlcPath;
}
    
const string& CopyModelOperation::GetSrcPlcPath()
{
    return m_srcPlcPath;
}
    
}

