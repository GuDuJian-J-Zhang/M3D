#ifndef XRAY_EFFECT_H_
#define XRAY_EFFECT_H_
#include "m3d/renderer/effect/Effect.h"

namespace M3D
{
	class RenderAction;
	class RenderQueue;

	class M3D_API XRayEffect :public Effect
	{
	public:
		XRayEffect(RenderAction* action);
		~XRayEffect();

		virtual void Render();
		void DrawXRayPass(RenderAction * action, RenderQueue * RenderStateArray);
		void DrawBasePass(RenderAction * action, RenderQueue * RenderStateArray);
	};
}
#endif