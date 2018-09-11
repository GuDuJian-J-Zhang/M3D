#ifndef M3D_MATERIAL_TEMPLATE_MANAGER
#define M3D_MATERIAL_TEMPLATE_MANAGER
#include "m3d/graphics/MaterialData.h"
namespace M3D
{
	class ResourceManager;
	
	class MaterialTemplateManager
	{
	public:
		MaterialTemplateManager(ResourceManager* resourceManager);
		~MaterialTemplateManager();
		//************************************
		// Method:    Load
		// FullName:  M3D::MaterialTemplateManager::Load
		// Access:    public 
		// Returns:   bool
		// Qualifier: 加载模板文件，并生成模板数据对象 load template file ,and generate MaterialData instance
		// Parameter: string templatePath
		//************************************
		bool Load(string templatePath);
		//************************************
		// Method:    Write
		// FullName:  M3D::MaterialTemplateManager::Write
		// Access:    public 
		// Returns:   bool
		// Qualifier: 写入模板文件 write template file
		// Parameter: string templatePath
		//************************************
		bool Write(string templatePath);
		//************************************
		// Method:    Clear
		// FullName:  M3D::MaterialTemplateManager::Clear
		// Access:    public 
		// Returns:   void
		// Qualifier: 清理所有数据
		//************************************
		void Clear();
		//************************************
		// Method:    GenerateMaterialFromData
		// FullName:  M3D::MaterialTemplateManager::GenerateMaterialFromData
		// Access:    public 
		// Returns:   BaseMaterial*
		// Qualifier: 通过模板数据对象来获取对应的材质
		// Parameter: MaterialData * data
		// Parameter: int mode  0 : if already exist name material,then return it; 
		//						1 : if already exist name material,create a new one,then return it.
		//						default is 0.
		//************************************
		BaseMaterial* GenerateMaterialFromData(MaterialData* data, int mode = 0);
		//************************************
		// Method:    GetMaterialFromTemplateByName
		// FullName:  M3D::MaterialTemplateManager::GetMaterialFromTemplateByName
		// Access:    public 
		// Returns:   BaseMaterial*
		// Qualifier: 通过模板名称来生成对应的材质
		// Parameter: string name
		// Parameter: int mode  0 : if already exist name material,then return it; 
		//						1 : if already exist name material,create a new one,then return it.
		//						default is 0.
		//************************************
		BaseMaterial* GetMaterialFromTemplateByName(string name, int mode = 0);
		//************************************
		// Method:    GetAllMaterialData
		// FullName:  M3D::MaterialTemplateManager::GetAllMaterialData
		// Access:    public 
		// Returns:   map<string, MaterialData*>&
		// Qualifier: 获取所有的模板材质数据
		//************************************
		map<string, MaterialData*>& GetAllMaterialData();
	//private:
	//	void ParaseMaterial();
	private:
		ResourceManager* m_resourceManager;
		map<string, MaterialData*> m_materialDataMap;
	};
}

#endif //M3D_MATERIAL_TEMPLATE_MANAGER
