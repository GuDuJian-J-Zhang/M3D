#include "m3d/utils/Trackball.h"

#include "m3d/base/MathDefs.h"
#include "m3d/base/Vector3.h"
#include "m3d/graphics/CameraNode.h"
#include "m3d/scene/SceneNode.h"
#include "m3d/SceneManager.h"

#include "m3d/utils/CullerHelper.h"
#include "sview/views/Parameters.h"
using namespace SVIEW;

namespace M3D
{
bool Trackball::ISMOVING = false;
bool Trackball::ISROTATING = false;
int Trackball::MOVESTATE = -1;
int Trackball::TIMES = 10;
int Trackball::KEEPINGSTATETIMES = Trackball::TIMES;
float Trackball::CURRENTMODELSIZE = 1000.0f;
float Trackball::DrawLimit = 0.0f;
Trackball::Trackball(void)
{
	m_iWidth = m_iHeight = 400;
	m_iCenterX = m_iCenterY = 200;
	m_iRadius = 200;

	m_Start.m_x = 0;
	m_Start.m_y = 0;
	m_Start.m_z = 1;

	m_angle = 0;
	m_axis = Vector3(0,0,1);

	ISMOVING = false;
	DrawLimit = 0.0f;

	m_rotateSpeed = 1.5f;

	m_screenDepth = 0.5;
}

Trackball::~Trackball(void)
{
	ISMOVING = false;
	DrawLimit = 0;
}

void Trackball::SetTrackWindow(int iWidth, int iHeight)
{
	m_iWidth = iWidth;
	m_iHeight = iHeight;
	m_iCenterX = m_iWidth / 2;
	m_iCenterY = m_iHeight / 2;
	m_iRadius = (m_iWidth > m_iHeight) ? m_iCenterY : m_iCenterX;
}

void Trackball::SetTrackWindow(int iWidth, int iHeight, int iCenterX,
		int iCenterY)
{
	m_iWidth = iWidth;
	m_iHeight = iHeight;
	m_iCenterX = iCenterX;
	m_iCenterY = iCenterY;
	m_iRadius = (m_iWidth > m_iHeight) ? m_iCenterY : m_iCenterX;
}

Vector3 Trackball::ScreenToVector(int iMouseX, int iMouseY)
{
	Vector3 V(0, 0, 0);

	if (m_iCenterX >= m_iWidth / 2)
		V.m_x = float(iMouseX - m_iCenterX) / float(m_iCenterX);
	else
		V.m_x = float(iMouseX - m_iCenterX) / float(m_iWidth - m_iCenterX);

	if (m_iCenterY >= m_iHeight / 2)
		V.m_y = float(m_iCenterY - iMouseY) / float(m_iCenterY);
	else
		V.m_y = float(m_iCenterY - iMouseY) / float(m_iHeight - m_iCenterY);

	float d = float(sqrt(V.m_x * V.m_x + V.m_y * V.m_y));
	V.m_z = float(cos((3.14159265 / 2.0) * ((d < 1.0) ? d : 1.0)));
	V.Normalize();
	return V;
}

void Trackball::Tracking(int iDx, int iDy, Vector3 *axis, float *fAngle)
{
	if (Abs(iDx) < 1 && Abs(iDy) < 1)
	{
		*fAngle = 0.0f;
		return;
	}
 
	m_Stop = ScreenToVector(m_iWidth / 2 + iDx, m_iHeight / 2 + iDy);
	*axis = m_Start.CrossProduct(m_Stop); // cross product

	*fAngle = float(m_rotateSpeed* acos(m_Start.DotProduct(m_Stop)) / 3.14159 * 180.0);

	const float DELTA_ANGLE = 0.01f;
	if (*fAngle < DELTA_ANGLE)
		*fAngle = DELTA_ANGLE;

	m_Start = m_Stop;
}

void Trackball::OnePointStart(float* pos, int n)
{
	m_PriPointOne.m_x = pos[0];
	m_PriPointOne.m_y = pos[1];

	m_PriPointOneScale = m_PriPointOne;
	m_PriPointOneScaleCenter = m_PriPointOne;

	if (pSceneManager != NULL)
	{
		m_cachePriPoint = this->ScreenToDepthVector(m_PriPointOne);
	}

	m_PriPointOneMove = m_PriPointOne;
}
void Trackball::TwoPointsStart(float*pos, int n)
{
	m_PriPointTwo1.m_x = pos[0];
	m_PriPointTwo1.m_y = pos[1];
	m_PriPointTwo2.m_x = pos[2];
	m_PriPointTwo2.m_y = pos[3];

	IntVector2 centerPnt = (m_PriPointTwo1 + m_PriPointTwo2)/2;

	m_PirDistance = TwoPointsDis(pos);

	CameraNode* camera = pSceneManager->GetCamera();

	if (!camera->IsOrthographic())
	{
		Vector3 intersectPnt;
		Vector2 srcPnt(centerPnt.m_x, centerPnt.m_y);
		this->pSceneManager->GetPickPoint(srcPnt, intersectPnt, false);
		this->pSceneManager->SetRotationCenter(intersectPnt);
	}

	m_screenDepth = camera->GetFitClip();

	m_cachePriPoint = this->ScreenToDepthVector(centerPnt);
}

void Trackball::OnePointsMove(float* pos, int n)
{
	IntVector2 currentPnt(pos[0],pos[1]);
	//两次距离超过四个像素，可以认为移动了
	if ((m_PriPointOneScale - currentPnt).LengthSquared() > 4)
	{
		if (pSceneManager != NULL)
		{
			//LOGE("one point move %d",n);
			Trackball::ISMOVING = true;
			Trackball::KEEPINGSTATETIMES = Trackball::TIMES;
			Trackball::MOVESTATE = 1;

			CameraNode* camera = pSceneManager->GetCamera();

			m_cacheCurPointNear  = this->ScreenToDepthVector(currentPnt);
			m_cachePriPoint = this->ScreenToDepthVector(m_PriPointOneScale);

			mvMatrix.moveVector = m_cacheCurPointNear - m_cachePriPoint;
			m_cachePriPoint = m_cacheCurPointNear;
		}

		m_PriPointOneScale = currentPnt;
	}
	else
	{
		mvMatrix.moveVector = Vector3::ZERO;
		mvMatrix.rotation = Quaternion::IDENTITY;
		mvMatrix.scaleFactor = 1;
	}

}

void Trackball::OnePointsScale(float* pos, int n)
{
	//currDistance = TwoPointsDis(pos);
	//mvMatrix.scaleFactor = 1.0f;
	//y方向时移动距离超过1，可以认为时移动了
	if (fabs(m_PriPointOneScale.m_y - pos[1]) > 1)
	{
		if (pSceneManager != NULL)
		{
			Trackball::ISMOVING = true;

			Trackball::MOVESTATE = 2;

			Trackball::KEEPINGSTATETIMES = Trackball::TIMES;

			const IntRect& intRect = this->pSceneManager->GetCamera()->GetViewPort().GetRect();

			m_cacheCurPointNear == this->ScreenToDepthVector(m_PriPointOneScaleCenter);
	
			mvMatrix.scaleFactor = (intRect.m_bottom
					- m_PriPointOneScale.m_y)
					/ (intRect.m_bottom - pos[1]);

			//delete action;
			mvMatrix.moveVector = m_cacheCurPointNear - m_cachePriPoint;
			m_cachePriPoint = m_cacheCurPointNear;
		}
	}
	else
	{
		mvMatrix.moveVector = Vector3::ZERO;
		mvMatrix.rotation = Quaternion::IDENTITY;
		mvMatrix.scaleFactor = 1;
	}
	m_PriPointOneScale.m_x = pos[0];
	m_PriPointOneScale.m_y = pos[1];
}

void Trackball::OnePointRotate(float*pos, int n)
{
	m_Start = ScreenToVector(m_iWidth / 2, m_iHeight / 2);
	Tracking(pos[0] - m_PriPointOne.m_x, pos[1] - m_PriPointOne.m_y,
			&m_axis, &m_angle);
	if (fabs(m_angle) > 0.01f)
	{
//		if(m_angle>0)
//		{
//			m_angle = M3D::Min(m_angle,10.0f);
//		}
//		else
//		{
//			m_angle = M3D::Max(m_angle,-10.0f);
//		}
		Trackball::ISMOVING = true;
		Trackball::ISROTATING = true;
		Trackball::MOVESTATE = 0;
		Trackball::KEEPINGSTATETIMES = Trackball::TIMES;
		mvMatrix.rotation.FromAngleAxis(m_angle,m_axis);

	}
	m_PriPointOne.m_x = pos[0];
	m_PriPointOne.m_y = pos[1];
}
void Trackball::TwoPointsMove(float*pos, int n)
{
	//float angle = TwoPointsTrackingAngle(pos);
	m_CurrDistance = TwoPointsDis(pos);
	mvMatrix.scaleFactor = m_PirDistance / m_CurrDistance;
	mvMatrix.moveVector = Vector3::ZERO;
	mvMatrix.rotation = Quaternion::IDENTITY;

	if (fabs(m_PriPointTwo1.m_x - pos[0]) > 0.1 || fabs(m_PriPointTwo1.m_y - pos[1]) > 0.1
			|| fabs(m_PriPointTwo2.m_x - pos[2]) > 0.1
			|| fabs(m_PriPointTwo2.m_y - pos[3]) > 0.1)
	{
		if (pSceneManager != NULL)
		{
			Trackball::ISMOVING = true;
			Trackball::KEEPINGSTATETIMES = Trackball::TIMES;

			IntVector2 currentPnt((pos[0] + pos[2]) / 2,(pos[1] + pos[3]) / 2);
			m_cacheCurPointNear = this->ScreenToDepthVector(currentPnt);

			mvMatrix.moveVector = m_cacheCurPointNear - m_cachePriPoint;

			AdjustScaleToMoveVector(currentPnt);

			m_cachePriPoint = m_cacheCurPointNear;

			Trackball::ISMOVING = true;
			Trackball::ISROTATING = true;
		}
	}

	m_PirDistance = m_CurrDistance;

	m_PriPointTwo1.m_x = pos[0];
	m_PriPointTwo1.m_y = pos[1];
	m_PriPointTwo2.m_x = pos[2];
	m_PriPointTwo2.m_y = pos[3];
}

void Trackball::OnePointUp(float*pos, int n)
{
	Reset();
}
void Trackball::TwoPointsUp(float*pos, int n)
{
	Reset();
}

float Trackball::TwoPointsDis(float*pos, int n)
{
	float x = pos[0] - pos[2];
	float y = pos[1] - pos[3];
	return (float) sqrt(x * x + y * y);

}
float Trackball::TwoPointsTrackingAngle(float*pos, int n)
{
	double priRadians = atan2((float)m_PriPointTwo1.m_y - m_PriPointTwo2.m_y,
		(float)m_PriPointTwo1.m_x - m_PriPointTwo2.m_x);
	double delta_x = pos[0] - pos[n];
	double delta_y = pos[1] - pos[n + 1];
	double radians = atan2(delta_y, delta_x);
	return (float) (priRadians - radians) / 3.14159 * 360;
}

Vector3 Trackball::ScreenToDepthVector(const IntVector2& scrVector)
{
	Vector3 depthVector;
	if (pSceneManager != NULL)
	{
		CameraNode* camera = pSceneManager->GetCamera();
			///乘上camera->GetView()将点转换到摄像机eye坐标系，进行求两次的变化量,由于此处摄像机采用继承的方式实现，没有采用节点挂载的方式
			//此处的部分变换处理算法，待调整为节点挂载的方式后重构。。 TODO
		depthVector = camera->GetView()*camera->GetViewPort().ScreenToWorldPoint(scrVector.m_x, scrVector.m_y, m_screenDepth);
	}

	return depthVector;
}

//将缩放转换长透视投影对应的移动，如果需要的话
void Trackball::AdjustScaleToMoveVector(const IntVector2& scrVector)
{
	if (pSceneManager != NULL)
	{
		CameraNode* camera = pSceneManager->GetCamera();
		float scaleFactor = mvMatrix.scaleFactor;

#ifdef __MOBILE__
        static float baseScaleFactor = 0.1f;
#else
          static float baseScaleFactor = 0.0001f;
#endif
        
		if (!camera->IsOrthographic() && fabs(scaleFactor -1.0f) > baseScaleFactor)
		{
			Vector3 movTarget =
				camera->GetViewPort().ScreenToWorldPoint(
					scrVector.m_x, scrVector.m_y,
					m_screenDepth);
 
			Vector3 cameraPos = camera->GetPosition();
            camera->SetWorldPosition( cameraPos + mvMatrix.moveVector);
			Vector3 sceneCenter = camera->GetRotateCenter();

			float moveSpeed = 0.0f;
			float moveFactor = 1.0f;
			float stepLength = Min(pSceneManager->GetSceneBox().Length()*0.01f,50.0f);
			if ((cameraPos - sceneCenter).Length()> stepLength)
			{
				moveSpeed = (cameraPos - sceneCenter).Length()*0.1f;
				moveFactor = moveSpeed * scaleFactor * (scaleFactor > 1 ? -1 : 1);
			}
			else 
			{
				//当距离距离过近之后，只允许
				moveSpeed = stepLength;
				//if (scaleFactor > 1)
				{
					moveFactor = moveSpeed * scaleFactor * (scaleFactor > 1 ? -1 : 1);
				}
			}

			mvMatrix.moveVector = (movTarget - cameraPos).Normalized()
				* moveFactor;
			camera->SetWorldPosition( cameraPos + mvMatrix.moveVector);

			mvMatrix.moveVector = Vector3::ZERO;
			mvMatrix.scaleFactor = 1.0f;
		}
	}
}

void Trackball::SetSceneManager(SceneManager* pSceneManager)
{
	this->pSceneManager = pSceneManager;
}


void Trackball::SetRotateSpeed(float rotateSpeed)
{
	this->m_rotateSpeed = rotateSpeed;
}

float Trackball::GetRotateSpeed()
{
	return this->m_rotateSpeed;
}
void Trackball::Reset()
{
	mvMatrix.ReSet();
	ISMOVING = false;
	ISROTATING = false;
	MOVESTATE = -1;
	KEEPINGSTATETIMES = Trackball::TIMES;
	DrawLimit = 0;

	m_angle = 0;
	m_axis = Vector3(0,0,1);
}


}

