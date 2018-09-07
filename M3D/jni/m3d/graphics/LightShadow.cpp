#include "m3d/graphics/LightShadow.h"
namespace M3D
{

	LightShadow::LightShadow()
	{
		m_camera = NULL;
		renderTarget = NULL;
	}

	void LightShadow::Update(BaseLight* light, CameraNode* mainCamera, int face /*= 1*/)
	{

	}

	void LightShadow::SetMapSize(Vector2& size)
	{
		m_mapSize = size;
	}

	Vector2& LightShadow::GetMapSize()
	{
		return m_mapSize;
	}

	CameraNode* LightShadow::GetCamera()
	{
		return m_camera;
	}

	void LightShadow::Bias(float val)
	{
		m_bias = val;
	}

	float LightShadow::Bias()
	{
		return m_bias;
	}

	void LightShadow::Radius(float val)
	{
		m_radius = val;
	}

	float LightShadow::Radius()
	{
		return m_radius;
	}

	M3D::Matrix4& LightShadow::Matrix()
	{
		return m_matrix;
	}

	void LightShadow::RenderTarget(HardWareFrameBuffer* val)
	{
		renderTarget = val;
	}

	HardWareFrameBuffer* LightShadow::RenderTarget()
	{
		return renderTarget;
	}

}


