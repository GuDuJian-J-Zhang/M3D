#include "m3d/ResourceManager.h"

#include "m3d/M3D.h"

#include "m3d/model/Image.h"

#include "m3d/model/ComText.h"
#include "m3d/model/Edge.h"
#include "m3d/model/Model.h"
#include "m3d/model/Face.h"
#include "m3d/model/Body.h"
#include "m3d/model/Curve.h"

#include "m3d/utils/ImageHelper.h"

#include "m3d/graphics/Texture.h"

#include "m3d/graphics/Material.h"

#include "m3d/graphics/Texture2D.h"
#include "m3d/graphics/TextureCube.h"

#include "prebuilt/soil/include/SOIL.h"

#include "sview/views/Parameters.h"
#include "m3d/graphics/GeometryBuffer.h"

#include "m3d/base/FileCacheManager.h"

using namespace SVIEW;

namespace M3D
{
const int InternationalManager::ZH_rCN = 0;
const int InternationalManager::EN_rUN = 1;
InternationalManager::InternationalManager()
{
	/*-----------------------------------Chinese Sample-------------------------------------------*/
	float cnParameter = 45.0f;
	m_values_zh_rCN.insert(std::make_pair("PointPointDistance",MeasureTypeValue("点点距离",4 * cnParameter)));
	m_values_zh_rCN.insert(std::make_pair("PointLineToPolyLineDistance",MeasureTypeValue("点到折线距离",6 * cnParameter)));
	m_values_zh_rCN.insert(std::make_pair("PointLineProjectDistance",MeasureTypeValue("投影距离",4 * cnParameter)));
	m_values_zh_rCN.insert(std::make_pair("PointLineMinDistance",MeasureTypeValue("最短距离",4 * cnParameter)));
	m_values_zh_rCN.insert(std::make_pair("PointLineToTerminalMaxDistance",MeasureTypeValue("端点最大距离",6 * cnParameter)));
	m_values_zh_rCN.insert(std::make_pair("PointLineToTerminalMinDistance",MeasureTypeValue("端点最小距离",6 * cnParameter)));
	m_values_zh_rCN.insert(std::make_pair("PointFaceProjectDistance",MeasureTypeValue("投影距离",4 * cnParameter)));
	m_values_zh_rCN.insert(std::make_pair("PointFaceMaxDistance",MeasureTypeValue("最大距离",4* cnParameter)));
	m_values_zh_rCN.insert(std::make_pair("PointFaceMinDistance",MeasureTypeValue("最小距离",4 * cnParameter)));
	m_values_zh_rCN.insert(std::make_pair("LineLineParallelDistance",MeasureTypeValue("平行距离",4* cnParameter)));
	m_values_zh_rCN.insert(std::make_pair("LineLineMinDistance",MeasureTypeValue("最短距离",4 * cnParameter)));
	m_values_zh_rCN.insert(std::make_pair("LineLineSkewLinesDistance",MeasureTypeValue("公垂线段",4 * cnParameter)));
	m_values_zh_rCN.insert(std::make_pair("LineLineCirclesCenterDistance",MeasureTypeValue("圆心距",3 * cnParameter)));
	m_values_zh_rCN.insert(std::make_pair("LineLineWheelbase",MeasureTypeValue("轴距",2 * cnParameter)));
	m_values_zh_rCN.insert(std::make_pair("LineLineRadiusDifference",MeasureTypeValue("半径差",3 * cnParameter)));
	m_values_zh_rCN.insert(std::make_pair("LineFaceDistance",MeasureTypeValue("线面距离",4 * cnParameter)));
	m_values_zh_rCN.insert(std::make_pair("FaceFaceDistance",MeasureTypeValue("面面距离",4 * cnParameter)));
	m_values_zh_rCN.insert(std::make_pair("InvalidMeasureObject",MeasureTypeValue("无效测量对象",6 * cnParameter)));
	m_values_zh_rCN.insert(std::make_pair("LineLineAngle",MeasureTypeValue("线线夹角",4 * cnParameter)));
	m_values_zh_rCN.insert(std::make_pair("LineFaceAngle",MeasureTypeValue("线面夹角",4 * cnParameter)));
	m_values_zh_rCN.insert(std::make_pair("FaceFaceAngle",MeasureTypeValue("面面夹角",4 * cnParameter)));
	m_values_zh_rCN.insert(std::make_pair("PointPropertiesCoordinater",MeasureTypeValue("点坐标",3 * cnParameter)));
	m_values_zh_rCN.insert(std::make_pair("LinePropertiesLength",MeasureTypeValue("长度",3 * cnParameter)));
	m_values_zh_rCN.insert(std::make_pair("LinePropertiesCircleCenter",MeasureTypeValue("圆心",2 * cnParameter)));
	m_values_zh_rCN.insert(std::make_pair("LinePropertiesCircleArcLength",MeasureTypeValue("弧长",2 * cnParameter)));
	m_values_zh_rCN.insert(std::make_pair("LinePropertiesCircleRadius",MeasureTypeValue("半径",2 * cnParameter)));
	m_values_zh_rCN.insert(std::make_pair("FacePropertiesArea",MeasureTypeValue("面积",2 * cnParameter)));
	m_values_zh_rCN.insert(std::make_pair("FacePropertiesVolume",MeasureTypeValue("体积",2 * cnParameter)));
	m_values_zh_rCN.insert(std::make_pair("FacePropertiesTrianglePatchNumber",MeasureTypeValue("面片数量",4 * cnParameter)));
	m_values_zh_rCN.insert(std::make_pair("ModelPropertiesName",MeasureTypeValue("名称",2 * cnParameter)));
	m_values_zh_rCN.insert(std::make_pair("ModelPropertiesTrianglePatchNumber",MeasureTypeValue("三角面片",4 * cnParameter)));

	m_values_zh_rCN.insert(std::make_pair("LinePropertiesName",MeasureTypeValue("名称")));

	//-------------------模型
	m_values_zh_rCN.insert(std::make_pair("ModelPropertiesID",MeasureTypeValue("ID")));
	m_values_zh_rCN.insert(std::make_pair("ModelPropertiesPlaceID",MeasureTypeValue("PlaceID")));
	m_values_zh_rCN.insert(std::make_pair("ModelPropertiesProtoTypeID",MeasureTypeValue("ProtoTypeID")));
	m_values_zh_rCN.insert(std::make_pair("ModelPropertiesColor",MeasureTypeValue("颜色")));
	m_values_zh_rCN.insert(std::make_pair("ModelPropertiesSubModelNumber",MeasureTypeValue("子模型数量")));
	m_values_zh_rCN.insert(std::make_pair("ModelPropertiesInstanceCount",MeasureTypeValue("实例数量")));
	m_values_zh_rCN.insert(std::make_pair("ModelPropertiesModleViewCount",MeasureTypeValue("视图数量")));
	m_values_zh_rCN.insert(std::make_pair("ModelPropertiesPMICount",MeasureTypeValue("PMI数量")));
	m_values_zh_rCN.insert(std::make_pair("ModelPropertiesLOD0PatchNumber",MeasureTypeValue("LOD0 面片数量",4 * cnParameter)));
	m_values_zh_rCN.insert(std::make_pair("ModelPropertiesLOD1PatchNumber",MeasureTypeValue("LOD1 面片数量",4 * cnParameter)));
	//-------------------面
	m_values_zh_rCN.insert(std::make_pair("FacePropertiesName",MeasureTypeValue("名称",2 * cnParameter)));
	m_values_zh_rCN.insert(std::make_pair("FacePropertiesID",MeasureTypeValue("ID")));
	m_values_zh_rCN.insert(std::make_pair("FacePropertiesColor",MeasureTypeValue("颜色")));
	m_values_zh_rCN.insert(std::make_pair("FacePropertiesLOD0PatchNumber",MeasureTypeValue("LOD0 面片数量",4 * cnParameter)));
	m_values_zh_rCN.insert(std::make_pair("FacePropertiesLOD1PatchNumber",MeasureTypeValue("LOD1 面片数量",4 * cnParameter)));
	//-------------------线
	m_values_zh_rCN.insert(std::make_pair("EdgePropertiesName",MeasureTypeValue("名称",2 * cnParameter)));
	m_values_zh_rCN.insert(std::make_pair("EdgePropertiesID",MeasureTypeValue("ID")));
	m_values_zh_rCN.insert(std::make_pair("EdgePropertiesColor",MeasureTypeValue("颜色")));
	m_values_zh_rCN.insert(std::make_pair("EdgePropertiesLOD0PointNumber",MeasureTypeValue("LOD0边界线点数",4 * cnParameter)));
	m_values_zh_rCN.insert(std::make_pair("EdgePropertiesLOD1PointNumber",MeasureTypeValue("LOD1 边界线点数",4 * cnParameter)));

	//-------------------点

	/*------------------------------------------American English-------------------------------------------*/

	float enParameter = 21.0f;
	m_values_en_rUS.insert(std::make_pair("PointPointDistance",MeasureTypeValue("Point Point",12*enParameter)));
	m_values_en_rUS.insert(std::make_pair("PointLineToPolyLineDistance",MeasureTypeValue("Point PolyLine",15*enParameter)));
	m_values_en_rUS.insert(std::make_pair("PointLineProjectDistance",MeasureTypeValue("Project",10*enParameter)));
	m_values_en_rUS.insert(std::make_pair("PointLineMinDistance",MeasureTypeValue("Minimum",10*enParameter)));
	m_values_en_rUS.insert(std::make_pair("PointLineToTerminalMaxDistance",MeasureTypeValue("Minimum To Terminal",20*enParameter)));
	m_values_en_rUS.insert(std::make_pair("PointLineToTerminalMinDistance",MeasureTypeValue("Maximum To Terminal",20*enParameter)));
	m_values_en_rUS.insert(std::make_pair("PointFaceProjectDistance",MeasureTypeValue("Project",8*enParameter)));
	m_values_en_rUS.insert(std::make_pair("PointFaceMaxDistance",MeasureTypeValue("Minimum",9*enParameter)));
	m_values_en_rUS.insert(std::make_pair("PointFaceMinDistance",MeasureTypeValue("Maximum",9*enParameter)));
	m_values_en_rUS.insert(std::make_pair("LineLineParallelDistance",MeasureTypeValue("Parallel",9*enParameter)));
	m_values_en_rUS.insert(std::make_pair("LineLineMinDistance",MeasureTypeValue("Minimum",10*enParameter)));
	m_values_en_rUS.insert(std::make_pair("LineLineSkewLinesDistance",MeasureTypeValue("Skew Lines",11*enParameter)));
	m_values_en_rUS.insert(std::make_pair("LineLineCirclesCenterDistance",MeasureTypeValue("Center",7*enParameter)));
	m_values_en_rUS.insert(std::make_pair("LineLineWheelbase",MeasureTypeValue("Wheel",6*enParameter)));
	m_values_en_rUS.insert(std::make_pair("LineLineRadiusDifference",MeasureTypeValue("Radius Difference",17*enParameter)));
	m_values_en_rUS.insert(std::make_pair("LineFaceDistance",MeasureTypeValue("Line Face",10*enParameter)));
	m_values_en_rUS.insert(std::make_pair("FaceFaceDistance",MeasureTypeValue("Face Face",10*enParameter)));
	m_values_en_rUS.insert(std::make_pair("InvalidMeasureObject",MeasureTypeValue("Invalid",7*enParameter)));
	m_values_en_rUS.insert(std::make_pair("LineLineAngle",MeasureTypeValue("Line Line",10*enParameter)));
	m_values_en_rUS.insert(std::make_pair("LineFaceAngle",MeasureTypeValue("Line Face",10*enParameter)));
	m_values_en_rUS.insert(std::make_pair("FaceFaceAngle",MeasureTypeValue("Face Face",10*enParameter)));
	m_values_en_rUS.insert(std::make_pair("PointPropertiesCoordinater",MeasureTypeValue("Point Coordinates",17*enParameter)));
	m_values_en_rUS.insert(std::make_pair("LinePropertiesLength",MeasureTypeValue("Length",6*enParameter)));
	m_values_en_rUS.insert(std::make_pair("LinePropertiesCircleCenter",MeasureTypeValue("Center",8*enParameter)));
	m_values_en_rUS.insert(std::make_pair("LinePropertiesCircleArcLength",MeasureTypeValue("Length",6*enParameter)));
	m_values_en_rUS.insert(std::make_pair("LinePropertiesCircleRadius",MeasureTypeValue("Radius",6*enParameter)));
	m_values_en_rUS.insert(std::make_pair("FacePropertiesArea",MeasureTypeValue("Area",4*enParameter)));
	m_values_en_rUS.insert(std::make_pair("FacePropertiesVolume",MeasureTypeValue("Volume",4*enParameter)));
	m_values_en_rUS.insert(std::make_pair("FacePropertiesTrianglePatchNumber",MeasureTypeValue("Triangle Patch Number",21*enParameter)));
	m_values_en_rUS.insert(std::make_pair("ModelPropertiesName",MeasureTypeValue("Name",4*enParameter)));
	m_values_en_rUS.insert(std::make_pair("ModelPropertiesTrianglePatchNumber",MeasureTypeValue("Triangle Patch Number",21*enParameter)));

	//-------------------模型
	m_values_en_rUS.insert(std::make_pair("ModelPropertiesID",MeasureTypeValue("ID",2*enParameter)));
	m_values_en_rUS.insert(std::make_pair("ModelPropertiesPlaceID",MeasureTypeValue("PlaceID",7*enParameter)));
	m_values_en_rUS.insert(std::make_pair("ModelPropertiesProtoTypeID",MeasureTypeValue("ProtoTypeID",11*enParameter)));
	m_values_en_rUS.insert(std::make_pair("ModelPropertiesColor",MeasureTypeValue("Color",5*enParameter)));
	m_values_en_rUS.insert(std::make_pair("ModelPropertiesSubModelNumber",MeasureTypeValue("Child Model Number")));
	m_values_en_rUS.insert(std::make_pair("ModelPropertiesInstanceCount",MeasureTypeValue("Instance Number")));
	m_values_en_rUS.insert(std::make_pair("ModelPropertiesModleViewCount",MeasureTypeValue("Modelview Number")));
	m_values_en_rUS.insert(std::make_pair("ModelPropertiesPMICount",MeasureTypeValue("PMI Number")));
	m_values_en_rUS.insert(std::make_pair("ModelPropertiesLOD0PatchNumber",MeasureTypeValue("LOD0 Patch Number",4 * cnParameter)));
	m_values_en_rUS.insert(std::make_pair("ModelPropertiesLOD1PatchNumber",MeasureTypeValue("LOD1 patch Number",4 * cnParameter)));
	//-------------------面
	m_values_en_rUS.insert(std::make_pair("FacePropertiesName",MeasureTypeValue("Name",2 * cnParameter)));
	m_values_en_rUS.insert(std::make_pair("FacePropertiesID",MeasureTypeValue("ID")));
	m_values_en_rUS.insert(std::make_pair("FacePropertiesColor",MeasureTypeValue("Color")));
	m_values_en_rUS.insert(std::make_pair("FacePropertiesLOD0PatchNumber",MeasureTypeValue("LOD0 Patch Number",4 * cnParameter)));
	m_values_en_rUS.insert(std::make_pair("FacePropertiesLOD1PatchNumber",MeasureTypeValue("LOD1 Patch Number",4 * cnParameter)));
	//-------------------线
	m_values_en_rUS.insert(std::make_pair("EdgePropertiesName",MeasureTypeValue("Name",2 * cnParameter)));
	m_values_en_rUS.insert(std::make_pair("EdgePropertiesID",MeasureTypeValue("ID")));
	m_values_en_rUS.insert(std::make_pair("EdgePropertiesColor",MeasureTypeValue("Color")));
	m_values_en_rUS.insert(std::make_pair("EdgePropertiesLOD0PointNumber",MeasureTypeValue("LOD0 Edge Point Number",4 * cnParameter)));
	m_values_en_rUS.insert(std::make_pair("EdgePropertiesLOD1PointNumber",MeasureTypeValue("LOD1 Edge Point Number",4 * cnParameter)));

}
InternationalManager::~InternationalManager()
{

}

int InternationalManager::GetCurrentLanguage()
{
	return m_language;
}
void InternationalManager::SetDefaultLangrage()
{
	m_currentLanguage = &m_values_zh_rCN;
}

void InternationalManager::SetLanguage(int language)
{
	switch(language)
	{
	case ZH_rCN:
		m_language = language;
		m_currentLanguage = &m_values_zh_rCN;
		break;
	case EN_rUN:
		m_language = language;
		m_currentLanguage = &m_values_en_rUS;
		break;
	}
}

string InternationalManager::GetCurrentTypeValue(string key)
{
	string ret = "";
	map<string,MeasureTypeValue>::iterator it;
	it = m_currentLanguage->find(key);
	if(it != m_currentLanguage->end())
	{
		ret =  it->second.GetTypeValue();
	}
	return ret;
}

float InternationalManager::GetCurrentTypeValueLength(string key)
{
	float ret = 100.0f;
	map<string,MeasureTypeValue>::iterator it;
	it = m_currentLanguage->find(key);
	if(it != m_currentLanguage->end())
	{
		ret =  it->second.GetLength();
	}
	return ret;
}
MeasureTypeValue *InternationalManager:: GetCurrentMeasureTypeValue(string key)
{
	MeasureTypeValue * ret = NULL;

	map<string,MeasureTypeValue>::iterator it;
	it = m_currentLanguage->find(key);
	if(it != m_currentLanguage->end())
	{
		ret =  &(it->second);
	}

	return ret;
}

ResourceManager* ResourceManager::Instance = NULL;

const string ResourceManager::VoiceImagePath = "/data/pic/voicenote.png";

Texture* ResourceManager::defaultVoiceImage = NULL;

Texture* ResourceManager::GetDefaultVoiceImage()
{
	if(!defaultVoiceImage)
	{
//		defaultVoiceImage = new Texture2D;
//
//		if(defaultVoiceImage && defaultVoiceImage->GetType() == Texture::TEXTURE_2D)
//		{
//			Texture2D* texture2d = (Texture2D*)defaultVoiceImage;
//			texture2d->SetImagePath(Parameters::Instance()->m_appWorkPath + VoiceImagePath);
//		}
		string imagePath = Parameters::Instance()->m_appWorkPath
				+ VoiceImagePath;
		defaultVoiceImage = this->GetOrCreateTexture(imagePath);
	}
	return defaultVoiceImage;
}

const string ResourceManager::PntImagePath = "/data/pic/pointx.png";

const string ResourceManager::PointOImagePath = "/data/pic/pointo.png";

const string ResourceManager::PntR2ImagePath = "/data/pic/pointr2.png";

Texture* ResourceManager::defaultPntTexture = NULL;

Texture* ResourceManager::defaultPntTextureO = NULL;

Texture* ResourceManager::defaultPntTexturer2 = NULL;

Texture* ResourceManager::defaultSphereTexture = NULL;
const string ResourceManager::defaultSphereTexPath = "/data/pic/pointo.png";

Texture* ResourceManager::defaultCubeMapTexture = NULL;
const string ResourceManager::defaultCubeMapTexPath = "/data/pic/";

Texture* ResourceManager::GetDefaultPointTexture(int type)
{
	Texture* texture = NULL;
	if (type == 1)
	{
		if (!defaultPntTextureO)
		{
//			defaultPntTextureO = new Texture2D;
//
//			if (defaultPntTextureO
//					&& defaultPntTextureO->GetType() == Texture::TEXTURE_2D)
//			{
//				Texture2D* texture2d = (Texture2D*) defaultPntTextureO;
//				texture2d->SetImagePath(
//						Parameters::Instance()->m_appWorkPath
//								+ PointOImagePath);
//			}
			string imagePath = Parameters::Instance()->m_appWorkPath
					+ PointOImagePath;
			defaultPntTextureO = this->GetOrCreateTexture(imagePath);
		}
		texture = defaultPntTextureO;
	}
	else if (type == 2)
	{
		if (!defaultPntTexture)
		{
//			defaultPntTexture = new Texture2D;
//
//			if (defaultPntTexture
//					&& defaultPntTexture->GetType() == Texture::TEXTURE_2D)
//			{
//				Texture2D* texture2d = (Texture2D*) defaultPntTexture;
//				texture2d->SetImagePath(
//						Parameters::Instance()->m_appWorkPath + PntImagePath);
//			}
			string imagePath = Parameters::Instance()->m_appWorkPath
					+ PntImagePath;
			defaultPntTexture = this->GetOrCreateTexture(imagePath);
		}
		texture = defaultPntTexture;
	}
	else if (type == 3)
	{
		if(!defaultPntTexturer2)
		{
//			defaultPntTexturer2 = new Texture2D;
//
//			if(defaultPntTexturer2 && defaultPntTexturer2->GetType() == Texture::TEXTURE_2D)
//			{
//				Texture2D* texture2d = (Texture2D*)defaultPntTexturer2;
//				texture2d->SetImagePath(Parameters::Instance()->m_appWorkPath + PntR2ImagePath);
//			}
			string imagePath = Parameters::Instance()->m_appWorkPath
					+ PntR2ImagePath;
			defaultPntTexturer2 = this->GetOrCreateTexture(imagePath);
		}
		texture = defaultPntTexturer2;
	}

	return texture;
}

Texture * ResourceManager::GetDefaultSphereMap()
{
	Texture* texture = NULL;
	if (!defaultSphereTexture)
	{
		string imagePath = Parameters::Instance()->m_appWorkPath + defaultSphereTexPath;
		defaultSphereTexture = this->GetOrCreateTexture(imagePath);
	}
	texture = defaultSphereTexture;

	return texture;
}

//ResourceManager* ResourceManager::GetInstance()
//{
//	if (Instance == NULL)
//	{
//		Instance = new ResourceManager();
//	}
//	return Instance;
//}

ResourceManager::ResourceManager()
{
	memset(m_OGLObjIDS,1,OGLOBJ_MAX_NUM*sizeof(true));

	cubeMappingTexture=NULL;

	m_fileCacheMgr = NULL;

	this->Clear();
}

ResourceManager::~ResourceManager()
{
	this->Clear();

	delete m_fileCacheMgr;
	m_fileCacheMgr = NULL;
}

Texture* ResourceManager::GetTexture(string key)
{
	Texture* texture = NULL;
    LOGI("m_allTextures.size %d",m_allTextures.size());
    if (m_allTextures.size() > 0) {
        map<string, Texture*>::iterator it = m_allTextures.find(key);
        
        if (it != m_allTextures.end())
        {
            texture = it->second;
        }
    }
	return texture;
}

Texture* ResourceManager::GetOrCreateTexture(string key, int type)
{
	Texture* texture =  this->GetTexture(key);
	if(!texture)
	{
		switch(type)
		{
		case Texture::TEXTURE_BASE:
			texture = new Texture2D();
			break;
		case Texture::TEXTURE_2D:
			texture = new Texture2D();
			break;
		case Texture::TEXTURE_3D:
			texture = new Texture2D();
			break;
		case Texture::TEXTURE_GEO:
			texture = new GeometryBuffer();
			break;
		default:
			break;
		}
		texture->SetResourceManager(this);
		texture->SetName(key);
		texture->SetImagePath(key);
		this->AddTexture(key,texture);
	}

	return texture;
}
bool ResourceManager::AddTexture(string key, Texture* texture)
{
	bool addState = false;

	map<string, Texture*>::iterator it = m_allTextures.find(key);

	if (it == m_allTextures.end())
	{
		m_allTextures.insert(map<string, Texture*>::value_type(key, texture));
		texture->AddRef();
		addState = true;
	}
//	else
//	{
//		//先删除在添加
//		addState = true;
//		Texture* oldTexture = it->second;
//		delete oldTexture;
//
//		m_allTextures.insert(map<string, Texture*>::value_type(key, oldTexture));
//	}

	return addState;
}

bool ResourceManager::RemoveTexture(string key)
{
	bool removeState = false;

	map<string, Texture*>::iterator it = m_allTextures.find(key);

	if (it != m_allTextures.end())
	{
		it->second->Release();
		m_allTextures.erase(it);
		removeState = true;
	}

	return removeState;
}

Material* ResourceManager::GetMaterial(string key)
{
	Material* material = NULL;

	map<string, Material*>::iterator it = m_allMaterials.find(key);

	if (it != m_allMaterials.end())
	{
		material = it->second;
	}

	return material;
}

Material* ResourceManager::GetOrCreateMaterial(string key)
{
	Material* material = this->GetMaterial(key);
	if(!material)
	{
		material = new Material();
		material->SetName(key);
		material->SetResourceManager(this);

		this->AddMaterial(key,material);
	}

	return material;
}

bool ResourceManager::AddMaterial(string key,  Material* material)
{
	bool addState = false;

	map<string, Material*>::iterator it = m_allMaterials.find(key);

	if (it == m_allMaterials.end())
	{
		m_allMaterials.insert(map<string, Material*>::value_type(key, material));
		material->AddRef();
		addState = true;
	}

	return addState;
}

bool ResourceManager::RemoveMaterial(string key)
{
	bool removeState = false;

	map<string, Material*>::iterator it = m_allMaterials.find(key);

	if (it != m_allMaterials.end())
	{
		it->second->Release();
		m_allMaterials.erase(it);

		removeState = true;
	}

	return removeState;
}

Image* ResourceManager::GetImage(const string& path)
{
	Image* image = NULL;

	map<string, Image*>::iterator it = m_allImages.find(path);

	if (it != m_allImages.end())
	{
		image = it->second;
	}
	else
	{
//		image = ImageHelper::LoadImage(path);
//		if (image)
//		{
//			m_allImages.insert(map<string, Image*>::value_type(path, image));
//		}
	}

	return image;
}

bool ResourceManager::RemoveImage(const string& path)
{
	bool removeState = false;
	map<string, Image*>::iterator it = m_allImages.find(path);

	if (it != m_allImages.end())
	{
		m_allImages.erase(it);
		removeState = true;
	}

	return removeState;
}

unsigned int ResourceManager::GetOGLTexture(const string& path)
{
	unsigned int OGLObj = 0;

	map<string, unsigned int>::iterator it = m_allOGLTextures.find(path);

	if (it != m_allOGLTextures.end())
	{
		OGLObj = it->second;
	}

	return OGLObj;
}

bool ResourceManager::AddOGLTexture(const string& path, unsigned int OGLObj)
{
	bool addState = false;

	map<string, unsigned int>::iterator it = m_allOGLTextures.find(path);

	if (it == m_allOGLTextures.end())
	{
		m_allOGLTextures.insert(
				map<string, unsigned int>::value_type(path, OGLObj));

		addState = true;

	}
	else
	{
		//先删除在添加
		addState = true;
		unsigned int oldTexture = it->second;

		m_allOGLTextures.insert(
				map<string, unsigned int>::value_type(path, oldTexture));
	}

	return addState;
}

bool ResourceManager::RemoveOGLTexture(const string& path)
{
	bool removeState = false;

	map<string, unsigned int>::iterator it = m_allOGLTextures.find(path);

	if (it != m_allOGLTextures.end())
	{
		m_allOGLTextures.erase(it);
		removeState = true;
	}

	return removeState;
}

void  ResourceManager::DeleteOGLObjID(int id)
{
	if (id >= 0 && id < OGLOBJ_MAX_NUM)
	{
		this->m_OGLObjIDS[id] = true;
//		LOGE("DeleteOGLObjID %d",id);
	}
}

int  ResourceManager::NewOGLObjID()
{
	int id = 0;
	for(int i= 1; i< OGLOBJ_MAX_NUM;i++)
	{
		if(this->m_OGLObjIDS[i])
		{
			LOGE("NewOGLObjID %d",i);
			this->m_OGLObjIDS[i] = false;
			id = i;
			break;
		}
	}
	return id;
}

bool ResourceManager::AddPMI(int key,PMIData* pmiData)
{
	bool addState = false;


	return addState;
}

bool ResourceManager::RemovePMI(int key)
{
	bool removeState = false;


	return removeState;
}

PMIData* ResourceManager::GetPMI(int key)
{
	PMIData* pmiData = NULL;


	return pmiData;
}

bool ResourceManager::AddView(int key,ModelView* modelView)
{
	bool addState = false;


	return addState;
}

bool ResourceManager::RemoveView(int key)
{
	bool removeState = false;


	return removeState;
}

ModelView* ResourceManager::GetView(int key)
{
	ModelView* modelView = NULL;


	return modelView;
}

void ResourceManager::SetAnimationData(const string& xmlData)
{
	m_xmlAnimatinData = xmlData;
}

string& ResourceManager::GetAnimationData()
{
	return m_xmlAnimatinData;
}

bool ResourceManager::AddModel(int key,Model* model)
{
	bool addState = false;

	if(!this->GetModel(key))
	{
		m_ModelMap.insert(pair<int, Model*>(key, model));

		addState = true;
	}

	return addState;
}

bool ResourceManager::RemoveModel(int key)
{
    bool removeState = false;
	Model* model;
	map<int, Model*>::iterator iter;
	iter = m_ModelMap.find(key);
	if (iter != m_ModelMap.end())
	{
		m_ModelMap.erase(iter);
		model = iter->second;
		delete model;
        removeState = true;
	}
    return removeState;
}

Model* ResourceManager::GetModel(int key)
{
	Model* model = NULL;

	map<int, Model*>::iterator iter;
	iter = m_ModelMap.find(key);
	if (iter != m_ModelMap.end())
	{
		model = iter->second;
	}

	return model;
}

void ResourceManager::Clear()
{
	//释放材质资源
	map<string, Material*>::iterator materialIt = m_allMaterials.begin();

	vector<Material*> materialsCaches;
	while (materialIt != m_allMaterials.end())
	{
		Material* material = materialIt->second;
		if (material != NULL)
		{
			materialsCaches.push_back(material);
		}
		materialIt++;
	}

	for(int i=0;i<materialsCaches.size();i++)
	{
			Material* material = materialsCaches[i];
			material->Release();
	}

	m_allMaterials.clear();

	//释放纹理资源
	vector<Texture*> textureCaches;
	map<string, Texture*>::iterator textureIt = m_allTextures.begin();
	while (textureIt != m_allTextures.end())
	{
		Texture* texture = textureIt->second;
		if (texture != NULL)
		{
			textureCaches.push_back(texture);
		}
		textureIt++;
	}
    m_allTextures.clear();
    
	for(int i=0;i<textureCaches.size();i++)
	{
		Texture* material = textureCaches[i];
		material->Release();
	}
	

	//释放图片资源
//	map<string, Image*>::iterator imageIt = m_allImages.begin();

//	while (imageIt != m_allImages.end())
//	{
//		Image* image = imageIt->second;
//		if (image != NULL)
//		{
//			delete image;
//			imageIt->second = NULL;
//		}
//
//		imageIt++;
//	}

	m_allImages.clear();

	//
//	map<string, unsigned int>::iterator oglit = m_allOGLTextures.begin();
//	while (oglit != m_allOGLTextures.end())
//	{
//		oglit->second = 0;
//		oglit++;
//	}

	m_allOGLTextures.clear();

	//清空动画数据
	this->SetAnimationData("");

	defaultVoiceImage = NULL;
	defaultPntTexture = NULL;
	defaultPntTextureO = NULL;
	defaultPntTexturer2 = NULL;
    
    defaultSphereTexture = NULL;
	if(m_fileCacheMgr)
	{
		m_fileCacheMgr->endCache();
	}
}

void ResourceManager::CreateCubeMappingTexture(const string& filePath)
{
	if(this->cubeMappingTexture == NULL){
		cubeMappingTexture = new TextureCube(filePath);
	}

	if(this->cubeMappingTexture->GetImagePath() != filePath)
	{
		delete cubeMappingTexture;

		cubeMappingTexture = new TextureCube(filePath);
	}
	cubeMappingTexture->SetResourceManager(this);
}

void ResourceManager::CreateCubeMappingTexture(string name ,vector<string> & filePathes)
{
	map<string,Texture*>::iterator it;
	it = m_cubeMappingTextures.find(name);
	if(it == m_cubeMappingTextures.end())
	{
		TextureCube * tempCubeMappingTexture = new TextureCube(filePathes);
		tempCubeMappingTexture->SetResourceManager(this);

		m_cubeMappingTextures.insert(make_pair(name,tempCubeMappingTexture));
	}
	else
	{
		LOGI("Exist %s texture");
		return;
	}

	LOGI("ResourceManager::CreateCubeMappingTexture END");

}

Texture * ResourceManager::GetDefaultCubeMap()
{
	Texture* texture = NULL;
	if (!defaultCubeMapTexture)
	{
		vector<string> filePathes;
		for(int i = 0;i<6;i++)
		{
		string imagePath = Parameters::Instance()->m_appWorkPath + defaultCubeMapTexPath+ IntToString(i)+".jpg";
		LOGE(imagePath.c_str());
		filePathes.push_back(imagePath);
		}
		defaultCubeMapTexture = new TextureCube(filePathes);

		string name = "default";
		m_cubeMappingTextures.insert(make_pair(name,defaultCubeMapTexture));
	}
	texture = defaultCubeMapTexture;

	texture->SetResourceManager(this);
	return texture;
}

Texture* ResourceManager::GetCubeMappingTexture(string key)
{
	//LOGI("ResourceManager::GetCubeMappingTexture BEGIN");
	Texture * ret = NULL;
	map<string,Texture*>::iterator it;

	it = m_cubeMappingTextures.find(key);

	if(it != m_cubeMappingTextures.end())
	{
		ret = it->second;
	}
	//LOGI("ResourceManager::GetCubeMappingTexture END");
	return ret;
}

Texture* ResourceManager::GetCubeMappingtexture()
{
	return this->cubeMappingTexture;
}

void ResourceManager::ReleaseGLObjects()
{
	//清除vbo对象
	for (int i = 0; i < m_GLVBOObjCache.size(); i++)
	{
		unsigned objectId = m_GLVBOObjCache[i];
		if (objectId > 0)
		{
//			LOGI("delete VBO");
			glDeleteBuffers(1, &objectId);
		}
	}
	this->m_GLVBOObjCache.clear();

	//清除纹理
	for (int i = 0; i < m_GLTextureObjCache.size(); i++)
	{
		unsigned objectId = m_GLTextureObjCache[i];
		if (objectId > 0)
		{
			glDeleteTextures(1, &objectId);
		}
	}
	this->m_GLTextureObjCache.clear();

	//清除帧缓存对象
	for (int i = 0; i < m_GLFrameBufferObjCache.size(); i++)
	{
		unsigned objectId = m_GLFrameBufferObjCache[i];
		if (objectId > 0)
		{
			glDeleteFramebuffers(1, &objectId);
		}
	}

	this->m_GLFrameBufferObjCache.clear();
	//清除RBO对象
	for (int i = 0; i < m_GLRenderBufferObjCache.size(); i++)
	{
		unsigned objectId = m_GLRenderBufferObjCache[i];
		if (objectId > 0)
		{
			glDeleteRenderbuffers(1, &objectId);
		}
	}
	this->m_GLRenderBufferObjCache.clear();
}

void ResourceManager::AddGLObject(unsigned objectId,int type)
{
	if(type == ResourceManager::VBO)
	{
		this->m_GLVBOObjCache.push_back(objectId);
	}
	else if(type == ResourceManager::TEXTURE)
	{
		this->m_GLTextureObjCache.push_back(objectId);
	}
	else if(type == ResourceManager::FBO)
	{
		this->m_GLFrameBufferObjCache.push_back(objectId);
	}
	else if(type == ResourceManager::RBO)
	{
		this->m_GLRenderBufferObjCache.push_back(objectId);
	}
}

CFileCacheManager*  ResourceManager::GetFileCacheMgr()
{
	if (!m_fileCacheMgr)
	{
		m_fileCacheMgr = new CFileCacheManager();
	}

	return m_fileCacheMgr;
}

}

