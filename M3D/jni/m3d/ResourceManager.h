/**@file
 *@brief	资源管理类
 *@author	XL
 *@date		2014-1-7
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */
#ifndef M3D_RESOURCEMANAGER_H_
#define M3D_RESOURCEMANAGER_H_

#include "m3d/model/Image.h"
#include "m3d/graphics/BaseMaterial.h"
#include "m3d/renderer/RenderContext.h"
#include <map>

using std::map;

namespace M3D
{
const static  int OGLOBJ_MAX_NUM = 1000; //OGL GPU 对象最大个数

class BaseMaterial;
class Texture;
class Image;

class PMIData;
class ModelView;
class CAMERA_INFO;
class ComText;
class Model;
class Face;
class Body;
class Edge;
class Curve;
class IVertexSet;
class IPolyLine;
class SPolyLine;
class RefPolyLine;
class GeometryAttribute;
class CFileCacheManager;
class MaterialTemplateManager;

/**
 * @class
 * @brief 测量类型
 */
class M3D_API MeasureTypeValue
{
public:
	MeasureTypeValue():m_valueLength(0.0f),m_typeValue(""){}
	MeasureTypeValue(string typeValue)
	{
		m_typeValue = typeValue;
	}

	MeasureTypeValue(string typeValue , float valueLength)
	{
		m_typeValue = typeValue;
		m_valueLength =valueLength;
	}
	~MeasureTypeValue(){}
	/**
	 * 设置长度
	 * @param valueLength
	 */
	void SetLength(float valueLength)
	{
		m_valueLength = valueLength;
	}
	/**
	 * 获取长度
	 * @return
	 */
	float GetLength()
	{
		return m_valueLength;
	}

	/**
	 * 设置类型值
	 * @param typeValue
	 */
	void SetTypeValue(string typeValue)
	{
		m_typeValue = typeValue;
	}
	/**
	 * 获取类型值
	 * @return
	 */
	string GetTypeValue()
	{
		return m_typeValue;
	}
private:
	float m_valueLength;//!<类型转化为字符串的长度
	string m_typeValue;//!<类型值
};
/**
 * @class
 * @brief 测量有关的国际化管理
 */
class  M3D_API InternationalManager
{
public:
	InternationalManager();
	~InternationalManager();
	/**
	 * 设置语言 现在支持中 英
	 * @param language
	 */
	void SetLanguage(int language);
	/**
	 * 获取当前语言
	 * @return
	 */
	int GetCurrentLanguage();
	/**
	 * @brief 设置默认语言
	 */
	void SetDefaultLangrage();
	/**
	 * 获取当前类型值
	 * @param key
	 * @return
	 */
	string GetCurrentTypeValue(string key);
	/**
	 * 获取类型值转化为字符串的长度
	 * @param key
	 * @return
	 */
	float GetCurrentTypeValueLength(string key);
	/**
	 * 获取当前测量类型
	 * @param key
	 * @return
	 */
	MeasureTypeValue * GetCurrentMeasureTypeValue(string key);
private:
	int m_language;
	map<string,MeasureTypeValue> *m_currentLanguage;//!<当前语言
	map<string,MeasureTypeValue> m_values_zh_rCN;//!<中文对应的测量类型名字-->长度
	map<string,MeasureTypeValue> m_values_en_rUS;//!<英文对应的测量类型名字-->长度
public:
	static const int ZH_rCN;
	static const int EN_rUN;
};


/**@class
 * @brief 资源管理器
 *
 */
class M3D_API ResourceManager
{
public:
	//static ResourceManager* Instance;

	const static int TOPLODLEVEL = 0;

	const static int TEXTURE = 0;
	const static int VBO = 1;
	const static int FBO = 2;
	const static int RBO = 3;
	const static int SHADER_PROGRAM =4;

	static  const string AxisXImagePath;
	static  const string AxisYImagePath;
	static  const string AxisZImagePath;

	Texture* DefaultPBRSpecularTexture() { return defaultPBRSpecularTexture; }
	void DefaultPBRSpecularTexture(Texture* val) { defaultPBRSpecularTexture = val; }
	Texture* DefaultPBRDiffuseTexture() { return defaultPBRDiffuseTexture; }
	void DefaultPBRDiffuseTexture(Texture* val) { defaultPBRDiffuseTexture = val; }
	Texture* DefaultPBRLUTTexture() { return defaultPBRLUTTexture; }
	void DefaultPBRLUTTexture(Texture* val) { defaultPBRLUTTexture = val; }
private:
	static  const string VoiceImagePath;
	static Texture* defaultVoiceImage;

	static  const string PntImagePath;
	static  const string PointOImagePath;
	static  const string PntR2ImagePath;
 
	static Texture* defaultPntTexture;
	static Texture* defaultPntTextureO;
	static Texture* defaultPntTexturer2;

	static Texture* defaultSphereTexture;
	static  const string defaultSphereTexPath;

	static Texture* defaultCubeMapTexture;
	static  const string defaultCubeMapTexPath;

	void GeneratePbrTextures();
	Texture* defaultPBRSpecularTexture;
	Texture* defaultPBRDiffuseTexture;
	Texture* defaultPBRLUTTexture;
public:
	/**
	 * 默认的语音批注对应的图片
	 * @return
	 */
	Texture* GetDefaultVoiceImage();
	/**
	 * 默认点图片
	 * @param type
	 * @return
	 */
	Texture* GetDefaultPointTexture(int type);
	/**
	 * 默认球面映射贴图
	 * @return
	 */
	Texture * GetDefaultSphereMap();
	/**
	 * 默认的立方体贴图
	 * @return
	 */
	Texture * GetDefaultCubeMap();

public:
	/**
	 * 获取单例
	 * @return
	 */
	//static ResourceManager* GetInstance();
public:
	ResourceManager();
	virtual ~ResourceManager();

	/**
	 * 获取纹理
	 * @param key
	 * @return
	 */
	Texture* GetTexture(string key);

	/**
	 * 获取纹理，若没有则创建
	 * @param key
	 * @param type
	 * @return
	 */
	Texture* GetOrCreateTexture(string key, int type = 1);

	/**
	 * 添加纹理
	 * @param key
	 * @param texture
	 * @return
	 */
	bool AddTexture(string key, Texture* texture);

	/**
	 * 移除纹理
	 * @param key
	 * @return
	 */
	bool RemoveTexture(string key);

	/**
	 * 查找名称为Key的Material
	 * @param key
	 * @return
	 */
	BaseMaterial* GetMaterial(string key);

	/**
	 * 得到或者创建Material，首先去查找存在的Material，如果没有找到，则创建名称为Key的Material
	 * @param key
	 * @return
	 */
	BaseMaterial* GetOrCreateMaterial(string key, int type = MaterialType::MaterialType_Phong);

	/**
	 * 添加材质
	 * @param key
	 * @param material
	 * @return
	 */
	bool AddMaterial(string key, BaseMaterial* material);

	/**
	 * 移除材质
	 * @param key
	 * @return
	 */
	bool RemoveMaterial(string key);

	bool AddImage(const string& path, Image* image);

	/**
	 * 获取贴图
	 * @param path
	 * @return
	 */
	Image* GetImage(const string& path);

	/**
	 * 移除贴图
	 * @param path
	 * @return
	 */
	bool RemoveImage(const string& path);

	/**
	 * 获取OpenGL 纹理对象
	 * @param path
	 * @return
	 */
	unsigned int GetOGLTexture(const string& path);

	/**
	 * 添加OpenGL纹理对象
	 * @param path
	 * @param OGLObj
	 * @return
	 */
	bool AddOGLTexture(const string& path, unsigned int OGLObj);

	/**
	 * 移除OpenGL纹理对象
	 * @param path
	 * @return
	 */
	bool RemoveOGLTexture(const string& path);

	//TODO
	void DeleteOGLObjID(int id);

	int NewOGLObjID();

	///Model
	/**
	 * 添加模型
	 * @param key
	 * @param model
	 * @return
	 */
	bool AddModel(int key,Model* model);
	bool RemoveModel(int key);
	Model* GetModel(int key);

	///PMIS
	bool AddPMI(int key,PMIData* pmiData);
	bool RemovePMI(int key);
	PMIData* GetPMI(int key);

	///Views
	bool AddView(int key,ModelView* modelView);
	bool RemoveView(int key);
	ModelView* GetView(int key);

	///Animation XML
	/**
	 * 设置动画数据
	 * @param xmlData
	 */
	void SetAnimationData(const string& xmlData);
	string& GetAnimationData();

	/**
	 * @brief 清除资源
	 */
	void Clear();

	/**
	 * 创建立方体纹理
	 * @param filePath
	 */
	//void CreateCubeMappingTexture(const string& filePath);

		/**
		 * 创建立方体纹理
		 * @param name
		 * @param filePathes
		 */
		Texture* GetOrCreateCubeMappingTexture(string name, vector<string> & filePathes);//TODO
		/**
		* 创建立方体纹理，使用buffer
		* @param name
		* @param filePathes
		*/
		Texture* GetOrCreateCubeMappingTexture(string name, vector<Image*> & cubeImages);//TODO

	/**
	 * 获取立方体纹理
	 * @param key
	 * @return
	 */
	Texture* GetCubeMappingTexture(string key);

	/**
	 * 获取立方体纹理
	 * @return
	 */
	Texture* GetCubeMappingtexture();

	Texture* cubeMappingTexture;//!<立方体纹理

	/**
	 * @brief 释放OpenGL资源
	 */
	void ReleaseGLObjects();

	void AddGLObject(unsigned objectId,int type);

	CFileCacheManager*  GetFileCacheMgr();
	map<string, BaseMaterial*>& GetAllMaterials();

	MaterialTemplateManager*GetMaterialTemplateManager();

private:
	map<string, Texture*> m_allTextures; 	//!<存储所有的纹理资源  TODO 暂时存储两份

	map<string, unsigned int> m_allOGLTextures; //!<key图片路径，value存储OGL纹理对象ID

	map<string, Image*> m_allImages; 	//!<存储所有的图片资源

	map<string, BaseMaterial*> m_allMaterials;	//!<存储所有的材质资源

	bool m_OGLObjIDS[OGLOBJ_MAX_NUM];//!<GPU对象数组

	map<int, Model*> m_ModelMap; //!< Model的表

	string m_xmlAnimatinData;//!<存储svl内部读取的动画文件

	map<string, Texture* > m_cubeMappingTextures;//!所有的环境映射纹理  string为材质名字 TODO

	vector<unsigned> m_GLVBOObjCache;//!<VBO对应的GPU对象

	vector<unsigned> m_GLTextureObjCache;//!<纹理对应的GPU对象

	vector<unsigned> m_GLFrameBufferObjCache;//!<FBO对应的GPU对象

	vector<unsigned> m_GLRenderBufferObjCache;//!<RBO对应的GPU对象

	vector<unsigned> m_GLShaderProgramObjCache;//!<RBO对应的GPU对象

	CFileCacheManager*  m_fileCacheMgr;//!<

	map<string, Texture*> m_allGlobalTextures; 	//!<存储所有的纹理资源 在结束整个SView时才进行释放的资源

	MaterialTemplateManager* m_materialTemplateManager;//模板材质管理器
};
}

#endif /*M3D_RESOURCEMANAGER_H_*/
