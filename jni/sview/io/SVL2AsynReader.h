/*
 * AssimpReader.h
 *
 *  Created on: 2015-6-18
 *      Author: bing
 */

#ifndef SVL2_ASYN_READER_H_
#define SVL2_ASYN_READER_H_

#include "Reader.h"
#include "m3d/model/PMIData.h"
#include "m3d/base/BoundingBox.h"
#include "m3d/base/Mutex.h"
#include "SVLLib/Stk_Declare_Ptr.h"
#include "SVLLib/Stk_Enum.h"
#include "SVLLib/Stk_Define.h"
namespace M3D
{
	class BaseMaterial;
	class Model;
	class Model;
	class Body;
	class SPolyLine;
	class RefPolyLine;
	class Color;
	class PMIData;
	class ComText;
	class GeometryAttribute;
	class Curve;
	class ModelView;
	class Texture;
	class ShapeProperty;
}
using M3D::BaseMaterial;
using M3D::Model;
using M3D::BoundingBox;
using M3D::Body;
using M3D::SPolyLine;
using M3D::RefPolyLine;
using M3D::Color; 
using M3D::PMIData;
using M3D::ComText;
using M3D::GeometryAttribute;
using M3D::Curve;
using M3D::ModelView;
using M3D::Texture;
using M3D::ShapeProperty;

using HoteamSoft::SVLLib::Stk_PMIPtr;
using HoteamSoft::SVLLib::Stk_ComTextPtr;
using HoteamSoft::SVLLib::Stk_GeometryAttributePtr;
using HoteamSoft::SVLLib::STK_UINT32;
using HoteamSoft::SVLLib::Stk_MeshFacePtr;
using HoteamSoft::SVLLib::Stk_MeshEdgePtr;
using HoteamSoft::SVLLib::Stk_ViewPtr;

namespace SVIEW
{	
class View;
typedef  map<int, Model*> AsynPartModelMap;
typedef  map<Model*,int> Model_StkModel_Map;
typedef vector<HoteamSoft::SVLLib::STK_PNT32> SFPOINT3;
class M3D_API SVL2AsynReader : public Reader {

public:
	SVL2AsynReader();
	virtual ~SVL2AsynReader();
	virtual Model* GetModel(int id);

	void ReadFromBuffer(char* fileBuffer, int bufferLength);
	void FillModelColor(Model* model);
	void FillModelMesh(View* view, Model* model);
	void FillModelMesh(View* view, vector<Model*>& models);
	Model* GetModelAssembly();
	void FillModelDrawData(Model* model, void* pstk_NodePtr);
	void FillModelDrawDataMergeFace(Model* model, void* pstk_NodePtr);
	void FillModelDrawDataMergeFaceWithColor(Model* model, void* pstk_NodePtr);
	void AddProtoTypeToCache(int prototypeID, Model * model);
	Model* FindPrototypeFromCache(int id);
	void CopyFormPrototype(Model* model, Model* part_model);

	virtual void Clear();

	void MemoryTest();
	void LoadDoc(string path);
	int GetSVLXFileItem(const std::string& i_strFileName, unsigned int& o_bufSize, char** o_bufP, bool b_ByExt = true);
    int GetSVLXFileItemToFile(const std::string& i_strFileName, const std::string& i_strOutFilePath,bool bByExt);
	int GetSVLXFileItemToFileByReOpen(string & path , const std::string& i_strFileName, const std::string& i_strOutFilePath,bool bByExt);

	/**
	* 获取视图信息
	* @param inInstance 	dm实例
	* @param inModel		当前model
	* @param outViewNum	返回的视图数量
	* @param outViewData	返回的视图数组
	* @return 成功返回true，否则返回false
	*/
	bool GetViewInfo(vector<Stk_ViewPtr> *pStkViewList, Model* inModel, int *outViewNum,
		vector<ModelView*>& outViewData);

	void SetPolyLineForPMI(PMIData* pPMIData, vector<HoteamSoft::SVLLib::STK_PNT32>& vertexBuf,
		vector<STK_UINT32>& indexBuf);
	void SetPolyLineForPMI(PMIData* pPMIData, vector<SFPOINT3>& vecPolylinePnts);

	ComText* ConvertStkComTextToComText(Stk_ComTextPtr stkComText);

	static void FillModelAttribute(Model* model, void* svlDoc);
	void LoadAttribute(Model* model, string & path,View* view);
	/**
	* 添加指定文件路径下的文件几何属性
	* @param model 指定路径对应的模型的顶级model
	* @param strFilePath 待添加的模型文件
	* @param view 视图对象
	* @creator qinyp 18/06/19
	*/
	void AddGeoAttribute(Model* model, string& strFilePath, View* view);
	/**
	* 获取Face的几何属性
	* @param pTriMeshData
	* @param pMeshFace
	*/
	static void  GetTriMeshGeoAttribute(int iModelID, IVertexSet* pTriMeshData, int MeshID, void* svlDoc);
	/**
	* 通过skt_geo 构造 m3d中的geo数据
	* @param stk_geo
	* @return
	*/
	static GeometryAttribute* GetGeometryAttributeFromStk(Stk_GeometryAttributePtr stk_geo);
	/**
	* 获得边界线的几何属性
	* @param curve
	* @param pEdgeInfo
	*/
	static void GetEdgeGeoAttribute(int iModelID, Curve* curve, int EdgeID, void* svlDoc);
    void LoadFileInfo();
private:
	Model* ReadFile(const char* thePath);
	void FillAssemblyBBox(Model* model);
	void FillSingleModelBBox(Model* model);
	void AddEdgeDataToBody(Body* body, void* pStkMesh, int readLodLevel);

	void AddMergeEdgeDataToBody(Body* body, void* pStkMesh, int readLodLevel);
	bool FillMergeEdges(void* stk_edge, vector<float>* outVertices, SPolyLine* bodyPolyLine, RefPolyLine * edgeLine);
	bool FillEdges(void* stk_edge,
		vector<float>* points, SPolyLine* bodyPolyLine,
		RefPolyLine * edgeLine);

	void FillShapeProperty(const map<wstring, wstring>& protoTypeProperties, ShapeProperty * shapeProperty);

	void SimplyFillAssembly(Model* model, void* pstk_NodePtr);
	void FillAssembly(Model* model, void* binInstance);

	string GetInstanceName(Model* model);

	bool GetMatrialColor(int materialId,Color& difuseColor, BaseMaterial* &material);	
	
	//清除临时存储Map的关联信息
	void  ClearPartsMap();
	void SetPercent(float percent);
	void updateReadInstancePercent();
	string GetSVLMaterialID(Model* model, unsigned int renderID);

	/**
	* 得到SVL文件中存储的xml格式动画字符串
	* @return
	*/
	virtual const string& GetXMLAnimationData();

	/**
	* 设置xml格式的动画文件
	* @param xmlData
	*/
	virtual void SetXMLAnimationData(const string& xmlData);
	string GetAnimationName(const char* path);
	void GetInstanceData(Model* model, void* instancePtr);
	void GetPMIsData(Model* model);
	bool GetPMIInfo(vector<Stk_PMIPtr>* pStkPMIList, int *pmiCount, vector<PMIData*>& outPMIData);
    /**
     读取用户信息
     */
    void ReadUserData(Model* model);

	void ReadModelAttribute(Model* model);

	void GetViewsData(Model* model);
 
	/**
	* 通过stk中Geo属性构建M3D中的属性数据
	* @param stk_geo
	* @return
	*/
	static GeometryAttribute* GetPlanfaceGeoFromStk(Stk_GeometryAttributePtr stk_geo);
	static GeometryAttribute* GetRevolutionfaceGeoFromStk(Stk_GeometryAttributePtr stk_geo);
	static GeometryAttribute* GetCylinderfaceGeoFromStk(Stk_GeometryAttributePtr stk_geo);
	static GeometryAttribute* GetConicalfaceGeoFromStk(Stk_GeometryAttributePtr stk_geo);
	static GeometryAttribute* GetSpherefaceGeoFromStk(Stk_GeometryAttributePtr stk_geo);
	static GeometryAttribute* GetToroidaifaceFromStk(Stk_GeometryAttributePtr stk_geo);
	static GeometryAttribute* GetLineGeoFromStk(Stk_GeometryAttributePtr stk_geo);
	static GeometryAttribute* GetEllipseGeoFromStk(Stk_GeometryAttributePtr stk_geo);

	void SVLXRenderToMaterial(BaseMaterial* material, void* pStkTexture);
    
    void FillModelVisible(Model* model);

	//材质库函数 测试
	
	//************************************
	// Method:    GetMatrial
	// FullName:  SVIEW::SVL2AsynReader::GetMatrial
	// Access:    private 
	// Returns:   bool
	// Qualifier: 解析材质
	// Parameter: int materialId
	// Parameter: Color & difuseColor
	// Parameter: BaseMaterial *  & material
	//************************************
	bool GetMatrial(int materialId, Color& difuseColor, BaseMaterial* &material);

	Texture* GetTexture2D(void* pStkTexture);
	
private:
	void ParseMaterialParameters(HoteamSoft::SVLLib::Stk_MaterialPtr ptr, BaseMaterial* material);
	void GetImageData(unsigned int imageId, unsigned int & o_buffersize, char * &o_data,string& o_svePath);
    /**
     去除多余字符
     
     @param strStream
     @return
     */
    string RemoveExcess(string& strStream);
private:
	Model* 					m_M3DModel;				///根model
	AsynPartModelMap m_partModelsMap; //缓存Model的列表
	Model_StkModel_Map m_modelStkModelMap;
	vector<BaseMaterial*>		m_MaterialList;			///存储材质列表
	void* m_svl2Doc;

	map<int, Model*> m_mapIdToInstance;	
	map<int, void*> m_mapNodeIdToInstanceNode;
	map<int, vector<void*> > m_parentIdToBinInstanceNode;

	mutable M3D::Mutex m_mutex;//!<线程锁
	float m_readerPercent;

	int readInstanceIndex;
	int m_instanceCount; //纪录instance的总数
	//Document m_nodeDoc;
	string svlxVersion;
	char* m_fileBuffer;
	int m_fileBufferLength;
	bool m_mergeface;//是否进行合并面操作
	string m_xmlAnimatinData;//!<存储svl内部读取的动画文件
	
	map<int,BaseMaterial*> m_protoTypeMaterialCache;
	map<int, Color> m_protoTypeColorCache;	
	map<STK_UINT32, std::wstring> i_mapInstanceIDtoName;

	bool m_usePrototypeProperty;
	bool m_useInstanceProperty;
	int m_SVLIDOffset;
};

} //end namespace

#endif /* ASSIMPREADER_H_ */
