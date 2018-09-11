/*
 * AssimpReader.h
 *
 *  Created on: 2015-6-18
 *      Author: bing
 */

#ifndef ASSIMPREADER_H_
#define ASSIMPREADER_H_

#include "Reader.h"
#include "m3d/model/Model.h"

class aiScene;
class aiNode;

namespace M3D
{
	class Model;
	class BaseMaterial;
	class Material;
}
using M3D::BaseMaterial;
using M3D::Model;
using M3D::Material;

namespace SVIEW
{
class M3D_API AssimpReader : public Reader {
public:
	/**
	 * 得到obj文件的材质路径
	 * @param filePath
	 * @return
	 */
	static string GetObjFileMtlPath(const string filePath);
public:
	AssimpReader();
	virtual ~AssimpReader();
	virtual Model* GetModel(int id);

private:
	Model* ReadFile(const char* thePath);

	Model* CreateModelByAINode(const aiScene* scene,aiNode* curNode,const string& strParentPlacePath,int sortId);

	void GetMtlPath(const string& filePath);

	void LoadMaterials(const aiScene* scene,const string& fileDir,const string& file_name,const string& preFix);
private:
	Model* 					m_M3DModel;				///根model
//	map<int, Model*> 		m_ModelMap;				///model表

	vector<Material*>		m_MaterialList;			///存储材质列表
};

} //end namespace

#endif /* ASSIMPREADER_H_ */
