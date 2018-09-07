/*
 * AssembleManager.cpp
 *
 *  Created on: 2015-12-24
 *      Author: CDJ
 */

#include "m3d/SceneManager.h"
#include "m3d/utils/ShapeHelper.h"
#include "m3d/scenemanager.h"
#include "sview/utils/ViewHelper.h"
#include "m3d/extras/modelmanager/ModelManager.h"
#include "m3d/scene/ShapeNode.h"
#include "m3d/scene/GroupNode.h"
#include "m3d/model/Body.h"
#include "m3d/model/ShapeSet.h"
#include "m3d/extras/OperationHistoryManager.h"
#include "m3d/handler/TranslateAxisDragger.h"
#include "m3d/handler/RotateCylinderAxisDragger.h"
#include "m3d/handler/ScaleAxisDragger.h"
#include "m3d/handler/DraggerManager.h"
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

Dragger* DraggerManager::BindExplosionDragger(vector<Model*> models,int explosionType)
{
	m_view->GetSceneManager()->Lock();
	Dragger* dragger = NULL;
	dragger = BindExplosionAxisDragger(models,explosionType);
	m_view->GetSceneManager()->UnLock();
	m_dragger = dragger;
	return dragger;
}
 
Dragger* DraggerManager::BindDragger(vector<Model*> models, int draggerType)
{
	m_view->GetSceneManager()->Lock();
	Dragger* dragger = NULL;
	switch (draggerType)
	{
	case 1:
		dragger = BindAxisDragger(models);
		break;
	case 2:
		dragger = BindRotateDragger(models);
		break;
	case 3:
		dragger = BindScaleDragger(models);
		break;
	default:
		break;
	}
	m_view->GetSceneManager()->UnLock();
	m_dragger = dragger;
	return dragger;
}

/*
±¬Õ¨ÍÏ×§Æ÷µÄ°ó¶¨
*/
Dragger* DraggerManager::BindExplosionAxisDragger(vector<Model*> models,int explosionType)
{
	TranslateAxisDragger* translateAixsDragger = NULL;

	if (m_view)
	{
		//³¡¾°°üÎ§ºÐµÄ²ÎÊý£¬ÍÏ×§Æ÷µÄÎ»ÖÃ

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

		translateAixsDragger = m_view->GetSceneManager()->GetHandlerGroup()->GetTransformHandler();
		translateAixsDragger->SetName("TranslateAxisDragger");
		translateAixsDragger->SetVisible(true);
		translateAixsDragger->SetWorldPosition(position);

		ExplosionDragerCallBack* draggerCallback = new ExplosionDragerCallBack();
		draggerCallback->m_view = m_view;
		draggerCallback->boundingBox = boundingBox;
		draggerCallback->explosionType = explosionType;
		draggerCallback->arrayModels = models;
		draggerCallback->AddRef();
		translateAixsDragger->addDraggerCallback(draggerCallback);
		draggerCallback->Release();
	}
	return translateAixsDragger;
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
 

Dragger* DraggerManager::BindAxisDragger(vector<Model*> models)
{
	TranslateAxisDragger* translateAixsDragger = NULL;
	int nModelCount = models.size();
	if (nModelCount <= 0)
		return NULL;
	Model* pLastModel = models[nModelCount - 1];
	if (pLastModel == NULL)
		return NULL;
	Vector3 position = GetBindCenter(pLastModel);
	if (m_view)
	{
		translateAixsDragger =  m_view->GetSceneManager()->GetHandlerGroup()->GetTransformHandler();
		translateAixsDragger->SetName("TranslateAxisDragger");
		translateAixsDragger->SetVisible(true);
		//translateAixsDragger->SetOrigScale(Vector3(scale,scale,scale));
		//translateAixsDragger->SetNeedScale(true);
		translateAixsDragger->SetWorldPosition(position);
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

Dragger* DraggerManager::BindRotateDragger(vector<Model*> models)
{
	RotateCylinderAxisDragger* rotateAixsDragger = NULL;
	int nModelCount = models.size();
	if (nModelCount <= 0)
		return NULL;
	Model* pLastModel = models[nModelCount - 1];
	if (pLastModel == NULL)
		return NULL;
	Vector3 position = GetBindCenter(pLastModel);
	if (m_view)
	{
		rotateAixsDragger = m_view->GetSceneManager()->GetHandlerGroup()->GetRotateCylinderAxisDragger();
		rotateAixsDragger->SetName("RotateCylinderAxisDragger");
		rotateAixsDragger->SetVisible(true);
		//translateAixsDragger->SetNeedScale(true);
		//translateAixsDragger->SetOrigScale(Vector3(scale, scale, scale));
		rotateAixsDragger->SetWorldPosition(position);
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
