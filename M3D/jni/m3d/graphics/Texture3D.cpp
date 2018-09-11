#include "m3d/graphics/Texture.h"

namespace M3D
{
string Texture::APKPath = "";

Texture::~Texture()
{
	if (this->m_image != NULL)
	{
		this->m_image;
	}
	this->m_image = NULL;
}

Texture::Texture()
{
	m_image = NULL;
	this->m_textures[0] = 0;
}

void Texture::Read(const string& fileName)
{
	//LOGI("MTextureImage::Read step1");

	if (this->m_image == NULL)
	{
		//LOGI("MTextureImage::Read step2");
		this->m_image = new Image;
	}
	//LOGI("MTextureImage::Read step3");
	this->m_image->Read(fileName);
	//LOGI("MTextureImage::Read step4");
}

void Texture::SetData(const void* data)
{

}

bool Texture::CreateOGLTexture()
{
	if (this->m_textures[0] == 0)
	{
//		LOGE("MTextureImage::CreateOGLTexture() w %d h %d",image->m_iWidth,
//				image->m_iHeight);
		glEnable(GL_TEXTURE_2D);
		// 创建纹理
		glGenTextures(1, m_textures);

		// 绑定纹理
		glBindTexture(GL_TEXTURE_2D, m_textures[0]);

		// 生成纹理
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_image->m_iWidth,
				m_image->m_iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE,
				m_image->m_pPixelData);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // 放大时
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // 缩小时

		glDisable(GL_TEXTURE_2D);
	}else
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_textures[0]);
	}
	return true;
}
}

