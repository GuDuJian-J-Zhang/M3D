// All Rights Reserved. Copyright (C) 2014 HOTEAMSOFT, Ltd
/*****************************************************************************
 *	@file	Stk_Node.h
 *
 *	@brief	Node节点类
 *
 *	@par	历史:
 *		2014/04/16	创建。Gaoqq@HOTEAMSOFT
****************************************************************************/

#ifndef _STK_NODEENTITY_H_
#define _STK_NODEENTITY_H_

#ifndef __MOBILE__
#pragma once
#endif

#include "Stk_Entity.h"
#include "Stk_File.h"
#include "Stk_DocumentManager.h"
#include "Stk_TopoEntity.h"
#include "Stk_PMIEntity.h"
#include "Stk_View.h"
#include "Stk_MetaDataEntity.h"
#include "Stk_LayerInfosSet.h"
#include "Stk_Information.h"
#include "Stk_Feat.h"

/****************************************************************************
	 *	@class	Stk_Node
	 *	@brief	Node类
	 *
	 *	创建用于存储Node节点数据的类库。<br>
 ****************************************************************************/
typedef struct
{
	STK_ID		plcid;
	STK_ID		nodeid;
}PlcNode;

class DATAMANAGER_API Stk_Node
	//public Stk_Entity
{
private:
	Stk_DocumentManager*		m_DocManager;
	Stk_ProtoType*				m_stkProtoP;		//!< 所属零件ID
	STK_UINT32					m_nuID;				//!< NodeID
	STK_BOOL					m_bDispStat;		//!< 是否隐藏
	StkNodeTypeEnum				m_eType;			//!< Node类型
	StkNodeSubTypeEnum			m_eSubType;			//!< Node Sub类型
	wstring						m_wstrName;			//!< Node名称
	//wstring					m_wstrLink;			//!< LinkString
	vector<PlcNode>				m_vcChildNodeIDs;	//!< 子Node列
													//!< 注意：Stk_Node与子Node的联合，应该采用ID而不是指针
	Stk_Entity*					m_dataP;			//!< Node数据
	STK_INT32					m_idRefCount;		//!< 对象计数器
	vector<Stk_Information*>	m_vcInfos;			//!< Node节点基本信息
	STK_ID						m_idMaxPlc;			//!< 最大PLCID（不是该Node的Plcid）
	Stk_Feat*					m_stkFeatP;			//!< 特征
	vector<Stk_Node*>			m_vcPaNodes;		//!< 父Node列，可以是多个
	vector<Stk_NodeConnector*>	m_vcNodeConnectors;	//!< Node与要素的关联
public:
	/****************************************************************************
	 *	@brief		Stk_Node构造函数
	 *	@param[in]	i_DocManagerP		DocumentManager
	 *	@param[in]	i_stkProtoP			ProtoType
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_Node( Stk_DocumentManager *i_DocManagerP, Stk_ProtoType* i_stkProtoP);
	
	/****************************************************************************
	 *	@brief		Stk_Node析构函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		1. 会删除Node所包含的数据	<br>
	 *				2. 会移除与父Node的关联	<br>
	 *				3. 会删除子Node
	 *	@attention	无
	 ****************************************************************************/
	~Stk_Node();

	/****************************************************************************
	 *	@brief		引用计数器
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	void					AddRef();

	/****************************************************************************
	 *	@brief		获取引用计数器
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		引用计数器
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_INT32				GetRef();

	/****************************************************************************
	 *	@brief		Node内存释放
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			释放
	 *				STK_SUCCESS_EXIST	跳过
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				Release();

public:
	/****************************************************************************
	 *	@brief		设置Node ID
	 *	@param[in]	i_Id			Node ID
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetID(STK_UINT32 i_Id);

	/****************************************************************************
	 *	@brief		获取Node ID
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_nuID			Node ID
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_UINT32				GetID();

#if 0
	/****************************************************************************
	 *	@brief		获取Node Link
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_wstrLink	Node Link
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	wstring					GetLinkString();

	/****************************************************************************
	 *	@brief		设置Node Link
	 *	@param[in]	i_wstrLink		Node Link
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		成功
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				SetLinkString(wstring i_wstrLink);

	/****************************************************************************
	 *	@brief		注册Node路径
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			注册成功
	 *	@retval		STK_SUCCESS_EXIST	已注册
	 *	@retval		其他				注册失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				RegNodeLink();
#endif

	/****************************************************************************
	 *	@brief		注册Node ID
	 *	@param[in]	i_idNode		Node ID <br>
	 *				== STK_NEW_ID				注册新ID <br>
	 *				== 其它						注册指定ID
	 *	@retval		STK_SUCCESS					注册成功
	 *	@retval		STK_SUCCESS_EXIST			指定ID已被注册，退出
	 *	@retval		STK_ERROR					注册失败
	 *	@note		指定ID需大于等于1的整数
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				RegisterID(STK_ID i_idNode);
	
	/****************************************************************************
	 *	@brief		读取Node文件
	 *	@param[in]	i_stkFileP			文件指针
	 *	@param[in]	i_NodeHeadSP		Node表头
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			读取成功
	 *	@retval		STK_SUCCESS_EXIST	已读取跳出
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				ReadFile(Stk_File *i_stkFileP, StkSegmentHead *i_NodeHeadSP);
	
	/****************************************************************************
	 *	@brief		写Node文件
	 *	@param[in]	i_stkFileP		文件指针
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		写入成功
	 *	@retval		其他			写入失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				WriteFile(Stk_File *i_stkFileP);

	/****************************************************************************
	 *	@brief		设置Node类型
	 *	@param[in]	i_eType			Node类型
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetNodeType(StkNodeTypeEnum i_eType);
	
	/****************************************************************************
	 *	@brief		获取Node类型
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_eType			Node类型
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	StkNodeTypeEnum			GetNodeType();

	/****************************************************************************
	 *	@brief		设置显隐状态
	 *	@param[in]	i_bDispStat		显隐状态
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetDispStat(STK_BOOL i_bDispStat);
	
	/****************************************************************************
	 *	@brief		获取显隐状态
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_bDispStat		显隐状态
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_BOOL				GetDispStat();

	/****************************************************************************
	 *	@brief		设置Node名称
	 *	@param[in]	i_wstrNodeName	Node名称
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *  @note		最大63字符
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetNodeName(wstring i_wstrNodeName);
	
	/****************************************************************************
	 *	@brief		获取Node名称
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_wstrName		Node名称
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	wstring					GetNodeName();

	/****************************************************************************
	 *	@brief		获取当前Node节点的子节点的数目
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		子节点的数目
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_SIZE_T				GetChildNodeNum();

	/****************************************************************************
	 *	@brief		获取当前Node节点的指定索引的子节点
	 *	@param[in]	i_nuKeyPose		指定索引
	 *	@param[out]	o_nuPlcID		制定Node的配置ID
	 *	@retval		stkNodeP		指定Node的指针
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_Node*				GetChildNode(STK_UINT32 i_nuKeyPose, STK_ID &o_nuPlcID);

	/****************************************************************************
	 *	@brief		获取当前Node节点的父节点的数目
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		父节点的数目
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_SIZE_T				GetParentNodeNum();

	/****************************************************************************
	 *	@brief		获取当前Node节点的指定索引的父节点
	 *	@param[in]	i_nuKeyPose		指定索引
	 *	@param[out]	无
	 *	@retval		stkNodeP		指定的Node节点
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_Node*				GetParentNodeByIndex(STK_UINT32 i_nuIndex);

	/****************************************************************************
	 *	@brief		向当前Node节点中添加父节点
	 *	@param[in]	i_stkNodeP		父Node指针
	 *	@retval		STK_SUCCESS		添加成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				AddParentNode(Stk_Node* i_stkNodeP);

	/****************************************************************************
	 *	@brief		向当前Node节点中添加子节点
	 *	@param[in]	i_idNode		子节点ID
	 *	@param[out]	o_idPlc			配置ID
	 *	@retval		STK_SUCCESS		添加成功
	 *	@note		子Node会自动AddRef
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				AddChildNode(STK_ID i_idNode, STK_ID& o_idPlc);

	/****************************************************************************
	 *	@brief		通过配置ID删除子Node，释放子Node内存
	 *	@param[in]	i_idPlc			配置ID
	 *	@retval		STK_SUCCESS		删除成功
	 *	@retval		STK_ERROR		删除失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				DelChildNodeByPlc(STK_ID i_idPlc);

	/****************************************************************************
	 *	@brief		删除所有子Node，释放子Node内存
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		删除成功
	 *	@retval		STK_ERROR		删除失败
	 *	@note		delete node时会自动调用DelAllChildNodes，请避免重复调用
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				DelAllChildNodes();

	/****************************************************************************
	 *	@brief		断开与所有父Node的关联
	 *	@param[in]	无
	 *	@retval		STK_SUCCESS		删除成功
	 *	@retval		STK_ERROR		删除失败
	 *	@note		仅从所有的父Node中移除关联，不释放Node的内存
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				RemoveFromAllParents();

	/****************************************************************************
	 *	@brief		获取配置ID
	 *	@param[in]	无
	 *	@retval		配置ID
	 *	@note		配置ID不存在在Node节点中，而且一个Node节点可以被配置多次 <br>
	 *				因此仅仅通过Stk_Node指针是无法判定其PlcID的。
	 *	@attention	无
	 ****************************************************************************/
	//STK_ID		GetPlcID();

	/****************************************************************************
	 *	@brief		向当前Node节点中添加基本信息
	 *	@param[in]	stkInfoP		添加基本信息
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		添加成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				AddInfo(Stk_Information* stkInfoP);

	/****************************************************************************
	 *	@brief		获取基本信息大小
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		基本信息大小
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_UINT32				GetInfoNum();

	/****************************************************************************
	 *	@brief		获取指定索引的基本信息
	 *	@param[in]	i_nuIndex			指定的索引
	 *	@param[out]	无
	 *	@retval		Stk_Information		基本信息
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_Information*		GetInfoByIndex(STK_UINT32 i_nuIndex);

	/****************************************************************************
	 *	@brief		获取Body指针
	 *	@param[in]	i_stkBodyP		Body对象指针
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@retval		其它			设置失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetBody(Stk_Body* i_stkBodyP);
	
	/****************************************************************************
	 *	@brief		获取Body指针
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_dataP			Body对象指针
	 *	@retval		其它			获取失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_Body*				GetBody();

	/****************************************************************************
	 *	@brief		获取Camera指针
	 *	@param[in]	i_stkCameraP		Camera对象指针
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@retval		其它			设置失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetCamera(Stk_Camera* i_stkCameraP);
	
	/****************************************************************************
	 *	@brief		获取Camera指针
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_dataP			Camera对象指针
	 *	@retval		其它			获取失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_Camera*				GetCamera();


	/****************************************************************************
	 *	@brief		设置Mesh指针
	 *	@param[in]	i_stkMeshP		Mesh指针
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@retval		其它			设置失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetMesh(Stk_Mesh_Com* i_stkMeshP);

	/****************************************************************************
	 *	@brief		获取Mesh指针
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_dataP			Mesh对象指针
	 *	@retval		其它			获取失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_Mesh_Com*			GetMesh();

	/****************************************************************************
	 *	@brief		获取PMI指针
	 *	@param[in]	i_stkPMIP		PMI对象指针
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@retval		其它			设置失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetPMI(Stk_PMI* i_stkPMIP);
	
	/****************************************************************************
	 *	@brief		获取PMI指针
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_dataP			PMI对象指针
	 *	@retval		其它			获取失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_PMI*				GetPMI();

	/****************************************************************************
	 *	@brief		设置View指针
	 *	@param[in]	i_stkViewP		视图对象指针
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@retval		其它			设置失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetView(Stk_View* i_stkViewP);
	
	/****************************************************************************
	 *	@brief		获取View指针
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_dataP			PMI对象指针
	 *	@retval		其它			获取失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_View*				GetView();

	/****************************************************************************
	 *	@brief		设置MetaData指针
	 *	@param[in]	i_stkMetaDataP	自定义对象指针
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@retval		其它			设置失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetMetaData(Stk_MetaData* i_stkMetaDataP);
	
	/****************************************************************************
	 *	@brief		获取自定义对象指针
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_dataP			自定义属性对象指针
	 *	@retval		其它			获取失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_MetaData*			GetMetaData();

	/****************************************************************************
	 *	@brief		设置图层信息集
	 *	@param[in]	i_LayerInfosSet	图层信息集
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@retval		其它			设置失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetLayerInfosSet(Stk_LayerInfosSet* i_LayerInfosSet);
	
	/****************************************************************************
	 *	@brief		获取图层信息集
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_dataP			图层信息集对象指针
	 *	@retval		其它			获取失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_LayerInfosSet*		GetLayerInfosSet();
	
	/****************************************************************************
	 *	@brief		移除子Node，不释放子Node内存
	 *	@param[in]	i_idPlc			Node配置ID
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		移除成功
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				RemoveChild(STK_ID i_idPlc);

	/****************************************************************************
	 *	@brief		删除子Node并释放内存
	 *	@param[in]	i_idNode		Node ID
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		删除成功
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				DeleteChildNodeByID(STK_ID i_idNode);

	/****************************************************************************
	 *	@brief		填写特征数据
	 *	@param[in]	i_stkFeatP		特征数据
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		填写成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetFeat(Stk_Feat* i_stkFeatP);

	/****************************************************************************
	 *	@brief		获取特征数据
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		特征数据
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_Feat*				GetFeat();

	/****************************************************************************
	 *	@brief		获取DocManager
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		DocManager
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_DocumentManager*	GetDocManager();

	/****************************************************************************
	 *	@brief		添加Node与要素之间的关联
	 *	@param[in]	i_NodeConnectorP	Node与要素之间的关联
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			添加成功
	 *	@retval		STK_ERROR			添加失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					AddNodeConnector(Stk_NodeConnector* i_NodeConnectorP);

	/****************************************************************************
	 *	@brief		获取Node与要素之间的关联的数量
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		Node与要素之间的关联的数量
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_UINT32					GetNodeConnectorNum();

	/****************************************************************************
	 *	@brief		获取指定的Node与要素之间的关联
	 *	@param[in]	i_nuIndex		指定的关联的索引号
	 *	@param[out]	无
	 *	@retval		指定的Node与要素之间的关联
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_NodeConnector*			GetNodeConnectorByIndex(STK_UINT32 i_nuIndex);

	/****************************************************************************
	 *	@brief		删除Node与要素之间的关联
	 *	@param[in]	i_idNode			被关联的Node ID号
	 *	@param[in]	i_PointedEnt		被指向要素类型
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			删除成功
	 *	@retval		STK_ERROR			删除失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					DelNodeConnector(STK_ID i_idNode, STK_UINT32 i_PointedEnt);

	/****************************************************************************
	 *	@brief		删除Node与实例之间的关联
	 *	@param[in]	i_wstrPlcPath		被关联的实例配置路径
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			删除成功
	 *	@retval		STK_ERROR			删除失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					DelNodeConnector(wstring i_wstrPlcPath);

	/****************************************************************************
	 *	@brief		设置Node Sub类型
	 *	@param[in]	i_eSubType		Node Sub类型
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetNodeSubType(StkNodeSubTypeEnum i_eSubType);
	
	/****************************************************************************
	 *	@brief		获取Node Sub类型
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		Node Sub类型
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	StkNodeSubTypeEnum		GetNodeSubType();
};

#endif