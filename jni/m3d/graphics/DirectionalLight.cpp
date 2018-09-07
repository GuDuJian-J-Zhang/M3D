
#include "m3d/graphics/DirectionalLight.h"
#include "m3d/action/RenderAction.h"
#include "sview/views/Parameters.h"
#include "m3d/SceneManager.h"
#include "m3d/base/BoundingBox.h"
#include "m3d/graphics/LightShadow.h"
#include "m3d/graphics/DirectionalLightShadow.h"
namespace M3D
{

DirectionalLight::DirectionalLight():BaseLight()
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

	m_ShowAllSign = true;
	m_showSimpleSign = true;
	//m_squarePoints;
	SetLightSourceType(LightType_Directional);
	m_lightShadow = new DirectionalLightShadow;
	m_castShadow = true;
}

DirectionalLight::~DirectionalLight()
{

}

//void DirectionalLight::ComputeBox()
//{
//	//m_BoundingBox = BoundingBox(Vector3::MINIMUM,Vector3::MAXMUN);
//}

//void DirectionalLight::RayPick(RayPickAction* action)
//{
//
//}

void DirectionalLight::FindVisiableObject(RenderAction* renderAction)
{
	if (m_needUpdataSign)
	{		
		Matrix3x4 worldMatrix = this->GetWorldTransform();
		m_glworldMatrix = worldMatrix.ToMatrix4().Transpose();
		Vector3 newPos = Vector3::ZERO;
		//仅显示图片标识
		if (this->m_showSimpleSign)
		{		
			if (!this->m_simpleSignModel)
			{
				this->m_simpleSignModel = new ImageModel();
				AddRefMe(this->m_simpleSignModel);

				string directLightImage = SVIEW::Parameters::Instance()->m_appWorkPath + "\\data\\pic\\" + string("directional.png");
				m_simpleSignModel->SetImagePath(directLightImage);
                Vector2 tempSize(0.5f, 0.5f);
				m_simpleSignModel->SetImageSize(newPos, tempSize);
				this->AddSubModel(this->m_simpleSignModel);
			}
			//m_simpleSignModel->SetImagePosition(newPos);
		}

		//显示包括线框的标识
		if (this->m_ShowAllSign)
		{
			if (!this->m_allSignModel)
			{
				this->m_allSignModel = new Model();

				AddRefMe(this->m_allSignModel);
				this->AddSubModel(this->m_allSignModel);
			}
			m_allSignModel->ClearLineData();
			vector<Vector3> lines;
			
			//Vector3 target = GetTargetPosition();
			Vector3 position = newPos;
			Vector3 direction = m_direction;
			direction.Normalize();
			float length = renderAction->GetScene()->GetSceneBox().Length();
			lines.push_back(position - direction*length*0.5);
			lines.push_back(position);
			m_allSignModel->AddLineData(lines);
			//构造四边形
			vector<Vector3> square;
			float size = length*0.02;

			Vector3 p1(-size, size, 0.0),p2(-size,-size,0.0),p3(size,-size,0.0),p4(size,size,0.0);
			square.push_back(p1); //lines.push_back(p2);
			square.push_back(p2); //lines.push_back(p3);
			square.push_back(p3); //lines.push_back(p4);
			square.push_back(p4); //lines.push_back(p1);
			square.push_back(p1);

			m_allSignModel->AddLineData(square);
		}

		m_needUpdataSign = false;
	}

	//if(this->IsVisible())
	{
		this->SetRenderWorldMatrix(&m_glworldMatrix);
	//	renderAction->AddLight(this);
	}

	Model::FindVisiableObject(renderAction);
}

void DirectionalLight::InitProperties()
{

}

void DirectionalLight::SetLightType(SHAPETYPE type)
{
   this->SetType(type);
}

void DirectionalLight::SetPerVertex(bool enable)
{
    m_perVertex = enable;

}

void DirectionalLight::SetSpecularIntensity(float intensity)
{
	m_intensity = Max(intensity, 0.0f);
}

void DirectionalLight::SetBrightness(float brightness)
{
    m_brightness = brightness;
}

void DirectionalLight::SetRange(float range)
{
    m_range = Max(range, 0.0f);
}

void DirectionalLight::SetFov(float fov)
{
    m_fov = Clamp(fov, 0.0f, M_MAX_FOV);
}

//void DirectionalLight::ShowShape(void)
//{
//
//}
//
//void DirectionalLight::HideShape(void)
//{
//
//}
//
//bool DirectionalLight::IsShowShape(void)
//{
//	bool ret = true;
//	
//	return ret;
//}

void DirectionalLight::SetAspectRatio(float aspectRatio)
{
	m_aspectRatio = Max(aspectRatio, M_EPSILON);
}

float & DirectionalLight::GetSpecularIntensity()
{
	return m_intensity;
}

LightShadow* DirectionalLight::GetLightShadow()
{
	return m_lightShadow;
}

}




