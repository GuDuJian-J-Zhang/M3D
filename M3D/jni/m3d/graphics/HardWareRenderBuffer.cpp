/*
 * HardWareRenderBuffer.cpp
 *
 *  Created on: 2016-4-7
 *      Author: CDJ
 */

#include "m3d/graphics/HardWareRenderBuffer.h"
#include "m3d/renderer/gl20/GLRenderTarget.h"
#include "m3d/ResourceManager.h"
namespace M3D
{

HardWareRenderBuffer::HardWareRenderBuffer(GLenum internalFormat,int width ,int height ):
		GPUObject(),GLRenderTarget()
{

	m_targetType = RBO_TARGET;
	m_internalFormat = internalFormat;
	m_width = width;
	m_height = height;

}

HardWareRenderBuffer::~HardWareRenderBuffer()
{
	if (m_object)
	{
//		glDeleteBuffers(1, &m_object);
		if(this->m_resourceMgr)
		{
			this->m_resourceMgr->AddGLObject(m_object,ResourceManager::RBO);
		}
	}
}

/**
 * 创建RBO
 */
void HardWareRenderBuffer::Create(bool useMS, int sample)
{
	glGenRenderbuffers(1, &m_object);
	glBindRenderbuffer(GL_RENDERBUFFER, m_object);
	if (useMS)
	{
#ifdef _WIN32
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, sample, m_internalFormat, m_width, m_height);
#endif
	}
	else
	{
		glRenderbufferStorage(GL_RENDERBUFFER, m_internalFormat, m_width, m_height);
	}
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}
/**
 * 绑定RBO
 */
void HardWareRenderBuffer::Bind()
{
	glBindRenderbuffer(GL_RENDERBUFFER, m_object);
}
/**
 * 解绑RBO
 */
void HardWareRenderBuffer::Unbind()
{
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

} /* namespace M3D */
