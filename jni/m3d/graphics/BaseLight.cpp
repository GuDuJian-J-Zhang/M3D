#include "m3d/graphics/BaseLight.h"
#include "m3d/scene/SceneNode.h"
#include "../action/RayPickAction.h"

namespace M3D
{
	BaseLight::BaseLight() :SignModel()
	{
		 ////这里默认构造一个用来占位置的很小的模型
		 //////
		 //vector<Vector3> points;
		 //vector<Vector3> normals;
		 //for (int i = 0;i<3;i++)
		 //{
			// points.push_back(Vector3::ZERO);
			// normals.push_back(Vector3::ZERO);
		 //}

		 //vector<Vector3> uvs;
		 //this->AddMeshData(points,normals,uvs);
		m_lightColor = Color(1.0,1.0,1.0);
		
	  m_intensity = 1.0;
	  m_direction = Vector3(0,0,1);
		m_castShadow = false;
		m_allSignModel = NULL;
		m_simpleSignModel = NULL;
		m_needUpdateInfo = true;
		m_isInWorld = false;
		m_isTurnOn = true;
	}


	BaseLight::~BaseLight()
	{
		
	}

	M3D::LightType BaseLight::GetLightSourceType() const
	{
		return m_lightType;
	}

	void BaseLight::SetLightSourceType(M3D::LightType val)
	{
		m_lightType = val;
	}

	Color BaseLight::GetLightColor() const
	{
		return m_lightColor;
	}

	void BaseLight::SetLightColor(Color& val)
	{
		m_lightColor = val;
	}

	float BaseLight::GetIntensity() const
	{
		return m_intensity;
	}

	void BaseLight::SetIntensity(float val)
	{
		m_intensity = val;
	}

	M3D::Vector3 BaseLight::GetNodeWorldPosition()
	{
		Vector3 ret;
		//SceneNode* node = GetSceneNode();
		//if (node)
		//{
		//	ret = node->GetWorldPosition();
		//}
		//ret = this->GetWorldBoundingBox().Center();
		ret = GetWorldPosition();
		return ret;
	}

	void BaseLight::SetNodeWorldPosition(Vector3& position)
	{
		//SceneNode* node = GetSceneNode();
		//if (node)
		//{
		//	node->SetWorldPosition(position);
		//}
		SetWorldPosition(position);
	}

	void BaseLight::SetCastShadow(bool castShadow)
	{
		this->m_castShadow = castShadow;
	}

	void BaseLight::FindVisiableObject(RenderAction* renderAction)
	{

	}

	void BaseLight::NeedUpdateInfo(bool val)
	{
		m_needUpdateInfo = val;
	}

	bool BaseLight::NeedUpdateInfo() const
	{
		return m_needUpdateInfo;
	}

	M3D::SHAPETYPE BaseLight::GetType(void)
	{
		return SHAPETYPE::SHAPE_LIGHT_BASE;
	}

	bool BaseLight::IsInWorld()
	{
		return m_isInWorld;
	}

	void BaseLight::IsInWorld(bool value)
	{
		m_isInWorld = value;
	}

	LightShadow* BaseLight::GetLightShadow()
	{
		return nullptr;
	}

	void BaseLight::IsTurnOn(bool value)
	{
		m_isTurnOn = value;
	}

	bool BaseLight::IsTurnOn()
	{
		return m_isTurnOn;
	}

	bool BaseLight::CastShadow() const
	{
		return m_castShadow;
	}

	Vector3 BaseLight::GetWorldDirection()
	{
		//Vector3 ret = m_direction;

		return GetWorldRotation()*m_direction;
	}

	void BaseLight::RayPick(RayPickAction* action)
	{
		if (!this->IsVisible() || !this->RendreVisible() /*|| !action->CanPickShape(this->GetType())*/)
		{
			return;
		}

		action->BeginPickAsGroup(this);

		Model::RayPick(action);

		action->EndPickAsGroup(this,1);

	}

	void BaseLight::FramePick(RayPickAction* action)
	{
		if (!this->IsVisible() || !this->RendreVisible() /*|| !action->CanPickShape(this->GetType())*/)
		{
			return;
		}

		/*if (this->m_modelShape)
		{
			this->m_modelShape->FramePick(action);
		}

		for (int j = 0; j < m_SubModelArray.size(); j++)
		{
			m_SubModelArray[j]->RayPick(action);
		}*/
	}



	void BaseLight::SetUpHelperData()
	{
		

	}

}

