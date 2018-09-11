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

	HardWareRenderBuffer::HardWareRenderBuffer(GLenum internalFormat, int width, int height) :
		GPUObject(), GLRenderTarget()
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
			if (this->m_resourceMgr)
			{
				this->m_resourceMgr->AddGLObject(m_object, ResourceManager::RBO);
			}
		}
	}

	/**
	 * 创建RBO
	 */
	void HardWareRenderBuffer::Create(bool useMS, int sample)
	{
#ifdef WIN32
		glGenRenderbuffersEXT(1, &m_object);
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, m_object);
		if (useMS)
		{
			glRenderbufferStorageMultisampleEXT(GL_RENDERBUFFER_EXT, sample, m_internalFormat, m_width, m_height);
		}
		else
		{
			glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, m_internalFormat, m_width, m_height);
		}
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
#else
		glGenRenderbuffers(1, &m_object);
		glBindRenderbuffer(GL_RENDERBUFFER, m_object);
		if (useMS)
		{
			glRenderbufferStorage(GL_RENDERBUFFER, m_internalFormat, m_width, m_height);
		}
		else
		{
			glRenderbufferStorage(GL_RENDERBUFFER, m_internalFormat, m_width, m_height);
		}
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
#endif


	}
	/**
	 * 绑定RBO
	 */
	void HardWareRenderBuffer::Bind()
	{
#ifdef WIN32
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, m_object);
#else
		glBindRenderbuffer(GL_RENDERBUFFER, m_object);
#endif

	}
	/**
	 * 解绑RBO
	 */
	void HardWareRenderBuffer::Unbind()
	{
#ifdef WIN32
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
#else
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
#endif
	}

} /* namespace M3D */
