/*
 * ShaderParameter.cpp
 *
 *  Created on: 2016-2-16
 *      Author: Administrator
 */

#include "m3d/renderer/gl20/ShaderParameter.h"

namespace M3D
{

ShaderParameter::ShaderParameter()
{
	// TODO Auto-generated constructor stub
	this->m_Type = GL_FLOAT;
}

ShaderParameter::ShaderParameter(GLint location, GLenum type,
		string name,int size)
{
	this->m_Type = type;
	this->m_location = location;
	this->m_name = name;
	this->size = size;
}

ShaderParameter::~ShaderParameter()
{
	// TODO Auto-generated destructor stub
}

} /* namespace M3D */
