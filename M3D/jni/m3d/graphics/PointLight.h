

#ifndef M3D_POINT_LIGHT_H_
#define M3D_POINT_LIGHT_H_
#include "m3d/graphics/BaseLight.h"
namespace M3D
{
	/************************************************************************/
	/* 点光源                                                                     */
	/************************************************************************/
	class M3D_API PointLight : public BaseLight
	{
	public:
		PointLight();
		~PointLight();

		virtual SHAPETYPE GetType(void)
		{
			return SHAPETYPE::SHAPE_LIGHT_POINT;
		}

		//************************************
		// Method:    Decay
		// FullName:  M3D::PointLight::Decay
		// Access:    public 
		// Returns:   float
		// Qualifier: 衰减
		//************************************
		float Decay() const { return m_decay; }
		void Decay(float val) { m_decay = val; }
		//************************************
		// Method:    Distance
		// FullName:  M3D::PointLight::Distance
		// Access:    public 
		// Returns:   float
		// Qualifier: 影响距离
		//************************************
		float Distance() const { return m_distance; }
		void Distance(float val) { m_distance = val; }
		virtual void FindVisiableObject(RenderAction* renderAction);
	private:
		float m_decay;
		float m_distance;
	};
}
#endif