// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_Manager.h
 *
 *	@brief	SVLToolKit命令管理器
 *
 *	@par	历史:
 *		2012/11/30	创建。Gaoqq@HOTEAMSOFT
****************************************************************************/

#ifndef _STK_MANAGER_H_
#define _STK_MANAGER_H_

#ifndef __MOBILE__
#pragma once
#endif

#include "DataManager.h"
#include "Stk_File.h"
#include "Stk_Instance.h"
#include "Stk_ProtoType.h"
#include "Stk_TopoEntity.h"
#include "Stk_MeshEntity.h"
#include "Stk_PMIEntity.h"
#include "Stk_View.h"
#include "Stk_Node.h"
#include "Stk_Feat.h"
#include "Stk_ComponentFeature.h"
#include "Stk_Render.h"

/**************************************************************************************
	 *	@class	Stk_ProtoIDManager
	 *	@brief	ProtoType（零件）内部ID管理类
	 *
***************************************************************************************/
typedef map<STK_ID, Stk_File*>			Stk_File_Map;
typedef map<STK_ID, Stk_Body*>			Stk_Body_Map;
typedef map<STK_ID, Stk_Mesh_Com*>		Stk_MeshCom_Map;
typedef map<STK_ID, Stk_Mesh_Face*>		Stk_MeshFace_Map;
typedef map<STK_ID, Stk_Mesh_Edge*>		Stk_MeshEdge_Map;
typedef map<STK_ID, Stk_PMI*>			Stk_PMI_Map;
typedef map<STK_ID, Stk_View*>			Stk_View_Map;
typedef map<STK_ID, Stk_NodeConnector*>	Stk_NodeConnector_Map;
typedef map<STK_ID, Stk_LineSet*>		Stk_LineSet_Map;
typedef map<STK_ID, Stk_Node*>			Stk_Node_Map;
//typedef map<wstring, STK_ID>			Stk_NodeLinkString_Map;
typedef map<STK_ID, Stk_Feat*>			Stk_Feat_Map;
typedef map<STK_ID, Stk_ComponentFeature*>	Stk_ComponentFeat_Map;	// 组件特征
typedef map<STK_ID, Stk_Render*>		Stk_Render_Map;
typedef map<STK_ID, Stk_CurveEntity*>	Stk_Curve_Map;

class DATAMANAGER_API  Stk_ProtoIDManager
{
private:
	Stk_Body_Map			m_mapBodyID;
	Stk_MeshCom_Map			m_mapMeshComID;
	Stk_MeshFace_Map		m_mapMeshFaceID;
	Stk_MeshEdge_Map		m_mapMeshEdgeID;
	Stk_PMI_Map				m_mapPMIID;
	Stk_View_Map			m_mapViewID;
	Stk_NodeConnector_Map	m_mapNodeConnectorID;
	Stk_LineSet_Map			m_mapLineSetID;
	Stk_Feat_Map			m_mapFeatID;
	Stk_ComponentFeat_Map	m_mapComponentFeatID;
	Stk_Render_Map			m_mapRenderID;
	Stk_Curve_Map			m_mapCurveID;
	// Node唯一发行
	Stk_Node_Map			m_mapNodeID;
	//Stk_NodeLinkString_Map m_mapNodeLinkString;
	//STK_UINT32			 m_nuMaxNodeLinkID;
public:
	/****************************************************************************
	 *	@brief		Stk_ProtoIDManager构造函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	Stk_ProtoIDManager(void);

	/****************************************************************************
	 *	@brief		Stk_ProtoIDManager析构函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	~Stk_ProtoIDManager(void);

	/****************************************************************************
	 *	@brief		通过ID获取Body信息
	 *	@param[in]	i_idBody				Body ID
	 *	@param[out]	o_BodyP					Body类对象指针
	 *	@retval		STK_SUCCESS				获取成功
	 *	@retval		STK_ERROR_ID_NOT_FOUND	获取失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				GetBodyByID(STK_ID i_idBody, Stk_Body* &o_BodyP);
	
	/****************************************************************************
	 *	@brief		通过ID获取Mesh信息
	 *	@param[in]	i_idMeshCom				Mesh ID
	 *	@param[out]	o_MeshComP				Mesh类对象指针
	 *	@retval		STK_SUCCESS				获取成功
	 *	@retval		STK_ERROR_ID_NOT_FOUND	获取失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				GetMeshComByID(STK_ID i_idMeshCom, Stk_Mesh_Com* &o_MeshComP);
	
	/****************************************************************************
	 *	@brief		通过ID获取Face信息
	 *	@param[in]	i_idMeshFace			Face ID
	 *	@param[out]	o_MeshFaceP				Face类对象指针
	 *	@retval		STK_SUCCESS				获取成功
	 *	@retval		STK_ERROR_ID_NOT_FOUND	获取失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				GetMeshFaceByID(STK_ID i_idMeshFace, Stk_Mesh_Face* &o_MeshFaceP);
	
	/****************************************************************************
	 *	@brief		通过ID获取Edge信息
	 *	@param[in]	i_idMeshEdge			Edge ID
	 *	@param[out]	o_MeshEdgeP				Edge类对象指针
	 *	@retval		STK_SUCCESS				获取成功
	 *	@retval		STK_ERROR_ID_NOT_FOUND	获取失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				GetMeshEdgeByID(STK_ID i_idMeshEdge, Stk_Mesh_Edge* &o_MeshEdgeP);
	
	/****************************************************************************
	 *	@brief		通过ID获取PMI信息
	 *	@param[in]	i_idPMI					PMI ID
	 *	@param[out]	o_stkPMIP				PMI类对象指针
	 *	@retval		STK_SUCCESS				获取成功
	 *	@retval		STK_ERROR_ID_NOT_FOUND	获取失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				GetPMIByID( STK_ID i_idPMI, Stk_PMI* &o_stkPMIP);
	
	/****************************************************************************
	 *	@brief		通过ID获取视图信息
	 *	@param[in]	i_idView				视图 ID
	 *	@param[out]	o_stkViewP				视图类对象指针
	 *	@retval		STK_SUCCESS				获取成功
	 *	@retval		STK_ERROR_ID_NOT_FOUND	获取失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				GetViewByID(STK_ID i_idView, Stk_View* &o_stkViewP);
	
	/****************************************************************************
	 *	@brief		通过ID获取视图与PMI的关联信息
	 *	@param[in]	i_idNodeConnector		视图与PMI的关联 ID
	 *	@param[out]	o_stkNodeConnectorP		视图与PMI的关联类对象指针
	 *	@retval		STK_SUCCESS				获取成功
	 *	@retval		STK_ERROR_ID_NOT_FOUND	获取失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				GetNodeConnectorByID(STK_ID i_idNodeConnector, Stk_NodeConnector* &o_stkNodeConnectorP);
	
	/****************************************************************************
	 *	@brief		通过ID获取线集
	 *	@param[in]	i_idLineSet				线集 ID
	 *	@param[out]	o_stkLineSetP			线集对象指针
	 *	@retval		STK_SUCCESS				获取成功
	 *	@retval		STK_ERROR_ID_NOT_FOUND	获取失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				GetLineSetByID(STK_ID i_idLineSet, Stk_LineSet* &o_stkLineSetP);
	
	/****************************************************************************
	 *	@brief		通过ID获取特征
	 *	@param[in]	i_idFeat				特征 ID
	 *	@param[out]	o_stkFeatP				特征对象指针
	 *	@retval		STK_SUCCESS				获取成功
	 *	@retval		STK_ERROR_ID_NOT_FOUND	获取失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				GetFeatByID(STK_ID i_idFeat, Stk_Feat* &o_stkFeatP);

	/****************************************************************************
	 *	@brief		通过ID获取Render信息
	 *	@param[in]	i_idRender				Render ID
	 *	@param[out]	o_stkRenderP			Render类对象指针
	 *	@retval		STK_SUCCESS				获取成功
	 *	@retval		STK_ERROR_ID_NOT_FOUND	获取失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				GetRenderByID(STK_ID i_idRender, Stk_Render* &o_stkRenderP);

	/****************************************************************************
	 *	@brief		注册Body ID
	 *	@param[in]	i_BodyP				Body类对象指针
	 *	@param[out]	io_idBody			Body ID
	 *	@retval		STK_SUCCESS			注册成功
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				RegBodyID(Stk_Body* i_BodyP, STK_ID& io_idBody);
	
	/****************************************************************************
	 *	@brief		注册Mesh ID
	 *	@param[in]	i_MeshComP			Mesh类对象指针
	 *	@param[out]	o_idMeshCom			Mesh ID
	 *	@retval		STK_SUCCESS			注册成功
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				RegMeshComID(Stk_Mesh_Com* i_MeshComP, STK_ID &o_idMeshCom);
	
	/****************************************************************************
	 *	@brief		注册Face ID
	 *	@param[in]	i_MeshFaceP			Face类对象指针
	 *	@param[out]	o_idMeshFace		Face ID
	 *	@retval		STK_SUCCESS			注册成功
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				RegMeshFaceID(Stk_Mesh_Face* i_MeshFaceP, STK_ID &o_idMeshFace);
	
	/****************************************************************************
	 *	@brief		注册Edge ID
	 *	@param[in]	i_MeshEdgeP			Edge类对象指针
	 *	@param[out]	o_idMeshEdge		Edge ID
	 *	@retval		STK_SUCCESS			注册成功
	 *	@retval		STK_ERROR			注册失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				RegMeshEdgeID(Stk_Mesh_Edge* i_MeshEdgeP, STK_ID &o_idMeshEdge);
	
	/****************************************************************************
	 *	@brief		注册PMI ID
	 *	@param[in]	i_stkPMIP			PMI类对象指针
	 *	@param[out]	io_idPMI			PMI ID
	 *	@retval		STK_SUCCESS			注册成功
	 *	@retval		STK_ERROR			注册失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				RegPMIID(Stk_PMI* i_stkPMIP, STK_ID &io_idPMI );
	
	/****************************************************************************
	 *	@brief		注册View ID
	 *	@param[in]	i_stkViewP			视图类对象指针
	 *	@param[out]	io_idView			视图 ID
	 *	@retval		STK_SUCCESS			注册成功
	 *	@retval		其他				注册失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				RegViewID(Stk_View* i_stkViewP, STK_ID& io_idView);
	
	/****************************************************************************
	 *	@brief		注册视图与PMI关联 ID
	 *	@param[in]	i_stkNodeConnectorP		视图与PMI关联类对象指针
	 *	@param[out]	o_idNodeConnector		视图与PMI关联 ID
	 *	@retval		STK_SUCCESS				注册成功
	 *	@retval		其他					注册失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				RegNodeConnectorID(Stk_NodeConnector* i_stkNodeConnectorP, STK_ID& o_idNodeConnector);
	
	/****************************************************************************
	 *	@brief		注册线集 ID
	 *	@param[in]	i_stkLineSetP		线集对象指针
	 *	@param[out]	o_idLineSet			线集 ID
	 *	@retval		STK_SUCCESS			注册成功
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				RegLineSetID(Stk_LineSet* i_stkLineSetP, STK_ID& o_idLineSet);
	
	/****************************************************************************
	 *	@brief		注册特征ID
	 *	@param[in]	i_stkFeatP			特征类对象指针
	 *	@param[out]	o_idFeat			特征ID
	 *	@retval		STK_SUCCESS			注册成功
	 *	@retval		其他				注册失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				RegFeatID(Stk_Feat* i_stkFeatP, STK_ID& o_idFeat);

	/****************************************************************************
	 *	@brief		注册Render ID
	 *	@param[in]	i_stkRenderP		Render类对象指针
	 *	@param[out]	io_idRender			Render ID
	 *	@retval		STK_SUCCESS			注册成功
	 *	@retval		STK_ERROR			注册失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				RegRenderID(Stk_Render* i_stkRenderP, STK_ID &io_idRender);

	/****************************************************************************
	 *	@brief		注册Curve ID
	 *	@param[in]	i_stkCurveP			Curve对象指针
	 *	@param[out]	o_idCurve			Curve ID
	 *	@retval		STK_SUCCESS			注册成功
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				RegCurveID(Stk_CurveEntity* i_stkCurveP, STK_ID& io_idCurve);
	
	// 因为IDMANAGER只给数据管理内部使用，不对外提供。
	// 这里暂时确定一个原则：IDMANAGER中的Del方法，只删除ID，不删除对象 by Gaoqq on 2013.6.28
	
	/****************************************************************************
	 *	@brief		删除Body与ID之间的映射
	 *	@param[in]	i_idBody			Body ID
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			删除成功
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				DelBodyByID(STK_ID i_idBody);
	
	/****************************************************************************
	 *	@brief		删除Mesh与ID之间的映射
	 *	@param[in]	i_idMeshCom			Mesh ID
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			删除成功
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				DelMeshComByID(STK_ID i_idMeshCom);

	/****************************************************************************
	 *	@brief		删除Face与ID之间的映射
	 *	@param[in]	i_idMeshFace		Face ID
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			删除成功
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				DelMeshFaceByID(STK_ID i_idMeshFace);

	/****************************************************************************
	 *	@brief		删除Edge与ID之间的映射
	 *	@param[in]	i_idMeshEdge		Edge ID
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			删除成功
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				DelMeshEdgeByID(STK_ID i_idMeshEdge);

	/****************************************************************************
	 *	@brief		删除PMI与ID之间的映射
	 *	@param[in]	i_idPMI				PMI ID
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			删除成功
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				DelPMIByID(STK_ID	i_idPMI);

	/****************************************************************************
	 *	@brief		删除View与ID之间的映射
	 *	@param[in]	i_idView			View ID
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			删除成功
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				DelViewByID(STK_ID i_idView);

	/****************************************************************************
	 *	@brief		删除视图和PMI之间的关联与ID之间的映射
	 *	@param[in]	i_idNodeConnector	视图和PMI之间的关联 ID
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			删除成功
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				DelNodeConnectorByID(STK_ID i_idNodeConnector);

	/****************************************************************************
	 *	@brief		删除线集与ID之间的映射
	 *	@param[in]	i_idLineSet			线集 ID
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			删除成功
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				DelLineSetByID(STK_ID i_idLineSet);

	/****************************************************************************
	 *	@brief		通过制定ID删除特征
	 *	@param[in]	i_idFeat					特征ID
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS					删除成功
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				DelFeatByID(STK_ID i_idFeat);

	/****************************************************************************
	 *	@brief		删除Render与ID之间的映射
	 *	@param[in]	i_idRender			Render ID
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			删除成功
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				DelRenderByID(STK_ID i_idRender);

	// Node唯一性管理

	/****************************************************************************
	 *	@brief		注册Node LinkString与ID的映射表
	 *	@param[in]	i_wstrLink					Node LinkString
	 *	@param[out]	o_idNode					Node ID
	 *	@retval		STK_SUCCESS					注册成功
	 *	@retval		STK_SUCCESS_EXIST			Link已注册
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				RegNodeLinkString( const wstring &i_wstrLink,STK_ID &o_idNode);
	
	/****************************************************************************
	 *	@brief		获取Node LinkString与ID的映射表
	 *	@param[in]	i_wstrLink					Node LinkString
	 *	@param[out]	o_idNode					Node ID
	 *	@retval		STK_SUCCESS					获取成功
	 *	@retval		STK_ERROR_ID_NOT_FOUND		ID未找到
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				GetNodeIDByLinkString( const wstring &i_wstrLink, STK_ID &o_idNode );
	
	/****************************************************************************
	 *	@brief		删除Node LinkString与ID的映射
	 *	@param[in]	i_wstrLink					Node LinkString
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS					删除成功
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				DelNodeLinkString(const wstring &i_wstrLink);
	
	/****************************************************************************
	 *	@brief		注册Node ID
	 *	@param[in]	i_NodeP						Node指针
	 *	@param[out]	io_idNode					Node ID
	 *				== STK_NEW_ID				注册新ID <br>
	 *				== 其它						注册指定ID
	 *	@retval		STK_SUCCESS					注册成功
	 *	@retval		STK_SUCCESS_EXIST			指定ID已被注册，退出
	 *	@retval		STK_ERROR					注册失败
	 *	@note		
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				RegNodeID(Stk_Node* o_stkNodeP, STK_ID &i_idNode);
	
	/****************************************************************************
	 *	@brief		通过ID获取Node
	 *	@param[in]	i_idNode					Node ID
	 *	@param[out]	o_NodeP						Node类对象指针
	 *	@retval		STK_SUCCESS					获取成功	
	 *	@retval		STK_ERROR_ID_NOT_FOUND		未找到ID
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				GetNodeByID(STK_ID i_idNode, Stk_Node* &o_NodeP);

	/****************************************************************************
	 *	@brief		删除Node与ID的映射
	 *	@param[in]	i_idNode				Node ID
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS					删除成功
	 *	@retval		STK_ERROR_ID_NOT_FOUND		ID未找到
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				DelNodeByID(STK_ID i_idNode);

	/****************************************************************************
	 *	@brief		注册组件特征 ID
	 *	@param[in]	i_ComponentFeatP			组件特征指针
	 *	@param[out]	io_idComponentFeat			组件特征 ID
	 *	@retval		STK_SUCCESS					注册成功
	 *	@retval		STK_ERROR					注册失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				RegComponentFeatID(Stk_ComponentFeature* i_ComponentFeatP, STK_ID& io_idComponentFeat);

	/****************************************************************************
	 *	@brief		通过ID获取组件特征
	 *	@param[in]	i_idComponentFeat			组件特征 ID
	 *	@param[out]	o_ComponentFeatP			组件特征类对象指针
	 *	@retval		STK_SUCCESS					获取成功	
	 *	@retval		STK_ERROR_ID_NOT_FOUND		未找到ID
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				GetComponentFeatByID(STK_ID i_idComponentFeat, Stk_ComponentFeature* &o_ComponentFeatP);
	
	/****************************************************************************
	 *	@brief		删除组件特征与ID的映射
	 *	@param[in]	i_idComponentFeat			组件特征 ID
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS					删除成功
	 *	@retval		STK_ERROR_ID_NOT_FOUND		ID未找到
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				DelComponentFeatID(STK_ID i_idComponentFeat);
};

/**************************************************************************************
	 *	@class	Stk_GlobalIDManager
	 *	@brief	全局ID管理类
	 *
***************************************************************************************/
typedef map<STK_ID, Stk_Instance*>			Stk_Instance_Map;
typedef map<STK_ID, Stk_ProtoType*>			Stk_ProtoType_Map;
//typedef map<STK_ID, Stk_File*>			Stk_ProtoIDManger_Map;
typedef map<wstring, STK_ID>				Stk_ProtoLinkString_Map;
//typedef map<wstring/*SVLFilePath*/, STK_ID> Stk_FileLinkString_Map; //打开SVL文件时用
typedef map<wstring/*CADFilePath*/, STK_ID> Stk_CADFilePath_Map; // 保存SVL时用

class DATAMANAGER_API  Stk_GlobalIDManager
{
private:
	//static Stk_GlobalIDManager*	s_GlobalIDInstanceP;	//!< 唯一实例
	//Stk_ProtoIDManger_Map	m_mapProtoIDManager; //改进到ProtoType内进行管理 2014.3.31 By Gaoqq

	Stk_File_Map			m_mapFileID;
	Stk_CADFilePath_Map		m_mapCADFilePath;
	STK_UINT32				m_nuMaxCADFileID;
	//Stk_FileLinkString_Map m_mapFileLinkString;
	//STK_UINT32			 m_nuMaxFileLinkID;

	Stk_Instance_Map		m_mapInstanceID;
	Stk_ProtoType_Map		m_mapProtoTypeID;
	Stk_ProtoLinkString_Map	m_mapProtoLinkString;
	STK_UINT32				m_nuMaxLinkID;
public:
	/****************************************************************************
	 *	@brief		Stk_GlobalIDManager构造函数
	 *	@param[in]	无		
	 *	@param[out]	无		
	 *	@retval		无		
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	Stk_GlobalIDManager(void);
	
	/****************************************************************************
	 *	@brief		Stk_GlobalIDManager析构函数
	 *	@param[in]	无		
	 *	@param[out]	无		
	 *	@retval		无		
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	~Stk_GlobalIDManager(void);

	// 获取ID管理器
	//static Stk_GlobalIDManager*	GetIDManager(void);

	/*****************************************************************************
	 *	@brief		设置ProtoType的修改状态
	 *	@param[in]	i_bModified				修改状态
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS				修改成功
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				SetModifiedFlag(STK_BOOL i_bModified = TRUE);
	//const Stk_File_Map&		GetAllFiles();

	//---------------------------------------------------------
	//					全局ID控制
	//---------------------------------------------------------
	// 浏览器打开SVL文件场景，File的发行是受FileLink决定

	/****************************************************************************
	 *	@brief		注册文件类对象ID
	 *	@param[in]	i_stkFileP					文件类对象指针
	 *	@param[out]	o_ID						文件类对象ID
	 *	@retval		STK_SUCCESS					注册成功
	  *	@retval		其它						注册失败
	 *	@note		特殊情况，拥有注册Link与RegisterID的能力
	 *	@attention	内部接口
	 ****************************************************************************/
	//STK_STATU				RegFileLink(Stk_File* i_stkFileP, STK_ID &o_ID);
	//STK_STATUS			GetFileIDByLinkString(const wstring &i_wstrLink, STK_ID &o_idFile);// 用GetCADFileIDByPath代替
	
	/****************************************************************************
	 *	@brief		删除FileLinkString与ID的映射
	 *	@param[in]	i_wstrFileLink				File LinkString
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS					删除成功
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	//STK_STATUS			DelFileLinkString(const wstring& i_wstrFileLink);	// add by Shaoqy on 2013/12/25

	// 转换器创建DataManager场景，File的发行是受CADFile决定
	
	/*****************************************************************************
	 *	@brief		注册CAD物理文件与路径之间的映射
	 *	@param[in]	i_wstrCADFilePath		CAD物理文件路径
	 *	@param[in]	o_ID					CAD File ID
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS				注册成功
	 *	@retval		其他					注册失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				RegCADFilePath(wstring i_wstrCADFilePath, STK_ID &o_ID);
	
	/*****************************************************************************
	 *	@brief		通过CAD物理文件路径查找指定的File的ID
	 *	@param[in]	i_wstrCADPath			CAD物理文件路径
	 *	@param[out]	o_ID					CAD File ID
	 *	@retval		STK_SUCCESS				查找成功
	 *	@retval		其他					查找失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				GetCADFileIDByPath(wstring i_wstrCADPath, STK_ID &o_ID);
	
	/*****************************************************************************
	 *	@brief		删除指定CAD物理文件路径的File
	 *	@param[in]	i_wstrCADPath			CAD物理文件路径
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS				删除成功
	 *	@retval		其他					删除失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				DelCADFileIDByPath(wstring i_wstrCADPath);

	// Proto的发行是受ProtoLink决定
	// 也就是说一个ProtoLink对应一个ProtoType
	
	/****************************************************************************
	 *	@brief		注册ProtoType LinkString与ID的映射表
	 *	@param[in]	i_wstrLink					ProtoType LinkString
	 *	@param[out]	o_idProtoType				ProtoType ID
	 *	@retval		STK_SUCCESS					注册成功
	 *	@retval		STK_SUCCESS_EXIST			Link已注册
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				RegProtoLinkString(const wstring &i_wstrLink, STK_ID &o_idProtoType);
	
	/****************************************************************************
	 *	@brief		获取ProtoType LinkString与ID的映射表
	 *	@param[in]	i_wstrLink					ProtoType LinkString
	 *	@param[out]	o_idProtoType				ProtoType ID
	 *	@retval		STK_SUCCESS					获取成功
	 *	@retval		STK_ERROR_ID_NOT_FOUND		ID未找到
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				GetProtoIDByLinkString(const wstring &i_wstrLink, STK_ID &o_idProtoType);
	
	/****************************************************************************
	 *	@brief		删除ProtoType LinkString与ID的映射
	 *	@param[in]	i_wstrLink					ProtoType LinkString
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS					删除成功
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				DelProtoLinkString(const wstring &i_wstrLink);

	/****************************************************************************
	 *	@brief		注册文件类对象ID
	 *	@param[in]	i_FileP						文件类对象指针
	 *	@param[out]	o_idFile					文件类对象ID
	 *	@retval		STK_SUCCESS					注册成功	
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				RegFileID( Stk_File* i_FileP, STK_ID &o_idFile);
	
	/****************************************************************************
	 *	@brief		通过ID获取文件管理器
	 *	@param[in]	i_idFile					文件类对象ID
	 *	@param[out]	o_FileP						文件类对象指针
	 *	@retval		STK_SUCCESS					获取成功	
	 *	@retval		STK_ERROR_ID_NOT_FOUND		未找到ID
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				GetFileByID(STK_ID i_idFile, Stk_File* &o_FileP);
	
	/****************************************************************************
	 *	@brief		删除文件类对象与ID的映射
	 *	@param[in]	i_idFile					文件类对象ID
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS					删除成功
	 *	@note		这里暂时确定一个原则：IDMANAGER中的Del方法，只删除ID，不删除对象
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				DelFileID(STK_ID i_idFile);

	/****************************************************************************
	 *	@brief			注册实例ID
	 *	@param[in]		i_InstanceP					实例指针
	 *	@param[out]	io_idInstance					实例ID
	 *	@retval			STK_SUCCESS					注册成功
	 *	@retval			STK_ERROR					注册失败
	 *	@note			无
	 *	@attention		内部接口
	 ****************************************************************************/
	STK_STATUS				RegInstanceID( Stk_Instance* i_InstanceP, STK_ID& io_idInstance);
	
	/****************************************************************************
	 *	@brief		通过ID获取实例
	 *	@param[in]	i_idInstance				实例ID
	 *	@param[out]	o_InstanceP					实例类对象指针
	 *	@retval		STK_SUCCESS					获取成功	
	 *	@retval		STK_ERROR_ID_NOT_FOUND		未找到ID
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				GetInstanceByID(STK_ID i_idInstance, Stk_Instance* &o_InstanceP);
	
	/****************************************************************************
	 *	@brief		删除实例类对象与ID的映射
	 *	@param[in]	i_idInstance				实例类对象ID
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS					删除成功
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				DelInstanceID(STK_ID i_idInstance);

	/****************************************************************************
	 *	@brief		注册ProtoType ID
	 *	@param[in]	i_ProtoTypeP				ProtoType指针
	 *	@param[out]	io_idProtoType				ProtoType ID
	 *	@retval		STK_SUCCESS					注册成功
	 *	@retval		STK_ERROR					注册失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				RegProtoTypeID( Stk_ProtoType* i_ProtoTypeP, STK_ID& io_idProtoType);
	
	/****************************************************************************
	 *	@brief		通过ID获取ProtoType
	 *	@param[in]	i_idProtoType				ProtoType ID
	 *	@param[out]	o_ProtoTypeP				ProtoType类对象指针
	 *	@retval		STK_SUCCESS					获取成功	
	 *	@retval		STK_ERROR_ID_NOT_FOUND		未找到ID
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				GetProtoTypeByID(STK_ID i_idProtoType, Stk_ProtoType* &o_ProtoTypeP);
	
	/****************************************************************************
	 *	@brief		删除ProtoType与ID的映射
	 *	@param[in]	i_idProtoType				ProtoType ID
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS					删除成功
	 *	@retval		STK_ERROR_ID_NOT_FOUND		ID未找到
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				DelProtoTypeID(STK_ID i_idProtoType);

	//---------------------------------------------------------
	//					零件内ID控制
	//---------------------------------------------------------

	/****************************************************************************
	 *	@brief		通过ID获取Body信息
	 *	@param[in]	i_idProtoType				ProtoType ID
	 *	@param[in]	i_idBody					Body ID
	 *	@param[out]	o_BodyP						Body类对象指针
	 *	@retval		STK_SUCCESS					获取成功
	 *	@retval		STK_ERROR_ID_NOT_FOUND		ID未找到
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				GetBodyByID(STK_ID i_idProtoType, STK_ID i_idBody, Stk_Body* &o_BodyP);
	
	/****************************************************************************
	 *	@brief		通过ID获取Mesh信息
	 *	@param[in]	i_idProtoType				ProtoType ID
	 *	@param[in]	i_idMeshCom					Mesh ID
	 *	@param[out]	o_MeshComP					Mesh类对象指针
	 *	@retval		STK_SUCCESS					获取成功
	 *	@retval		STK_ERROR_ID_NOT_FOUND		ID未找到
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				GetMeshComByID(STK_ID i_idProtoType, STK_ID i_idMeshCom, Stk_Mesh_Com* &o_MeshComP);
	
	/****************************************************************************
	 *	@brief		通过ID获取Face信息
	 *	@param[in]	i_idProtoType				ProtoType ID
	 *	@param[in]	i_idMeshFace				Face ID
	 *	@param[out]	o_MeshFaceP					Face类对象指针
	 *	@retval		STK_SUCCESS					获取成功
	 *	@retval		STK_ERROR_ID_NOT_FOUND		ID未找到
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				GetMeshFaceByID(STK_ID i_idProtoType, STK_ID i_idMeshFace, Stk_Mesh_Face* &o_MeshFaceP);
	
	/****************************************************************************
	 *	@brief		通过ID获取Edge信息
	 *	@param[in]	i_idProtoType				ProtoType ID
	 *	@param[in]	i_idMeshEdge				Edge ID
	 *	@param[out]	o_MeshEdgeP					Edge类对象指针
	 *	@retval		STK_SUCCESS					获取成功
	 *	@retval		STK_ERROR_ID_NOT_FOUND		ID未找到
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				GetMeshEdgeByID(STK_ID i_idProtoType, STK_ID i_idMeshEdge, Stk_Mesh_Edge* &o_MeshEdgeP);
	
	/****************************************************************************
	 *	@brief		通过ID获取PMI信息
	 *	@param[in]	i_idProtoType				ProtoType ID
	 *	@param[in]	i_idPMI						PMI ID
	 *	@param[out]	o_stkPMIP					PMI类对象指针
	 *	@retval		STK_SUCCESS					获取成功
	 *	@retval		STK_ERROR_ID_NOT_FOUND		ID未找到
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				GetPMIByID(STK_ID i_idProtoType,STK_ID i_idPMI, Stk_PMI* &o_stkPMIP);
	
	/****************************************************************************
	 *	@brief		通过ID获取视图信息
	 *	@param[in]	i_idProtoType				ProtoType ID
	 *	@param[in]	i_idView					视图 ID
	 *	@param[out]	o_stkViewP					视图类对象指针
	 *	@retval		STK_SUCCESS					获取成功
	 *	@retval		STK_ERROR_ID_NOT_FOUND		ID未找到
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				GetViewByID(STK_ID i_idProtoType, STK_ID i_idView, Stk_View* &o_ViewP);
	
	/****************************************************************************
	 *	@brief		通过ID获取视图与PMI的关联信息
	 *	@param[in]	i_idProtoType				ProtoType ID
	 *	@param[in]	i_idNodeConnector			视图与PMI的关联ID
	 *	@param[out]	o_NodeConnectorP			视图与PMI的类对象指针
	 *	@retval		STK_SUCCESS					获取成功
	 *	@retval		STK_ERROR_ID_NOT_FOUND		ID未找到
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				GetNodeConnectorByID(STK_ID i_idProtoType, STK_ID i_idNodeConnector, Stk_NodeConnector* &o_NodeConnectorP);
	
	/****************************************************************************
	 *	@brief		通过ID获取线集信息
	 *	@param[in]	i_idProtoType				ProtoType ID
	 *	@param[in]	i_idLineSet					线集 ID
	 *	@param[out]	o_LineSetP					线集对象指针
	 *	@retval		STK_SUCCESS					获取成功
	 *	@retval		STK_ERROR_ID_NOT_FOUND		ID未找到
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				GetLineSetByID(STK_ID i_idProtoType, STK_ID i_idLineSet, Stk_LineSet* &o_LineSetP);

	/****************************************************************************
	 *	@brief		通过ID获取Render信息
	 *	@param[in]	i_idProtoType				ProtoType ID
	 *	@param[in]	i_idRender					Render ID
	 *	@param[out]	o_stkRenderP				Render类对象指针
	 *	@retval		STK_SUCCESS					获取成功
	 *	@retval		STK_ERROR_ID_NOT_FOUND		ID未找到
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				GetRenderByID(STK_ID i_idProtoType,STK_ID i_idRender, Stk_Render* &o_stkRenderP);

	/****************************************************************************
	 *	@brief		注册Body的ID
	 *	@param[in]	i_idProtoType				ProtoType ID
	 *	@param[in]	i_BodyP						Body类对象指针
	 *	@param[out]	o_idBody					Body ID
	 *	@retval		STK_SUCCESS					注册成功
	 *	@retval		其它						注册失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				RegBodyID(STK_ID i_idProtoType, Stk_Body* i_BodyP, STK_ID &o_idBody);
	
	/****************************************************************************
	 *	@brief		注册Mesh的ID
	 *	@param[in]	i_idProtoType				ProtoType ID
	 *	@param[in]	i_MeshComP					Mesh类对象指针
	 *	@param[out]	o_idMeshCom					Mesh ID
	 *	@retval		STK_SUCCESS					注册成功
	 *	@retval		其它						注册失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				RegMeshComID(STK_ID i_idProtoType, Stk_Mesh_Com* i_MeshP, STK_ID &o_idMesh);
	
	/****************************************************************************
	 *	@brief		注册Face的ID
	 *	@param[in]	i_idProtoType				ProtoType ID
	 *	@param[in]	i_MeshFaceP					Face类对象指针
	 *	@param[out]	o_idMeshFace				Face ID
	 *	@retval		STK_SUCCESS					注册成功
	 *	@retval		其它						注册失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				RegMeshFaceID(STK_ID i_idProtoType, Stk_Mesh_Face* i_MeshFaceP, STK_ID &o_idMeshFace);
	
	/****************************************************************************
	 *	@brief		注册Edge的ID
	 *	@param[in]	i_idProtoType				ProtoType ID
	 *	@param[in]	i_MeshEdgeP					Edge类对象指针
	 *	@param[out]	o_idMeshEdge				Edge ID
	 *	@retval		STK_SUCCESS					注册成功
	 *	@retval		其它						注册失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				RegMeshEdgeID(STK_ID i_idProtoType, Stk_Mesh_Edge* i_MeshEdgeP, STK_ID &o_idMeshEdge);
	
	/****************************************************************************
	 *	@brief		注册PMI的ID
	 *	@param[in]	i_idProtoType				ProtoType ID
	 *	@param[in]	i_stkPMIP					PMI类对象指针
	 *	@param[out]	io_idPMI					PMI ID
	 *	@retval		STK_SUCCESS					注册成功
	 *	@retval		其它						注册失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				RegPMIID( STK_ID i_idProtoType, Stk_PMI* i_stkPMIP, STK_ID &io_idPMI);
	
	/****************************************************************************
	 *	@brief		注册View的ID
	 *	@param[in]	i_idProtoType				ProtoType ID
	 *	@param[in]	i_stkViewP					视图类对象指针
	 *	@param[out]	io_idView					视图 ID
	 *	@retval		STK_SUCCESS					注册成功
	 *	@retval		其它						注册失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				RegViewID(STK_ID i_idProtoType, Stk_View* i_stkViewP, STK_ID& io_idView);
	
	/****************************************************************************
	 *	@brief		注册视图与PMI关联的ID
	 *	@param[in]	i_idProtoType				ProtoType ID
	 *	@param[in]	i_stkNodeConnectorP			视图与PMI关联类对象指针
	 *	@param[out]	o_idNodeConnector			视图与PMI关联 ID
	 *	@retval		STK_SUCCESS					注册成功
	 *	@retval		其它						注册失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				RegNodeConnectorID(STK_ID i_idProtoType, Stk_NodeConnector* i_stkNodeConnectorP, STK_ID& o_idNodeConnector);
	
	/****************************************************************************
	 *	@brief		注册线集的ID
	 *	@param[in]	i_idProtoType				ProtoType ID
	 *	@param[in]	i_stkLineSetP				线集对象指针
	 *	@param[out]	o_idLineSet					线集 ID
	 *	@retval		STK_SUCCESS					注册成功
	 *	@retval		其它						注册失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				RegLineSetID(STK_ID i_idProtoType, Stk_LineSet* i_stkLineSetP, STK_ID& o_idLineSet);

	/****************************************************************************
	 *	@brief		注册Render的ID
	 *	@param[in]	i_idProtoType				ProtoType ID
	 *	@param[in]	i_stkRenderP				Render类对象指针
	 *	@param[out]	io_idRender					Render ID
	 *	@retval		STK_SUCCESS					注册成功
	 *	@retval		其它						注册失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				RegRenderID(STK_ID i_idProtoType, Stk_Render* i_stkRenderP, STK_ID &io_idRender);
	
	/****************************************************************************
	 *	@brief		删除Body与ID之间的映射
	 *	@param[in]	i_idProtoType			ProtoType ID
	 *	@param[in]	i_idBody				Body ID
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS				删除成功
	 *	@retval		其它						注册失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				DelBodyID(STK_ID i_idProtoType, STK_ID i_idBody);
	
	/****************************************************************************
	 *	@brief		删除Mesh与ID之间的映射
	 *	@param[in]	i_idProtoType			ProtoType ID
	 *	@param[in]	i_idMeshCom				Mesh ID
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS				删除成功
	 *	@retval		STK_ERROR_ID_NOT_FOUND	ID未找到
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				DelMeshComID(STK_ID i_idProtoType, STK_ID i_idMeshCom);
	
	/****************************************************************************
	 *	@brief		删除Face与ID之间的映射
	 *	@param[in]	i_idProtoType			ProtoType ID
	 *	@param[in]	i_idMeshFace			Face ID
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS				删除成功
	 *	@retval		STK_ERROR_ID_NOT_FOUND	ID未找到
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				DelMeshFaceID(STK_ID i_idProtoType, STK_ID i_idMeshFace);
	
	/****************************************************************************
	 *	@brief		删除Edge与ID之间的映射
	 *	@param[in]	i_idProtoType			ProtoType ID
	 *	@param[in]	i_idMeshEdge			Edge ID
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS				删除成功
	 *	@retval		STK_ERROR_ID_NOT_FOUND	ID未找到
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				DelMeshEdgeID(STK_ID i_idProtoType, STK_ID i_idMeshEdge);
	
	/****************************************************************************
	 *	@brief		删除PMI与ID之间的映射
	 *	@param[in]	i_idProtoType			ProtoType ID
	 *	@param[in]	i_idPMI					PMI ID
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS				删除成功
	 *	@retval		STK_ERROR_ID_NOT_FOUND	ID未找到
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				DelPMIID(STK_ID i_idProtoType,STK_ID i_idPMI);
	
	/****************************************************************************
	 *	@brief		删除View与ID之间的映射
	 *	@param[in]	i_idProtoType			ProtoType ID
	 *	@param[in]	i_idView				View ID
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS				删除成功
	 *	@retval		STK_ERROR_ID_NOT_FOUND	ID未找到
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				DelViewID(STK_ID i_idProtoType, STK_ID i_idView);
	
	/****************************************************************************
	 *	@brief		删除视图和PMI的关联与ID之间的映射
	 *	@param[in]	i_idProtoType			ProtoType ID
	 *	@param[in]	i_idNodeConnector		视图和PMI的关联 ID
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS				删除成功
	 *	@retval		STK_ERROR_ID_NOT_FOUND	ID未找到
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				DelNodeConnectorID(STK_ID i_idProtoType, STK_ID i_idNodeConnector);
	
	/****************************************************************************
	 *	@brief		删除线集与ID之间的映射
	 *	@param[in]	i_idProtoType			ProtoType ID
	 *	@param[in]	i_idLineSet				线集 ID
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS				删除成功
	 *	@retval		STK_ERROR_ID_NOT_FOUND	ID未找到
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				DelLineSetID(STK_ID i_idProtoType, STK_ID i_idLineSet);

	/****************************************************************************
	 *	@brief		删除Render与ID之间的映射
	 *	@param[in]	i_idProtoType			ProtoType ID
	 *	@param[in]	i_idRender				Render ID
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS				删除成功
	 *	@retval		STK_ERROR_ID_NOT_FOUND	ID未找到
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				DelRenderID(STK_ID i_idProtoType,STK_ID i_idRender);
};

#endif