#include <math.h>
#include "sview/views/Parameters.h"
#include "TouchHandler.h"
#include "m3d/SceneManager.h"
#include "m3d/RenderManager.h"
#include "m3d/utils/CullerHelper.h"
#include "sview/views/View.h"
#include "sview/extern/PerspectiveData.h"
#include "sview/extern/PerspectiveOperator.h"
#include "m3d/Handler/HandlerPoint.h"
#include "m3d/handler/dragger.h"
#include "m3d/scene/ScreenUILayerGroup.h"


#define EBUILDING
namespace SVIEW
{

#ifdef EBUILDING
bool TouchHandler::m_freeViewMode = true;

bool TouchHandler::m_controlLockXY = false;

bool TouchHandler::m_oribitMode = false;
#endif

TouchHandler::TouchHandler()
{
	m_KeepRotateTimer.SetTimer(TimerTask, this, 0, 144);
	m_screenDepth = 0.001;
	//m_SelectedNodes = NULL;
	m_SceneManager = NULL;
	m_pView = NULL;

	m_freeViewMode = true;

	m_controlLockXY = false;
	m_oribitMode = false;
	m_needInitDirection = true;
	m_oldCameraPos = Vector3(0.0f, 0.0f, 0.0f);
	m_theta = 0.0f;
	m_touchHandlerType = HANDLER_WALKTHROUGH;
	m_firstPersionSpeed = 1.0f;
	m_changeDisOnRotate = true;
	m_birdEyeSpeed = 1.0f;
	//m_upDirection = Vector3::UP;
	m_modelRotation = Quaternion::IDENTITY;
	m_standardSpeed = 60.0f;

	m_controlLockXY = true;

	m_modelRotateAngle = 0.0f;

	m_showModeRotAngle = 0.5f;
	m_rotCircleCount = 0;
	m_showModeSpeed = 1.0;

	SetDefaultView(PerspectiveData::ISOMETRIC);

	this->SetRestoreCamera(true);
}
void TouchHandler::SetUpDirection(Vector3 & up, View * view)
{
	view->GetPerspectiveOperator()->FinishAnimation();

	Parameters::Instance()->m_upDirectionValue = up;
	SceneManager* scene = m_SceneManager;

	SceneNode* modelRootNode = scene->GetSceneRoot()->Search(MAINMODELROOT);
	if (!modelRootNode)
	{
		return;
	}
	Quaternion modelRotation(Vector3::UP, Parameters::Instance()->m_upDirectionValue);
	BoundingBox& pBoundingBox0 = scene->GetSceneBox();
	pBoundingBox0.Clear();
	BoundingBox& pBoundingBox = scene->GetSceneBox();
	Vector3 center = pBoundingBox.Center();

	Matrix3x4 trans1, trans2, rot1;
	trans1.MultiTranslate(-center);
	rot1.MultiScale(1.0 / view->GetUnitScale());
	rot1.MultiRotatiton(modelRotation);	
	trans2.MultiTranslate(center);
	Matrix3x4 tempmat = trans2 * rot1 * trans1;
	Matrix3x4 plcMat = tempmat;

	Vector3 centerScale = center * 1.0 / view->GetUnitScale();
	Matrix3x4 transScale;
	transScale.MultiTranslate(centerScale);

	plcMat = transScale * trans1*plcMat;
 
	//modelRootNode->SetOrigPlcMatrix(plcMat);
	m_setUpPlcMat = plcMat;

	if (this->m_SceneManager->GetModel())
	{
		map<int, PMIData*>* mpis = scene->GetModel()->GetPMIs();
		if (mpis)
		{
			map<int, PMIData*>::iterator it = mpis->begin();
			for (;it!= mpis->end();++it)
			{
				it->second->MarkDefinePlaneDirty();
			}
		}
		this->m_SceneManager->GetModel()->SetPlaceMatrix(m_setUpPlcMat);
		this->m_SceneManager->GetModel()->SetOrigPlcMatrix(m_setUpPlcMat);
	}

	SceneNode* groundNode = scene->GetSceneRoot()->Search(M3D::SCENEGROUND);
	groundNode->SetRotation(modelRotation);
	
	pBoundingBox.Clear();

	this->m_SceneManager->RequestUpdateWhenSceneBoxChanged();
	this->m_SceneManager->OptimizeCameraView(true);
}

void TouchHandler::OribitMode(bool value)
{

}

void TouchHandler::ConstraintMode(bool value)
{

}

void TouchHandler::FreeViewMode(bool value)
{

}

M3D::Vector3 TouchHandler::GetCurrentUpDirection()
{
	return  Parameters::Instance()->m_upDirectionValue;
}

void TouchHandler::OnUpDataTouchIntent()
{

}

void TouchHandler::SetView(View* pView)
{
	this->m_pView = pView;
	this->m_SceneManager = pView->GetSceneManager();
	m_KeepRotateTimer.SetTimer(TimerTask, this, 0, 36);
	this->m_TrackBall.SetSceneManager(m_SceneManager);
}

TouchHandler::~TouchHandler()
{
	if (m_ModelRotateOnFixedPointTimer.IsStart())
	{
		m_ModelRotateOnFixedPointTimer.StopTimer();
	}
}
void TouchHandler::HandleTouchEvent(float* p, int n, int touchType)
{
	HandleTouchEvent(p, n, touchType, 0);
}

void TouchHandler::HandleTouchEvent(float* p, int n, int touchType,
		int moveType)
{
	this->m_Width = m_SceneManager->GetRenderManager()->GetWindowWidth();
	this->m_Height = m_SceneManager->GetRenderManager()->GetWindowHeight();

	//如果托拽器处理了时间，则返回
	if (this->HandleDragger(p,n,touchType,moveType))
	{
		return;
	}

	//this->m_SelectedNodes = m_pView->GetWorkNodes();

	switch (touchType)
	{
	case TOUCHDOWN:
		TouchDown(p, n);
		break;
	case TOUCHMOVE:
		TouchMove(moveType, p, n);
		break;
	case TOUCHUP:
		TouchUp(p, n);
		break;
	default:
		break;
	}
}

bool TouchHandler::HandleDragger(float* p, int n, int touchType, int moveType)
{
	TouchEvent touchEvent(p,n);
	touchEvent.SetView(m_pView);
	if (m_pView)
	{
		switch (touchType)
		{
		case TOUCHDOWN:
			touchEvent.setEventType(TouchEvent::PUSH);
			break;
		case TOUCHMOVE:
			touchEvent.setEventType(TouchEvent::DRAG);
			break;
		case TOUCHUP:
			touchEvent.setEventType(TouchEvent::RELEASE);
			break;
		default:
			break;
		}
 
		m_pView->GetSceneManager()->GetHandlerGroup()->HandleDragger(touchEvent);
	}

	return touchEvent.getHandled();
}



void TouchHandler::CloseKeepState()
{
	this->EndKeepState();
}

void TouchHandler::Open(bool useAni)
{
	this->InitCamera(useAni);
}

void TouchHandler::Close()
{

}

void TouchHandler::InitCamera(bool useAni)
{

}
void TouchHandler::OptimizeCamera()
{

}

void TouchHandler::OnTouchUp(float* p, int n)
{

}

void TouchHandler::OnTouchMove(int moveType, float* p, int n)
{

}

void TouchHandler::OnTouchDown(float* p, int n)
{

}

void TouchHandler::TouchUp(float* p, int n)
{
	this->OnTouchUp(p, n);
	this->m_pView->RequestDraw();
}
void TouchHandler::TouchMove(int moveType, float* p, int n)
{
	this->OnTouchMove(moveType, p, n);
	this->m_pView->RequestDraw();
}
void TouchHandler::TouchDown(float* p, int n)
{
	//LOGI("Touch down %d",n);
	this->OnTouchDown(p, n);
	this->m_pView->RequestDraw();
}

void TouchHandler::UpdateRenderQuality(bool isMoving)
{
	SceneManager* senceMgr = this->m_SceneManager;
	if (senceMgr != NULL)
	{
		RenderManager* renderMgr = senceMgr->GetRenderManager();
//		if (renderMgr)
//		{
//			//bool useLowQuality = isMoving ? true : false;
//			renderMgr->SetUseLowQuality(true);
//		}

		bool showRotateCenter = (isMoving ? true : false)
				& Parameters::Instance()->m_isShowRotateCenter;

		if (this->GetTouchHandlerType() == HANDLER_WALKTHROUGH)
		{
			showRotateCenter = false;
		}

		HandlerGroup* handlerGroup =
				(HandlerGroup*) senceMgr->GetSceneRoot()->Search(
						HANDLER_GROUPNODE);

		if (handlerGroup)
		{
			if (showRotateCenter)
			{
				Vector3 rotateCenter = senceMgr->GetCamera()->GetRotateCenter();
				handlerGroup->UpdateRotateCenterPos(rotateCenter);
			}
			handlerGroup->SetRotateCenterVisible(showRotateCenter);
		}
	}
}

bool TouchHandler::StateChanged()
{
	if (fabs(m_TrackBall.mvMatrix.rotation.LengthSquared()) < 0.01
			&& (m_TrackBall.mvMatrix.moveVector.Length()) < 0.02
			&& fabs(m_TrackBall.mvMatrix.scaleFactor - 1) < 0.05)
	{
		return false;
	}
	return true;
}

bool TouchHandler::IsModelInScene(Model* model)
{
	bool ret = false;
	if (!model)
	{
		return ret;
	}

	BoundingBox modelBox;
	modelBox = model->GetWorldBoundingBox();
	BoundingBox& sceneBox = this->m_pView->GetSceneManager()->GetSceneBox();
	if (sceneBox.IsInside(modelBox) != INSIDE)
	{
		ret = true;
	}

	return ret;
}

void TouchHandler::UpDataTouchIntent()
{
	this->UpDataTouchIntentWithSpeedControl(1.0f);
}

void TouchHandler::UpDataTouchIntentWithSpeedControl(float speedControl)
{
	OnUpDataTouchIntent();
	this->m_SceneManager->ShowRotationCenter(Trackball::ISROTATING);
	this->m_SceneManager->GetRenderManager()->SetUseLowQuality(
			Trackball::ISROTATING || Trackball::MOVESTATE != -1);
}
void TouchHandler::KeepState()
{
	if (StateChanged())
	{
		UpDataTouchIntentWithSpeedControl(0.3f);
		this->m_pView->RequestDraw();
		this->m_pView->RequestUILayerDrawCB(1);
	}

}

void TouchHandler::StartKeepState()
{
	if (!this->m_KeepRotateTimer.IsStart())
	{
		//LOGE("start timer");
		this->m_KeepRotateTimer.StartTimer();
	}
}

void TouchHandler::EndKeepState()
{
	if (this->m_KeepRotateTimer.IsStart())
	{
		//LOGE("end timer");
		this->m_KeepRotateTimer.StopTimer();

	}
}

void* TouchHandler::TimerTask(void* data)
{
	TouchHandler* touchHandle = (TouchHandler*) data;
	touchHandle->KeepState();
	return NULL;
}

void TouchHandler::SetRestoreCamera(bool restoreCamera)
{
	this->m_restoreCamera = restoreCamera;
}

bool TouchHandler::GetRestoreCamera()
{
	return this->m_restoreCamera;
}

void TouchHandler::Reset()
{
	this->EndKeepState();
	EndRotateOnFixedPoint();
	m_TrackBall.Reset();
	UpdateRenderQuality(false);
	this->m_pView->RequestDraw();
}

int TouchHandler::GetDefaultView()
{
	return Parameters::Instance()->m_defaultViewType;
}

void TouchHandler::SetDefaultView(int viewType)
{
	//this->m_defaultView = viewType;
	Parameters::Instance()->m_defaultViewType = viewType;
}

void TouchHandler::StartRotateOnFixedPoint()
{
	LOGI(" TouchHandler::StartRotateOnFixedPoint BEGIN");
	if (m_RotateOnFixedPointTimer.IsStart())
	{
		LOGI("HAVE START");
		m_RotateOnFixedPointTimer.StopTimer();
		m_needInitDirection = true;
		m_theta = 0.0f;
	}
	m_needInitDirection = true;
	m_theta = 0.0f;
	m_RotateOnFixedPointTimer.SetTimer(GetRotateOnFixedPointFunc, this, 0, 36);
	m_RotateOnFixedPointTimer.StartTimer();

	LOGI(" TouchHandler::StartRotateOnFixedPoint END");

}

void TouchHandler::EndRotateOnFixedPoint()
{
	if (m_RotateOnFixedPointTimer.IsStart())
	{
		m_RotateOnFixedPointTimer.StopTimer();
	}
	m_needInitDirection = true;
	m_theta = 0.0f;
}

void TouchHandler::SetBirdEyeParameter(int pointId, float speed)
{
	LOGI("TouchHandler::SetBirdEyeParameter");
	m_birdEyeSpeed = speed;
	IShape* firstShape = this->m_SceneManager->GetShape(pointId);
	if (firstShape && firstShape->GetType() == SHAPE_POINT_HANDLE)
	{
		LOGI("TouchHandler::SetBirdEyeParameter Get POSITION");
		HandlerPoint* firstPnt = (HandlerPoint*) firstShape;
		m_birdEyeFixedPnt = firstPnt->GetPosition();
	}
	else
	{
		m_birdEyeFixedPnt = this->m_SceneManager->GetSceneBox().Center();
	}
	m_changeDisOnRotate = false;
}

void TouchHandler::SetBirdEyeSpeed(float speed)
{
	LOGI("TouchHandler::SetBirdEyeSpeed");
	m_birdEyeSpeed = speed;
}

void TouchHandler::SetFixedPointRotateParameter(int pointId, float speed)
{
	LOGI("TouchHandler::SetFixedPointRotateParameter");
	m_birdEyeSpeed = speed;

	IShape* firstShape = this->m_SceneManager->GetShape(pointId);

	if (firstShape && firstShape->GetType() == SHAPE_POINT_HANDLE)
	{
		HandlerPoint* firstPnt = (HandlerPoint*) firstShape;

		m_birdEyeFixedPnt = firstPnt->GetPosition();
	}
	else
	{
		m_birdEyeFixedPnt = this->m_SceneManager->GetSceneBox().Center();
	}

	m_changeDisOnRotate = true;
}

void TouchHandler::SetFirstPersionParameter(float speed)
{
	LOGI("TouchHandler::SetFirstPersionParameter");
	BoundingBox& pBoundingBox = m_SceneManager->GetSceneBox();
	this->m_firstPersionSpeed = speed;
	this->m_standardSpeed = m_firstPersionSpeed * pBoundingBox.Length()
			/ 500.0f;
	LOGI("m_firstPersionSpeed === %f ", speed);
}

void * TouchHandler::GetRotateOnFixedPointFunc(void * data)
{
//LOGI(" TouchHandler::GetRotateOnFixedPointFunc BEGIN");
	TouchHandler* touchHandle = (TouchHandler*) data;
	touchHandle->RotateOnFixedPoint();
//LOGI(" TouchHandler::GetRotateOnFixedPointFunc END");
	return NULL;
}

void TouchHandler::RotateOnFixedPoint()
{
	if (!this->m_SceneManager)
	{
		return;
	}
	SceneManager* scene = m_SceneManager;
	CameraNode* camera = scene->GetCamera();
	Vector3 rotateAxis = Vector3::UP;
	if (m_needInitDirection)
	{
		Vector3 camPos = camera->GetWorldPosition();
		m_oldCameraPos = camPos - m_birdEyeFixedPnt;
		m_rotateRadius = m_oldCameraPos.Length();
		m_needInitDirection = false;
	}

	Quaternion rotateQua(m_theta, rotateAxis);
	rotateQua.Normalize();
	Quaternion oldQua(0.0f, m_oldCameraPos.m_x, m_oldCameraPos.m_y,
			m_oldCameraPos.m_z); //“™–˝◊™µƒµ„∂‘”¶µƒÀƒ‘™ ˝
	Quaternion newQua = rotateQua * oldQua * rotateQua.Inverse(); //«Û–˝◊™∫Ûµƒµ„∂‘”¶µƒÀƒ‘™ ˝
	Vector3 newPos(newQua.m_x, newQua.m_y, newQua.m_z); //–˝◊™∫Ûµƒµ„
	if (!m_changeDisOnRotate)
	{
		camera->SetWorldPosition(newPos + m_birdEyeFixedPnt);
	}
	else
	{
		camera->SetWorldPosition(
				newPos + m_birdEyeFixedPnt
						+ newPos.Normalized() * M3D::Sin(m_theta * 2.0f)
								* m_rotateRadius / 5.0f);
	}
	camera->LookAt(m_birdEyeFixedPnt, rotateAxis);
	m_theta = m_theta + m_birdEyeSpeed;
	if ((m_theta - 360.0f > -0.0000001 && m_theta - 360.0f < 0.00000001)
			|| (m_theta > 360.0f))
	{
		m_theta = m_theta - 360.0f;
	}

	this->m_pView->RequestDraw();
	this->m_pView->RequestUILayerDrawCB(1);
}

Vector3 TouchHandler::GetPickPointCoord(SceneManager * scene,
		Vector2 & screenPos)
{
	CameraNode* camera = scene->GetCamera();
	Vector3 coordinate;

	bool intersect = m_SceneManager->GetPickPoint(screenPos, coordinate, false);
	return coordinate;

}
float TouchHandler::GetBoxLength(float mark)
{
	BoundingBox& sceneBox = m_SceneManager->GetSceneBox();
	float XLength = sceneBox.GetXLength();
	float YLength = sceneBox.GetYLength();
	float ZLength = sceneBox.GetZLength();
	if (mark == 0)//表示平面前进的距离
	{
		return (XLength > YLength)?XLength:YLength;
	}
	else if(mark == 1)
	{
		return ZLength;
	}
    return -1;
}
//TODO 德方要求
void TouchHandler::VerticalScreenRotation(Vector2 & screenPos,float angle)
{
    CameraNode * camera = this->m_SceneManager->GetCamera();
    Vector3 priCenter = camera->GetRotateCenter();
    const Viewport & viewPort = camera->GetViewPort();
    Vector3 worldPos = viewPort.ScreenToWorldPoint(screenPos.m_x,screenPos.m_y,0.5f);
    camera->SetRotateCenter(worldPos);
    camera->RotateAroundCenter(Quaternion(1,0.0,0.0,sinf(angle*0.5f)),TS_WORLD);
    camera->SetRotateCenter(priCenter);
}

void TouchHandler::TwoFingersRotation(float* p, int n)
{
    if (n<2)
    {
        return;
    }
    Vector2 priPointTwo1(this->m_PriPointTwo1.m_x,this->m_PriPointTwo1.m_y),
    		priPointTwo2(this->m_PriPointTwo2.m_x,this->m_PriPointTwo2.m_y);
    Vector2 curPos1(p[0],p[1]);
    Vector2 curPos2(p[2],p[3]);

    Vector2 v1  = priPointTwo1 - priPointTwo2;
    Vector2 v2 = curPos1 - curPos2;

    float v1L = v1.Length();
    float v2L = v2.Length();

    if(v1L+v2L < SVIEW::Parameters::Instance()->GetLimitDistance())
    {
    	return;
    }

    Vector2 centerPoint = (priPointTwo1+priPointTwo2)*0.5;
    float cosAlpha = v1.DotProduct(v2)/(v1L*v2L);

    if(cosAlpha>1.0f)
    {
    	cosAlpha = 1.0f;
    }

    float dAngle = acosf(cosAlpha);
    v1.Normalize();
    v2.Normalize();
    Vector2 v2Vec(v2.m_y,-v2.m_x);
    float vDot = v1.DotProduct(v2Vec);
    if(vDot<=0)
    {
    	dAngle = -dAngle;
    }

    CameraNode * camera = this->m_SceneManager->GetCamera();
    Vector3 priCenter = camera->GetRotateCenter();
    const Viewport & viewPort = camera->GetViewPort();
    Vector3 worldPos = viewPort.ScreenToWorldPoint(centerPoint.m_x,centerPoint.m_y,m_screenDepth);
    camera->SetRotateCenter(worldPos);
    camera->RotateAroundCenter(Quaternion(1,0.0,0.0,sinf(dAngle*0.5f)),TS_WORLD);
    camera->SetRotateCenter(priCenter);
}
void TouchHandler::StartModelRotateOnFixedPoint() {
	LOGI(" TouchHandler::StartRotateOnFixedPoint BEGIN");
	if (m_ModelRotateOnFixedPointTimer.IsStart())
	{
		LOGI("HAVE START");
		m_ModelRotateOnFixedPointTimer.StopTimer();
//            		m_needInitDirection = true;
//            m_modelRotateAngle = 0.0f;
	}
//        	m_needInitDirection = true;
//        m_modelRotateAngle = 0.0f;
	#ifdef WIN32
	m_ModelRotateOnFixedPointTimer.SetTimer(GetModelRotateOnFixedPointFunc,
			this, 0, 36000);
			#else
 	m_ModelRotateOnFixedPointTimer.SetTimer(GetModelRotateOnFixedPointFunc,
			this, 0, 36);
#endif
	m_ModelRotateOnFixedPointTimer.StartTimer();
	LOGI(" TouchHandler::StartRotateOnFixedPoint END");
}
void TouchHandler::EndModelRotateOnFixedPoint()
{
	if (m_ModelRotateOnFixedPointTimer.IsStart())
	{
		m_ModelRotateOnFixedPointTimer.StopTimer();
	}
	//	m_needInitDirection = true;
//        m_modelRotateAngle = 0.0f;
}
void TouchHandler::ModelRotateOnFixedPoint()
{
//        LOGI(" TouchHandler::ModelRotateOnFixedPoint");
	//this->m_isUpDirection = open;
	SceneManager* scene = m_SceneManager;

	SceneNode* modelRootNode = scene->GetSceneRoot()->Search(MAINMODELROOT);
	if (!modelRootNode)
	{
		return;
	}
	Quaternion modelRotation(m_modelRotateAngle, Vector3::UP);
	BoundingBox& pBoundingBox = scene->GetSceneBox();
	Vector3 center = pBoundingBox.Center();

	Matrix3x4 trans1, trans2, rot1;
	trans1.MultiTranslate(-center);
	rot1.MultiRotatiton(modelRotation);
	trans2.MultiTranslate(center);
	Matrix3x4 tempmat = trans2 * rot1 * trans1;

	Matrix3x4 plcMat = m_setUpPlcMat * tempmat;

	//	modelRootNode->SetOrigPlcMatrix(plcMat);
	modelRootNode->SetPlcMatrix(plcMat);
	m_modelRotateAngle = m_modelRotateAngle + m_birdEyeSpeed;
	if ((m_modelRotateAngle - 360.0f > -0.0000001
			&& m_modelRotateAngle - 360.0f < 0.00000001)
			|| (m_modelRotateAngle > 360.0f))
	{
		m_modelRotateAngle = m_modelRotateAngle - 360.0f;
	}
	m_pView->RequestUILayerDrawCB(1);
	
}

void TouchHandler::CameraRotateOnFixedPoint()
{
	Quaternion quat(1,0.0,sinf(-0.01*m_birdEyeSpeed/2),0.0);

	this->m_SceneManager->GetCamera()->RotateAroundCenter(quat,TS_WORLD);

	this->m_modelRotateAngle += -0.01 * this->m_birdEyeSpeed;

	if(this->m_modelRotateAngle >= 2* M3D::M_PI)
	{
		m_modelRotateAngle = 0.0f;
	}
    m_pView->RequestUILayerDrawCB(1);
    m_pView->RequestDraw();
}

void * TouchHandler::GetModelRotateOnFixedPointFunc(void * data) {
	//LOGI(" TouchHandler::GetRotateOnFixedPointFunc BEGIN");
	TouchHandler* touchHandle = (TouchHandler*) data;
//	touchHandle->ModelRotateOnFixedPoint();
	touchHandle->CameraRotateOnFixedPoint();
	//LOGI(" TouchHandler::GetRotateOnFixedPointFunc END");
	return NULL;
}
void TouchHandler::OnlyRotateOnAxisFunc(void * data)
{
	TouchHandler* touchHandle = (TouchHandler*)data;
	touchHandle->m_SceneManager->GetCamera()->
		RotateAroundCenter( Quaternion(1, 0.0, sin(-0.02 * touchHandle->m_showModeSpeed / 2), 0.0), TS_WORLD);
}
void TouchHandler::GetRotateOnScreenMiddleLIneFunc(void * data)
{
	TouchHandler* touchHandle = (TouchHandler*)data;
	touchHandle->m_SceneManager->GetCamera()->
		RotateAroundCenter(Quaternion(1, 0.0, sin(-0.02 * touchHandle->m_showModeSpeed / 2), 0.0), TS_WORLD);
	touchHandle->m_showModeRotAngle += 0.02*touchHandle->m_showModeSpeed;
	if (touchHandle->m_showModeRotAngle >= 2 * 3.1415926f ) {
		touchHandle->m_showModeRotAngle = 0;
		touchHandle->m_rotCircleCount++;
		if (touchHandle->m_rotCircleCount ==99999) {
			touchHandle->m_rotCircleCount = 0;
		}
	}
}
void TouchHandler::SetShowModeSpeed(float speed)
{
	m_showModeSpeed = speed;
}

void TouchHandler::MoveUpAndDown(float speed)
{
	if (Abs(speed)<0.01)
	{
		return;
	}
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
	float m_movSpeed = GetBoxLength(1)/500.0f;
	{
		newPos = cameraPos + direction*(speed)*m_movSpeed*m_firstPersionSpeed;
	}
	camera->SetWorldPosition(newPos);
	this->m_pView->RequestDraw();
}
void TouchHandler::VirtualKeyMove(float strSpeed, float sidSpeed)
{

	if (Abs(strSpeed) < 0.01 && Abs(sidSpeed) < 0.01)
	{
		return;
	}
	CameraNode* camera = m_SceneManager->GetCamera();
	Vector3 cameraPos = camera->GetPosition();
	Matrix3 viewMat = camera->GetView().ToMatrix3();
	//	LOGI("viewMat === %s",viewMat.Tostring().c_str());

	Vector3 directionZ(viewMat.m_m20, 0.0f, viewMat.m_m22);

	Vector3 directionX(viewMat.m_m00, 0.0f, viewMat.m_m02);//获取相机坐标系Z轴坐标

	directionZ = -directionZ; //反方向即为观察方向
	directionZ.Normalize();

	directionX = -directionX; //反方向即为观察方向
	directionX.Normalize();

	Vector3 direction = directionZ + directionX;
	direction.Normalize();
	float m_movSpeed = GetBoxLength(0)/800.0f;
	Vector3 newPos;
	{
		newPos = cameraPos + directionZ * (strSpeed)* m_movSpeed*m_firstPersionSpeed + directionX * (sidSpeed)* m_movSpeed*m_firstPersionSpeed;
	}
	camera->SetWorldPosition(newPos);

}
void TouchHandler::WalkAroundTarget(float speed)
{
	this->m_SceneManager->GetCamera()->
		RotateAroundCenter(Quaternion(1, 0.0, sin(-0.02 * speed / 2), 0.0), TS_WORLD);
}
void TouchHandler::ResetViewCamera()
{
}
bool TouchHandler::GetConstraintMode()
{
	return m_controlLockXY;
}
}
