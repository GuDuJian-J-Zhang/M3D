#ifndef M3D_UNIFORM_HELPHER_H_
#define M3D_UNIFORM_HELPHER_H_
#include "m3d/M3D.h"
#include "m3d/base/Any.h"
#include "m3d/graphics/BaseMaterial.h"
#include "m3d/renderer/gl20/ShaderProgram.h"
namespace M3D
{
	class ResourceManager;
	struct UniformMap
	{
		bool needUpdate;
		map<string, Uniform> m_uniformMap;
	};
	class UniformHelper
	{
	public:
		UniformHelper();
		~UniformHelper();
		static UniformMap* GetUniformMap(string key);
		//************************************
		// Method:    UpdateUniform
		// FullName:  M3D::UniformHelper::UpdateUniform
		// Access:    public static 
		// Returns:   void
		// Qualifier: 更新uniform数据
		// Parameter: RenderAction * action
		// Parameter: map<int
		// Parameter: GLenum> & textureBindingTargetMap
		// Parameter: ShaderProgram * shaderEffect
		// Parameter: SPHashMap & shaderUniformMap
		// Parameter: map<string
		// Parameter: Uniform> & values
		//************************************
		static void UpdateUniform(RenderAction* action, map<int, GLenum>&textureBindingTargetMap, ShaderProgram* shaderEffect, 
			SPHashMap& shaderUniformMap, map<string, Uniform>& values);
		//************************************
		// Method:    SetUniformValueToMap
		// FullName:  M3D::UniformHelper::SetUniformValueToMap
		// Access:    public static 
		// Returns:   void
		// Qualifier: 将unform变量缓存
		// Parameter: UniformMap * uniformMap
		// Parameter: const string & name
		// Parameter: string valueType
		// Parameter: const Any & value
		//************************************
		static void SetUniformValueToMap(UniformMap* uniformMap, const string& name, string valueType, const Any& value);

		//************************************
		// Method:    ParseMaterialParameters
		// FullName:  M3D::UniformHelper::ParseMaterialParameters
		// Access:    public static 
		// Returns:   void
		// Qualifier: 解析材质中参数
		// Parameter: ResourceManager * resourceManager
		// Parameter: BaseMaterial * material
		// Parameter: map<string
		// Parameter: std::pair<string
		// Parameter: string> > & parametresMap
		//************************************
		static void ParseMaterialParameters(ResourceManager*resourceManager,BaseMaterial* material , map<string, std::pair<string,string> > & parametresMap);
        
        /**
         生成一个uniform,uniform里面的Object子对象会自动添加ref

         @param valueType 类型
         @param value zhi
         @return 新生成的Uniform实例
         */
        static Uniform* MakeUniform(string valueType,  Any& value);

		//************************************
		// Method:    Clear
		// FullName:  M3D::UniformHelper::Clear
		// Access:    public static 
		// Returns:   void
		// Qualifier:	clear all uniform maps
		//************************************
		static void Clear();
	private:
		static map<string, UniformMap*> m_uniformMaps;
	};
}
#endif



