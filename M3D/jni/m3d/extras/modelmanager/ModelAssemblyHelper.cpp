/*
 * ModelAssemblyHelper.cpp
 *
 *  Created on: 2016-1-4
 *      Author: CDJ
 */

#include "m3d/extras/modelmanager/ModelAssemblyHelper.h"
#include "m3d/model/GeoAttribute.h"
#include "m3d/SceneManager.h"
#include "m3d/utils/M3DTools.h"
#include "m3d/utils/ShapeHelper.h"
#include "m3d/ResourceManager.h"
#include "m3d/SceneManager.h"
#include "m3d/scenemanager.h"
#include "sview/utils/ViewHelper.h"
#include "m3d/scene/ShapeNode.h"
#include "m3d/scene/GroupNode.h"
#include "m3d/scene/ModelNode.h"
#include "m3d/Utils/FileHelper.h"
#include "sview/io/Reader.h"
#include "sview/io/Writer.h"
#include "m3d/model/Body.h"
#include "m3d/model/ShapeSet.h"
#include "m3d/extras/OperationHistoryManager.h"
#include "m3d/base/Quaternion.h"

using SVIEW::Reader;
namespace M3D
{

ModelAssemblyHelper::ModelAssemblyHelper()
{
	// TODO Auto-generated constructor stub

}

ModelAssemblyHelper::~ModelAssemblyHelper()
{
	// TODO Auto-generated destructor stub
}




Matrix3x4 ModelAssemblyHelper::GetPlaceMatrix(Model* model)
{
	Matrix3x4 mat;
	if (model)
	{
		ModelNode * tempNode = GetModelNode(model);
		if(tempNode)
		{
			mat = tempNode->GetPlcMatrix();
		}

	}
	return mat;
}

Matrix3x4 ModelAssemblyHelper::GetWorldMatrix(Model* model)
{
	Matrix3x4 mat;
	if (model)
	{
		ModelNode * tempNode = GetModelNode(model);
		if(tempNode)
		{
			mat = tempNode->GetWorldTransform();
		}
	}
	return mat;
}

string ModelAssemblyHelper::GetPlacePath(Model* model)
{
	string path = "";
	if (model)
	{
		ModelNode * tempNode =  GetModelNode(model);
		if(tempNode)
		{
			path = tempNode->GetName();
		}
		//LOGI("ModelManager::GetPlacePath path === %s",path.c_str());
	}

	return path;
}

void ModelAssemblyHelper::SetPlacePath(Model* model, string path)
{
	if (model)
	{
		ModelNode * tempNode =  GetModelNode(model);
		if(tempNode)
		{
			tempNode->SetName(path);
		}
	}
}

void ModelAssemblyHelper::SetPlacePath(Model* model, Model* pareModel)
{
	if (model && pareModel)
	{
		string parePlcPath = GetPlacePath(pareModel);

		int tempId = model->GetPlcId();
		char tempIdBuf[100];
		sprintf(tempIdBuf, "%x", tempId); //转化成16进制
		string idStr(tempIdBuf);

		string childPlcPath = parePlcPath + "|" + idStr;

//		LOGI("childPlcPath == %s", childPlcPath.c_str());
		SetPlacePath(model, childPlcPath);
		if (model->IsAssembly())
		{
//			LOGI("model->GetSubModelCount() ==%d", model->GetSubModelCount());
			for (int i = 0; i < model->GetSubModelCount(); i++)
			{
				SetPlacePath(model->GetSubModels().at(i), model);
			}
		}

	}
}

void ModelAssemblyHelper::SetPlaceMatrix(Model* model, const Matrix3x4& plcMat)
{
	if (model)
	{
		ModelNode * tempNode = GetModelNode(model);
		if(tempNode)
		{
			tempNode->SetOrigPlcMatrix(plcMat);
		}
	}
}

void ModelAssemblyHelper::ComputePlaceMatrix(Model* model)
{
	ModelNode * modelNode = GetModelNode(model);
	ModelNode * parentNode = GetParentNode(model);
	if(modelNode  && parentNode)
	{

		Matrix3x4 plcMat =
				parentNode->GetWorldTransform().Inverse() *modelNode->GetWorldTransform();
		LOGI("=====================================");
		LOGI("%s",parentNode->GetWorldTransform().Inverse().ToString().c_str());
		LOGI("=====================================");
		LOGI("=====================================");
		LOGI("%s",modelNode->GetWorldTransform().ToString().c_str());
		LOGI("=====================================");
		SetPlaceMatrix(model, plcMat);
	}
	else
	{
		LOGI("parentNode is NULL");
	}
}

void ModelAssemblyHelper::SetWorldMatrix(Model* model, const Matrix3x4& wldMat)
{
	if (model)
	{
		ModelNode * tempNode =GetModelNode(model);
		if(tempNode)
		{
			tempNode->SetWorldTransform(wldMat);
		}
	}
}

int ModelAssemblyHelper::GetMaxPlcId(Model* model)
{
	int ret = -1;
	int maxNum = -3;
	if(model)
	{
		if(model->GetSubModelCount()<1)
		{
			LOGE("model's sub models is null ");
			return ret;
		}
		else
		{
			vector<Model*>& subModels = model->GetSubModels();
			for(int i = 0;i<subModels.size();i++)
			{
				Model * tempModel = subModels.at(i);
				int plcId = tempModel->GetPlcId();
				if(plcId>maxNum)
				{
					maxNum = plcId;
				}
			}
		}

	}

	ret  = maxNum+1;

	return ret;
}

void ModelAssemblyHelper::PrintPlcID(Model * model)
{
	ModelNode* sourceModelNod = GetModelNode(model);
	for(int i = 0;i<model->GetSubModels().size();i++)
	{
		PrintPlcID(model->GetSubModels().at(i));
	}
	LOGI("sourceModelNod name == %s",sourceModelNod->GetName().c_str());
}

bool ModelAssemblyHelper::InsertBefore(View * view,Model* sourceModel, Model* desModel)
{
	//LOGI(" ModelManager::InsertBefore BEGIN");
	bool msg =false;
	if (sourceModel && desModel)
	{
		//LOGI(" ModelManager::InsertBefore 1");
		string desModelName = GetPlacePath(desModel);

		ModelNode* sourceModelNod = GetModelNode(sourceModel);

		ModelNode* desModelNod = GetModelNode(desModel);

	//	LOGI("sourceModelNod pointer = %p,desModelNod pointer = %p",sourceModelNod,desModelNod);

		if (sourceModelNod && desModelNod)
		{
		//	LOGI(" ModelManager::InsertBefore 2");
/*			//判断子模型数量，若为0则是零件，不为零则为装配
			if(desModel->GetSubModelCount()<1)
			{
//				GroupNode* pare = (GroupNode*) desModel->GetParent();

//				pare->AddChildInSameLevelBefore(sourceModelNod, desModelName);

				msg =  ASSEMBLYMSG::ASSAMBLY_INS_BEFO_ERR;
			}
			else
			{
			*/	//若原零件是个装配，那么把要插入的零件之间放入最后
			int tempId = desModel->GetUseablePlcId();
				desModelNod->AddChild(sourceModelNod);
				desModel->AddSubModel(sourceModel);


				sourceModel->SetPlcId(tempId);
//				LOGI("temoId == %d",tempId);
				view->GetSceneManager()->ReIndexIDMapAfterAddModel(sourceModel);
				sourceModelNod->UpdateName();
//				PrintPlcID(sourceModel);
/*			}*/
		}

		msg =  true;
	}
	else
	{
		//LOGI("ModelManager::InsertBefor ASSEMBLYMSG::ASSAMBLY_INS_BEFO_ERR");
		msg = false;
	}
//	LOGI(" ModelManager::InsertBefore END");
	return msg;

}

bool ModelAssemblyHelper::DetachModel(Model* model)
{
	bool msg = false;
	if (model)
	{
		ModelNode* tempNode = GetModelNode(model);
		ModelNode* pare = GetParentNode(model);
		if(pare && tempNode)
		{
			pare->DetachChild(tempNode);
		}

		Model * parent  = model->GetParent();
		if(parent)
		{
			parent->DetachSubModel(model);
		}

		msg =  true;

	}
	else
	{
		msg =  false;
	}

	return msg;
}

bool ModelAssemblyHelper::DeleteModel(View* view, Model* model)
{
	LOGI("ModelAssemblyHelper::DeleteModel BEGIN");
	bool msg = false;
	if (model)
		{
		LOGI("ModelAssemblyHelper::DeleteModel 1");
			ModelNode* tempNode = GetModelNode(model);
			ModelNode* pare = GetParentNode(model);
			if(pare && tempNode)
			{
				LOGI("pare->DeleteChild");
				pare->DeleteChild(tempNode);
			}

			Model * parentMdl  = model->GetParent();
			if(parentMdl)
			{
				LOGI("parentMdl->RemoveSubModel");
				LOGI("parentMdl pointer is %p",parentMdl);
				parentMdl->DetachSubModel(model);	//这里不要用removeModel
			}

			SceneManager* scene = view->GetSceneManager();
			LOGI("scene->ReIndexIDMapAfterDeleteModel BEGIN");
			scene->ReIndexIDMapAfterDeleteModel(model);//在这里就已经将model删除了，因此上面只需detach
			LOGI("scene->ReIndexIDMapAfterDeleteModel END");
			msg =  true;

		}
		else
		{
			LOGI("ModelAssemblyHelper::DeleteModel ASSAMBLY_DEL_ERR");
			msg =  false;
		}

	LOGI("ModelAssemblyHelper::DeleteModel END");
	return msg;
}

Model* ModelAssemblyHelper::GetModelFromId(View* view, int modelId)
{
	Model * ret = NULL;
	Shape * modelShp = view->GetShape(modelId);
	if(modelShp&&modelShp->GetType() == SHAPE_MODEL)
	{
		//LOGI("ModelManager::GetModelFromId complete");
		ret = (Model * )modelShp;
	}
	return ret;
}

ModelNode* ModelAssemblyHelper::GetParentNode(Model* model)
{
	ModelNode * ret = NULL;

	if(model)
	{
		SceneNode * node = model->GetSceneNode()->GetParent();
		if(node)
		{
			SceneNode * tempRet = node->GetParent();
			if(tempRet && tempRet->GetType()==MODEL_NODE)
			{
				ret = (ModelNode *) (tempRet);
			}
		}

	}

	return ret;
}

Model* ModelAssemblyHelper::AddModel(View * view,const string& filePath)
{
	string curFilePath = filePath;
	curFilePath = FileHelper::GetUnionStylePath(curFilePath);

	LOGI("curFilePath is ==%s",curFilePath.c_str());
	Reader* reader = Reader::GetReader(curFilePath);
    reader->SetView(view);
	Model* model = reader->GetModel();

	//得到顶级模型
	Model* topModel = view->GetSceneManager()->GetModel();
	//得到子模型,加入Model装配中
//	if(topModel->IsAssembly())
//	{
//		topModel->AddSubModel(model);
//	}
	SceneManager* scene = view->GetSceneManager();

	int defaultPlcId = topModel->GetSubModelCount()-1;

	//将模型加入装配中
	SceneNode* sceneNode = topModel->GetSceneNode()->GetParent();
	if(sceneNode && sceneNode->GetType() == MODEL_NODE)
	{
		ModelNode* modelNode = (ModelNode*)sceneNode;

		GroupNode* modelGroup = scene->CreateModelNodes(model,modelNode->GetName(),defaultPlcId);

//		modelNode->AddChild(modelGroup);
//
//		scene->ReIndexIDMapAfterAddModel(model);
	}

	delete reader;

	return model;
}

ModelNode* ModelAssemblyHelper::GetModelNode(Model* model)
{
	//LOGI("ModelManager::GetModelNode BEGIN");
	ModelNode* ret = NULL;
	if (model)
	{
		SceneNode* tempRet = model->GetSceneNode()->GetParent();
		if (tempRet)
//			LOGI("tempRet->GetType()== %d", tempRet->GetType());
		if (tempRet && tempRet->GetType() == MODEL_NODE)
		{
			ret = (ModelNode *) tempRet;
		}
	}
	//LOGI("ModelManager::GetModelNode END");
	return ret;
}
/**
 * @brief 旋转装配模型
 * @param model 要旋转的模型
 * @param quat 旋转量
 */
void ModelAssemblyHelper::RotateAssemblyModel(Model* model, Quaternion& quat)
{
	ModelNode* modelNode = GetModelNode(model);
	ModelNode * parentNode = GetParentNode(model);
	ShapeNode * shpNd = (ShapeNode*) model->GetSceneNode();

	Matrix3x4 modelWMat;
	if(modelNode)
	{
		modelWMat = modelNode->GetWorldTransform();
	}
	else
	{
		modelWMat = Matrix3x4::IDENTITY;
	}
	Matrix3x4 pareWMat; //父节点世界转换矩阵

	if (parentNode)
	{
		pareWMat = parentNode->GetWorldTransform();
	}
	else
	{
		pareWMat = Matrix3x4::IDENTITY;
	}

	Vector3 boxCenter = GetModelWorldBoundingBox(model).Center();
//	LOGI("boxCenter === %s",boxCenter.Tostring().c_str());
	Matrix3x4 trans1, trans2, rot1;
	trans1.MultiTranslate(-boxCenter);
	rot1.MultiRotatiton(quat);
	trans2.MultiTranslate(boxCenter);
	Matrix3x4 tempmat = trans2 * rot1 * trans1;
	Matrix3x4 plcMat = pareWMat.Inverse() * tempmat * modelWMat;
#ifdef WIN32
	modelNode->SetPlcMatrix(plcMat);
#else
	modelNode->SetOrigPlcMatrix(plcMat);
	modelNode->SetPlcMatrix(plcMat);
#endif // WIN32
}

/**
 * @brief 平移装配模型
 * @param model 要平移的模型
 * @param move  平移量
 */
void ModelAssemblyHelper::TranslateAssemblyModel(Model* model, Vector3& move)
{
	ModelNode* modelNode = GetModelNode(model);
	ModelNode * parentNode = GetParentNode(model);
	ShapeNode * shpNd = (ShapeNode*) model->GetSceneNode();

	Matrix3x4 modelWMat;
	if(modelNode)
	{
		modelWMat = modelNode->GetWorldTransform();
	}
	else
	{
		modelWMat = Matrix3x4::IDENTITY;
	}
	Matrix3x4 pareWMat; //父节点世界转换矩阵

	if (parentNode)
	{
		pareWMat = parentNode->GetWorldTransform();
	}
	else
	{
		pareWMat = Matrix3x4::IDENTITY;
	}

	Matrix3x4 tempmat;
	tempmat.MultiTranslate(move);
	Matrix3x4 plcMat = pareWMat.Inverse() * (tempmat * modelWMat);
#ifdef WIN32
	modelNode->SetPlcMatrix(plcMat);
#else
	modelNode->SetOrigPlcMatrix(plcMat);
	modelNode->SetPlcMatrix(plcMat);
#endif // WIN32

}

/**
 * @brief 缩放装配模型
 * @param model 要缩放的模型
 * @param scale 缩放量
 */
void ModelAssemblyHelper::ScaleAssemblyModel(Model* model, Vector3& scale)
{
	ModelNode* modelNode = GetModelNode(model);
	ModelNode * parentNode = GetParentNode(model);
	ShapeNode * shpNd = (ShapeNode*) model->GetSceneNode();

	Matrix3x4 modelWMat;
	if(modelNode)
	{
		modelWMat = modelNode->GetWorldTransform();
	}
	else
	{
		modelWMat = Matrix3x4::IDENTITY;
	}
	Matrix3x4 pareWMat; //父节点世界转换矩阵

	if (parentNode)
	{
		pareWMat = parentNode->GetWorldTransform();
	}
	else
	{
		pareWMat = Matrix3x4::IDENTITY;
	}

	Vector3 boxCenter =GetModelWorldBoundingBox(model).Center();
	Matrix3x4 trans1, trans2, rot1;
	trans1.MultiTranslate(-boxCenter);
	rot1.MultiScale(scale);
	trans2.MultiTranslate(boxCenter);
	Matrix3x4 tempmat = trans2 * rot1 * trans1;
	Matrix3x4 plcMat = pareWMat.Inverse() * tempmat * modelWMat;
#ifdef WIN32
	modelNode->SetPlcMatrix(plcMat);
#else
	modelNode->SetOrigPlcMatrix(plcMat);
	modelNode->SetPlcMatrix(plcMat);
#endif // WIN32


}

BoundingBox ModelAssemblyHelper::GetModelWorldBoundingBox(Model * model)
{
	ShapeNode * shpNd = (ShapeNode*) model->GetSceneNode();
	BoundingBox tempBox = shpNd->GetWorldBoundingBox();
	if(model->GetSubModelCount()>0)
	{

		for(int i=0;i<model->GetSubModels().size();i++ )
		{
			tempBox.Merge(GetModelWorldBoundingBox(model->GetSubModels().at(i)));
		}
	}
	return tempBox;
}

}

