/*
 * AssembleManager.cpp
 *
 *  Created on: 2015-12-24
 *      Author: CDJ
 */

#include "m3d/SceneManager.h"
#include "m3d/utils/ShapeHelper.h"
#include "m3d/SceneManager.h"
#include "sview/utils/ViewHelper.h"
#include "m3d/extras/modelmanager/ModelManager.h"
#include "m3d/scene/ShapeNode.h"
#include "m3d/scene/GroupNode.h"
#include "m3d/model/Body.h"
#include "m3d/model/Face.h"
#include "m3d/model/MeshData.h"
#include "m3d/model/ShapeSet.h"
#include "m3d/extras/OperationHistoryManager.h"
#include "m3d/Handler/TranslateAxisDragger.h"
#include "m3d/Handler/TranslateMinusAxisDragger.h"
#include "m3d/Handler/RotateCylinderAxisDragger.h"
#include "m3d/Handler/ScaleAxisDragger.h"
#include "m3d/Handler/DraggerManager.h"
#include "m3d/extras/modelmanager/DraggerCallbacks.h"
#include "m3d/extras/modelmanager/ExplosionDragerCallBack.h"
#include "m3d/scene/SectionNode.h"


using namespace M3D;
using namespace SVIEW;
namespace M3D
{

DraggerManager::DraggerManager(View * view):Object()
{
	m_commandHistoryManager = NULL;
	m_view = view;
	m_dragger = NULL;
}

DraggerManager::~DraggerManager()
{
	ReleaseMe(m_commandHistoryManager);
}

OperationHistoryManager* DraggerManager::GetCommandHistoryManager()
{
	return m_commandHistoryManager;
}
/************************************************************************/
/* 爆炸功能拖拽器的绑定                                                 */
/************************************************************************/

Dragger* DraggerManager::BindExplosionDragger(vector<Model*> models, int explosionType)
{
	m_view->GetSceneManager()->Lock();
	Dragger* dragger = NULL;
	dragger = BindExplosionStdDragger(models, explosionType);
	m_view->GetSceneManager()->UnLock();
	m_dragger = dragger;
	return dragger;
}
Dragger* DraggerManager::BindExplosionDragger(vector<Model*> models,int explosionType, Vector3 vect)
{
	m_view->GetSceneManager()->Lock();
	Dragger* dragger = NULL;
	dragger = BindExplosionAxisDragger(models,explosionType, vect);
	m_view->GetSceneManager()->UnLock();
	m_dragger = dragger;
	return dragger;
}
 
Dragger* DraggerManager::BindDragger(vector<Model*> models, int draggerType, bool bGloal)
{
	m_view->GetSceneManager()->Lock();
	Dragger* dragger = NULL;
	switch (draggerType)
	{
	case 1:
		dragger = BindAxisDragger(models, bGloal);
		break;
	case 2:
		dragger = BindRotateDragger(models, bGloal);
		break;
	case 3:
		dragger = BindScaleDragger(models);
		break;
	case 4:
		dragger = bindMinusAxisDragger(models);
	default:
		break;
	}
	m_view->GetSceneManager()->UnLock();
	m_dragger = dragger;
	return dragger;
}


/*
爆炸拖拽器的绑定:标准坐标轴方向
*/
Dragger* DraggerManager::BindExplosionStdDragger(vector<Model*> models, int explosionType)
{
	TranslateAxisDragger* explosioinAixsDragger = NULL;
	if (m_view)
	{
		//场景包围盒的参数，拖拽器的位置

		BoundingBox boundingBox;
		if (models.size() == 0)
		{
			boundingBox = m_view->GetSceneManager()->GetSceneBox();
		}
		else
		{
			boundingBox = ((Model*)models[0])->GetWorldBoundingBox();
			for (int i = 1; i < models.size(); i++)
			{
				boundingBox.Merge(((Model*)models[i])->GetWorldBoundingBox());
			}
		}

		Vector3 position = (boundingBox.m_max + boundingBox.m_min)*0.5f;
		explosioinAixsDragger = m_view->GetSceneManager()->GetHandlerGroup()->GetTransformHandler();
		explosioinAixsDragger->SetXZPlaneVisible(false);
		explosioinAixsDragger->SetYZPlaneVisible(false);
		explosioinAixsDragger->SetXYPlaneVisible(false);
		if (explosionType == 0)
		{
			explosioinAixsDragger->SetYAxisVisible(false);
			explosioinAixsDragger->SetZAxisVisible(false);
		}
		else
		{
			explosioinAixsDragger->SetYAxisVisible(true);
			explosioinAixsDragger->SetZAxisVisible(true);
		}
		explosioinAixsDragger->SetName("TranslateAxisDragger");
		explosioinAixsDragger->SetVisible(true);
		explosioinAixsDragger->SetWorldPosition(position);

		ExplosionDragerCallBack* draggerCallback = new ExplosionDragerCallBack();
		draggerCallback->m_view = m_view;
		draggerCallback->boundingBox = boundingBox;
		draggerCallback->explosionType = explosionType;
		draggerCallback->arrayModels = models;
		draggerCallback->AddRef();
		explosioinAixsDragger->addDraggerCallback(draggerCallback);

		draggerCallback->Release();
	}

	return explosioinAixsDragger;
}

/*指示器拖拽器*/

Dragger* DraggerManager::BindIndicatorDragger(Vector3 direction, Vector3 pos) 
{
	TranslateMinusAxisDragger* minusAxisDragger = NULL;
	if (m_view)
	{

		minusAxisDragger = m_view->GetSceneManager()->GetHandlerGroup()->GetTransMinusformHandler();
		minusAxisDragger->SetName("TranslateMinusAxisDragger");
		minusAxisDragger->SetVisible(true);
		minusAxisDragger->SetMinusXAxisVisible(false);
		minusAxisDragger->SetWorldPosition(pos);
		minusAxisDragger->SetOrientation(direction);
		return minusAxisDragger;
	}
    return NULL;
}

/*
爆炸拖拽器的绑定:指定拖拽器方向
*/
Dragger* DraggerManager::BindExplosionFreedomDragger(vector<Model*> models, int explosionType, Vector3 direction,Vector3 pos) {
	
	TranslateMinusAxisDragger* minusAxisDragger = NULL;
	if (m_view)
	{
		//场景包围盒的参数，拖拽器的位置

		BoundingBox boundingBox;
		if (models.size() == 0)
		{
			boundingBox = m_view->GetSceneManager()->GetSceneBox();
		}
		else
		{
			boundingBox = ((Model*)models[0])->GetWorldBoundingBox();
			for (int i = 1; i < models.size(); i++)
			{
				boundingBox.Merge(((Model*)models[i])->GetWorldBoundingBox());
			}
		}

		Vector3 position = (boundingBox.m_max + boundingBox.m_min)*0.5f;

		minusAxisDragger = m_view->GetSceneManager()->GetHandlerGroup()->GetTransMinusformHandler();
		minusAxisDragger->SetName("TranslateMinusAxisDragger");
		minusAxisDragger->SetVisible(true);
		minusAxisDragger->SetMinusXAxisVisible(false);
		if (pos == Vector3(0, 0, 0))
		{
			minusAxisDragger->SetWorldPosition(position);
		}
		else
		{
			minusAxisDragger->SetWorldPosition(pos);
		}
		if (direction !=  Vector3(0, 0, 0))
		{
			minusAxisDragger->SetOrientation(direction);
		}
		ExplosionDragerCallBack* draggerCallback = new ExplosionDragerCallBack();
		draggerCallback->m_view = m_view;
		draggerCallback->boundingBox = boundingBox;
		draggerCallback->explosionType = explosionType;
		draggerCallback->arrayModels = models;
		draggerCallback->AddRef();
		minusAxisDragger->addDraggerCallback(draggerCallback);
		draggerCallback->Release();

		return minusAxisDragger;

	}
    return NULL;
}
Dragger* DraggerManager::BindExplosionAxisDragger(vector<Model*> models,int explosionType, Vector3 vect)
{
	TranslateAxisDragger* explosioinAixsDragger = NULL;
	TranslateMinusAxisDragger* minusAxisDragger = NULL;
	if (m_view)
	{
		//场景包围盒的参数，拖拽器的位置

		BoundingBox boundingBox;
		if (models.size() == 0)
		{
			boundingBox = m_view->GetSceneManager()->GetSceneBox();
		}
		else
		{
			boundingBox = ((Model*)models[0])->GetWorldBoundingBox();
			for (int i = 1; i < models.size(); i++)
			{
				boundingBox.Merge(((Model*)models[i])->GetWorldBoundingBox());
			}
		}

		Vector3 position = (boundingBox.m_max + boundingBox.m_min)*0.5f;
		if (vect == Vector3(0,0,0))
		{
			explosioinAixsDragger = m_view->GetSceneManager()->GetHandlerGroup()->GetTransformHandler();
			explosioinAixsDragger->SetXZPlaneVisible(false);
			explosioinAixsDragger->SetYZPlaneVisible(false);
			explosioinAixsDragger->SetXYPlaneVisible(false);
			if (explosionType == 0)
			{
				explosioinAixsDragger->SetYAxisVisible(false);
				explosioinAixsDragger->SetZAxisVisible(false);
			}
			else
			{
				explosioinAixsDragger->SetYAxisVisible(true);
				explosioinAixsDragger->SetZAxisVisible(true);
			}
			explosioinAixsDragger->SetName("TranslateAxisDragger");
			explosioinAixsDragger->SetVisible(true);
			explosioinAixsDragger->SetWorldPosition(position);
		}
		else
		{
// 			Face* face = (Face*)m_view->GetShape(shapeId);
// 			Mesh* mesh = face->GetData();
// 			VertexSet*  meshVertexSet = mesh->GetRefMesh();
// 			std::vector<Vector3>* pnts = meshVertexSet->GetPositionArray();
// 			Vector3 normal;
// 			if (pnts->size() > 3)
// 			{
// 				Plane plane = Plane(pnts->at(0), pnts->at(1), pnts->at(2));
// 				normal = plane.m_normal;
// 			}
			minusAxisDragger = m_view->GetSceneManager()->GetHandlerGroup()->GetTransMinusformHandler();
			minusAxisDragger->SetName("TranslateMinusAxisDragger");
			minusAxisDragger->SetWorldPosition(position);
			minusAxisDragger->SetVisible(true);
			minusAxisDragger->SetMinusXAxisVisible(false);
			minusAxisDragger->SetOrientation(vect);
		}

		ExplosionDragerCallBack* draggerCallback = new ExplosionDragerCallBack();
		draggerCallback->m_view = m_view;
		draggerCallback->boundingBox = boundingBox;
		draggerCallback->explosionType = explosionType;
		draggerCallback->arrayModels = models;
		draggerCallback->AddRef();
		if (vect == Vector3(0, 0, 0))
		{
			explosioinAixsDragger->addDraggerCallback(draggerCallback);
		}
		else 
		{
			minusAxisDragger->addDraggerCallback(draggerCallback);
		}
		
		draggerCallback->Release();
	}
	if (vect == Vector3(0, 0, 0))
	{
		return explosioinAixsDragger;
	}
	else
	{
		return minusAxisDragger;
	}
	
}

bool DraggerManager::MoveIndicatorDragger(Dragger* dragger, Vector3 direction, Vector3 pos) {

	if (dragger) 
	{
		dragger->SetWorldPosition(pos);
		((TranslateMinusAxisDragger*)dragger)->SetOrientation(direction);
		return true;
	}
	return false;
}


bool DraggerManager::UnBindDragger(Dragger* dragger)
{
	if (dragger)
	{
		m_view->GetSceneManager()->Lock();
		dragger->ClearDraggerCallbacks();
		dragger->SetVisible(false);
		m_view->GetSceneManager()->UnLock();
	}
	m_dragger = NULL;
	return true;
}
Dragger* DraggerManager::BindAxisDragger(vector<Model*> models, bool bGloal/* = true*/)
{
	TranslateAxisDragger* translateAixsDragger = NULL;

	int nModelCount = models.size();
	if (nModelCount <= 0)
		return NULL;
	Model* pLastModel = models[nModelCount - 1];
	if (pLastModel == NULL)
		return NULL;
	if (m_view)
	{
		translateAixsDragger = m_view->GetSceneManager()->GetHandlerGroup()->GetTransformHandler();
		translateAixsDragger->SetName("TranslateAxisDragger");
		translateAixsDragger->SetVisible(true);
		Vector3 position = GetBindCenter(pLastModel);
		translateAixsDragger->SetWorldPosition(position);
		if (!bGloal)
		{
			translateAixsDragger->SetRotation(pLastModel->GetWorldRotation());
		}
		
		ModelDraggerCallback* draggerCallback = new ModelDraggerCallback();
		for (int i = 0; i < nModelCount; i++)
		{
			if (models[i] == NULL)
				continue;
			draggerCallback->AddModel(models[i]);
		}
		draggerCallback->AddRef();
		translateAixsDragger->addDraggerCallback(draggerCallback);
		draggerCallback->Release();
	}
	return translateAixsDragger;
}

/************************************************************************/
/* 有正负两个方向的线性拖拽器                                           */
/************************************************************************/
Dragger* DraggerManager::bindMinusAxisDragger(vector<Model*> models)
{
	TranslateMinusAxisDragger* minusAixsDragger = NULL;
	
	int nModelCount = models.size();
	if (nModelCount <= 0)
		return NULL;
	Model* pLastModel = models[nModelCount - 1];
	if (pLastModel == NULL)
		return NULL;
	Vector3 position = GetBindCenter(pLastModel);
	if (m_view)
	{
		minusAixsDragger =  m_view->GetSceneManager()->GetHandlerGroup()->GetTransMinusformHandler();
		minusAixsDragger->SetName("TranslateMinusAxisDragger");
		minusAixsDragger->SetVisible(true);
		minusAixsDragger->SetWorldPosition(position);
		//方向设置1为X轴2为Y轴3为Z轴
		//minusAixsDragger->SetOrientation(models.size()%3);
		ModelDraggerCallback* draggerCallback = new ModelDraggerCallback();
		for (int i = 0; i < nModelCount; i++)
		{
			if (models[i] == NULL)
				continue;
			draggerCallback->AddModel(models[i]);
		}
		draggerCallback->AddRef();
		minusAixsDragger->addDraggerCallback(draggerCallback);
		draggerCallback->Release();
	}
	return minusAixsDragger;
}

Dragger* DraggerManager::BindScaleDragger(vector<Model*> models)
{
	ScaleAxisDragger* scaleAixsDragger = NULL;
	int nModelCount = models.size();
	if (nModelCount <= 0)
		return NULL;      
	Model* pLastModel = models[nModelCount - 1];
	if (pLastModel == NULL)
		return NULL;
	Vector3 position = GetBindCenter(pLastModel);
	if (m_view)
	{
		scaleAixsDragger = m_view->GetSceneManager()->GetHandlerGroup()->GetScaleAxisDragger();
		scaleAixsDragger->SetName("SCaleAxisDragger");
		scaleAixsDragger->SetVisible(true);
		//translateAixsDragger->SetOrigScale(Vector3(scale, scale, scale));
		//translateAixsDragger->SetNeedScale(true);
		scaleAixsDragger->SetWorldPosition(position);
		ModelDraggerCallback* draggerCallback = new ModelDraggerCallback();
		for (int i = 0; i < nModelCount; i++)
		{
			if (models[i] == NULL)
				continue;
			draggerCallback->AddModel(models[i]);
		}
		draggerCallback->SetScene(m_view->GetSceneManager());
		draggerCallback->AddRef();
		scaleAixsDragger->addDraggerCallback(draggerCallback);
		draggerCallback->Release();
	}
	return scaleAixsDragger;
}

Dragger* DraggerManager::BindRotateDragger(vector<Model*> models, bool bGloal/* = true*/)
{
	RotateCylinderAxisDragger* rotateAixsDragger = NULL;
	int nModelCount = models.size();
	if (nModelCount <= 0)
		return NULL;
	Model* pLastModel = models[nModelCount - 1];
	if (pLastModel == NULL)
		return NULL;
	if (m_view)
	{
		rotateAixsDragger = m_view->GetSceneManager()->GetHandlerGroup()->GetRotateCylinderAxisDragger();
		rotateAixsDragger->SetName("RotateCylinderAxisDragger");
		rotateAixsDragger->SetVisible(true);
		//translateAixsDragger->SetNeedScale(true);
		//translateAixsDragger->SetOrigScale(Vector3(scale, scale, scale));
		Vector3 position = GetBindCenter(pLastModel);
		rotateAixsDragger->SetWorldPosition(position);
		if (!bGloal)
		{
			rotateAixsDragger->SetRotation(pLastModel->GetWorldRotation());
		}
		ModelDraggerCallback* draggerCallback = new ModelDraggerCallback();
		for (int i = 0; i < nModelCount; i++)
		{
			if (models[i] == NULL)
				continue;
			draggerCallback->AddModel(models[i]);
		}
		draggerCallback->SetScene(m_view->GetSceneManager());
		draggerCallback->AddRef();
		rotateAixsDragger->addDraggerCallback(draggerCallback);
		draggerCallback->Release();
	}
	return rotateAixsDragger;
}

M3D::Vector3 DraggerManager::GetBindCenter(Model* model)
{
	Vector3 center = Vector3::ZERO;

	 if (model->GetType() == SHAPETYPE::SHAPE_LIGHT_BASE||
		model->GetType() == SHAPETYPE::SHAPE_LIGHT_DIRECTIONAL||
		model->GetType() == SHAPETYPE::SHAPE_LIGHT_SPOT ||
		model->GetType() == SHAPETYPE::SHAPE_LIGHT_POINT)
	{
		SignModel* signModel = static_cast<SignModel*>(model);

		M3D::ImageModel* imageModel = signModel->GetSimpleSignModel();
		if (imageModel)
		{
			center = imageModel->GetWorldBoundingBox().Center();
		}
	 }
	 else
	 {
		center = model->GetTotalWorldBoundingBox().Center();
	 }
	return center;
}

}
 
 /* namespace M3D */
