/**@file
 *@brief
 *@author	CDJ
 *@date		2016-2-16
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */
#ifndef M3D_SHADER_H_
#define M3D_SHADER_H_
#include "m3d/M3DExport.h"
#include "m3d/renderer/gl20/ShaderParameter.h"
#include "m3d/renderer/GLESHead.h"
#include "m3d/graphics/GPUObject.h"
namespace M3D
{
class ShaderProgram;
class GPUObject;
class M3D_API Shader:public GPUObject
{
public:
	Shader(GLenum type);
	virtual ~Shader();
	/**
	 * 从字符串编译shader
	 * @param sourceCode
	 * @return
	 */
	bool CompileSourceCode(const char * sourceCode);
	/**
	 * 从文件编译shader
	 * @param fileName
	 * @return
	 */
	bool CompileSourceFile(string fileName);
	/**
	 * 是否编译成功
	 * @return
	 */
	bool IsCompiled() const;
	/**
	 * shader句柄
	 * @return
	 */
	GLuint ShaderId() const;
	/**
	 * 删除shader
	 */
	void DeleteShader();
	/**
	 * 获取shader名字
	 * @return
	 */
	string GetShaderName();
	/**
	 * 设置shader名字
	 * @param name
	 */
	void SetShaderName(string name);

	/**
	 * 获取shade类型
	 * @return
	 */
	GLenum GetShaderType();

	/**
	 * 获取着色器程序ID
	 * @return
	 */
	GLuint GetShaderProgramId();
	/**
	 * 设置着色器程序ID
	 * @param id
	 */
	void SetShaderProgramId(GLuint id);

private:
	friend class ShaderProgram;//!<友元类
	bool m_isCompiled;//!<是否编译
	GLuint m_id;//!<ID
	string m_name;//!<名字
	GLenum m_type;//!<类型
	GLuint m_programId;//!<program ID
};

} /* namespace M3D */
#endif /* M3D_SHADER_H_ */
