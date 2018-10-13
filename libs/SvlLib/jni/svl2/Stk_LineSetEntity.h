// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_LineSetEntity.h
 *
 *	@brief	Stk_LineSet线集类
 *
 *	@par	历史:
 *		2014/03/04	创建。WangY@HOTEAMSOFT
****************************************************************************/

#ifndef _Stk_SUBLINE_ENTITY_H_
#define _Stk_SUBLINE_ENTITY_H_

#ifndef __MOBILE__
#pragma once
#endif

#include "DataManager.h"
#include "Stk_File.h"
#include "Stk_CurveEntity.h"
#include "Stk_Entity.h"

/**************************************************************************************
	 *	@class	Stk_LineSet
	 *	@brief	线集类
	 *
	 *	线集类。<br>
	 *	继承自Stk_Entity类。
***************************************************************************************/

class DATAMANAGER_API  Stk_LineSet :
	public Stk_Entity
{
private:
	wstring						m_wstrName;						//!< 线集名字
	StkLineSetTypeEnum			m_eType;						//!< 线集类型
	STK_BOOL					m_bDispStat;					//!< 线集是否隐藏
	STK_RGBA32					m_rgbaLineSet;					//!< 线集颜色
	vector<Stk_CurveEntity*>	m_vcLines;						//!< 线集
public:
	/****************************************************************************
	 *	@brief		Stk_LineSet构造函数
	 *	@param[in]	i_stkProtoTypeP		ProtoType对象指针
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_LineSet(Stk_ProtoType* i_stkProtoTypeP);
	
	/****************************************************************************
	 *	@brief		Stk_LineSet析构函数
	 *	@param[in]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	~Stk_LineSet();
public:
	/****************************************************************************
	 *	@brief		注册线集ID
	 *	@param[in]	i_idLineSet		线集ID <br>
	 *				== STK_NEW_ID	注册新ID <br>
	 *				== 其它			注册指定ID
	 *	@retval		STK_SUCCESS		注册成功
	 *	@retval		STK_ERROR		注册失败
	 *	@note		指定ID需大于等于1的整数
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					RegisterID(STK_ID i_idLineSet);

	/****************************************************************************
	 *	@brief		获取线集ID
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_ID			线集ID
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_ID						GetID();

	/****************************************************************************
	 *	@brief		设置线集ID
	 *	@param[in]	i_ID			线集ID
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					SetID(STK_ID i_ID);

	/****************************************************************************
	 *	@brief		获取ProtoTypeID
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		STK_ID
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_ID						GetProtoTypeID();

	/****************************************************************************
	 *	@brief		设置线集名字
	 *	@param[in]	i_wstrName		线集名字
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					SetName(wstring i_wstrName);

	/****************************************************************************
	 *	@brief		获取线集名字
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_wstrName		线集名字
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	wstring						GetName();
	
	/****************************************************************************
	 *	@brief		设置线集显隐状态
	 *	@param[in]	i_bDispStat		线集显隐状态
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					SetDispStat(STK_BOOL i_bDispStat);
	
	/****************************************************************************
	 *	@brief		获取线集显隐状态
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_bDispStat		线集显隐状态
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_BOOL					GetDispStat();

	/****************************************************************************
	 *	@brief		设置线集颜色
	 *	@param[in]	i_rgbaLineSet	线集颜色
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					SetColor(STK_RGBA32 i_rgbaLineSet);
	
	/****************************************************************************
	 *	@brief		获取线集颜色
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_rgbaLineSet		线集颜色
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_RGBA32					GetColor();

	/****************************************************************************
	 *	@brief		设置线集颜色
	 *	@param[in]	i_rgbaLineSet	线集颜色
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					SetType(StkLineSetTypeEnum i_eType);

	/****************************************************************************
	 *	@brief		获取线集颜色
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_rgbaLineSet		线集颜色
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	StkLineSetTypeEnum			GetType();

	/****************************************************************************
	 *	@brief		往线集中添加线数据
	 *	@param[in]	i_stkLine			线数据
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			添加成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					AddLine(Stk_CurveEntity* i_stkLine);
	
	/****************************************************************************
	 *	@brief		获取线集中的线数据
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_vcLines			线集中的线数据
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	vector<Stk_CurveEntity*>&	GetAllLines();

	/****************************************************************************
	 *	@brief		从SVL中读取线集数据
	 *	@param[in]	i_FileP				SVL文件指针
	 *	@param[in]	i_LineSetHeadP		线集段头
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			读取成功
	 *	@retval		STK_ERROR_MEMORY	内存申请失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					ReadFile(Stk_File* i_FileP, StkSegmentHead* i_LineHeadP);
	
	/****************************************************************************
	 *	@brief		往SVL文件填写线集数据
	 *	@param[in]	i_stkFileP		SVL文件指针
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		填写成功
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					WriteLineSet(Stk_File *i_stkFileP);
};

#endif