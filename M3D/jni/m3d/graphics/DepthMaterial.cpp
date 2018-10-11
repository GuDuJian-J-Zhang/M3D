#include "m3d/graphics/DepthMaterial.h"

namespace M3D
{

	DepthMaterial::DepthMaterial()
	{
		m_materialType = MaterialType_Depth;
		m_useLight = false;
	}

	DepthMaterial::DepthMaterial(DepthMaterial& org):InnerMaterial(org)
	{

	}

	DepthMaterial::~DepthMaterial()
	{

	}

	BaseMaterial* DepthMaterial::Clone()
	{
		return new DepthMaterial(*this);
	}

	bool DepthMaterial::GetUseLight() const
	{
		return m_useLight;
	}

	void DepthMaterial::SetUseLight(bool val)
	{
		m_useLight = val;
	}

}