// All Rights Reserved. Copyright (C) 2016 Hoteamsoft. 
/**************************************************************************
*	@class	Stk_MetaData
*			自定义属性类
*
**************************************************************************/
#ifndef _SVLLIB_METADATA_H_
#define _SVLLIB_METADATA_H_

#pragma once
#include "Stk_Object.h"


namespace HoteamSoft {
	namespace SVLLib {
		class Stk_MetaDataDocument;
		class STK_TOOLKIT_EXPORT Stk_MetaData : public Stk_Object
		{
		STK_DECLARE_CLASS(Stk_MetaData)
		protected:
			enum { _typeID = _SVLLIB_TYPE_METADATA };	//!< 类别	
		public:
			Stk_MetaData();
			virtual ~Stk_MetaData();
		public:
			void Init(Stk_MetaDataDocument* pDoc, unsigned int parentID/*, bool bForWrite = true*/);
		public:
			/****************************************************************************
			*	@brief		获取自定义属性ID
			*	@param[in]	无
			*	@param[out]	m_uiID
			*	@retval		STK_SUCCESS			设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_UINT32             GetID();

			/****************************************************************************
			*	@brief		设置自定义属性名称
			*	@param[in]	i_wstrTitle			自定义属性名称
			*	@param[out]	无
			*	@retval		STK_SUCCESS			设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS                SetName(wstring i_wstrName);

			/****************************************************************************
			*	@brief		获取自定义属性名称
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_wstrName		自定义属性名称
			*	@note		无
			*	@attention	无
			****************************************************************************/
			wstring						GetName();
			
			/****************************************************************************
			*	@brief		设置自定义属性值的类型
			*	@param[in]	i_eValueType	自定义属性值的类型
			*	@param[out]	无
			*	@retval		STK_SUCCESS		设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS					SetType(StkMetaDataValueTypeEnum i_eValueType);

			/****************************************************************************
			*	@brief		获取自定义属性值的类型
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_eValueType		自定义属性值的类型
			*	@note		无
			*	@attention	无
			****************************************************************************/
			StkMetaDataValueTypeEnum	GetType();

			/****************************************************************************
			*	@brief		设置自定义属性值
			*	@param[in]	i_wstrValue		自定义属性值
			*	@param[out]	无
			*	@retval		STK_SUCCESS		设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS					SetValue(wstring i_wstrValue);

			/****************************************************************************
			*	@brief		获取自定义属性值
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_wstrValue		自定义属性值
			*	@note		无
			*	@attention	无
			****************************************************************************/
			wstring						GetValue();

			/****************************************************************************
			*	@brief		设置自定义属性单位
			*	@param[in]	i_wstrUnit		自定义属性单位
			*	@param[out]	无
			*	@retval		STK_SUCCESS		设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS					SetUnit(wstring i_wstrUnit);

			/****************************************************************************
			*	@brief		获取自定义属性单位
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		m_wstrUnit		自定义属性单位
			*	@note		无
			*	@attention	无
			****************************************************************************/
			wstring						GetUnit();
		};
	}
}

#endif