// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_OutFrame.h
 *
 *	@brief	外框类
 *
 *	@par	历史:
 *		2013/08/19	创建。WangY@HOTEAMSOFT
 *		2013/12/14	更新。Gaoqq#HOTEAMSOFT
 *				对GetFrameData和SetFrameData接口定义进行改善
****************************************************************************/

#ifndef _Stk_OutFrame_H_
#define _Stk_OutFrame_H_

#ifndef __MOBILE__
#pragma once
#endif

#include "Stk_CurveEntity.h"
#include <vector>

using namespace std;


/**************************************************************************************
	 *	@class	Stk_OutFrame
	 *	@brief	OutFrame外框类
	 *
	 *	PMI的外框
***************************************************************************************/

class DATAMANAGER_API Stk_OutFrame
{
private:
	StkOutFrameUsageEnum			m_eFrameType;
	vector<Stk_CurveEntity* >		m_vcCurves;
	StkOutFrameDefEnum				m_eDefType;
public:
	/****************************************************************************
	 *	@brief		Stk_OutFrame构造函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_OutFrame(void);
	
	/****************************************************************************
	 *	@brief		Stk_OutFrame析构函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	~Stk_OutFrame(void);

	/****************************************************************************
	 *	@brief		获取外框的应用类型
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_eFrameType		外框类型
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	StkOutFrameUsageEnum			GetFrameType();
	
	/****************************************************************************
	 *	@brief		设置外框的应用类型
	 *	@param[in]	i_eType				外框类型
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS						SetFrameType(StkOutFrameUsageEnum i_eType);

	/****************************************************************************
	 *	@brief		获取所有的外框线数据
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_vcCurves			所有的外框线数据
	 *	@note		不再建议使用,使用GetFrameLineNum()和GetCurve()替代
	 *	@attention	无
	 ****************************************************************************/
	const vector<Stk_CurveEntity* >& GetFrameData();
	
	/****************************************************************************
	 *	@brief		设置外框线数据
	 *	@param[in]	i_CurveP			外框线数据
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		不再建议使用，使用AddCurve()替代
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS						SetFrameData(Stk_CurveEntity* i_CurveP);

	/****************************************************************************
	 *	@brief		获取外框的定义类型
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_eFrameType		外框类型
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	StkOutFrameDefEnum				GetDefineType();

	/****************************************************************************
	 *	@brief		设置外框的定义类型
	 *	@param[in]	i_eType				外框类型
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS						SetDefineType(StkOutFrameDefEnum i_eType);

	/****************************************************************************
	 *	@brief		获取外框线的数量
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		外框线的数量
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_UINT32						GetCurvesNum();

	/****************************************************************************
	 *	@brief		根据索引获取外框线的数量
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		线指针
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_CurveEntity*				GetCurveByIndex(STK_UINT32 i_nuIndex);
	
	/*****************************************************************************
	 *	@brief		写入外框数据
	 *	@param[in]	i_head			数据段头指针
	 *	@param[in]	i_stkFileP		文件对象指针
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		写入成功
	 *	@retval		其它			写入失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS						WriteFrame(StkSegmentHead *i_head, Stk_File *i_stkFileP);
};

#endif
