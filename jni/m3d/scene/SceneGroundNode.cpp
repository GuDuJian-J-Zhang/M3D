/*
 * SceneGroundNode.cpp
 *
 *  Created on: 2016-4-6
 *      Author: CDJ
 */

#include "m3d/scene/SceneGroundNode.h"
#include "m3d/SceneManager.h"
#include "m3d/graphics/CameraNode.h"
#include "m3d/action/RenderAction.h"

namespace M3D
{

SceneGroundNode::SceneGroundNode()
{
	m_renderAction = NULL;


}

SceneGroundNode::~SceneGroundNode()
{

}
int SceneGroundNode::GetType(void)
{
	return SCENEGROUND_NODE;
}

void SceneGroundNode::FindVisiableObject(RenderAction* renderAction)
{
	//if (!this->IsVisible() || true)
	//{
	//	renderAction->SetSceneGroundNode(NULL);
	//}
	//else
	//{
	//	m_renderAction = renderAction;
	//	CameraNode cameraLight;
	//	CameraNode * camera = m_renderAction->GetCamera();
	//	SceneManager * scene = m_renderAction->GetScene();
	//	BoundingBox& pBoundingBox = scene->GetSceneBox();
	//	Vector3 center = pBoundingBox.Center();
	//	float length = pBoundingBox.Length();
	//	Vector3 lightPos(center.m_x, center.m_y + length, center.m_z);
	//	cameraLight.SetWorldPosition(lightPos);
	//	cameraLight.LookAt(center, Vector3::LEFT, TS_WORLD);

	//	float w, h;
	//	camera->GetOrthoSize(&w, &h);
	//	cameraLight.SetOrthoSize(Vector2(w, h));
	//	cameraLight.SetNearClip(camera->GetNearClip());
	//	cameraLight.SetFarClip(camera->GetFarClip());
	//	m_projectMat = cameraLight.GetProjection().Transpose();
	//	m_viewMat = cameraLight.GetView().ToMatrix4().Transpose();

	//	renderAction->SetSceneGroundNode(this);
	//}
}


Vector3* SceneGroundNode::GetVertexs()
{

	if(!m_renderAction)
	{
		return NULL;
	}

	CameraNode * camera = m_renderAction->GetCamera();
	SceneManager * scene = m_renderAction->GetScene();
	BoundingBox& pBoundingBox = scene->GetSceneBox();
	Vector3 center = pBoundingBox.Center();
	float YLength = pBoundingBox.GetYLength();

	if(m_vertexs.size() == 0 ||m_box != pBoundingBox)
	{
		MutexLock lock(m_mutex);
		m_vertexs.clear();
		m_box = pBoundingBox;
		float length = pBoundingBox.Length();
		Vector3 PX = center + length * Vector3::RIGHT;
		Vector3 NX = center + length * Vector3::LEFT;
		Vector3 PZ = center + length * Vector3::FORWARD;
		Vector3 NZ = center + length * Vector3::BACK;

		m_vertexs.push_back(Vector3(NX.m_x, center.m_y - YLength * 0.51f, NZ.m_z));

		m_vertexs.push_back(Vector3(NX.m_x, center.m_y - YLength * 0.51f, PZ.m_z));

		m_vertexs.push_back(Vector3(PX.m_x, center.m_y - YLength * 0.51f, NZ.m_z));

		m_vertexs.push_back(Vector3(PX.m_x, center.m_y - YLength * 0.51f, PZ.m_z));
	}

	return m_vertexs.data();
}

Vector3* SceneGroundNode::GetNormals()
{
	if (m_normals.size() == 0)
	{
		MutexLock lock(m_mutex);

		m_normals.push_back(Vector3(0.0, 1.0,0.0));

		m_normals.push_back(Vector3(0.0, 1.0,0.0));

		m_normals.push_back(Vector3(0.0, 1.0,0.0));

		m_normals.push_back(Vector3(0.0, 1.0,0.0));
	}

	return m_normals.data();
}

Vector2* SceneGroundNode::GetTexCoords()
{
	if (m_texCoords.size() == 0)
	{
		MutexLock lock(m_mutex);

		m_texCoords.push_back(Vector2(0.0, 1.0));

		m_texCoords.push_back(Vector2(0.0, 0.0));

		m_texCoords.push_back(Vector2(1.0, 1.0));

		m_texCoords.push_back(Vector2(1.0, 0.0));
	}

	return m_texCoords.data();
}

Matrix4& SceneGroundNode::GetProjectMat()
{
	return m_projectMat;
}

Matrix4& SceneGroundNode::GetViewMat()
{
	return m_viewMat;
}

Vector3& SceneGroundNode::GetSkyLightPos()
{
	return m_skyLightPos;
}

}





 /* namespace M3D */
