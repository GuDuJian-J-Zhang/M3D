#include "m3d/renderer/effect/Effect.h"
#include "m3d/graphics/Material.h"
#include "m3d/renderer/gl20/GLShapeDrawer20.h"
#include "m3d/graphics/TextNode.h"
#include "m3d/graphics/Texture2D.h"
#include "m3d/graphics/TextureCube.h"
#include "m3d/graphics/CameraNode.h"
#include "m3d/SceneManager.h"
#include "m3d/ResourceManager.h"
namespace M3D{
    
	Effect::Effect(RenderAction* action)
	{
		this->m_action = action;
	}

	Effect::~Effect()
	{
		textureBindingTargetMap.clear();
	}

	void Effect::Render()
	{

	}

	void Effect::ClearResource()
	{
	}

	void Effect::SetSize(int w, int h)
	{
	}

	void Effect::SetUniform(ShaderProgram * shaderEffect, SPHashMap& shaderUniformMap, map<string,Uniform>& uniformValues)
	{
		for (SPHashMap::iterator it = shaderUniformMap.begin(); it != shaderUniformMap.end(); it++)
		{
			const string& key = it->first;
			//string ss = key.Tostring();
			string name = it->second.m_name;
			ShaderParameter* para = &it->second;
			if (uniformValues.find(key) == uniformValues.end())
			{
				if (para->m_Type == GL_SAMPLER_2D )
				{
					int textureUnit = GLShapeDrawer20::AllocTextureUnit();
					//设置纹理单元
					shaderEffect->SetUniformValue(shaderEffect->GetShaderUniformParameter(key)->m_location, textureUnit);
					textureBindingTargetMap[textureUnit] = GL_TEXTURE_2D;
					glActiveTexture(GL_TEXTURE0 + textureUnit);
					glBindTexture(GL_TEXTURE_2D, 0);
				}
				else if ( para->m_Type == GL_SAMPLER_CUBE)
				{
					int textureUnit = GLShapeDrawer20::AllocTextureUnit();
					shaderEffect->SetUniformValue(shaderEffect->GetShaderUniformParameter(key)->m_location, textureUnit);
					textureBindingTargetMap[textureUnit] = GL_TEXTURE_CUBE_MAP;
					glActiveTexture(GL_TEXTURE0 + textureUnit);
					glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
				}
				else if (para->m_Type == GL_BOOL)
				{
					int value = 0;
					shaderEffect->SetUniformValue(shaderEffect->GetShaderUniformParameter(key)->m_location, value);
				}
				else if (para->m_Type == GL_FLOAT)
				{
					float value = 0;
					shaderEffect->SetUniformValue(key, &value);
				}
				
				continue;
				
				
			}
			
			Uniform& unifrom = uniformValues[key];
			string uniformType = unifrom.type;
			
			if (para)
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
								glBindTexture(GL_TEXTURE_2D, obj);
							}
					}
				}
				if (uniformType == "GeometryBuffer")
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
							glBindTexture(GL_TEXTURE_2D, obj);
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
							glBindTexture(GL_TEXTURE_CUBE_MAP, obj);
						}
					}
				}
				else if (uniformType == "Float")
				{
					//void * value = unifrom.value;
					float floatArray = anyCast<float>(unifrom.value);
					shaderEffect->SetUniformValue(key,&floatArray);
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
					shaderEffect->SetUniformValue(location,value->m_x,value->m_y,value->m_z);
				}
				else if (uniformType == "Vector4")
				{
					Vector4* value = anyCast<Vector4*>(unifrom.value);
					GLint location = shaderEffect->GetShaderUniformParameter(key)->m_location;
					shaderEffect->SetUniformValue(location, value->m_x, value->m_y, value->m_z,value->m_w);
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
				else if (uniformType == "Bool" )
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

			}
		}
	}

	void Effect::MergeUnifom(map<string, Uniform>&src, map<string, Uniform>&dist)
	{
		for (map<string, Uniform>::iterator it = src.begin(); it != src.end(); it++) {
			string key = it->first;
			Uniform value = it->second;
			dist[key] = value;
		}
	}

	HardWareFrameBuffer & Effect::GetHardWareFrameBuffer(string key)
	{
		CameraNode * camera = this->m_action->GetCamera();
		ResourceManager* rsMgr = this->m_action->GetScene()->GetResourceManager();
		map<string, HardWareFrameBuffer>::iterator it = m_hardWareFrameBufferMap.find(key);
		if (it != m_hardWareFrameBufferMap.end())
		{
			return it->second;
		}
		else
		{
			HardWareFrameBuffer fbo;
			m_hardWareFrameBufferMap[key] = fbo;
			m_hardWareFrameBufferMap[key].SetSize(camera->GetViewPort().GetRect().Width(), camera->GetViewPort().GetRect().Height());
			m_hardWareFrameBufferMap[key].SetResourceManager(rsMgr);
			return m_hardWareFrameBufferMap[key];
		}
	}

}
