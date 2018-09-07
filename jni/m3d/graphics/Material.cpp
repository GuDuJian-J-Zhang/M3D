#include "m3d/graphics/Material.h"

#include "m3d/ResourceManager.h"
#include "m3d/graphics/Texture2D.h"

namespace M3D
{

Material::Material():InnerMaterial()
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

		if (this->m_ambientTexture)
		{
			this->m_ambientTexture->Release();
			this->m_ambientTexture = NULL;
		}



		if (this->m_displacementMap)
		{
			this->m_displacementMap->Release();
			this->m_displacementMap = NULL;
		}
		if (this->m_emissiveMap)
		{
			this->m_emissiveMap->Release();
			this->m_emissiveMap = NULL;
		}
		if (this->m_matcapMap)
		{
			this->m_matcapMap->Release();
			this->m_matcapMap = NULL;
		}
		if (this->m_reflectiveTexture)
		{
			this->m_reflectiveTexture->Release();
			this->m_reflectiveTexture = NULL;
		}

		if (this->m_specularMap)
		{
			this->m_specularMap->Release();
			this->m_specularMap = NULL;
		}

		if (this->m_normalMap)
		{
			this->m_normalMap->Release();
			this->m_normalMap = NULL;
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
    Color temp = Color(0.067,0.067,0.067);
	this->setSpecular(temp);
    temp =Color(0, 0, 0);
	SetEmissive(temp);
	this->m_texture = NULL;
	m_normalMap = NULL;
	m_specularMap = NULL;
	m_displacementMap = NULL;
	m_matcapMap = NULL;
	m_displacementScale = 5.0;
	m_displacementBias = 0.0;
	m_normalMapScale = Vector2(2.0,2.0);
	this->SetDiffuseMap(NULL);
	this->SetBumpMap(NULL);
	this->m_textureTransform = NULL;
	this->m_ambientTexture = NULL;
	this->m_shininess = 30.0f;
	m_reflectiveTexture = NULL;
	needsUpdateUniformParameters = true;
	m_materialType = MaterialType_Phong;
	Opacity(1.0);
	m_isGammaOutpute = false;
	m_emissiveMap = NULL;
}


void Material::SetAmbient( Color& ambient)
{
	this->m_ambient = ambient;
}
void Material::SetDiffuse( Color& diffuse)
{
	this->m_Diffuse = diffuse;
}

void Material::setSpecular( Color& specular)
{
	this->m_Specular = specular;
}

void Material::SetEmissive(Color& emissive)
{
	m_emissive = emissive;
}

Color& Material::GetAmbient()
{
	return this->m_ambient;
}

 Color& Material::GetDiffuse() 
{
	return this->m_Diffuse;
}

 Color& Material::GetSpecular() 
{
	return this->m_Specular;
}

 Color& Material::GetEmissive()
 {
	 return m_emissive;
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

void Material::SetDiffuseMap(Texture* texture)
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
	NeedUpdate(true);
}

Texture* Material::GetDiffuseMap()
{
	return this->m_texture;
}

void Material::SetSpecularMap(Texture * texture)
{
	if (this->m_specularMap)
	{
		this->m_specularMap->Release();
		this->m_specularMap = NULL;
	}
	this->m_specularMap = texture;
	if (this->m_specularMap)
	{
		
		this->m_specularMap->AddRef();
	}
	NeedUpdate(true);
}

Texture * Material::GetSpecularMap()
{
	return m_specularMap;
}

void Material::SetEmissiveMap(Texture * texture)
{
	if (this->m_emissiveMap)
	{
		this->m_emissiveMap->Release();
		this->m_emissiveMap = NULL;
	}
	this->m_emissiveMap = texture;
	if (this->m_emissiveMap)
	{
		
		this->m_emissiveMap->AddRef();
	}
	NeedUpdate(true);
}

Texture * Material::GetEmissiveMap()
{
	return m_emissiveMap;
}

void Material::SetNormalMap(Texture * texture)
{
	if (this->m_normalMap)
	{
		this->m_normalMap->Release();
		this->m_normalMap = NULL;
	}
	this->m_normalMap = texture;
	if (this->m_normalMap) 
	{
		
		this->m_normalMap->AddRef();
	}
	NeedUpdate(true);
}

Texture * Material::GetNormalMap()
{
	return m_normalMap;
}

void Material::SetEvnMap(Texture * texture)
{
}

Texture * Material::GetEvnMap()
{
	return nullptr;
}

void Material::SetBumpMap(Texture* bumpmap)
{
	this->m_bumpmap = bumpmap;
}

Texture* Material::GetBumpMap()
{
	return this->m_bumpmap;
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

void Material::SetAmbientMap(Texture* texture)
{
	this->m_ambientTexture = texture;
}

Texture * Material::GetAmbientMap()
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

Texture* Material::GetReflectiveTextureTexture()
{
	return this->m_reflectiveTexture;
}

void Material::SetReflectiveTexture(Texture* texture)
{
	this->m_reflectiveTexture = texture;
}



void Material::DisplacementMap(Texture* val)
{
	if (this->m_displacementMap)
	{
		this->m_displacementMap->Release();
		this->m_displacementMap = NULL;
	}
	this->m_displacementMap = val;
	if (this->m_displacementMap)
	{
		
		this->m_displacementMap->AddRef();
	}
	NeedUpdate(true);
}

Texture * Material::MatcapMap()
{
	return m_matcapMap;
}

void Material::MatcapMap(Texture * val)
{
	if (this->m_matcapMap)
	{
		this->m_matcapMap->Release();
		this->m_matcapMap = NULL;
	}
	this->m_matcapMap = val;
	if (this->m_matcapMap)
	{
		
		this->m_matcapMap->AddRef();
	}
	NeedUpdate(true);
}

}

