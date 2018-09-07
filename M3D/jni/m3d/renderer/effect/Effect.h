#ifndef EFFECT_H_
#define EFFECT_H_
#include "m3d/action/RenderAction.h"
#include "m3d/graphics/HardWareFrameBuffer.h"
#include "m3d/renderer/gl20/ShaderProgram.h"
namespace M3D
{
	/************************************************************************/
	/* effect���Լ��������࣬���ڴ���һЩ������Ч���������������Ч������outline���鱦�ȣ�
	��effect���ڲ����ļ���ʹ���ض���flag�����                                                                   */
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
		// Qualifier: ��Ⱦ����
		//************************************
		virtual void Render();
		//************************************
		// Method:    ClearResource
		// FullName:  M3D::Effect::ClearResource
		// Access:    virtual public 
		// Returns:   void
		// Qualifier: ������Դ
		//************************************
		virtual void ClearResource();
		//************************************
		// Method:    SetSize
		// FullName:  M3D::Effect::SetSize
		// Access:    virtual public 
		// Returns:   void
		// Qualifier: ���ô�С
		// Parameter: int w
		// Parameter: int h
		//************************************
		virtual void SetSize(int w, int h);
		//************************************
		// Method:    SetUniform
		// FullName:  M3D::Effect::SetUniform
		// Access:    public 
		// Returns:   void
		// Qualifier: ����unform��������uniformhelp���е��滻����Ϊ�ú�������д�ģ�����û�и���
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
		// Qualifier: �ϲ�uniform
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
		// Qualifier: ����key��ȡfbo
		// Parameter: string key
		//************************************
		HardWareFrameBuffer& GetHardWareFrameBuffer(string key);
		map<string, HardWareFrameBuffer> m_hardWareFrameBufferMap;
		map<int, GLenum> textureBindingTargetMap;
		RenderAction * m_action;
	};
}
#endif