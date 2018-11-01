#ifndef  JEWELEFFECT_H_
#define  JEWELEFFECT_H_
#include "m3d/renderer/effect/Effect.h"
#include "m3d/graphics/HardWareFrameBuffer.h"
namespace M3D
{	
	/************************************************************************/
	/* 珠宝特效。珠宝通过多层叠加的方式最后实现效果。1是玉石、钻石、珍珠的front层，2是back层
	3是金属指环层 4是高光层 5是note层，最后将各层按一定方法叠加，详见shader*/
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
		// Qualifier: 渲染函数
		//************************************
		void Render();
		//************************************
		// Method:    ClearResource
		// FullName:  M3D::JewelEffect::ClearResource
		// Access:    public 
		// Returns:   void
		// Qualifier: 清理资源
		//************************************
		void ClearResource();
		//************************************
		// Method:    SetSize
		// FullName:  M3D::JewelEffect::SetSize
		// Access:    public 
		// Returns:   void
		// Qualifier: 设置大小
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
		// Qualifier: 渲染钻石front层
		// Parameter: RenderQueue * RenderStateArray
		//************************************
		void RenderDiamondFront( RenderQueue * RenderStateArray);
		//************************************
		// Method:    RenderJewelBack
		// FullName:  M3D::JewelEffect::RenderJewelBack
		// Access:    private 
		// Returns:   void
		// Qualifier: 钻石back层
		// Parameter: RenderQueue * RenderStateArray
		//************************************
		void RenderDiamondBack( RenderQueue * RenderStateArray);

		//************************************
		// Method:    RenderDimodFront
		// FullName:  M3D::JewelEffect::RenderDimodFront
		// Access:    private
		// Returns:   void
		// Qualifier: 渲染珠宝front层
		// Parameter: RenderQueue * RenderStateArray
		//************************************
		void RenderJewelFront( RenderQueue * RenderStateArray);
		//************************************
		// Method:    RenderDimodBack
		// FullName:  M3D::JewelEffect::RenderDimodBack
		// Access:    private
		// Returns:   void
		// Qualifier: back层
		// Parameter: RenderQueue * RenderStateArray
		//************************************
		void RenderJewelBack( RenderQueue * RenderStateArray);
		//************************************
		// Method:    RenderJewelBack
		// FullName:  M3D::JewelEffect::RenderJewelBack
		// Access:    private
		// Returns:   void
		// Qualifier: FrontInfo层
		// Parameter: RenderQueue * RenderStateArray
		//************************************
		void RenderJewelFrontInfo( RenderQueue * RenderStateArray);
		//************************************
		// Method:    RenderRing
		// FullName:  M3D::JewelEffect::RenderRing
		// Access:    private 
		// Returns:   void
		// Qualifier: 金属指环层
		// Parameter: RenderQueue * RenderStateArray
		//************************************
		void RenderRing(RenderQueue* RenderStateArray);
		//************************************
		// Method:    DrawJewelType
		// FullName:  M3D::JewelEffect::DrawJewelType
		// Access:    private 
		// Returns:   void
		// Qualifier: 珠宝类型层
		// Parameter: RenderQueue * RenderStateArray
		//************************************
		void DrawJewelType(RenderQueue* RenderStateArray);
		//************************************
		// Method:    DrawJewelHighLight
		// FullName:  M3D::JewelEffect::DrawJewelHighLight
		// Access:    private 
		// Returns:   void
		// Qualifier: 珠宝高光层
		// Parameter: RenderQueue * RenderStateArray
		//************************************
		void DrawJewelHighLight(RenderQueue* RenderStateArray);
		//************************************
		// Method:    DrawJadeBlendQuad
		// FullName:  M3D::JewelEffect::DrawJadeBlendQuad
		// Access:    private 
		// Returns:   void
		// Qualifier: 珠宝混合层
		//************************************
		void DrawJadeBlendQuad();
		//************************************
			// Method:    DrawJadeBlendQuad
			// FullName:  M3D::JewelEffect::DrawJadeBlendQuad
			// Access:    private
			// Returns:   void
			// Qualifier: 钻石混合层
			//************************************
		void DrawDiamondBlendQuad();
		//************************************
		// Method:    DrawJewelQuad
		// FullName:  M3D::JewelEffect::DrawJewelQuad
		// Access:    private 
		// Returns:   void
		// Qualifier: 珠宝最终渲染到的矩形
		//************************************
		void DrawJewelQuad();
		void DrawJewelPassGroup(vector<RenderQueue*>& RenderStateArray);

		//************************************
		// Method:    DrawFrameBufferDebug
		// FullName:  M3D::JewelEffect::DrawFrameBufferDebug
		// Access:    private 
		// Returns:   void
		// Qualifier: debug测试
		//************************************
		void DrawFrameBufferDebug();
	private:
		bool  isDiamond ;

	};
}

#endif
