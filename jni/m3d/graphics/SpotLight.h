#ifndef M3D_SPOT_LIGHT_H_
#define M3D_SPOT_LIGHT_H_
#include "m3d/graphics/BaseLight.h"
namespace M3D
{
	/************************************************************************/
	/* �۹���ࡣ                                                                     */
	/************************************************************************/
	class M3D_API SpotLight : public BaseLight
	{
	public:
		SpotLight();
		~SpotLight();

		virtual SHAPETYPE GetType(void)
		{
			return SHAPETYPE::SHAPE_LIGHT_SPOT;
		}
		//�����ɲ���three�ƹ�
		float Decay() const { return m_decay; }
		void Decay(float val) { m_decay = val; }
		float Distance() const { return m_distance; }
		void Distance(float val) { m_distance = val; }
		float Angle() const { return m_angle; }
		void Angle(float val) { m_angle = val; }
		float Penumbra() const { return m_penumbra; }
		void Penumbra(float val) { m_penumbra = val; }

		virtual void FindVisiableObject(RenderAction* renderAction);
	private:
		float m_decay;
		float m_distance;
		float m_angle;
		float m_penumbra;
	};
}
#endif