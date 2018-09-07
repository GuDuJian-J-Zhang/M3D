#ifndef EFFECT_H_
#define EFFECT_H_
#include "m3d/action/RenderAction.h"
#include "m3d/graphics/HardWareFrameBuffer.h"
#include "m3d/renderer/gl20/ShaderProgram.h"
namespace M3D
{
	/************************************************************************/
	/* effect类以及其派生类，用于处理一些后期特效，或者其他定义的效果，如outline，珠宝等，
	该effect会在材质文件中使用特定的flag标出。                                                                   */
	/************************************************************************/
	struct Uniform;
	class M3D_API Effect :Resource
	{
	public:
		Effect(RenderAction* action);
		~Effect();
		//************************************
		// Method:    Render
		// FullName:  M3D::Effect::Render
		// Access:    virtual public 
		// Returns:   void
		// Qualifier: 渲染函数
		//************************************
		virtual void Render();
		//************************************
		// Method:    ClearResource
		// FullName:  M3D::Effect::ClearResource
		// Access:    virtual public 
		// Returns:   void
		// Qualifier: 清理资源
		//************************************
		virtual void ClearResource();
		//************************************
		// Method:    SetSize
		// FullName:  M3D::Effect::SetSize
		// Access:    virtual public 
		// Returns:   void
		// Qualifier: 重置大小
		// Parameter: int w
		// Parameter: int h
		//************************************
		virtual void SetSize(int w, int h);
		//************************************
		// Method:    SetUniform
		// FullName:  M3D::Effect::SetUniform
		// Access:    public 
		// Returns:   void
		// Qualifier: 设置unform，可以用uniformhelp类中的替换，因为该函数首先写的，所以没有更改
		// Parameter: ShaderProgram * shaderEffect
		// Parameter: SPHashMap & shaderUniformMap
		// Parameter: map<string
		// Parameter: Uniform> & uniformValues
		//************************************
		void SetUniform(ShaderProgram * shaderEffect, SPHashMap& shaderUniformMap, map<string, Uniform>& uniformValues);
		//************************************
		// Method:    MergeUnifom
		// FullName:  M3D::Effect::MergeUnifom
		// Access:    public 
		// Returns:   void
		// Qualifier: 合并uniform
		// Parameter: map<string
		// Parameter: Uniform> & src
		// Parameter: map<string
		// Parameter: Uniform> & dist
		//************************************
		void MergeUnifom(map<string, Uniform>&src, map<string, Uniform>&dist);

	protected:
		//************************************
		// Method:    GetHardWareFrameBuffer
		// FullName:  M3D::Effect::GetHardWareFrameBuffer
		// Access:    protected 
		// Returns:   HardWareFrameBuffer&
		// Qualifier: 根据key获取fbo
		// Parameter: string key
		//************************************
		HardWareFrameBuffer& GetHardWareFrameBuffer(string key);
		map<string, HardWareFrameBuffer> m_hardWareFrameBufferMap;
		map<int, GLenum> textureBindingTargetMap;
		RenderAction * m_action;
	};
}
#endif