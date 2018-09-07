#include "m3d/graphics/DirectionalLightShadow.h"
#include "m3d/graphics/CameraNode.h"
#include "BaseLight.h"
#include "m3d/SceneManager.h"

namespace M3D
{

	DirectionalLightShadow::DirectionalLightShadow():
		LightShadow()
	{
		Init();
	}

	DirectionalLightShadow::~DirectionalLightShadow()
	{
		m_camera->Release();
		m_camera = nullptr;
	}

	void DirectionalLightShadow::SetCameraInfo(float f_size, float f_near, float f_far)
	{
		orthoSize = f_size;
		cameraNear = f_near;
		cameraFar = f_far;
		needUpdata = true;
	}

	void DirectionalLightShadow::Update(BaseLight* light, CameraNode* mainCamera, int face /*= 1*/)
	{
		UpdateCamera(light,mainCamera);
		UpdateMatrix();
	}

	void DirectionalLightShadow::UpdateCamera(BaseLight* light, CameraNode* mainCamera)
	{
		Vector3 position = light->GetNodeWorldPosition();
		Vector3 direction = -light->GetWorldDirection();
		direction.Normalize();
		Vector3 target;
		BoundingBox sceneBox = mainCamera->GetSceneManager()->GetSceneBox();
		Vector3 center = sceneBox.Center();
		float length = sceneBox.Length();
		if (!light->IsInWorld())
		{
			Matrix3x4 inv = mainCamera->GetView().Inverse();
			direction = inv*Vector4( direction,0.0);
			//position = inv*Vector4(position, 1.0);
			target = center;
			position = -direction*length*0.5 + center;
		}
		else
		{
			target = position + direction;
		}
		
		
		/*if (!m_camera->GetWorldPosition().Equals(position)) */
		{
			m_camera->SetWorldPosition(position);
			m_camera->LookAt(target, Vector3(0, 1, 0), TS_WORLD);
		}
		if (needUpdata) {
			m_camera->SetOrthoSize(Vector2(orthoSize,orthoSize));
			m_camera->SetNearClip(cameraNear);
			m_camera->SetFarClip(cameraFar);
			needUpdata = false;
		}
	}

	void DirectionalLightShadow::UpdateMatrix()
	{
		
		m_matrix =  Matrix4(
			0.5, 0.0, 0.0, 0.5,
			0.0, 0.5, 0.0, 0.5,
			0.0, 0.0, 0.5, 0.5,
			0.0, 0.0, 0.0, 1.0
		);
		//this.matrix.TransposeED();

		m_matrix = m_matrix*(m_camera->GetProjection());
		m_matrix = m_matrix*(m_camera->GetView().ToMatrix4());
	}
	 
	void DirectionalLightShadow::Init()
	{
		orthoSize = 64;
		cameraNear = 0.1;
		cameraFar = 10000;
		m_mapSize = Vector2(2048, 2048);
		m_camera = new CameraNode;
		m_camera->SetOrthographic(true);
		m_camera->SetOrthoSize(Vector2(orthoSize, orthoSize));
		m_camera->SetNearClip(cameraNear);
		m_camera->SetFarClip(cameraFar);
		needUpdata = false;
		m_bias = 0.0000;
		m_radius = 4.0;
	}

}