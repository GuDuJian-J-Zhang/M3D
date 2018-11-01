/*
 * ShaderProgram.cpp
 *
 *  Created on: 2016-2-16
 *      Author: Administrator
 */

#include "m3d/renderer/gl20/ShaderProgram.h"
#include "m3d/graphics/GraphicsDefs.h"
#include "m3d/renderer/RenderContext.h"
#include "m3d/ResourceManager.h"
namespace M3D
{
/**
 * @brief 构造函数,一定要链接着色器
 */
ShaderProgram::ShaderProgram():m_vsh(NULL),m_fsh(NULL),m_isLinked(false),m_dirty(false)
{
	m_object = 0;
	CreatShaderProgram();
	m_type = (ShaderMaterialType)0;
}
/**
 * @brief 内部实现了着色器链接
 * @param vertexShader
 * @param fragShader
 */
ShaderProgram::ShaderProgram(Shader * vertexShader,Shader * fragShader)
{
	m_object = 0;
	CreatShaderProgram();
	m_vsh = vertexShader;
	m_fsh = fragShader;
	AddShader(m_vsh);
	AddShader(m_fsh);

}

bool ShaderProgram::IsDirty()
{
	return m_dirty;
}
void ShaderProgram::MakeDirty()
{
	m_dirty = true;
}

/**
 * @brief 析构函数
 */
ShaderProgram::~ShaderProgram()
{
	m_shaderUniformMap.clear();
	m_shaderAttributeMap.clear();
	//保证析构在GL线程中进行
	if (this->m_resourceMgr)
	{
		//在析构时，将material从资源管理器中移除。此对象真正点不存在了

		if (m_object > 0)
		{
			m_resourceMgr->AddGLObject(m_object,
				ResourceManager::SHADER_PROGRAM);
			m_object = 0;
		}
		this->m_resourceMgr = NULL;
	}
}

/**
 * @brief 链接shader
 * @param shader
 * @return
 */
bool ShaderProgram::AddShader(Shader* shader)
{
	bool ret = false;
	if (!shader->IsCompiled())
	{
		ret = false;
	}
	if (m_object)
	{
		glAttachShader(m_object, shader->m_id);

		if(GL_VERTEX_SHADER==shader->GetShaderType())
		{
			m_vsh = shader;
		}
		else if(GL_FRAGMENT_SHADER == shader->GetShaderType())
		{
			m_fsh = shader;
		}
		ret = true;
	}
	return ret;
}

/**
 * @brief
 * @param shader
 * @return
 */
bool ShaderProgram::RemoveShader(Shader* shader)
{
	bool ret = false;

	if(shader->GetShaderProgramId() == m_object)
	{
		glDetachShader(m_object, shader->m_id);
		ret = true;
	}
	return ret;
}

/**
 * @brief 创建着色器程序
 */
void ShaderProgram::CreatShaderProgram()
{
	m_object = glCreateProgram();
	////LOGI("program id is %d", m_object);
}

/**
 *  @brief 删除着色器程序
 */
void ShaderProgram::DeleteShaderProgram()
{
	m_isLinked = false;
	glDeleteProgram(m_object);
}

/**
 *  @brief 停用着色器程序
 */
void ShaderProgram::ReleaseShaderProgram()
{
	glUseProgram(0);
}

/**
 * @brief	获取着色器程序对象(ID)
 * @return
 */
GLuint ShaderProgram::GetProgramId() const
{
	return m_object;
}

/**
 * @brief 	链接程序
 * @return
 */
bool ShaderProgram::LinkProgram()
{
	////LOGI("program link , program id is %d programId", m_object);
	if(!m_vsh && !m_fsh && !m_vsh->IsCompiled()&&!m_fsh->IsCompiled())
	{
		////LOGI("Shader is NULL");
		m_isLinked = false;
		return m_isLinked;
	}
	glLinkProgram(m_object);
	M3D_GL_ERROR_CHECK
	GLint linkStatus,length;
	////LOGI("program link END");
	glGetProgramiv(m_object,GL_LINK_STATUS,&linkStatus);
	M3D_GL_ERROR_CHECK
    if (!linkStatus)
    {
        glGetProgramiv(m_object, GL_INFO_LOG_LENGTH, &length);
		M3D_GL_ERROR_CHECK
        char *msg = new char[length];
        int outLength;
        glGetProgramInfoLog(m_object, length, &outLength, &msg[0]);
		M3D_GL_ERROR_CHECK
        glDeleteProgram(m_object);
		M3D_GL_ERROR_CHECK
        m_object = 0;
        LOGE("Could not Linke shader :\n%s\n",  msg);
        m_isLinked = GL_FALSE;
        delete [] msg;
    }
    else
    {
		m_isLinked = GL_TRUE;
		MakeDirty();
    }

    //链接完成后就可以删除shader了
    m_vsh->DeleteShader();
    m_fsh->DeleteShader();
    ////LOGI("LinkProgram() END");
	return m_isLinked;
}

/**
 * @brief 	使用程序，内部初始化了uniform和attribute映射
 */
void ShaderProgram::UseProgram()
{
	if(!IsDirty())
	{
		glUseProgram(m_object);
		M3D_GL_ERROR_CHECK
		return;
	}
	const int MAX_PARAMETER_NAME_LENGTH = 256;
	char valueName[MAX_PARAMETER_NAME_LENGTH];
	GLint valueCount = 0;

	GetActiveInfo InfoFoo[2];
	GetLocation LocaFoo[2];
	GLenum ActiveType[2];

	InfoFoo[0] = glGetActiveUniform;
	InfoFoo[1] = glGetActiveAttrib;
	LocaFoo[0] = glGetUniformLocation;
	LocaFoo[1] = glGetAttribLocation;

	ActiveType[0] = GL_ACTIVE_UNIFORMS;
	ActiveType[1] = GL_ACTIVE_ATTRIBUTES;
//	//LOGI("1");
	m_shaderUniformMap.clear();
	m_shaderAttributeMap.clear();
 	LOGD("2");
	if (m_isLinked)
	{
		glUseProgram(m_object);
		M3D_GL_ERROR_CHECK
		for (int j = 0; j < 2; j++)
		{
			LOGD("j ====%d",j);
			glGetProgramiv(m_object, ActiveType[j], &valueCount); //获取OpenGL优化后的uniform数量
			M3D_GL_ERROR_CHECK
			LOGD("value Count ====%d",valueCount);
			for (int i = 0; i < valueCount; i++)
			{
				GLenum type = GL_FLOAT;
				int count;
				//LOGD("21s");
				InfoFoo[j](m_object, (GLuint) i,
						MAX_PARAMETER_NAME_LENGTH, 0, &count, &type, valueName);
				M3D_GL_ERROR_CHECK
				LOGD("m_TYPE == %x",type);
				string tempName(valueName);
				//为什么要去除"[0]",参考http://www.gamedev.net/topic/666216-glgetactiveuniform-causes-access-violation/
				////LOGI("origin name = %s",valueName);
				int subStrIndex = tempName.find("[0]");
				int subCharIndex = tempName.find('.');
				if (subStrIndex != string::npos)
				{
					if(subCharIndex == string::npos)
					{
						tempName = tempName.substr(0, subStrIndex);
					}
				}
				LOGD("new name = %s",tempName.c_str());
				////LOGI("22s");
				int location = LocaFoo[j](m_object, tempName.c_str());
				M3D_GL_ERROR_CHECK
				////LOGI("location == %d",location);
				////LOGI("22e");
				ShaderParameter newPara(location, type, tempName,count);
				string strHashName(tempName);
				if (0 == j)
				{
					m_shaderUniformMap.insert(
							std::make_pair(strHashName, newPara));
				}
				else
				{
					m_shaderAttributeMap.insert(
							std::make_pair(strHashName, newPara));
				}
			}

		}
		m_dirty = false;
	}

}
/**
 * @brief 	链接并使用
 * @return
 */
bool ShaderProgram::LinkAndUse()
{
	bool ret = false;
	if (LinkProgram())
	{
		UseProgram();
		ret = true;
	}
	return ret;
}

/**
 * @brief  获取attribute在shader中的地址
 * @param name
 * @return
 */
GLint ShaderProgram::GetAttributeLocation(string & name)
{
	return glGetAttribLocation(m_object, name.c_str());
}

/**
 * @brief 	获取uniform在shader中的地址
 * @param name
 * @return
 */
GLint ShaderProgram::GetUniformLocation(string & name)
{
	return glGetUniformLocation(m_object, name.c_str());
}

/**
 * @brief 	指定attribute在shader中的地址值，可以在连接之前也可以在连接之后指定，在连接之后指定要重新连接
 * @param name
 * @param location
 */
void ShaderProgram::BindAttributeLocation(string & name, GLuint location)
{
	glBindAttribLocation(m_object,location,name.c_str());
	M3D_GL_ERROR_CHECK
}


/**
 * @brief 	给Uniform赋值
 * @param location
 * @param value
 */
void ShaderProgram::SetUniformValue(GLint location, GLfloat value)
{
	glUniform1f(location,value);
	M3D_GL_ERROR_CHECK
}
void ShaderProgram::SetUniformValue(const string& paraName, GLfloat value)
{
	ShaderParameter * para = GetShaderUniformParameter(paraName);
	if (para)
	{
		glUniform1f(para->m_location, value);
		M3D_GL_ERROR_CHECK
	}
	
}

void ShaderProgram::SetUniformValue(const string & paraName, GLint value)
{
	ShaderParameter * para = GetShaderUniformParameter(paraName);
	if (para)
	{
		glUniform1i(para->m_location, value);
		M3D_GL_ERROR_CHECK
	}
}

void ShaderProgram::SetUniformValue(GLint location, GLfloat x, GLfloat y)
{
	glUniform2f(location, x, y);
	M3D_GL_ERROR_CHECK
}

void ShaderProgram::SetUniformValue(GLint location, GLfloat x, GLfloat y,
		GLfloat z)
{
	glUniform3f(location, x, y, z);
	M3D_GL_ERROR_CHECK
}

void ShaderProgram::SetUniformValue(GLint location, GLfloat x, GLfloat y,
		GLfloat z, GLfloat w)
{
	glUniform4f(location, x, y, z,w);
	M3D_GL_ERROR_CHECK
}

void ShaderProgram::SetUniformValue(const string& paraName, GLsizei count,
		GLboolean transpose, const GLfloat* v)
{
	if (m_object)
	{
		ShaderParameter * para = GetShaderUniformParameter(paraName);
		if (para)
		{
			switch (para->m_Type)
			{
			case GL_FLOAT_MAT2:
				glUniformMatrix2fv(para->m_location, count, transpose, v);
				break;
			case GL_FLOAT_MAT3:
				glUniformMatrix3fv(para->m_location, count, transpose, v);
				break;
			case GL_FLOAT_MAT4:
				glUniformMatrix4fv(para->m_location, count, transpose, v);
				break;
			}
			M3D_GL_ERROR_CHECK
		}
	}
}

void ShaderProgram::SetUniformValue(GLint location, GLint value)
{
	glUniform1i(location, value);
	M3D_GL_ERROR_CHECK
}

void ShaderProgram::SetUniformValue(const string& paraName, const Color& color)
{
	SetUniformValue(paraName,1,color.Data());
	//ShaderParameter * para = GetShaderUniformParameter(paraName);
	//if (para)
	//{
	//	glUniform4f(para->m_location, color.m_r, color.m_g, color.m_b,color.m_a);
	//}
}

void ShaderProgram::SetUniformValue(const string& paraName, const Vector2& vec2)
{
	//SetUniformValue(paraName,1,vec2.Data());
	ShaderParameter * para = GetShaderUniformParameter(paraName);
	if (para)
	{
		glUniform2f(para->m_location, vec2.m_x,vec2.m_y);
		M3D_GL_ERROR_CHECK
	}
}

void ShaderProgram::SetUniformValue(const string& paraName, const Vector3& vec3)
{
	//SetUniformValue(paraName,1,vec3.Data());
	ShaderParameter * para = GetShaderUniformParameter(paraName);
	if (para)
	{
		glUniform3f(para->m_location, vec3.m_x, vec3.m_y,vec3.m_z);
		M3D_GL_ERROR_CHECK
	}
}

void ShaderProgram::SetUniformValue(const string& paraName, const Vector4& vec4)
{
	//SetUniformValue(paraName,1,vec4.Data());
	ShaderParameter * para = GetShaderUniformParameter(paraName);
	if (para)
	{
		glUniform4f(para->m_location, vec4.m_x, vec4.m_y, vec4.m_z,vec4.m_w	);
		M3D_GL_ERROR_CHECK
	}
}

void ShaderProgram::SetUniformValue(const string& paraName, GLsizei count, const Vector4* vec4)
{
	vector<float> vecValue;
	for (int i = 0; i < count; i++)
	{
		vecValue.push_back(vec4[i].m_x);
		vecValue.push_back(vec4[i].m_y);
		vecValue.push_back(vec4[i].m_z);
		vecValue.push_back(vec4[i].m_w);
	}
	SetUniformValue(paraName, count, vecValue.data());
}

void ShaderProgram::SetUniformValue(const string& paraName, const Matrix3& mat3)
{
	SetUniformValue(paraName,1,false,mat3.Data());
}

void ShaderProgram::SetUniformValue(const string& paraName, const Matrix4& mat4)
{
	SetUniformValue(paraName,1,false,mat4.Data());
}

void ShaderProgram::SetUniformValue(const string& paraName, const Matrix3x4& mat3x4)
{
	if (m_object)
	{
		ShaderParameter * para = GetShaderUniformParameter(paraName);
		if (para)
		{
			// Expand to a full Matrix4
			static Matrix4 fullMatrix;
			fullMatrix.m_m00 = mat3x4.m_m00;
			fullMatrix.m_m01 = mat3x4.m_m01;
			fullMatrix.m_m02 = mat3x4.m_m02;
			fullMatrix.m_m03 = mat3x4.m_m03;
			fullMatrix.m_m10 = mat3x4.m_m10;
			fullMatrix.m_m11 = mat3x4.m_m11;
			fullMatrix.m_m12 = mat3x4.m_m12;
			fullMatrix.m_m13 = mat3x4.m_m13;
			fullMatrix.m_m20 = mat3x4.m_m20;
			fullMatrix.m_m21 = mat3x4.m_m21;
			fullMatrix.m_m22 = mat3x4.m_m22;
			fullMatrix.m_m23 = mat3x4.m_m23;
			SetUniformValue(paraName, fullMatrix);
		}

	}
}

void ShaderProgram::SetUniformValue(const string& paraName, GLsizei count,
		const GLfloat* v)
{
	if(m_object)
	{
		ShaderParameter * para = GetShaderUniformParameter(paraName);
		if(para)
		{
			switch(para->m_Type)
			{
		      case GL_FLOAT:
		        glUniform1fv(para->m_location,count,v);
		        break;
		      case GL_FLOAT_VEC2:
		        glUniform2fv(para->m_location,count,v);
		        break;
		      case GL_FLOAT_VEC3:
		        glUniform3fv(para->m_location,count,v);
		        break;
		      case GL_FLOAT_VEC4:
		        glUniform4fv(para->m_location,count,v);
		        break;
		      default:
		        break;
			}
			M3D_GL_ERROR_CHECK
		}
	}
}

void ShaderProgram::SetUniformValue(const string& paraName,const GLfloat * v)
{
	if(m_object)
	{
		ShaderParameter * para = GetShaderUniformParameter(paraName);
//		//LOGI("para is %p",para);
		if(para)
		{
			switch(para->m_Type)
			{
		      case GL_FLOAT:
		        glUniform1f(para->m_location,v[0]);
		        ////LOGI("para->m_location is %d, value is %f",para->m_location,v[0]);
		        break;
		      case GL_FLOAT_VEC2:
		        glUniform2f(para->m_location,v[0],v[1]);
//		        //LOGI("para->m_location is %d, value is %f",para->m_location,v[0],v[1]);
		        break;
		      case GL_FLOAT_VEC3:
		        glUniform3f(para->m_location,v[0],v[1],v[2]);
//		        //LOGI("para->m_location is %d, value is %f",para->m_location,v[0],v[1],v[2]);
		        break;
		      case GL_FLOAT_VEC4:
		        glUniform4f(para->m_location,v[0],v[1],v[2],v[3]);
//		        //LOGI("para->m_location is %d, value is %f",para->m_location,v[0],v[1],v[2],v[3]);
		        break;
		      default:
		        break;
			}
			M3D_GL_ERROR_CHECK
		}
	}
}


void ShaderProgram::SetUniformValue(const string& paraName, const GLint* v)
{

	if(m_object)
	{
		ShaderParameter * para = GetShaderUniformParameter(paraName);
//		//LOGI("para is %p",para);
		if(para)
		{
			switch(para->m_Type)
			{
		      case GL_INT:
			  case GL_BOOL:
		        glUniform1i(para->m_location,v[0]);
		        ////LOGI("GLint para->m_location is %d, value is %d",para->m_location,v[0]);
		        break;
		      case GL_INT_VEC2:
			  case GL_BOOL_VEC2:
		        glUniform2i(para->m_location,v[0],v[1]);
//		        //LOGI("para->m_location is %d, value is %f",para->m_location,v[0],v[1]);
		        break;
		      case GL_INT_VEC3:
			  case GL_BOOL_VEC3:
		        glUniform3i(para->m_location,v[0],v[1],v[2]);
//		        //LOGI("para->m_location is %d, value is %f",para->m_location,v[0],v[1],v[2]);
		        break;
		      case GL_INT_VEC4:
			  case GL_BOOL_VEC4:
		        glUniform4i(para->m_location,v[0],v[1],v[2],v[3]);
//		        //LOGI("para->m_location is %d, value is %f",para->m_location,v[0],v[1],v[2],v[3]);
		        break;
		      default:
		        break;
			}
			M3D_GL_ERROR_CHECK
		}
	}

}

void ShaderProgram::SetUniformValue(const string& paraName, GLsizei count,
		const GLint * v)
{
	if(m_object)
	{
		ShaderParameter * para = GetShaderUniformParameter(paraName);
		if(para)
		{
			switch(para->m_Type)
			{
			case GL_SAMPLER_2D:
		      case GL_INT:
			  case GL_BOOL:
		        glUniform1iv(para->m_location,count,v);
		        break;
		      case GL_INT_VEC2:
			  case GL_BOOL_VEC2:
		        glUniform2iv(para->m_location,count,v);
		        break;
		      case GL_INT_VEC3:
			  case GL_BOOL_VEC3:
		        glUniform3iv(para->m_location,count,v);
		        break;
		      case GL_INT_VEC4:
			  case GL_BOOL_VEC4:
		        glUniform4iv(para->m_location,count,v);
		        break;
		      default:
		        break;
			}
			M3D_GL_ERROR_CHECK
		}
	}
}

/**
 * @brief 开启指定的shader中的顶点属性数组
 * @param location
 */
void ShaderProgram::EnableAttributeArray(GLint location)
{
	glEnableVertexAttribArray(location);
	M3D_GL_ERROR_CHECK
}

void ShaderProgram::DisableAttributeArray(GLint location)
{
	glDisableVertexAttribArray(location);
	M3D_GL_ERROR_CHECK
}


/**
 * @brief	解释顶点属性数组含义
 * @param location
 * @param tupleSize 取几个数据
 * @param type
 * @param stride
 * @param values
 */
void ShaderProgram::SetVertexAttribPointer(int location, int tupleSize, GLenum type, int stride,
		const void* values)
{
	glVertexAttribPointer(location,tupleSize,type,GL_FALSE,stride,values);
	M3D_GL_ERROR_CHECK
}

 SPHashMap & ShaderProgram::GetShaderUniformMap()
 {
	 return m_shaderUniformMap;
 }

/**
 * @brief	获取顶点着色器
 * @return
 */
Shader* ShaderProgram::GetVertexShader()
{
	return m_vsh;
}

/**
 * @brief	获取片元着色器
 * @return
 */
Shader* ShaderProgram::GetFragmentShader()
{
	return m_fsh;
}

/**
 * @brief	获取uniform名字对应的shaderParameter
 * @param name
 * @return
 */
ShaderParameter* ShaderProgram::GetShaderUniformParameter(const string& name)
{
	SPHashMap::iterator it =m_shaderUniformMap.find(name);
	if(it != m_shaderUniformMap.end())
	{
		return &it->second;
	}
	else
	{
		return NULL;
	}
}

ShaderParameter* ShaderProgram::GetShaderAttributeParameter(const string& name)
{

	SPHashMap::iterator it =m_shaderAttributeMap.find(name);
	if(it != m_shaderAttributeMap.end())
	{
		return &it->second;
	}
	else
	{
		return NULL;
	}
}




}
/* namespace M3D */

