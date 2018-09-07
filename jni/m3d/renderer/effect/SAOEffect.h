#ifndef M3D_SAO_EFFECT_H_
#define M3D_SAO_EFFECT_H_
#include "m3d/renderer/effect/Effect.h"
#include "m3d/graphics/HardWareFrameBuffer.h"
namespace M3D
{
	/************************************************************************/
	/* SAO特效，效果不好，测试用。                                                                */
	/************************************************************************/
	class M3D_API SAOEffect :public Effect
	{
	public:
		SAOEffect(RenderAction* action);
		void Render();
		void Render(RenderQueue* RenderStateQueue);
		void ClearResource();
		void SetSize(int w, int h);
		GLint GetSAOTextureObject();
		GeometryBuffer* GetSAOTexture();
	private:
		void InitBuffers();
		void RenderModel(RenderQueue * RenderStateArray);
		void RenderSAO();
		void RenderBlur();
		void RenderCameraSpaceZ();
		void RenderCameraSpaceMinifyZ(int level);
		void RenderHorizontalBlur();
		void RenderVerticalBlur();
		//void RenderGaussianBlurWidth();
		//void RenderGaussianBlurHeight();
		void InnerRender(vector<RenderQueue*>& RenderStateArray);

		void DrawFrameBufferDebug();
	private:
		bool m_firstTime;
		int m_compute_guard_band;
		GLuint m_ssaoTextureObj;
		GeometryBuffer* m_ssaoTexture;
		GeometryBuffer* m_cszBuffer;
		
	};
}
#endif//M3D_SSAO_EFFECT_H_