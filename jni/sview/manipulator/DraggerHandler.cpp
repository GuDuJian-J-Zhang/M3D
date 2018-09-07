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
#include "sview/views/Selector.h"
#include "m3d/extras/modelmanager/ModelAssemblyHelper.h"
#include "m3d/base/glue/GlueObj.h"
using namespace M3D;

namespace SVIEW
{
DraggerHandler::DraggerHandler()
{
	m_touchHandlerType = HANDLER_DRAGGER;
	m_axis = (TRANSAXIS)0;
	m_mode = (OPEMODE)0;
	m_bDragStarted = false;
	m_moveAxis = Vector3(1.0f,0.0f,0.0f);
	m_fMoveValue = 0.0f;
		m_isChangeSceneBox = false;
		m_coutChange = 0;
}

DraggerHandler::~DraggerHandler()
{
	m_bDragStarted = false;
	m_mapSelectedModelInitWorldMatrix.clear();
}

void DraggerHandler::InitCamera(bool useAni)
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
                                                     true, false, false, useAni);
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
    ResetDragInfo();
	UpdateRenderQuality(false);
	UpDataTouchIntent();
		if (m_coutChange>0)
		{
			ResetViewCamera();
			m_coutChange = 0;
		}

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
		vector<Model*> models = GetSelectModels();
		if (models.size() == 0)
		{
			UpDataTouchIntent();
		}
		else
		{
			for (int i = 0; i < models.size(); i++)
			{
				Model* selectModel = models.at(i);
				if(!selectModel)
					continue;

				if (!m_bDragStarted)
				{
					Matrix3x4 modelMatrix = selectModel->GetWorldTransform();
					m_mapSelectedModelInitWorldMatrix.insert(make_pair(selectModel, modelMatrix));
				}
				switch (m_mode)
				{
				case OPEMODE::ROTATE:
				{
						Vector2 pri(m_PriPointTwo1.m_x, m_PriPointTwo1.m_y);
						Vector2 cur(p[0], p[1]);
						float factor1 = (cur.m_x - pri.m_x)*0.001;
						float factor2 = (cur.m_y - pri.m_y)* -0.001;
						float moveDelta = Abs(factor1) > Abs(factor2) ? factor1 : factor2;
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
							Quaternion zQuat(mainQuat.m_w, 0.0f, 0.0f, moveDelta);
						zQuat.Normalize();
						ModelAssemblyHelper::RotateAssemblyModel(selectModel,
							zQuat);
					}
					else if (m_axis == TRANSAXIS::AXIS_Y)
					{
							Quaternion yQuat(mainQuat.m_w, 0.0f, moveDelta, 0.0f);
						yQuat.Normalize();
						ModelAssemblyHelper::RotateAssemblyModel(selectModel,
							yQuat);
					}
					else if (m_axis == TRANSAXIS::AXIS_X)
					{
							Quaternion xQuat(mainQuat.m_w, moveDelta, 0.0f, 0.0f);
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
					float scaleFactor = 1/m_TrackBall.mvMatrix.scaleFactor;

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
				bool tempb = ModelAssemblyHelper::IsModelInScene(this->m_pView->GetSceneManager(),selectModel);
				if (tempb)
				{
					m_coutChange++;
				}
			}

			m_bDragStarted = true;
		}

		Model * selectModel = GetSelectModel();
		if (selectModel)
		{
			if (m_bDragStarted)
			{
				Matrix3x4 modelMatrix = selectModel->GetWorldTransform();
				Matrix3x4 modelInitWorldMatrix = m_mapSelectedModelInitWorldMatrix.at(selectModel);
				Matrix3x4 modelInitWorldMatrixInv = modelInitWorldMatrix;
				modelInitWorldMatrixInv = modelInitWorldMatrixInv.Inverse();
				Matrix3x4 modelOffsetMatrix = Matrix3x4::IDENTITY;
				modelOffsetMatrix = modelInitWorldMatrixInv*modelMatrix;
				switch (m_mode)
				{
				case OPEMODE::ROTATE:
				{
					Quaternion rotation = modelOffsetMatrix.Rotation();
					float fAngle;
					rotation.GetValue(m_moveAxis, fAngle);
					GlueObj* pGlueObj = this->m_SceneManager->GetGlueObj();
                    if (pGlueObj && pGlueObj->GetDragModelCBeFun()){
                        pGlueObj->GetDragModelCBeFun()(1,fAngle, Parameters::Instance()->m_measureUnitStyle);
                        
                    }
					
					m_fMoveValue = fAngle;
					Matrix3 modelInitWorldMatrix3 = modelInitWorldMatrix.ToMatrix3();
					m_moveAxis = modelInitWorldMatrix3*m_moveAxis;
					break;
				}
				case OPEMODE::TRANSLATE:
				{
					Vector3 moveVector(modelMatrix.m_m03 - modelInitWorldMatrix.m_m03, modelMatrix.m_m13 - modelInitWorldMatrix.m_m13, modelMatrix.m_m23 - modelInitWorldMatrix.m_m23);
					float moveDistance = moveVector.Length();
					if (this->m_SceneManager)
					{
						GlueObj* pGlueObj = this->m_SceneManager->GetGlueObj();
						if (pGlueObj && pGlueObj->GetDragModelCBeFun())
							pGlueObj->GetDragModelCBeFun()(0,moveDistance, Parameters::Instance()->m_measureUnitStyle);
						m_moveAxis = moveVector;
						m_moveAxis.Normalize();
						m_fMoveValue = moveDistance;
					}
					break;
				}
				}
			}
				//			this->m_pView->GetSceneManager()->UpdateSceneByModel(selectModel);
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

	//m_SelectedNodes->Translate(m_TrackBall.mvMatrix.moveVector,
	//		m_TrackBall.mvMatrix.currPos);
	camera->Translate(m_TrackBall.mvMatrix.moveVector*-1, TS_LOCAL);
	camera->ZoomView(1 / m_TrackBall.mvMatrix.scaleFactor);
	//m_SelectedNodes->Zoom(m_TrackBall.mvMatrix.scaleFactor);
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

vector<Model*>  DraggerHandler::GetSelectModels()
{
	vector<Model*> models;

	Selector * selector = m_pView->GetSelector();
	vector<IShape*>& selectShapes = selector->GetAll();

	for (int i  =0;i<selectShapes.size();i++)
	{
		IShape* shape = selectShapes.at(i);
		if (shape && shape->GetType() == SHAPE_MODEL)
		{
			Model* model = dynamic_cast<Model *>(shape);
			models.push_back(model);
		}
	}
 
	return models;
}
	void DraggerHandler::SetIsChangerSceneBox(bool state)
	{
		m_isChangeSceneBox = state;
	}
void  DraggerHandler::SetMode(OPEMODE mode )
{
	m_mode = mode;
	ResetDragInfo();
}
void  DraggerHandler::SetAxis(TRANSAXIS axis)
{
	m_axis = axis;
	ResetDragInfo();
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

void  DraggerHandler::SetMoveValue(float fValue)
{
	float fRealMoveValue = fValue - m_fMoveValue;
	vector<Model*> models = GetSelectModels();
	for (int i = 0; i < models.size(); i++)
	{
		Model* selectModel = models.at(i);
		if(!selectModel)
			continue;
		switch (m_mode)
		{
		case OPEMODE::ROTATE:
		{
			Quaternion rotation(fRealMoveValue, m_moveAxis);
			ModelAssemblyHelper::RotateAssemblyModel(selectModel,
				rotation);
		}
		break;
		case OPEMODE::TRANSLATE:
		{
			Vector3 moveVector = m_moveAxis * fRealMoveValue;
			ModelAssemblyHelper::TranslateAssemblyModel(selectModel,
				moveVector);
		}
		break;
		}
 
			bool tempb = ModelAssemblyHelper::IsModelInScene(this->m_pView->GetSceneManager(),
				selectModel);
			if (tempb)
			{
				m_coutChange++;
			}
	}
	m_fMoveValue = fValue;
		if (m_coutChange > 0)
		{
			ResetViewCamera();
			m_coutChange = 0;
		}
	//刷新
	m_pView->RequestDraw();
}

void DraggerHandler::ResetDragInfo()
{
	m_bDragStarted = false;
	switch (m_axis)
	{
	case TRANSAXIS::AXIS_X:
		m_moveAxis = Vector3(1.0f, 0.0f, 0.0f);
		break;
	case TRANSAXIS::AXIS_Y:
		m_moveAxis = Vector3(0.0f, 1.0f, 0.0f);
		break;
	case TRANSAXIS::AXIS_Z:
		m_moveAxis = Vector3(0.0f, 0.0f, 1.0f);
		break;
	default:
		m_moveAxis = Vector3(1.0f, 0.0f, 0.0f);
		break;
	}
	m_fMoveValue = 0.0f;
	m_mapSelectedModelInitWorldMatrix.clear();
	GlueObj* pGlueObj = this->m_SceneManager->GetGlueObj();
	if (pGlueObj && pGlueObj->GetDragModelCBeFun())
		{
			int iOperation = 0;
			switch (m_mode)
			{
			case OPEMODE::ROTATE:
			{
				iOperation = 1;
			}
			break;
			case OPEMODE::TRANSLATE:
			{
				iOperation = 0;
			}
			break;
			}
			pGlueObj->GetDragModelCBeFun()(iOperation, 0.0f, Parameters::Instance()->m_measureUnitStyle);
		}
}

	void DraggerHandler::ResetViewCamera()
	{

		ModelAssemblyHelper::RequestShowAllModel(this->m_pView->GetSceneManager());
	}
}
 /* namespace SVIEW */
