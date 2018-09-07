#include "m3d/graphics/InnerMaterial.h"

namespace M3D
{

	InnerMaterial::InnerMaterial()
	{
		m_updateUvTransform = true;
		m_uvScale = Vector2(1.0, 1.0);
		m_uvTranslate = Vector2(0.0, 0.0);
		m_uvRotate = 0.0;
		m_materialType = MaterialType_Inner;
		m_acceptLight = true;
	}

	InnerMaterial::~InnerMaterial()
	{

	}

	void InnerMaterial::UpdateUvTransform()
	{
		float c = cosf(m_uvRotate);
		float s = sinf(m_uvRotate);
		float sx = m_uvScale.m_x;
		float sy = m_uvScale.m_y;
		float tx = m_uvTranslate.m_x;
		float ty = m_uvTranslate.m_y;
		m_uvTransform = Matrix3(
			sx * c, sx * s, tx,
			-sy * s, sy * c, ty,
			0, 0, 1
		);
		m_updateUvTransform = false;
	}

	void InnerMaterial::SetUvTranslate(Vector2& translate)
	{
		m_uvTranslate = translate;
		m_updateUvTransform = true;
	}

	void InnerMaterial::SetUvScale(Vector2&scale)
	{
		m_uvScale = scale;
		m_updateUvTransform = true;
	}

	void InnerMaterial::SetUvRotate(float rotate)
	{
		m_uvRotate = rotate;
		m_updateUvTransform = true;
	}

	Matrix3& InnerMaterial::GetUvTransform()
	{
		if (m_updateUvTransform)
		{
			UpdateUvTransform();
		}

		return m_uvTransform;
	}



	M3D::Vector2 InnerMaterial::GetUvTranslate()
	{
		return m_uvTranslate;
	}

	M3D::Vector2 InnerMaterial::GetUvScale()
	{
		return m_uvScale;
	}

	float InnerMaterial::GetUvRotate()
	{
		return m_uvRotate;
	}

}