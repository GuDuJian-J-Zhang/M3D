#include "m3d/scene/LightGroup.h"
#include "m3d/model/Note.h"
#include "m3d/scene/ShapeNode.h"
#include "m3d/SceneManager.h"
#include "m3d/graphics/CameraNode.h"
#include "m3d/renderer/LightManager.h"

namespace M3D
{

LightGroup::LightGroup()
{
	this->SetScene(NULL);
}


LightGroup::~LightGroup()
{

}

void LightGroup::Initialize()
{

}

int LightGroup::GetType( void )
{
	return MEASURE_GROUP_NODE;
}

void LightGroup::SetScene(SceneManager* scene)
{
	this->m_scene = scene ;
}

SceneManager* LightGroup::GetScene()
{
	return this->m_scene ;
}

void LightGroup::RayPick(RayPickAction * action)
{
	GroupNode::RayPick(action);

	LightManager* lightMgr = this->m_scene->GetLightManager();
	vector<BaseLight*>& allLights =  lightMgr->GetAllLight();
	for (int i = 0;i<allLights.size();i++)
	{
		allLights.at(i)->RayPick(action);
	}
}

void LightGroup::FindVisiableObject(RenderAction * renderAction)
{
	GroupNode::FindVisiableObject(renderAction);

	LightManager* lightMgr = this->m_scene->GetLightManager();
	vector<BaseLight*>& allLights = lightMgr->GetAllLight();
	for (int i = 0; i < allLights.size(); i++)
	{
		allLights.at(i)->FindVisiableObject(renderAction);
	}
}

void LightGroup::OnMarkedDirty()
{
	GroupNode::OnMarkedDirty();

	LightManager* lightMgr = this->m_scene->GetLightManager();
	vector<BaseLight*>& allLights = lightMgr->GetAllLight();
	for (int i = 0; i < allLights.size(); i++)
	{
		//allLights.at(i)->RayPick(action);
	}
}

}
