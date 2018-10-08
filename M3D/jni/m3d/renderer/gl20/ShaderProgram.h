/**@file
 *@brief
 *@author	CDJ
 *@date		2016-2-16
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */
#ifndef SHADERPROGRAM_H_
#define SHADERPROGRAM_H_

#include "m3d/renderer/gl20/Shader.h"
#include "m3d/base/StringHash.h"

#ifdef __MOBILE__
#ifdef __GNUC__
#include <ext/hash_map>
#else
#include <hash_map>
#endif
using namespace __gnu_cxx;

#elif _WIN32
#include <hash_map>
using namespace std;
using namespace stdext;

#endif

#include "m3d/base/Color.h"
#include "m3d/base/Matrix3.h"
#include "m3d/base/Matrix4.h"
#include "m3d/base/Matrix3x4.h"


namespace M3D
{
/**
 * shader材质枚举
 */
enum ShaderMaterialType
{
	SMT_MultiLight = 0,//!< SMT_MultiLight
	SMT_Brdf,          //!< SMT_Brdf
	SMT_DepthMap,      //!< SMT_DepthMap
	SMT_Image,         //!< SMT_Image
	SMT_FBODebug,      //!< SMT_FBODebug
	SMT_CubeMap,       //!< SMT_CubeMap
	SMT_BaseVertex,    //!< SMT_BaseVertex
	SMT_Background,    //!< SMT_Background
	SMT_Axis,          //!< SMT_Axis
	SMT_Edge,          //!< SMT_Edge
	SMT_PlaneShadow,   //!< SMT_PlaneShadow
	SMT_Blur,          //!< SMT_Blur
	SMT_Specular,      //!< SMT_Specular
	SMT_EdgeDetection,  //!< SMT_EdgeDetection
	SMT_Quad,
	SMT_MultilightPerFrag,
};

#ifdef __MOBILE__
typedef void (*GetActiveInfo)(GLuint program, GLuint index, GLsizei bufsize,
			GLsizei* length, GLint* size, GLenum* type, GLchar* name);
typedef GLint (*GetLocation)(GLuint program, const GLchar* name);

struct StrHash
{
	size_t operator()(const StringHash & strHash) const
	{
		return size_t(strHash.Value());
	}
};

#elif _WIN32
typedef void  (GLAPIENTRY *GetActiveInfo)(GLuint program, GLuint index, GLsizei bufsize,GLsizei* length, GLint* size, GLenum* type, GLchar* name);
typedef GLint (GLAPIENTRY *GetLocation)(GLuint program, const GLchar* name);

inline size_t CString_hash_value(const StringHash& strHash) 
{ 
	return size_t(strHash.Value());
}

class M3D_API StrHash : public hash_compare<StringHash> 
{ 
public: 
	size_t operator()(const StringHash& _Key) const 
	{ 
		return((size_t)CString_hash_value(_Key));
	}

	bool operator()(const StringHash& _Keyval1, const StringHash& _Keyval2) const 
	{ 
		return _Keyval1 <_Keyval2; 
	} 
};

#endif 

typedef hash_map<StringHash, ShaderParameter,StrHash > SPHashMap;


/**
 * @class
 * @brief shaderProgam类。用来生成着色器程序
 */
class M3D_API ShaderProgram:public GPUObject
{
public:
	ShaderProgram();
	ShaderProgram(Shader * vertexShader, Shader * fragShader);
	virtual ~ShaderProgram();
	/**
	 * @brief 链接shader
	 * @param shader
	 * @return
	 */
	bool AddShader(Shader * shader); //添加已有的着色器程序
	/**
	 * @brief 移除
	 * @param shader
	 * @return
	 */
	bool RemoveShader(Shader * shader);

	/**
	 *  @brief 删除着色器程序
	 */
	void DeleteShaderProgram();
	/**
	 *  @brief 停用着色器程序
	 */
	void ReleaseShaderProgram();
	/**
	 * @brief	获取着色器程序对象(ID)
	 * @return
	 */
	GLuint GetProgramId() const;
	/**
	 * @brief	获取顶点着色器
	 * @return
	 */
	Shader * GetVertexShader();
	/**
	 * @brief	获取片元着色器
	 * @return
	 */
	Shader * GetFragmentShader();
	/*以下是Uniform 向量赋值函数*/
	void SetUniformValue(GLint location, GLint value);


	void SetUniformValue(GLint location, GLfloat value);

	void SetUniformValue(GLint location, GLfloat x, GLfloat y);

	void SetUniformValue(GLint location, GLfloat x, GLfloat y, GLfloat z);

	void SetUniformValue(GLint location, GLfloat x, GLfloat y, GLfloat z,
		GLfloat w);

	void SetUniformValue(const StringHash& paraName,const GLfloat * v);
	void SetUniformValue(const StringHash& paraName,const GLint * v);

	void SetUniformValue(const StringHash& paraName, GLsizei count,
		const GLfloat * v);

	void SetUniformValue(const StringHash& paraName, GLsizei count,
		const GLint * v);
	/**
	 * 为矩阵赋值
	 * @param paraName
	 * @param count
	 * @param transpose
	 * @param v
	 */
	void SetUniformValue(const StringHash& paraName, GLsizei count,
		GLboolean transpose, const GLfloat * v);
	/**
	 * 为内部类型赋值
	 * @param paraName
	 * @param
	 */
	void SetUniformValue(const StringHash& paraName, const Color & color);
	void SetUniformValue(const StringHash& paraName, const Vector2 & vec2);
	void SetUniformValue(const StringHash& paraName, const Vector3 & vec3);
	void SetUniformValue(const StringHash& paraName, const Vector4 & vec4);
	void SetUniformValue(const StringHash& paraName, const Matrix3 & mat3);
	void SetUniformValue(const StringHash& paraName, const Matrix4 & mat4);
	void SetUniformValue(const StringHash& paraName, const Matrix3x4 & mat3x4);

	/**
	 * 获取uniform参数
	 * @param name
	 * @return
	 */
	ShaderParameter* GetShaderUniformParameter(const StringHash&  name);
	/**
	 * 获取attribute参数
	 * @param name
	 * @return
	 */
	ShaderParameter* GetShaderAttributeParameter(const StringHash&  name);
	/**
	 * @brief 	链接程序
	 * @return
	 */
	bool LinkProgram();
	/**
	 * @brief 	使用程序，内部初始化了uniform和attribute映射
	 */
	void UseProgram();
	/**
	 * @brief 	链接并使用
	 * @return
	 */
	bool LinkAndUse();

	/**
	 * @brief  获取attribute在shader中的地址
	 * @param name
	 * @return
	 */
	GLint GetAttributeLocation(string & name);

	/**
	 * @brief 	获取uniform在shader中的地址
	 * @param name
	 * @return
	 */
	GLint GetUniformLocation(string & name);
	/**
	 * @brief 	指定attribute在shader中的地址值，可以在连接之前也可以在连接之后指定，在连接之后指定要重新连接
	 * @param name
	 * @param location
	 */
	void BindAttributeLocation(string&  name, GLuint location);
	/**
	 * @brief 开启指定的shader中的顶点属性数组
	 * @param location
	 */
	void EnableAttributeArray(GLint location);

	/**
	 * 停止作用
	 * @param location
	 */
	void DisableAttributeArray(GLint location);

	/**
	 * @brief	解释顶点属性数组含义
	 * @param location
	 * @param tupleSize
	 * @param type
	 * @param stride
	 * @param values
	 */
	void SetVertexAttribPointer(int location, int tupleSize, GLenum type,
			int stride, const void* values);

	ShaderMaterialType GetType(){return m_type;}
	void SetType(ShaderMaterialType type){this->m_type = type;}

private:
	/**
	 * @brief 创建着色器程序
	 */
	void CreatShaderProgram();
	/**
	 * 标记
	 * @return
	 */
	bool IsDirty();
	void MakeDirty();

private:
	bool m_isLinked;//!<是否连接
	SPHashMap m_shaderUniformMap; //!<uniform映射
	SPHashMap m_shaderAttributeMap; //!<attribute映射
	Shader * m_vsh, *m_fsh;//!<vertex和fragment shader
	bool m_dirty;//!<标记位

	ShaderMaterialType m_type; //!<shader材质类型
};

} /* namespace M3D */
#endif /* SHADERPROGRAM_H_ */
