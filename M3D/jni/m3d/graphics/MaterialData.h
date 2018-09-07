#ifndef M3D_MATERIAL_DATA
#define M3D_MATERIAL_DATA
#include "m3d/graphics/BaseMaterial.h"
namespace M3D
{
    class PbrMaterial;
    class Material;
	class M3D_API MaterialData
	{
	public:
		MaterialData();
		~MaterialData();

		//************************************
		// Method:    DataType
		// FullName:  M3D::MaterialData::DataType
		// Access:    public 
		// Returns:   int
		// Qualifier: 对应材质类型
		//************************************
		int DataType();
		void DataType(int val);

		//************************************
		// Method:    ImagePath
		// FullName:  M3D::MaterialData::ImagePath
		// Access:    public 
		// Returns:   string
		// Qualifier:	模板材质图片路径
		//************************************
		string ImagePath();
		void ImagePath(string val);
		//************************************
		// Method:    DisplayName
		// FullName:  M3D::MaterialData::DisplayName
		// Access:    public 
		// Returns:   string
		// Qualifier:	材质数据名称，一般以MaterialTemplate为前缀
		//************************************
		string DisplayName();
		void DisplayName(string val);
	protected:
		int m_dataType;
		string m_imagePath;
		string m_displayName;

	};

	class M3D_API PhongMaterialData : public MaterialData
	{
	public:
        
		PhongMaterialData(Material* material);
		PhongMaterialData();
        
        /**
         获取环境光
         

         @return 环境光
         */
        Color AmbientColor();
        /**
         设置环境光

         @param value
         */
        void AmbientColor(Color& value);
        /**
         获取发光色
         
         @return
         */
        Color EmissiveColor();
        /**
         设置发光色
         
         @param value
         */
        void EmissiveColor(Color& value);
        /**
         获取漫反射颜色

         @return return value description
         */
        Color DiffuseColor();
        /**
         设置漫反射

         @param value
         */
        void DiffuseColor(Color& value);
        /**
         获取高光

         @return
         */
        Color SpecularColor();
        /**
         设置高光

         @param value
         */
        void SpecularColor(Color& value);
        /**
         设置光泽度

         @return
         */
        float Shininess();
        /**
         设置光泽度

         @param value
         */
        void Shininess(float value);
        /**
         获取透明度

         @return
         */
        float Opcity();
        /**
         设置透明度

         @param value 
         */
        void Opcity(float value);
    private:
        Color m_ambientColor;
        Color m_diffuseColor;
        Color m_specularColor;
        Color m_emissiveColor;
        float m_shininess;
        float m_opcity;
        
        

	};

	class M3D_API PhysicalMaterialData : public MaterialData
	{
	public:
		PhysicalMaterialData();
		PhysicalMaterialData(PbrMaterial * material);
		~PhysicalMaterialData();
        
        /**
         获取发光色

         @return
         */
        Color EmissiveColor();
        /**
         设置发光色

         @param value
         */
        void EmissiveColor(Color& value);
        
        /**
         获取颜色

         @return
         */
        Color DiffuseColor();
        /**
         设置颜色

         @param value
         */
        void DiffuseColor(Color& value);
        
        /**
         获取粗糙度

         @return
         */
        float Roughness();
        /**
         设置粗糙度

         @param value
         */
        void Roughness(float value);
        
        /**
         获取金属度

         @return
         */
        float Metallic();
        /**
         设置金属度

         @param value
         */
        void Metallic(float value);
        
        /**
         获取透明度
         
         @return
         */
        float Opcity();
        /**
         设置透明度
         
         @param value
         */
        void Opcity(float value);

	private:
        Color m_emissiveColor;
        Color m_diffuseColor;
        float m_roughtness;
        float m_metallic;
        float m_opcity;

	};

	class M3D_API ShaderMaterialData : public MaterialData
	{
	public:
		ShaderMaterialData();
		~ShaderMaterialData();

		map<string, std::pair<string, string> >& ParametersMap();
		
	protected:
		map<string, std::pair<string, string> > m_valueMap;

	};

	//晶石材质类数据，使用与BaseMaterial
	class M3D_API CrystalMaterialData :public ShaderMaterialData
	{
	public:
		CrystalMaterialData();
	};
	//玉石材质类数据
	class M3D_API JadeMaterialData :public ShaderMaterialData
	{
	public:
		JadeMaterialData();
	};
	//珍珠材质类数据
	class M3D_API PearlMaterialData:public ShaderMaterialData
	{
	public:
		PearlMaterialData();
	};
	//指环金属材质类数据
	class M3D_API RingMaterialData :public ShaderMaterialData
	{
	public:
		RingMaterialData();
	};



}
#endif//M3D_MATERIAL_DATA
