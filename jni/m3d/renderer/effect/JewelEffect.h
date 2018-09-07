#ifndef  JEWELEFFECT_H_
#define  JEWELEFFECT_H_
#include "m3d/renderer/effect/Effect.h"
#include "m3d/graphics/HardWareFrameBuffer.h"
namespace M3D
{	
	/************************************************************************/
	/* �鱦��Ч���鱦ͨ�������ӵķ�ʽ���ʵ��Ч����1����ʯ����ʯ�������front�㣬2��back�� 
	3�ǽ���ָ���� 4�Ǹ߹�� 5��note�㣬��󽫸��㰴һ���������ӣ����shader*/
	/************************************************************************/
	class M3D_API JewelEffect :public Effect
	{

	public:
		JewelEffect(RenderAction* action);
		//************************************
		// Method:    Render
		// FullName:  M3D::JewelEffect::Render
		// Access:    public 
		// Returns:   void
		// Qualifier: ��Ⱦ����
		//************************************
		void Render();
		//************************************
		// Method:    ClearResource
		// FullName:  M3D::JewelEffect::ClearResource
		// Access:    public 
		// Returns:   void
		// Qualifier: ������Դ
		//************************************
		void ClearResource();
		//************************************
		// Method:    SetSize
		// FullName:  M3D::JewelEffect::SetSize
		// Access:    public 
		// Returns:   void
		// Qualifier: ���ô�С
		// Parameter: int w
		// Parameter: int h
		//************************************
		void SetSize(int w, int h);
	private:
		//************************************
		// Method:    RenderJewelFront
		// FullName:  M3D::JewelEffect::RenderJewelFront
		// Access:    private 
		// Returns:   void
		// Qualifier: ��Ⱦ�鱦front��
		// Parameter: RenderQueue * RenderStateArray
		//************************************
		void RenderJewelFront( RenderQueue * RenderStateArray);
		//************************************
		// Method:    RenderJewelBack
		// FullName:  M3D::JewelEffect::RenderJewelBack
		// Access:    private 
		// Returns:   void
		// Qualifier: back��
		// Parameter: RenderQueue * RenderStateArray
		//************************************
		void RenderJewelBack( RenderQueue * RenderStateArray);
		//************************************
		// Method:    RenderRing
		// FullName:  M3D::JewelEffect::RenderRing
		// Access:    private 
		// Returns:   void
		// Qualifier: ����ָ����
		// Parameter: RenderQueue * RenderStateArray
		//************************************
		void RenderRing(RenderQueue* RenderStateArray);
		//************************************
		// Method:    DrawJewelType
		// FullName:  M3D::JewelEffect::DrawJewelType
		// Access:    private 
		// Returns:   void
		// Qualifier: �鱦���Ͳ�
		// Parameter: RenderQueue * RenderStateArray
		//************************************
		void DrawJewelType(RenderQueue* RenderStateArray);
		//************************************
		// Method:    DrawJewelHighLight
		// FullName:  M3D::JewelEffect::DrawJewelHighLight
		// Access:    private 
		// Returns:   void
		// Qualifier: �鱦�߹��
		// Parameter: RenderQueue * RenderStateArray
		//************************************
		void DrawJewelHighLight(RenderQueue* RenderStateArray);
		//************************************
		// Method:    DrawJadeBlendQuad
		// FullName:  M3D::JewelEffect::DrawJadeBlendQuad
		// Access:    private 
		// Returns:   void
		// Qualifier: �鱦��ϲ�
		//************************************
		void DrawJadeBlendQuad();
		//************************************
		// Method:    DrawJewelQuad
		// FullName:  M3D::JewelEffect::DrawJewelQuad
		// Access:    private 
		// Returns:   void
		// Qualifier: �鱦������Ⱦ���ľ���
		//************************************
		void DrawJewelQuad();
		void DrawJewelPassGroup(vector<RenderQueue*>& RenderStateArray);

		//************************************
		// Method:    DrawFrameBufferDebug
		// FullName:  M3D::JewelEffect::DrawFrameBufferDebug
		// Access:    private 
		// Returns:   void
		// Qualifier: debug����
		//************************************
		void DrawFrameBufferDebug();
	private:

	};
}

#endif