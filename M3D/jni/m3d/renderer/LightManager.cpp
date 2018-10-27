#include "m3d/renderer/LightManager.h"
#include "m3d/SceneManager.h"
#include "m3d/scene/GroupNode.h"
#include "m3d/scene/SceneNode.h"
#include "m3d/scene/ShapeNode.h"
#include "m3d/graphics/CameraNode.h"
#include "m3d/graphics/PointLight.h"
#include "m3d/graphics/SpotLight.h"
#include "m3d/utils/StringHelper.h"
#include "../scene/LightGroup.h"
#include "m3d/graphics/AmbientLight.h"
#include "../graphics/HemisphereLight.h"
#include "../graphics/DirectionalLightShadow.h"

namespace M3D
{
	LightManager::LightManager()
	{
	}


	LightManager::~LightManager()
	{
		map<int, LightUniforms*>::iterator it = m_lightUniformCatch.begin();
		
		for (; it != m_lightUniformCatch.end(); ++it)
		{
			if (it->second)
			{
				delete it->second;
				it->second = NULL;
			}
		}
		m_lightUniformCatch.clear();
		for (size_t i = 0,length = m_sceneLights.size(); i < length; i++)
		{
			m_sceneLights[i]->Release();
			m_sceneLights[i] = NULL;
		}
		m_sceneLights.clear();
	}

	//************************************
	// Method:    AddLight
	// FullName:  M3D::LightManager::AddLight
	// Access:    public 
	// Returns:   void
	// Qualifier:	添加灯光
	// Parameter: BaseLight * light
	//************************************
	void LightManager::AddLight(BaseLight* light)
	{
		this->m_sceneManager->Lock();
		if (light) {
			for (int i = 0; i < m_sceneLights.size(); i++) {
				if (m_sceneLights[i] == light) {
					this->m_sceneManager->UnLock();
					return;
				}
			}
			AddRefMe(light); 
			light->CreateSignModel(m_sceneManager);
			light->BindDragger(m_sceneManager);
			this->m_sceneManager->AsynUpdateModelCacheInfo(light,true);
			m_sceneLights.push_back(light);
			//AddLightToScene(light);
		}
		this->m_sceneManager->UnLock();
	}

	//************************************
	// Method:    RemoveLight
	// FullName:  M3D::LightManager::RemoveLight
	// Access:    public 
	// Returns:   void
	// Qualifier:	从场景中移除灯光
	// Parameter: BaseLight * light
	//************************************
	void LightManager::RemoveLight(BaseLight* light)
	{
		this->m_sceneManager->Lock();
		for (int i = 0,length = m_sceneLights.size();i<length;i++)
		{
			if (m_sceneLights[i] == light)
			{
				LightUniforms* lightUnifrom = GetLightUniform(light);
				if (lightUnifrom)
				{
					delete lightUnifrom;
					lightUnifrom = NULL;
				}
				RemoveLightUniform(light);
				//m_sceneLights[i] = m_sceneLights[length - 1];	
				this->m_sceneManager->AsynUpdateModelCacheInfo(light, false); 
				m_sceneLights.erase(m_sceneLights.begin()+i);
//                light->UnBindDragger(m_sceneManager);
				ReleaseMe(light);
				this->m_sceneManager->UnLock();
				return;
				//SceneNode* node = light->GetSceneNode();
				//LightGroup* lightGroup = m_sceneManager->GetLightGroup();
				/*if (node) {
					lightGroup->DeleteChild(node);
				}*/
			}
		}
		this->m_sceneManager->UnLock();
	}

	//************************************
	// Method:    AddLightToScene
	// FullName:  M3D::LightManager::AddLightToScene
	// Access:    public 
	// Returns:   bool
	// Qualifier:	将灯光加入到场景中
	// Parameter: BaseLight * light
	//************************************
	//bool LightManager::AddLightToScene(BaseLight*light)
	//{
	//	bool ret = false;
	//	if (this->m_sceneManager == NULL || light == NULL) {
	//		return ret;
	//	}
	//	// 得到测量组节点，创建的测量节点加入到此组中
	//	LightGroup* lightGroup = m_sceneManager->GetLightGroup();

	//	// 创建一个节点，用来挂在创建的测量measureNote对象
	//	ShapeNode* node = new ShapeNode();

	//	node->SetShape(light);
	//	//note.SetScene(scene);

	//	//给新创建的Node设置一个名字
	//	char strID[10];
	//	sprintf(strID, "%d", node->GetID());
	//	node->SetName(lightGroup->GetName() + "|" + strID);

	//	//修改场景结构，加锁
	//	m_sceneManager->Lock();

	//	//将创建的测量对象加入到测量组中
	//	lightGroup->AddChild(node);

	//	m_sceneManager->AddShapeIDToMap(light);

	//	//m_sceneManager->GetRenderManager()->RequestRedraw();
	//	//完成修改，解锁
	//	m_sceneManager->UnLock();

	//	ret = true;

	//	return ret;
	//}

	//************************************
	// Method:    SetUp
	// FullName:  M3D::LightManager::SetUp
	// Access:    public 
	// Returns:   void
	// Qualifier:	更新灯光数据
	// Parameter: CameraNode * camera
	//************************************
	void LightManager::SetUp(CameraNode*camera)
	{
		m_directionalLightNumber = 0;
		m_pointLightNumber = 0;
		m_spotLightNumber = 0;
		m_hemisphereLightNumber = 0;
		m_state.Resize();
		Matrix3x4& viewMat = camera->GetView();
		float r = 0.0, g = 0.0, b = 0.0;
		for (int i = 0,l = m_sceneLights.size();i<l;i++)
		{
			
			BaseLight* light = m_sceneLights[i];
			if (!light->IsTurnOn())
			{
				continue;
			}
			LightType lightType = light->GetLightSourceType();
			LightUniforms* uniforms = GetLightUniform(light);
			
			float intensity = light->GetIntensity();
			Color lightColor = light->GetLightColor()*intensity;
			if (lightType == LightType_Ambient)
			{
				r += lightColor.m_r;
				g += lightColor.m_g;
				b += lightColor.m_b;
			}
			else if (lightType == LightType_Directional)
			{				
				DirectionalLightUniforms* directionLightUniforms = static_cast<DirectionalLightUniforms*>(uniforms);
				directionLightUniforms->color = lightColor;
			//	Vector3 target = light->GetTargetPosition();
                Vector3 dir;
                if(light->IsInWorld())
                {
                    dir = viewMat*Vector4(light->GetWorldDirection(),0.0f);
                }
                else
                {
                    dir = light->GetWorldDirection();
                }
                //Vector3 dir = Vector3(1,1,1);
				dir.Normalize();
				directionLightUniforms->direction=(dir);

				if (light->CastShadow()) 
				{
					//TODO
					DirectionalLightShadow* lightShadow = static_cast<DirectionalLightShadow* >(light->GetLightShadow());
					directionLightUniforms->shadow = 1;
					directionLightUniforms->shadowBias = lightShadow->Bias();
					directionLightUniforms->shadowMapSize = lightShadow->GetMapSize();
					directionLightUniforms->shadowRadius = lightShadow->Radius();
				}

				m_state.directional.push_back(directionLightUniforms);
				m_directionalLightNumber++;
				
			}
			else if(lightType == LightType_Point)
			{
				PointLightUniforms* pointLightUniforms = static_cast<PointLightUniforms*>(uniforms);
				PointLight* pointLight = static_cast<PointLight*>(light);
				pointLightUniforms->color = lightColor;
                
                Vector3 position;
                if(light->IsInWorld())
                {
                    position =  viewMat*Vector4( light->GetNodeWorldPosition(),1.0f);
                }
                else
                {
                    position = light->GetNodeWorldPosition();
                }

				pointLightUniforms->position = position;

				pointLightUniforms->distance = pointLight->Distance();

				pointLightUniforms->decay = pointLight->Decay();

				if (pointLight->CastShadow())
				{
					//TODO
				}

				m_state.point.push_back(pointLightUniforms);
				this->m_pointLightNumber++;

			}
			else if (lightType == LightType_Spot)
			{
				SpotLightUniforms* spotLightUniforms = static_cast<SpotLightUniforms*>(uniforms);
				SpotLight* spotLight = static_cast<SpotLight*>(light);

				spotLightUniforms->color = lightColor;

				spotLightUniforms->distance = spotLight->Distance();

				spotLightUniforms->decay = spotLight->Decay();

				float lightAngle =  spotLight->Angle()*M3D::M_DEGTORAD;
				float lightPenumbra = spotLight->Penumbra()*M3D::M_DEGTORAD;
				spotLightUniforms->coneCos = cos(lightAngle);		

				spotLightUniforms->penumbraCos = cos(lightAngle*(1.0f-lightPenumbra));				

				//Vector3 target = light->GetTargetPosition();
				//Vector3 position = light->GetNodeWorldPosition();
                
                Vector3 position;
                Vector3 dir;
                if(light->IsInWorld())
                {
                    position =  viewMat*Vector4( light->GetNodeWorldPosition(),1.0f);
                    dir = viewMat*Vector4(light->GetWorldDirection(),0.0f);
                }
                else
                {
                    position = light->GetNodeWorldPosition();
                    dir = light->GetWorldDirection();
                }
                spotLightUniforms->position = position;
				dir.Normalize();
				spotLightUniforms->direction = (dir);

				if (spotLight->CastShadow())
				{
					//TODO
				}

				m_spotLightNumber++;
				m_state.spot.push_back(spotLightUniforms);
			}
			else if (lightType == LightType_Hemisphere)
			{
				HemisphereLightUniforms * hemisphereUniforms = static_cast<HemisphereLightUniforms*>(uniforms);
				HemisphereLight* hemispherelight = static_cast<HemisphereLight*>(light);
				Vector3 position;

				if (light->IsInWorld())
				{
					position = viewMat*Vector4(light->GetNodeWorldPosition(), 0.0f);
				}
				else
				{
					position = light->GetNodeWorldPosition();
				}
				hemisphereUniforms->direction = position;
				hemisphereUniforms->direction.Normalize();

				hemisphereUniforms->skyColor = lightColor;
				hemisphereUniforms->groundColor = hemispherelight->GroundColor()*intensity;

				m_hemisphereLightNumber++;
				m_state.hemisphere.push_back(hemisphereUniforms);
			}
		}
		m_state.ambient.m_x = r;
		m_state.ambient.m_y = g;
		m_state.ambient.m_z = b;
		m_state.hash = StringHelper::IntToString(m_directionalLightNumber) + "," + StringHelper::IntToString(m_pointLightNumber) + "," + StringHelper::IntToString(m_spotLightNumber) + "," + StringHelper::IntToString(m_hemisphereLightNumber);

	}

	//************************************
	// Method:    GetState
	// FullName:  M3D::LightManager::GetState
	// Access:    public 
	// Returns:   SceneLightState&
	// Qualifier:	获取灯光灯光状态
	//************************************
	SceneLightState& LightManager::GetState()
	{
		return m_state;
	}

	void LightManager::Clear()
	{
		for (int i = 0, l = m_sceneLights.size(); i < l; i++)
		{
			BaseLight* light = m_sceneLights[i];
			RemoveLight(light);
		}
	}

	//************************************
	// Method:    Get
	// FullName:  M3D::LightManager::Get
	// Access:    private 
	// Returns:   LightUniforms*
	// Qualifier:	获取缓存的灯光uniform
	// Parameter: BaseLight * light
	//************************************
	LightUniforms* LightManager::GetLightUniform(BaseLight* light)
	{
		if (!light)
		{
			return nullptr;
		}

		LightUniforms* ret = nullptr;
		map<int, LightUniforms*>::iterator it = m_lightUniformCatch.find(light->GetID());
		if (it!= m_lightUniformCatch.end())
		{
			return it->second;
		}

		switch (light->GetLightSourceType())
		{
		//case  LightType_Ambient:
		//	ret = new AmbientLight;
		//	break;
		case LightType::LightType_Directional:
			ret = new DirectionalLightUniforms();
			break;
		case LightType::LightType_Point:
			ret = new PointLightUniforms();
			break;
		case LightType::LightType_Spot:
			ret = new SpotLightUniforms();
			break;
		case LightType::LightType_Hemisphere:
			ret = new HemisphereLightUniforms();
			break;
		default:
			break;
		}
		m_lightUniformCatch[light->GetID()] = ret;

		return ret;
	}

	void LightManager::RemoveLightUniform(BaseLight* light)
	{
		map<int, LightUniforms*>::iterator it =  m_lightUniformCatch.find(light->GetID());
		//if (it != m_lightUniformCatch.end())
		//{
			//delete it->second;
			//it->second = NULL;
		//}
		m_lightUniformCatch.erase(light->GetID());
		
	}

}

