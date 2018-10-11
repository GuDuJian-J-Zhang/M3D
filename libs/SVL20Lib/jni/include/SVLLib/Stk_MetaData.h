// All Rights Reserved. Copyright (C) 2016 Hoteamsoft. 
/**************************************************************************
*	@class	Stk_MetaData
*			�Զ���������
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
			enum { _typeID = _SVLLIB_TYPE_METADATA };	//!< ���	
		public:
			Stk_MetaData();
			virtual ~Stk_MetaData();
		public:
			void Init(Stk_MetaDataDocument* pDoc, unsigned int parentID/*, bool bForWrite = true*/);
		public:
			/****************************************************************************
			*	@brief		��ȡ�Զ�������ID
			*	@param[in]	��
			*	@param[out]	m_uiID
			*	@retval		STK_SUCCESS			���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_UINT32             GetID();

			/****************************************************************************
			*	@brief		�����Զ�����������
			*	@param[in]	i_wstrTitle			�Զ�����������
			*	@param[out]	��
			*	@retval		STK_SUCCESS			���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS                SetName(wstring i_wstrName);

			/****************************************************************************
			*	@brief		��ȡ�Զ�����������
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_wstrName		�Զ�����������
			*	@note		��
			*	@attention	��
			****************************************************************************/
			wstring						GetName();
			
			/****************************************************************************
			*	@brief		�����Զ�������ֵ������
			*	@param[in]	i_eValueType	�Զ�������ֵ������
			*	@param[out]	��
			*	@retval		STK_SUCCESS		���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS					SetType(StkMetaDataValueTypeEnum i_eValueType);

			/****************************************************************************
			*	@brief		��ȡ�Զ�������ֵ������
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_eValueType		�Զ�������ֵ������
			*	@note		��
			*	@attention	��
			****************************************************************************/
			StkMetaDataValueTypeEnum	GetType();

			/****************************************************************************
			*	@brief		�����Զ�������ֵ
			*	@param[in]	i_wstrValue		�Զ�������ֵ
			*	@param[out]	��
			*	@retval		STK_SUCCESS		���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS					SetValue(wstring i_wstrValue);

			/****************************************************************************
			*	@brief		��ȡ�Զ�������ֵ
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_wstrValue		�Զ�������ֵ
			*	@note		��
			*	@attention	��
			****************************************************************************/
			wstring						GetValue();

			/****************************************************************************
			*	@brief		�����Զ������Ե�λ
			*	@param[in]	i_wstrUnit		�Զ������Ե�λ
			*	@param[out]	��
			*	@retval		STK_SUCCESS		���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS					SetUnit(wstring i_wstrUnit);

			/****************************************************************************
			*	@brief		��ȡ�Զ������Ե�λ
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_wstrUnit		�Զ������Ե�λ
			*	@note		��
			*	@attention	��
			****************************************************************************/
			wstring						GetUnit();
		};
	}
}

#endif