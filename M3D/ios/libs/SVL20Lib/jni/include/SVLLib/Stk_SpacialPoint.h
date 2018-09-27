// All Rights Reserved. Copyright (C) 2017 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_SpacialPointEntity.h
 *
 *	@brief	Stk_SpacialPoint空间点类
 *
 *	@par	历史:
 *		2017/07/18	创建。likun@HOTEAMSOFT
****************************************************************************/

#ifndef _SVLLIB_SPACIALPOINT_ENTITY_H_
#define _SVLLIB_SPACIALPOINT_ENTITY_H_

#pragma once
#include "Stk_Object.h"

namespace HoteamSoft {
	namespace SVLLib {
		/**************************************************************************************
			 *	@class	Stk_SpacialPointEntity
			 *	@brief	空间点类
			 *
			 *	空间点类。<br>
			 *	继承自Stk_Entity类。
		***************************************************************************************/

		class STK_TOOLKIT_EXPORT  Stk_SpacialPointEntity :
			public Stk_Object
		{
			STK_DECLARE_CLASS(Stk_SpacialPointEntity)
		protected:
			enum { _typeID = _SVLLIB_TYPE_SPACIALPOINT };	//!< 类别
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