#include "m3d/graphics/MatCapMaterial.h"
#include "Texture.h"
namespace M3D
{
	MatCapMaterial::MatCapMaterial()
	{
		// m_albedoMap = NULL;//!<固有色贴图

		// m_matCapMap = NULL;//!<环境贴图 

		// m_normalMap = NULL;
		//m_albedoColor = Color::WHITE;//!<albedo color
		//Opacity(1.0);
		//m_isGammaOutpute = false;
		//m_normalMapScale = Vector2(1.0, 1.0);
		//Init();
		m_materialType = MaterialType_MatCap;
	}

	MatCapMaterial::~MatCapMaterial()
	{

	}

	/*void MatCapMaterial::Init()
	{
		Color color(0.0f, 0.0f, 0.0f, 1.0f);
		Color temp = Color::WHITE;
		this->SetAmbient(color);
		this->SetDiffuse(temp);
		
		this->setSpecular(color);
	
		SetEmissive(color);
		this->m_texture = NULL;
		m_normalMap = NULL;
		m_specularMap = NULL;
		m_displacementMap = NULL;
		m_matcapMap = NULL;
		m_displacementScale = 5.0;
		m_displacementBias = 0.0;
		m_normalMapScale = Vector2(1.0, 1.0);
		this->SetDiffuseMap(NULL);
		this->SetBumpMap(NULL);
		this->m_textureTransform = NULL;
		this->m_ambientTexture = NULL;
		this->m_shininess = 0.0f;
		m_reflectiveTexture = NULL;
		needsUpdateUniformParameters = true;
	
		Opacity(1.0);
		m_isGammaOutpute = false;
		m_materialType = MaterialType_MatCap;
	}

	void MatCapMaterial::MatCapMap(Texture* val)
	{
		if (this->m_matCapMap)
		{
			m_matCapMap->Release();
			this->m_matCapMap = NULL;
		}
		this->m_matCapMap = val;
		if (this->m_matCapMap)
		{
			this->m_matCapMap->AddRef();
		}
		NeedUpdate(true);
	}

	Texture* MatCapMaterial::MatCapMap() const
	{
		return m_matCapMap;
	}

	Texture* MatCapMaterial::NormalMap() const
	{
		return m_normalMap;
	}

	void MatCapMaterial::NormalMap(Texture* val)
	{
		if (this->m_normalMap)
		{
			m_normalMap->Release();
			this->m_normalMap = NULL;
		}
		this->m_normalMap = val;
		if (this->m_normalMap)
		{
			this->m_normalMap->AddRef();
		}
		NeedUpdate(true);
	}

	void MatCapMaterial::AlbedoColor(Color val)
	{
		m_albedoColor = val;
	}

	Color& MatCapMaterial::AlbedoColor()
	{
		return m_albedoColor;
	}

	void MatCapMaterial::Define(string val)
	{
		m_define = val;
	}

	string MatCapMaterial::Define() const
	{
		return m_define;
	}

	void MatCapMaterial::NormalMapScale(Vector2 val)
	{
		m_normalMapScale = val;
	}

	Vector2& MatCapMaterial::NormalMapScale()
	{
		return m_normalMapScale;
	}

	void MatCapMaterial::Opacity(float val)
	{
		m_opacity = val;
	}

	float MatCapMaterial::Opacity() const
	{
		return m_opacity;
	}*/
}
