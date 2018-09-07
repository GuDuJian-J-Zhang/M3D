/**@file
 *@brief svl文件读取器.
 *@author billy
 *@date 2013-5-31
 *@version 1.0
 *
 *提供读取svl文件的方法。
 *
 */
#ifndef SVLREADER_H_
#define SVLREADER_H_

#include "sview/io/Reader.h"

#include "m3d/model/ComText.h"
#include "m3d/model/Edge.h"
#include "m3d/model/Model.h"
#include "m3d/model/Face.h"
#include "m3d/model/Body.h"
#include "m3d/model/Curve.h"
#include "m3d/graphics/CameraNode.h"
#include "m3d/model/GeoAttribute.h"

#include "prebuilt/svl2/include/Stk_Define.h"
#include "prebuilt/svl2/include/Stk_MeshEntity.h"
#include "prebuilt/svl2/include/Stk_CompositeText.h"
#include "prebuilt/svl2/include/Stk_PMIEntity.h"
#include "prebuilt/svl2/include/Stk_Listener.h"

using M3D::PMIData;
using M3D::ModelView;
using M3D::CAMERA_INFO;
using M3D::ComText;
using M3D::Face;
using M3D::Body;
using M3D::Edge;
using M3D::Curve;
using M3D::IPolyLine;
using M3D::SPolyLine;
using M3D::RefPolyLine;
using M3D::GeometryAttribute;


namespace M3D
{
	class Model;
	class ShapeSet;
	class BaseMaterial;
	class ShapeProperty;
	class Material;
}
using M3D::ShapeSet;
using M3D::BaseMaterial;
using M3D::Material;
using M3D::ShapeProperty;
using M3D::Model;

class Stk_File;
class Stk_Instance;
class Stk_ProtoType;
class Stk_Body;
class Stk_Mesh_Com;
class Stk_Mesh_Face;
class Stk_LineSet;

typedef vector<STK_PNT32> FPOINT3;


namespace SVIEW
{
class SvlReader;

class M3D_API SVLReaderListener:public  Stk_Listener{
public:
	SVLReaderListener();
	virtual ~SVLReaderListener();

	virtual void LoadingProcess(STK_FLOAT32 i_fLoadingPercent);

	void SetReader(SvlReader* reader);
private:
	SvlReader* m_reader;
};

/**@class SvlReader
 * @brief svl文件读取器类
 *
 * 提供读取svl文件的方法
 *
 */
class M3D_API SvlReader: public Reader {
public:
	SvlReader(void);
	virtual ~SvlReader();
	virtual Model* GetModel(int id);

	virtual void Clear();

private:
	const static int TOPLODLEVEL = 0;
protected:

	/**@brief 读取文件
	 * @param file 文件路径
	 * @return Model对象指针
	 */
	Model* ReadFile(string file);

	void ConvertModelColorToMaterial(Model * model);

	/**@brief 获取实例数据
	 * @param model 要填充的当前model
	 * @param ins	当前实例
	 * @param upHasColor	父实例是否有颜色
	 * @param upInsColor	父实例的颜色
	 */
	void GetInstanceData(Model* model, Stk_Instance* ins
			, STK_BOOL upHasColor,STK_RGBA32 upInsColor ,STK_BOOL upVisible);

	/**@brief 获取原型数据
	 * @param model 要填充的当前model
	 * @param pProtoType	当前原型
	 * @param upHasColor	父实例是否有颜色
	 * @param upInsColor	父实例的颜色
	 */
	void GetProtoTypeData(Model* model, Stk_ProtoType *pProtoType,
			STK_BOOL upHasColor, const STK_RGBA32& upInsColor);

	/**
	 * 获取实例model数据
	 * @param model
	 * @param pProtoType
	 * @param upHasColor
	 * @param upInsColor
	 */
	void AddProtoDataToModel(Model* model, Stk_ProtoType *pProtoType,
		STK_BOOL upHasColor,  const STK_RGBA32&  upInsColor);
	/**
	 * 获取实例Body数据
	 * @param body
	 * @param pCurBody
	 * @param pProtoType
	 * @param upHasColor
	 * @param upInsColor
	 */
	void GetProtoBodyData(Body* body,Stk_Body *pCurBody,Stk_ProtoType *pProtoType,
			STK_BOOL upHasColor,  const STK_RGBA32&  upInsColor);
	/**
	 * 获取实例MeshCom数据
	 * @param body
	 * @param pMeshCom
	 * @param pProtoType
	 * @param upHasColor
	 * @param bodyColor
	 * @param upInsColor
	 */
	void AddProtoMeshComDataToBody(Body* body, Stk_Mesh_Com *pMeshCom,int protoTypeLODCount,
				STK_BOOL upHasColor,  const STK_RGBA32&  bodyColor, const STK_RGBA32&  upInsColor);
	/**
	 * 获取合并后的Face数据，当comMesh 使用同一颜色标记为真时使用
	 * @param body
	 * @param pMeshCom
	 * @param upHasColor
	 * @param meshcolor
	 * @param lodCount
	 */
	void  AddMergeFaceToBody(Body* body, VertexSet* bodymeshData, Stk_Mesh_Com *pMeshCom,
			STK_BOOL upHasColor,  const STK_RGBA32&  meshcolor, int lodCount);

	/**
	 * 将LOD数据填充到Body中
	 * @param body
	 * @param pMeshCom
	 * @param readLODLevel
	 */
	void  AddLodDataToBody(Body* body, VertexSet* bodymeshData, Stk_Mesh_Com *pMeshCom,int readLODLevel);

	/**
	 * 通过comMesh数据填充body
	 * @param body
	 * @param pMeshCom
	 * @param lodCount
	 */
	void FillBody(Body* body, VertexSet* bodymeshData, Stk_Mesh_Com *pMeshCom,int lodCount);
	/**
	 * 获取实例Face的数据
	 * @param body
	 * @param pMeshCom
	 * @param upHasColor
	 * @param meshcolor
	 * @param lodCount
	 */
	void AddFaceToBody(Body* body, VertexSet* bodyMesh, Stk_Mesh_Com *pMeshCom,
			STK_BOOL upHasColor,  const STK_RGBA32&  meshcolor, int lodCount);
	/**
	* 判断STK_Mesh是否有相同的颜色
	*/
	bool IsMeshHasUniColor(Stk_Mesh_Com *pMeshCom, STK_BOOL upHasColor, const STK_RGBA32& meshcolor);
	/**
	 * 获取实例Face的Lod显示数据
	 * @param pFace
	 * @param pMeshFace
	 * @param allMeshLOD
	 * @param lodLevel
	 */
	void AddLodDataToFace(Face* pFace,Stk_Mesh_Face* pMeshFace, VertexSet* bodyMesh, Stk_Mesh_Com *pMeshCom,map<unsigned int, MeshLODInfo*>& allMeshLOD,
			int lodLevel);
	/**
	 * 获取合并后实例的LOD数据
	 * @param pFace
	 * @param pMeshCom
	 * @param readLodLevel
	 */
	void AddMergeLodDataToFace(Face* pFace, VertexSet* bodymeshData, Stk_Mesh_Com *pMeshCom, int readLodLevel);

	/**
	 * 获取实例边界线数据
	 * @param pFace
	 * @param pMeshFace
	 * @param allMeshLOD
	 * @param readLodLevel
	 */
	void AddEdgeDataToFace(Face* pFace,Stk_Mesh_Face* pMeshFace,
			MeshLODInfo* meshLodInfo, int readLodLevel);

	/**
	 * 获得comMesh中的所有边界线数据
	 * @param body
	 * @param pMeshCom
	 * @param allMeshLOD
	 * @param readLodLevel
	 */
	void AddEdgeDataToBody(Body* body, Stk_Mesh_Com *pMeshCom, map<unsigned int,
			MeshLODInfo*>& allMeshLOD, int readLodLevel);

	void AddLineSetsToShapeCollection(Body *pBody,Stk_Node* pNode,
			ShapeSet* shapeSet);

	void AddPointSetsToShapeCollection(Body *pBody,
			Stk_Node* pNode,ShapeSet* shapeSet);

	/**
	 * 获取Face的颜色
	 * @param faceolor
	 * @param pMeshFace
	 * @param upHasColor
	 * @param meshcolor
	 */
	void GetFaceColor(STK_RGBA32& faceolor,Stk_Mesh_Face* pMeshFace,
			STK_BOOL upHasColor,  const STK_RGBA32&  meshcolor);
	/**
	 * 获取合并后Face的颜色
	 * @param faceolor
	 * @param pMeshCom
	 * @param upHasColor
	 * @param meshcolor
	 */
	void GetMergeFaceColor(STK_RGBA32& faceolor, Stk_Mesh_Com *pMeshCom,
			STK_BOOL upHasColor,  const STK_RGBA32&  meshcolor);
	/**
	 * 获取Body的颜色
	 * @param bodycolor
	 * @param pCurBody
	 * @param upHasColor
	 * @param upInsColor
	 */
	void GetBodyColor(STK_RGBA32& bodycolor, Stk_Body *pCurBody,STK_BOOL upHasColor,
			 const STK_RGBA32&  upInsColor);
	/**
	 * 获取MeshCom的颜色
	 * @param meshComColor
	 * @param pMeshCom
	 * @param upHasColor
	 * @param upInsColor
	 */
	void GetMeshComColor(STK_RGBA32& meshComColor, Stk_Mesh_Com *pMeshCom,STK_BOOL upHasColor,
			 const STK_RGBA32&  upInsColor);

	/**
	 * 通过renderId,获取SVL render Material 数据
	 * @param renderId
	 * 内部存储类protoTypeMaterial的信息，在解析一个proto数据时不用每次都传递
	 */
	BaseMaterial*  GetMaterialById(unsigned int renderId);

	/**
	 *
	 * @param renderId
	 */
	void AddFileMaterialToResourceMgr(Stk_File *pProtoType);

	/**
	 * 判读是否已经存在Model的实例数据
	 * @param model
	 * @param pProtoType
	 * @return
	 */
	bool IsExistProtoModel(Model* model, Stk_ProtoType *pProtoType);

	/**
	 * 如果存在model的实例数据，则通过此函数拷贝到新的模型中去，注意共享一份显示数据
	 * @param model
	 * @param orig
	 */
	void CopyModelDrawData(Model* model, Model* orig);

	/**
	 * 所有的边界线都通过此map来进行统一管理
	 * @param ID 边界线对应的id，唯一
	 * @param line 边界线地址
	 * @param readLevel lod的level
	 */
//	void AddLineToClearManager(int ID, IPolyLine* line, int readLevel);
private:

	/**
	 * 获得边界线的几何属性
	 * @param curve
	 * @param pEdgeInfo
	 */
	void GetEdgeGeoAttribute(Curve* curve,Stk_Mesh_Edge* pEdgeInfo);

	/**
	 * 获取Face的几何属性
	 * @param pTriMeshData
	 * @param pMeshFace
	 */
	void  GetTriMeshGeoAttribute(IVertexSet* pTriMeshData,Stk_Mesh_Face* pMeshFace,int readLodLevel);

	/**
	 * 通过skt_geo 构造 m3d中的geo数据
	 * @param stk_geo
	 * @return
	 */
	GeometryAttribute* GetGeometryAttributeFromStk(Stk_GeometryAttribute* stk_geo);

	/**
	 * 通过stk中Geo属性构建M3D中的属性数据
	 * @param stk_geo
	 * @return
	 */
	GeometryAttribute* GetPlanfaceGeoFromStk(Stk_GeometryAttribute* stk_geo);
	GeometryAttribute* GetRevolutionfaceGeoFromStk(Stk_GeometryAttribute* stk_geo);
	GeometryAttribute* GetCylinderfaceGeoFromStk(Stk_GeometryAttribute* stk_geo);
	GeometryAttribute* GetConicalfaceGeoFromStk(Stk_GeometryAttribute* stk_geo);
	GeometryAttribute* GetSpherefaceGeoFromStk(Stk_GeometryAttribute* stk_geo);
	GeometryAttribute* GetToroidaifaceFromStk(Stk_GeometryAttribute* stk_geo);
	GeometryAttribute* GetLineGeoFromStk(Stk_GeometryAttribute* stk_geo);
	GeometryAttribute* GetEllipseGeoFromStk(Stk_GeometryAttribute* stk_geo);

	void ReadTopProtoInfo(Stk_ProtoType* topProto, Model* topModel);

	/**
	 * 从model原型表中获取指定id的model
	 * @param id modelId
	 * @return model原型指针
	 */
	Model*	FindModelFromProtoTypeID(int id);

	/**
	 * 新增到model原型表
	 * @param model 新增的model原型
	 */
	void AddProtoType(Model * model);

	/**
	 * 获取PMI信息
	 * @param ins	dm实例
	 * @param pmiCount	返回的PMI数量
	 * @param outPMIData	返回的PMI数组
	 * @return 成功返回true，否则返回false
	 */
	bool GetPMIInfo(vector<Stk_PMI*>* pStkPMIList, int *pmiCount, vector<PMIData*>& outPMIData);

	/**
	 * 填充PMICreator生成出的顶点和索引信息到PMIData
	 * @param pPMIData	PMIData指针
	 * @param vertexBuf	顶点数组
	 * @param indexBuf	索引数组
	 * @note 由于PMICreator生成出的索引信息中包含coin3d专用的分隔符，因此在填充时进行了过滤。
	 */
	void SetPolyLineForPMI(PMIData* pPMIData, vector<STK_PNT32>& vertexBuf,
			vector<STK_UINT32>& indexBuf);

	void SetPolyLineForPMI(PMIData* pPMIData, vector<FPOINT3>& vecPolylinePnts);
	/**
	 * 将Stk_ComText转换为ComText
	 * @param stkComText	Dm中的comText
	 * @return	转换后的ComText*
	 */
	ComText* ConvertStkComTextToComText(Stk_ComText* stkComText);

	/**
	 * 获取视图信息
	 * @param inInstance 	dm实例
	 * @param inModel		当前model
	 * @param outViewNum	返回的视图数量
	 * @param outViewData	返回的视图数组
	 * @return 成功返回true，否则返回false
	 */
	bool GetViewInfo(vector<Stk_View*> *pStkViewList,  Model* inModel, int *outViewNum,
			vector<ModelView*>& outViewData);

	/**
	 * 是否使用高新能模式
	 * @return
	 */
	bool IsUseHighPerformance();

	/**
	 * 设置使用高性能模式
	 * @param highPerformance
	 */
	void SetHighPerformance(bool highPerformance);

	/**
	 * 加载文件读取时配置
	 */
	void LoadReadConfig();

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

    /**
     * 将MeshFace中的索引数据加入 faceMeshData
     * @param refMeshData
     * @param outPolyIndeies
     * @param readLodLevel
     */
	void AddIndexDataToMesh(Mesh* refMeshData, vector<STK_UINT32> *outPolyIndeies,
			int readLodLevel);

    /**
     *
     * @param refMeshData
     * @param outPolyIndeies
     * @param outVertices
     */
	void AddVertexDataToMesh(Mesh* refMeshData, vector<STK_UINT32> *outPolyIndeies,
			vector<STK_PNT32>* outVertices,vector<STK_PNT32>* outTextureCoords);

    /**
     *
     * @param meshData
     * @param outPolyIndeies
     * @param outVertices
     * @param readLodLevel
     */
	void AddVertexToFace(VertexSet* meshData, vector<STK_UINT32> *outPolyIndeies,
				vector<STK_PNT32>* outVertices,int readLodLevel);

    /**
     *
     * @param meshId
     * @param comMeshTriData
     */
//	void AddMeshToClearManager(int lodLevel,IVertexSet* iMeshData);

	/**
	 * 填充边界线的折线数据和几何属性信息
	 * @param stk_edge
	 * @param outVertices
	 * @param bodyPolyLine
	 * @param edgeLine
	 * @return
	 */
	bool FillEdges(Stk_Mesh_Edge* stk_edge,vector<STK_PNT32>* outVertices, SPolyLine* bodyPolyLine,RefPolyLine * edgeLine );

	bool FillLine(Stk_CurveEntity* curLine,RefPolyLine * edgeLine,SPolyLine* bodyPolyLine );

	/**
	 * 将SVL的LOD数据和SView的LOD级别对应起来
	 * @param svlLodLevel
	 * @return
	 */
	int ConvertToSViewUseLevel(int svlLodLevel);

	/**
	 * 得到此ComMesh下，所有faceMesh总顶点的个数
	 * @param pMeshCom
	 * @param readLevel
	 * @return 总顶点的个数
	 */
	int GetComMeshAllFaceIndexCount(Stk_Mesh_Com *pMeshCom,int readLevel);

	/**
	 * 递归读取STK_Node
	 * @param pModel
	 * @param pNode
	 * @return void
	 */
	void GetStkNodeData(Model* pModel, Body* pBody,Stk_Node* pNode,int protoTypeLODCount,
			STK_BOOL upHasColor, const STK_RGBA32& upInsColor,vector<Stk_PMI*>* pStkPMIList,
			vector<Stk_View*>* pStkViewList,ShapeSet *pParentShapeSet);

	void SetUseCatia(bool useCatia);
	bool UseCatia();

	/**
	 * 计算全局唯一ID
	 * @param protoTypeMaterialId
	 * @param renderId
	 * @return
	 */
	string GetSVLMaterialID(Stk_File* protoType,unsigned int renderID);

	void SVLRenderToMaterial(Material* material,Stk_Render* stkRenderP);

	void FillShapeProperty(const vector<Stk_MetaData*>& protoTypeProperties,ShapeProperty* shapeProperty);

	/**
	 * 获取SVL中存储的用户自定义属性信息
	 */
	void ReadUserData(Stk_ProtoType *pProtoType);

private:
	Stk_DocumentManager* m_stkDocMgr;  //!< 当前dm的文件对象
	map<int, Model*> m_ModelMap;       //!< Model的表
	map<string, Model*> m_modelPlcPathMap; //!< 根据装配路径进行缓存

	bool m_IsSetDefaultColor;	//!< 是否设置默认颜色
	bool m_IsUseLOD;//!< 是否使用LOD
	map<int, Model *> m_ProtoTypeMap;//!< 原型model表
	Model* m_M3DModel; //!< root Model
	bool isReadFaceMeshData; //!< 是否将Face组合成Body显示
	bool isMergeFace;  //!<是否合并有相同颜色的面
	bool isHighPerformance;  //!<是否使用高性能模式

	string m_xmlAnimatinData;//!<存储svl内部读取的动画文件

	bool m_readEdge;

	bool m_usePrototypeProperty;
	bool m_useInstanceProperty;

	int m_readMaxLodLevel;
	int m_readMinLodLevel;

	vector<Stk_PMI*> m_StkPMIList;
	vector<Stk_View*> m_StkViewList;

	bool m_useCATIAMode;//!<使用catai方式构建节点

	map<int,BaseMaterial*> m_protoTypeMaterialCache;

	int totaolInsCount;
	int m_currentInsIndex;
};

}

#endif /* SVLREADER_H_ */
