
#include "m3d/graphics/DirectionalLight.h"
#include "m3d/action/RenderAction.h"
#include "sview/views/Parameters.h"
#include "m3d/SceneManager.h"
#include "m3d/base/BoundingBox.h"
#include "m3d/graphics/LightShadow.h"
#include "m3d/graphics/DirectionalLightShadow.h"
#include "m3d/utils/MeshHelper.h"
#include "m3d/scene/ShapeNode.h"
#include "m3d/graphics/Billboard.h"
#include "m3d/utils/ShapeHelper.h"
#include "m3d/Handler/Translate3DDragger.h"
#include "m3d/graphics/LightDraggerCallback.h"
#include "m3d/model/Body.h"
#include "m3d/graphics/Material.h"
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
	m_arrow = nullptr;
	m_orgScale = 1.0f;
	sourceDragger = nullptr;
	directionDragger = nullptr;
}

DirectionalLight::~DirectionalLight()
{
	ReleaseMe(m_arrow);
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
	if(!IsVisible())
	{
		return;
	}

	Matrix3x4 worldMatrix = this->GetWorldTransform();
	m_glworldMatrix = worldMatrix.ToMatrix4().Transpose();

	this->SetRenderWorldMatrix(&m_glworldMatrix);

	vector<Model*>& children = GetSubModels();
	for (size_t i = 0; i < GetSubModelCount(); i++)
	{
		Model* child = children[i];
		if (child)
		{
			if (child->GetName().compare("Arrow") == 0)
			{
				Vector3 position = this->GetWorldTransform()* Vector3::ZERO;
				float scale = Billboard::GetFitShowScale(renderAction, position) / 0.4f;
				scale *= m_orgScale;
				Vector3 scaleVec3(scale, scale, scale);
				child->SetScale(scaleVec3);
			}
			child->FindVisiableObject(renderAction);
		}
	}
}

void DirectionalLight::SetSelected(bool select)
{
	m_IsSelect = select;
	if (m_simpleSignModel) {
		m_simpleSignModel->SetSelected(select);
	}
	if (m_arrow) {
		m_arrow->SetSelected(select);
	}
	if (m_Visible)
	{
		if (m_allSignModel)
		{
			m_allSignModel->SetVisible(select);
		}

		if (sourceDragger)
		{
			sourceDragger->SetVisible(select);
			sourceDragger->GetDrawModel()->SetVisible(select);
		}
		if (directionDragger)
		{
			directionDragger->SetVisible(select);
			directionDragger->GetDrawModel()->SetVisible(select);
		}
	}
}

void DirectionalLight::SetVisible(bool visible)
{
	m_Visible = visible;
	IsTurnOn(visible);
	//SetSelected(m_IsSelect);
	
	visible = m_IsSelect && m_Visible;
	//if(m_IsSelect && m_Visible)
	{
		if (m_allSignModel)
		{
			m_allSignModel->SetVisible(visible);
		}
		if (sourceDragger)
		{
			sourceDragger->SetVisible(visible);
			sourceDragger->GetDrawModel()->SetVisible(visible);
		}
		if (directionDragger)
		{
			directionDragger->SetVisible(visible);
			directionDragger->GetDrawModel()->SetVisible(visible);
		}
	}
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

void DirectionalLight::CreateSignModel(SceneManager* sceneManager)
{
	if (m_needUpdataSign)
	{
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
				m_simpleSignModel->SetName("LightModel");
				m_simpleSignModel->SetAllowClip(false);
				m_simpleSignModel->SetInTopShow(true);
				this->AddSubModel(this->m_simpleSignModel);
			}

			if (!m_arrow)
			{
				string toolsFilePath = SVIEW::Parameters::Instance()->m_appWorkPath + "\\data\\handler\\" + string("AxisHandler.stl");
				if (MeshHelper::ReadSingleModel(toolsFilePath, m_arrow))
				{
					if (m_arrow) {

						Vector2 size = ShapeHelper::GetCommonSize(sceneManager, Vector2(1.0f, 1.0f));
						m_orgScale = size.m_x;
						m_arrow->SetInitHightlight(true);
						m_arrow->SetUserData(this);
						m_arrow->SetMaterial(InitBaseMaterial());
						m_arrow->SetName("Arrow");
						m_arrow->SetWorldPosition(newPos);
						m_arrow->SetWorldDirection(m_direction.Nagative());
						AddSubModel(m_arrow);
					}
				}
			}
			if (!m_allSignModel)
			{
				m_allSignModel = new Model();
				m_allSignModel->SetName("Lines");
				AddRefMe(this->m_allSignModel);

				float length = sceneManager->GetSceneBox().Length() * 0.5;
				Vector3 direction = m_direction.Normalized();
				Vector3 pnt = newPos - direction*length;

				vector<Vector3> lines;
				lines.push_back(newPos);
				lines.push_back(pnt);
				m_allSignModel->AddLineData(lines, Color::YELLOW, true);
				m_allSignModel->SetVisible(false);
				m_allSignModel->SetNeedClip(false);
				AddSubModel(this->m_allSignModel);
			}
		}
		m_needUpdataSign = false;
	}
}

BaseMaterial* DirectionalLight::InitBaseMaterial()
{
	Material* material = new Material();
	material->Opacity(1.0f);
	material->SetShininess(0.0f);
	Color diffuse(1.0, 1.0, 0.0);//(0.882352948, 0.5647059, 0.0);
	Color ambient(0.0, 0.0, 0.0);
	Color emissive(1.0,1.0,0.0);//(0.882352948, 0.5647059, 0.0);
	Color specular(0.0, 0.0, 0.0);
	material->SetDiffuse(diffuse);
	material->SetAmbient(ambient);
	material->SetEmissive(emissive);
	material->setSpecular(specular);
	Uniform uniform("Int", (int)material->GetMaterialType());
	material->SetUniformParameter("type", uniform);
	return material;
}

void DirectionalLight::BindDragger(SceneManager* sceneManager)
{
	sceneManager->Lock();
	Vector3 position = sceneManager->GetSceneBox().Center();
	sourceDragger = sceneManager->GetHandlerGroup()->GetTranslate3DHandle();
	sourceDragger->SetNeedScale(false);
	sourceDragger->SetVisible(false);
	sourceDragger->SetWorldPosition(GetWorldPosition());
	//sourceDragger->SetID(GetID());

	LightDraggerCallback* draggerCallback = new LightDraggerCallback();
	draggerCallback->AddModel(this);
	draggerCallback->AddRef();
	draggerCallback->SetDraggerType(DraggerType::SOURCE_DRAGGER);
	sourceDragger->addDraggerCallback(draggerCallback);
	draggerCallback->Release();


	directionDragger = sceneManager->GetHandlerGroup()->GetTranslate3DPntHandle();
	directionDragger->SetNeedScale(false);
	directionDragger->SetVisible(false);

	
	Matrix3x4 transformMaxtix;
	transformMaxtix.MultiTranslate(GetWorldPosition());
	float length = sceneManager->GetSceneBox().Length() * 0.5;
	transformMaxtix.MultiTranslate(GetWorldDirection().Nagative()*length);
	m_DirecPosition = transformMaxtix.Translation();
	directionDragger->SetWorldPosition(m_DirecPosition);

	LightDraggerCallback* directionCallback = new LightDraggerCallback();
	directionCallback->AddModel(this);
	directionCallback->AddRef();
	directionCallback->SetDraggerType(DraggerType::DIRECTION_DRAGGER);
	directionDragger->addDraggerCallback(directionCallback);
	directionCallback->Release();

	sceneManager->UnLock();
}

void DirectionalLight::UnBindDragger(SceneManager* sceneManager)
{
	sceneManager->Lock();
	sourceDragger->ClearDraggerCallbacks();
	directionDragger->ClearDraggerCallbacks();
	sceneManager->UnLock();
}

Vector3 DirectionalLight::GetDirectionPos()
{
	return m_DirecPosition;
}
void DirectionalLight::SetDirectionPos(Vector3 pos)
{
	m_DirecPosition = pos;
}

bool DirectionalLight::ChangeLines()
{
	if (m_allSignModel)
	{
		vector<Body*>* bodies = m_allSignModel->GetBodys();
		if (bodies != nullptr)
		{
			for (vector<Body*>::iterator it = bodies->begin(); it != bodies->end(); it++)
			{
				ReleaseMe(*it);
			}
			bodies->clear();
		}
		
		Matrix3x4 pareWMat = m_allSignModel->GetWorldTransform();
		Vector3 start = pareWMat.Inverse()*GetWorldPosition();
		Vector3 end = pareWMat.Inverse()*m_DirecPosition;

		vector<Vector3> lines;
		lines.push_back(start);
		lines.push_back(end);
		m_allSignModel->AddLineData(lines, Color::YELLOW, true);
		m_allSignModel->SetNeedClip(false);
	}
	return true;
}

}




