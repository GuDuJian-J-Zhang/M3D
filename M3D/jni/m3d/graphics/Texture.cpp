#include "m3d/graphics/Texture.h"

#include "m3d/ResourceManager.h"
namespace M3D
{
Texture::~Texture()
{
	if (this->m_image)
	{
		this->m_image->Release();
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
	m_isMipMap = false;
	m_minFliter = GL_LINEAR;
	m_magFliter = GL_LINEAR;
	m_wrapS = GL_REPEAT;
	m_wrapT = GL_REPEAT;
	m_textureEncoding = TEXEL_ENCODING_TYPE_LINEAR;
	m_minmapFliter = GL_LINEAR_MIPMAP_LINEAR;
	m_gammaInput = false;
	
	this->MarkDirty();
}

void Texture::SetPath(const string& fileName)
{
	if (this->m_image == NULL)
	{
		this->m_image = new Image;
		this->m_image->AddRef();
	}
	this->m_image->SetPath(fileName);
	this->MarkDirty();
}

string Texture::GetPath()
{
	if (this->m_image)
	{
		return this->m_image->GetPath();
	}
	return "";
}

void Texture::SetData(unsigned char* data, int length)
{
	if (this->m_image == NULL)
	{
		this->m_image = new Image;
		this->m_image->AddRef();
	}

	this->m_image->SetData(data,length);
	this->MarkDirty();
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

//void Texture::SetImagePath(const string& imagePath)
//{
//	m_imagePath = imagePath;
//	this->MarkDirty();
//}
//
//const string& Texture::GetImagePath() const
//{
//	return m_imagePath;
//}

void Texture::SetImage(Image* image)
{
	ReleaseMe(this->m_image);
	this->m_image = image;
	AddRefMe(this->m_image);
	if (this->m_image)
	{
		this->MarkDirty();
	}
}

Image* Texture::GetImage()
{
	return this->m_image;
}

void Texture::GenerateMipMap()
{

}

void Texture::NeedUpdate()
{
	MarkDirty();
}

bool Texture::Equals(Texture* texture)
{
	if (this->GetPath() == "" && texture->GetPath() == ""
		&& this->GetImage() == NULL && texture->GetImage() == NULL)
	{
		return true;
	}

	if (this->GetPath() != texture->GetPath() || this->GetImage() != texture->GetImage())
	{
		return false;
	}

	return true;
}

}

