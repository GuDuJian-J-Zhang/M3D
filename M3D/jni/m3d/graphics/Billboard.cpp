#include "m3d/graphics/Billboard.h"
#include "m3d/graphics/CameraNode.h"
#include "m3d/action/RenderAction.h"
#include "m3d/graphics/Renderable.h"
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

Matrix3x4& Billboard::GetWorldMatrix()
{
	return m_worldMatrix;
}
Matrix3x4& Billboard::GetWorldMatrix(RenderAction* renderAction)
{
	m_worldMatrix = Matrix3x4::IDENTITY;

	if(this->GetScale() && this->GetRotate() && this->IsAllowTran())
	{
		return m_worldMatrix;
	}

	Matrix3x4* parentworldMatrix = renderAction->GetWorldMatrix();

	CameraNode* camera = renderAction->GetCamera();

	Matrix3x4 parentModelView = (camera->GetView()*(*parentworldMatrix)).Inverse();
	//
	//由于要实现缩放时，所有的点缩放到中心的效果。因此需要将Billboard先移动到坐标原点，在进行平移旋转和缩放操作
	//下面为逆矩阵的正操作，通过改变乘的顺序，可以减少一次矩阵求逆操作

	//将点移动回去
	m_worldMatrix.MultiTranslate(m_center);

	//不允许缩放
	if(!this->GetScale())
	{
		float baseFactor = 1.0f;

		if(!camera->IsOrthographic()){
			//得到摄像机位置
			Vector3 cameraPos = camera->GetWorldPosition();
			//获取当前ortho宽和高的较小值
			float heigh = camera->GetOrthoSize();

			float dis = (cameraPos - this->GetCenter()).Length();

			baseFactor = dis*2/heigh;
		}

		m_worldMatrix.MultiScale(baseFactor/camera->GetZoom());
	}

//
//	//不允许旋转
	if(!this->GetRotate())
	{
		Quaternion rotation = parentModelView.Rotation();
		m_worldMatrix.MultiRotatiton(rotation);
	}

//	//不允许平移
	if(!this->IsAllowTran())
	{
		m_worldMatrix.MultiTranslate(parentModelView.Translation().Nagative());
	}

	//将点移动到坐标原点
	m_worldMatrix.MultiTranslate(m_center.Nagative());

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
}
