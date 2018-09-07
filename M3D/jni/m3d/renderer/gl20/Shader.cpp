/*
 * Shader.cpp
 *
 *  Created on: 2016-2-16
 *      Author: Administrator
 */

#include "m3d/renderer/gl20/Shader.h"
#include "Utility.h"


namespace M3D
{

Shader::Shader(GLenum type) :
		m_name(""), m_id(-1), m_isCompiled(false)
{

	m_type = type;

}

Shader::~Shader()
{

}

/**
 * @brief 	编译来自字符串的shader
 * @param 	sourceCode 源码
 * @param 	type	shader类型
 * @return	true成功 false失败
 */
bool Shader::CompileSourceCode(const char* sourceCode)
{
	bool ret = true;
	GLenum type = m_type;
	GLuint shader = glCreateShader(type);
	LOGI("shader id is %d",shader);

	if (shader)
	{
		LOGI("Begin compile source ");
		glShaderSource(shader, 1, &sourceCode, NULL);
		glCompileShader(shader);
		m_isCompiled = true;
		GLint compiled = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
		if (!compiled)
		{
			LOGI("Failed to compile!");
			ret = false;
			GLint infoLen = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
			if (infoLen)
			{
				char* buf = (char*) malloc(infoLen);
				if (buf)
				{
					glGetShaderInfoLog(shader, infoLen, NULL, buf);
					LOGI("Could not compile shader %d:\n%s\n", type, buf);
					LOGI("%s",sourceCode);
					free(buf);
				}
				glDeleteShader(shader);
				shader = 0;
			}

			m_isCompiled = false;
		}
	}
	m_id = shader;
	return ret;
}

/**
 * @brief 			编译来自文件的shader
 * @param fileName 	文件路径
 * @param type		着色器类型
 * @return 			true成功 false失败
 */
bool Shader::CompileSourceFile(string fileName)
{
	bool ret = false;
	FILE * pFile;
	long length = 0;
	char * pBuffer = NULL;
	pFile = fopen(fileName.c_str(), "r");

	fseek(pFile, 0L, SEEK_END);

	length = ftell(pFile);
	rewind(pFile);
	pBuffer = (char *) malloc((length + 1) * sizeof(char));
	if (pBuffer)
		LOGI("Memory get %d,length is %d", sizeof(pBuffer), length);

	memset(pBuffer, '\0', length + 1);
	fread(pBuffer, sizeof(char), length + 1, pFile);
	LOGI("code is %s", pBuffer);
	bool flag;
	flag = CompileSourceCode(pBuffer);

	fclose(pFile);

	free(pBuffer);
	if (flag)
	{
		ret = true;
	}
	return ret;
}

/**
 * @brief 是否编译成功
 * @return true成功 false失败
 */
bool Shader::IsCompiled() const
{
	return m_isCompiled;
}

/**
 * @brief	返回shader对象ID
 * @return
 */
GLuint Shader::ShaderId() const
{
	GLuint id = -1;
	if (IsCompiled())
	{
		id = m_id;
	}
	return id;
}

/**
 * @brief	删除shader
 */
void Shader::DeleteShader()
{
	if (m_isCompiled)
	{
		glDeleteShader(m_id);
	}
}

string Shader::GetShaderName()
{
	return m_name;
}

void Shader::SetShaderName(string name)
{
	 m_name = name;
}

GLenum Shader::GetShaderType()
{
	return m_type;
}


GLuint Shader::GetShaderProgramId()
{
	return m_programId;
}

void Shader::SetShaderProgramId(GLuint id)
{
	this->m_programId = id;
}
}




 /* namespace M3D */
