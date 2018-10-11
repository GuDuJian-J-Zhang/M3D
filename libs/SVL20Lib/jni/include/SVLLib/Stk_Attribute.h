#pragma once
// All Rights Reserved. Copyright (C) 2017 HOTEAMSOFT, Ltd
/****************************************************************************
*	@file	Stk_Attribute.h
*
*	@brief	属性类
*
*	@par	历史:
*		2017/07/03	创建。qinyp@HOTEAMSOFT
****************************************************************************/

#ifndef _SVLLIB_ATTRIBUTE_H_
#define _SVLLIB_ATTRIBUTE_H_

#include "Stk_Object.h"

namespace HoteamSoft {
	namespace SVLLib {
		
		class STK_TOOLKIT_EXPORT Stk_Attribute : public Stk_Object
		{
			STK_DECLARE_CLASS(Stk_Attribute)
		protected:
			enum { _typeID = _SVLLIB_TYPE_ATTRIBUTE };	//!< 类别
		public:
			Stk_Attribute();
			virtual ~Stk_Attribute();
		public:
			/****************************************************************************
			*	@brief		获取名称
			*	@param[in]	void
			*	@param[out]	void
			*	@retval		名称
			*	@note		无
			*	@attention	无
			****************************************************************************/
			virtual wstring  GetName();

			/****************************************************************************
			*	@brief		设置名称
			*	@param[in]	i_wstrName 名称
			*	@param[out]	void
			*	@retval		void
			*	@note		无
			*	@attention	无
			****************************************************************************/
			virtual void SetName(const wstring& i_wstrName);

			/****************************************************************************
			*	@brief		获取属性属主类型
			*	@param[in]	void
			*	@param[out]	void
			*	@retval		属主类型
			*	@note		无
			*	@attention	无
			****************************************************************************/
			//virtual StkAttributeOwnerType GetAttributeOwnerType();

			/****************************************************************************
			*	@brief		获取属性属主类型
			*	@param[in]	i_eAttributeOwnerType 属主类型
			*	@param[out]	void
			*	@retval		属主类型
			*	@note		无
			*	@attention	无
			****************************************************************************/
			//virtual STK_STATUS SetAttributeOwnerType(const StkAttributeOwnerType& i_eAttributeOwnerType);

			/****************************************************************************
			*	@brief		写文件
			*	@param[in]	i_DataP		数据写入位置
			*	@param[in]	i_JsonDocP	Json文档对象
			*	@param[out]	无
			*	@retval		STK_SUCCESS	写入成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			virtual	STK_STATUS					ToJson(void* i_DataP, void* i_JsonDocP);

			/****************************************************************************
			*	@brief		读文件
			*	@param[in]	i_DataP		数据读入位置
			*	@param[in]	i_JsonDocP	Json文档对象
			*	@param[out]	无
			*	@retval		STK_SUCCESS	读入成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			virtual	STK_STATUS					FromJson(void* i_DataP, void* i_JsonDocP);
		};
	}
}

#endif