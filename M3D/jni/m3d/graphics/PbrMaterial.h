#ifndef M3D_PBR_MATERIAL_H_
#define M3D_PBR_MATERIAL_H_
#include "m3d/graphics/InnerMaterial.h"

namespace M3D
{
	/************************************************************************/
	/* 基于物理的材质。                                                                     */
	/************************************************************************/
	class Texture;
	class M3D_API PbrMaterial :
		public InnerMaterial
	{
	public:
		PbrMaterial();
		PbrMaterial(PbrMaterial& org);
		~PbrMaterial();
		virtual BaseMaterial* Clone();
		//************************************
		// Method:    AlbedoMap
		// FullName:  M3D::PbrMaterial::AlbedoMap
		// Access:    public 
		// Returns:   Texture*
		// Qualifier: 漫反射贴图
		//************************************
		Texture* AlbedoMap() const;
		void AlbedoMap(Texture* val);
		//************************************
		// Method:    EnvMap
		// FullName:  M3D::PbrMaterial::EnvMap
		// Access:    public 
		// Returns:   Texture*
		// Qualifier: 环境贴图
		//************************************
		Texture* EnvMap() const;
		void EnvMap(Texture* val);
		//************************************
		// Method:    EnvIrradianceMap
		// FullName:  M3D::PbrMaterial::EnvIrradianceMap
		// Access:    public 
		// Returns:   Texture*
		// Qualifier: 环境辐照度贴图
		//************************************
		Texture* EnvIrradianceMap() const;
		void EnvIrradianceMap(Texture* val);
		//************************************
		// Method:    EmissiveMap
		// FullName:  M3D::PbrMaterial::EmissiveMap
		// Access:    public 
		// Returns:   Texture*
		// Qualifier: 发光贴图
		//************************************
		Texture* EmissiveMap() const;
		void EmissiveMap(Texture* val);
		//************************************
		// Method:    AmbientOcclusiontMap
		// FullName:  M3D::PbrMaterial::AmbientOcclusiontMap
		// Access:    public 
		// Returns:   Texture*
		// Qualifier: 环境光遮蔽贴图
		//************************************
		Texture* AmbientOcclusiontMap() const;
		void AmbientOcclusiontMap(Texture* val);
		//************************************
		// Method:    NormalMap
		// FullName:  M3D::PbrMaterial::NormalMap
		// Access:    public 
		// Returns:   Texture*
		// Qualifier: 法线贴图
		//************************************
		Texture* NormalMap() const;
		void NormalMap(Texture* val);
		//************************************
		// Method:    EnvTextureMapping
		// FullName:  M3D::PbrMaterial::EnvTextureMapping
		// Access:    public 
		// Returns:   M3D::EnvTextureMappingType
		// Qualifier: 环境反射贴图类型
		//************************************
		M3D::EnvTextureMappingType EnvTextureMapping() const;
		void EnvTextureMapping(M3D::EnvTextureMappingType val);
		//************************************
		// Method:    MetalnessRoughnessMap
		// FullName:  M3D::PbrMaterial::MetalnessRoughnessMap
		// Access:    public 
		// Returns:   Texture*
		// Qualifier: 粗糙度金属度贴图
		//************************************
		Texture* MetalnessRoughnessMap() const;
		void MetalnessRoughnessMap(Texture* val);
		//************************************
		// Method:    RougthnessFactor
		// FullName:  M3D::PbrMaterial::RougthnessFactor
		// Access:    public 
		// Returns:   float
		// Qualifier: 粗糙度参数
		//************************************
		float RougthnessFactor() const;
		void RougthnessFactor(float val);
		//************************************
		// Method:    MetalnessFactor
		// FullName:  M3D::PbrMaterial::MetalnessFactor
		// Access:    public 
		// Returns:   float
		// Qualifier: 金属度参数
		//************************************
		float MetalnessFactor() const;
		void MetalnessFactor(float val);
		//************************************
		// Method:    AlbedoColor
		// FullName:  M3D::PbrMaterial::AlbedoColor
		// Access:    public 
		// Returns:   Color&
		// Qualifier: 漫反射颜色
		//************************************
		Color& AlbedoColor();
		void AlbedoColor(Color val);
		virtual string Define() ;
		virtual void Define(string val);	
		//************************************
		// Method:    EmissiveColor
		// FullName:  M3D::PbrMaterial::EmissiveColor
		// Access:    public 
		// Returns:   Color&
		// Qualifier: 发光颜色
		//************************************
		Color& EmissiveColor();
		void EmissiveColor(Color val);
		//************************************
		// Method:    AoMapIntensity
		// FullName:  M3D::PbrMaterial::AoMapIntensity
		// Access:    public 
		// Returns:   float
		// Qualifier: 环境光遮蔽强度
		//************************************
		float AoMapIntensity() const;
		void AoMapIntensity(float val);
		//************************************
		// Method:    NormalMapScale
		// FullName:  M3D::PbrMaterial::NormalMapScale
		// Access:    public 
		// Returns:   Vector2&
		// Qualifier: 法线贴图缩放
		//************************************
		Vector2& NormalMapScale();
		void NormalMapScale(Vector2 val);
		//************************************
		// Method:    DisplacementMap
		// FullName:  M3D::PbrMaterial::DisplacementMap
		// Access:    public 
		// Returns:   Texture*
		// Qualifier: 高度图
		//************************************
		Texture* DisplacementMap();
		void DisplacementMap(Texture* val);
		//************************************
		// Method:    DisplacementScale
		// FullName:  M3D::PbrMaterial::DisplacementScale
		// Access:    public 
		// Returns:   float
		// Qualifier: 高度图参数
		//************************************
		float DisplacementScale() const;
		void DisplacementScale(float val);
		float DisplacementBias() const;
		void DisplacementBias(float val);
		//************************************
		// Method:    EnvMapIntensity
		// FullName:  M3D::PbrMaterial::EnvMapIntensity
		// Access:    public 
		// Returns:   float
		// Qualifier: 环境光强度
		//************************************
		float EnvMapIntensity() const;
		void EnvMapIntensity(float val);
		//************************************
		// Method:    ClearCoat
		// FullName:  M3D::PbrMaterial::ClearCoat
		// Access:    public 
		// Returns:   float
		// Qualifier: 清漆效果范围 0 -1 此版本为简化版
		//************************************
		float ClearCoat() const;
		void ClearCoat(float val);
		//************************************
		// Method:    ClearCoatRoughness
		// FullName:  M3D::PbrMaterial::ClearCoatRoughness
		// Access:    public 
		// Returns:   float
		// Qualifier: 清漆粗糙度 0 - 1
		//************************************
		float ClearCoatRoughness() const;
		void ClearCoatRoughness(float val);
		//************************************
		// Method:    UseClearCoat
		// FullName:  M3D::PbrMaterial::UseClearCoat
		// Access:    public 
		// Returns:   bool
		// Qualifier: 是否使用清漆效果
		//************************************
		bool UseClearCoat() const;
		void UseClearCoat(bool val);
		//************************************
		// Method:    Opacity
		// FullName:  M3D::PbrMaterial::Opacity
		// Access:    public 
		// Returns:   float
		// Qualifier: 设置透明度
		//************************************
		float Opacity() const;
		void Opacity(float val);
	private:

		Texture* m_albedoMap;//!<固有色贴图
		//!<粗糙度贴图
		Texture* m_metalnessRoughnessMap;//!<金属度贴图
		Texture* m_envMap;//!<环境贴图 specularMap
		Texture* m_envIrradianceMap;//!<irradianceMap 辐照度贴图
		Texture* m_emissiveMap;
		Texture* m_ambientOcclusiontMap;
		float m_aoMapIntensity;
		Texture* m_normalMap;
		Texture* m_displacementMap;
		float m_displacementScale;
		float m_displacementBias;
		
		Color m_albedoColor;//!<albedo color
		Color m_emissiveColor;
		float m_rougthnessFactor;
		float m_metalnessFactor;
		Vector2 m_normalMapScale;
		EnvTextureMappingType m_envTextureMapping;

		float m_envMapIntensity;

		float m_clearCoat;
		float m_clearCoatRoughness;
		bool m_isUseClearCoat;

		float m_opacity;

	};
}
#endif

