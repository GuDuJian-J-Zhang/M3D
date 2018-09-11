#include "m3d/graphics/HemisphereLight.h"
#include "m3d/scene/SceneNode.h"
#include "../action/RayPickAction.h"
#include "m3d/graphics/BaseLight.h"
namespace M3D
{
	HemisphereLight::HemisphereLight() :BaseLight()
	{

		m_lightColor = Color(1.0, 1.0, 1.0);
		m_groundColor = Color(1.0, 1.0, 1.0);
		m_lightType = LightType_Hemisphere;

	}


	HemisphereLight::~HemisphereLight()
	{

	}
}

