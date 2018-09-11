#include "m3d/graphics/Billboard.h"
#include "m3d/graphics/CameraNode.h"
#include "m3d/action/RenderAction.h"
#include "m3d/graphics/Renderable.h"
#include "sview/views/Parameters.h"
#include "m3d/action/RenderAction.h"
namespace M3D
{
Billboard::Billboard()
{
	this->AllowRotate(false);
	this->AllowTran(true);
	this->AllowScale(false);
}

Billboard::~Billboard()
{

}

void Billboard::AllowTran(bool allow)
{
	this->m_allowTran = allow;
}

void Billboard::AllowRotate(bool allow)
{
	this->m_allowRotate = allow;
}

void Billboard::AllowScale(bool allow)
{
	this->m_allowScale = allow;
}

bool Billboard::IsAllowTran()
{
	return this->m_allowTran;
}

bool Billboard::GetRotate()
{
	return this->m_allowRotate;
}

bool Billboard::GetScale()
{
	return this->m_allowScale;
}

void Billboard::SetCenter(const Vector3& center)
{
	this->m_center = center;
}

Vector3& Billboard::GetCenter()
{
	return this->m_center;
}

float Billboard::GetFitShowScale(RenderAction* renderAction,Vector3& position)
{
	float baseFactor = 1.0f;
	CameraNode* camera = renderAction->GetCamera();
	if (!camera->IsOrthographic()) {
		//得到摄像机位置
		Vector3 cameraPos = camera->GetWorldPosition();
		//获取当前ortho宽和高的较小值
		float heigh = camera->GetOrthoSize();

		float dis = (cameraPos - position).Length();

		baseFactor *= dis * 2 / heigh;
	}

	float scale2 = 1.0f;
#ifdef _WIN32
	//获取当前ortho宽和高的较小值
	scale2 = camera->GetViewPort().GetRect().Height();
	if (scale2 < camera->GetViewPort().GetRect().Width())
	{
		scale2 = camera->GetViewPort().GetRect().Width();
	}

	if (SVIEW::Parameters::Instance()->m_screenHeight > SVIEW::Parameters::Instance()->m_screenWidth)
	{
		scale2 = SVIEW::Parameters::Instance()->m_screenWidth / scale2;
	}
	else
	{
		scale2 = SVIEW::Parameters::Instance()->m_screenHeight / scale2;
	}
#endif
	return (baseFactor*scale2 / camera->GetZoom());
}

Matrix3x4& Billboard::GetWorldMatrix()
{
	return m_worldMatrix;
}

Matrix3x4& Billboard::GetWorldMatrix(RenderAction* renderAction)
{
	Matrix3x4* parentworldMatrix = renderAction->GetWorldMatrix();

	m_worldMatrix = *parentworldMatrix;

	if(this->GetScale() && this->GetRotate() && this->IsAllowTran())
	{
		return m_worldMatrix;
	}

	
	CameraNode* camera = renderAction->GetCamera();

	
	Matrix3x4 parentModelView = (camera->GetView()*(*parentworldMatrix));
	
	Vector3 center = parentModelView*m_center;

	Vector3 center2 = (*parentworldMatrix)*m_center;

	m_worldMatrix = parentModelView;
	//将点移动回去
	m_worldMatrix.LeftMultiTranslate(-center);

	//不允许缩放
	if(!this->GetScale())
	{
		m_worldMatrix.LeftMultiScale(GetFitShowScale(renderAction, center2));
	}

//
//	//不允许旋转
	if(!this->GetRotate())
	{
		Quaternion rotation = parentModelView.Rotation().Inverse();
		m_worldMatrix.LeftMultiRotatiton(rotation);
	}

//	//不允许平移
	if(!this->IsAllowTran())
	{
		m_worldMatrix.LeftMultiTranslate(parentModelView.Translation().Nagative());
	}

	//将点移动到坐标原点
	m_worldMatrix.LeftMultiTranslate(center);

	m_worldMatrix = camera->GetView().Inverse()*m_worldMatrix;

	return m_worldMatrix;
}

M3D::Matrix3x4& Billboard::GetWorldMatrix(const Matrix3x4& modelViewMatrix)
{
	m_worldMatrix = Matrix3x4::IDENTITY;

	if (this->GetScale() && this->GetRotate() && this->IsAllowTran())
	{
		return m_worldMatrix;
	}

	const Matrix3x4& parentModelView = modelViewMatrix;
	m_worldMatrix = parentModelView;

	Vector3 center = parentModelView*m_center;

	//将点移动回去
	m_worldMatrix.LeftMultiTranslate(-center);

	if (!this->GetScale())
	{
		float baseFactor = 1.0f;
		float scale2 = 1.0f;

		m_worldMatrix.LeftMultiScale(baseFactor*scale2);
	}

	if (!this->GetRotate())
	{
		Quaternion rotation = parentModelView.Rotation().Inverse();
		m_worldMatrix.LeftMultiRotatiton(rotation);
	}

	if (!this->IsAllowTran())
	{
		m_worldMatrix.LeftMultiTranslate(parentModelView.Translation().Nagative());
	}

	m_worldMatrix.LeftMultiTranslate(center);

	m_worldMatrix = modelViewMatrix.Inverse()*m_worldMatrix;
	return m_worldMatrix;
}

Matrix4&  Billboard::GetGLWorldMatrix(RenderAction* renderAction)
{
	m_glWorldMatrix = this->GetWorldMatrix(renderAction).ToMatrix4().Transpose();

	return this->m_glWorldMatrix;
}

Matrix4& Billboard::GetGLWorldMatrix()
{
	return this->m_glWorldMatrix;
}

M3D::Matrix4& Billboard::GetGLWorldMatrix(const Matrix3x4& modelViewMatrix)
{
	m_glWorldMatrix = this->GetWorldMatrix(modelViewMatrix).ToMatrix4().Transpose();
	return this->m_glWorldMatrix;
}

}
