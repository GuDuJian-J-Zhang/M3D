#include "m3d/renderer/UniformHelper.h"
#include "m3d/graphics/Texture.h"
#include "m3d/graphics/Texture2D.h"
#include "m3d/graphics/TextureCube.h"
#include "m3d/SceneManager.h"
#include "m3d/ResourceManager.h"
//#include "m3d/renderer/gl20/ShaderProgram.h"
#include "m3d/renderer/gl20/GLShapeDrawer20.h"
#include "m3d/utils/StringHelper.h"
namespace M3D
{
	UniformHelper::UniformHelper()
	{
	}


	UniformHelper::~UniformHelper()
	{

	}

	UniformMap* UniformHelper::GetUniformMap(string key)
	{
		UniformMap* ret = nullptr;
		map<string, UniformMap*>::iterator it = m_uniformMaps.find(key);
		if (it == m_uniformMaps.end())
		{
			ret = new UniformMap;
			m_uniformMaps.insert(make_pair(key, ret));
		}
		else {
			ret = it->second;
		}
		return ret;
	}

	//************************************
	// Method:    UpdateUniform
	// FullName:  M3D::UniformHelper::UpdateUniform
	// Access:    public static 
	// Returns:   void
	// Qualifier:
	// Parameter: RenderAction * action
	// Parameter: map<int
	// Parameter: GLenum> & textureBindingTargetMap 纹理绑定记录
	// Parameter: ShaderProgram * shaderEffect 着色器程序
	// Parameter: SPHashMap & shaderUniformMap shader中活动的uniform
	// Parameter: map<string
	// Parameter: Uniform> & values uniform 对应的变量的值
	//************************************
	void UniformHelper::UpdateUniform(RenderAction* action, map<int, GLenum>&textureBindingTargetMap, ShaderProgram* shaderEffect,
		SPHashMap& shaderUniformMap, map<string, Uniform>& uniformValues)
	{
		for (map<string, Uniform>::iterator it = uniformValues.begin(); it != uniformValues.end(); it++)
		{
			const string& key = it->first;
			//string ss = key.Tostring();
			string name = it->second.type;
			//ShaderParameter* para = &it->second;
			if (shaderUniformMap.find(key) == shaderUniformMap.end())
			{
				if (name == "Texture2D")
				{
					int textureUnit = GLShapeDrawer20::AllocTextureUnit();
					ShaderParameter* temp = shaderEffect->GetShaderUniformParameter(key);
					if (temp)
					{
						shaderEffect->SetUniformValue(temp->m_location, textureUnit);
						textureBindingTargetMap[textureUnit] = GL_TEXTURE_2D;
						glActiveTexture(GL_TEXTURE0 + textureUnit);
						M3D_GL_ERROR_CHECK
							glBindTexture(GL_TEXTURE_2D, 0);
						M3D_GL_ERROR_CHECK
					}

				}
				else if (name == "TextureCube")
				{
					int textureUnit = GLShapeDrawer20::AllocTextureUnit();
					ShaderParameter* temp = shaderEffect->GetShaderUniformParameter(key);
					if (temp)
					{
						shaderEffect->SetUniformValue(temp->m_location, textureUnit);
						textureBindingTargetMap[textureUnit] = GL_TEXTURE_CUBE_MAP;
						glActiveTexture(GL_TEXTURE0 + textureUnit);
						M3D_GL_ERROR_CHECK
							glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
						M3D_GL_ERROR_CHECK
					}
				
				}
				//else if (para->m_Type == GL_BOOL)
				//{
				//	int value = 0;
				//	shaderEffect->SetUniformValue(shaderEffect->GetShaderUniformParameter(key)->m_location, value);
				//}

				continue;


			}

			Uniform& unifrom = uniformValues[key];
			string uniformType = unifrom.type;

			if (/*para*/ true)
			{

				if (uniformType == "Texture2D")
				{
					int textureUnit = GLShapeDrawer20::AllocTextureUnit();
					shaderEffect->SetUniformValue(shaderEffect->GetShaderUniformParameter(key)->m_location, textureUnit);
					Texture2D * texture = anyCast <Texture2D*>(unifrom.value);
					if (texture)
					{
						unsigned int obj = texture->GetOGLObj();
						if (obj)
						{
							textureBindingTargetMap[textureUnit] = GL_TEXTURE_2D;
							glActiveTexture(GL_TEXTURE0 + textureUnit);
							M3D_GL_ERROR_CHECK
							glBindTexture(GL_TEXTURE_2D, obj);
							M3D_GL_ERROR_CHECK
						}
					}
				}
				else if (uniformType == "GeometryBuffer")
				{
					int textureUnit = GLShapeDrawer20::AllocTextureUnit();
					shaderEffect->SetUniformValue(shaderEffect->GetShaderUniformParameter(key)->m_location, textureUnit);
					GeometryBuffer * texture = anyCast <GeometryBuffer*>(unifrom.value);
					if (texture)
					{
						unsigned int obj = texture->GetOGLObj();
						if (obj)
						{
							textureBindingTargetMap[textureUnit] = GL_TEXTURE_2D;
							glActiveTexture(GL_TEXTURE0 + textureUnit);
							M3D_GL_ERROR_CHECK
							glBindTexture(GL_TEXTURE_2D, obj);
							M3D_GL_ERROR_CHECK
						}
					}
				}
				else if (uniformType == "GeometryBufferArray")
				{
					//zhege正确
					vector<GeometryBuffer*> textures = anyCast<vector<GeometryBuffer*>>(unifrom.value);
					vector<int> textureUnits;
					for (int i = 0, length = textures.size(); i < length; i++)
					{
						int textureUnit = GLShapeDrawer20::AllocTextureUnit();
						textureUnits.push_back(textureUnit);						
						GeometryBuffer * texture = static_cast <GeometryBuffer*>(textures[i]);
						if (texture)
						{
							unsigned int obj = texture->GetOGLObj();
							if (obj)
							{
								textureBindingTargetMap[textureUnit] = GL_TEXTURE_2D;
								glActiveTexture(GL_TEXTURE0 + textureUnit);
								M3D_GL_ERROR_CHECK
									glBindTexture(GL_TEXTURE_2D, obj);
								M3D_GL_ERROR_CHECK
							}
							else
							{
								textureBindingTargetMap[textureUnit] = GL_TEXTURE_2D;
								glActiveTexture(GL_TEXTURE0 + textureUnit);
								M3D_GL_ERROR_CHECK
									glBindTexture(GL_TEXTURE_2D, 0);
								M3D_GL_ERROR_CHECK
							}
						}
					}
					shaderEffect->SetUniformValue(key,textureUnits.size(), &textureUnits[0]);
				}
				else if (uniformType == "Texture2DArray")
				{
					vector<Texture2D*>* textures = anyCast<vector<Texture2D*>*>(unifrom.value);
					vector<int> textureUnits;
					for (int i = 0, length = textures->size(); i < length; i++)
					{
						int textureUnit = GLShapeDrawer20::AllocTextureUnit();
						textureUnits.push_back(textureUnit);
						Texture* texture = (*textures)[i];
						if (texture)
						{
							unsigned int obj = texture->GetOGLObj();
							if (obj)
							{
								textureBindingTargetMap[textureUnit] = GL_TEXTURE_2D;
								glActiveTexture(GL_TEXTURE0 + textureUnit);
								M3D_GL_ERROR_CHECK
								glBindTexture(GL_TEXTURE_2D, obj);
								M3D_GL_ERROR_CHECK
							}
							else
							{
								textureBindingTargetMap[textureUnit] = GL_TEXTURE_2D;
								glActiveTexture(GL_TEXTURE0 + textureUnit);
								M3D_GL_ERROR_CHECK
								glBindTexture(GL_TEXTURE_2D, 0);
								M3D_GL_ERROR_CHECK
							}
						}
					}
				}
				else if (uniformType == "TextureCube")
				{
					int textureUnit = GLShapeDrawer20::AllocTextureUnit();
					shaderEffect->SetUniformValue(shaderEffect->GetShaderUniformParameter(key)->m_location, textureUnit);
					TextureCube * texture = anyCast<TextureCube*>(unifrom.value);
					if (texture)
					{
						unsigned int obj = texture->GetOGLObj();
						if (obj)
						{
							textureBindingTargetMap[textureUnit] = GL_TEXTURE_CUBE_MAP;
							glActiveTexture(GL_TEXTURE0 + textureUnit);
							M3D_GL_ERROR_CHECK
							glBindTexture(GL_TEXTURE_CUBE_MAP, obj);
							M3D_GL_ERROR_CHECK
						}
					}
				}
				else if (uniformType == "Float")
				{
					//void * value = unifrom.value;
					float floatArray = anyCast<float>(unifrom.value);
					GLint location = shaderEffect->GetShaderUniformParameter(key)->m_location;
					shaderEffect->SetUniformValue(location, floatArray);
				}
				else if (uniformType == "FloatArray")
				{
					//void * value = unifrom.value;
					vector<float>* floatArray = anyCast<vector<float>*>(unifrom.value);
					shaderEffect->SetUniformValue(key, floatArray->size(), &floatArray->at(0));
				}
				else if (uniformType == "Vector2")
				{
					Vector2* value = anyCast<Vector2*>(unifrom.value);
					GLint location = shaderEffect->GetShaderUniformParameter(key)->m_location;
					shaderEffect->SetUniformValue(location, value->m_x, value->m_y);
				}
				else if (uniformType == "Vector3")
				{
					Vector3* value = anyCast<Vector3*>(unifrom.value);
					GLint location = shaderEffect->GetShaderUniformParameter(key)->m_location;
					shaderEffect->SetUniformValue(location, value->m_x, value->m_y, value->m_z);
				}
				else if (uniformType == "Vector4")
				{
					Vector4* value = anyCast<Vector4*>(unifrom.value);
					GLint location = shaderEffect->GetShaderUniformParameter(key)->m_location;
					shaderEffect->SetUniformValue(location, value->m_x, value->m_y, value->m_z, value->m_w);
				}
				else if (uniformType == "Color")
				{
					Color* value = anyCast<Color*>(unifrom.value);
					GLint location = shaderEffect->GetShaderUniformParameter(key)->m_location;
					shaderEffect->SetUniformValue(location, value->m_r, value->m_g, value->m_b, value->m_a);
				}
				else if (uniformType == "Vector4Array")
				{
					vector<Vector4>* value = anyCast<vector<Vector4>*>(unifrom.value);
					shaderEffect->SetUniformValue(key, value->size(), &value->at(0));
				}
				else if (uniformType == "Bool")
				{
					int value = anyCast<int>(unifrom.value);
					shaderEffect->SetUniformValue(shaderEffect->GetShaderUniformParameter(key)->m_location, value);
				}
				else if (uniformType == "BoolArray")
				{
					vector<int>* floatArray = anyCast <vector<int>*>(unifrom.value);

					shaderEffect->SetUniformValue(key, floatArray->size(), &floatArray->at(0));
				}
				else if (uniformType == "Matrix3")
				{
					Matrix3* value = anyCast<Matrix3*>(unifrom.value);
					shaderEffect->SetUniformValue(key, *((Matrix3*)value));
				}
				else if (uniformType == "Matrix4")
				{
					Matrix4* value = anyCast<Matrix4*>(unifrom.value);
					shaderEffect->SetUniformValue(key, *((Matrix4*)value));
				}
				else if (uniformType == "Matrix4Array")
				{
					vector<Matrix4> value = anyCast<vector<Matrix4>>(unifrom.value);
					shaderEffect->SetUniformValue(key,value.size(),false,&value[0].m_m00);
				}
			}
		}
	}

	void UniformHelper::SetUniformValueToMap(UniformMap* uniformMap, const string& name, string valueType, const Any& value)
	{
		map<string, Uniform>& uniformMapValue = uniformMap->m_uniformMap;
		map<string, Uniform>::iterator it = uniformMapValue.find(name);
		if (it != uniformMapValue.end())
		{
			it->second.value = value;
		}
		else {
			Uniform uniformValue(valueType, value);
			uniformMapValue.insert(std::make_pair(name, uniformValue));
		}
	}

	void UniformHelper::ParseMaterialParameters(ResourceManager*resourceManager, BaseMaterial* material, map<string, std::pair<string, string> > & parametersMap)
	{
		if (!material)
		{
			return;
		}

		map<string, pair<string, string> >::iterator it = parametersMap.begin();
		for (; it != parametersMap.end(); it++)
		{
			string name = it->first;
			string type = it->second.first;
			string value = it->second.second;

			if (type == "Texture2D")
			{
				Texture2D* texture2d = (Texture2D*)resourceManager->GetOrCreateTexture(value, Texture::TEXTURE_2D);
				texture2d->SetImageParameter(TEXTURE_LOAD_RGBA, TEXTURE_FLAG_MIPMAPS | TEXTURE_FLAG_INVERT_Y);
				Image* image = new Image();
				image->SetPath(value);
				image->ReadFile();
				AddRefMe(image);
				texture2d->SetImage(image);
				ReleaseMe(image);

				Uniform uniform(type, (Texture2D*)texture2d);
				texture2d->AddRef();
				if (!material->SetUniformParameter(name, uniform))
				{
					//Exist this name already
					map<string, Uniform>& allMaps =material->GetUnifomParameters();
					Uniform* oldUniform =  material->GetUniformParameter(name);
					Texture2D* oldValue = anyCast<Texture2D*>(oldUniform->value);
					if (oldValue != texture2d)//if the old's pointer not equal the new's
					{
						oldValue->Release();//release the old 
						allMaps[name] = uniform;//replace it by the new 	
					}
					else
					{
						texture2d->Release();
					}
				}
			}
			else if (type == "TextureCube")
			{
				vector<string> imageIDStrArray;

				string savePathes = "";
				vector<string> pathes = StringHelper::Split(value, ",");


				for (int i = 0; i < pathes.size(); i++)
				{
					unsigned int buffersize = 0;
					char* data;
					//去除首尾空格 
					if (!pathes[i].empty())
					{
						pathes[i].erase(0, pathes[i].find_first_not_of(" "));
						pathes[i].erase(pathes[i].find_last_not_of(" ") + 1);
					}
					imageIDStrArray.push_back(pathes[i]);
					savePathes += pathes[i];
				}
				TextureCube * cubeTexture = (TextureCube*)resourceManager->GetOrCreateCubeMappingTexture(name, imageIDStrArray);

				Uniform uniform(type, cubeTexture);
				cubeTexture->AddRef();
				if (!material->SetUniformParameter(name, uniform))
				{
					//Exist this name already
					map<string, Uniform>& allMaps = material->GetUnifomParameters();
					Uniform* oldUniform = material->GetUniformParameter(name);
					TextureCube* oldValue = anyCast<TextureCube*>(oldUniform->value);
					if (oldValue != cubeTexture)//if the old's pointer not equal the new's
					{
						oldValue->Release();//release the old 
						allMaps[name] = uniform;//replace it by the new 	
					}
					else
					{
						cubeTexture->Release();
					}
					
				}

			}
			else if (type == "Vector3")
			{
				vector<string> vectorStrArray = StringHelper::Split(value, ",");
				Vector3* vec = new Vector3();
				vec->m_x = StringHelper::StringToFloat(vectorStrArray[0]);
				vec->m_y = StringHelper::StringToFloat(vectorStrArray[1]);
				vec->m_z = StringHelper::StringToFloat(vectorStrArray[2]);
				Uniform uniform(type, vec);
				if (!material->SetUniformParameter(name, uniform))
				{
					//Exist this name already
					map<string, Uniform>& allMaps = material->GetUnifomParameters();
					Uniform* oldUniform = material->GetUniformParameter(name);
					Vector3* oldValue = anyCast<Vector3*>(oldUniform->value);
					if (oldValue != vec)//if the old's pointer not equal the new's
					{
						delete oldValue;
						oldValue = NULL;
						allMaps[name] = uniform;//replace it by the new 	
					}
					else
					{
						delete vec;
						vec = NULL;
					}
				}
			}
			else if (type == "Vector4")
			{
				vector<string> vectorStrArray = StringHelper::Split(value, ",");
				Vector4* vec = new Vector4();
				vec->m_x = StringHelper::StringToFloat(vectorStrArray[0]);
				vec->m_y = StringHelper::StringToFloat(vectorStrArray[1]);
				vec->m_z = StringHelper::StringToFloat(vectorStrArray[2]);
				vec->m_w = StringHelper::StringToFloat(vectorStrArray[3]);
				Uniform uniform(type, vec);
				if (!material->SetUniformParameter(name, uniform))
				{
					//Exist this name already
					map<string, Uniform>& allMaps = material->GetUnifomParameters();
					Uniform* oldUniform = material->GetUniformParameter(name);
					Vector4* oldValue = anyCast<Vector4*>(oldUniform->value);
					if (oldValue != vec)//if the old's pointer not equal the new's
					{
						delete oldValue;
						oldValue = NULL;
						allMaps[name] = uniform;//replace it by the new 	
					}
					else
					{
						delete vec;
						vec = NULL;
					}
				}
			}
			else if (type == "Float")
			{
				float fv;
				fv = (StringHelper::StringToFloat(value));
				Uniform uniform(type, fv);
				if (!material->SetUniformParameter(name, uniform))
				{
					map<string, Uniform>& allMaps = material->GetUnifomParameters();
					Uniform* oldUniform = material->GetUniformParameter(name);
					float oldValue = anyCast<float>(oldUniform->value);
					if (oldValue != fv)//if the old's pointer not equal the new's
					{
						allMaps[name] = uniform;//replace it by the new 	
					}

				}
			}
			else if (type == "Bool")
			{

				int v = value == "true" ? 1 : 0;
				Uniform uniform(type, v);
				if (!material->SetUniformParameter(name, uniform))
				{
					map<string, Uniform>& allMaps = material->GetUnifomParameters();
					Uniform* oldUniform = material->GetUniformParameter(name);
					int oldValue = anyCast<int>(oldUniform->value);
					if (oldValue != v)//if the old's pointer not equal the new's
					{
						allMaps[name] = uniform;//replace it by the new 	
					}
				}
			}
			else if (type == "Int")
			{
				int fv;
				fv = (StringHelper::StringToFloat(value));
				Uniform uniform(type, fv);
				if (!material->SetUniformParameter(name, uniform))
				{
					map<string, Uniform>& allMaps = material->GetUnifomParameters();
					Uniform* oldUniform = material->GetUniformParameter(name);
					int oldValue = anyCast<int>(oldUniform->value);
					if (oldValue != fv)//if the old's pointer not equal the new's
					{
						allMaps[name] = uniform;//replace it by the new 	
					}
				}
			}
		}
		material->SetNeedsUpdateUniformParamerers(false);
    }
    
    M3D::Uniform *UniformHelper::MakeUniform(std::string valueType, M3D::Any &value) { 
        Uniform * ret = new Uniform(valueType,value);
        
        string uniformType = valueType;
        
 
            
            if (uniformType == "Texture2D")
            {
              
                Texture2D * texture = anyCast <Texture2D*>(value);
                texture->AddRef();

            }
            else if (uniformType == "GeometryBuffer")
            {
               
                GeometryBuffer * texture = anyCast <GeometryBuffer*>(value);
                texture->AddRef();
               
            }
            else if (uniformType == "Texture2DArray")
            {

            }
            else if (uniformType == "TextureCube")
            {
  
                TextureCube * texture = anyCast<TextureCube*>(value);
                texture->AddRef();
            
            }
//            else if (uniformType == "Float")
//            {
//
//            }
//            else if (uniformType == "FloatArray")
//            {
//
//            }
//            else if (uniformType == "Vector2")
//            {
//
//            }
//            else if (uniformType == "Vector3")
//            {
//
//            }
//            else if (uniformType == "Vector4")
//            {
//
//            }
//            else if (uniformType == "Color")
//            {
//
//            }
//            else if (uniformType == "Vector4Array")
//            {
//
//            }
//            else if (uniformType == "Bool")
//            {
//
//            }
//            else if (uniformType == "BoolArray")
//            {
//
//            }
//            else if (uniformType == "Matrix3")
//            {
//
//            }
//            else if (uniformType == "Matrix4")
//            {
//
//            }

        
        return ret;
    }
    

	void UniformHelper::Clear()
	{
		map<string, UniformMap*>::iterator it = m_uniformMaps.begin();
		for (; it != m_uniformMaps.end(); ++it)
		{
			delete it->second;
		}
		m_uniformMaps.clear();
	}

	map<string, UniformMap*> UniformHelper::m_uniformMaps;

}
