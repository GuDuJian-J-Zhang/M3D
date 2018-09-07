#include "m3d/scene/CameraGroup.h"
#include "m3d/model/Note.h"
#include "m3d/scene/ShapeNode.h"
#include "m3d/SceneManager.h"
#include "m3d/graphics/CameraNode.h"

namespace M3D
{

CameraGroup::CameraGroup()
{
	this->SetScene(NULL);
}

CameraGroup::~CameraGroup()
{

}

void CameraGroup::Initialize()
{

}


int CameraGroup::GetType( void )
{
	return CAMERA_GROUP_NODE;
}


void CameraGroup::SetScene(SceneManager* scene)
{
	this->m_scene = scene ;
}


SceneManager* CameraGroup::GetScene()
{
	return this->m_scene ;
}

void CameraGroup::RayPick(RayPickAction * action)
{
	GroupNode::RayPick(action);

	CameraManager* cameraMgr = this->m_scene->GetCameraManager();
	vector<CameraNode*>& allCameras = cameraMgr->GetAllCameras();
	for (int i = 0; i < allCameras.size(); i++)
	{
		allCameras.at(i)->RayPick(action);
	}
}

void CameraGroup::FindVisiableObject(RenderAction * renderAction)
{
	GroupNode::FindVisiableObject(renderAction);

	CameraManager* cameraMgr = this->m_scene->GetCameraManager();
	vector<CameraNode*>& allCameras = cameraMgr->GetAllCameras();
	for (int i = 0; i < allCameras.size(); i++)
	{
		allCameras.at(i)->FindVisiableObject(renderAction);
	}
}

void CameraGroup::OnMarkedDirty()
{
	GroupNode::OnMarkedDirty();

	CameraManager* cameraMgr = this->m_scene->GetCameraManager();
	vector<CameraNode*>& allCameras = cameraMgr->GetAllCameras();
	for (int i = 0; i < allCameras.size(); i++)
	{
		//allLights.at(i)->RayPick(action);
	}
}

CameraManager::CameraManager()
{
}


CameraManager::~CameraManager()
{
}

void CameraManager::AddCamera(CameraNode* camera)
{
	if (camera) {
		for (int i = 0; i < m_sceneCameras.size(); i++) {
			if (m_sceneCameras[i] == camera) {
				return;
			}
		}
		AddRefMe(camera);
		this->m_sceneManager->AsynUpdateModelCacheInfo(camera, true);
		m_sceneCameras.push_back(camera);
	}
}

void CameraManager::RemoveCamera(CameraNode* camera)
{
	for (int i = 0, length = m_sceneCameras.size(); i<length; i++)
	{
		if (m_sceneCameras[i] == camera)
		{
			this->m_sceneManager->AsynUpdateModelCacheInfo(camera, false);
			m_sceneCameras.erase(m_sceneCameras.begin() + i);
			ReleaseMe(camera);
			return;
		}
	}
}
}
