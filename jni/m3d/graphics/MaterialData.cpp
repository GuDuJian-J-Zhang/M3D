#include "m3d/graphics/MaterialData.h"
#include "m3d/base/Color.h"
#include "m3d/graphics/PbrMaterial.h"
#include "m3d/graphics/Material.h"
namespace M3D
{
	MaterialData::MaterialData()
	{
		m_dataType = MaterialType_Base;
		m_imagePath = "";
		m_displayName = "default";
	}

	MaterialData::~MaterialData()
	{
	}

	void MaterialData::DataType(int val)
	{
		m_dataType = val;
	}

	int MaterialData::DataType()
	{
		return m_dataType;
	}

	void MaterialData::ImagePath(string val)
	{
		m_imagePath = val;
	}

	string MaterialData::ImagePath()
	{
		return m_imagePath;
	}

	void MaterialData::DisplayName(string val)
	{
		m_displayName = val;
	}

	string MaterialData::DisplayName()
	{
		return m_displayName;
	}

	PhongMaterialData::PhongMaterialData(Material* material) :
		MaterialData()
	{
        DataType(MaterialType::MaterialType_Phong);
	}

	PhongMaterialData::PhongMaterialData() :
		MaterialData()
	{
        DataType(MaterialType::MaterialType_Phong);
    }
    
    M3D::Color PhongMaterialData::AmbientColor() { 
        return m_ambientColor;
    }
    
    void PhongMaterialData::AmbientColor(M3D::Color &value) { 
        m_ambientColor = value;
    }
    
    float PhongMaterialData::Opcity() { 
        return m_opcity;
    }
    
    
    void PhongMaterialData::Shininess(float value) { 
        m_shininess = value;
    }
    
    
    float PhongMaterialData::Shininess() { 
        return m_shininess;
    }
    
    
    void PhongMaterialData::SpecularColor(M3D::Color &value) { 
        m_specularColor = value;
    }
    
    
    M3D::Color PhongMaterialData::SpecularColor() { 
        return m_specularColor;
    }
    
    
    void PhongMaterialData::DiffuseColor(M3D::Color &value) { 
        m_diffuseColor = value;
    }
    
    
    M3D::Color PhongMaterialData::DiffuseColor() { 
        return m_diffuseColor;
    }
    
    void PhongMaterialData::Opcity(float value) { 
        m_opcity = value;
    }
    
    void PhongMaterialData::EmissiveColor(M3D::Color &value) { 
        m_emissiveColor = value;
    }
    
    
    M3D::Color PhongMaterialData::EmissiveColor() { 
        return m_emissiveColor;
    }
    
    
    
    
    

	PhysicalMaterialData::PhysicalMaterialData(PbrMaterial* material) :
		MaterialData()
	{
        DataType(MaterialType::MaterialType_Pbr);
	}

	PhysicalMaterialData::PhysicalMaterialData():
		MaterialData()
	{
        DataType(MaterialType::MaterialType_Pbr);
	}

	PhysicalMaterialData::~PhysicalMaterialData()
	{

    }
        
        
        void PhysicalMaterialData::Opcity(float value) { 
            m_opcity = value;
        }
        
        
        float PhysicalMaterialData::Opcity() { 
            return m_opcity;
        }
        
        
        float PhysicalMaterialData::Metallic() { 
            return m_metallic;
        }
        
        
        float PhysicalMaterialData::Roughness() { 
            return m_roughtness;
        }
        
        
        void PhysicalMaterialData::DiffuseColor(M3D::Color &value) { 
            m_diffuseColor = value;
        }
        
        
        M3D::Color PhysicalMaterialData::DiffuseColor() { 
            return m_diffuseColor;
        }
    
    
    M3D::Color PhysicalMaterialData::EmissiveColor() { 
        return m_emissiveColor;
    }
    
    void PhysicalMaterialData::Metallic(float value) { 
        m_metallic = value;
    }
    
    void PhysicalMaterialData::Roughness(float value) { 
        m_roughtness = value;
    }
    
    void PhysicalMaterialData::EmissiveColor(M3D::Color &value) { 
        m_emissiveColor = value;
    }
    
    
    
    

	ShaderMaterialData::~ShaderMaterialData()
	{

	}

	map<string, std::pair<string, string> >& ShaderMaterialData::ParametersMap()
	{
		return m_valueMap;
	}

	ShaderMaterialData::ShaderMaterialData() :
		MaterialData()
	{

	}

	CrystalMaterialData::CrystalMaterialData()
	{
		DataType(103);
	}

	JadeMaterialData::JadeMaterialData()
	{
		DataType(101);
	}

	PearlMaterialData::PearlMaterialData()
	{
		DataType(102);
	}

	RingMaterialData::RingMaterialData()
	{
		DataType(104);
	}

}
