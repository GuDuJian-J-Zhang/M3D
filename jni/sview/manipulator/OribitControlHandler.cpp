#include <math.h>
#include "sview/views/Parameters.h"
#include "sview/manipulator/OribitControlHandler.h"
#include "m3d/SceneManager.h"
#include "m3d/RenderManager.h"
#include "m3d/graphics/CameraNode.h"
#include "m3d/utils/CullerHelper.h"
#include "sview/views/View.h"
#include "sview/extern/PerspectiveData.h"
#include "sview/extern/PerspectiveOperator.h"

namespace SVIEW
{

OribitControlHandler::OribitControlHandler():TouchHandler()
{
	m_cameraToCenterDis = 0.0001;
	m_movSpeed = 0.00001;
	m_needInitDirection = true;
	m_oldCameraPos = Vector3(0.0f,0.0f,0.0f);
	m_theta = 0.0f;
	this->m_TrackBall.SetRotateSpeed(1.0f);
}

OribitControlHandler::~OribitControlHandler()
{

}

void OribitControlHandler::InitCamera(bool useAni)
{
	if(!this->m_SceneManager)
	{
		return;
	}
	SceneManager* scene = m_SceneManager;
	CameraNode* camera = scene->GetCamera();
    camera->SetOrthographic(false);
    //scene->ResetCamera();
    camera->SetFov(35.0f);
	BoundingBox& pBoundingBox = scene->GetSceneBox();
    
	float length = pBoundingBox.Length();
	camera->SetNearClip(length*0.005);
	camera->SetFarClip(length*2.5);
    
    if(m_pView)
    {
		m_pView->GetPerspectiveOperator()->Show(m_pView, PerspectiveData::ISOMETRIC,
                                                 true, false, false, useAni);
        this->SetUpDirection(Parameters::Instance()->m_upDirectionValue,m_pView);
    }

    const IntRect& rect = camera->GetViewPort().GetRect();
    int width = rect.Width()>rect.Height()?rect.Height():rect.Width();
    this->m_TrackBall.SetTrackWindow(width,width);

}

void OribitControlHandler::OptimizeCamera()
{
	if(!this->m_SceneManager)
	{
		return;
	}

	SceneManager* scene = m_SceneManager;
	CameraNode* camera = scene->GetCamera();
	BoundingBox& pBoundingBox = scene->GetSceneBox();
	///摄像机距离包围盒的距离
	m_cameraToCenterDis = (camera->GetWorldPosition() - pBoundingBox.Center()).Length();
	///移动速度
//	if(camera->GetFrustum().IsInside(pBoundingBox) == INSIDE)
//	{
//		m_movSpeed = m_cameraToCenterDis/50;
//		CameraNode* camera = scene->GetCamera();
//		BoundingBox& pBoundingBox = scene->GetSceneBox();
////		float length = pBoundingBox.Length();
////		camera->SetNearClip(length*0.8);
////		camera->SetFarClip(length*2.5);
//		LOGI("out side");
//	}
//	else
	{
//		CameraNode* camera = scene->GetCamera();
//		BoundingBox& pBoundingBox = scene->GetSceneBox();
		float length = pBoundingBox.Length();
//		camera->SetOrthoSize(Vector2(30,30));
//		camera->SetNearClip(length*0.005);
//		camera->SetFarClip(length*2.5);

	}
//	LOGE("mov speed %f",m_movSpeed);
	//摄像机在包围盒外部，设置旋转中心为包围盒中心点，能够提升操作体验
	if(pBoundingBox.IsInside(camera->GetWorldPosition()) == OUTSIDE)
	{
		camera->SetRotateCenter(pBoundingBox.Center());
		m_movSpeed = pBoundingBox.Length()/50;
	}
	else
	{
		camera->SetRotateCenter(camera->GetPosition());
		m_movSpeed = pBoundingBox.Length()/200;
	}
//	camera->SetFov(60);
}

void OribitControlHandler::OnTouchUp(float* p, int n)
{
	if (Parameters::Instance()->m_IsConRotate)
	{
		if (1 == n)
		{
			//LOGE("Touch Up keeping %d",n);
			//LOGE("angele is %f",m_trackBall.mvMatrix.rotation.angle);
			if (m_TrackBall.m_angle > 3)
			{
				if (m_TrackBall.m_angle > 30)
				{
					m_TrackBall.mvMatrix.rotation.FromAngleAxis(30,m_TrackBall.m_axis);
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
			TwoPointsUp(p, n);
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
			TwoPointsUp(p, n);
		}
		else if (3 == n)
		{

		}
		m_TrackBall.Reset();
	}

	UpDataTouchIntent();
	UpdateRenderQuality(false);

}

void OribitControlHandler::OnTouchMove(int moveType, float* p, int n)
{
	if (1 == n)
	{
		if (moveType == 1)
			m_TrackBall.OnePointRotate(p, n);
		else if (moveType == 2)
			m_TrackBall.OnePointsMove(p, n);
		else if (moveType == 3)
			m_TrackBall.OnePointsScale(p, n);
	}
	else if (2 == n)
	{
		TwoPointsMove(p, n);
	}
	else if (3 == n)
	{

	}
	if (StateChanged())
	{
		UpDataTouchIntent();
	}

	UpdateRenderQuality(true);
}


void OribitControlHandler::OnTouchDown(float* p, int n)
{
	OptimizeCamera();
	if (1 == n)
	{
		m_TrackBall.OnePointStart(p, n);
	}
	else if (2 == n)
	{
		TwoPointsStart(p, n);
	}
	else if (3 == n)
	{

	}

}

void OribitControlHandler::TwoPointsUp(float*pos, int n)
{
	Reset();
}

float OribitControlHandler::TwoPointsDis(float*pos, int n)
{
	float x = pos[0] - pos[2];
	float y = pos[1] - pos[3];
	return (float) sqrt(x * x + y * y);

}
void OribitControlHandler::TwoPointsStart(float*pos, int n)
{
	m_PriPointTwo1.m_x = pos[0];
	m_PriPointTwo1.m_y = pos[1];
	m_PriPointTwo2.m_x = pos[2];
	m_PriPointTwo2.m_y = pos[3];

	IntVector2 centerPnt = (m_PriPointTwo1 + m_PriPointTwo2)/2;

	m_PirDistance = TwoPointsDis(pos);
	if (m_SceneManager != NULL)
	{
		CameraNode* camera = m_SceneManager->GetCamera();
		cachePriPoint = camera->GetView()* camera->GetViewPort().ScreenToWorldPoint(
				centerPnt.m_x,centerPnt.m_y,m_screenDepth);
	}
}


void OribitControlHandler::TwoPointsMove(float*pos, int n)
{
	m_CurrDistance = TwoPointsDis(pos);
	float scaleFactor = m_PirDistance / m_CurrDistance;
	m_TrackBall.mvMatrix.rotation = Quaternion::IDENTITY;
	m_TrackBall.mvMatrix.moveVector = Vector3::ZERO;
	if (fabs(m_PriPointTwo1.m_x - pos[0]) > 1
			|| fabs(m_PriPointTwo1.m_y - pos[1]) > 1
			|| fabs(m_PriPointTwo2.m_x - pos[2]) > 1
			|| fabs(m_PriPointTwo2.m_y - pos[3]) > 1)
	{
		if (m_SceneManager != NULL)
		{
			CameraNode* camera = m_SceneManager->GetCamera();
			if (!camera->IsOrthographic())
			{
				Trackball::ISMOVING = true;
				Trackball::KEEPINGSTATETIMES = Trackball::TIMES;

				IntVector2 currentPnt((pos[0] + pos[2]) / 2,
						(pos[1] + pos[3]) / 2);
				IntVector2 priPnt((m_PriPointTwo1 + m_PriPointTwo2)/ 2);

				if (fabs(scaleFactor - 1.0f) > 0.03)
				{
					//得到移动目标点
					Vector3 movTarget =
							camera->GetViewPort().ScreenToWorldPoint(
									currentPnt.m_x, currentPnt.m_y,
									m_screenDepth);
					Vector3 cameraPos = camera->GetPosition();

					float moveFactor = m_movSpeed * scaleFactor
							* (scaleFactor > 1 ? -1 : 1);
					//				m_TrackBall.mvMatrix.moveVector = Vector3::ZERO;
					camera->SetWorldPosition(
							cameraPos
									+ (movTarget - cameraPos).Normalized()
											* moveFactor);
					m_PirDistance = m_CurrDistance;
				}
				else
				{
					cacheCurPointNear = camera->GetView()
							* camera->GetViewPort().ScreenToWorldPoint(
									currentPnt.m_x, currentPnt.m_y,
									m_screenDepth);
					cachePriPoint = camera->GetView()
							* camera->GetViewPort().ScreenToWorldPoint(
									priPnt.m_x, priPnt.m_y, m_screenDepth);
					m_TrackBall.mvMatrix.moveVector = (cacheCurPointNear
							- cachePriPoint).Normalized() * m_movSpeed/4;
				}

				this->OptimizeCamera();
				m_TrackBall.mvMatrix.scaleFactor = 1.0f;
//				LOGE("Walkthrougher::TwoPointsMove %f", moveFactor);
			}
		}
	}
	m_PriPointTwo1.m_x = pos[0];
	m_PriPointTwo1.m_y = pos[1];
	m_PriPointTwo2.m_x = pos[2];
	m_PriPointTwo2.m_y = pos[3];
}

void OribitControlHandler::RotateOnFixedPoint()
{
	//LOGI("void WalkthroughHandler::RotateOnFixedPoint BEGIN");
	if(!this->m_SceneManager)
	{
		return ;
	}

	float theta = 1.0f;
	Vector3 yDirection(0.0f,1.0f,0.0f);
	Vector3 xDirection(1.0f,0.0f,0.0f);
	Vector3 rotateAxis(0.0f,1.0f,0.0f);
	rotateAxis.Normalize();
	SceneManager* scene = m_SceneManager;
	CameraNode* camera = scene->GetCamera();

	BoundingBox& pBoundingBox = scene->GetSceneBox();

	Vector3 boundingBoxCenter = pBoundingBox.Center();//包围盒中心

	float length = pBoundingBox.Length() ;

	if(m_needInitDirection)
	{
		m_oldDirectionVector = -(yDirection * length+ xDirection * length);//暂定都为length //TODO
		m_rotateRadius = m_oldDirectionVector.Length();
		m_oldCameraPos = -m_oldDirectionVector;

		m_needInitDirection = false;
	}

	Quaternion rotateQua(m_theta,rotateAxis);
	rotateQua.Normalize();

	Quaternion oldQua(0.0f,m_oldCameraPos.m_x,m_oldCameraPos.m_y,m_oldCameraPos.m_z);//要旋转的点对应的四元数

	Quaternion newQua = rotateQua * oldQua * rotateQua.Inverse();//求旋转后的点对应的四元数

	Vector3 newPos(newQua.m_x,newQua.m_y,newQua.m_z);//旋转后的点

	camera->SetWorldPosition(newPos + boundingBoxCenter /*+ newPos.Normalized() * M3D::Sin(m_theta*2.0f)*80.0f*/  );
	camera->LookAt(boundingBoxCenter,yDirection);

	m_theta ++;
	if((m_theta-360.0f>-0.0000001 && m_theta-360.0f<0.00000001)||(m_theta > 360.0f))
	{
		m_theta = m_theta-360.0f;
		//LOGE("rotate a round");
	}
	float newDis = (newPos).Length();
	//LOGI("void WalkthroughHandler::RotateOnFixedPoint END");
}
void * OribitControlHandler::GetRotateOnFixedPointFunc(void * data)
{
	//LOGI(" WalkthroughHandler::GetRotateOnFixedPointFunc BEGIN");
	OribitControlHandler* touchHandle = (OribitControlHandler*) data;
	touchHandle->RotateOnFixedPoint();
	//LOGI(" WalkthroughHandler::GetRotateOnFixedPointFunc END");
	return NULL;
}

void OribitControlHandler::StartRotateOnFixedPoint()
{
	LOGI(" WalkthroughHandler::StartRotateOnFixedPoint BEGIN");
	m_RotateOnFixedPointTimer.StartTimer();
	m_RotateOnFixedPointTimer.SetTimer(GetRotateOnFixedPointFunc,this,0,36);
	LOGI(" WalkthroughHandler::StartRotateOnFixedPoint END");

}

void OribitControlHandler::EndRotateOnFixedPoint()
{
	m_RotateOnFixedPointTimer.StopTimer();
	m_needInitDirection = true;
}

}
