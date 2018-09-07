#ifndef  OUTLINEEFFECT_H_
#define  OUTLINEEFFECT_H_
#include "m3d/renderer/effect/Effect.h"
#include "m3d/graphics/HardWareFrameBuffer.h"
namespace M3D
{
	/************************************************************************/
	/* ѡ���������Ч                                                                     */
	/************************************************************************/
	class M3D_API OutlineEffect :public Effect
	{

	public:
		OutlineEffect(RenderAction* action);
		void Render();
		void Render(RenderQueue* RenderStateQueue);
		void ClearResource();
		void SetSize(int w, int h);
		GLint GetOutlineTextureObject();
	private:
		//************************************
		// Method:    RenderModel
		// FullName:  M3D::OutlineEffect::RenderModel
		// Access:    private 
		// Returns:   void
		// Qualifier: ��Ⱦģ��
		// Parameter: RenderQueue * RenderStateArray
		//************************************
		void RenderModel(RenderQueue * RenderStateArray);
		//************************************
		// Method:    RenderOutline
		// FullName:  M3D::OutlineEffect::RenderOutline
		// Access:    private 
		// Returns:   void
		// Qualifier: ��Ⱦ�����
		//************************************
		void RenderOutline();
		//************************************
		// Method:    RenderGaussianBlurWidth
		// FullName:  M3D::OutlineEffect::RenderGaussianBlurWidth
		// Access:    private 
		// Returns:   void
		// Qualifier: ��˹ģ��
		//************************************
		void RenderGaussianBlurWidth();
		void RenderGaussianBlurHeight();
		void InnerRender(vector<RenderQueue*>& RenderStateArray);

		//************************************
		// Method:    DrawFrameBufferDebug
		// FullName:  M3D::OutlineEffect::DrawFrameBufferDebug
		// Access:    private 
		// Returns:   void
		// Qualifier: ���Ժ���
		//************************************
		void DrawFrameBufferDebug();
	private:
		bool m_firstTime;
		GLuint m_outlineTextureObj;


	};
}

#endif