#include "m3d/graphics/Texture.h"

#include "m3d/ResourceManager.h"
namespace M3D
{
string Texture::APKPath = "";

Texture::~Texture()
{
	if (this->m_image)
	{
		delete this->m_image;
		this->m_image = NULL;
	}

	//保证析构在GL线程中进行
	if (this->m_resourceMgr)
	{
		//在析构时，将material从资源管理器中移除。此对象真正点不存在了
		//this->m_resourceMgr->RemoveTexture(m_name);
		if (m_textures[0] > 0)
		{
			this->m_resourceMgr->AddGLObject(m_textures[0],
					ResourceManager::TEXTURE);
			m_textures[0] = 0;
		}
		this->m_resourceMgr = NULL;
	}
}

Texture::Texture() :
		Resource()
{
	Init();
}

void Texture::Init()
{
	m_image = NULL;
	this->m_textures[0] = 0;
	this->m_name = "";
	this->MarkDirty();
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

unsigned int Texture::GetID()
{
	//以GPU返回的ID标识作为ID
	return this->m_textures[0];
}

unsigned int Texture::GetOGLObj()
{
	if (m_isTextureDirty)
	{
		LOGI("dirty");
		this->UpdataOGLObj();

		m_isTextureDirty = false;
	}

	return this->m_textures[0];
}

void Texture::SetImageParameter(int forceChannels, unsigned int flags)
{
	m_forceChannels = forceChannels;
	m_flags = flags;
}

void Texture::SetName(const string& name)
{
	this->m_name = name;
}

const string& Texture::GetName()
{
	return this->m_name;
}

void Texture::UpdataOGLObj()
{

}

void Texture::MarkDirty()
{
	m_isTextureDirty = true;
}

void Texture::SetImagePath(const string& imagePath)
{
	m_imagePath = imagePath;
	LOGI(" Texture:: SetImagePath()%s", m_imagePath.c_str());
	this->MarkDirty();
}

const string& Texture::GetImagePath() const
{
	return m_imagePath;
}

void Texture::SetImage(Image* image)
{
	this->m_image = image;
	if (this->m_image)
	{
		this->MarkDirty();
	}
}

Image* Texture::GetImage()
{
	return this->m_image;
}

}

