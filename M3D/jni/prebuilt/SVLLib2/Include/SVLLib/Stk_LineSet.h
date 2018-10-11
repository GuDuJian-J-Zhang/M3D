// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_LineSet.h
 *
 *	@brief	Stk_LineSet线集类
 *
 *	@par	历史:
 *		2017/07/14	创建。Likun@HOTEAMSOFT
****************************************************************************/

#ifndef _SVLLIB_SUBLINE_H_
#define _SVLLIB_SUBLINE_H_

#ifndef __MOBILE__
#pragma once
#endif

#include "Stk_Object.h"
#include "Stk_CurveEntity.h"

/**************************************************************************************
	 *	@class	Stk_LineSet
	 *	@brief	线集类
	 *
	 *	线集类。<br>
	 *	继承自Stk_Entity类。
***************************************************************************************/
namespace HoteamSoft {
	namespace SVLLib {
		class STK_TOOLKIT_EXPORT  Stk_LineSet : public Stk_Object
		{
			STK_DECLARE_CLASS(Stk_LineSet)
		protected:
			enum { _typeID = _SVLLIB_TYPE_LINESET };	//!< 类别
	
		public:
			/****************************************************************************
			 *	@brief		Stk_LineSet构造函数
			 *	@param[in]	i_stkProtoTypeP		ProtoType对象指针
			 *	@retval		无
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			Stk_LineSet();

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
			 *	@brief		获取ModelID
			 *	@param[in]	无
			 *	@param[out]	无
			 *	@retval		STK_ID
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			STK_ID						GetModelID();

			/****************************************************************************
			*	@brief		设置ModelID
			*	@param[in]	无
			*	@param[out]	无
			 *	@retval		STK_SUCCESS		设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS						SetModelID(STK_ID i_ID);

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
			STK_STATUS					AddLine(Stk_CurveEntityPtr i_stkLine);

			/****************************************************************************
			 *	@brief		获取线集中的线数据
			 *	@param[in]	无
			 *	@param[out]	无
			 *	@retval		m_vcLines			线集中的线数据
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			vector<Stk_CurveEntityPtr>&	GetAllLines();

			/****************************************************************************
			 *	@brief		从Json字符串读取线集数据
			 *	@param[in]	i_strJson			Json字符串
			 *	@param[out]	无
			 *	@retval		STK_SUCCESS			读取成功
			 *	@retval		STK_ERROR_MEMORY	内存申请失败
			 *	@note		无
			 *	@attention	
			 ****************************************************************************/
			STK_STATUS					FromJson(string i_strJson);

			/****************************************************************************
			 *	@brief		将线集数据转换为Json字符串
			 *	@param[in]	无
			 *	@param[out]	无
			 *	@retval		Json字符串
			 *	@note		无
			 *	@attention	
			 ****************************************************************************/
			string					ToJson();
		};
	}
}
#endif