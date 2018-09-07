#ifndef M3D_DIRECTIONAL_LIGHT_SHADOW_H_
#define M3D_DIRECTIONAL_LIGHT_SHADOW_H_
#include "m3d/graphics/LightShadow.h"
namespace M3D
{
	/************************************************************************/
	/* ƽ���������Ӱ�ࡣ                                                                     */
	/************************************************************************/
	class DirectionalLightShadow : public LightShadow
	{
	public:
		DirectionalLightShadow();
		~DirectionalLightShadow();
		//************************************
		// Method:    SetCameraInfo
		// FullName:  M3D::DirectionalLightShadow::SetCameraInfo
		// Access:    public 
		// Returns:   void
		// Qualifier: ���������Ϣ
		// Parameter: float f_size
		// Parameter: float f_near
		// Parameter: float f_far
		//************************************
		void SetCameraInfo(float f_size,float f_near,float f_far);
		//************************************
		// Method:    Update
		// FullName:  M3D::DirectionalLightShadow::Update
		// Access:    public 
		// Returns:   void
		// Qualifier: ��������
		// Parameter: BaseLight * light
		// Parameter: CameraNode * mainCamera
		// Parameter: int face
		//************************************
		void Update(BaseLight* light, CameraNode* mainCamera, int face = 1);
		//************************************
		// Method:    UpdateCamera
		// FullName:  M3D::DirectionalLightShadow::UpdateCamera
		// Access:    public 
		// Returns:   void
		// Qualifier: �������
		// Parameter: BaseLight * light
		// Parameter: CameraNode * mainCamera
		//************************************
		void UpdateCamera(BaseLight* light, CameraNode* mainCamera);
		//************************************
		// Method:    UpdateMatrix
		// FullName:  M3D::DirectionalLightShadow::UpdateMatrix
		// Access:    public 
		// Returns:   void
		// Qualifier: ���¾���
		//************************************
		void UpdateMatrix();
	private:
		void Init();
		float orthoSize;
		float cameraNear;
		float cameraFar;
		bool needUpdata;
		Matrix4 lightMatrix;
	};
}
#endif//M3D_DIRECTIONAL_LIGHT_SHADOW_H_