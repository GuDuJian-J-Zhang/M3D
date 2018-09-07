
#include "m3d/graphics/Light.h"
#include "m3d/action/RenderAction.h"
namespace M3D
{

Light::Light():Shape()
{
	/// 聚光灯
	m_spotExponent = 20.0f;
	m_spotCutoff = 180.0f;
	m_spotCosCutoff = 0.5f;
	///衰减系数
	m_constantAttenuation = 1.0f;
	m_linearAttenuation = 0.0f;
	m_quadraticAttenuation = 0.0f;
	m_specularIntensity = 1.0f;
	m_diffuse = Color::WHITE;
	m_intensity = 1.0f;
	m_turnOn = true;
}

Light::~Light()
{

}

void Light::ComputeBox()
{
	m_BoundingBox = BoundingBox(Vector3::MINIMUM,Vector3::MAXMUN);
}

void Light::RayPick(RayPickAction* action)
{

}

void Light::FindVisiableObject(RenderAction* renderAction)
{
	if(this->IsVisible())
	{
		this->SetRenderWorldMatrix(renderAction->GetGLWorldMatrix());
		renderAction->AddLight(this);
	}
}

//string Light::GetGeoInfo()
//{
//	return "Light :: TODO ";
//}


void Light::InitProperties()
{

}


void Light::TurnOff(void)
{
	this->m_turnOn = false;
}

void Light::TurnOn(void)
{
	this->m_turnOn = true;
}

void Light::SetLightType(SHAPETYPE type)
{
   this->SetType(type);
}

void Light::SetPerVertex(bool enable)
{
    m_perVertex = enable;

}

void Light::SetColor(const Color& color)
{
    m_color = color;

}

void Light::SetSpecularIntensity(float intensity)
{
	m_intensity = Max(intensity, 0.0f);
}

void Light::SetBrightness(float brightness)
{
    m_brightness = brightness;
}

void Light::SetRange(float range)
{
    m_range = Max(range, 0.0f);
}

void Light::SetFov(float fov)
{
    m_fov = Clamp(fov, 0.0f, M_MAX_FOV);
}

void Light::ShowShape(void)
{
}

void Light::HideShape(void)
{
}

bool Light::IsShowShape(void)
{
	bool ret = true;
	return ret;
}

void Light::SetAspectRatio(float aspectRatio)
{
	m_aspectRatio = Max(aspectRatio, M_EPSILON);
}

float & Light::GetSpecularIntensity()
{
	return m_intensity;
}
}




