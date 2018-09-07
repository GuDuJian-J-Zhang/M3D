#include "m3d/graphics/Texture2D.h"

#include "m3d/ResourceManager.h"
#include "prebuilt/soil/include/SOIL.h"
#include "prebuilt/soil/include/stb_image.h"

namespace M3D
{

unsigned int Texture2D::LoadOGLTexture(const string& filename,
			int force_channels,
			unsigned int reuse_texture_ID,
			unsigned int flags)
{
	unsigned int OGLObj = SOIL_load_OGL_texture(filename.c_str(),
			force_channels,
			reuse_texture_ID,
			flags);
	glBindTexture(GL_TEXTURE_2D, OGLObj);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_minFliter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_magFliter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_wrapT);
	if (m_isMipMap)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	return OGLObj;
}

unsigned int Texture2D::LoadOGLTexture(const unsigned char *const buffer,
		int buffer_length,
		int force_channels,
		unsigned int reuse_texture_ID,
		unsigned int flags)
{
	unsigned int OGLObj = SOIL_load_OGL_texture_from_memory(buffer,
			buffer_length,
			force_channels,
			reuse_texture_ID,
			flags);
	glBindTexture(GL_TEXTURE_2D, OGLObj);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_minFliter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_magFliter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_wrapT);
	if (m_isMipMap)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	return OGLObj;
}

bool Texture2D::FreeOGLTexture(unsigned int OGLObj)
{
	bool freeState = false;


	return freeState;
}

void Texture2D::UpdateImage(Image* image)
{
	if (image)
	{
		if (this->m_textures[0])
		{
			glBindTexture(GL_TEXTURE_2D, m_textures[0]);
			if (image->m_pPixelData)
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->m_iWidth, image->m_iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->m_pPixelData);
			}
			else if (image->GetPath()!="")
			{
				void * pData = stbi_load(image->GetPath().c_str(), &image->m_iWidth, &image->m_iHeight, &image->m_channels, 0);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->m_iWidth, image->m_iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pData);
				stbi_image_free(pData);
			}		
			
		}
	}
}

Texture2D::Texture2D() :Texture()
{
	Init();
}

Texture2D::Texture2D(const string& imagePath):Texture()
{
	Init();

	this->SetPath(imagePath);
}

Texture2D::Texture2D(const string& imagePath,int forceChannels,unsigned int flags):Texture()
{
	Init();

	this->SetPath(imagePath);

	this->SetImageParameter(forceChannels,flags);
}

Texture2D::~Texture2D()
{

}


void Texture2D::Init()
{
	m_image = NULL;
	this->m_textures[0] = 0;
	m_isMipMap = true;
	this->SetImageParameter(TEXTURE_LOAD_RGBA,/*TEXTURE_FLAG_MIPMAPS*/0);
	this->MarkDirty();
}

void Texture2D::UpdataOGLObj()
{
//	LOGI(" Texture2D:: UpdataOGLObj() 111 %s",m_imagePath.c_str());
	ResourceManager* resourceMgr = this->m_resourceMgr;
	//路径有值，根据路径去生成纹理
	if (this->m_image)
	{
		string imagePath = this->GetName();
		unsigned int OGLObj = this->m_textures[0];

		//根据路径在资源管理器中没有找到资源，则创建OGL对象加入资源管理器中
		if (OGLObj == 0)
		{
			if (this->m_image->GetPath().length())
			{
				this->m_image->ReadFile();
				//OGLObj = LoadOGLTexture(this->m_image->GetPath(), m_forceChannels, 0, m_flags);
			}

			const unsigned char *const buffer = (const unsigned char*)m_image->m_pPixelData;
			int buffer_length = m_image->m_dataLength;
			if (buffer && buffer_length)
			{
				OGLObj = LoadOGLTexture(buffer, buffer_length, m_forceChannels,
					0, m_flags);
			}
			
			if (OGLObj != 0)
			{
				resourceMgr->AddOGLTexture(imagePath, OGLObj);
			}
			else
			{
				LOGE("create Texture2D error!!");
			}
		}
		this->m_textures[0] = OGLObj;
		//LOGI(" Texture2D:: UpdataOGLObj() id %d", OGLObj);
	}
//	else if(m_imagePath.length() > 0)
//	{
//		unsigned int OGLObj = this->m_textures[0];
////		resourceMgr->GetOGLTexture(this->m_imagePath);
//		//根据路径在资源管理器中没有找到资源，则创建OGL对象加入资源管理器中
//		if(OGLObj == 0)
//		{
//		
//
//			if(OGLObj != 0)
//			{
//				resourceMgr->AddOGLTexture(m_imagePath,OGLObj);
//			}else
//			{
//				LOGE("create Texture2D error!!");
//			}
//		}
//		this->m_textures[0] = OGLObj;
//		LOGI(" Texture2D:: UpdataOGLObj() id %d",OGLObj);
//	}
}


}

