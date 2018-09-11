#include "m3d/graphics/PbrMaterial.h"
#include "Texture.h"
namespace M3D
{
	PbrMaterial::PbrMaterial():InnerMaterial()
	{
		m_materialType = MaterialType_Pbr;
		m_rougthnessFactor = 0.5f;
		m_metalnessFactor = 0.04;
		m_aoMapIntensity = 1.0;
		m_emissiveColor = Color(0, 0, 0);
		m_normalMapScale = Vector2(2.0, 2.0);
		m_displacementMap = NULL;
		m_displacementScale = 5.0;
		m_displacementBias = 0.0;
		m_envMapIntensity = 1.0;
		m_clearCoat = 0.9f;
		m_clearCoatRoughness = 0.04f;
		m_isUseClearCoat = true;
		m_albedoMap = NULL;//!<固有色贴图
							 //!<粗糙度贴图
		m_metalnessRoughnessMap = NULL;//!<金属度贴图
		m_envMap = NULL;//!<环境贴图 specularMap
		m_envIrradianceMap = NULL;//!<irradianceMap 辐照度贴图
		m_emissiveMap = NULL;
		m_ambientOcclusiontMap = NULL;
		m_normalMap = NULL;
		//m_define = "#define PHYSICALLY_CORRECT_LIGHTSS\n";
		SetDefine("PHYSICALLY_CORRECT_LIGHTSS", "#define PHYSICALLY_CORRECT_LIGHTSS\n");
		m_envTextureMapping = CubeReflectionMapping;
		Opacity(1.0);
		m_isGammaOutpute = true;
		UseClearCoat(false);
	}


	PbrMaterial::~PbrMaterial()
	{
		if (this->m_resourceMgr)
		{
			//在析构时，将material从资源管理器中移除。此对象真正点不存在了
			//this->m_resourceMgr->RemoveMaterial(m_name);

			if (this->m_albedoMap)
			{
				this->m_albedoMap->Release();
				this->m_albedoMap = NULL;
			}

			if (this->m_ambientOcclusiontMap)
			{
				this->m_ambientOcclusiontMap->Release();
				this->m_ambientOcclusiontMap = NULL;
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
			if (this->m_envIrradianceMap)
			{
				this->m_envIrradianceMap->Release();
				this->m_envIrradianceMap = NULL;
			}
			if (this->m_envMap)
			{
				this->m_envMap->Release();
				this->m_envMap = NULL;
			}

			if (this->m_metalnessRoughnessMap)
			{
				this->m_metalnessRoughnessMap->Release();
				this->m_metalnessRoughnessMap = NULL;
			}

			if (this->m_normalMap)
			{
				this->m_normalMap->Release();
				this->m_normalMap = NULL;
			}
			this->m_resourceMgr = NULL;
		}
	}

	void PbrMaterial::AlbedoMap(Texture* val)
	{
		if (m_albedoMap != val)
		{
			if (this->m_albedoMap)
			{
				m_albedoMap->Release();
				this->m_albedoMap = NULL;
			}
			this->m_albedoMap = val;
			if (this->m_albedoMap)
			{
				m_albedoMap->IsGammaInput(true);

				this->m_albedoMap->AddRef();
			}
			NeedUpdate(true);
		}

	}

	Texture* PbrMaterial::AlbedoMap() const
	{
		return m_albedoMap;
	}

	void PbrMaterial::EnvMap(Texture* val)
	{
		if (m_envMap != val)
		{
			if (m_envMap)
			{
				m_envMap->Release();
				this->m_envMap = NULL;
			}
			m_envMap = val;
			if (this->m_envMap)
			{
				m_envMap->IsGammaInput(true);

				this->m_envMap->AddRef();
			}
			NeedUpdate(true);
		}		
	}

	Texture* PbrMaterial::EnvMap() const
	{
		return m_envMap;
	}

	void PbrMaterial::EnvIrradianceMap(Texture* val)
	{
		if (m_envIrradianceMap != val)
		{
			if (m_envIrradianceMap)
			{
				m_envIrradianceMap->Release();
				this->m_envIrradianceMap = NULL;
			}
			m_envIrradianceMap = val;
			if (this->m_envIrradianceMap)
			{
				m_envIrradianceMap->IsGammaInput(true);

				this->m_envIrradianceMap->AddRef();
			}
			NeedUpdate(true);
		}
	}

	Texture* PbrMaterial::EnvIrradianceMap() const
	{
		return m_envIrradianceMap;
	}

	void PbrMaterial::EmissiveMap(Texture* val)
	{
		if (m_emissiveMap != val)
		{
			if (this->m_emissiveMap)
			{
				m_emissiveMap->Release();
				this->m_emissiveMap = NULL;
			}
			this->m_emissiveMap = val;
			if (this->m_emissiveMap)
			{

				this->m_emissiveMap->AddRef();
			}
			NeedUpdate(true);
		}

	}

	Texture* PbrMaterial::EmissiveMap() const
	{
		return m_emissiveMap;
	}

	void PbrMaterial::AmbientOcclusiontMap(Texture* val)
	{
		if (m_ambientOcclusiontMap != val)
		{
			if (this->m_ambientOcclusiontMap)
			{
				m_ambientOcclusiontMap->Release();
				this->m_ambientOcclusiontMap = NULL;
			}
			this->m_ambientOcclusiontMap = val;
			if (this->m_ambientOcclusiontMap)
			{

				this->m_ambientOcclusiontMap->AddRef();
			}
			NeedUpdate(true);
		}

	}

	Texture* PbrMaterial::AmbientOcclusiontMap() const
	{
		return m_ambientOcclusiontMap;
	}

	void PbrMaterial::NormalMap(Texture* val)
	{
		if (m_normalMap != val)
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

	}

	Texture* PbrMaterial::NormalMap() const
	{
		return m_normalMap;
	}

	void PbrMaterial::EnvTextureMapping(M3D::EnvTextureMappingType val)
	{
		m_envTextureMapping = val;
	}

	M3D::EnvTextureMappingType PbrMaterial::EnvTextureMapping() const
	{
		return m_envTextureMapping;
	}

	void PbrMaterial::MetalnessRoughnessMap(Texture* val)
	{
		if (m_metalnessRoughnessMap != val)
		{
			if (this->m_metalnessRoughnessMap)
			{
				m_metalnessRoughnessMap->Release();
				this->m_metalnessRoughnessMap = NULL;
			}
			this->m_metalnessRoughnessMap = val;
			if (this->m_metalnessRoughnessMap)
			{

				this->m_metalnessRoughnessMap->AddRef();
			}
			NeedUpdate(true);
		}

	}

	Texture* PbrMaterial::MetalnessRoughnessMap() const
	{
		return m_metalnessRoughnessMap;
	}

	void PbrMaterial::RougthnessFactor(float val)
	{
		m_rougthnessFactor = val;
	}

	float PbrMaterial::RougthnessFactor() const
	{
		return m_rougthnessFactor;
	}

	void PbrMaterial::MetalnessFactor(float val)
	{
		m_metalnessFactor = val;
	}

	float PbrMaterial::MetalnessFactor() const
	{
		return m_metalnessFactor;
	}

	void PbrMaterial::AlbedoColor(Color val)
	{
		m_albedoColor = val;
	}

	Color& PbrMaterial::AlbedoColor()
	{
		return m_albedoColor;
	}

	void PbrMaterial::Define(string val)
	{
		m_define = val;
	}

	string PbrMaterial::Define() 
	{
		m_define = "";
		map<string,string>::const_iterator it = m_definesMap.begin();
		for (;it != m_definesMap.end(); ++it)
		{
			m_define += it->second;
		}
		return m_define;
	}

	void PbrMaterial::EmissiveColor(Color val)
	{
		m_emissiveColor = val;
	}

	Color& PbrMaterial::EmissiveColor()
	{
		return m_emissiveColor;
	}

	void PbrMaterial::AoMapIntensity(float val)
	{
		m_aoMapIntensity = val;
	}

	float PbrMaterial::AoMapIntensity() const
	{
		return m_aoMapIntensity;
	}

	void PbrMaterial::NormalMapScale(Vector2 val)
	{
		m_normalMapScale = val;
	}

	Vector2& PbrMaterial::NormalMapScale()
	{
		return m_normalMapScale;
	}

	void PbrMaterial::DisplacementMap(Texture* val)
	{
		if (m_displacementMap != val)
		{
			if (this->m_displacementMap)
			{
				m_displacementMap->Release();
				this->m_displacementMap = NULL;
			}
			this->m_displacementMap = val;
			if (this->m_displacementMap)
			{

				this->m_displacementMap->AddRef();
			}
			NeedUpdate(true);
		}

	}

	Texture* PbrMaterial::DisplacementMap()
	{
		return m_displacementMap;
	}

	void PbrMaterial::DisplacementScale(float val)
	{
		m_displacementScale = val;
	}

	float PbrMaterial::DisplacementScale() const
	{
		return m_displacementScale;
	}

	void PbrMaterial::DisplacementBias(float val)
	{
		m_displacementBias = val;
	}

	float PbrMaterial::DisplacementBias() const
	{
		return m_displacementBias;
	}

	void PbrMaterial::EnvMapIntensity(float val)
	{
		m_envMapIntensity = val;
	}

	float PbrMaterial::EnvMapIntensity() const
	{
		return m_envMapIntensity;
	}

	void PbrMaterial::ClearCoat(float val)
	{
		m_clearCoat = val;
	}

	float PbrMaterial::ClearCoat() const
	{
		return m_clearCoat;
	}

	void PbrMaterial::ClearCoatRoughness(float val)
	{
		m_clearCoatRoughness = val;
	}

	float PbrMaterial::ClearCoatRoughness() const
	{
		return m_clearCoatRoughness;
	}

	void PbrMaterial::UseClearCoat(bool val)
	{
		m_isUseClearCoat = val;
		if (!m_isUseClearCoat)
		{			
			SetDefine("STANDARD", "#define STANDARD\n");
		}
		else
		{
			SetDefine("STANDARD", "");
		}
		NeedUpdate(true);
	}

	bool PbrMaterial::UseClearCoat() const
	{
		return m_isUseClearCoat;
	}

	void PbrMaterial::Opacity(float val)
	{
		m_opacity = val;
	}

	float PbrMaterial::Opacity() const
	{
		return m_opacity;
	}

}
