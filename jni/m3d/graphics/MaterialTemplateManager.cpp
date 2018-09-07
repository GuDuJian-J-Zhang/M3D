
#include "JsonMacro.h"
#include "document.h"
#include "stringbuffer.h"
#include "prettywriter.h"
#include "allocators.h"
using rapidjson::Value;
using rapidjson::Document;
using rapidjson::StringBuffer;
using rapidjson::PrettyWriter;
using rapidjson::Writer;
using rapidjson::StringRef;
using rapidjson::MemoryPoolAllocator;

#include "m3d/graphics/MaterialTemplateManager.h"
#include "m3d/ResourceManager.h"
#include "m3d/graphics/InnerMaterial.h"
#include "m3d/graphics/Material.h"
#include "m3d/graphics/PbrMaterial.h"
#include "m3d/utils/StringHelper.h"
#include "m3d/renderer/UniformHelper.h"
#include "m3d/utils/FileHelper.h"
#include "sview/views/Parameters.h"
#include "m3d/utils/IDCreator.h"
#include "m3d/graphics/MaterialData.h"


namespace M3D
{
	MaterialTemplateManager::MaterialTemplateManager(ResourceManager* resourceManager)
	{
		m_resourceManager = resourceManager;
	}


	MaterialTemplateManager::~MaterialTemplateManager()
	{
	}

	bool MaterialTemplateManager::Load(string templatePath)
	{
		string srccode = FileHelper::ReadTxt(templatePath);//json源码

		Document infoDoc;
		infoDoc.Parse(srccode.c_str());		
		if (infoDoc.HasMember("materials"))
		{
			Value& infoNode = infoDoc["materials"];

			if (infoNode.IsArray())
			{
				for (size_t i = 0; i < infoNode.Size(); i++)
				{
					MaterialData * materialData = NULL;
					Value& materialNode = infoNode[i];
					string name = "";
					//name
					if (materialNode.HasMember("name"))
					{
						name = materialNode["name"].GetString();

					}
					//type
					int type;
					if (materialNode.HasMember("type"))
					{
						type = materialNode["type"].GetInt();

						if (type == 101 || type == 102 || type == 103 || type == 104 ||
							type == MaterialType_Shader
							)
						{
							if (type == 101)
							{
								materialData = new JadeMaterialData;							
								
							}
							else if (type == 102)
							{
								materialData = new PearlMaterialData;
							}
							else if (type == 103)
							{
								materialData = new CrystalMaterialData;
							}
							else if (type == 104)
							{
								materialData = new RingMaterialData;
							}
							else
							{
								materialData = new ShaderMaterialData;
							}
							materialData->DisplayName(name);
							map < string, pair<string, string> >&parametersMap = ((ShaderMaterialData*)materialData)->ParametersMap();
							
							parametersMap["type"] = make_pair("Int", StringHelper::IntToString(type));
							
							//获取材质类型
							Value::MemberIterator parametersIter = materialNode.FindMember("parameters");
							if ((parametersIter != materialNode.MemberEnd() && parametersIter->value.IsObject()))
							{
								Value& parameters = parametersIter->value;

								//	Value::MemberIterator iter = parameters.MemberBegin();
								//	Value& value = iter->value;
								//	string name = iter->name.GetString();
								for (Value::MemberIterator iter = parameters.MemberBegin(); iter != parameters.MemberEnd(); iter++)
								{
									Value& value = iter->value;
									string name = iter->name.GetString();
									Value::MemberIterator paraTypeIter = value.FindMember("type");
									Value::MemberIterator paraValueIter = value.FindMember("value");
									if (paraTypeIter != value.MemberEnd() && paraValueIter != value.MemberEnd())
									{
										string paraTypeStr = paraTypeIter->value.GetString();
										Value& paraValue = paraValueIter->value;
										if (paraTypeStr == "TextureCube" || paraTypeStr == "Texture2D" || paraTypeStr == "Int")
										{

											string result = "";

											result = paraValue.GetString();


											vector<string> pathes = StringHelper::Split(result, ",");

											string newResult = "";
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
												newResult+=SVIEW::Parameters::Instance()->m_appWorkPath + pathes[i];
												if (i<pathes.size()-1)
												{
													newResult += ",";
												}
												
											}

											parametersMap[name] = make_pair(paraTypeStr, newResult);
										}
										else if (paraTypeStr == "Vector4" || paraTypeStr == "Vector3" || paraTypeStr == "Float")
										{
											string result = "";
											for (int i = 0; i < paraValue.Size(); i++)
											{
												float temp = paraValue[i].GetFloat();
												char s[10];
												sprintf(s, "%f", temp);
												result = result + string(s);
												if (i < paraValue.Size() - 1)
												{
													result = result + ",";
												}
											}
											parametersMap[name] = make_pair(paraTypeStr, result);
										}
										else if (paraTypeStr == "Bool")
										{
											string temp = paraValue.GetBool() ? "true" : "false";
											parametersMap[name] = make_pair(paraTypeStr, temp);
										}

									}

								}

							}
							
						}
						else if (type == MaterialType_Base || type == MaterialType_Phong)
						{
                            materialData = new PhongMaterialData;
                            PhongMaterialData* tempData = static_cast<PhongMaterialData*>(materialData );
                            materialData->DisplayName(name);
                            
                            if(materialNode.HasMember("ambientColor"))
                            {
                                Value& paraValue = materialNode["ambientColor"];
                                if(paraValue.IsArray())
                                {
                                    Color temp;
                                    
                                    temp.m_r = paraValue[0].GetFloat();
                                    temp.m_g = paraValue[1].GetFloat();
                                    temp.m_b = paraValue[2].GetFloat();
                                    temp.m_a = paraValue[3].GetFloat();
                                    
                                    tempData->AmbientColor(temp);
                                    
                                }
                            }
                            
                            if(materialNode.HasMember("diffuseColor"))
                            {
                                Value& paraValue = materialNode["diffuseColor"];
                                if(paraValue.IsArray())
                                {
                                    Color temp;
                                    
                                    temp.m_r = paraValue[0].GetFloat();
                                    temp.m_g = paraValue[1].GetFloat();
                                    temp.m_b = paraValue[2].GetFloat();
                                    temp.m_a = paraValue[3].GetFloat();
                                    
                                    tempData->DiffuseColor(temp);
                                    
                                }
                            }
                            if(materialNode.HasMember("specularColor"))
                            {
                                Value& paraValue = materialNode["specularColor"];
                                if(paraValue.IsArray())
                                {
                                    Color temp;
                                    
                                    temp.m_r = paraValue[0].GetFloat();
                                    temp.m_g = paraValue[1].GetFloat();
                                    temp.m_b = paraValue[2].GetFloat();
                                    temp.m_a = paraValue[3].GetFloat();
                                    
                                    tempData->SpecularColor(temp);
                                    
                                }
                            }
                            
                            if(materialNode.HasMember("emissiveColor"))
                            {
                                Value& paraValue = materialNode["emissiveColor"];
                                if(paraValue.IsArray())
                                {
                                    Color temp;
                                    
                                    temp.m_r = paraValue[0].GetFloat();
                                    temp.m_g = paraValue[1].GetFloat();
                                    temp.m_b = paraValue[2].GetFloat();
                                    temp.m_a = paraValue[3].GetFloat();
                                    
                                    tempData->EmissiveColor(temp);
                                    
                                }
                            }
                            
                            if (materialNode.HasMember("transparency")) {
                                float temp = materialNode["transparency"].GetFloat();
                                tempData->Opcity(temp);
                            }
                            
                            if (materialNode.HasMember("shininess")) {
                                float temp = materialNode["shininess"].GetFloat();
                                tempData->Shininess(temp);
                            }
                            

						}
						else if (type == MaterialType_Pbr)
						{
                            materialData = new PhysicalMaterialData;
                            PhysicalMaterialData* tempData = static_cast<PhysicalMaterialData*>(materialData );
                            materialData->DisplayName(name);
                            
                            if(materialNode.HasMember("emissiveColor"))
                            {
                                Value& paraValue = materialNode["emissiveColor"];
                                if(paraValue.IsArray())
                                {
                                    Color temp;
                                    
                                    temp.m_r = paraValue[0].GetFloat();
                                    temp.m_g = paraValue[1].GetFloat();
                                    temp.m_b = paraValue[2].GetFloat();
                                    temp.m_a = paraValue[3].GetFloat();
                                    
                                    tempData->EmissiveColor(temp);
                                    
                                }
                            }
                            
                            if(materialNode.HasMember("diffuseColor"))
                            {
                                Value& paraValue = materialNode["diffuseColor"];
                                if(paraValue.IsArray())
                                {
                                    Color temp;
                                    
                                    temp.m_r = paraValue[0].GetFloat();
                                    temp.m_g = paraValue[1].GetFloat();
                                    temp.m_b = paraValue[2].GetFloat();
                                    temp.m_a = paraValue[3].GetFloat();
                                    
                                    tempData->DiffuseColor(temp);
                                    
                                }
                            }
                            
                            if (materialNode.HasMember("transparency")) {
                                float temp = materialNode["transparency"].GetFloat();
                                tempData->Opcity(temp);
                            }
                            
                            if (materialNode.HasMember("metalnessFactor")) {
                                float temp = materialNode["metalnessFactor"].GetFloat();
                                tempData->Metallic(temp);
                            }
                            
                            if (materialNode.HasMember("roughnessFactor")) {
                                float temp = materialNode["roughnessFactor"].GetFloat();
                                tempData->Roughness(temp);
                            }
                            
						}
					}

					m_materialDataMap[name] = materialData;

				}
			}
		}
		return true;
	}

	bool MaterialTemplateManager::Write(string templatePath)
	{
		return true;
	}

	void MaterialTemplateManager::Clear()
	{
		map<string, MaterialData*>::iterator it = m_materialDataMap.begin();
		for (;it!= m_materialDataMap.end();++it)
		{
			delete it->second;
		}
		m_materialDataMap.clear();
	}

	BaseMaterial* MaterialTemplateManager::GenerateMaterialFromData(MaterialData* data, int mode /*= 0*/)
	{
		BaseMaterial* ret = nullptr;
		if (m_resourceManager && data)
		{
			string name = data->DisplayName();
			int datetype = data->DataType();

			string keyStr = "";
			 if (mode == 0)
			 {
				 keyStr = name;
				 ret = m_resourceManager->GetMaterial(name);				
			 }
			 else if (mode == 1)
			 {
				 keyStr = name + "_uuid_" + IDCreator::GetUUID();
				 ret = m_resourceManager->GetMaterial(keyStr);				 
			 }
			
			 if (!ret)
			 {
				 ret = m_resourceManager->GetOrCreateMaterial(keyStr, datetype);
				 ret->DisplayName(name);

				 if (datetype == MaterialType_Base || datetype == MaterialType_Phong)
				 {
                     PhongMaterialData* tempMateriaData = static_cast<PhongMaterialData*>(data);
                     if (tempMateriaData)
                     {
                         
                         Material* tempMaterial = static_cast<Material*>(ret);
                         if(tempMaterial)
                         {
                             Color tempColor = tempMateriaData->DiffuseColor();
                             tempMaterial->SetDiffuse(tempColor);
                             
                             tempColor = tempMateriaData->AmbientColor();
                             tempMaterial->SetAmbient(tempColor);
                             
                             tempColor = tempMateriaData->EmissiveColor();
                             tempMaterial->SetEmissive(tempColor);
                             
                             tempColor = tempMateriaData->SpecularColor();
                             tempMaterial->setSpecular(tempColor);
                             
                             
                             tempMaterial->Opacity(tempMateriaData->Opcity());
                             tempMaterial->SetShininess(tempMateriaData->Shininess());
                             
                         }
                         
                         
                     }

				 }
				 else if (datetype == MaterialType_Pbr)
				 {
                     PhysicalMaterialData* tempMateriaData = static_cast<PhysicalMaterialData*>(data);
                     if (tempMateriaData)
                     {
                         
                         PbrMaterial* tempMaterial = static_cast<PbrMaterial*>(ret);
                         if(tempMaterial)
                         {
                             Color tempColor = tempMateriaData->DiffuseColor();
                             tempMaterial->AlbedoColor(tempColor);
                             
                             tempColor = tempMateriaData->EmissiveColor();
                             tempMaterial->EmissiveColor(tempColor);
                             
                             tempMaterial->Opacity(tempMateriaData->Opcity());
                             tempMaterial->MetalnessFactor(tempMateriaData->Metallic());
                             tempMaterial->RougthnessFactor(tempMateriaData->Roughness());
                             
                         }
                         
                         
                     }

				 }
				 else if (datetype == MaterialType_Shader)
				 {
				 }
				 else if (datetype == 101 ||//玉石
					 datetype == 102 ||//珍珠
					 datetype == 103 ||//晶石
					 datetype == 104//金属
					 )
				 {
					 ShaderMaterialData* tempMateriaData = static_cast<ShaderMaterialData*>(data);
					 if (tempMateriaData)
					 {
						 map<string, std::pair<string, string> >& parametersMap = tempMateriaData->ParametersMap();
						 UniformHelper::ParseMaterialParameters(m_resourceManager, ret, parametersMap);
					 }

				 }
			 }
		}
		return ret;

	}

	BaseMaterial* MaterialTemplateManager::GetMaterialFromTemplateByName(string name, int mode)
	{
		BaseMaterial* ret = nullptr;
		size_t pos = name.find("materialTemplate_");
		if (pos == string::npos || pos != 0)
		{
			name = "materialTemplate_" + name;
		}
		map<string, MaterialData*>::iterator it = m_materialDataMap.find(name);
		if (it != m_materialDataMap.end())
		{
			MaterialData* materialData = it->second;

			ret = GenerateMaterialFromData(materialData,mode);
		}

		return ret;
	}

	map<string, MaterialData*>& MaterialTemplateManager::GetAllMaterialData()
	{
		// TODO: 在此处插入 return 语句
		return m_materialDataMap;
	}

}

