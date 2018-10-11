
#include "m3d/graphics/ShaderMaterial.h"
#include "m3d/ResourceManager.h"
#include "m3d/graphics/Texture2D.h"
#include "m3d/graphics/TextureCube.h"
namespace M3D
{
	ShaderMaterial::ShaderMaterial():BaseMaterial()
	{
		m_materialType = MaterialType_Shader;
	}
	ShaderMaterial::ShaderMaterial(ShaderMaterial& org):BaseMaterial(org)
	{
		m_fragmentShader = org.m_fragmentShader;
		m_vertexShader = org.m_vertexShader;
	}

	ShaderMaterial::~ShaderMaterial()
	{
		RealeseParameters();
	}
	BaseMaterial* ShaderMaterial::Clone()
	{
		return new ShaderMaterial(*this);
	}
	void ShaderMaterial::RealeseParameters()
	{
		map<string, Uniform>::iterator it = m_uniformParameters.begin();
		for (; it != m_uniformParameters.end(); ++it)
		{
			string type = it->second.type;
			//void * value = it->second.value;
			if (type == "Texture2D")
			{
				Texture2D* value = anyCast<Texture2D*> (it->second.value);
				if (value)
				{
					value->Release();
					value = NULL;
				}
			}
			else if (type == "TextureCube")
			{
				TextureCube* value = anyCast<TextureCube*>(it->second.value);
				if (value)
				{
					value->Release();
					value = NULL;
				}
			}
			else if (type == "Float")
			{
				//vector<float>* fv = (vector<float>*)value;
				//if (fv)
				//{
				//	delete fv;
				//	value = NULL;
				//}

			}
			else if (type == "Int")
			{
	/*			vector<int>* iv = (vector<int>*)value;
				if (iv)
				{
					delete iv;
					value = NULL;
				}*/
			}
			else if (type == "Vector3")
			{
				Vector3 * vec3 = anyCast<Vector3*>(it->second.value);
				if (vec3)
				{
					delete vec3;
					vec3 = nullptr;
				}
			}
			else if (type == "Vector4")
			{
				Vector4 * vec4 = anyCast<Vector4*>(it->second.value);
				if (vec4)
				{
					delete vec4;
					vec4 = nullptr;
				}
			}
			else if (type == "Bool")
			{
				//int * b = (int*)value;
				//if (b)
				//{
				//	delete b;
				//	value = NULL;
				//}
			}
		}
	}
}

