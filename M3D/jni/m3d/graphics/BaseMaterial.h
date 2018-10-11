#ifndef M3D_BASE_MATERIAL_H_
#define M3D_BASE_MATERIAL_H_

#include "m3d/base/Any.h"
#include "m3d/base/Color.h"

#include "m3d/model/Image.h"
#include "m3d/renderer/RenderContext.h"
#include "m3d/graphics/Resource.h"
#include "m3d/base/StringHash.h"
namespace M3D 
{
	class ShaderProgram;
	enum M3D_API MaterialType
	{
		/*
			材质类型，根据类型获取对应的shaderprogram
		*/
		MaterialType_Base = 0,
		MaterialType_Phong = 1,
		MaterialType_Pbr = 2,
		MaterialType_Shader=3,
		MaterialType_MatCap = 4,
		MaterialType_Inner = 5,
		MaterialType_Depth = 6,

	};
	enum M3D_API EnvTextureMappingType {
		CubeReflectionMapping,
		CubeRefractionMapping,
		EquirectangularReflectionMapping,
		EquirectangularRefractionMapping,
		SphericalReflectionMapping,
		CubeUVReflectionMapping,
		CubeUVRefractionMapping
	};
	struct M3D_API Uniform
	{
		/*
			存储Unifom变量的值
		*/
		Uniform(string type, Any value) {
			this->type = type;
			this->value = value;
		}
		Uniform() {}

		//bool operator==(Uniform& uniform) {
		//	return (this->type == uniform.type) && (this->value == uniform.value);
		//}

		string type;//类型说明
		Any value;//数据指针
	};
	class ResourceManager;
	/************************************************************************/
	/* 材质基础类。                                                                     */
	/************************************************************************/
	class M3D_API BaseMaterial :public Resource
	{
	public:
		BaseMaterial();
		BaseMaterial(BaseMaterial& org);
		~BaseMaterial();
		virtual BaseMaterial* Clone();
		/**
		*
		* @param name
		*/
		void SetName(const string& name);

		/**
		*
		* @return
		*/
		const string& GetName()const;

		//************************************
		// Method:    GetMaterialType
		// FullName:  M3D::BaseMaterial::GetMaterialType
		// Access:    virtual public 
		// Returns:   M3D::MaterialType
		// Qualifier: 材质类型
		//************************************
		virtual MaterialType GetMaterialType();
		virtual void SetMaterialType(MaterialType val);

		//************************************
		// Method:    SetUniformParameter
		// FullName:  M3D::BaseMaterial::SetUniformParameter
		// Access:    virtual public 
		// Returns:   bool
		// Qualifier: 材质中额外的参数，是材质文件中parameters对应的数据
		// Parameter: string key
		// Parameter: Uniform & unformValue
		//************************************
		virtual bool SetUniformParameter(string key, Uniform& unformValue);
		virtual void SetUniformParameter(ResourceManager* resourceManager, string key, string type, string value);
		//************************************
		// Method:    RemoveUniformParameter
		// FullName:  M3D::BaseMaterial::RemoveUniformParameter
		// Access:    virtual public 
		// Returns:   bool
		// Qualifier: 移除参数
		// Parameter: string key
		//************************************
		virtual bool RemoveUniformParameter(string key);
		virtual Uniform* GetUniformParameter(string key);

		//************************************
		// Method:    GetUnifomParameters
		// FullName:  M3D::BaseMaterial::GetUnifomParameters
		// Access:    public 
		// Returns:   map<string, Uniform>&
		// Qualifier: 获取所有参数
		//************************************
		map<string, Uniform>& GetUnifomParameters();

		//************************************
		// Method:    GetNeedsUpdateUniformParamerers
		// FullName:  M3D::BaseMaterial::GetNeedsUpdateUniformParamerers
		// Access:    public 
		// Returns:   bool
		// Qualifier: 是否更新uniform变量参数
		//************************************
		bool GetNeedsUpdateUniformParamerers();
		void SetNeedsUpdateUniformParamerers(bool value);
		//************************************
		// Method:    GetUseLight
		// FullName:  M3D::BaseMaterial::GetUseLight
		// Access:    virtual public 
		// Returns:   bool
		// Qualifier: 该材质是否接受光源
		//************************************
		virtual bool GetUseLight() const;
		virtual void SetUseLight(bool val);
		//************************************
		// Method:    NeedUpdate
		// FullName:  M3D::BaseMaterial::NeedUpdate
		// Access:    public 
		// Returns:   bool
		// Qualifier: 是否需要更新
		//************************************
		bool NeedUpdate() const;
		void NeedUpdate(bool val);
		//************************************
		// Method:    Program
		// FullName:  M3D::BaseMaterial::Program
		// Access:    public 
		// Returns:   ShaderProgram*
		// Qualifier: 材质中关联的program
		//************************************
		ShaderProgram* Program() const;
		void Program(ShaderProgram* val);
		//************************************
		// Method:    Uuid
		// FullName:  M3D::BaseMaterial::Uuid
		// Access:    public 
		// Returns:   string
		// Qualifier: 材质的UUID
		//************************************
		string Uuid() const;
		//************************************
		// Method:    Define
		// FullName:  M3D::BaseMaterial::Define
		// Access:    virtual public 
		// Returns:   string
		// Qualifier: 材质预定宏
		//************************************
		virtual string Define() ;
		virtual void Define(string val);
		//************************************
		// Method:    LightHash
		// FullName:  M3D::BaseMaterial::LightHash
		// Access:    public 
		// Returns:   string
		// Qualifier: 关联光源的key,用来判断是否需要更新
		//************************************
		string LightHash() const;
		void LightHash(string val);
		
		//************************************
		// Method:    DisplayName
		// FullName:  M3D::BaseMaterial::DisplayName
		// Access:    public 
		// Returns:   string
		// Qualifier: 显示给用户的名字
		//************************************
		string DisplayName() const;
		void DisplayName(string val);
		//************************************
		// Method:    IsGammaOutpute
		// FullName:  M3D::BaseMaterial::IsGammaOutpute
		// Access:    public 
		// Returns:   bool
		// Qualifier: 是否gamma空间输出
		//************************************
		bool IsGammaOutpute() const;
		void IsGammaOutpute(bool val);
		//************************************
		// Method:    AcceptLight
		// FullName:  M3D::BaseMaterial::AcceptLight
		// Access:    public 
		// Returns:   bool
		// Qualifier: 是否接受光源
		//************************************
		bool AcceptLight();
		void AcceptLight(bool val);
		//************************************
		// Method:    SetDefine
		// FullName:  M3D::BaseMaterial::SetDefine
		// Access:    public 
		// Returns:   void
		// Qualifier: 插入名字为key的值value
		// Parameter: string key
		// Parameter: string value
		//************************************
		void SetDefine(string key, string value);
		void ClearDefine(string key);
	protected:
		void Rename(string newName);//TODO
		MaterialType m_materialType;
		map<string, Uniform> m_uniformParameters;
		string m_name;//!<名称
		bool needsUpdateUniformParameters;
		bool m_useLight;
		bool m_needUpdate;
		ShaderProgram* m_program;
		string m_uuid;
		string m_define;
		string m_lightHash;
		string m_displayName;
		bool m_isGammaOutpute;
		bool m_acceptLight;
		map<string, string > m_definesMap;
	};
}
#endif

