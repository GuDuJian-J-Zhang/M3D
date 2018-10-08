#include "sview/extern/ViewOperator.h"
#include "m3d/action/CallBackAction.h"
#include "m3d/SceneManager.h"
#include "sview/views/Parameters.h"
#include "m3d/RenderManager.h"
#include "m3d/graphics/CameraNode.h"
#include "m3d/scene/ShapeNode.h"

using SVIEW::Parameters;

namespace M3D
{

ViewOperator::ViewOperator() :
		CallBackAction()
{

}

ViewOperator::~ViewOperator()
{

}

void ViewOperator::SetScene(SceneManager* scene)
{
	this->m_scene = scene;
}

SceneManager* ViewOperator::GetScene()
{
	return this->m_scene;
}

BoundingBox& ViewOperator::GetFitViewBox()
{
	return this->m_fitViewBox;
}

bool ViewOperator::FitView(SceneManager* scene)
{
	bool ret = false;
	LOGI("FitView start");
	if (scene)
	{
		CameraNode* camera = scene->GetCamera();
		Model* topModel = (Model*)scene->GetModel();
		if (camera && topModel)
		{
			LOGI("FitView step 1");
			camera->ReSet();
			ViewOperator* viewOperator = new ViewOperator();
			viewOperator->SetScene(scene);
			CallBackAction* computeBoxAction = new CallBackAction();
			computeBoxAction->SetActionData(viewOperator);
			computeBoxAction->SetActionFun(ViewOperator::ComputeFitViewBox);
			scene->ExecuteAction(computeBoxAction);

			delete computeBoxAction;

			BoundingBox& fitViewBox = viewOperator->GetFitViewBox();
			RenderManager* renderManager = scene->GetRenderManager();

			int screenHeight = renderManager->GetWindowHeight();
			int screenWidth = renderManager->GetWindowWidth();

			 float length = fitViewBox.Length();

			if (renderManager != NULL)
			{
				renderManager->GetCullerHelper().SetModelLength(
						fitViewBox.Length());
			}
            
            float width = 100;
            float height = 100;
            if (screenHeight <= screenWidth)
            {
                width = fitViewBox.Length();
                height = width * screenHeight / screenWidth;
            }
            else
            {
                height = fitViewBox.Length();
                width = height * screenWidth / screenHeight;
            }
   
			LOGI("FitView step 2");
			//camera->SetViewPort(0, 0, screenWidth, screenHeight);
			Vector3 center = fitViewBox.Center();

			center.m_z += fitViewBox.Length() * 1.5f;

			camera->SetWorldPosition(center);

			camera->SetOrthoSize(Vector2(width * 2, height * 2));

            //camera->SetNearClip(length * 0.1);
			//camera->SetFarClip(length * 3.5);

			float defaultZoom = Parameters::Instance()->m_DefaultZoomFactor;

			if (screenWidth < screenHeight)
			{
				defaultZoom = Parameters::Instance()->m_DefaultZoomFactor * 1.5;
			}

			camera->SetZoom(defaultZoom);

			camera->SetInitRotateCenter(fitViewBox.Center());

			//camera->SetFov(90);

			camera->LookAt(fitViewBox.Center(), Vector3(0, 1, 0), TS_WORLD);

			delete viewOperator;
		}

		ret = true;
	}
	LOGI("FitView end");
	return ret;
}
void ViewOperator::ComputeFitViewBox(void* data,SceneNode* node)
{
	if(node!= NULL && node->GetType() == SHAPE_NODE)
	{
		ViewOperator* viewOperator = (ViewOperator*)data;

		SceneManager* scene =(SceneManager*)viewOperator->GetScene();
		if(scene)
		{
			BoundingBox& sceneBox = viewOperator->GetFitViewBox();
			BoundingBox& shapeNodeBox = node->GetWorldBoundingBox();
			ShapeNode* shapeNode = (ShapeNode*)node;

			Shape* shape = shapeNode->GetShape();
			if(shape && shape->IsVisible() && shapeNodeBox.Defined())
			{
				sceneBox.Merge(shapeNodeBox);
			}
		}
	}
}
}
