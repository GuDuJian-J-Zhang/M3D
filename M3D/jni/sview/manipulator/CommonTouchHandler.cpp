#include <math.h>
#include "sview/views/Parameters.h"
#include "CommonTouchHandler.h"
#include "m3d/SceneManager.h"
#include "m3d/RenderManager.h"
#include "m3d/graphics/CameraNode.h"
#include "m3d/utils/CullerHelper.h"
#include "sview/views/View.h"
#include "sview/extern/PerspectiveData.h"
#include "sview/extern/PerspectiveOperator.h"

namespace SVIEW
{

CommonTouchHandler::CommonTouchHandler() :
		TouchHandler()
{
	m_touchHandlerType = HANDLER_COMMON;
	m_controlLockXY = false;

	m_oribitMode = false;

	m_oribitPhi = 90.0f;
	m_oribitTheta = 0.0f;
}

CommonTouchHandler::~CommonTouchHandler()
{

}
void CommonTouchHandler::InitCamera()
{
	if (this->m_SceneManager)
	{
		CameraNode* camera = m_SceneManager->GetCamera();
		camera->ReSet();

		camera->SetOrthographic(true);
		BoundingBox& pBoundingBox = this->m_SceneManager->GetSceneBox();

		Viewport viewport = camera->GetViewPort();
		int screenHeight = viewport.GetRect().Height();
		int screenWidth = viewport.GetRect().Width();

		this->m_SceneManager->GetRenderManager()->WindowSizeChanged(screenWidth, screenHeight);

		float length = pBoundingBox.Length();

		Vector3 center = pBoundingBox.Center();
		m_oribitControlTarget = center;

		center.m_z += pBoundingBox.Length() * CAMERA_POSFACTOR;

		camera->SetWorldPosition(center);
		length = this->m_SceneManager->GetDefaultFocusLength();

		camera->SetNearClip(length * NEAR_CLIP_PLANE_FACTOR);
		camera->SetFarClip(length * FAR_CLIP_PLANE_FACTOR);

		camera->SetZoom(this->m_SceneManager->GetDefaultZoom());

		camera->SetInitRotateCenter(pBoundingBox.Center());

		camera->SetFov(90);

		camera->LookAt(pBoundingBox.Center(), Vector3(0, 1, 0), TS_WORLD);

		this->m_TrackBall.SetRotateSpeed(2.0f);

		if (m_pView)
		{
			m_pView->GetPerspectiveOperator()->Show(m_pView, this->GetDefaultView(), true, false, false);
			this->SetUpDirection(this->m_upDirection, m_pView);
		}

		const IntRect& rect = camera->GetViewPort().GetRect();
		int width = rect.Width() > rect.Height() ? rect.Height() : rect.Width();
		this->m_TrackBall.SetTrackWindow(width, width);
	}

}

void CommonTouchHandler::OptimizeCamera()
{

}

void CommonTouchHandler::OribitMode(bool value)
{
	if (value)
	{
		m_controlLockXY = false;
		m_freeViewMode = false;
	}
	m_oribitMode = value;
}

void CommonTouchHandler::ConstraintMode(bool value)
{
	if (value)
	{
		m_oribitMode = false;
		m_freeViewMode = false;
	}
	m_controlLockXY = value;
}

void CommonTouchHandler::FreeViewMode(bool value)
{
	if (value)
	{
		m_oribitMode = false;
		m_controlLockXY = false;
	}

	m_freeViewMode = value;
}

void CommonTouchHandler::OnTouchUp(float* p, int n)
{
	if (Parameters::Instance()->m_IsConRotate)
	{
		if (1 == n)
		{
			if (m_TrackBall.m_angle > 3)
			{
				float angle = m_TrackBall.m_angle;

				if (angle > 0)
				{
					angle = M3D::Min(angle, 15.0f);
				}
				else
				{
					angle = M3D::Max(angle, -15.0f);
				}

				if (M3D::Abs(m_TrackBall.m_angle) > 14)
				{
					m_TrackBall.mvMatrix.rotation.FromAngleAxis(angle, m_TrackBall.m_axis);
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

void CommonTouchHandler::OrbitControl()
{
	m_SelectedNodes->Translate(m_TrackBall.mvMatrix.moveVector, m_TrackBall.mvMatrix.currPos);

	CameraNode * camera = m_SceneManager->GetCamera();

	Vector3 movVec = m_TrackBall.mvMatrix.moveVector;

	Matrix3 viewMat = camera->GetView().ToMatrix3().Inverse();
	movVec = viewMat * movVec;

	Vector3 position = camera->GetWorldPosition();
	Quaternion quat;

	quat.Normalize();
	Vector3 offset;
	offset = position - m_oribitControlTarget;

	offset = quat * offset;
	m_oribitTheta = M3D::Atan2(offset.m_x, offset.m_z);
	Quaternion rotation = m_TrackBall.mvMatrix.rotation;
	Vector3 eulerAngles = rotation.EulerAngles();
	Vector3 rotateAngle = eulerAngles;
	m_oribitTheta -= rotateAngle.m_y;

	m_oribitPhi = M3D::Atan2(sqrtf(offset.m_x * offset.m_x + offset.m_z * offset.m_z), offset.m_y);
	m_oribitPhi -= rotateAngle.m_x;
	m_oribitPhi = M3D::Max(0.1f, M3D::Min(180.0 - 0.1f, m_oribitPhi));

	float radius = offset.Length();
	m_oribitControlTarget -= movVec;
	offset.m_x = radius * M3D::Sin(m_oribitPhi) * M3D::Sin(m_oribitTheta);
	offset.m_y = radius * M3D::Cos(m_oribitPhi);
	offset.m_z = radius * M3D::Sin(m_oribitPhi) * M3D::Cos(m_oribitTheta);

	offset = quat.Inverse() * offset;
	position = m_oribitControlTarget + offset; //

	camera->SetWorldPosition(position);
	camera->LookAt(m_oribitControlTarget, quat * Vector3::UP);
	m_SelectedNodes->Zoom(m_TrackBall.mvMatrix.scaleFactor);

}

void CommonTouchHandler::RotateAroundAxis()
{
	CameraNode * camera = m_SceneManager->GetCamera();
	Vector3 position = camera->GetWorldPosition();
	BoundingBox& pBoundingBox = this->m_SceneManager->GetSceneBox();
	Vector3 center = pBoundingBox.Center();
	Quaternion rotation = m_TrackBall.mvMatrix.rotation;
	Vector3 eulerAngles = rotation.EulerAngles();
	Vector3 rotateAngle = eulerAngles;
	Quaternion rotateQua(-eulerAngles.m_y * 3, Vector3::UP);

	Matrix4 R1, R2, R3, R;
	R1.MultiTranslate(-center);
	R2.MultiRotatiton(rotateQua);
	R3.MultiTranslate(center);
	R = R3 * R2 * R1;
	Matrix4 view = camera->GetView().ToMatrix4();

	Matrix4 temp = (view * R * view.Inverse());

	camera->Translate(temp.Translation());
	camera->Rotate(temp.Rotation());

	m_SelectedNodes->Translate(m_TrackBall.mvMatrix.moveVector, m_TrackBall.mvMatrix.currPos);

	m_SelectedNodes->Zoom(m_TrackBall.mvMatrix.scaleFactor);

}

void CommonTouchHandler::FreeViewRotate()
{
	Quaternion rotation = m_TrackBall.mvMatrix.rotation.Inverse();
	CameraNode * camera = m_SceneManager->GetCamera();
	camera->RotateAroundCenter(rotation, TS_WORLD);

	m_SelectedNodes->Translate(m_TrackBall.mvMatrix.moveVector, m_TrackBall.mvMatrix.currPos);
	m_SelectedNodes->Zoom(m_TrackBall.mvMatrix.scaleFactor);
}

void CommonTouchHandler::OnTouchMove(int moveType, float* p, int n)
{
	OptimizeCamera();
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
		m_TrackBall.TwoPointsMove(p, n);
		if (!SVIEW::Parameters::Instance()->m_isOpenScale)
		{
			m_TrackBall.mvMatrix.scaleFactor = 1.0f;
		}
		this->TwoFingersRotation(p, n);
		this->m_SceneManager->RestoreRotationCenter();
		this->m_PriPointTwo1.m_x = p[0];
		this->m_PriPointTwo1.m_y = p[1];
		this->m_PriPointTwo2.m_x = p[2];
		this->m_PriPointTwo2.m_y = p[3];
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

void CommonTouchHandler::OnUpDataTouchIntent()
{
	if (m_freeViewMode)
	{
		FreeViewRotate();
	}

//TODO
	if (m_oribitMode)
	{
		OrbitControl();
	}

	if (m_controlLockXY)
	{
		RotateAroundAxis();
	}
}

void CommonTouchHandler::OnTouchDown(float* p, int n)
{
	m_TrackBall.Reset();
	if (1 == n)
	{
		m_TrackBall.OnePointStart(p, n);
	}
	else if (2 == n)
	{
		this->m_PriPointTwo1.m_x = p[0];
		this->m_PriPointTwo1.m_y = p[1];
		this->m_PriPointTwo2.m_x = p[2];
		this->m_PriPointTwo2.m_y = p[3];
		m_TrackBall.TwoPointsStart(p, n);
	}
	else if (3 == n)
	{

	}
}

}
