#include "m3d/graphics/CameraNode.h"
#include "m3d/scene/ShapeNode.h"
#include "m3d/SceneManager.h"
#include "sview/extern/WorkNodes.h"

#include "m3d/model/Shape.h"
#include "m3d/model/Model.h"
namespace M3D
{
void WorkNodes::AddNode(const string& name, SceneNode* node)
{
	//RemoveNode(name);
	map<string, SceneNode*>::iterator it = m_workNodesMap.find(name);
	if (it != m_workNodesMap.end())
	{
		it->second = node;
	}
	else
	{
		m_workNodesMap.insert(
				map<string, SceneNode*>::value_type(name, node));
	}
}

void WorkNodes::RemoveNode(const string& name)
{
	map<string, SceneNode*>::iterator it = m_workNodesMap.find(name);
	if (it != m_workNodesMap.end())
	{
		m_workNodesMap.erase(it);
	}
}

int WorkNodes::Size()
{
	return m_workNodesMap.size();
}

void WorkNodes::SetRotationAroundCenter(const Quaternion& rotation)
{
	map<string, SceneNode*>::iterator it = m_workNodesMap.find(AXIS);
	if (it != m_workNodesMap.end())
	{
		it->second->SetRotation(rotation.Inverse());
	}

	it = m_workNodesMap.find(MAINCAMERA);
	if (it != m_workNodesMap.end())
	{
		CameraNode* camera = ((CameraNode*)it->second);
		//根据当前摄像机状态，求出应该做的旋转增量
		Quaternion deltaRotation = rotation.Inverse();
		deltaRotation = camera->GetRotation().Inverse()* deltaRotation;
		camera->RotateAroundCenter(deltaRotation,TS_WORLD);
	}
}

void WorkNodes::SetRotation(const Quaternion& rotation)
{
	map<string, SceneNode*>::iterator it = m_workNodesMap.find(AXIS);
	if (it != m_workNodesMap.end())
	{
		it->second->SetRotation(rotation.Inverse());
	}

	it = m_workNodesMap.find(MAINCAMERA);
	if (it != m_workNodesMap.end())
	{
		CameraNode* camera = ((CameraNode*)it->second);
		camera->SetRotation(rotation);
	}
}

void WorkNodes::SetPosition(const Vector3& mov)
{
	map<string, SceneNode*>::iterator it = m_workNodesMap.find(MAINCAMERA);
	if (it != m_workNodesMap.end())
	{
		CameraNode* camera = ((CameraNode*)it->second);
		camera->SetWorldPosition(mov);
	}

}

void WorkNodes::SetZoom(const float zoom)
{
	map<string, SceneNode*>::iterator it = m_workNodesMap.find(MAINCAMERA);
	if (it != m_workNodesMap.end())
	{
		((CameraNode*) it->second)->SetZoom(1/zoom);
	}

}

void WorkNodes::GetRotaton(Quaternion& rotation)
{
	map<string, SceneNode*>::iterator it = m_workNodesMap.find(MAINCAMERA);
	if (it != m_workNodesMap.end())
	{
		rotation = it->second->GetRotation();
	}
}

void WorkNodes::GetMVMatrix(ControlInfo& mvMatrix,
		ControlInfo& oldmvMatrix)
{
	map<string, SceneNode*>::iterator it = m_workNodesMap.find(MAINCAMERA);
	if (it != m_workNodesMap.end())
	{
		mvMatrix.rotation = it->second->GetRotation();
		mvMatrix.moveVector = it->second->GetPosition();
		mvMatrix.scaleFactor =  ((CameraNode*) it->second)->GetZoom();

		oldmvMatrix.rotation = it->second->GetOldRotation();
		oldmvMatrix.moveVector = it->second->GetOldPosition();
		oldmvMatrix.scaleFactor =  ((CameraNode*) it->second)->GetZoom();
	}

}
void WorkNodes::RotateAroundCenter(const Quaternion& rotation)
{
	Quaternion trotaton = rotation;
	map<string, SceneNode*>::iterator it;
	float scale = 1;
	it = m_workNodesMap.find(MAINCAMERA);
	if (it != m_workNodesMap.end())
	{
		scale = ((CameraNode*) it->second)->GetZoom();

		((CameraNode*)it->second)->RotateAroundCenter(trotaton,TS_WORLD);
	}

	it = m_workNodesMap.find(AXIS);
	if (it != m_workNodesMap.end())
	{
		it->second->Rotate(trotaton);
	}

}

void WorkNodes::Rotate(const Quaternion& rotation)
{
	Quaternion trotaton = rotation;
	map<string, SceneNode*>::iterator it;
	float scale = 1;
	it = m_workNodesMap.find(MAINCAMERA);
	if (it != m_workNodesMap.end())
	{
		scale = ((CameraNode*) it->second)->GetZoom();

		((CameraNode*)it->second)->Rotate(trotaton,TS_WORLD);
	}

	it = m_workNodesMap.find(AXIS);
	if (it != m_workNodesMap.end())
	{
		it->second->Rotate(trotaton);
	}

}

void WorkNodes::Translate(const Vector3& vec, const Vector3& curPos)
{
	map<string, SceneNode*>::iterator it = m_workNodesMap.find(MAINCAMERA);
	if (it != m_workNodesMap.end())
	{
		it->second->Translate(vec*-1,TS_LOCAL);
	}
}

void WorkNodes::Zoom(float scaleFactor)
{
	map<string, SceneNode*>::iterator it = m_workNodesMap.find(MAINCAMERA);
	if (it != m_workNodesMap.end())
	{
		((CameraNode*) it->second)->ZoomView(1/scaleFactor);
	}
}
}
