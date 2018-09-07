// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_NodeConnector.h
 *
 *	@brief	Stk_NodeConnector关联类
 *
 *	@par	历史:
 *		2012/11/30	创建。Gaoqq@HOTEAMSOFT
****************************************************************************/

#ifndef _Stk_CONNECTOR_ENTITY_H_
#define _Stk_CONNECTOR_ENTITY_H_

#ifndef __MOBILE__
#pragma once
#endif

#include "DataManager.h"
#include "Stk_Entity.h"
#include "Stk_File.h"


/**************************************************************************************
	 *	@class	Stk_NodeConnector
	 *	@brief	关联类
	 *
	 *	Node之间的关联。<br>
***************************************************************************************/

typedef struct{
	STK_ID					idEntity;
	StkEntityTypeEnum		nuEntityType;
	STK_PNT32				pntPick;		// 为攻长义应用层提供，Pick坐标
	STK_DIR32				dirPick;		// 为攻长义应用层提供，Pick方向向量
}SubEntity;

class DATAMANAGER_API  Stk_NodeConnector
{
private:
	Stk_ProtoType*				m_stkProtoP;			//!< 所属零件

	StkEntityTypeEnum			m_ePointingEntityType;	//!< 要素类型
	StkEntityTypeEnum			m_ePointedEntityType;	//!< 被指向要素的类型

	STK_BOOL					m_bIsLinkProto;			//!< 是否指向其它ProtoType
	wstring						m_wstrPlcPath;			//!< 配置路径

	STK_BOOL					m_bIsLinkNode;			//!< 是否指向Node
	STK_ID						m_idNode;				//!< NodeConnectorID

	STK_BOOL					m_bIsLinkEntities;		//!< 是否指向Node下的要素
	vector<SubEntity>			m_vcEntities;			//!< Node下的要素列表（例如边、面、图层等）

public:
	/****************************************************************************
	 *	@brief		Stk_NodeConnector构造函数
	 *	@param[in]	i_stkProtoTypeP		ProtoType对象指针
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_NodeConnector(Stk_ProtoType* i_stkProtoTypeP);
	
	/****************************************************************************
	 *	@brief		Stk_NodeConnector析构函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	~Stk_NodeConnector(void);

public:
	/****************************************************************************
	 *	@brief		设置Entity类型
	 *	@param[in]	i_ePointingEnt			指向类型
	 *	@param[in]	i_ePointedEnt			被指向类型
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	void						SetEntityType(StkEntityTypeEnum i_ePointingEnt, StkEntityTypeEnum i_ePointedEnt );
	
	/****************************************************************************
	 *	@brief		获取Entity类型
	 *	@param[in]	无
	 *	@param[out]	o_ePointingEnt			指向类型
	 *	@param[out]	o_ePointedEnt			被指向类型
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	void						GetEntityType(StkEntityTypeEnum& o_ePointingEnt, StkEntityTypeEnum& o_ePointedEnt );

	/****************************************************************************
	 *	@brief		设置Proto Link
	 *	@param[in]	i_wstrPlcPath			配置路径
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	void						SetLinkProto(const wstring &i_wstrPlcPath);
	
	/****************************************************************************
	 *	@brief		获取Proto Link
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_wstrPlcPath	配置路径
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	wstring						GetLinkProto();

	/****************************************************************************
	 *	@brief		判断是否有Proto Link
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_bIsLinkProto	配置路径状态
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_BOOL					IsLinkProto();

	/****************************************************************************
	 *	@brief		设置NodeConnector Link
	 *	@param[in]	i_idNode		Node ID
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		例如PMI、图层信息集等Node级
	 *	@attention	无
	 ****************************************************************************/
	void						SetLinkNode(STK_ID i_idNode);
	
	/****************************************************************************
	 *	@brief		获取NodeConnector Link
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_idNode		Node ID
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_ID						GetLinkNode();
	
	/****************************************************************************
	 *	@brief		判断是否有Node Link
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_bIsLinkNode	Node Link状态
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_BOOL					IsLinkNode();

	/****************************************************************************
	 *	@brief		设置子Entity
	 *	@param[in]	i_SubEntity		子Entity
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		例如边、面、图层等
	 *	@attention	无
	 ****************************************************************************/
	void						AddSubEntity(SubEntity i_SubEntity);
	
	/****************************************************************************
	 *	@brief		获取子Entity
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_vcEntities	所有子Entity
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	vector<SubEntity>			GetSubEntity();
	
	/****************************************************************************
	 *	@brief		判断是否有子Entity Link
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_bIsLinkEntities	子Entity Link状态
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_BOOL					IsLinkEntities();

	/****************************************************************************
	 *	@brief		读取文件中的关联类数据
	 *	@param[in]	i_FileP				文件对象指针
	 *	@param[in]	i_ViewHeadP			View记录头指针
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			读取成功
	 *	@retval		STK_ERROR_MEMORY	内存申请失败
	 *	@retval		其他				读取失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					ReadFile(Stk_File *i_FileP, StkSegmentHead *i_ViewHeadP);
	
	/*****************************************************************************
	 *	@brief		写关联类数据
	 *	@param[in]	i_stkFileP			文件对象指针
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			写入成功
	 *	@retval		STK_ERROR			写入失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					WriteFile(Stk_File *i_stkFileP);
};

#endif