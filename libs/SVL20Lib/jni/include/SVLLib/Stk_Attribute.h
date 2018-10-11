#pragma once
// All Rights Reserved. Copyright (C) 2017 HOTEAMSOFT, Ltd
/****************************************************************************
*	@file	Stk_Attribute.h
*
*	@brief	������
*
*	@par	��ʷ:
*		2017/07/03	������qinyp@HOTEAMSOFT
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
			enum { _typeID = _SVLLIB_TYPE_ATTRIBUTE };	//!< ���
		public:
			Stk_Attribute();
			virtual ~Stk_Attribute();
		public:
			/****************************************************************************
			*	@brief		��ȡ����
			*	@param[in]	void
			*	@param[out]	void
			*	@retval		����
			*	@note		��
			*	@attention	��
			****************************************************************************/
			virtual wstring  GetName();

			/****************************************************************************
			*	@brief		��������
			*	@param[in]	i_wstrName ����
			*	@param[out]	void
			*	@retval		void
			*	@note		��
			*	@attention	��
			****************************************************************************/
			virtual void SetName(const wstring& i_wstrName);

			/****************************************************************************
			*	@brief		��ȡ������������
			*	@param[in]	void
			*	@param[out]	void
			*	@retval		��������
			*	@note		��
			*	@attention	��
			****************************************************************************/
			//virtual StkAttributeOwnerType GetAttributeOwnerType();

			/****************************************************************************
			*	@brief		��ȡ������������
			*	@param[in]	i_eAttributeOwnerType ��������
			*	@param[out]	void
			*	@retval		��������
			*	@note		��
			*	@attention	��
			****************************************************************************/
			//virtual STK_STATUS SetAttributeOwnerType(const StkAttributeOwnerType& i_eAttributeOwnerType);

			/****************************************************************************
			*	@brief		д�ļ�
			*	@param[in]	i_DataP		����д��λ��
			*	@param[in]	i_JsonDocP	Json�ĵ�����
			*	@param[out]	��
			*	@retval		STK_SUCCESS	д��ɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			virtual	STK_STATUS					ToJson(void* i_DataP, void* i_JsonDocP);

			/****************************************************************************
			*	@brief		���ļ�
			*	@param[in]	i_DataP		���ݶ���λ��
			*	@param[in]	i_JsonDocP	Json�ĵ�����
			*	@param[out]	��
			*	@retval		STK_SUCCESS	����ɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			virtual	STK_STATUS					FromJson(void* i_DataP, void* i_JsonDocP);
		};
	}
}

#endif