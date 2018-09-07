#ifndef M3D_SSAO_EFFECT_H_
#define M3D_SSAO_EFFECT_H_
#include "m3d/renderer/effect/Effect.h"
#include "m3d/graphics/HardWareFrameBuffer.h"
namespace M3D
{
	class M3D_API SSAOEffect:public Effect
	{
	public:
		SSAOEffect(RenderAction* action);
		void Render();
		void Render(RenderQueue* RenderStateQueue);
		void ClearResource();
		void SetSize(int w, int h);
		GLint GetSSAOTextureObject();
		GeometryBuffer* GetSSAOTexture();
	private:
		void RenderModel(RenderQueue * RenderStateArray);
		void RenderSSAO();
		void RenderBlur();
		//void RenderGaussianBlurWidth();
		//void RenderGaussianBlurHeight();
		void InnerRender(vector<RenderQueue*>& RenderStateArray);

		void DrawFrameBufferDebug();
	private:
		bool m_firstTime;
		GLuint m_ssaoTextureObj;
		GeometryBuffer* m_ssaoTexture;
	};
}
#endif//M3D_SSAO_EFFECT_H_