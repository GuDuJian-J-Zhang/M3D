///*
// * ModelAssemblyHelper.cpp
// *
// *  Created on: 2016-1-4
// *      Author: CDJ
// */
//
#include "m3d/ResourceManager.h"
#include "m3d/extras/modelmanager/ModelAssemblyHelper.h"
#include "m3d/model/GeoAttribute.h"
#include "m3d/model/Model.h"

#include "m3d/SceneManager.h"
#include "m3d/SceneManager.h"
#include "m3d/utils/ShapeHelper.h"
#include "m3d/utils/M3DTools.h"
#include "m3d/utils/FileHelper.h"
#include "sview/utils/ViewHelper.h"
#include "m3d/extras/OperationHistoryManager.h"

#include "sview/views/Parameters.h"
#include "m3d/scene/ShapeNode.h"
#include "sview/io/Writer.h"
#include "m3d/model/Body.h"
#include "m3d/model/ShapeSet.h"
#include "m3d/SceneManager.h"
#include "m3d/scene/GroupNode.h"
#include "sview/io/Reader.h"
#include "sview/io/SVL2AsynReader.h"
#include "m3d/base/Quaternion.h"

using SVIEW::Reader;
using SVIEW::Parameters;
using SVIEW::SVL2AsynReader;
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
	/*	ModelNode * tempNode = GetModelNode(model);
		if(tempNode)
		{
			mat = tempNode->GetPlcMatrix();
		}*/

	}
	return mat;
}

Matrix3x4 ModelAssemblyHelper::GetWorldMatrix(Model* model)
{
	Matrix3x4 mat;
	if (model)
	{
		mat = model->GetWorldTransform();
	}
	return mat;
}

string ModelAssemblyHelper::GetPlacePath(Model* model)
{
	string path = "";
	if (model)
	{
		//ModelNode * tempNode =  GetModelNode(model);
		//if(tempNode)
		//{
		//	path = tempNode->GetName();
		//}
		//LOGI("ModelManager::GetPlacePath path === %s",path.c_str());
	}

	return path;
}

void ModelAssemblyHelper::SetPlacePath(Model* model, string path)
{
	if (model)
	{
		//ModelNode * tempNode =  GetModelNode(model);
		//if(tempNode)
		//{
		//	tempNode->SetName(path);
		//}
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
				SetPlacePath((Model*)model->GetSubModels().at(i), model);
			}
		}

	}
}

void ModelAssemblyHelper::SetPlaceMatrix(Model* model, const Matrix3x4& plcMat)
{
	if (model)
	{
		//ModelNode * tempNode = GetModelNode(model);
		//if(tempNode)
		//{
		//	tempNode->SetOrigPlcMatrix(plcMat);
		//}
	}
}

void ModelAssemblyHelper::ComputePlaceMatrix(Model* model)
{
	//ModelNode * modelNode = GetModelNode(model);
	//ModelNode * parentNode = GetParentNode(model);
	//if(modelNode  && parentNode)
	//{

	//	Matrix3x4 plcMat =
	//			parentNode->GetWorldTransform().Inverse() *modelNode->GetWorldTransform();
	//	LOGI("=====================================");
	//	LOGI("%s",parentNode->GetWorldTransform().Inverse().ToString().c_str());
	//	LOGI("=====================================");
	//	LOGI("=====================================");
	//	LOGI("%s",modelNode->GetWorldTransform().ToString().c_str());
	//	LOGI("=====================================");
	//	SetPlaceMatrix(model, plcMat);
	//}
	//else
	//{
	//	LOGI("parentNode is NULL");
	//}
}

void ModelAssemblyHelper::ComputePlaceMatrix(Model* sourceModel, Model* desModel)
{
	//1 获取目标装配相对世界坐标系的矩阵a
	Matrix3x4 parentWorldMat = ModelAssemblyHelper::GetWorldMatrix(desModel);
	//2 求取矩阵a的逆装配矩阵b
	Matrix3x4 inverseParentWorldMat = parentWorldMat.Inverse();
	//3 获取源装配相对于世界坐标系的矩阵c
	Matrix3x4 sourceWorldMat = ModelAssemblyHelper::GetWorldMatrix(sourceModel);
	//4 求源装配相对于目标装配的矩阵d
	Matrix3x4 tempMat;
	tempMat = Matrix3x4::IDENTITY;
	Matrix3x4 sourcePlaceMatrix = inverseParentWorldMat * (tempMat * sourceWorldMat);
	//5 将矩阵设置给源装配
	sourceModel->SetOrigPlcMatrix(sourcePlaceMatrix);
}

void ModelAssemblyHelper::SetWorldMatrix(Model* model, const Matrix3x4& wldMat)
{
	if (model)
	{
		/*ModelNode * tempNode =GetModelNode(model);
		if(tempNode)
		{
			tempNode->SetWorldTransform(wldMat);
		}*/
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
	/*ModelNode* sourceModelNod = GetModelNode(model);
	for(int i = 0;i<model->GetSubModels().size();i++)
	{
		PrintPlcID((Model*)model->GetSubModels().at(i));
	}
	LOGI("sourceModelNod name == %s",sourceModelNod->GetName().c_str());*/
}

bool ModelAssemblyHelper::InsertInLast(View * view,Model* sourceModel, Model* desModel)
{
	LOGI(" ModelManager::InsertBefore BEGIN");
	bool msg =false;
	if (sourceModel && desModel)
	{
		//LOGI(" ModelManager::InsertBefore 1");
		//string desModelName = GetPlacePath(desModel);

		//ModelNode* sourceModelNod = GetModelNode(sourceModel);

		//ModelNode* desModelNod = GetModelNode(desModel);

	//	LOGI("sourceModelNod pointer = %p,desModelNod pointer = %p",sourceModelNod,desModelNod);

		//if (sourceModelNod && desModelNod)
		{
		//	LOGI(" ModelManager::InsertBefore 2");
/*			//判断子模型数量，若为0则是零件，不为零则为装配
			if(desModel->GetSubModelCount()<1)
			{
//				GroupNode* pare = (GroupNode*) desModel->GetParent();

				pare->AddChildInSameLevelBefore(sourceModelNod, desModelName);

				msg =  ASSEMBLYMSG::ASSAMBLY_INS_BEFO_ERR;
			}
			else
			{
			*/	//若原零件是个装配，那么把要插入的零件之间放入最后
			int tempId = desModel->GetUseablePlcId();
			SceneManager* scene = view->GetSceneManager();
			scene->Lock();
				//desModelNod->AddChild(sourceModelNod);
				desModel->AddSubModel(sourceModel);
				sourceModel->SetPlcId(tempId);
				sourceModel->ClearPlcPath(true);
				view->GetSceneManager()->AsynUpdateModelCacheInfo(sourceModel,true);
//				PrintPlcID(sourceModel);
            //设置ocTree区域
                view->GetSceneManager()->RequestUpdateWhenSceneBoxChanged();    //将模型加入装配中
/*			}*/

              scene->UnLock();
		}

		msg =  true;
	}
	else
	{
		//LOGI("ModelManager::InsertBefor ASSEMBLYMSG::ASSAMBLY_INS_BEFO_ERR");
		msg = false;
	}
	LOGI(" ModelManager::InsertBefore END %s ",msg);
	return msg;

}

bool ModelAssemblyHelper::InsertInPos(View* view, Model* sourceModel, int iId, Model* parentModel)
{
	LOGI(" ModelManager::InsertInPos BEGIN");
	bool bRet = false;
	if (!view || !sourceModel || !parentModel)
		return bRet;
	//以下的判断有没有必要？若加上则应该添加parentModel子ID计数减一的代码
	//if (iPos > parentModel->GetUseablePlcId())
	//	return bRet;
	SceneManager* scene = view->GetSceneManager(); 
	scene->Lock();
	parentModel->AddSubModel(sourceModel);
	sourceModel->SetPlcId(iId);
	sourceModel->ClearPlcPath(true);
	view->GetSceneManager()->AsynUpdateModelCacheInfo(sourceModel, true);
	//设置ocTree区域
	view->GetSceneManager()->RequestUpdateWhenSceneBoxChanged();    //将模型加入装配中
	scene->UnLock();
	bRet = true;
	LOGI(" ModelManager::InsertInPos END");
	return bRet;
}

//bool ModelAssemblyHelper::DetachModel(Model* model)
//{
//	bool msg = false;
//	if (model)
//	{
//        //model->AddRef();
//		//SceneManager* scene = view->GetSceneManager(); scene->Lock();
//		
//		//scene->UnLock();
//		msg =  true;
//	}
//	else
//	{
//		msg =  false;
//	}
//
//	return msg;
//}

bool ModelAssemblyHelper::DeleteModel(View* view, Model* model)
{
	bool msg = false;
	if (model)
		{
            SceneManager* scene = view->GetSceneManager();
			msg = view->GetSceneManager()->AsynRemoveModelFromeScene(model->GetParent(), model);
			msg =  true;
		}
		else
		{
			msg =  false;
		}

	return msg;
}

Model* ModelAssemblyHelper::GetModelFromId(View* view, int modelId)
{
	Model * ret = NULL;
	IShape * modelShp = view->GetShape(modelId);
	if(modelShp&&modelShp->GetType() == SHAPE_MODEL)
	{
		//LOGI("ModelManager::GetModelFromId complete");
		ret = (Model * )modelShp;
	}
	return ret;
}

//ModelNode* ModelAssemblyHelper::GetParentNode(Model* model)
//{
//	ModelNode * ret = NULL;
//
//	if(model)
//	{
//		SceneNode * node = model->GetSceneNode()->GetParent();
//		if(node)
//		{
//			SceneNode * tempRet = node->GetParent();
//			if(tempRet && tempRet->GetType()==MODEL_NODE)
//			{
//				ret = (ModelNode *) (tempRet);
//			}
//		}
//
//	}
//
//	return ret;
//}

//void ModelAssemblyHelper::FillAssembly(View* view, Model* model)
//{
//	SModelFileInfo* fileInfo = model->GetFileInfo();
//	if (fileInfo) {
//		string curFilePath = "";// fileInfo->GetPath();
//		curFilePath = FileHelper::GetUnionStylePath(curFilePath);
//
//		LOGI("curFilePath is ==%s", curFilePath.c_str());
//		Reader* reader = Reader::GetReader(curFilePath);
//
//		Model* subModel = static_cast<Model*>(reader->GetModel());
//		SceneManager* scene = view->GetSceneManager();
//
//		int defaultPlcId = model->GetSubModelCount() - 1;
//
//		if (defaultPlcId < 0)
//		{
//			defaultPlcId = 0;
//		}
//
//		{			//将模型加入装配中
//			SceneNode* sceneNode = model->GetSceneNode()->GetParent();
//
//			//if (subModel && sceneNode && sceneNode->GetType() == MODEL_NODE)
//			//{
//			//	ModelNode* modelNode = (ModelNode*)sceneNode;
//			//	GroupNode* modelGroup = scene->CreateModelNodes(subModel, modelNode->GetName(), defaultPlcId);
//			//	//对场景进行锁定
//			//	SceneManager* scene = view->GetSceneManager();scene->Lock();
//			//	model->AddSubModel(subModel);
//			//	modelNode->AddChild(modelGroup);
//			//	scene->UnLock();
//			//}
//		}
//
//		delete reader;
//
//		model->ClearFileInfo();
//	}	
//}

Model* ModelAssemblyHelper::AddModel(View * view,const string& filePath)
{
	string curFilePath = filePath;
	curFilePath = FileHelper::GetUnionStylePath(curFilePath);

	LOGI("curFilePath is ==%s",curFilePath.c_str());
	Reader* reader = Reader::GetReader(curFilePath);
    reader->SetView(view);
	Model* model = static_cast<Model*>(reader->GetModel());
    string ext = FileHelper::GetExt(curFilePath);
    if (model) {
        if (ext == "svlx") {
            vector<Model*> subModelArray;
            model->GetAllSubModels(subModelArray);
            subModelArray.push_back(model);
            for (int i = 0; i < subModelArray.size(); i++) {
                Model *subModel = subModelArray.at(i);
                ((SVL2AsynReader*)reader)->FillModelMesh(view, subModel);
				//view->GetSceneManager()->AsynUpdateModelCacheInfo(subModel, true);
            }
			((SVL2AsynReader*)reader)->AddGeoAttribute(model, curFilePath, view);
        }
		model->AddRef();
        if (reader) {
            delete reader;
            reader = NULL;
        }
    }
	return model;
}

//ModelNode* ModelAssemblyHelper::GetModelNode(Model* model)
//{
//	//LOGI("ModelManager::GetModelNode BEGIN");
//	ModelNode* ret = NULL;
//	if (model)
//	{
//		SceneNode* tempRet = model->GetSceneNode()->GetParent();
//		if (tempRet)
////			LOGI("tempRet->GetType()== %d", tempRet->GetType());
//		if (tempRet && tempRet->GetType() == MODEL_NODE)
//		{
//			ret = (ModelNode *) tempRet;
//		}
//	}
//	//LOGI("ModelManager::GetModelNode END");
//	return ret;
//}
/**
 * @brief 旋转装配模型
 * @param model 要旋转的模型
 * @param quat 旋转量
 */
void ModelAssemblyHelper::RotateAssemblyModel(Model* model, Quaternion& quat)
{	

	Model* parent = model->GetParent();
	Matrix3x4 modelWMat;
	modelWMat = model->GetWorldTransform();
	Matrix3x4 pareWMat; //父节点世界转换矩阵
	pareWMat = parent ? parent->GetWorldTransform() : pareWMat;

	Vector3 boxCenter = GetModelWorldBoundingBox(model).Center();
//	LOGI("boxCenter === %s",boxCenter.Tostring().c_str());
	Matrix3x4 trans1, trans2, rot1;
	trans1.MultiTranslate(-boxCenter);
	rot1.MultiRotatiton(quat);
	trans2.MultiTranslate(boxCenter);
	Matrix3x4 tempmat = trans2 * rot1 * trans1;
	Matrix3x4 plcMat = pareWMat.Inverse() * tempmat * modelWMat;
	model->SetPlaceMatrix(plcMat);
}

/**
* @brief 旋转装配模型
* @param model 要旋转的模型
* @param quat 旋转量
* @param pivotCenter 旋转中心点
*/
void ModelAssemblyHelper::RotateAssemblyModel(Model* model, Quaternion& quat, Vector3& pivotCenter)
{

	Model* parent = model->GetParent();
	Matrix3x4 modelWMat;
	modelWMat = model->GetWorldTransform();
	Matrix3x4 pareWMat; //父节点世界转换矩阵
	pareWMat = parent ? parent->GetWorldTransform() : pareWMat;

	Matrix3x4 trans1, trans2, rot1;
	trans1.MultiTranslate(-pivotCenter);
	rot1.MultiRotatiton(quat);
	trans2.MultiTranslate(pivotCenter);
	Matrix3x4 tempmat = trans2 * rot1 * trans1;
	Matrix3x4 plcMat = pareWMat.Inverse() * tempmat * modelWMat;
	model->SetPlaceMatrix(plcMat);
}

/**
 * @brief 平移装配模型
 * @param model 要平移的模型
 * @param move  平移量
 */
void ModelAssemblyHelper::TranslateAssemblyModel(Model* model, Vector3& move)
{
	if (!model)
	{
		return;
	}

	Model* parent = model->GetParent();
	Matrix3x4 modelWMat;	
	modelWMat = model->GetWorldTransform();	
	Matrix3x4 pareWMat; //父节点世界转换矩阵
	pareWMat = parent ? parent->GetWorldTransform() : pareWMat;
	Matrix3x4 tempmat;
	tempmat.MultiTranslate(move);
	Matrix3x4 plcMat = pareWMat.Inverse() * (tempmat * modelWMat);
	model->SetPlaceMatrix(plcMat);
}


void ModelAssemblyHelper::RequestShowAllModel(SceneManager* scene)
{
	scene->RequestUpdateWhenSceneBoxChanged();
	scene->OptimizeCameraView(false);
}

void ModelAssemblyHelper::RequestShowAllAfterAddModel(SceneManager* scene, Model* addModel)
{
	if (!addModel)
	{
		return;
	}

	if (IsModelInScene(scene,addModel))
	{
		scene->GetSceneBox().Clear();
		RequestShowAllModel(scene);
	}
}

bool ModelAssemblyHelper::IsModelInScene(SceneManager* scene, Model* model)
{
	bool ret = false;
	if (!model)
	{
		return ret;
	}

	BoundingBox modelBox;
	modelBox = model->GetWorldBoundingBox();
	BoundingBox& sceneBox = scene->GetSceneBox();
	if (sceneBox.IsInside(modelBox) != INSIDE)
	{
		ret = true;
	}

	return ret;
}

/**
 * @brief 缩放装配模型
 * @param model 要缩放的模型
 * @param scale 缩放量
 */
void ModelAssemblyHelper::ScaleAssemblyModel(Model* model, Vector3& scale)
{
	Model* parent = model->GetParent();
	Matrix3x4 modelWMat;
	modelWMat = model->GetWorldTransform();
	Matrix3x4 pareWMat; //父节点世界转换矩阵
	pareWMat = parent ? parent->GetWorldTransform() : pareWMat;

	Vector3 boxCenter =GetModelWorldBoundingBox(model).Center();
	Matrix3x4 trans1, trans2, rot1;
	trans1.MultiTranslate(-boxCenter);
	rot1.MultiScale(scale);
	trans2.MultiTranslate(boxCenter);
	Matrix3x4 tempmat = trans2 * rot1 * trans1;
	Matrix3x4 plcMat = pareWMat.Inverse() * tempmat * modelWMat;
	model->SetPlaceMatrix(plcMat);
}
 
BoundingBox ModelAssemblyHelper::GetModelWorldBoundingBox(Model * model)
{
	BoundingBox center;

	if (model->GetType() == SHAPETYPE::SHAPE_LIGHT_BASE ||
		model->GetType() == SHAPETYPE::SHAPE_LIGHT_DIRECTIONAL ||
		model->GetType() == SHAPETYPE::SHAPE_LIGHT_SPOT ||
		model->GetType() == SHAPETYPE::SHAPE_LIGHT_POINT)
	{
		SignModel* signModel = static_cast<SignModel*>(model);

		M3D::ImageModel* imageModel = signModel->GetSimpleSignModel();
		if (imageModel)
		{
			center = imageModel->GetWorldBoundingBox();
		}
	}
	else
	{
		center = model->GetTotalWorldBoundingBox();
	}

	return center;
}

}

