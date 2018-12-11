#include "m3d/graphics/Material.h"

#include "m3d/ResourceManager.h"
#include "m3d/graphics/Texture2D.h"

namespace M3D
{

Material::Material():InnerMaterial()
{
	this->Init();
}
Material::Material(Material& org):InnerMaterial(org)
{
	this->SetAmbient(org.m_ambient);
	this->SetDiffuse(org.m_Diffuse);
	this->setSpecular(org.m_Specular);

	this->SetEmissive(org.m_emissive);
	this->m_texture = new Texture();
	if (org.m_texture==NULL)
	{
		if (this->m_texture)
		{
			this->m_texture->Release();
			this->m_texture = NULL;
		}
	} 
	else
	{
		*m_texture = *(org.m_texture);
		m_texture->AddRef();
	}


	m_normalMap = new Texture();
	if (org.m_normalMap==NULL)
	{
		if (m_normalMap==NULL)
		{
			m_normalMap->Release();
			m_normalMap = NULL;
		}
	} 
	else
	{	
		*m_normalMap = *(org.m_normalMap);
		m_normalMap->AddRef();
	}


	m_specularMap = new Texture();
	if (org.m_specularMap==NULL)
	{
		m_specularMap->Release();
		m_specularMap = NULL;
	} 
	else
	{
		*m_specularMap = *(org.m_specularMap);
		m_specularMap->AddRef();
	}


	m_displacementMap = new Texture();
	if (org.m_displacementMap==NULL)
	{
		m_displacementMap->Release();
		m_displacementMap = NULL;
	} 
	else
	{
		*m_displacementMap = *(org.m_displacementMap);
		m_displacementMap->AddRef();
	}


	m_matcapMap = new Texture();
	if (org.m_matcapMap==NULL)
	{
		if (m_matcapMap)
		{
			m_matcapMap->Release();
			m_matcapMap = NULL;
		}
	} 
	else
	{
		*m_matcapMap = *(org.m_matcapMap);
		m_matcapMap->AddRef();
	}


	m_displacementScale = org.m_displacementScale;
	m_displacementBias = org.m_displacementBias;
	m_normalMapScale = org.m_normalMapScale;
	
	m_bumpmap = new Texture();
	if (org.m_bumpmap==NULL)
	{
		if (m_bumpmap)
		{
			m_bumpmap->Release();
			m_bumpmap = NULL;
		}
	}
	else
	{
		*m_bumpmap = *(org.m_bumpmap);
		m_bumpmap->AddRef();
	}


	
	this->m_textureTransform = new Matrix4();
	if (this->m_textureTransform)
	{
		if (org.m_textureTransform == NULL)
		{
			m_textureTransform = NULL;
		}
		else
		{
			*m_textureTransform = *(org.m_textureTransform);
		}
	}
	
	this->m_ambientTexture = new Texture();
	if (org.m_ambientTexture==NULL)
	{
		if (m_ambientTexture)
		{
			m_ambientTexture->Release();
			m_ambientTexture = NULL;
		}
	} 
	else
	{
		*m_ambientTexture = *(org.m_ambientTexture);
		m_ambientTexture->AddRef();
	}


	this->m_shininess = org.m_shininess;
	m_reflectiveTexture = new Texture();
	if (org.m_reflectiveTexture==NULL)
	{
		if (m_reflectiveTexture)
		{
			m_reflectiveTexture->Release();
			m_reflectiveTexture = NULL;
		}
	} 
	else
	{
		*m_reflectiveTexture = *(org.m_reflectiveTexture);
		m_reflectiveTexture->AddRef();
	}


	needsUpdateUniformParameters = org.needsUpdateUniformParameters;
	m_materialType = org.m_materialType;
	m_opacity = org.m_opacity;
	m_isGammaOutpute = org.m_isGammaOutpute;
	m_emissiveMap = new Texture();
	if (org.m_emissiveMap==NULL)
	{
		m_emissiveMap->Release();
		m_emissiveMap = NULL;
	} 
	else
	{	*(m_emissiveMap) = *(org.m_emissiveMap);
		m_emissiveMap->AddRef();
	}

}
BaseMaterial* Material::Clone()
{
	return new Material(*this);
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

bool Material::Compare(BaseMaterial * sBaseMaterial)
{
	InnerMaterial::Compare(sBaseMaterial);
	Material * smaterial = (Material *)sBaseMaterial;

	
	if (!BaseMaterial::Compare(smaterial))
	{
		return false;
	}

	if (!this->GetDiffuse().Equals(smaterial->GetDiffuse()) )
	{
		return false;
	}
	
	if (!this->GetEmissive().Equals(smaterial->GetEmissive()))
	{
		return false;
	}

	if (!this->GetAmbient().Equals(smaterial->GetAmbient()))
	{
		return false;
	}

	if (!this->GetSpecular().Equals(smaterial->GetSpecular()))
	{
		return false;
	}

	if (!this->GetShadingMode()==smaterial->GetShadingMode())
	{
		return false;
	}

	if (!this->GetTextureOp()==smaterial->GetTextureOp())
	{
		return false;
	}

	if (!this->GetTextureMapMode()==smaterial->GetTextureMapMode())
	{
		return false;
	}

	if (!this->GetTextureMapping()==smaterial->GetTextureMapping())
	{
		return false;
	}

	if (this->GetDiffuseMap()!=NULL && smaterial->GetDiffuseMap()!=NULL)
	{
		if (!this->GetDiffuseMap()->Equals(smaterial->GetDiffuseMap()))
		{
			return false;
		}
		
	}


	if (this->GetAmbientMap()!=NULL &&smaterial->GetAmbientMap()!=NULL)
	{
		if (!this->GetAmbientMap()->Equals(smaterial->GetAmbientMap()))
		{
			return false;
		}
		
	}

	if (this->GetBumpMap()!=NULL && smaterial->GetBumpMap()!=NULL)
	{
		if (!this->GetBumpMap()->Equals(smaterial->GetBumpMap()))
		{
			return false;
		}
		
	}

	if (this->GetNormalMap()!=NULL && smaterial->GetNormalMap()!=NULL)
	{
		if (!this->GetNormalMap()->Equals(smaterial->GetNormalMap()))
		{
			return false;
		}
	}

	if (this->GetSpecularMap()!=NULL && smaterial->GetSpecularMap()!=NULL)
	{
		if (!this->GetSpecularMap()->Equals(smaterial->GetSpecularMap()))
		{
			return false;
		}
	}

	if (this->GetReflectiveTextureTexture()!=NULL && smaterial->GetReflectiveTextureTexture()!=NULL)
	{
		if (!this->GetReflectiveTextureTexture()->Equals(smaterial->GetReflectiveTextureTexture()))
		{
			return false;
		}
	}


	if (this->GetTexture2DTransform()!=NULL && *smaterial->GetTexture2DTransform()!=NULL)
	{
		if (!this->GetTexture2DTransform()->Equals(*smaterial->GetTexture2DTransform()))
		{
			return false;
		}
	}


	if (!Equals(this->GetShininess(),smaterial->GetShininess()))
	{
		return false;
	}

	if (this->MatcapMap()!=NULL && smaterial->MatcapMap()!=NULL)
	{
		if (!this->MatcapMap()->Equals(smaterial->MatcapMap()))
		{
			return false;
		}
	}


	if (this->GetEmissiveMap()!=NULL && smaterial->GetEmissiveMap()!=NULL)
	{
		if (!this->GetEmissiveMap()->Equals(smaterial->GetEmissiveMap()))
		{
			return false;
		}
	}

	return true;
}

}

