#ifndef M3D_HEMISPHERE_LIGHT_H_
#define M3D_HEMISPHERE_LIGHT_H_
#include "m3d/graphics/BaseLight.h"
namespace M3D
{
	/************************************************************************/
	/* 半球光源                                                                     */
	/************************************************************************/
	class M3D_API HemisphereLight : public BaseLight
	{
	public:
		HemisphereLight();
		~HemisphereLight();
		//************************************
		// Method:    GroundColor
		// FullName:  M3D::HemisphereLight::GroundColor
		// Access:    public 
		// Returns:   Color&
		// Qualifier: 地面颜色设置
		//************************************
		Color& GroundColor() { return m_groundColor; }
		void GroundColor(Color val) { m_groundColor = val; }
	private:
		Color m_groundColor;
	};
}
#endif


