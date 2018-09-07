#include <math.h>
#include "sview/views/Parameters.h"
#include "sview/manipulator/WalkthroughHandler.h"
#include "m3d/SceneManager.h"
#include "m3d/RenderManager.h"
#include "m3d/graphics/CameraNode.h"
#include "m3d/utils/CullerHelper.h"
#include "sview/views/View.h"
#include "sview/extern/PerspectiveData.h"
#include "sview/extern/PerspectiveOperator.h"
#include "m3d/Handler/HandlerPoint.h"

namespace SVIEW
{

WalkthroughHandler::WalkthroughHandler():CommonTouchHandler()
{
	m_cameraToCenterDis = 0.0001;
	m_movSpeed = 0.00001;

	m_touchHandlerType = HANDLER_WALKTHROUGH;
	m_firstPersionSpeed = 1.0f;
	m_changeDisOnRotate = true;

	m_modelRotation = Quaternion::IDENTITY;
	m_insideBoundingBox = false;
	m_isUpDirection = false;
	m_speedControlValue = 1.0f;
	m_standardSpeed = 1.0f;
	m_isOribteRotateState = true;
	//this->m_TrackBall.SetRotateSpeed(0.3f);

	this->m_useOribit = true;
	m_phi =0.0;
	m_controlLockXY = true;
	this->m_freeViewMode = true;
	this->m_vrModel = false;
	m_virtualKeyTarget = Vector3::ZERO;
}

WalkthroughHandler::~WalkthroughHandler()
{

}

void WalkthroughHandler::InitCamera(bool useAni)
{
	CommonTouchHandler::InitCamera(useAni);
	return;
	//LOGI("reset");
	//if(!this->m_SceneManager)
	//{
	//	return;
	//}
	//SceneManager* scene = m_SceneManager;
	//CameraNode* camera = scene->GetCamera();
 //   camera->ReSet();
 //   camera->SetOrthographic(false);
	//BoundingBox& pBoundingBox = scene->GetSceneBox();
	//pBoundingBox.Clear();
	//pBoundingBox = scene->GetSceneBox();//TODO 需要重新计算包围盒
	//Viewport viewport =  camera->GetViewPort();
	//int screenHeight =viewport.GetRect().Height();
	//int screenWidth = viewport.GetRect().Width();

	//m_SceneManager->GetRenderManager()->WindowSizeChanged(screenWidth, screenHeight);
	//float length = pBoundingBox.Length();
	//m_standardSpeed = pBoundingBox.Length()/500.0f;//标准速度
	//Vector3 center =  pBoundingBox.Center();

	//center.m_z += pBoundingBox.Length() * CAMERA_POSFACTOR;

	//camera->SetWorldPosition(center);
 //
	//length = this->m_SceneManager->GetDefaultFocusLength();

	//camera->SetNearClip(length * NEAR_CLIP_PLANE_FACTOR);
	//camera->SetFarClip(length * FAR_CLIP_PLANE_FACTOR);

	//camera->SetZoom(this->m_SceneManager->GetDefaultZoom());

	//camera->SetInitRotateCenter(pBoundingBox.Center());

	//camera->SetFov(90);

	//camera->LookAt(pBoundingBox.Center(), Vector3(0, 1, 0), TS_WORLD);
 //   
	//this->m_TrackBall.SetRotateSpeed(2.0f);
 //   m_oribitControlTarget = pBoundingBox.Center() ;
 //
 //   OptimizeCamera();

 //   if(m_pView)
 //   {
	//	m_pView->GetPerspectiveOperator()->Show(m_pView, PerspectiveData::FRONT,
 //                                                true, false, false);

 //       this->SetUpDirection(this->m_upDirection,m_pView);
 //   }

 //   //optimize trackball
 //   const IntRect& rect = camera->GetViewPort().GetRect();
 //   int width = rect.Width()>rect.Height()?rect.Height():rect.Width();
 //   this->m_TrackBall.SetTrackWindow(width,width);
//    LOGI("init camera pos ==%s",camera->GetWorldPosition().Tostring().c_str());
}

//void WalkthroughHandler::InitModelViewCamera()
//{
//
//	if(!this->m_SceneManager)
//	{
//		return;
//	}
//	SceneManager* scene = m_SceneManager;
//	CameraNode* camera = scene->GetCamera();
//    camera->ReSet();
//    camera->SetOrthographic(false);
//    //scene->ResetCamera();
//    //
//	BoundingBox& pBoundingBox = scene->GetSceneBox();
//	Viewport viewport =  camera->GetViewPort();
//	int screenHeight =viewport.GetRect().Height();
//	int screenWidth = viewport.GetRect().Width();
//
//	m_SceneManager->GetRenderManager()->WindowSizeChanged(screenWidth, screenHeight);
//
//	float length = pBoundingBox.Length();
//
//	Vector3 center =  pBoundingBox.Center();
//
//    //平行投影
//    if(camera->IsOrthographic())
//    {
//        center.m_z += pBoundingBox.Length()*8.5f;
//    }
//    else//垂直投影
//    {
//        center.m_z += pBoundingBox.Length()*1.5f;
//    }
//
//	camera->SetWorldPosition(center);
//    //
//    camera->SetFov(100.0f);
// 
//	camera->SetNearClip(length*0.005);
//	camera->SetFarClip(length*2.5);
//
//	float defaultZoom = Parameters::Instance()->m_DefaultZoomFactor;
//
//	defaultZoom = defaultZoom * screenHeight/screenWidth;
//
//	if(screenHeight > screenWidth )
//	{
//		defaultZoom = defaultZoom *0.5*(screenWidth*1.0/screenHeight);
//	}
//
//    camera->SetZoom(defaultZoom);
//
//	camera->SetInitRotateCenter(pBoundingBox.Center());
//
//	camera->SetFov(90);
//
//	camera->LookAt(pBoundingBox.Center(),Vector3(0,1,0),TS_WORLD);
//}

void WalkthroughHandler::OptimizeCamera()
{
	if (!this->m_SceneManager)
	{
		return;
	}

	CommonTouchHandler::OptimizeCamera();

	if (this->m_vrModel)
	{
		SceneManager* scene = m_SceneManager;
		CameraNode* camera = scene->GetCamera();
		BoundingBox pBoundingBox = scene->GetSceneBox();

		camera->SetRotateCenter(camera->GetPosition());
		float length = pBoundingBox.Length();

		//camera->SetNearClip(Min(1.0f, length*0.05));
		//camera->SetFarClip(length*2.5);

		this->m_TrackBall.SetRotateSpeed(0.5);
		m_insideBoundingBox = true;

		return;
	}


	if(m_controlLockXY || m_oribitMode)
	{
	if(m_insideBoundingBox)
	{
		m_useOribit = false;
	}
	else
	{
		m_useOribit = true;
	}
	}

	SceneManager* scene = m_SceneManager;
	CameraNode* camera = scene->GetCamera();
	BoundingBox pBoundingBox = scene->GetSceneBox();
	//pBoundingBox.ScaleBox(1.5f);

	///摄像机距离包围盒的距离
	m_cameraToCenterDis =
			(camera->GetWorldPosition() - pBoundingBox.Center()).Length();
    
    float length = pBoundingBox.Length();

	//摄像机在包围盒外部，设置旋转中心为包围盒中心点，能够提升操作体验
	if (pBoundingBox.IsInside(camera->GetWorldPosition()) == OUTSIDE)
	{
		if(!this->m_useOribit)
		{
			camera->SetRotateCenter(pBoundingBox.Center());
		}

		m_movSpeed = (pBoundingBox.Length() / 100
				+ m_cameraToCenterDis * 0.5f * m_firstPersionSpeed);

		this->m_TrackBall.SetRotateSpeed(0.5);
		m_insideBoundingBox = false;
        
        /*camera->SetNearClip(Min(5,length*0.1));
        camera->SetFarClip(length*3.5);*/
	}
	else
	{
		if(!this->m_useOribit)
		{
			camera->SetRotateCenter(camera->GetPosition());
		}
  
        //camera->SetNearClip(Min(1.0f,length*0.05));
        //camera->SetFarClip(length*2.5);

		this->m_TrackBall.SetRotateSpeed(0.5);
		m_insideBoundingBox = true;
	}
}

void WalkthroughHandler::RotateAroundAxis()
{
	CameraNode * camera = m_SceneManager->GetCamera();
	Vector3 position = camera->GetWorldPosition();
	Vector3 oldCameraPos = position - m_oribitControlTarget;
	Quaternion rotation = m_TrackBall.mvMatrix.rotation;
	Vector3 eulerAngles = rotation.EulerAngles();
	Vector3 rotateAngle = eulerAngles ;
	Quaternion rotateQua(-eulerAngles.m_y*1.5, Vector3::UP);
	//	rotateQua.Normalize();

		Matrix4 R1,R2,R3,R;
		R1.MultiTranslate(-m_oribitControlTarget);
		R2.MultiRotatiton(rotateQua);
		R3.MultiTranslate(m_oribitControlTarget);
		R = R3*R2*R1;
		Matrix4 view = camera->GetView().ToMatrix4();

		Matrix4 temp = (view * R *view.Inverse());
//		LOGI("m_oribitControlTarget = %s",m_oribitControlTarget.Tostring().c_str());

		camera->Translate(temp.Translation());
		camera->Rotate(temp.Rotation());

		//m_SelectedNodes->Translate(m_TrackBall.mvMatrix.moveVector,
		//		m_TrackBall.mvMatrix.currPos);
		camera->Translate(m_TrackBall.mvMatrix.moveVector*-1, TS_LOCAL);
		//m_SelectedNodes->Zoom(m_TrackBall.mvMatrix.scaleFactor);
		camera->ZoomView(1 / m_TrackBall.mvMatrix.scaleFactor);

}

void WalkthroughHandler::FreeViewRotate()
{
	m_useOribit = false;
	Quaternion rotation = m_TrackBall.mvMatrix.rotation.Inverse();
	CameraNode * camera = m_SceneManager->GetCamera();
	camera->RotateAroundCenter(rotation, TS_WORLD);

	//m_SelectedNodes->Translate(m_TrackBall.mvMatrix.moveVector, m_TrackBall.mvMatrix.currPos);
	camera->Translate(m_TrackBall.mvMatrix.moveVector*-1, TS_LOCAL);
	//m_SelectedNodes->Zoom(m_TrackBall.mvMatrix.scaleFactor);
	camera->ZoomView(1 / m_TrackBall.mvMatrix.scaleFactor);
}

void WalkthroughHandler::OnUpDataTouchIntent()
{
	CommonTouchHandler::OnUpDataTouchIntent();
}

//void WalkthroughHandler::OnTouchUp(float* p, int n)
//{
//	if (Parameters::Instance()->m_IsConRotate)
//	{
//		if (1 == n)
//		{
//			if (m_TrackBall.m_angle > 3)
//			{
//				if (m_TrackBall.m_angle > 30)
//				{
//					m_TrackBall.mvMatrix.rotation.FromAngleAxis(30,m_TrackBall.m_axis);
//				}
//				this->StartKeepState();
//			}
//			else
//			{
//				this->EndKeepState();
//			}
//		}
//		else if (2 == n)
//		{
//			TwoPointsUp(p, n);
//		}
//		else if (3 == n)
//		{
//
//		}
//	}
//	else
//	{
//		if (1 == n)
//		{
//			this->EndKeepState();
//			m_TrackBall.OnePointUp(p, n);
//		}
//		else if (2 == n)
//		{
//			TwoPointsUp(p, n);
//		}
//		else if (3 == n)
//		{
//
//		}
//		m_TrackBall.Reset();
//	}
//
//	UpDataTouchIntent();
//
//	UpdateRenderQuality(false);
//}
//
//void WalkthroughHandler::OnTouchMove(int moveType, float* p, int n)
//{
////	LOGI("one point move");
//	OptimizeCamera();
//	if (1 == n)
//	{
//		if(!m_isOribteRotateState)
//		{
//			updateOribitControlTarget();
//			m_isOribteRotateState = true;
//		}
//		if (moveType == 1)
//		{
//			m_TrackBall.OnePointRotate(p, n);
//		}
//	}
//	else if (2 == n)
//	{
//		TwoPointsMove(p, n);
//	}
//	else if (3 == n)
//	{
//
//	}
//	if (StateChanged())
//	{
//		UpDataTouchIntent();
//		if (m_oribitMode)
//		{
//			if (this->m_useOribit)
//			{
//				if (n == 1)
//				{
//					if (m_insideBoundingBox && Parameters::Instance()->m_openFirstPersionCamera)
//					{
////					LOGI("inside scene");
//						ControlCloseRangeByScreenCenter(); //场景内部以近点为目标点旋转
//					}
////				LOGI("oribit control");
//					OrbitControl();
//				}
//			}
//			else
//			{
//
//				if (m_insideBoundingBox)
//				{
////				LOGI("lock Pointer");
//					this->LockPointer();
//				}
//			}
//		}
//		else if(m_controlLockXY)
//		{
////			m_useOribit = false;
//			RotateAroundAxis();
//		}
//		else if(m_freeViewMode)
//		{
//			FreeViewRotate();
//		}
//	}
//
//	UpdateRenderQuality(true);
//}
//
//
//void WalkthroughHandler::OnTouchDown(float* p, int n)
//{
//	OptimizeCamera();
//	if (1 == n)
//	{
//		m_TrackBall.OnePointStart(p, n);
//	}
//	else if (2 == n)
//	{
//		TwoPointsStart(p, n);
//	}
//	else if (3 == n)
//	{
//
//	}
////	StartVirtualKeyControlTask();
//}

void WalkthroughHandler::TwoPointsUp(float*pos, int n)
{
	Reset();
}

float WalkthroughHandler::TwoPointsDis(float*pos, int n)
{
	float x = pos[0] - pos[2];
	float y = pos[1] - pos[3];
	return (float) sqrt(x * x + y * y);

}
void WalkthroughHandler::TwoPointsStart(float*pos, int n)
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

	//为了提高性能，不再求交点。而是用屏幕的反投影点来替代
	if(false)
	{
		Vector2 cur(centerPnt.m_x, centerPnt.m_y);
		bool intersect = m_SceneManager->GetPickPoint(cur,m_insectPoint, false);
		m_oribitControlTarget = m_insectPoint;
	}

	m_insectPoint = cachePriPoint;

}
    
void WalkthroughHandler::ControlCloseRangeByScreenCenter()
{
    OptimizeCamera();

    //根据屏幕中心点，构造射线和模型交点，作为oribit的目标点
    const Viewport& viewport = m_SceneManager->GetCamera()->GetViewPort();
    const IntRect& rect = viewport.GetRect();
    IntVector2 screenCenter((rect.m_left+rect.m_right)/2, (rect.m_bottom+rect.m_top)/2);
 
    CameraNode* camera = m_SceneManager->GetCamera();
 
    Vector3 coordinate = camera->GetViewPort().ScreenToWorldPoint(screenCenter.m_x, screenCenter.m_y,0.5);
// LOGI("screen center is %s",coordinate.Tostring().c_str());
    m_movSpeed = m_standardSpeed;
//	m_movSpeed = m_movSpeed*m_firstPersionSpeed;
    
    //如果使用oribit摄像机
    m_oribitControlTarget = coordinate;
#if  0
    ////为了适应BIM项目提的要求，增加虚拟键靠近墙面时速度减慢的要求
	if(m_insideBoundingBox)
//	{
//		BoundingBox pBoundingBox = m_SceneManager->GetSceneBox();
//		m_cameraToCenterDis = (camera->GetWorldPosition() - pBoundingBox.Center()).Length();
//
//		float length = pBoundingBox.Length();
//		m_movSpeed = (pBoundingBox.Length() / 100 + m_cameraToCenterDis * 0.05f * m_firstPersionSpeed);
//
//	}
//	else
	{
	    Vector2 cur(screenCenter.m_x, screenCenter.m_y);
	    Vector3 insectPoint;
	    bool intersect = m_SceneManager->GetPickPoint(cur,insectPoint, false);

		float distance = (camera->GetPosition() - insectPoint).Length();
		float rat = distance / m_movSpeed;

		float movSpeed = distance / 8.0f;
		m_movSpeed = M3D::Max(m_standardSpeed * 0.1f, movSpeed);
	}
#endif
}
#ifdef BIMSPEED

void WalkthroughHandler::VirtualKeyControlSpeed()
{
    const Viewport& viewport = m_SceneManager->GetCamera()->GetViewPort();
    const IntRect& rect = viewport.GetRect();
    IntVector2 screenCenter((rect.m_left+rect.m_right)/2, (rect.m_bottom+rect.m_top)/2);
	if(m_insideBoundingBox)
	{
	    Vector2 cur(screenCenter.m_x, screenCenter.m_y);
	    Vector3 insectPoint;
	    bool intersect = m_SceneManager->GetPickPoint(cur,insectPoint, false);
	    m_virtualKeyTarget = insectPoint;
	    CameraNode* camera = m_SceneManager->GetCamera();

		float distance = (camera->GetPosition() - insectPoint).Length();
		float rat = distance / m_movSpeed;

		float movSpeed = distance / 8.0f;
		m_movSpeed = M3D::Max(m_standardSpeed * 0.1f, movSpeed);
//		LOGI("m_movSpeed ======================%f",m_movSpeed);
	}
}

bool WalkthroughHandler::IsNeedUpdateSpeed(Vector3& walkDirection)
{
	bool ret = false;
	CameraNode* camera = m_SceneManager->GetCamera();
	Vector3 cameraPos = camera->GetPosition();
	Vector3 targetDirection = m_virtualKeyTarget - cameraPos;
	targetDirection.Normalize();
	float cosTheta = targetDirection.DotProduct(walkDirection);
	if(cosTheta<0.966)
	{
		ret = true;
	}

	return ret;
}


void* WalkthroughHandler::VirtualKeyControlTask(void * data)
{
	//LOGI(" WalkthroughHandler::GetRotateOnFixedPointFunc BEGIN");
	WalkthroughHandler* touchHandle = (WalkthroughHandler*) data;
	touchHandle->VirtualKeyControlSpeed();
	//LOGI(" WalkthroughHandler::GetRotateOnFixedPointFunc END");
	return NULL;
}

void  WalkthroughHandler::StartVirtualKeyControlTask()
{
	if(m_virtualKeyControlTimer.IsStart())
	 {
		m_virtualKeyControlTimer.StopTimer();
	 }
	m_virtualKeyControlTimer.StartTimer();
	m_virtualKeyControlTimer.SetTimer(VirtualKeyControlTask,this,0,330000);
}
void  WalkthroughHandler::EndVirtualKeyControlTask()
{
	if (m_virtualKeyControlTimer.IsStart())
	{
		m_virtualKeyControlTimer.StopTimer();
	}
}

#endif

void WalkthroughHandler::VirtualKeyMove(float strSpeed,float sidSpeed)
{
	if (Abs(strSpeed) < 0.01 && Abs(sidSpeed) < 0.01)
	{
		return;
	}

	ControlCloseRangeByScreenCenter();

	CameraNode* camera = m_SceneManager->GetCamera();
	Vector3 cameraPos = camera->GetPosition();
	Matrix3 viewMat = camera->GetView().ToMatrix3();
	//	LOGI("viewMat === %s",viewMat.Tostring().c_str());

	Vector3 directionZ(viewMat.m_m20, 0.0f, viewMat.m_m22);

	 Vector3 directionX(viewMat.m_m00,0.0f,viewMat.m_m02);//获取相机坐标系Z轴坐标

	directionZ = -directionZ; //反方向即为观察方向
	directionZ.Normalize();

	directionX = -directionX; //反方向即为观察方向
	directionX.Normalize();

	Vector3 direction = directionZ+directionX;
	direction.Normalize();

	///为了满足BIM虚拟键的要求做的
	Vector3 newPos;
#ifdef BIMSPEED
	if(IsNeedUpdateSpeed(direction))
	{
//	VirtualKeyControlSpeed();
	}
	if (m_insideBoundingBox)
	{
		float dis = GetDirectionDistance(direction, 0);
//		m_movSpeed = M3D::Min(m_movSpeed, dis / 800.0f);
		m_movSpeed = dis / 800.0f*m_firstPersionSpeed;
//		LOGI("m_movSpeed MoveStraight ======================%f",m_movSpeed);
		newPos = cameraPos + directionZ * (strSpeed) * m_movSpeed *0.3+directionX * (sidSpeed) * m_movSpeed *0.3;
	}
	else
#endif
	{
		newPos = cameraPos + directionZ * (strSpeed) * m_movSpeed*m_firstPersionSpeed +directionX * (sidSpeed) * m_movSpeed*m_firstPersionSpeed;
	}
	camera->SetWorldPosition(newPos);

//	this->m_pView->RequestDraw();
	m_isOribteRotateState = false;
}

void  WalkthroughHandler::ControlCloseRangeSpeed(const IntVector2& currentPnt)
{
	CameraNode* camera = m_SceneManager->GetCamera();
	Vector3 coordinate = m_insectPoint;
	Vector2 cur(currentPnt.m_x, currentPnt.m_y);
#ifdef BIMSPEED
	if (false)//配置参数放的位置，当小模型时就采用实时求交的方式
	{
		bool intersect = m_SceneManager->GetPickPoint(cur, coordinate, false);
	}
	else
	{
		Matrix3 viewMat = camera->GetView().ToMatrix3();
		Vector3 direction(viewMat.m_m20, 0.0f, viewMat.m_m22);
		direction = -direction; //反方向即为观察方向
		direction.Normalize();
		Vector3 newPos;
		if (m_insideBoundingBox)
		{
			float dis = GetDirectionDistance(direction, 0);
			m_movSpeed = M3D::Min(m_movSpeed, dis / 800.0f);
		}
		else
		{
			float dis = GetDirectionDistance(direction, 0);
//			LOGI("dis ==%f", dis);
			m_movSpeed = M3D::Min(m_movSpeed, dis / 10.0f);
		}
	}
#endif
	if(false)
	{
		float distance = (camera->GetPosition() - coordinate).Length();
		float movSpeed = distance / 8.0f;
		m_movSpeed = M3D::Max(m_standardSpeed * 0.1f, movSpeed);
	}

	//如果使用oribit摄像机
	if (this->m_useOribit)
	{
		updateOribitControlTarget();
	}
}

void WalkthroughHandler::TwoPointsMove(float*pos, int n)
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
				IntVector2 priPnt((m_PriPointTwo1 + m_PriPointTwo2) / 2);

				IntVector2 curPos1(pos[0], pos[1]);
				IntVector2 curPos2(pos[2], pos[3]);

				float priLength = (m_PriPointTwo1 - m_PriPointTwo2).Length();
				float curLength = (curPos1 - curPos2).Length();
				float deltaLength = M3D::Abs(priLength - curLength);
				float deltaStart = M3D::Abs(curLength - m_PirDistance);
				/*				LOGI("priLength ==== %f",priLength);
				 LOGI("curLength ==== %f",curLength);
				 LOGI("deltaLength ==== %f",deltaLength);
				 LOGI("deltaStart ==== %f",deltaStart);
				 LOGI("m_PirDistance ==== %f",m_PirDistance);*/
				/*				Vector2 direction((curPos2 - curPos1).m_x,
				 (curPos2 - curPos1).m_y);
				 Vector2 yDir(0.0f, 1.0f);
				 direction.Normalize();

				 float tempAngle = M3D::Acos(direction.DotProduct(yDir));
				 float angle = tempAngle < 90 ? tempAngle : 180.0f - tempAngle;*/

				//两指旋转
				/*				if (fabs(scaleFactor - 1.0f) < 0.1
				 && fabs(scaleFactor - 1.0f) > 0.03 && angle > 45.0f)
				 {
				 //	LOGI("Rotate it!!!");
				 TwoPointsRotate(camera, pos, currentPnt, priPnt);
				 //	m_PirDistance = m_CurrDistance;

				 }*/

				ControlCloseRangeSpeed(currentPnt);

				//前进后退
				if (/*fabs(scaleFactor - 1.0f) > 0.05 &&*/(deltaStart > 25.0f))
				{

					Vector3 movTarget =
							camera->GetViewPort().ScreenToWorldPoint(
									currentPnt.m_x, currentPnt.m_y,
									m_screenDepth);
					Vector3 cameraPos = camera->GetPosition();

					float moveFactor = m_movSpeed * scaleFactor
							* (scaleFactor > 1 ? -1 : 1);

					camera->SetWorldPosition(
							cameraPos
									+ (movTarget - cameraPos).Normalized()
											* moveFactor);

					m_PirDistance = m_CurrDistance;
				}
				//平移
				else if (/*fabs(scaleFactor - 1.0f) < 0.01 &&*/(deltaLength
						< 8.0f))
				{
					Vector2 priPosW(priPnt.m_x, priPnt.m_y);

					Vector3 priPickPoint ;
//					priPickPoint = GetPickPointCoord(m_SceneManager,
//												priPosW);
					priPickPoint = m_insectPoint;

					BoundingBox& pBoundingBox = m_SceneManager->GetSceneBox();
					Vector3 center = pBoundingBox.Center();

					Vector3 cameraPos = camera->GetPosition();
					Vector3 priPoint = camera->GetViewPort().ScreenToWorldPoint(
							priPnt.m_x, priPnt.m_y, m_screenDepth);
					Vector3 currentPoint =
							camera->GetViewPort().ScreenToWorldPoint(
									currentPnt.m_x, currentPnt.m_y,
									m_screenDepth);

					Vector3 priPoint1 =
							camera->GetViewPort().ScreenToWorldPoint(priPnt.m_x,
									priPnt.m_y, m_screenDepth / 2);
					Vector3 currentPoint1 =
							camera->GetViewPort().ScreenToWorldPoint(
									currentPnt.m_x, currentPnt.m_y,
									m_screenDepth / 2);

					float priLengthA = (cameraPos - priPoint).Length();

					float priLengthB = (cameraPos - priPickPoint).Length();

					float colLengthA = (priPoint - currentPoint).Length();

					float direLength = colLengthA * priLengthB / priLengthA;

					Vector3 directionPToC =
							(currentPoint - priPoint).Normalized();

					camera->SetWorldPosition(
							cameraPos - directionPToC * direLength);
					m_needMoveValue = -directionPToC * direLength;
				}

				m_TrackBall.mvMatrix.scaleFactor = 1.0f;
			}
		}
	}
	m_PriPointTwo1.m_x = pos[0];
	m_PriPointTwo1.m_y = pos[1];
	m_PriPointTwo2.m_x = pos[2];
	m_PriPointTwo2.m_y = pos[3];
}
    
void WalkthroughHandler::LimitCamera()
{
    if (m_SceneManager != NULL)
    {
        CameraNode* camera = m_SceneManager->GetCamera();
        if (!camera->IsOrthographic())
        {
            
        }
    }
}

void WalkthroughHandler::TwoPointsRotate(CameraNode * camera,float * pos,
		const IntVector2 &currentPnt,const IntVector2 &priPnt)
{
	Vector3 movTarget =
			camera->GetViewPort().ScreenToWorldPoint(
					currentPnt.m_x, currentPnt.m_y,
					m_screenDepth);

	Vector3 oriTarget =
			camera->GetViewPort().ScreenToWorldPoint(
					priPnt.m_x, priPnt.m_y,
					m_screenDepth);

	Vector3 cameraPos = camera->GetPosition();

	cameraPos = camera->GetWorldPosition();

	IntVector2 curPos1(pos[0], pos[1]);

	IntVector2 curPos2(pos[2], pos[3]);

	Vector3 cameraPositon;

	Matrix4 mat = Matrix4::IDENTITY;

	GetTwoPointsRotateMatrix(m_PriPointTwo1, m_PriPointTwo2, curPos1,
			curPos2, m_SceneManager,
			(movTarget+oriTarget )/2.0f, mat, m_screenDepth);

	mat = mat.Inverse();

	camera->Translate(mat.Translation(),TS_WORLD);

	camera->Rotate(mat.Rotation(),TS_WORLD);
}

void WalkthroughHandler::LockPointer()
{
	if(Parameters::Instance()->m_upDirectionValue != Vector3::ZERO)
	{
		SceneManager* scene = m_SceneManager;

		CameraNode* camera = scene->GetCamera();

		Vector3 pos = camera->GetPosition();
		BoundingBox& pBoundingBox = scene->GetSceneBox();
		Vector3 center = pBoundingBox.Center();

		Quaternion rotation = camera->GetWorldRotation();

		Vector3 eulerAngles = rotation.EulerAngles();

		float angleX = eulerAngles.m_x;

		angleX = Max(-85.0f,Min(85.0f,angleX));

//		LOGI("angleX is %f,Y is %f",angleX,eulerAngles.m_y);
		//约束模式
		if(m_controlLockXY)
		{
			Quaternion newRotation(m_phi,eulerAngles.m_y,0);
			camera->SetRotation(newRotation );
		}
		else
		{

			Quaternion newRotation(angleX,eulerAngles.m_y,0);

			camera->SetRotation(newRotation );
		}
	}

	Quaternion rotation = m_TrackBall.mvMatrix.rotation.Inverse() ;
	CameraNode * camera = m_SceneManager->GetCamera();
	camera->RotateAroundCenter(rotation,TS_WORLD);

	//m_SelectedNodes->Translate(m_TrackBall.mvMatrix.moveVector,
	//		m_TrackBall.mvMatrix.currPos);
	camera->Translate(m_TrackBall.mvMatrix.moveVector*-1, TS_LOCAL);
	//m_SelectedNodes->Zoom(m_TrackBall.mvMatrix.scaleFactor);
	camera->ZoomView(1 / m_TrackBall.mvMatrix.scaleFactor);
}

void WalkthroughHandler::ConstraintMode(bool value)
{
	if (value)
	{
		SceneManager* scene = m_SceneManager;

		CameraNode* camera = scene->GetCamera();

		Vector3 pos = camera->GetPosition();
		BoundingBox& pBoundingBox = scene->GetSceneBox();
		Vector3 center = pBoundingBox.Center();

		Quaternion rotation = camera->GetWorldRotation();

		Vector3 eulerAngles = rotation.EulerAngles();

		float angleX = eulerAngles.m_x;

		m_phi = angleX;

		m_oribitMode = false;
		m_freeViewMode = false;
	}
	m_controlLockXY = value;

}

void WalkthroughHandler::OribitMode(bool value)
{
	if(value)
	{
		m_controlLockXY = false;
		m_freeViewMode = false;
	}
	m_oribitMode = value;

}

void WalkthroughHandler::FreeViewMode(bool value)
{
	if(value)
	{
		m_oribitMode = false;
		m_controlLockXY = false;

	}
	m_freeViewMode = value;
}


void WalkthroughHandler:: TwoPointsRotateAngle(const IntVector2 & lastVector1,const IntVector2 & lastVector2,
		const IntVector2 & currentVector1,const IntVector2 & currentVector2,const CameraNode * camera,Vector3 & axis
		,float depth,float & angle)
{

	IntVector2 line1Left;
	IntVector2 line1Right;
	IntVector2 line2Left;
	IntVector2 line2Right;

	if(lastVector1.m_x<lastVector2.m_x)
	{
		line1Left = lastVector1;
		line1Right = lastVector2;
	}
	else
	{
		line1Left = lastVector2;
		line1Right = lastVector1;
	}

	if(currentVector1.m_x<currentVector2.m_x)
	{
		line2Left = currentVector1;
		line2Right = currentVector2;
	}
	else
	{
		line2Left = currentVector2;
		line2Right = currentVector1;
	}
	Vector3 line1LeftVec3 =
			camera->GetViewPort().ScreenToWorldPoint(
					line1Left.m_x, line1Left.m_y,
					depth);
	Vector3 line1RightVec3 =
			camera->GetViewPort().ScreenToWorldPoint(
					line1Right.m_x, line1Right.m_y,
					depth);
	Vector3 line2LeftVec3 =
			camera->GetViewPort().ScreenToWorldPoint(
					line2Left.m_x, line2Left.m_y,
					depth);
	Vector3 line2RightVec3 =
			camera->GetViewPort().ScreenToWorldPoint(
					line2Right.m_x, line2Right.m_y,
					depth);

	Vector3 line1Direction = line1RightVec3 - line1LeftVec3;
	line1Direction.Normalize();
	Vector3 line2Direction = line2RightVec3 - line2LeftVec3;
	line2Direction.Normalize();

	float tempAngle = M3D::Acos(line1Direction.DotProduct(line2Direction));

	angle = tempAngle < 90 ? tempAngle: 180.0f - tempAngle;

	axis = (line1Direction.CrossProduct(line2Direction)).Normalized();

}

void WalkthroughHandler::TwoPointsRotate(const IntVector2 & lastVector1,const IntVector2 & lastVector2,
		const IntVector2 & currentVector1,const IntVector2 & currentVector2,const CameraNode * camera
		,const Vector3 & target,const Vector3 & oldPos,Vector3 &cameraPositon,float depth)
{
	float angle = 0.0f;
	Quaternion rotateQuat;
	Vector3 rotateAxis;
	Vector3 tempPos = oldPos - target;
	TwoPointsRotateAngle(lastVector1, lastVector2,	currentVector1,currentVector2
			,camera,rotateAxis,depth, angle);

	rotateQuat.FromAngleAxis(angle,rotateAxis);
	rotateQuat.Normalize();
//	LOGI("target ==== %s",target.Tostring().c_str());
//	LOGI("rotateAxis ==== %s",rotateAxis.Tostring().c_str());
//	LOGI("angle ==== %f",angle);
	Quaternion oldQuat(0.0f,tempPos.m_x,tempPos.m_y,tempPos.m_z);

	Quaternion newQuat = rotateQuat * oldQuat * rotateQuat.Inverse();

	Vector3 rotateTemPos(newQuat.m_x,newQuat.m_y,newQuat.m_z);

	cameraPositon = rotateTemPos + target;
//	LOGI("cameraPositon ==== %s",cameraPositon.Tostring().c_str());


}

void WalkthroughHandler::GetTwoPointsRotateMatrix(const IntVector2 & lastVector1,const IntVector2 & lastVector2,
		const IntVector2 & currentVector1,const IntVector2 & currentVector2, SceneManager* scene
		,const Vector3 & target,Matrix4 & mat,float depth)
{
	float angle = 0.0f;
	Quaternion rotateQuat;
	Vector3 rotateAxis;

	CameraNode* camera = scene->GetCamera();
	TwoPointsRotateAngle(lastVector1, lastVector2,	currentVector1,currentVector2
			,camera,rotateAxis,depth, angle);
//	LOGI("angle ==== %f",angle);

	Vector3 coordinate;
	Vector2 screenPnt ((currentVector1+currentVector2).m_x/2.0f,(currentVector1+currentVector2).m_y/2.0f);
	bool intersect = scene->GetPickPoint(screenPnt,coordinate,true);
	if(intersect)
	{
	Matrix4 transMat;
	transMat.SetTranslation(-coordinate);

	rotateQuat.FromAngleAxis(angle,rotateAxis);

	transMat.MultiRotatiton(rotateQuat);

	transMat.MultiTranslate(coordinate);

	mat =transMat ;
	}

}

void WalkthroughHandler::OrbitControl()
{
	CameraNode * camera = m_SceneManager->GetCamera();
	Vector3 position = camera->GetWorldPosition();
	float theta ;
	float phi;
	Quaternion quat ;//= Quaternion(Vector3::UP,Vector3::Vector3::UP) ;//默认向上，不需要构建

	quat.Normalize();
	Vector3 offset;
	offset = position - m_oribitControlTarget;
//	LOGI("walk offset = %s,target = %s",offset.Tostring().c_str(),m_oribitControlTarget.Tostring().c_str());
	offset = quat * offset;
	theta = M3D::Atan2(offset.m_x,offset.m_z);
	phi = M3D::Atan2(sqrtf(offset.m_x*offset.m_x + offset.m_z*offset.m_z),offset.m_y);

	Quaternion rotation = m_TrackBall.mvMatrix.rotation;

	Vector3 eulerAngles = rotation.EulerAngles();
	Vector3 rotateAngle = eulerAngles ;

	theta -= rotateAngle.m_y;

	//m_phi = phi;
//	LOGI("walk phi = %f",phi);
	//约束模式
	if(m_oribitMode)
	{
		phi -= rotateAngle.m_x;

		phi = M3D::Max(0.1f, M3D::Min(180.0 - 0.1f, phi));
	}

	float radius = offset.Length();

	offset.m_x = radius * M3D::Sin(phi) * M3D::Sin(theta);
	offset.m_y = radius * M3D::Cos(phi);
	offset.m_z = radius * M3D::Sin(phi) * M3D::Cos(theta);

	offset = quat.Inverse()*offset ;
	position = m_oribitControlTarget + offset;//

	camera->SetWorldPosition(position);
	camera->LookAt(m_oribitControlTarget,quat*Vector3::UP);
//	LOGI("orbit");

}

/**
 * @brief 前后移动
 * @param speed
 */
void WalkthroughHandler::MoveStraight(float speed)
{
	if (Abs(speed) < 0.01)
	{
		return;
	}

	ControlCloseRangeByScreenCenter();

	CameraNode* camera = m_SceneManager->GetCamera();
	Vector3 cameraPos = camera->GetPosition();
	Matrix3 viewMat = camera->GetView().ToMatrix3();
	//	LOGI("viewMat === %s",viewMat.Tostring().c_str());

	Vector3 direction(viewMat.m_m20, 0.0f, viewMat.m_m22);

	direction = -direction; //反方向即为观察方向

	direction.Normalize();

	///为了满足BIM虚拟键的要求做的
	Vector3 newPos;
#ifdef BIMSPEED
	if(true||IsNeedUpdateSpeed(direction))
	{
//	VirtualKeyControlSpeed();
	}
	if (m_insideBoundingBox)
	{
		float dis = GetDirectionDistance(direction, 0);
//		m_movSpeed = M3D::Min(m_movSpeed, dis / 800.0f);
		m_movSpeed = dis / 800.0f*m_firstPersionSpeed;
//		LOGI("m_movSpeed MoveStraight ======================%f",m_movSpeed);
		newPos = cameraPos + direction * (speed) * m_movSpeed *0.3;
	}
	else
#endif
	{
		newPos = cameraPos + direction * (speed) * m_movSpeed*m_firstPersionSpeed;
	}
	camera->SetWorldPosition(newPos);

	this->m_pView->RequestDraw();
	m_isOribteRotateState = false;
}

/**
 * @brief 左右移动
 * @param speed
 */
void WalkthroughHandler::MoveSideways(float speed)
{
	if(Abs(speed)<0.01)
	{
		return;
	}

    ControlCloseRangeByScreenCenter();
    //
    CameraNode* camera = m_SceneManager->GetCamera();
    Vector3 cameraPos = camera->GetPosition();
    Matrix3 viewMat = camera->GetView().ToMatrix3();
    
    Vector3 direction(viewMat.m_m00,0.0f,viewMat.m_m02);//获取相机坐标系Z轴坐标
    
    direction = -direction;//反方向即为观察方向
    
    direction.Normalize();
    
    ///为了满足BIM虚拟键的要求做的2
    Vector3 newPos;
#ifdef BIMSPEED
	if(true||IsNeedUpdateSpeed(direction))
	{
//	VirtualKeyControlSpeed();
	}
    if(m_insideBoundingBox)
    {
    	float dis = GetDirectionDistance(direction,0);
//    	m_movSpeed = M3D::Min(m_movSpeed,dis/800.0f);///根据当前方向上的距离来指定最大速度
		m_movSpeed = dis / 800.0f*m_firstPersionSpeed;
 //   	LOGI("m_movSpeed MoveSideways ======================%f",m_movSpeed);
     newPos = cameraPos + direction*(speed)*m_movSpeed*0.3;
    }
    else
#endif
    {
    	 newPos = cameraPos + direction*(speed)*m_movSpeed*m_firstPersionSpeed;
    }
    camera->SetWorldPosition(newPos);
    
    this->m_pView->RequestDraw();
    m_isOribteRotateState = false;
}

void WalkthroughHandler::MoveUpAndDown(float speed)
{
	if (Abs(speed)<0.01)
	{
		return;
	}

	ControlCloseRangeByScreenCenter();

	//
	CameraNode* camera = m_SceneManager->GetCamera();
	Vector3 cameraPos = camera->GetPosition();
	Matrix3 viewMat = camera->GetView().ToMatrix3();

	//Vector3 direction(viewMat.m_m10, viewMat.m_m11, viewMat.m_m12);
	Vector3 direction = Vector3::UP;

	direction = -direction;//反方向即为观察方向

	direction.Normalize();

	///为了满足BIM虚拟键的要求做的2
	Vector3 newPos;
#ifdef BIMSPEED
	if (true || IsNeedUpdateSpeed(direction))
	{
		//	VirtualKeyControlSpeed();
	}
	if (m_insideBoundingBox)
	{
		float dis = GetDirectionDistance(direction, 0);
		//m_movSpeed = M3D::Min(m_movSpeed, dis / 800.0f);///根据当前方向上的距离来指定最大速度
		m_movSpeed = dis / 800.0f*m_firstPersionSpeed;
														//   	LOGI("m_movSpeed MoveSideways ======================%f",m_movSpeed);
		newPos = cameraPos + direction*(speed)*m_movSpeed*0.3;
	}
	else
#endif
	{
		newPos = cameraPos + direction*(speed)*m_movSpeed*m_firstPersionSpeed;
	}
	camera->SetWorldPosition(newPos);

	this->m_pView->RequestDraw();
	m_isOribteRotateState = false;
}

/**
 * @brief 上下旋转
 * @param degree
 */
void WalkthroughHandler::RotateUpAndDown (float degree)
{
	if(Abs(degree)<0.01)
	{
		return;
	}

	CameraNode * camera = m_SceneManager->GetCamera();
	Matrix3 viewMat = camera->GetView().ToMatrix3();
	Vector3 rotateAxis(viewMat.m_m10,viewMat.m_m11,viewMat.m_m12);//获取相机坐标系Z轴坐标
	Quaternion rotation = camera->GetWorldRotation();

	Vector3 eulerAngles = rotation.EulerAngles();

	float angleX = eulerAngles.m_x+degree;
	angleX = Max(-85.0f,Min(85.0f,angleX));
	Quaternion newRotation(angleX,eulerAngles.m_y,0);
    
    this->m_pView->RequestDraw();
	camera->SetRotation(newRotation );
}

/**
 * @brief 左右旋转
 * @param degree
 */
void WalkthroughHandler::RotateSideways (float degree)
{
	if(Abs(degree)<0.01)
	{
		return;
	}

	CameraNode * camera = m_SceneManager->GetCamera();
	Matrix3 viewMat = camera->GetView().ToMatrix3();
	Vector3 rotateAxis(viewMat.m_m10,viewMat.m_m11,viewMat.m_m12);//获取相机坐标系Z轴坐标
	Quaternion rotation = camera->GetWorldRotation();

	Vector3 eulerAngles = rotation.EulerAngles();

	float angleY = eulerAngles.m_y - degree;

	Quaternion newRotation(eulerAngles.m_x,angleY,0);
    
    this->m_pView->RequestDraw();
	camera->SetRotation(newRotation );
}

void WalkthroughHandler::SetVRMode(bool vrMode)
{
	this->m_vrModel = vrMode;
}


void WalkthroughHandler::updateOribitControlTarget()
{
	//根据屏幕中心点，构造射线和模型交点，作为oribit的目标点
    const Viewport& viewport = m_SceneManager->GetCamera()->GetViewPort();
    const IntRect& rect = viewport.GetRect();
    Vector2 screenCenter((rect.m_left+rect.m_right)/2, (rect.m_bottom+rect.m_top)/2);

    Vector3 intersectPnt;
    bool intersect = m_SceneManager->GetPickPoint(screenCenter,
                                                    intersectPnt, false);
    m_oribitControlTarget = intersectPnt;
}

/**
 * @brief 设置相机视角
 * @param fov
 */
void WalkthroughHandler::SetCameraFov(float fov)
{
	SceneManager* scene = m_SceneManager;
	CameraNode* camera = scene->GetCamera();
	camera->SetFov(fov);
    
    this->m_pView->RequestDraw();
}

#ifdef BIMSPEED
/*
 * 只有摄像机在内部时才使用，粗略计算摄像机方向距离，通过与传入的mark代表的世界坐标系来指定
 * 如果需要精确获得需要真正计算与包围盒交点来指定的。返回的是距离
 */
float WalkthroughHandler::GetDirectionDistance(Vector3& direction,int mark)
{
	float ret = 0.0f;

	BoundingBox& sceneBox = m_SceneManager->GetSceneBox();

	switch(mark)
	{
	case 0:
	{
		float theta = M3D::Abs(direction.DotProduct(Vector3::RIGHT));
		if(theta>0.7071)
		{
			ret = sceneBox.GetXLength();
		}
		else
		{
			ret = sceneBox.GetZLength();
		}
	}
		break;
	case 3:
		float theta = M3D::Abs(direction.DotProduct(Vector3::FORWARD));
		if(theta>0.7071)
		{
			ret = sceneBox.GetZLength();
		}
		else
		{
			ret = sceneBox.GetXLength();
		}
		break;
	}


	return ret;
}
#endif

}
