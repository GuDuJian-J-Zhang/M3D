#include "m3d/graphics/BaseMaterial.h"
#include "m3d/utils/IDCreator.h"
#include "m3d/graphics/Texture2D.h"
#include "m3d/graphics/TextureCube.h"
#include "m3d/ResourceManager.h"
#include "m3d/renderer/UniformHelper.h"


namespace M3D
{
	BaseMaterial::BaseMaterial():Resource()
	{
		m_materialType = MaterialType::MaterialType_Base;
		m_useLight = true;
		m_uuid = IDCreator::GetUUID();
		m_program = NULL;
		m_needUpdate = true;
		m_define = "";
		m_lightHash = "";
        needsUpdateUniformParameters = true;
	}


	BaseMaterial::~BaseMaterial()
	{
	}
	void  BaseMaterial::SetName(const string& name)
	{
		this->m_name = name;
	}

	const string&  BaseMaterial::GetName()const
	{
		return this->m_name;
	}

	M3D::MaterialType BaseMaterial::GetMaterialType()
	{
		return m_materialType;
	}

	void BaseMaterial::SetMaterialType(MaterialType val)
	{
		m_materialType = val;
	}

	bool BaseMaterial::SetUniformParameter(string key, Uniform& unformValue)
	{
		bool ret = false;
		if (m_uniformParameters.find(key) == m_uniformParameters.end())
		{
			m_uniformParameters.insert(std::make_pair(key, unformValue));
			ret = true;
		}
		//else
		//{
		//	Uniform& unifrom = m_uniformParameters[key];
		//	string uniformType = unifrom.type;

		//	if (uniformType == "Texture2D")
		//	{

		//		Texture2D * texture = anyCast <Texture2D*>(unifrom.value);
		//		texture->Release();

		//	}
		//	//else if (uniformType == "GeometryBuffer")
		//	//{

		//	//}
		//	//else if (uniformType == "Texture2DArray")
		//	//{
		//	//	
		//	//}
		//	else if (uniformType == "TextureCube")
		//	{

		//		TextureCube * texture = anyCast<TextureCube*>(unifrom.value);
		//		texture->Release();

		//	}		
		//	else if (uniformType == "FloatArray")
		//	{
		//		//void * value = unifrom.value;
		//		//vector<float>* floatArray = anyCast<vector<float>*>(unifrom.value);
		//		
		//	}
		//	else if (uniformType == "Vector2")
		//	{
		//		Vector2* value = anyCast<Vector2*>(unifrom.value);

		//	}
		//	else if (uniformType == "Vector3")
		//	{
		//		Vector3* value = anyCast<Vector3*>(unifrom.value);

		//	}
		//	else if (uniformType == "Vector4")
		//	{
		//		Vector4* value = anyCast<Vector4*>(unifrom.value);

		//	}
		//	else if (uniformType == "Color")
		//	{
		//		Color* value = anyCast<Color*>(unifrom.value);

		//	}
		//	else if (uniformType == "Vector4Array")
		//	{
		//		vector<Vector4>* value = anyCast<vector<Vector4>*>(unifrom.value);

		//	}
		//	//else if (uniformType == "Bool")
		//	//{
		//	//	
		//	//}
		//	//else if (uniformType == "BoolArray")
		//	//{
		//	//	vector<int>* floatArray = anyCast <vector<int>*>(unifrom.value);


		//	//}
		//	//else if (uniformType == "Matrix3")
		//	//{
		//	//	Matrix3* value = anyCast<Matrix3*>(unifrom.value);

		//	//}
		//	//else if (uniformType == "Matrix4")
		//	//{
		//	//	Matrix4* value = anyCast<Matrix4*>(unifrom.value);

		//	//}

		//}
		return ret;
	}

	void BaseMaterial::SetUniformParameter(ResourceManager* resourceManager, string key, string type, string value)
	{
		map<string, std::pair<string, string> >  parametersMap;
		parametersMap[key] = std::pair<string, string>(type, value);
		UniformHelper::ParseMaterialParameters(resourceManager, this, parametersMap);
	}

	bool BaseMaterial::RemoveUniformParameter(string key)
	{
		bool ret = false;
		if (m_uniformParameters.find(key) == m_uniformParameters.end())
		{		
			return ret;
		}
		else
		{
			Uniform& unifrom = m_uniformParameters[key];
			string uniformType = unifrom.type;

			if (uniformType == "Texture2D")
			{

				Texture2D * texture = anyCast <Texture2D*>(unifrom.value);
				texture->Release();
				m_uniformParameters.erase(key);
                ret = true;

			}
			//else if (uniformType == "GeometryBuffer")
			//{

			//}
			//else if (uniformType == "Texture2DArray")
			//{
			//	
			//}
			else if (uniformType == "TextureCube")
			{

				TextureCube * texture = anyCast<TextureCube*>(unifrom.value);
				texture->Release();
				m_uniformParameters.erase(key);
                 ret = true;
			}		
			else if (uniformType == "FloatArray")
			{
				//void * value = unifrom.value;
				//vector<float>* floatArray = anyCast<vector<float>*>(unifrom.value);
                 ret = true;
				
			}
			else if (uniformType == "Vector2")
			{
				Vector2* value = anyCast<Vector2*>(unifrom.value);
				delete value;
				value = NULL;
				m_uniformParameters.erase(key);
                 ret = true;

			}
			else if (uniformType == "Vector3")
			{
				Vector3* value = anyCast<Vector3*>(unifrom.value);
				delete value;
				value = NULL;
				m_uniformParameters.erase(key);
                 ret = true;
			}
			else if (uniformType == "Vector4")
			{
				Vector4* value = anyCast<Vector4*>(unifrom.value);
				delete value;
				value = NULL;
				m_uniformParameters.erase(key);
                 ret = true;
			}
			else if (uniformType == "Color")
			{
				Color* value = anyCast<Color*>(unifrom.value);
				delete value;
				value = NULL;
				m_uniformParameters.erase(key);
                 ret = true;
			}
			else if (uniformType == "Vector4Array")
			{
				vector<Vector4>* value = anyCast<vector<Vector4>*>(unifrom.value);
				delete value;
				value = NULL;
				m_uniformParameters.erase(key);
                 ret = true;
			}
			//else if (uniformType == "Bool")
			//{
			//	
			//}
			//else if (uniformType == "BoolArray")
			//{
			//	vector<int>* floatArray = anyCast <vector<int>*>(unifrom.value);


			//}
			//else if (uniformType == "Matrix3")
			//{
			//	Matrix3* value = anyCast<Matrix3*>(unifrom.value);

			//}
			//else if (uniformType == "Matrix4")
			//{
			//	Matrix4* value = anyCast<Matrix4*>(unifrom.value);

			//}

		}
		return ret;
	}

	Uniform * BaseMaterial::GetUniformParameter(string  key)
	{
		if (m_uniformParameters.find(key) != m_uniformParameters.end())
		{
			return &m_uniformParameters[key];
		}
		else
		{
			return nullptr;
		}
	}

	map<string, Uniform>& BaseMaterial::GetUnifomParameters()
	{
		// TODO: �ڴ˴����� return ���
		return m_uniformParameters;
	}

	bool BaseMaterial::GetNeedsUpdateUniformParamerers()
	{
		return needsUpdateUniformParameters;
	}

	void BaseMaterial::SetNeedsUpdateUniformParamerers(bool value)
	{
		this->needsUpdateUniformParameters = value;
	}

	bool BaseMaterial::GetUseLight() const
	{
		return m_useLight;
	}

	void BaseMaterial::SetUseLight(bool val)
	{
		m_useLight = val;
	}

	void BaseMaterial::NeedUpdate(bool val)
	{
		m_needUpdate = val;
	}

	bool BaseMaterial::NeedUpdate() const
	{
		return m_needUpdate;
	}

	void BaseMaterial::Program(ShaderProgram* val)
	{
		m_program = val;
	}

	ShaderProgram* BaseMaterial::Program() const
	{
		return m_program;
	}

	string BaseMaterial::Uuid() const
	{
		return m_uuid;
	}

	void BaseMaterial::Define(string val)
	{
		m_define = val;
	}

	string BaseMaterial::Define() 
	{
		return m_define;
	}

	void BaseMaterial::LightHash(string val)
	{
		m_lightHash = val;
	}

	string BaseMaterial::LightHash() const
	{
		return m_lightHash;
	}

	void BaseMaterial::DisplayName(string val)
	{
		m_displayName = val;
	}

	string BaseMaterial::DisplayName() const
	{
		return m_displayName;
	}

	void BaseMaterial::IsGammaOutpute(bool val)
	{
		m_isGammaOutpute = val;
	}

	bool BaseMaterial::IsGammaOutpute() const
	{
		return m_isGammaOutpute;
	}

	void BaseMaterial::Rename(string newName)
	{

	}

	void BaseMaterial::AcceptLight(bool val)
	{
		m_acceptLight = val;
	}

	bool BaseMaterial::AcceptLight()
	{
		return m_acceptLight;
	}

	void BaseMaterial::SetDefine(string key, string value)
	{
		m_definesMap[key] = value + "\n";
	}

	void BaseMaterial::ClearDefine(string key)
	{
		m_definesMap[key] = "";
	}

}
