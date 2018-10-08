/*
 * DraggerHandler.cpp
 *
 *  Created on: 2016-1-7
 *      Author: Administrator
 */

#include "sview/manipulator/DraggerHandler.h"
#include "m3d/graphics/cameranode.h"
#include "m3d/base/BoundingBox.h"
#include "m3d/base/Viewport.h"
#include <math.h>
#include "sview/views/Parameters.h"

#include "m3d/SceneManager.h"
#include "m3d/RenderManager.h"
#include "m3d/graphics/cameranode.h"
#include "m3d/utils/CullerHelper.h"
#include "SView/views/view.h"
#include "sview/extern/PerspectiveData.h"
#include "sview/extern/PerspectiveOperator.h"
#include "m3d/scene/ModelNode.h"
#include "sview/views/Selector.h"
#include "m3d/extras/modelmanager/ModelAssemblyHelper.h"
using namespace M3D;

namespace SVIEW
{
DraggerHandler::DraggerHandler()
{
	m_touchHandlerType = HANDLER_DRAGGER;
	m_axis = (TRANSAXIS)0;
	m_mode = (OPEMODE)0;
}

DraggerHandler::~DraggerHandler()
{
}

void DraggerHandler::InitCamera()
{
	return;
	if(this->m_SceneManager)
	{
		//this->m_SceneManager->ResetCamera();
		CameraNode* camera = m_SceneManager->GetCamera();
		camera->ReSet();
		camera->SetOrthographic(true);
		BoundingBox& pBoundingBox = this->m_SceneManager->GetSceneBox();
		Viewport viewport =  camera->GetViewPort();
		int screenHeight =viewport.GetRect().Height();
		int screenWidth = viewport.GetRect().Width();

		this->m_SceneManager->GetRenderManager()->WindowSizeChanged(screenWidth, screenHeight);


		float length = pBoundingBox.Length();

		Vector3 center =  pBoundingBox.Center();

        //平行投影
        if(camera->IsOrthographic())
        {
            center.m_z += pBoundingBox.Length()*8.5f;
        }
        else//垂直投影
        {
            center.m_z += pBoundingBox.Length()*1.5f;
        }

		camera->SetWorldPosition(center);

		camera->SetNearClip(length*0.1);
		camera->SetFarClip(length*15.5);

		float defaultZoom = Parameters::Instance()->m_DefaultZoomFactor;

		defaultZoom = defaultZoom * screenHeight/screenWidth;

		if(screenHeight > screenWidth )
		{
			defaultZoom = defaultZoom *0.5*(screenWidth*1.0/screenHeight);
		}

        camera->SetZoom(defaultZoom);

		camera->SetInitRotateCenter(pBoundingBox.Center());

		camera->SetFov(90);

		camera->LookAt(pBoundingBox.Center(),Vector3(0,1,0),TS_WORLD);

		this->m_TrackBall.SetRotateSpeed(2.0f);
       if(m_pView)
        {
		   m_pView->GetPerspectiveOperator()->Show(m_pView, this->GetDefaultView(),
                                                     true, false, false);
        }

        const IntRect& rect = camera->GetViewPort().GetRect();
        int width = rect.Width()>rect.Height()?rect.Height():rect.Width();
        this->m_TrackBall.SetTrackWindow(width,width);
	}
}

void DraggerHandler::InitModelViewCamera()
{
	return;
	if(this->m_SceneManager)
	{
		//this->m_SceneManager->ResetCamera();
		CameraNode* camera = m_SceneManager->GetCamera();
		camera->ReSet();
		camera->SetOrthographic(true);
		BoundingBox& pBoundingBox = this->m_SceneManager->GetSceneBox();
		Viewport viewport =  camera->GetViewPort();
		int screenHeight =viewport.GetRect().Height();
		int screenWidth = viewport.GetRect().Width();

		this->m_SceneManager->GetRenderManager()->WindowSizeChanged(screenWidth, screenHeight);


		float length = pBoundingBox.Length();

		Vector3 center =  pBoundingBox.Center();

        //平行投影
        if(camera->IsOrthographic())
        {
            center.m_z += pBoundingBox.Length()*8.5f;
        }
        else//垂直投影
        {
            center.m_z += pBoundingBox.Length()*1.5f;
        }

		camera->SetWorldPosition(center);

		camera->SetNearClip(length*0.1);
		camera->SetFarClip(length*15.5);

		float defaultZoom = Parameters::Instance()->m_DefaultZoomFactor;

		defaultZoom = defaultZoom * screenHeight/screenWidth;

		if(screenHeight > screenWidth )
		{
			defaultZoom = defaultZoom *0.5*(screenWidth*1.0/screenHeight);
		}

        camera->SetZoom(defaultZoom);

		camera->SetInitRotateCenter(pBoundingBox.Center());

		camera->SetFov(90);

		camera->LookAt(pBoundingBox.Center(),Vector3(0,1,0),TS_WORLD);

	}

}

void DraggerHandler::OptimizeCamera()
{
}

void DraggerHandler::OnTouchUp(float* p, int n)
{

	if (Parameters::Instance()->m_IsConRotate)
	{
		if (1 == n)
		{
			//LOGE("Touch Up keeping %d",n);
			//LOGE("angele is %f",m_trackBall.mvMatrix.rotation.angle);


			if (m_TrackBall.m_angle > 3)
			{
				float angle = m_TrackBall.m_angle;

				if(angle>0)
				{
					angle = M3D::Min(angle,15.0f);
				}
				else
				{
					angle = M3D::Max(angle,-15.0f);
				}

				if (M3D::Abs(m_TrackBall.m_angle) > 14)
				{
					m_TrackBall.mvMatrix.rotation.FromAngleAxis(angle,m_TrackBall.m_axis);
				}

				this->StartKeepState();
			}
			else
			{
				this->EndKeepState();
			}
		}
		else if (2 == n)
		{
			m_TrackBall.TwoPointsUp(p, n);
		}
		else if (3 == n)
		{

		}
	}
	else
	{

		if (1 == n)
		{
			this->EndKeepState();
			//	LOGI("Touch Up ");
			m_TrackBall.OnePointUp(p, n);
		}
		else if (2 == n)
		{
			m_TrackBall.TwoPointsUp(p, n);
		}
		else if (3 == n)
		{

		}
		m_TrackBall.Reset();
	}
	UpdateRenderQuality(false);
	UpDataTouchIntent();

}

void DraggerHandler::OnTouchMove(int moveType, float* p, int n)
{

	if (1 == n)
	{
		if (moveType == 1)
		{
			Model * selectModel = GetSelectModel();
			if (selectModel)
			{
				switch (m_mode)
				{
				case OPEMODE::ROTATE:
					m_TrackBall.OnePointRotate(p, n);
					break;
				case OPEMODE::TRANSLATE:
					m_TrackBall.OnePointsMove(p, n);
					break;
				case OPEMODE::SCALE:
					m_TrackBall.OnePointsScale(p, n);
					break;
				}
			}
			else
			{
				LOGI("");
				m_TrackBall.OnePointRotate(p, n);
			}
		}

	}
	else if (2 == n)
	{
		m_TrackBall.TwoPointsMove(p, n);
		this->m_SceneManager->RestoreRotationCenter();
	}
	else if (3 == n)
	{

	}
	if (StateChanged())
	{
		Model * selectModel = GetSelectModel();
		if (!selectModel)
		{
			UpDataTouchIntent();

		}
		else
		{
			switch (m_mode)
			{
			case OPEMODE::ROTATE:
			{
//			LOGI(" OPEMODE::ROTATE:");
				Quaternion mainQuat = m_TrackBall.mvMatrix.rotation;
				CameraNode * camera = m_SceneManager->GetCamera();
				Matrix3 viewMat = camera->GetView().ToMatrix3();
				Quaternion tempQuat(viewMat);
				tempQuat.Normalize();
				mainQuat = tempQuat.Inverse()*mainQuat*tempQuat;
//				LOGI("mainQuat ===%s",mainQuat.Tostring().c_str());
				if (m_axis == TRANSAXIS::AXIS_FACE) //任意旋转
				{
					ModelAssemblyHelper::RotateAssemblyModel(selectModel,
							mainQuat);
				}
				else if (m_axis == TRANSAXIS::AXIS_Z)
				{
					Quaternion zQuat(mainQuat.m_w, 0.0f, 0.0f, mainQuat.m_z);
					zQuat.Normalize();
					ModelAssemblyHelper::RotateAssemblyModel(selectModel,
							zQuat);
				}
				else if (m_axis == TRANSAXIS::AXIS_Y)
				{
					Quaternion yQuat(mainQuat.m_w, 0.0f, mainQuat.m_y, 0.0f);
					yQuat.Normalize();
					ModelAssemblyHelper::RotateAssemblyModel(selectModel,
							yQuat);
				}
				else if (m_axis == TRANSAXIS::AXIS_X)
				{
					Quaternion xQuat(mainQuat.m_w, mainQuat.m_x, 0.0f, 0.0f);
					xQuat.Normalize();
					ModelAssemblyHelper::RotateAssemblyModel(selectModel,
							xQuat);
				}
			}
				break;
			case OPEMODE::TRANSLATE:
			{
//			LOGI(" OPEMODE::TRANSLATE:");
				Vector3 movVec = m_TrackBall.mvMatrix.moveVector;

				CameraNode * camera = m_SceneManager->GetCamera();
				Matrix3 viewMat = camera->GetView().ToMatrix3().Inverse();
				movVec = viewMat *movVec;
//				LOGI("movVec==%s",movVec.Tostring().c_str());
				if (m_axis == TRANSAXIS::AXIS_X)
				{
					Vector3 xVec(movVec.m_x, 0.0f, 0.0f);
					ModelAssemblyHelper::TranslateAssemblyModel(selectModel,
							xVec);
				}
				else if (m_axis == TRANSAXIS::AXIS_Y)
				{
					Vector3 yVec(0, movVec.m_y, 0.0f);
					ModelAssemblyHelper::TranslateAssemblyModel(selectModel,
							yVec);

				}
				else if (m_axis == TRANSAXIS::AXIS_Z)
				{
					Vector3 zVec(0.0f, 0.0f, movVec.m_z);
					ModelAssemblyHelper::TranslateAssemblyModel(selectModel,
							zVec);
				}
				else if (m_axis == TRANSAXIS::AXIS_FACE) //垂直相机的平面方向进行移动
				{
					M3D::Vector3 startVec, endVec;

					//CameraNode* camera = m_SceneManager->GetCamera();
					//const Viewport& viewPort = camera->GetViewPort();
					//startVec = viewPort.ScreenToWorldPoint(m_PriPointTwo1.m_x,
					//		m_PriPointTwo1.m_y, 0.5);
					//endVec = viewPort.ScreenToWorldPoint(p[0], p[1], 0.5);

					//Vector3 nD(endVec - startVec);
					//Vector3 mov_ = nD;
					ModelAssemblyHelper::TranslateAssemblyModel(selectModel,
						movVec);
				}
			}
				break;
			case OPEMODE::SCALE:
			{
//			LOGI(" OPEMODE::SCALE:");
				float scaleFactor = m_TrackBall.mvMatrix.scaleFactor;

				if (m_axis == TRANSAXIS::AXIS_X)
				{
					Vector3 xVec(scaleFactor, 1.0f, 1.0f);
					ModelAssemblyHelper::ScaleAssemblyModel(selectModel, xVec);
				}
				else if (m_axis == TRANSAXIS::AXIS_Y)
				{
					Vector3 yVec(1.0f, scaleFactor, 1.0f);
					ModelAssemblyHelper::ScaleAssemblyModel(selectModel, yVec);
				}
				else if (m_axis == TRANSAXIS::AXIS_Z)
				{
					Vector3 zVec(1.0f, 1.0f, scaleFactor);
					ModelAssemblyHelper::ScaleAssemblyModel(selectModel, zVec);
				}
				else if (m_axis == TRANSAXIS::AXIS_FACE) //等比例缩放
				{
					Vector3 scales(scaleFactor, scaleFactor, scaleFactor);
					ModelAssemblyHelper::ScaleAssemblyModel(selectModel,
							scales);
				}
			}
				break;
			}
		}
	}
	m_PriPointTwo1.m_x = p[0];
	m_PriPointTwo1.m_y = p[1];
	UpdateRenderQuality(true);

}

void DraggerHandler::OnUpDataTouchIntent()
{
	Quaternion rotation = m_TrackBall.mvMatrix.rotation.Inverse() ;
	CameraNode * camera = m_SceneManager->GetCamera();
	camera->RotateAroundCenter(rotation,TS_WORLD);

	m_SelectedNodes->Translate(m_TrackBall.mvMatrix.moveVector,
			m_TrackBall.mvMatrix.currPos);

	m_SelectedNodes->Zoom(m_TrackBall.mvMatrix.scaleFactor);
}

Model *  DraggerHandler::GetSelectModel()
{
	Model * model = NULL;

	Selector * selector =  m_pView->GetSelector();
	IShape * shape =  selector->Get();
	if(shape && shape->GetType() == SHAPE_MODEL)
	{
		model = dynamic_cast<Model *>(shape);
	}

	return model;
}

void  DraggerHandler::SetMode(OPEMODE mode )
{
	m_mode = mode;
}
void  DraggerHandler::SetAxis(TRANSAXIS axis)
{
	m_axis = axis;
}

void DraggerHandler::OnTouchDown(float* p, int n)
{
	m_TrackBall.Reset();
	m_PriPointTwo1.m_x = p[0];
	m_PriPointTwo1.m_y = p[1];
	if (1 == n)
	{
		m_TrackBall.OnePointStart(p, n);
	}
	else if (2 == n)
	{
		m_TrackBall.TwoPointsStart(p, n);
	}
	else if (3 == n)
	{

	}

}

}
 /* namespace SVIEW */
