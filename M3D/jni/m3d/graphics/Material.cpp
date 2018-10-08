#include "m3d/graphics/Material.h"

#include "m3d/ResourceManager.h"
#include "m3d/graphics/Texture2D.h"

namespace M3D
{

Material::Material():Resource()
{
	this->Init();
}

Material::~Material()
{
	if(this->m_resourceMgr)
	{
		//在析构时，将material从资源管理器中移除。此对象真正点不存在了
		//this->m_resourceMgr->RemoveMaterial(m_name);

		if(this->m_texture)
		{
			this->m_texture->Release();
			this->m_texture = NULL;
		}

		if(this->m_bumpmap)
		{
			this->m_bumpmap->Release();
			this->m_bumpmap = NULL;
		}

		this->m_resourceMgr = NULL;
	}

	if(m_textureTransform)
	{
		delete m_textureTransform;
		m_textureTransform = NULL;
	}
}

void Material::Init()
{
	Color color(0.8f,0.8f,0.8f,1.0f);
	this->SetAmbient(color);
	this->SetDiffuse(color);
	this->setSpecular(color);
	this->m_texture = NULL;
	this->SetTexture(NULL);
	this->SetBumpMap(NULL);
	this->m_textureTransform = NULL;
	this->m_ambientTexture = NULL;
	this->m_shininess = 1.0f;

}


void Material::SetAmbient(const Color& ambient)
{
	this->m_ambient = ambient;
}
void Material::SetDiffuse(const Color& diffuse)
{
	this->m_Diffuse = diffuse;
}

void Material::setSpecular(const Color& specular)
{
	this->m_Specular = specular;
}

const Color& Material::GetAmbient() const
{
	return this->m_ambient;
}

const Color& Material::GetDiffuse() const
{
	return this->m_Diffuse;
}

const Color& Material::GetSpecular() const
{
	return this->m_Specular;
}

void Material::SetShadingMode(ShadingMode mode)
{
	this->m_shadingMode = mode;
}

void Material::SetTextureOp(TextureOp op)
{
	this->m_textureOp = op;
}

void Material::SetTextureMapMode(TextureMapMode mode)
{
	this->m_textureMapMode = mode;
}

void Material::SetTextureMapping(TextureMapping mode)
{
	this->m_textureMapping = mode;
}

ShadingMode Material::GetShadingMode() const
{
	return this->m_shadingMode;
}

TextureOp Material::GetTextureOp() const
{
	return this->m_textureOp;
}

TextureMapMode Material::GetTextureMapMode() const
{
	return this->m_textureMapMode;
}

TextureMapping Material::GetTextureMapping() const
{
	return this->m_textureMapping;
}

void Material::SetTexture(Texture* texture)
{
	if(this->m_texture)
	{
		this->m_texture ->Release();
		this->m_texture = NULL;
	}
	this->m_texture = texture;
	if(this->m_texture)
	{
		this->m_texture->AddRef();
	}
}

Texture* Material::GetTexture()
{
	return this->m_texture;
}

void Material::SetBumpMap(Texture* bumpmap)
{
	this->m_bumpmap = bumpmap;
}

Texture* Material::GetBumpMap()
{
	return this->m_bumpmap;
}

void  Material::SetName(const string& name)
{
	this->m_name = name;
}

const string&  Material::GetName()const
{
	return this->m_name;
}

Matrix4* Material::GetTexture2DTransform()
{
	return this->m_textureTransform;
}

void Material::CreateTexture2DTransform()
{
	if(!this->m_textureTransform )
	{
		this->m_textureTransform = new Matrix4();
	}
}

void Material::SetTexture2DTransform(Matrix4* texture2DTransform)
{
	this->m_textureTransform = texture2DTransform;
}

void Material::SetAmbientTexture(Texture* texture)
{
	this->m_ambientTexture = texture;
}

Texture * Material::GetAmbientTexture()
{
	return this->m_ambientTexture;
}

void Material::SetShininess(float shininess)
{
	this->m_shininess = shininess;
}

float Material::GetShininess()
{
	return this->m_shininess;
}

}

