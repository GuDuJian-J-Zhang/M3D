// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_MeshEntity.h
 *
 *	@brief	Mesh 基类
 *
 *	@par	历史:
 *		2012/11/30	创建。Gaoqq@HOTEAMSOFT
****************************************************************************/

#ifndef _Stk_Mesh_ENTITY_H_
#define _Stk_Mesh_ENTITY_H_

#ifndef __MOBILE__
#pragma once
#endif

#include "DataManager.h"
#include "Stk_File.h"
#include "Stk_GeoAttribute.h"
#include "Stk_MetaDataEntity.h"
#include "Stk_Entity.h"
#include "Stk_UserData.h"
#include "Stk_Render.h"

class Stk_Node;

/**************************************************************************************
	 *	@class	Stk_MeshEntity
	 *	@brief	Mesh基类
	 *
	 *	Mesh的基本属性。<br>
	 *	继承自Stk_Entity类。
***************************************************************************************/

class DATAMANAGER_API  Stk_MeshEntity :
	public Stk_Entity
{
private:

protected:
	STK_BOOL						m_bHasBox;		//!< 是否有包围盒
	STK_BOX32						m_boxBounding;	//!< 包围盒
	Stk_GeometryAttribute			*m_stkGeoAttrP;	//!< 基本属性
	//vector<Stk_MetaData*>			m_vcMetaDatas;	//!< 自定义属性

public:
	/****************************************************************************
	 *	@brief		Stk_MeshEntity构造函数
	 *	@param[in]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_MeshEntity(void);

	/****************************************************************************
	 *	@brief		Stk_MeshEntity析构函数
	 *	@param[in]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	~Stk_MeshEntity(void);

public:
	/****************************************************************************
	 *	@brief		设置包围盒
	 *	@param[in]	i_boxBounding	包围盒
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS						SetBoundingBox(const STK_BOX32 &i_boxBounding);
	
	/****************************************************************************
	 *	@brief		获取包围盒
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_boxBounding	包围盒
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_BOX32						GetBoundingBox();
	
	/*****************************************************************************
	 *	@brief		判断是否有包围盒
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_bHasBox	是否有包围盒
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_BOOL						HasBoundingBox();
	
	/*****************************************************************************
	 *	@brief		获取ProtoType的ID
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_idProtoType	ProtoType的ID
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_ID							GetProtoTypeID();

	/*****************************************************************************
	 *	@brief		获取Mesh的ID
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_ID	Mesh的ID
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_ID							GetID();

	/*****************************************************************************
	 *	@brief		设置ID
	 *	@param[in]	i_ID		ID
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS	设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS						SetID(STK_ID i_ID);

	/****************************************************************************
	 *	@brief		获取基本属性
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_stkGeoAttrP	基本属性对象
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_GeometryAttribute*			GetGeoAttr();

	/****************************************************************************
	 *	@brief		设置基本属性
	 *	@param[in]	*i_stkGeoAttrP	基本属性对象
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS	设置成功
	  *	@retval		STK_ERROR	设置失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS						SetGeoAttr(Stk_GeometryAttribute *i_stkGeoAttrP);

	//自定义属性
	//STK_STATUS					AddMetaData(Stk_MetaData* i_MetaDataP);
	//const vector<Stk_MetaData*>&	GetAllMetaDatas();
	//STK_SIZE_T					GetMetaDataNum();
	//STK_STATUS					ReadMetaDatas(Stk_File *i_FileP, StkSegmentHead *i_PropHeadP);
	//STK_STATUS					WriteMetaData(Stk_File* i_stkFileP);
	
};


/**************************************************************************************
	 *	@class	Stk_Mesh_Edge
	 *	@brief	Mesh 边界线类
	 *
	 *	保存Mesh的边界线索引。<br>
	 *	继承自Stk_MeshEntity类。
***************************************************************************************/
typedef vector<STK_UINT32>	vcEdgeInfo;	//!< 单条边界线的点列索引

class DATAMANAGER_API  Stk_Mesh_Edge :
	public Stk_MeshEntity
{
private:
	map<STK_UINT32, vcEdgeInfo*>	m_mapEdgeLODInfo;			//!< LOD（0-8）下该Edge的信息
	STK_RGBA32						m_rgbaEdge;					//!< Edge的颜色
	STK_BOOL						m_bHasColor;				//!< 是否有颜色
	STK_FLOAT32						m_fWidth;					//!< 线宽度
public:
	/*****************************************************************************
	 *	@brief		Stk_Mesh_Edge构造函数
	 *	@param[in]	i_stkProtoTypeP		ProtoType对象指针
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_Mesh_Edge(Stk_ProtoType* i_stkProtoTypeP);

	/*****************************************************************************
	 *	@brief		Stk_Mesh_Edge析构函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	~Stk_Mesh_Edge(void);

public:
	/*****************************************************************************
	 *	@brief		设置Edge指定LOD等级下的Edge信息
	 *	@param[in]	i_nuLODLevel		LOD等级
	 *	@param[in]	i_EdgeInfoP			对应LOD等级的Edge数据
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@retval		STK_ERROR			设置失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS						SetEdgeLODInfo(STK_UINT32 i_nuLODLevel, vcEdgeInfo *i_EdgeInfoP);
	
	/*****************************************************************************
	 *	@brief		获取所有LOD等级对应的Edge数据
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_mapEdgeLODInfo	所有LOD等级对应的Edge数据
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	map<STK_UINT32, vcEdgeInfo*>	GetAllEdgeLODInfo();

	/****************************************************************************
	 *	@brief		清空所有的EdgeLOD信息
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS						ClearAllEdgeLOD();

	/*****************************************************************************
	 *	@brief		判断边界线是否有颜色
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_bHasColor	边界线是否有颜色
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_BOOL						IsHasColor();

	/*****************************************************************************
	 *	@brief		设置边界线的颜色值
	 *	@param[in]	i_rgbaEdge	边界线的颜色
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS	设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS						SetColor(STK_RGBA32 &i_rgbaEdge);

	/*****************************************************************************
	 *	@brief		获取边界线的颜色
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_rgbaEdge	边界线的颜色
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_RGBA32						GetColor();

	/*****************************************************************************
	 *	@brief		注册ID
	 *	@param[in]	i_IDEdge		Edge ID <br>
	 *				== STK_NEW_ID	注册新ID <br>
	 *				== 其它			注册指定ID
	 *	@retval		STK_SUCCESS		注册成功
	 *	@retval		STK_ERROR		ID重复注册失败
	 *	@note		指定ID需大于等于1的整数 <br>
	 *				由于V2.33版之前ID存在重复的可能，RegisterID时不对这些版本检测ID重复
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS						RegisterID(STK_ID i_IDEdge);

	/*****************************************************************************
	 *	@brief		写MeshEdge数据
	 *	@param[in]	i_head				数据段头指针
	 *	@param[in]	i_stkFileP			文件对象指针
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			写入成功
	 *	@retval		其它				写入失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS						WriteFile(StkSegmentHead *i_head, Stk_File *i_stkFileP);

	/*****************************************************************************
	 *	@brief		设置边界线的宽度
	 *	@param[in]	i_fWidth	边界线的宽度
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS	设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS						SetWidth(STK_FLOAT32 i_fWidth);

	/*****************************************************************************
	 *	@brief		获取边界线的宽度
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		边界线的宽度
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_FLOAT32						GetWidth();
};


/**************************************************************************************
	 *	@class	Stk_Mesh_Face
	 *	@brief	Mesh 构成面类
	 *
	 *	保存Mesh的面数据信息。<br>
	 *	继承自Stk_MeshEntity类。
***************************************************************************************/

typedef vector<STK_UINT32>	vcPolylineIndices;	//!< 单条边界点的点列索引

typedef struct {
	vector<STK_UINT32>			TrianglesLOD;	//!< 该LOD等级下的面片索引
	vector<vcPolylineIndices>	EdgeLODIdx;		//!< 该LOD等级下的相应面的边界线信息：线+索引
}FaceLODInfo;

class DATAMANAGER_API  Stk_Mesh_Face :
	public Stk_MeshEntity
{
private:
	map<STK_UINT32, FaceLODInfo*> m_mapFaceLODInfo;	//!< LOD（0-8）下该Face的多边形所以及其线的信息

	STK_BOOL					m_bHasColor;		//!< 是否有颜色
	STK_RGBA32					m_rgbaFace;			//!< RGBA颜色
	STK_BOOL					m_bHasMaterial;		//!< 是否有材质
	STK_MAT32					m_matFace;			//!< 材质
	vector<STK_ID>				m_vcMeshEdgeID;		//!< 关联的MeshEdge的ID

	STK_BOOL					m_bHasRender;		//!< 是否有渲染
	STK_ID						m_idRender;			//!< Render(渲染)ID
public:
	/****************************************************************************
	 *	@brief		Stk_Mesh_Face构造函数
	 *	@param[in]	i_stkProtoTypeP		Face所属的ProtoType对象
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_Mesh_Face(Stk_ProtoType* i_stkProtoTypeP);

	/****************************************************************************
	 *	@brief		Stk_Mesh_Face析构函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	~Stk_Mesh_Face(void);

public:
	/****************************************************************************
	 *	@brief		设置Face的LOD等级
	 *	@param[in]	i_nuLODLevel		LOD等级
	 *	@param[in]	i_LODInfoP			对应LOD等级的Face数据
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@retval		STK_ERROR			设置失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					SetFaceLODInfo(STK_UINT32 i_nuLODLevel, FaceLODInfo* i_LODInfoP);
	
	/****************************************************************************
	 *	@brief		获取指定LOD等级下的Face数据
	 *	@param[in]	i_nuLODLevel		LOD等级
	 *	@param[out]	无
	 *	@retval		itor->second		Face数据
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	FaceLODInfo*				GetFaceLODInfo(STK_UINT32 i_nuLODLevel);

	/****************************************************************************
	 *	@brief		获取所有LOD等级对应的Face数据
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_mapFaceLODInfo	LOD与Face数据的映射
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	const map<STK_UINT32, FaceLODInfo*>& GetAllFaceLOD();

	/****************************************************************************
	 *	@brief		清空所有的LOD信息
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					ClearAllFaceLOD();

	/****************************************************************************
	 *	@brief		设置面的颜色
	 *	@param[in]	i_rgbaFace		面颜色
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	void						SetColor(STK_RGBA32 i_rgbaFace);

	/****************************************************************************
	 *	@brief		获取面的颜色
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_rgbaFace	面颜色
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_RGBA32					GetColor();

	/****************************************************************************
	 *	@brief		判断面是否有颜色
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_bHasColor	面颜色状态
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_BOOL					HasColor();

	/****************************************************************************
	 *	@brief		设置面的材质
	 *	@param[in]	i_matFace		面材质
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	void						SetMaterial(const STK_MAT32 &i_matFace);
	
	/****************************************************************************
	 *	@brief		获取面的材质
	 *	@param[in]	无	
	 *	@param[out]	无
	 *	@retval		m_matFace	面材质
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_MAT32					GetMaterial();
	
	/****************************************************************************
	 *	@brief		判断面是否有材质
	 *	@param[in]	无	
	 *	@param[out]	无
	 *	@retval		m_bHasMaterial		是否有面材质的标识
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_BOOL					HasMaterial();

	/****************************************************************************
	 *	@brief		注册ID
	 *	@param[in]	i_idMeshFace	Face ID <br>
	 *				== STK_NEW_ID	注册新ID <br>
	 *				== 其它			注册指定ID
	 *	@retval		STK_SUCCESS		注册成功
	 *	@retval		STK_ERROR		ID重复注册失败
	 *	@note		指定ID需大于等于1的整数 <br>
	 *				由于V2.33版之前ID存在重复的可能，RegisterID时不对这些版本检测ID重复
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					RegisterID(STK_ID i_idMeshFace);

	/*****************************************************************************
	 *	@brief		写MeshFace数据
	 *	@param[in]	i_stkFileP			文件对象指针
	 *	@param[in]	i_head				数据段头指针
	 *	@param[in]	i_eEdgeStroeType	MeshEdge的存储方式
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			写入成功
	 *	@retval		其它				写入失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					WriteFile(StkSegmentHead *i_head, Stk_File *i_stkFileP,StkMeshEdgeStoreTypeEnum i_eEdgeStroeType);

	/*****************************************************************************
	 *	@brief		增加与此面关联的MeshEdge的ID
	 *	@param[in]	i_IDEdge	关联的边的ID
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS	设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					AddMeshEdgeID(STK_ID i_idEdge);

	/*****************************************************************************
	 *	@brief		获取与此面关联的MeshEdge的条数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		nuEdgeNum		边界线条数
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_UINT32					GetMeshEdgeNum();

	/*****************************************************************************
	 *	@brief		获取与此面关联的MeshEdge的ID
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		与面相关联的边界线ID列
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	const vector<STK_ID>&		GetMeshEdgeIDs();

	/*****************************************************************************
	 *	@brief		获取与此面关联的MeshEdge
	 *	@param[in]	i_idEdge		边界线ID
	 *	@param[out]	无
	 *	@retval		stkMeshEdgeP	MeshEdge对象
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_Mesh_Edge*				GetMeshEdgeByID(STK_ID i_idEdge);

	/****************************************************************************
	 *	@brief		设置面的Render ID
	 *	@param[in]	i_idRender		面的Render ID
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					SetRenderID(STK_ID i_idRender);

	/****************************************************************************
	 *	@brief		获取面的Render ID
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		面的Render ID
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_ID						GetRenderID();

	/****************************************************************************
	 *	@brief		通过ID获取指定的Render(渲染)信息
	 *	@param[in]	i_idRender		Render ID
	 *	@param[out]	o_stkRenderP	检索到的渲染对象
	 *	@retval		STK_SUCCESS		检索成功
	 *	@retval		其它			检索失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					GetRenderByID(STK_ID i_idRender, Stk_Render* &o_stkRenderP);
};


/**************************************************************************************
	 *	@class	Stk_Mesh_Com
	 *	@brief	Mesh 复合图形类
	 *
	 *	保存Mesh的复合图形数据信息。<br>
	 *	继承自Stk_MeshEntity类。
***************************************************************************************/

typedef struct {
	STK_UINT32					PloygonNum;
	STK_FLOAT32					AngularTol;
	STK_FLOAT32					ChordError;
	vector<STK_PNT32>			Vertices;
	vector<STK_PNT32>			UVs;
}MeshLODInfo;

class DATAMANAGER_API Stk_Mesh_Com:
	public Stk_MeshEntity
{
private:
	Stk_Node*					m_NodeP;			//!< 所属Node
	STK_BOOL					m_bHasColor;		//!< 是否有颜色
	STK_RGBA32					m_rgbaMesh;			//!< RGB颜色
	vector<Stk_Mesh_Face *>		m_vcSubFaces;		//!< 构成面
	STK_BOOL					m_bHasMaterial;		//!< 是否有材质
	STK_MAT32					m_matMesh;			//!< 材质
	STK_BOOL					m_bIsClosed;		//!< 是否闭合
	STK_BOOL					m_bIsOnTop;			//!< 是否最前端显示
	STK_BOOL					m_bIsUniColor;		//!< 是否统一颜色（无Face颜色）
	STK_BOOL					m_bIsDisplay;		//!< 是否显示
	StkMeshStoreTypeEnum		m_eMeshType;		//!< Mesh存储类型
	StkMeshEdgeStoreTypeEnum	m_eMeshEgdeType;	//!< MeshEdge存储类型
	map<STK_ID, Stk_Mesh_Edge*>	m_mapMeshEdges;		//!< MeshEdge
	map<wstring,map<STK_ID,Stk_UserData*>*>	m_mapUserDatas;	//!< 用户数据
	STK_SIZE_T					m_szUserDataCnt;	//!< 用户数据数量
	STK_BOOL					m_bHasRender;		//!< 是否有Render(渲染)
	STK_ID						m_idRender;			//!< Render(渲染)ID
public:
	//LOD(0-8)
	map<STK_UINT32,MeshLODInfo*>	m_mapMeshLODInfo;	//!< LOD下的一系列信息

public:
	/****************************************************************************
	 *	@brief		Stk_Mesh_Com构造函数
	 *	@param[in]	i_stkProtoTypeP		当前Mesh所属的ProtoType对象
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_Mesh_Com(Stk_ProtoType* i_stkProtoTypeP);

	/****************************************************************************
	 *	@brief		Stk_Mesh_Com析构函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		如果已关联到Node，会自动析构Node并解除Node父子关联
	 *	@attention	无
	 ****************************************************************************/
	~Stk_Mesh_Com(void);

public:
	/****************************************************************************
	 *	@brief		向Mesh中添加Face数据
	 *	@param[in]	i_FaceP		Face类对象指针
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS	添加成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					AddMeshFace(Stk_Mesh_Face *i_FaceP);

	/****************************************************************************
	 *	@brief		获取Mesh中Face的数量
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		szNum	Face数量
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_SIZE_T					GetMeshFaceNum();

	/****************************************************************************
	 *	@brief		获取Mesh中指定的Face
	 *	@param[in]	i_nuKeyPose		指定的Face的索引
	 *	@param[out]	无
	 *	@retval		outMeshFaceP	指定的Face对象指针
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_Mesh_Face*				GetMeshFace(STK_UINT32 i_nuKeyPose);

	/****************************************************************************
	 *	@brief		从Mesh中删除指定的Face
	 *	@param[in]	i_stkFaceP		指定的Face对象
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		删除成功
	 *	@retval		其它			删除失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					DeleteMeshFace(Stk_Mesh_Face* i_stkFaceP);

	/*****************************************************************************
	 *	@brief		增加Mesh下的边界线
	 *	@param[in]	i_MeshEdgeP		边界线对象
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		获取成功
	  *	@retval		STK_ERROR		获取失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					AddMeshEdge(Stk_Mesh_Edge *i_MeshEdgeP);

	/*****************************************************************************
	 *	@brief		获取该MeshCom下所有MeshEdge的ID列
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		边界线ID列
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	vector<STK_ID>				GetMeshEdgeIDs();

	/*****************************************************************************
	 *	@brief		通过ID来获取MeshEdge
	 *	@param[in]	i_idEdge		ID号
	 *	@param[out]	无
	 *	@retval		MeshEdge指针
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_Mesh_Edge*				GetMeshEdgeByID(STK_ID i_idEdge);

	/****************************************************************************
	 *	@brief		设置Mesh的颜色
	 *	@param[in]	i_rgbaMesh		Mesh颜色
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	void						SetColor(const STK_RGBA32 &i_rgbaMesh);
	
	/****************************************************************************
	 *	@brief		获取Mesh的颜色
	 *	@param[in]	无		
	 *	@param[out]	无
	 *	@retval		m_rgbaMesh	Mesh颜色
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_RGBA32					GetColor();

	/****************************************************************************
	 *	@brief		判断Mesh是否有颜色
	 *	@param[in]	无		
	 *	@param[out]	无
	 *	@retval		m_bHasColor	Mesh颜色状态
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_BOOL					HasColor();

	/****************************************************************************
	 *	@brief		读取文件中的Mesh数据
	 *	@param[in]	i_FileP				文件对象指针
	 *	@param[in]	i_MeshHeadP			Mesh记录头指针
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			读取成功
	 *	@retval		STK_ERROR_MEMORY	内存申请失败
	 *	@retval		STK_ERROR			读取失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					ReadFile(Stk_File *i_FileP, StkSegmentHead *i_MeshHeadP);
	
	/****************************************************************************
	 *	@brief		注册ID
	 *	@param[in]	i_idMeshCom			Mesh ID <br>
	 *				== STK_NEW_ID	注册新ID <br>
	 *				== 其它			注册指定ID
	 *	@retval		STK_SUCCESS		注册成功
	 *	@retval		STK_ERROR		ID重复注册失败
	 *	@note		指定ID需大于等于1的整数 <br>
	 *				由于V2.33版之前ID存在重复的可能，RegisterID时不对这些版本检测ID重复
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					RegisterID(STK_ID i_idMeshCom);
	
	/*****************************************************************************
	 *	@brief		写Mesh数据
	 *	@param[in]	i_stkFileP			文件对象指针
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			写入成功
	 *	@retval		其它				写入失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					WriteFile(Stk_File *i_stkFileP);
	
	/****************************************************************************
	 *	@brief		设置Mesh的LOD等级
	 *	@param[in]	i_nuLODLevel		LOD等级
	 *	@param[in]	i_MeshLODInfoP		对应LOD等级的Mesh数据
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@retval		STK_ERROR			设置失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					SetMeshLODInfo(STK_UINT32 i_nuLODLevel, MeshLODInfo* i_MeshLODInfoP);
	
	/****************************************************************************
	 *	@brief		获取指定LOD等级下的Mesh数据
	 *	@param[in]	i_nuLODLevel		LOD等级
	 *	@param[out]	无
	 *	@retval		指定等级的Mesh数据
	 *	@note		不建议使用，请使用GetAllMeshLOD替代
	 *	@attention	无
	 ****************************************************************************/
	MeshLODInfo*				GetMeshLODInfo(STK_UINT32 i_nuLODLevel);

	/****************************************************************************
	 *	@brief		获取所有LOD对应的Mesh数据
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_mapMeshLODInfo	LOD与Mesh数据对应的映射
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	const map<STK_UINT32,MeshLODInfo*>& GetAllMeshLOD();

	/****************************************************************************
	 *	@brief		设置Mesh材质
	 *	@param[in]	i_matMesh		Mesh材质
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	void						SetMaterial(const STK_MAT32 &i_matMesh);
	
	/****************************************************************************
	 *	@brief		获取Mesh材质
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_matMesh		Mesh材质
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_MAT32					GetMaterial();

	/****************************************************************************
	 *	@brief		判断Mesh是否有材质
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_bHasMaterial		是否有Mesh材质标识
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_BOOL					HasMaterial();

	/****************************************************************************
	 *	@brief		判断Mesh是否闭合
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_bIsClosed		Mesh闭合状态
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_BOOL					IsClosed();

	/****************************************************************************
	 *	@brief		设置Mesh闭合状态
	 *	@param[in]	inIsClosed		Mesh闭合状态
	 *	@param[out]	无
	 *	@retval		无	
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	void						SetClosed(STK_BOOL inIsClosed);

	/****************************************************************************
	 *	@brief		判断Mesh是否最前端显示
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_bIsOnTop		是否最前端显示
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_BOOL					IsDisplayOnTop();

	/****************************************************************************
	 *	@brief		设置Mesh是否最前端显示
	 *	@param[in]	inIsOnTop		是否最前端显示
	 *	@param[out]	无
	 *	@retval		无	
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	void						SetDisplayOnTop(STK_BOOL inIsOnTop);

	/****************************************************************************
	 *	@brief		判断Mesh是否统一色
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_bIsUniColor	Mesh统一色状态
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_BOOL					IsUniColor();

	/****************************************************************************
	 *	@brief		设置Mesh是否统一色
	 *	@param[in]	i_bUniColor		Mesh统一色状态
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	void						SetUniColor(STK_BOOL i_bUniColor);

	/****************************************************************************
	 *	@brief		获取Mesh存储类型
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_nuMeshType	Mesh存储类型
	 *	@note		=1 不支持LOD方式 <br>
	 *				=2 LOD方式一：不共享点列 <br>
	 *				=3 LOD方式二：共享点列
	 *	@attention	无
	 ****************************************************************************/
	StkMeshStoreTypeEnum		GetMeshType();
	
	/****************************************************************************
	 *	@brief		设置Mesh存储类型
	 *	@param[in]	i_eMeshType		Mesh存储类型
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		=1 不支持LOD方式 <br>
	 *				=2 LOD方式一：不共享点列 <br>
	 *				=3 LOD方式二：共享点列
	 *	@attention	无
	 ****************************************************************************/
	void						SetMeshType(StkMeshStoreTypeEnum i_eMeshType);

	/****************************************************************************
	 *	@brief		获取MeshEdge的存储类型
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_eMeshEgdeType
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	StkMeshEdgeStoreTypeEnum	GetMeshEdgeType();

	/*****************************************************************************
	 *	@brief		设置所对应的Node
	 *	@param[in]	i_NodeP				所对应的Node
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					SetNode(Stk_Node* i_NodeP);

	/*****************************************************************************
	 *	@brief		获取所对应的Node
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		所对应的Node
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	Stk_Node*					GetNode();

	/****************************************************************************
	 *	@brief		插入用户数据
	 *	@param[in]	i_stkUserDataP		用户数据指针
	 *	@retval		STK_SUCCESS			成功
	 *	@retval		其它				失败
	 *	@note		1.请new Stk_UserData对象并将用户数据流填入，然后使用Stk_Mesh_Com::AddUserData()插入
	 *				2.通过“用户数据名称”+“用户数据ID”的方式来作为唯一标识
	 *				  例如“类型”+“1” 表示第一个数据， “类型”+“2” 表示第二个数据
	 *				  当两次设置的唯一标识相同时，会自动覆盖掉前次的数据
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					AddUserData(Stk_UserData* i_stkUserDataP);

	/****************************************************************************
	 *	@brief		通过名称获取用户数据列表
	 *	@param[in]	用户数据名称
	 *	@param[out]	无
	 *	@retval		用户数据列表
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	map<STK_ID,Stk_UserData*>* GetUserDataByName(wstring i_wstrDataName);

	/****************************************************************************
	 *	@brief		删除指定的用户数据
	 *	@param[in]	i_wstrDataName		用户数据名称
	 *	@param[in]	i_id				用户数据ID
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			找到数据，删除成功
	 *	@retval		STK_ERROR			没有找到数据，删除失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					DelUserData( wstring i_wstrDataName, STK_ID i_id);

	/*****************************************************************************
	 *	@brief		往SVL文件中填写用户数据
	 *	@param[in]	i_stkFileP		SVL文件指针
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		填写成功
	 *	@note		无
	 *	@attention	内部使用
	 ****************************************************************************/
	STK_STATUS					WriteUserData(Stk_File* i_stkFileP);

	/*****************************************************************************
	 *	@brief		从SVL文件中读取用户数据
	 *	@param[in]	i_stkFileP		SVL文件指针
	 *	@param[in]	i_UserHeadP		自定义属性记录头
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		填写成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					ReadUserDatas(Stk_File *i_FileP, StkSegmentHead *i_UserHeadP);

	/****************************************************************************
	 *	@brief		判断Mesh是否显示/隐藏
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		显示/隐藏状态
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_BOOL					IsDisplay();

	/****************************************************************************
	 *	@brief		设置Mesh是否显示/隐藏
	 *	@param[in]	i_bDisplay		显示/隐藏状态
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					SetDisplay(STK_BOOL i_bDisplay);

	/****************************************************************************
	 *	@brief		清空MeshCom里的点列及索引数据
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		清空成功
	 *	@retval		其他			清空失败
	 *	@note		无
	 *	@attention	清空MeshCom中的点列及Face和Edge索引
	 ****************************************************************************/
	STK_STATUS					ClearMeshVertexAndIndex();

	/****************************************************************************
	 *	@brief		设置面的Render ID
	 *	@param[in]	i_idRender		面的Render ID
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					SetRenderID(STK_ID i_idRender);

	/****************************************************************************
	 *	@brief		获取面的Render ID
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		面的Render ID
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_ID						GetRenderID();

	/****************************************************************************
	 *	@brief		通过ID获取指定的Render(渲染)信息
	 *	@param[in]	i_idRender		Render ID
	 *	@param[out]	o_stkRenderP	检索到的渲染对象
	 *	@retval		STK_SUCCESS		检索成功
	 *	@retval		其它			检索失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					GetRenderByID(STK_ID i_idRender, Stk_Render* &o_stkRenderP);
};

#endif