#ifndef M3D_HEMISPHERE_LIGHT_H_
#define M3D_HEMISPHERE_LIGHT_H_
#include "m3d/graphics/BaseLight.h"
namespace M3D
{
	/************************************************************************/
	/* �����Դ                                                                     */
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
		// Qualifier: ������ɫ����
		//************************************
		Color& GroundColor() { return m_groundColor; }
		void GroundColor(Color val) { m_groundColor = val; }
	private:
		Color m_groundColor;
	};
}
#endif


