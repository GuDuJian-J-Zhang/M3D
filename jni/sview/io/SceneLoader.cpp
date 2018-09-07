//
//  StlWriter.cpp
//  M3D
//
//  Created by administrator on 15/3/2.
//  Copyright (c) 2015年 hoteam. All rights reserved.
//

#include "m3d/ResourceManager.h"
#include "JsonMacro.h"
#include "document.h"
#include "stringbuffer.h"
#include "prettywriter.h"
#include "allocators.h"
#include "writer.h"
using rapidjson::Value;
using rapidjson::Document;
using rapidjson::StringBuffer;
using rapidjson::PrettyWriter;
using rapidjson::Writer;
using rapidjson::StringRef;
using rapidjson::MemoryPoolAllocator;

#include "sview/io/SceneLoader.h"
#include "sview/views/View.h"
#include "m3d/utils/StringHelper.h"
#include "m3d/graphics/Texture.h"

#include "sview/views/Parameters.h"
#include "m3d/SceneManager.h"

namespace SVIEW
{
	SceneLoader::SceneLoader(View* view)
	{
		m_view = view;
	}

	SceneLoader::~SceneLoader()
	{

	}

	void SceneLoader::Load(const string &sceneInfo)
	{
		if (m_view)
		{
			if (!sceneInfo.empty())
			{
				Document m_nodeDoc;

				m_nodeDoc.Parse<0>(sceneInfo.c_str());
				if (m_nodeDoc.HasParseError() || !m_nodeDoc.IsObject()) {
					return;
				}
				Value& valBom = m_nodeDoc["scene"]["background"];

				rapidjson::StringBuffer buffer;

				buffer.Clear();

				rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
				valBom.Accept(writer);

				this->Loadbackground(buffer.GetString());
			}
		}
	}

	void SceneLoader::Loadbackground(const string& backgrondInfo)
	{
		if (m_view)
		{
			if (!backgrondInfo.empty())
			{
				Document m_nodeDoc;

				m_nodeDoc.Parse<0>(backgrondInfo.c_str());
				if (m_nodeDoc.HasParseError() || !m_nodeDoc.IsObject()) {
					return;
				}

				Value::MemberIterator bomIter = m_nodeDoc.FindMember("type");
				Value::MemberIterator backStyle = m_nodeDoc.FindMember("style");
				Value& items = m_nodeDoc["items"];
				string name = m_nodeDoc["name"].GetString();
				//if ((bomIter != m_nodeDoc.MemberEnd() && bomIter->value.IsObject()))
				{
					Value* valBom = &bomIter->value;
					Value* valBackStype = &backStyle->value;
					int type = valBom->GetInt();
					int style = valBackStype->GetInt();
					if (type == 1)//颜色
					{
						if (style == 1)
						{
							/*map<string, string> items = infoFilePtr->GetBackgroundItems();
							for (map<string, string>::iterator it = items.begin(); it != items.end(); it++)
							{
								string name = it->first;
								string val = it->second;
								vector<string> colors = StringHelper::Split(val, ",");

								vector<string> tops = StringHelper::Split(colors[0], " ");
								Color top(StringHelper::StringToFloat(tops[0]), StringHelper::StringToFloat(tops[1]), StringHelper::StringToFloat(tops[2]));
								vector<string> bottoms = StringHelper::Split(colors[1], " ");
								Color bottom(StringHelper::StringToFloat(bottoms[0]), StringHelper::StringToFloat(bottoms[1]), StringHelper::StringToFloat(bottoms[2]));
								this->m_view->SetBackgroundColor(top, bottom);

							}
							this->m_view->SetBackgroundUseImage(false);
							this->m_view->SetBackgroundUseSkyBox(false);*/
						}
					}
					else if (type == 2)//图片
					{
						if (style == 1)
						{
							//map<string, string> items = infoFilePtr->GetBackgroundItems();
							//for (map<string, string>::iterator it = items.begin(); it != items.end(); it++)
							//{
							//	string name = it->first;
							//	string val = it->second;
							//	unsigned int buffersize = 0;
							//	char* data;
							//	(*m_svl2Doc)->GetSVLXFileItem(val, buffersize, &data, false);
							//	//将数据赋值给图片对象					

							//	Texture2D* texture2d = (Texture2D*)ResourceManager::GetInstance()->GetOrCreateTexture(val, Texture::TEXTURE_2D);
							//	texture2d->SetImageParameter(TEXTURE_LOAD_RGBA, TEXTURE_FLAG_MIPMAPS | TEXTURE_FLAG_INVERT_Y);
							//	//构造image，按道理来说image也应该重用
							//	Image* image = new Image();
							//	image->SetData((unsigned char *)(data), buffersize);
							//	texture2d->SetImage(image);
							//	this->m_view->SetBackgroundTexture(texture2d);

							//}
							//this->m_view->SetBackgroundUseImage(true);
							//this->m_view->SetBackgroundUseSkyBox(false);
						}
					}
					else if (type == 3)//天空盒
					{
						if (style == 1)
						{
							Value& imageSrc = items["imageSourceReleative"];

							vector<string> fullPaths;
							vector<string> pathes = M3D::StringHelper::Split(imageSrc.GetString(), ",");
							for (int i = 0; i < pathes.size(); i++)
							{
								//去除首尾空格 
								if (!pathes[i].empty())
								{
									pathes[i].erase(0, pathes[i].find_first_not_of(" "));
									pathes[i].erase(pathes[i].find_last_not_of(" ") + 1);
								}
								fullPaths.push_back(Parameters::Instance()->m_appWorkPath + "/" + pathes[i]);
							}
							if (fullPaths.size() == 6)
							{
								M3D::Texture * cubeTexture = this->m_view->GetSceneManager()->GetResourceManager()->GetOrCreateCubeMappingTexture(name, fullPaths);
								this->m_view->AddBackgroundSkyBoxTexture(name, cubeTexture);

								this->m_view->SetBackgroundUseImage(false);
								this->m_view->SetBackgroundUseSkyBox(true);
                                this->m_view->SetInterBackgroundState(true);
							}
						}
					}
				}
			}
		}
	}

}
