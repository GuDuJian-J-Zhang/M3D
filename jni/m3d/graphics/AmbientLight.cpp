#include "m3d/graphics/AmbientLight.h"
#include "m3d/scene/SceneNode.h"
#include "../action/RayPickAction.h"
#include "m3d/graphics/BaseLight.h"
namespace M3D
{
	AmbientLight::AmbientLight() :BaseLight()
	{

		m_lightColor = Color(1.0, 1.0, 1.0);
		m_lightType = LightType_Ambient;

	}


	AmbientLight::~AmbientLight()
	{

	}
}

