/**@file
*@brief
*@author	Huangderong
*@date		2017-7-29
*@version	1.0
*
*/

#ifndef     M3D_MODEL_EXTENDINFOMANAGER_H_
#define     M3D_MODEL_EXTENDINFOMANAGER_H_

#include "m3d/M3D.h"
namespace M3D
{
class BodyExtInfo;
class FaceExtInfo;
class PMIData;
class ModelView;
class SectionPlane;
class RenderAction;
class Edge;
class Face; 
class Model;
class GeometryAttribute;
class SceneManager;

class M3D_API ExtendInfoManager
{
public:
	ExtendInfoManager();
	virtual ~ExtendInfoManager();

	void AddModelPMIs(IDTYPE ModelId,const map<int, PMIData*>& pmis);
	map<int, PMIData*>* GetModelPMIs(IDTYPE ModelId);
	map<IDTYPE, map<int, PMIData*>> GetAllPMIs();
	int GetModelPMIsCount(IDTYPE ModelId);

	void AddModelView(IDTYPE ModelId, const vector<ModelView*>& vecViews);
	vector<ModelView*>* GetModelView(IDTYPE ModelId);
	int GetModelViewCount(IDTYPE ModelId);

	void AddModelSectionPlane(IDTYPE ModelId, const vector<SectionPlane*>& vecSectionPlanes);
	vector<SectionPlane*>* GetModelSectionPlane(IDTYPE ModelId);

	void Clear();
	void Clear(IDTYPE ModelId);
	void RayPick(RayPickAction * action);
	virtual void FindVisiableObject(RenderAction* renderAction);

    void SetAllMeshFaceAttr(map<unsigned int, map<unsigned int, map<wstring, wstring>>>& allAttr);
    void SetAllMeshEdgeAttr(map<unsigned int, map<unsigned int, map<wstring, wstring>>>& allAttr);
    map<wstring, wstring> GetMeshFaceAllAttribute(const int& i_iModelID, const int& i_MeshFaceID);
    map<wstring, wstring> GetMeshEdgeAllAttribute(const int& i_iModelID, const int& i_MeshEdgeID);
	GeometryAttribute* GetFaceGeoAttribute(int iModelID, int iMeshFaceID);
	GeometryAttribute* GetEdgeGeoAttribute(int iModelID, int iMeshEdgeID);
	//拷贝复制原有model
	void CopyMeshAttr(unsigned int iOldModelID, unsigned int iNewModelID);
	//添加面几何属性
	void AddMeshFaceAttr(map<unsigned int, map<unsigned int, map<wstring, wstring>>>& faceAttrs);
	//添加边几何属性
	void AddMeshEdgeAttr(map<unsigned int, map<unsigned int, map<wstring, wstring>>>& edgeAttrs);
	void SetSVLXFilePath(const string& filePath);
	void FillModelAttribute(Model* model);
	M3D::SceneManager* GetScene() const;
	void SetScene(M3D::SceneManager* val);
	void UpdateSceneByModel(Model* model);
private:
	void UnSVLXInfo(void* document);
	void LoadSVLXInfo(void* document);

private:
	map<IDTYPE, map<int, PMIData*>> m_AllModelPMIMap;         //!<所有PMI的列表 临时放Model下
	map<IDTYPE, vector<ModelView*>> m_AllModleViewList;       //!<视图列表
	map<IDTYPE, vector<SectionPlane*>> m_AllSectionPlaneList; //!<剖视平面列表
 
	string m_svlxFilePath;
    void* m_svlDoc;
	bool m_svlxFileExist;
    map<unsigned int, map<unsigned int, map<wstring, wstring>>> allMeshFaceAttr;
    map<unsigned int, map<unsigned int, map<wstring, wstring>>> allMeshEdgeAttr;

	SceneManager* m_scene;
};
}
#endif