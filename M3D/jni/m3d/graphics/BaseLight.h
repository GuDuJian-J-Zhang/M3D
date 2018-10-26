#ifndef M3D_BASE_LIGHT_H_
#define M3D_BASE_LIGHT_H_
#include "m3d/M3DMacros.h"

#include "m3d/base/Color.h"
#include "m3d/base/Matrix4.h"
#include "m3d/graphics/Renderable.h"
#include "m3d/model/Model.h"
namespace M3D
{
	class LightShadow;
	enum  M3D_API LightType
	{
		LightType_Ambient = 1,
		LightType_Directional,
		LightType_Point,
		LightType_Spot,
		LightType_Hemisphere
	};
	/************************************************************************/
	/* 光源基类。                                                                     */
	/************************************************************************/
	class M3D_API BaseLight : public SignModel, public Renderable
	{
	public:
		BaseLight();
		~BaseLight();
		//************************************
		// Method:    GetLightSourceType
		// FullName:  M3D::BaseLight::GetLightSourceType
		// Access:    public 
		// Returns:   M3D::LightType
		// Qualifier:  获取光源类型
		//************************************
		LightType GetLightSourceType() const;
		//************************************
		// Method:    SetLightSourceType
		// FullName:  M3D::BaseLight::SetLightSourceType
		// Access:    public 
		// Returns:   void
		// Qualifier: 设置光源类型
		// Parameter: M3D::LightType val
		//************************************
		void SetLightSourceType(M3D::LightType val);
		//************************************
		// Method:    GetLightColor
		// FullName:  M3D::BaseLight::GetLightColor
		// Access:    public 
		// Returns:   Color
		// Qualifier: 获取光源颜色
		//************************************
		Color GetLightColor() const;
		//************************************
		// Method:    SetLightColor
		// FullName:  M3D::BaseLight::SetLightColor
		// Access:    public 
		// Returns:   void
		// Qualifier: 设置光源颜色
		// Parameter: Color & val
		//************************************
		void SetLightColor(Color& val);
		//************************************
		// Method:    GetIntensity
		// FullName:  M3D::BaseLight::GetIntensity
		// Access:    public 
		// Returns:   float
		// Qualifier: 获取光源强度
		//************************************
		float GetIntensity() const;
		//************************************
		// Method:    SetIntensity
		// FullName:  M3D::BaseLight::SetIntensity
		// Access:    public 
		// Returns:   void
		// Qualifier: 设置光源强度
		// Parameter: float val
		//************************************
		void SetIntensity(float val);
		//************************************
		// Method:    GetNodeWorldPosition
		// FullName:  M3D::BaseLight::GetNodeWorldPosition
		// Access:    public 
		// Returns:   M3D::Vector3
		// Qualifier: 获取光源位置
		//************************************
		Vector3 GetNodeWorldPosition();
		//************************************
		// Method:    SetNodeWorldPosition
		// FullName:  M3D::BaseLight::SetNodeWorldPosition
		// Access:    public 
		// Returns:   void
		// Qualifier: 设置光源位置
		// Parameter: Vector3 & position
		//************************************
		void SetNodeWorldPosition(Vector3& position);
		//************************************
		// Method:    SetCastShadow
		// FullName:  M3D::BaseLight::SetCastShadow
		// Access:    public 
		// Returns:   void
		// Qualifier: 设置光源是否投射阴影
		// Parameter: bool castShadow
		//************************************
		void SetCastShadow(bool castShadow);
		//************************************
		// Method:    CastShadow
		// FullName:  M3D::BaseLight::CastShadow
		// Access:    public 
		// Returns:   bool
		// Qualifier: 获取光源是否投射阴影
		//************************************
		bool CastShadow() const;

		//************************************
		// Method:    GetWorldDirection
		// FullName:  M3D::BaseLight::GetWorldDirection
		// Access:    public 
		// Returns:   M3D::Vector3
		// Qualifier: 获取光源方向
		//************************************
		Vector3 GetWorldDirection();

		virtual void RayPick(RayPickAction* action);
		virtual void FramePick(RayPickAction* action);

		virtual void FindVisiableObject(RenderAction* renderAction);

		bool NeedUpdateInfo() const;
		void NeedUpdateInfo(bool val);
		virtual SHAPETYPE GetType(void);
		//************************************
		// Method:    IsInWorld
		// FullName:  M3D::BaseLight::IsInWorld
		// Access:    public 
		// Returns:   bool
		// Qualifier: 是否光源是静态光源。true是静态光源，false为动态光源，即位置是设置在摄像机上的
		//************************************
		bool IsInWorld();
		void IsInWorld(bool value);
		virtual LightShadow* GetLightShadow();
		//************************************
		// Method:    IsTurnOn
		// FullName:  M3D::BaseLight::IsTurnOn
		// Access:    public 
		// Returns:   void
		// Qualifier: 光源开关
		// Parameter: bool value
		//************************************
		void IsTurnOn(bool value);
		bool IsTurnOn();
		virtual void CreateSignModel(SceneManager* sceneManager) {};
		virtual void BindDragger(SceneManager* sceneManager) {};
		virtual void UnBindDragger(SceneManager* sceneManager) {};
	protected:
		virtual void SetUpHelperData();
	protected:
		Color m_lightColor;
		LightType m_lightType;
		float m_intensity;
		Vector3 m_direction;
		Vector3 m_worldPosition;
		bool m_castShadow;
		bool m_needUpdateInfo;
		bool m_isInWorld;
		Matrix4 m_glworldMatrix; //渲染是所需的矩阵
		bool m_isTurnOn;
		
	};
}
#endif


