// All Rights Reserved. Copyright (C) 2014 HOTEAMSOFT, Ltd
/*****************************************************************************
 *	@file	Stk_Info.h
 *
 *	@brief	Node信息类
 *
 *	@par	历史:
 *		2014/04/16	创建。WangY@HOTEAMSOFT
****************************************************************************/

#ifndef _STK_INFOENTITY_H_
#define _STK_INFOENTITY_H_

#pragma once
#include "DataManager.h"
#include "Stk_Entity.h"
#include "Stk_File.h"

/****************************************************************************
	 *	@class	Stk_Information
	 *	@brief	Node信息类
	 *
	 *	创建用于存储Node节点基本信息数据的类库。<br>
 ****************************************************************************/
class DATAMANAGER_API Stk_Information
{
private:
	Stk_ProtoType*	m_stkPrototypeP;
	STK_INT32		m_nLayer;
public:
	/****************************************************************************
	 *	@brief		Stk_Information构造函数
	 *	@param[in]	i_stkPrototypeP			ProtoType对象指针
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_Information(Stk_ProtoType* i_stkPrototypeP);
	//Stk_Information(const Stk_Information& i_stkInfo);

	/****************************************************************************
	 *	@brief		Stk_Information析构函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	~Stk_Information();
	//Stk_Information& operator = (const Stk_Information& i_stkInfo);

	/****************************************************************************
	 *	@brief		设置节点所在图层
	 *	@param[in]	i_nLayer		节点所在图层
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS		SetLayer(STK_INT32 i_nLayer);

	/****************************************************************************
	 *	@brief		获取节点所在图层
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_nuLayer		节点所在图层
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_INT32		GetLayer();

	/****************************************************************************
	 *	@brief		写基本信息段
	 *	@param[in]	i_stkFileP		文件指针
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		写入成功
	 *	@retval		其他			写入失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS		WriteFile(Stk_File *i_stkFileP);

	/****************************************************************************
	 *	@brief		读基本信息段
	 *	@param[in]	i_FileP			文件指针
	 *	@param[in]	i_InfoHeadP		段头
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		写入成功
	 *	@retval		其他			写入失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS		ReadFile(Stk_File *i_FileP, StkSegmentHead *i_InfoHeadP);
};

#endif