/**@file
 *@brief
 *@author	XL
 *@date		2014-1-7
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */

#ifndef M3D_LIGHT_H_
#define M3D_LIGHT_H_
#include "m3d/M3DMacros.h"

#include "m3d/base/Color.h"
#include "m3d/base/Matrix4.h"
#include "m3d/graphics/Renderable.h"
#include "m3d/model/Shape.h"
#include "m3d/graphics/BaseLight.h"
namespace M3D
{
class RenderAction;
class DirectionalLightShadow;
/**
 * @class
 * @brief 灯光类，管理灯光及参数
 */
class M3D_API DirectionalLight: public BaseLight
{
public:
	DirectionalLight();
	virtual ~DirectionalLight();

	virtual SHAPETYPE GetType(void)
	{
		return SHAPETYPE::SHAPE_LIGHT_DIRECTIONAL;
	}

	/**
	 * 拾取
	 * @param action
	 */
	//virtual void RayPick(RayPickAction* action);
	virtual void FindVisiableObject(RenderAction* renderAction);
	/**
	 * 获取集合信息
	 * @return
	 */
//	virtual string GetGeoInfo();
	/**
	 * @brief 初始化属性
	 */
	virtual void InitProperties();

	/**
	 * Set light type.
	 * @param type
	 */
	void SetLightType(SHAPETYPE type);

	/**
	 * 	Set vertex lighting mode.
	 * @param enable
	 */
	void SetPerVertex(bool enable);

	/**
	 * Set specular intensity. Zero disables specular calculations.
	 * @param intensity
	 */
	void SetSpecularIntensity(float intensity);
	/**
	 *  Set light brightness multiplier. Both the color and specular intensity are multiplied with this to get final values for rendering.
	 *
	 * @param brightness
	 */
	void SetBrightness(float brightness);

	/**
	 *  Set range.
	 * @param range
	 */
	void SetRange(float range);

	/**
	 * Set spotlight field of view.
	 * @param fov
	 */
	void SetFov(float fov);

	/**
	 * 	Set spotlight aspect ratio.
	 * @param aspectRatio
	 */
	void SetAspectRatio(float aspectRatio);;

	/**
	 * 获取LinearAttenuation参数
	 * @return
	 */
	float & GetLinearAttenuation()
	{
		return m_linearAttenuation;
	}

	/**
	 * 设置LinearAttenuation参数
	 * @param linearAttenuation
	 */
	void SetLinearAttenuation(float linearAttenuation)
	{
		this->m_linearAttenuation = linearAttenuation;
	}

	/**
	 * 获取Ambient
	 * @return
	 */
	Color& GetAmbient()
	{
		return m_ambient;
	}

	/**
	 * 设置ambient
	 * @param ambient
	 */
	void SetAmbient(Color ambient)
	{
		m_ambient = ambient;
	}

	/**
	 * 获取Constant Attenuation
	 * @return
	 */
	float& GetConstantAttenuation()
	{
		return m_constantAttenuation;
	}

	/**
	 * 设置ConstantAttenuation
	 * @param constantAttenuation
	 */
	void SetConstantAttenuation(float constantAttenuation)
	{
		m_constantAttenuation = constantAttenuation;
	}

	/**
	 * 获取环境光
	 * @return
	 */
	Color& GetDiffuse()
	{
		return m_diffuse;
	}

	/**
	 * 设置环境光
	 * @param diffuse
	 */
	void SetDiffuse(Color diffuse)
	{
		m_diffuse = diffuse;
	}

	/**
	 * 获取LightModelAmbient
	 * @return
	 */
	Color& GetLightModelAmbient()
	{
		return m_lightModelAmbient;
	}

	/**
	 * 设置LightModelAmbient
	 * @param lightModelAmbient
	 */
	void SetLightModelAmbient(Color lightModelAmbient)
	{
		m_lightModelAmbient = lightModelAmbient;
	}

	/**
	 * 获取QuadraticAttenuation
	 * @return
	 */
	float& GetQuadraticAttenuation()
	{
		return m_quadraticAttenuation;
	}

	/**
	 * 设置QuadraticAttenuation
	 * @param quadraticAttenuation
	 */
	void SetQuadraticAttenuation(float quadraticAttenuation)
	{
		m_quadraticAttenuation = quadraticAttenuation;
	}

	/**
	 * 获取高光
	 * @return
	 */
	Color& GetSpecular()
	{
		return m_specular;
	}

	/**
	 * 设置高光
	 * @param specular
	 */
	void SetSpecular(Color specular)
	{
		m_specular = specular;
	}

	/**
	 * 获取SpotCosCutoff
	 * @return
	 */
	float& GetSpotCosCutoff()
	{
		return m_spotCosCutoff;
	}

	/**
	 * 设置SpotCosCutoff
	 * @param spotCosCutoff
	 */
	void SetSpotCosCutoff(float spotCosCutoff)
	{
		m_spotCosCutoff = spotCosCutoff;
	}

	/**
	 * 获取SpotCutoff
	 * @return
	 */
	float& GetSpotCutoff()
	{
		return m_spotCutoff;
	}

	/**
	 * 设置SpotCutoff
	 * @param spotCutoff
	 */
	void SetSpotCutoff(float spotCutoff)
	{
		m_spotCutoff = spotCutoff;
	}

	/**
	 * 获取SpotDirection
	 * @return
	 */
	Vector3 &GetSpotDirection()
	{
		return m_spotDirection;
	}

	/**
	 * 设置SpotDirection
	 * @param spotDirection
	 */
	void SetSpotDirection(Vector3 spotDirection)
	{
		m_spotDirection = spotDirection;
	}

	/**
	 * 获取SpotExponent
	 * @return
	 */
	float &GetSpotExponent()
	{
		return m_spotExponent;
	}

	/**
	 * 设置SpotExponent
	 * @param spotExponent
	 */
	void SetSpotExponent(float spotExponent)
	{
		m_spotExponent = spotExponent;
	}

	/**
	 * 获取灯光位置
	 * @return
	 */
	Vector4& GetPositionOld()
	{
		return m_position_old;
	}

	/**
	 * 设置灯光位置
	 * @param position
	 */
	void SetPositionOld(Vector4 position)
	{
		m_position_old = position;
	}


	/**
	 * Return light type.
	 * @return
	 */
	int GetLightType()
	{
		return this->m_lightType;
	}


	/**
	 *  Return vertex lighting mode.
	 * @return
	 */
	bool GetPerVertex() const
	{
		return m_perVertex;
	}

	/**
	 * Return specular intensity.
	 * @return
	 */
	float &GetSpecularIntensity();


	/**
	 * Return brightness multiplier.
	 * @return
	 */
	float GetBrightness() const
	{
		return m_brightness;
	}

	/**
	 * 	 Return effective color, multiplied by brightness. Do not multiply the alpha so that can compare against the default black color to detect a light with no effect.
	 *
	 * @return
	 */
	Color GetEffectiveColor() const
	{
		return Color(m_lightColor * m_brightness, 1.0f);
	}


	/**
	 * Return effective specular intensity, multiplied by absolute value of brightness.
	 * @return
	 */
	float GetEffectiveSpecularIntensity() const
	{
		return m_specularIntensity * Abs(m_brightness);
	}


	/**
	 * Return range.
	 * @return
	 */
	float GetRange() const
	{
		return m_range;
	}


	/**
	 * Return spotlight field of view.
	 * @return
	 */
	float GetFov() const
	{
		return m_fov;
	}


/**
 * Return spotlight aspect ratio.
 * @return
 */
	float GetAspectRatio() const
	{
		return m_aspectRatio;
	}

	//void SetLightType(int type) { m_lightType = type; }

	virtual LightShadow* GetLightShadow();

private:
	float m_specularIntensity;//!<Specular intensity.

	float m_brightness;//!<Brightness multiplier.

	float m_range;//!<Range.

	float m_fov;//!<Spotlight field of view.

	float m_aspectRatio;//!<Spotlight aspect ratio.

	bool m_perVertex;//!<Per-vertex lighting flag.
	//Vector4 m_position;//!<光源位置，当w为0时，表示方向光源，其他为点光源或聚光灯

	Vector4 m_position_old;//兼容以前代码

	Vector3 m_spotDirection;//!<聚光灯朝向

	float m_spotExponent;//!<聚光灯
	float m_spotCutoff;//!<cut off
	float m_spotCosCutoff;//!< cos cut off

	float m_constantAttenuation;//!<衰减系数
	float m_linearAttenuation;//!<衰减系数
	float m_quadraticAttenuation;//!<衰减系数

	float m_intensity;//!<光强

	Color m_ambient;//!<环境光
	Color m_diffuse;//!<漫反射
	Color m_specular;//!<高光
	Color m_lightModelAmbient;//!<

	vector<Vector3> m_squarePoints;

	//int m_lightType;//!< 1 平行光 2 点光源 3 聚光灯

	DirectionalLightShadow* m_lightShadow;
};
}
#endif /*M3D_LIGHT_H_*/
