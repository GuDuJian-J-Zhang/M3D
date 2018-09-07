#ifndef M3D_LIGHT_SHADOW_H_
#define M3D_LIGHT_SHADOW_H_
#include "m3d/base/Matrix4.h"
namespace M3D
{
	class CameraNode;
	class HardWareFrameBuffer;
	class BaseLight;
	/************************************************************************/
	/* 灯光阴影的基类。现在完成了平行光的                                                                     */
	/************************************************************************/
	class LightShadow
	{
	public:
		LightShadow();
		virtual void Update(BaseLight* light, CameraNode* mainCamera,int face = 1);
		virtual void SetMapSize(Vector2& size);
		virtual Vector2& GetMapSize();
		virtual CameraNode* GetCamera();

		virtual float Bias();
		virtual void Bias(float val);
		virtual float Radius();
		virtual void Radius(float val);
		virtual Matrix4& Matrix();
		
		virtual HardWareFrameBuffer* RenderTarget();
		virtual void RenderTarget(HardWareFrameBuffer* val);
	protected:
		float m_bias;
		float m_radius;
		Vector2 m_mapSize;
		Matrix4 m_matrix;
		CameraNode* m_camera;
		HardWareFrameBuffer* renderTarget;
	};
}

#endif//M3D_LIGHT_SHADOW_H_