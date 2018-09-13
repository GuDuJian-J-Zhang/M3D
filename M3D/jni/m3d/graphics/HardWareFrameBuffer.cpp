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
#include "m3d/utils/IDCreator.h"
#include "../renderer/RenderContext.h"
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
GLuint HardWareFrameBuffer::Create()
{
	KeepOriginalFBO();
	GLuint fbo = 0;
	if (!m_object)
	{//TODO MergeAndroid
#ifdef _WIN32
		glGenFramebuffersEXT(1, &m_object);
#else
		glGenFramebuffers(1, &m_object);
#endif
	}
	fbo = m_object;
	return fbo;
}

/**
 * 清楚FBO绑定的深度或者颜色资源
 */
void HardWareFrameBuffer::ClearResource()
{
	LOGD("HardWareFrameBuffer::ClearResource()");
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
	Destroy();
	MakeDirty();
	LOGD("HardWareFrameBuffer::ClearResource() END");
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
			//LOGI("M_fbo exist === %d",m_fbo);//TODO MergeAndroid
#ifdef _WIN32
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_object);
#else
			glBindFramebuffer(GL_FRAMEBUFFER, m_object);
#endif

			if (m_depthRBO)
			{
				if (!m_depthMS)
				{
					HardWareRenderBuffer * rbo = NULL;
					if (!m_depthAttachment)
					{//TODO MergeAndroid
#ifdef _WIN32
						m_depthAttachment = new HardWareRenderBuffer(GL_DEPTH24_STENCIL8_EXT, m_width, m_height);
#else
						m_depthAttachment = new HardWareRenderBuffer(GL_DEPTH24_STENCIL8_OES, m_width, m_height);
#endif

						HardWareRenderBuffer * rbo = (HardWareRenderBuffer *)m_depthAttachment;
						rbo->Create(false, 0);
						rbo->AddRef();
					}//TODO MergeAndroid
#ifdef _WIN32
					glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER_EXT,
											((HardWareRenderBuffer *)m_depthAttachment)->GetObject());
					glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER_EXT,
						((HardWareRenderBuffer *)m_depthAttachment)->GetObject());
#else
					glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER,
											((HardWareRenderBuffer *)m_depthAttachment)->GetObject());
					glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER,
						((HardWareRenderBuffer *)m_depthAttachment)->GetObject());
#endif

				}
				else
				{
					HardWareRenderBuffer * rbo = NULL;
					if (!m_depthAttachment)
					{//TODO MergeAndroid
#ifdef _WIN32
						m_depthAttachment = new HardWareRenderBuffer(GL_DEPTH24_STENCIL8_EXT, m_width, m_height);
						HardWareRenderBuffer * rbo = (HardWareRenderBuffer *)m_depthAttachment;
#else
						m_depthAttachment = new HardWareRenderBuffer(GL_DEPTH24_STENCIL8_OES, m_width, m_height);
						HardWareRenderBuffer * rbo = (HardWareRenderBuffer *)m_depthAttachment;
#endif

						rbo->Create(true, SVIEW::Parameters::Instance()->m_msaaNum);
						rbo->AddRef();
					}//TODO MergeAndroid
#ifdef _WIN32
					glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER_EXT,
										((HardWareRenderBuffer *)m_depthAttachment)->GetObject());
					glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER_EXT,
						((HardWareRenderBuffer *)m_depthAttachment)->GetObject());
#else
					glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER,
										((HardWareRenderBuffer *)m_depthAttachment)->GetObject());
					glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER,
						((HardWareRenderBuffer *)m_depthAttachment)->GetObject());
#endif

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
					string key =IDCreator::GetUUID() + string("_depthAttachment");
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
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
					glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_width, m_height, 0, GL_DEPTH_COMPONENT,
							GL_UNSIGNED_INT, NULL);
					//TODO MergeAndroid
					#ifdef _WIN32
					glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, obj, 0);
					#else
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, obj, 0);
					#endif

					glBindTexture(GL_TEXTURE_2D, 0);
				}
			}
			//TODO MergeAndroid
			#ifdef _WIN32
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
			#else
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			#endif

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
			//TODO MergeAndroid
			#ifdef _WIN32
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_object);
			#else
			glBindFramebuffer(GL_FRAMEBUFFER, m_object);
			#endif

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
					//TODO MergeAndroid
					#ifdef _WIN32
					glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT + index, GL_RENDERBUFFER_EXT,
											((HardWareRenderBuffer *)m_colorAttachments[index])->GetObject());
					#else
					glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_RENDERBUFFER,
											((HardWareRenderBuffer *)m_colorAttachments[index])->GetObject());
					#endif

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
					//TODO MergeAndroid
					#ifdef _WIN32
					glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT + index, GL_RENDERBUFFER_EXT,
											((HardWareRenderBuffer *)m_colorAttachments[index])->GetObject());
					#else
					glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_RENDERBUFFER,
											((HardWareRenderBuffer *)m_colorAttachments[index])->GetObject());
					#endif

				}
			}
			else 
			{
				string key = IDCreator::GetUUID() + string("_colorAttachment") + IntToString(index);
				if (!(m_colorAttachments[index]))
				{
					m_colorAttachments[index] = (GeometryBuffer*)this->m_resourceMgr->GetOrCreateTexture(key,
						Texture::TEXTURE_GEO);
					((GeometryBuffer *)(m_colorAttachments[index]))->AddRef();

				}
				GLuint obj = ((GeometryBuffer *)(m_colorAttachments[index]))->GetOGLObj();
				if (obj)
				{
					LOGI("Resize m_width = %d,m_height = %d", m_width, m_height);
					glBindTexture(GL_TEXTURE_2D, obj);
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
					//TODO MergeAndroid
					#ifdef _WIN32
					glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT + index, GL_TEXTURE_2D, obj, 0);
					#else
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_2D, obj, 0);
					#endif

					glBindTexture(GL_TEXTURE_2D, 0);
				}
			}
			ret = true;
			//TODO MergeAndroid
			#ifdef _WIN32
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
			#else
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			#endif

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
	if (m_width == width && m_height == height)
	{
		return;
	}
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
		//TODO MergeAndroid
		#ifdef _WIN32
		glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &m_firstFBO);
		#else
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_firstFBO);
		#endif

	}
	return m_firstFBO;
}

bool HardWareFrameBuffer::DeatchFromPriFBO()
{
	m_firstFBO = 0;
	m_originalFBO = 0;
	return true;
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
		//TODO MergeAndroid
		#ifdef _WIN32
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_object);
		#else
		glBindFramebuffer(GL_FRAMEBUFFER, m_object);
		#endif

		for (int index = 0; index < m_maxColorAttachNum; index++)
		{
			if (m_colorAttachments[index])
			{
					if ((m_colorAttaRBOS[index]))
					{
						GLuint obj = ((HardWareRenderBuffer *)(m_colorAttachments[index]))->GetObject();
						//TODO MergeAndroid
						#ifdef _WIN32
						glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, obj);
						if ((m_colorAttaMSS[index]))
						{
							glRenderbufferStorageMultisampleEXT(GL_RENDERBUFFER_EXT, SVIEW::Parameters::Instance()->m_msaaNum, GL_RGBA, m_width, m_height);
						}
						else
						{
							glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_RGBA, m_width, m_height);
						}
						glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT + index, GL_RENDERBUFFER_EXT,
							((HardWareRenderBuffer *)m_colorAttachments[index])->GetObject());
						glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
						#else
						glBindRenderbuffer(GL_RENDERBUFFER, obj);
						if ((m_colorAttaMSS[index]))
						{
							glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, m_width, m_height);
						}
						else
						{
							glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, m_width, m_height);
						}
						glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_RENDERBUFFER,
							((HardWareRenderBuffer *)m_colorAttachments[index])->GetObject());
						glBindRenderbuffer(GL_RENDERBUFFER, 0);
						#endif

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
							//TODO MergeAndroid
							#ifdef _WIN32
							glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT + index, GL_TEXTURE_2D, obj, 0);
							#else
							glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_2D, obj, 0);
							#endif

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
					//TODO MergeAndroid
					#ifdef _WIN32
					glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, obj);
					if (m_depthMS)
					{
						glRenderbufferStorageMultisampleEXT(GL_RENDERBUFFER_EXT, SVIEW::Parameters::Instance()->m_msaaNum, GL_DEPTH24_STENCIL8_EXT, m_width, m_height);
					}
					else
					{
						glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH24_STENCIL8_EXT, m_width, m_height);
					}

					glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER_EXT,
						((HardWareRenderBuffer *)m_depthAttachment)->GetObject());
					glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER_EXT,
						((HardWareRenderBuffer *)m_depthAttachment)->GetObject());
					glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
					#else
					glBindRenderbuffer(GL_RENDERBUFFER, obj);
					if (m_depthMS)
					{
						glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8_OES, m_width, m_height);
					}
					else
					{
						glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8_OES, m_width, m_height);
					}

					glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER,
						((HardWareRenderBuffer *)m_depthAttachment)->GetObject());
					glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER,
						((HardWareRenderBuffer *)m_depthAttachment)->GetObject());
					glBindRenderbuffer(GL_RENDERBUFFER, 0);
					#endif

				}
				else
				{
					GLuint obj = ((GeometryBuffer *)m_depthAttachment)->GetOGLObj();
					if (obj)
					{
					glBindTexture(GL_TEXTURE_2D, obj);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
					glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_width, m_height, 0, GL_DEPTH_COMPONENT,
						GL_UNSIGNED_INT, NULL);
					//TODO MergeAndroid
					#ifdef _WIN32
					glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, obj, 0);
					#else
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, obj, 0);
					#endif

					glBindTexture(GL_TEXTURE_2D, 0);
					}
				}
		}

		//TODO MergeAndroid
		#ifdef _WIN32
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_object);
		bool ret = false;
		if (glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT) != GL_FRAMEBUFFER_COMPLETE_EXT)
		{
			LOGI("Framer buffer not complete,%0x", glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT));
			ret = false;
			//glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
		}
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
		#else
		glBindFramebuffer(GL_FRAMEBUFFER, m_object);
		bool ret = false;
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			LOGI("Framer buffer not complete,%0x", glCheckFramebufferStatus(GL_FRAMEBUFFER));
			ret = false;
			//glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		#endif

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
		//TODO MergeAndroid
		#ifdef _WIN32
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,m_object);
		#else
		glBindFramebuffer(GL_FRAMEBUFFER,m_object);
		#endif

	}
}

/**
 * 解绑FBO
 */
void HardWareFrameBuffer::UnBind()
{
	if(this->m_object){
		//TODO MergeAndroid
		#ifdef _WIN32
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,0);
		#else
		glBindFramebuffer(GL_FRAMEBUFFER,0);
		#endif

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
	m_depthTarget = nullptr;
	m_usedColorAttachment = 0;
	m_colorAttachmentNumber = 1;
	m_useDepthBuffer = true;
	m_useStencilBuffer = true;
	m_useMultisample = false;
	m_multisamplerNumber = 4;
	m_colorUseRenderBuffer = false;
	m_depthUseRenderBuffer = false;
	m_colorInternalFormat = GL_RGBA;
	m_depthInternalFormat = GL_DEPTH_COMPONENT;
	m_useOutColorTexture = false;
	m_outColorTextureLevel = -1;
	m_outColorTexture = NULL;
	m_colorTextureMipmap = false;
}

/**
 * 检查FBO是否创建成功
 */
bool HardWareFrameBuffer::CheckStatus()
{
	bool ret = false;
	//TODO MergeAndroid
	#ifdef _WIN32
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,m_object);

	if (glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT) != GL_FRAMEBUFFER_COMPLETE_EXT)
	{
		GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
		//LOGI("Framer buffer not complete,%0x",glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT));
		ret = false;
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,0);
	}
	else
	{
//		LOGI("Frame buffer create success");
		ret = true;
	}

	#else
	glBindFramebuffer(GL_FRAMEBUFFER,m_object);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		LOGI("Framer buffer not complete,%0x",glCheckFramebufferStatus(GL_FRAMEBUFFER));
		ret = false;
		glBindFramebuffer(GL_FRAMEBUFFER,0);
	}
	else
	{
//		LOGI("Frame buffer create success");
		ret = true;
	}

	#endif
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
	//TODO MergeAndroid
	#ifdef _WIN32
	glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &m_originalFBO);
	#else
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_originalFBO);
	#endif

}

/*
 * 在解绑当前的FBO使用后，恢复到原来的（主要用于IOS系统）
 */
void HardWareFrameBuffer::RestoreOriginalFBO()
{
//#ifdef __IOS__
	//TODO MergeAndroid
	#ifdef _WIN32
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,m_originalFBO);
	#else
	glBindFramebuffer(GL_FRAMEBUFFER,m_originalFBO);
	#endif

//#endif
}


int HardWareFrameBuffer::AllocColorAttachment() 
{
	int colorAttachment = m_usedColorAttachment;

	m_usedColorAttachment += 1;

	return colorAttachment;
}
void HardWareFrameBuffer::Destroy() 
{

	if (m_depthTarget)
	{
		if (m_depthTarget->GetTargetType() == RBO_TARGET)
		{
			HardWareRenderBuffer * p = dynamic_cast<HardWareRenderBuffer *>(m_depthTarget);
			if (p) p->Release();
		}
		else if (m_depthTarget->GetTargetType() == TEXTURE_TARGET)
		{
			GeometryBuffer * p = dynamic_cast<GeometryBuffer *>(m_depthTarget);
			if (p) p->Release();
		}
	}
	m_depthTarget = NULL;

	map<int, GLRenderTarget*>::iterator it = m_colorTargets.begin();
	for (;it != m_colorTargets.end();it++)
	{
		if (it->second)
		{
			if (it->second->GetTargetType() == RBO_TARGET)
			{
				((HardWareRenderBuffer*)(it->second))->Release();
			}
			else if (it->second->GetTargetType() == TEXTURE_TARGET)
			{
				((GeometryBuffer*)(it->second))->Release();
			}
		}
		it->second = nullptr;
	}
	//Bind();
//	UnBind();
}

void HardWareFrameBuffer::ReShape() 
{
	if (m_dirty) 
	{
		Destroy();
		if (m_object)
		{
#ifdef _WIN32
			glDeleteFramebuffersEXT(1, &m_object);
#else
			glDeleteFramebuffers(1, &m_object);
#endif
			m_object = 0;
		}
		m_usedColorAttachment = 0;
		GenerateFramerBuffer();
	}
}
void HardWareFrameBuffer::GenerateFramerBuffer()
{
	Create();

	GenerateColorAttatchment();

	GenerateDepthAttatchment();
	m_dirty = false;
}

void HardWareFrameBuffer::SetParameters(int colorNumber,bool rboColor, bool rboDepth,bool useDepth, bool useStencil, bool useMultisample, int multisampler)
{
	m_colorAttachmentNumber = colorNumber;
	m_useDepthBuffer = useDepth;
	m_useStencilBuffer = useStencil;
	m_useMultisample = useMultisample;
	m_multisamplerNumber = multisampler;
	m_colorUseRenderBuffer = rboColor;
	m_depthUseRenderBuffer = rboDepth;
}
void HardWareFrameBuffer::GenerateColorAttatchment()
{
	if (m_colorAttachmentNumber == 0)
	{
#ifdef _WIN32
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_object);
#else
		glBindFramebuffer(GL_FRAMEBUFFER, m_object);
#endif
#ifdef _WIN32
		//如果不写入颜色缓冲区，则执行以下代码，告诉opengl不需要写入颜色和读出颜色
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
#else
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
#endif
	}
	else
	{	
		vector<GLenum> buffers;
		for (int i = 0; i < m_colorAttachmentNumber; i++)
		{
#ifdef _WIN32
			buffers.push_back(GL_COLOR_ATTACHMENT0_EXT + i);
#else
			buffers.push_back(GL_COLOR_ATTACHMENT0 + i);
#endif
			
			if (m_colorUseRenderBuffer)
			{
				AttachRenderBufferToColor();
				//RenderContext::OgldevError();
			}
			else
			{
				AttachTextureToColor();
				//RenderContext::OgldevError();
			}
		}
	}
}
void HardWareFrameBuffer::GenerateDepthAttatchment()
{
	if (m_useDepthBuffer)
	{

		if (this->m_useMultisample) {
			this->m_depthUseRenderBuffer = true;
		}

		if (this->m_depthUseRenderBuffer)
		{
			this->AttachRenderBufferToDepth();
		}
		else
		{
			this->AttachTextureToDepth();
		}
	}
}
	GLuint HardWareFrameBuffer::AttachTextureToColor()
{
	GLenum format;
	GLenum dataType;
	switch (m_colorInternalFormat)
	{
	case GL_RGBA:
		format = GL_RGBA;
		dataType = GL_UNSIGNED_BYTE;
		break;
#ifdef _WIN32
	case GL_RGB32F:
		format = GL_RGB;
		dataType = GL_FLOAT;
		break;
	case GL_R32F:
		format = GL_RED;
		dataType = GL_FLOAT;
		break;
	case  GL_R16F:
		format = GL_RED;
		dataType = GL_FLOAT;
		break;
#endif
	default:
		break;
	}

#ifdef _WIN32
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_object);
#else
	glBindFramebuffer(GL_FRAMEBUFFER, m_object);
#endif
	int colorAttachment = AllocColorAttachment();

	GeometryBuffer* texture = NULL;

		GLuint textureHandle = 0;
		if (!m_outColorTexture)
		{
		string key = IDCreator::GetUUID() + string("_colorAttachment");

			texture = (GeometryBuffer*)this->m_resourceMgr->GetOrCreateTexture(key,
				Texture::TEXTURE_GEO);
		texture->AddRef();

		GLuint obj = texture->GetOGLObj();
		if (obj)
		{			
			glBindTexture(GL_TEXTURE_2D, obj);
				glTexImage2D(GL_TEXTURE_2D, 0, m_colorInternalFormat, m_width, m_height, 0, format, dataType, NULL);
				if (m_colorTextureMipmap)
				{
					texture->MipMap(true);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
					//RenderContext::OgldevError();
					texture->GenerateMipMap();
					//RenderContext::OgldevError();
				}

#ifdef _WIN32
			glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT + colorAttachment, GL_TEXTURE_2D, obj, 0);
#else
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + colorAttachment, GL_TEXTURE_2D, obj, 0);
#endif

			glBindTexture(GL_TEXTURE_2D, 0);
			}
		    textureHandle = obj;
		}
		else
		{
			texture = m_outColorTexture;
			texture->AddRef();

			GLuint obj = texture->GetOGLObj();
			if (obj)
			{
				glBindTexture(GL_TEXTURE_2D, obj);
				if (m_outColorTextureLevel == 0 && (m_colorTextureMipmap))
				{
					glTexImage2D(GL_TEXTURE_2D, m_outColorTextureLevel, m_colorInternalFormat, m_width, m_height, 0, format, dataType, NULL);
				}
				//RenderContext::OgldevError();
				if (m_outColorTextureLevel == 0 &&  (m_colorTextureMipmap))
				{
					m_outColorTexture->GenerateMipMap();
				}
#ifdef _WIN32
				glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT + colorAttachment, GL_TEXTURE_2D, obj, m_outColorTextureLevel);
#else
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + colorAttachment, GL_TEXTURE_2D, obj, 0);
#endif
				glBindTexture(GL_TEXTURE_2D, 0);
			}
			textureHandle = obj;
		}

#ifdef _WIN32
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
#else
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
#endif

		this->m_colorTargets[colorAttachment] = texture;
		return textureHandle;
	}
	void HardWareFrameBuffer::OnlyAttachTextureToColor(GLuint texture)
	{
		int colorAttachment = m_usedColorAttachment - 1;
		if (colorAttachment<0){
			return;
		}
		//this->Bind();
#ifdef _WIN32
		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT + colorAttachment, GL_RENDERBUFFER_EXT,
			texture);
#else
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + colorAttachment, GL_RENDERBUFFER,
			texture);
#endif
		//this->UnBind();
	}
	void HardWareFrameBuffer::AttachRenderBufferToColor()    
	{
		//TODO MergeAndroid
#ifdef _WIN32
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_object);
#else
			glBindFramebuffer(GL_FRAMEBUFFER, m_object);
#endif
			int colorAttachment = AllocColorAttachment();
			HardWareRenderBuffer * rbo = NULL;


			rbo = new HardWareRenderBuffer(m_colorInternalFormat, m_width, m_height);
			rbo->SetResourceManager(this->m_resourceMgr);
			if (m_useMultisample)
			{
				rbo->Create(true, SVIEW::Parameters::Instance()->m_msaaNum);
			}
			else
			{
				rbo->Create(false, 0);
			}
			rbo->AddRef();

#ifdef _WIN32
			glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT + colorAttachment, GL_RENDERBUFFER_EXT,
				rbo->GetObject());
#else
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + colorAttachment, GL_RENDERBUFFER,
				rbo->GetObject());
#endif


#ifdef _WIN32
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
#else
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
#endif

			this->m_colorTargets[colorAttachment] = rbo;

}
void HardWareFrameBuffer::AttachRenderBufferToDepth(bool useStencil)
{	

			
#ifdef _WIN32
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_object);
#else
			glBindFramebuffer(GL_FRAMEBUFFER, m_object);
#endif
			HardWareRenderBuffer * rbo = NULL;
#ifdef _WIN32
			rbo = new HardWareRenderBuffer(GL_DEPTH24_STENCIL8_EXT, m_width, m_height);
#else
			rbo = new HardWareRenderBuffer(GL_DEPTH24_STENCIL8_OES, m_width, m_height);
#endif
			rbo->SetResourceManager(this->m_resourceMgr);
			if (!m_useMultisample)
			{
				rbo->Create(false, 0);
			}
			else
			{
				rbo->Create(true, SVIEW::Parameters::Instance()->m_msaaNum);
			}
			rbo->AddRef();
#ifdef _WIN32
			glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER_EXT,
			rbo->GetObject());
			glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER_EXT,
			rbo->GetObject());
#else
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER,
			rbo->GetObject());
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER,
			rbo->GetObject());
#endif


#ifdef _WIN32
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
#else
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
#endif

			this->m_depthTarget = rbo;


}
void HardWareFrameBuffer::AttachTextureToDepth(bool useStencil)
{
	GLenum format;
	GLenum dataType;
	switch (m_depthInternalFormat)
	{
#ifdef _WIN32
	case GL_DEPTH_COMPONENT:
		format = GL_DEPTH_COMPONENT;
		dataType = GL_FLOAT;
		break;

	case GL_DEPTH_COMPONENT32:
		format = GL_DEPTH_COMPONENT;
		dataType = GL_FLOAT;
		break;
#else
        case GL_DEPTH_COMPONENT:
            format = GL_DEPTH_COMPONENT;
            dataType = GL_UNSIGNED_INT;
            break;
#endif
	//case GL_DEPTH_COMPONENT24:
	//	format = GL_RED;
	//	dataType = GL_FLOAT;
	//case  GL_R16F:
	//	format = GL_RED;
	//	dataType = GL_FLOAT;
	default:
		break;
	}
#ifdef _WIN32
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_object);
#else
	glBindFramebuffer(GL_FRAMEBUFFER, m_object);
#endif

	string key = IDCreator::GetUUID() + string("_depthAttachment");

	GeometryBuffer* texture = (GeometryBuffer*) this->m_resourceMgr->GetOrCreateTexture(key,
		Texture::TEXTURE_GEO);
	texture->AddRef();
	
	GLuint obj = texture->GetOGLObj();
	if (obj)
	{
		glBindTexture(GL_TEXTURE_2D, obj);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, m_depthInternalFormat, m_width, m_height, 0, format,
			dataType, NULL);
		//TODO MergeAndroid
#ifdef _WIN32
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, obj, 0);
#else
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, obj, 0);
#endif

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	//TODO MergeAndroid
#ifdef _WIN32
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
#else
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
#endif
	m_depthTarget = texture;
}
GLRenderTarget* HardWareFrameBuffer::GetColorTarget(int key)
{
	return m_colorTargets[key];
}

GLRenderTarget* HardWareFrameBuffer::GetDepthTarget()
{
	return m_depthTarget;
}

} /* namespace M3D */
