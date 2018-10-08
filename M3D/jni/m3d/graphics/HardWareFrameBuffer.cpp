/*
 * HardWareFrameBuffer.cpp
 *
 *  Created on: 2016-3-24
 *      Author: CDJ
 */

#include "m3d/graphics/HardWareFrameBuffer.h"
#include "m3d/graphics/Texture2D.h"
#include "m3d/graphics/GeometryBuffer.h"
#include "m3d/model/Image.h"
#include "m3d/ResourceManager.h"
#include "m3d/graphics/HardWareRenderBuffer.h"
#include "m3d/graphics/Texture.h"
#include "sview/views/Parameters.h"
namespace M3D
{

HardWareFrameBuffer::HardWareFrameBuffer():
		GPUObject()
{
	Init();
}

HardWareFrameBuffer::HardWareFrameBuffer(int width, int height):
		GPUObject()
{
	Init();
	m_width = width;
	m_height = height;

}
/**
 * 创建FBO，需要在绘制线程
 */
GLuint HardWareFrameBuffer::GetFBO()
{
	KeepOriginalFBO();
	GLuint fbo = 0;
	if (!m_object)
	{
		glGenFramebuffers(1, &m_object);
	}
	fbo = m_object;
	return fbo;
}

/**
 * 清楚FBO绑定的深度或者颜色资源
 */
void HardWareFrameBuffer::ClearResource()
{
	LOGI("HardWareFrameBuffer::ClearResource()");
//	LOGI("m_depthAttachment==%p",m_depthAttachment);
	if(m_depthAttachment)
	{
		if(m_depthAttachment->GetTargetType() == RBO_TARGET )
		{
			HardWareRenderBuffer * p = dynamic_cast<HardWareRenderBuffer *>(m_depthAttachment);
			if(p) p->Release();
		}
		else if(m_depthAttachment->GetTargetType() == TEXTURE_TARGET)
		{
			GeometryBuffer * p = dynamic_cast<GeometryBuffer *>(m_depthAttachment);
			if(p) p->Release();
		}
	}
	m_depthAttachment = NULL;

	for(int i=0;i<m_maxColorAttachNum;i++)
	{
//		LOGI("m_colorAttachments==%p",m_colorAttachments[i]);
		if(m_colorAttachments[i])
		{
			if(m_colorAttachments[i]->GetTargetType() == RBO_TARGET )
			{
				((HardWareRenderBuffer*)(m_colorAttachments[i]))->Release();
			}
			else if(m_colorAttachments[i]->GetTargetType() == TEXTURE_TARGET)
			{
				((GeometryBuffer*)(m_colorAttachments[i]))->Release();
			}
		}
		m_colorAttachments[i] = NULL;
	}

	LOGI("HardWareFrameBuffer::ClearResource() END");
}

HardWareFrameBuffer::~HardWareFrameBuffer()
{
	if (m_object)
	{
//		glDeleteBuffers(1, &m_object);
		if(this->m_resourceMgr)
		{
			this->m_resourceMgr->AddGLObject(m_object,ResourceManager::FBO);
		}
	}
	ClearResource();
}

/**
 * 获取深度挂在对象，useRBO决定是否使用纹理
 */
bool HardWareFrameBuffer::CreateDepthAttachment(bool useRBO)
{
	bool ret = false;
	//m_depthRBO = useRBO;
	//LOGI("m_depthAttachment.GetObj() === %d",*m_depthAttachment.GetObj());
	if (!m_depthAttachment)
	{
		//LOGI("m_depthAttachment.GetObj() has not zero number ==%d",*m_depthAttachment.GetObj());

//	LOGI("M_fbo  === %d",m_fbo);
		if (m_object)
		{
			//LOGI("M_fbo exist === %d",m_fbo);
			glBindFramebuffer(GL_FRAMEBUFFER, m_object);
			if (m_depthRBO)
			{
				if (!m_depthMS)
				{
					HardWareRenderBuffer * rbo = NULL;
					if (!m_depthAttachment)
					{
						m_depthAttachment = new HardWareRenderBuffer(GL_DEPTH_COMPONENT, m_width, m_height);
						HardWareRenderBuffer * rbo = (HardWareRenderBuffer *)m_depthAttachment;
						rbo->Create(false, 0);
						rbo->AddRef();
					}
					glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER,
						((HardWareRenderBuffer *)m_depthAttachment)->GetObject());
				}
				else
				{
					HardWareRenderBuffer * rbo = NULL;
					if (!m_depthAttachment)
					{
						m_depthAttachment = new HardWareRenderBuffer(GL_DEPTH_COMPONENT, m_width, m_height);
						HardWareRenderBuffer * rbo = (HardWareRenderBuffer *)m_depthAttachment;
						rbo->Create(true, SVIEW::Parameters::Instance()->m_msaaNum);
						rbo->AddRef();
					}
					glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER,
						((HardWareRenderBuffer *)m_depthAttachment)->GetObject());
				}
			}
			else
			{
//			LOGI("Depth image has been created");

//
//	        string extensions = string((const char *)glGetString(GL_EXTENSIONS));
//	        if(extensions.find("OES_standard_derivatives") != string::npos )
//	        { LOGE("support OES_standard_derivatives");}
//	        else{
//	        	LOGE( "support not OES_standard_derivatives");
//	        }

				if (!m_depthAttachment)
				{
					string key = IntToString(this->OBJID++) + string("_depthAttachment");
					//LOGE("key %s",key.c_str());
					m_depthAttachment = (GeometryBuffer*) this->m_resourceMgr->GetOrCreateTexture(key,
							Texture::TEXTURE_GEO);
					((GeometryBuffer *) m_depthAttachment)->AddRef();
					//LOGI("create m_depthAttachment");
				}
				//LOGI("((GeometryBuffer *) m_depthAttachment) = %p", ((GeometryBuffer *) m_depthAttachment));
				GLuint obj = ((GeometryBuffer *) m_depthAttachment)->GetOGLObj();
				if (obj)
				{
					glBindTexture(GL_TEXTURE_2D, obj);
					glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_width, m_height, 0, GL_DEPTH_COMPONENT,
							GL_UNSIGNED_INT, NULL);
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, obj, 0);
					glBindTexture(GL_TEXTURE_2D, 0);
				}
			}
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			ret = true;
		}
	}
	return ret;
}

/**
 * 挂在颜色挂在点
 */
bool HardWareFrameBuffer::LinkTextureColorAttachment(int index)
{
	bool ret = false;
	if (index >= m_maxColorAttachNum)
	{
		ret = false;
		return ret;
	}
	if (!m_colorAttachments.at(index))
	{

		if (m_object)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, m_object);
			if (m_colorAttaRBOS[index])
			{
				if (m_colorAttaMSS[index])
				{
					HardWareRenderBuffer * rbo = NULL;
					if (!m_colorAttachments[index])
					{
						m_colorAttachments[index] = new HardWareRenderBuffer(GL_RGBA, m_width, m_height);
						HardWareRenderBuffer * rbo = (HardWareRenderBuffer *)m_colorAttachments[index];
						rbo->Create(true, SVIEW::Parameters::Instance()->m_msaaNum);
						rbo->AddRef();
					}
					glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_RENDERBUFFER,
						((HardWareRenderBuffer *)m_colorAttachments[index])->GetObject());
				}
				else
				{
					HardWareRenderBuffer * rbo = NULL;
					if (!m_colorAttachments[index])
					{
						m_colorAttachments[index] = new HardWareRenderBuffer(GL_RGBA, m_width, m_height);
						HardWareRenderBuffer * rbo = (HardWareRenderBuffer *)m_colorAttachments[index];
						rbo->Create(false, 0);
						rbo->AddRef();
					}
					glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_RENDERBUFFER,
						((HardWareRenderBuffer *)m_colorAttachments[index])->GetObject());
				}
			}
			else 
			{
				string key = IntToString(this->OBJID++) + string("_colorAttachment") + IntToString(index);
				if (!(m_colorAttachments[index]))
				{

				m_colorAttachments[index] = (GeometryBuffer*) this->m_resourceMgr->GetOrCreateTexture(key,
						Texture::TEXTURE_GEO);
					((GeometryBuffer *)(m_colorAttachments[index]))->AddRef();

				}
				GLuint obj = ((GeometryBuffer *)(m_colorAttachments[index]))->GetOGLObj();
				if (obj)
				{
					LOGI("Resize m_width = %d,m_height = %d", m_width, m_height);
					glBindTexture(GL_TEXTURE_2D, obj);
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_2D, obj, 0);
					glBindTexture(GL_TEXTURE_2D, 0);
				}
			}
			ret = true;
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
	}
		return ret;
}

void HardWareFrameBuffer::SetColorAttachmentAttribute(int index, bool useMutiSample, bool useRBO)
{
	m_colorAttaMSS[index] = useMutiSample;
	m_colorAttaRBOS[index] = useRBO;
	m_depthMS = useMutiSample;
	m_depthRBO = useRBO;
}

/**
 * 设置FBO大小
 */
void HardWareFrameBuffer::SetSize(int width, int height)
{
	this->m_width = width;
	this->m_height = height;
	MakeDirty();
}

void HardWareFrameBuffer::MakeDirty()
{
	m_dirty = true;
}

bool HardWareFrameBuffer::HasValue()
{
	bool hasValue = false;

	if(m_depthAttachment)
	{
		hasValue = true;
	}
	for (int index = 0; index < m_maxColorAttachNum; index++)
			{
				if (m_colorAttachments[index])
				{
					hasValue = true;
					break;
				}
			}
	return hasValue;
}

GLint HardWareFrameBuffer::GetOriginalFBO()
{
	return m_originalFBO;
}

GLint HardWareFrameBuffer::GetFirstFBO()
{
	if (m_firstFBO == 0)
	{
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_firstFBO);
	}
	return m_firstFBO;
}

/**
 * 当视口变化时，需要重新调整FBO所绑定的纹理
 */
void HardWareFrameBuffer::Resize()
{
	if(!m_dirty)
	{
		return;
	}
	if (m_object)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_object);
		for (int index = 0; index < m_maxColorAttachNum; index++)
		{
			if (m_colorAttachments[index])
			{
					if ((m_colorAttaRBOS[index]))
					{
						GLuint obj = ((HardWareRenderBuffer *)(m_colorAttachments[index]))->GetObject();
						glBindRenderbuffer(GL_RENDERBUFFER, obj);
						if ((m_colorAttaMSS[index]))
						{
#ifdef _WIN32
							glRenderbufferStorageMultisample(GL_RENDERBUFFER, SVIEW::Parameters::Instance()->m_msaaNum, GL_RGBA, m_width, m_height);
#endif
						}
						else
						{
							glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, m_width, m_height);
						}
						glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_RENDERBUFFER,
							((HardWareRenderBuffer *)m_colorAttachments[index])->GetObject());
						glBindRenderbuffer(GL_RENDERBUFFER, 0);
					}
					else
					{
						GLuint obj = ((GeometryBuffer *)(m_colorAttachments[index]))->GetOGLObj();
						if (obj)
						{
							GLuint obj = ((GeometryBuffer *) (m_colorAttachments[index]))->GetOGLObj();
							LOGI("Resize m_width = %d,m_height = %d", m_width, m_height);
							glBindTexture(GL_TEXTURE_2D, obj);
							glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
							glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_2D, obj, 0);
							glBindTexture(GL_TEXTURE_2D, 0);
						}
					}

			}
		}

		if (m_depthAttachment)
		{

				if (m_depthRBO)
				{
					GLuint obj = ((HardWareRenderBuffer *)(m_depthAttachment))->GetObject();
					glBindRenderbuffer(GL_RENDERBUFFER, obj);
					if (m_depthMS)
					{
#ifdef _WIN32
						glRenderbufferStorageMultisample(GL_RENDERBUFFER, SVIEW::Parameters::Instance()->m_msaaNum, GL_DEPTH_COMPONENT24, m_width, m_height);
#endif
					}
					else
					{
						glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_width, m_height);
					}

					glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER,
						((HardWareRenderBuffer *)m_depthAttachment)->GetObject());
					glBindRenderbuffer(GL_RENDERBUFFER, 0);
				}
				else
				{
					GLuint obj = ((GeometryBuffer *)m_depthAttachment)->GetOGLObj();
					if (obj)
					{
					glBindTexture(GL_TEXTURE_2D, obj);
					glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_width, m_height, 0, GL_DEPTH_COMPONENT,
						GL_UNSIGNED_INT, NULL);
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, obj, 0);
					glBindTexture(GL_TEXTURE_2D, 0);
					}
				}
		}

		glBindFramebuffer(GL_FRAMEBUFFER, m_object);
		bool ret = false;
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			LOGI("Framer buffer not complete,%0x", glCheckFramebufferStatus(GL_FRAMEBUFFER));
			ret = false;
			//glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	m_dirty = false;
}

/**
 * 获取宽度
 */
int HardWareFrameBuffer::GetWidth()
{
	return m_width;
}

/**
 * 获取高度
 */
int HardWareFrameBuffer::GetHeight()
{
	return m_height;
}

/**
 * 设置最大颜色挂在数量
 */
void HardWareFrameBuffer::SetMaxColorAttachmentNumber(int maxNum)
{
	this->m_maxColorAttachNum = maxNum;
}

/**
 * 绑定FBO为当前FBO
 */
void HardWareFrameBuffer::Bind()
{
	if(this->m_object){
		glBindFramebuffer(GL_FRAMEBUFFER,m_object);
	}
}

/**
 * 解绑FBO
 */
void HardWareFrameBuffer::UnBind()
{
	if(this->m_object){
		glBindFramebuffer(GL_FRAMEBUFFER,0);

		RestoreOriginalFBO();
	}
}

/**
 * 初始化
 */
void HardWareFrameBuffer::Init()
{
	this->m_width = 512;
	this->m_height = 512;
	m_object = 0;
	m_maxColorAttachNum = 4;
	m_colorAttachments.resize(4,NULL);
	m_depthAttachment = NULL;
	m_dirty = true;
	m_depthMS = false;
	m_depthRBO = false;
	m_colorAttaMSS.resize(4, false);
	m_colorAttaRBOS.resize(4, false);
	m_originalFBO = 0;
	m_firstFBO = 0;
}

/**
 * 检查FBO是否创建成功
 */
bool HardWareFrameBuffer::CheckStatus()
{
	glBindFramebuffer(GL_FRAMEBUFFER,m_object);
	bool ret = false;
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		LOGI("Framer buffer not complete,%0x",glCheckFramebufferStatus(GL_FRAMEBUFFER));
		ret = false;
		glBindFramebuffer(GL_FRAMEBUFFER,0);
	}
	else
	{
//		LOGI("Frame buffer create success");
		ret = true;
	}
	return ret;
}

GLRenderTarget* HardWareFrameBuffer::GetColorAttachment(int index)
{
	GLRenderTarget* temp = NULL;
	temp = m_colorAttachments.at(index);
	return temp;
}

/*
 * 在使用绑定当前FBO之前，把原始的先存储起来，用来以后恢复（主要用于IOS系统）
 */
void HardWareFrameBuffer::KeepOriginalFBO()
{
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_originalFBO);
}

/*
 * 在解绑当前的FBO使用后，恢复到原来的（主要用于IOS系统）
 */
void HardWareFrameBuffer::RestoreOriginalFBO()
{
//#ifdef __IOS__
	 glBindFramebuffer(GL_FRAMEBUFFER,m_originalFBO);
//#endif
}

} /* namespace M3D */
