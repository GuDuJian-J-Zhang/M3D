/*
 * GeometryBuffer.cpp
 *
 *  Created on: 2016-3-29
 *      Author: CDJ
 */

#include "m3d/graphics/GeometryBuffer.h"
#include "m3d/ResourceManager.h"
#include "prebuilt/soil/include/SOIL.h"
#include "m3d/graphics/GPUObject.h"
namespace M3D
{

GeometryBuffer::GeometryBuffer():Texture(),GLRenderTarget()
{
	Init();
}

GeometryBuffer::~GeometryBuffer()
{
	LOGI("GeometryBuffer Release");
}

unsigned int GeometryBuffer::LoadOGLTexture(const unsigned char* const buffer, int buffer_length, int force_channels,
		unsigned int reuse_texture_ID, unsigned int flags)
{
	unsigned int OGLObj = SOIL_load_OGL_texture_from_memory(buffer,
			buffer_length,
			force_channels,
			reuse_texture_ID,
			flags);

	return OGLObj;
}

/**
 * 创建几何渲染目标（法线，深度等）
 */
void GeometryBuffer::UpdataOGLObj()
{
		ResourceManager* resourceMgr = this->m_resourceMgr;
		string imagePath = this->GetName();
		unsigned int OGLObj = this->m_textures[0];

		//根据路径在资源管理器中没有找到资源，则创建OGL对象加入资源管理器中
		if (OGLObj == 0)
		{

			glGenTextures(1,&OGLObj);
			LOGI("Gen Texture,texture ID is %d",OGLObj);
			glBindTexture(GL_TEXTURE_2D, OGLObj);
			glBindTexture(GL_TEXTURE_2D, OGLObj);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_minFliter);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_magFliter);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_wrapS);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_wrapT);
			//if (m_isMipMap)
			//{
			//	//glTexImage2D(GL_TEXTURE_2D,0,)
			//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_minmapFliter);
			//	glGenerateMipmap(GL_TEXTURE_2D);
			//}
			if (OGLObj != 0)
			{
				resourceMgr->AddOGLTexture(imagePath, OGLObj);
			}
			else
			{
				LOGE("create Texture2D error!!");
			}
		}
		LOGI("OGLObj======%d",OGLObj);
		this->m_textures[0] = OGLObj;

}
/* namespace M3D */

/**
 * 初始化
 */
void GeometryBuffer::Init()
{
	m_targetType = TEXTURE_TARGET;
	//m_imagePath = "";
	m_image = NULL;
	this->m_textures[0] = 0;
    m_minFliter = GL_LINEAR;
    m_magFliter = GL_LINEAR;
    m_wrapS = GL_CLAMP_TO_EDGE;
    m_wrapT = GL_CLAMP_TO_EDGE;
	this->SetImageParameter(TEXTURE_LOAD_RGBA,TEXTURE_FLAG_MIPMAPS);
	this->MarkDirty();
}

void GeometryBuffer::GenerateMipMap()
{
	if (m_isMipMap)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
	}
}

}

