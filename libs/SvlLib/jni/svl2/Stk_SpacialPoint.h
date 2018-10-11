// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_SpacialPointEntity.h
 *
 *	@brief	Stk_SpacialPoint空间点类
 *
 *	@par	历史:
 *		2014/04/17	创建。WangY@HOTEAMSOFT
****************************************************************************/

#ifndef _Stk_SPACIALPOINT_ENTITY_H_
#define _Stk_SPACIALPOINT_ENTITY_H_

#pragma once
#include "DataManager.h"
#include "Stk_Entity.h"

class Stk_ProtoType;
class Stk_File;

/**************************************************************************************
	 *	@class	Stk_SpacialPointEntity
	 *	@brief	空间点类
	 *
	 *	空间点类。<br>
	 *	继承自Stk_Entity类。
***************************************************************************************/

class DATAMANAGER_API  Stk_SpacialPointEntity :
	public Stk_Entity
{
private:
	Stk_ProtoType*					m_stkProtoP;					//!< 所属零件
	STK_ID							m_ID;							//!< 空间点ID
	wstring							m_wstrName;						//!< 空间点名字
	STK_BOOL						m_bDispStat;					//!< 空间点是否隐藏
	StkSpacialPointTypeEnum			m_eType;						//!< 点的类型
	STK_RGBA32						m_rgbaPoint;					//!< 点颜色
	STK_PNT32						m_pntPos;						//!< 点位置
public:
	/****************************************************************************
	 *	@brief		Stk_SpacialPointEntity构造函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_SpacialPointEntity();

	/****************************************************************************
	 *	@brief		Stk_SpacialPointEntity构造函数
	 *	@param[in]	i_stkProtoTypeP		空间点所属的组件类对象
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_SpacialPointEntity(Stk_ProtoType* i_stkProtoTypeP);
	//Stk_SpacialPointEntity(const Stk_SpacialPointEntity& i_stkSpacialPoint);
	
	/****************************************************************************
	 *	@brief		Stk_SpacialPointEntity析构函数
	 *	@param[in]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	~Stk_SpacialPointEntity();
	//Stk_SpacialPointEntity& operator = (const Stk_SpacialPointEntity& i_stkSpacialPoint);
public:
	/****************************************************************************
	 *	@brief		设置空间点ID
	 *	@param[in]	i_ID	空间点ID
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS						SetID(STK_ID i_ID);
	
	/****************************************************************************
	 *	@brief		获取空间点ID
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_ID	空间点ID
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_ID							GetID();
	
	/****************************************************************************
	 *	@brief		获取ProtoType ID
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		ProtoType ID
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_ID							GetProtoTypeID();
	
	/****************************************************************************
	 *	@brief		设置空间点的名字
	 *	@param[in]	i_wstrName		空间点的名字
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS						SetName(wstring i_wstrName);
	
	/****************************************************************************
	 *	@brief		获取空间点的名字
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_wstrName	空间点的名字
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	wstring							GetName();

	/****************************************************************************
	 *	@brief		设置显隐状态
	 *	@param[in]	i_bDispStat		显隐状态
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS						SetDispStat(STK_BOOL i_bDispStat);
	
	/****************************************************************************
	 *	@brief		获取显隐状态
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_bDispStat		显隐状态
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_BOOL						GetDispStat();

	/****************************************************************************
	 *	@brief		设置空间点的类型
	 *	@param[in]	i_eType			空间点的类型
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS						SetType(StkSpacialPointTypeEnum i_eType);
	
	/****************************************************************************
	 *	@brief		获取空间点的类型
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_eType		空间点的类型
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	StkSpacialPointTypeEnum			GetType();

	/****************************************************************************
	 *	@brief		设置空间点的颜色
	 *	@param[in]	i_rgbaSpacialPoint		空间点的颜色
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS						SetColor(STK_RGBA32 i_rgbaSpacialPoint);
	
	/****************************************************************************
	 *	@brief		获取空间点的颜色
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_rgbaPoint		空间点的颜色
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_RGBA32						GetColor();

	/****************************************************************************
	 *	@brief		设置空间点的位置
	 *	@param[in]	i_pntPos		空间点的位置
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS						SetPntPosition(STK_PNT32 i_pntPos);
	
	/****************************************************************************
	 *	@brief		获取空间点的位置
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_pntPos		空间点的位置
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_PNT32						GetPntPosition();

	/****************************************************************************
	 *	@brief		从SVL中读取空间点
	 *	@param[in]	i_stkFileP			SVL文件指针
	 *	@param[in]	i_PointHeadP		空间点段头
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			读取成功
	 *	@retval		STK_ERROR_MEMORY	内存申请失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS						ReadSpacialPoint(Stk_File* i_stkFileP, StkSegmentHead* i_PointHeadP);
	
	/****************************************************************************
	 *	@brief		往SVL文件填写空间点
	 *	@param[in]	i_PointHeadP	空间点段头
	 *	@param[in]	i_stkFileP		SVL文件指针
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		填写成功
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS						WriteSpacialPoint(StkSegmentHead* i_PointHeadP, Stk_File *i_stkFileP);
};

#endif