// All Rights Reserved. Copyright (C) 2017 HOTEAMSOFT, Ltd
/****************************************************************************
*	@file	Stk_UserData.h
*
*	@brief	�û�������
*
*	@par	��ʷ:
*		2017/06/10	������qinyp@HOTEAMSOFT
****************************************************************************/

#ifndef _SVLLIB_USERDATA_H_
#define _SVLLIB_USERDATA_H_

#pragma once
#include "Stk_Object.h"
namespace HoteamSoft {
	namespace SVLLib {
		class STK_TOOLKIT_EXPORT Stk_UserData : public Stk_Object
		{
			STK_DECLARE_CLASS(Stk_UserData)
		public:
			Stk_UserData();
			virtual ~Stk_UserData();
		public:
			/****************************************************************************
			*	@brief		�����û�����
			*	@param[in]	i_wstrDataName	��������
			*	@param[in]	i_id			����ID
			*	@param[in]	i_dataP			������
			*	@param[in]	i_dataSize		����������
			*	@param[out]	i_wstrFileName	�����ļ���(���������׺)
			*	@param[out]	��
			*	@retval		STK_SUCCESS		�ɹ�
			*	@retval		����			ʧ��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS					SetUserData(wstring i_wstrDataName, STK_ID i_id, 
				STK_CHAR *i_dataP, STK_SIZE_T i_dataSize, wstring i_wstrFileName);

			/****************************************************************************
			*	@brief		��ȡ�û�����
			*	@param[in]	��
			*	@param[out]	o_wstrDataName	��������
			*	@param[out]	o_id			����ID
			*	@param[out]	o_dataP			������
			*	@param[out]	o_dataSize		����������
			*	@param[out]	o_wstrFileName	�����ļ���
			*	@retval		STK_SUCCESS		�ɹ�
			*	@retval		����			ʧ��
			*	@note		����o_dataP��Ϊ�û�������ڴ渱����ʹ��������free�ͷ��ڴ�
			*	@attention
			****************************************************************************/
			STK_STATUS					GetUserData(wstring &o_wstrDataName, STK_ID &o_id, 
				STK_CHAR* &o_dataP, STK_SIZE_T &o_dataSize, wstring &o_wstrFileName);

			/****************************************************************************
			*	@brief		д�ļ�
			*	@param[in]	i_DataP		����д��λ��
			*	@param[out]	��
			*	@retval		STK_SUCCESS	д��ɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS					ToJson(void* i_DataP, void* pDoc);

			/****************************************************************************
			*	@brief		���ļ�
			*	@param[in]	i_DataP		���ݶ���λ��
			*	@param[out]	��
			*	@retval		STK_SUCCESS	����ɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS					FromJson(void* i_DataP);
		};
	}
}
#endif
