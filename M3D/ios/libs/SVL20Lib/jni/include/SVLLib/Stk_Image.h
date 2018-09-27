#ifndef _SVLLIB_IMAGE_H_
#define _SVLLIB_IMAGE_H_


// All Rights Reserved. Copyright (C) 2017 HOTEAMSOFT, Ltd
/****************************************************************************
*	@file	Stk_Image.h
*
*	@brief	ͼ����
*
*	@par	��ʷ:
*		2017/06/10	������qinyp@HOTEAMSOFT
****************************************************************************/

#pragma once
#include "Stk_Object.h"
namespace HoteamSoft {
	namespace SVLLib {
		class STK_TOOLKIT_EXPORT Stk_Image : public Stk_Object
		{
			STK_DECLARE_CLASS(Stk_Image)
		public:
			enum {
				_typeID = _SVLLIB_TYPE_IMAGE
			};	//!< ���
			Stk_Image();
			virtual ~Stk_Image();
		public:
			/****************************************************************************
			*	@brief		����ͼƬ����
			*	@param[in]	i_eType				ͼƬ����
			*	@param[out]	��
			*	@retval		STK_SUCCESS			���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS				SetType(StkPictureTypeEnum i_eType);

			/****************************************************************************
			*	@brief		��ȡͼƬ����
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		ͼƬ����
			*	@note		��
			*	@attention	��
			****************************************************************************/
			StkPictureTypeEnum		GetType();

			/****************************************************************************
			*	@brief		����ͼƬλ��
			*	@param[in]	i_pntLocation		ͼƬλ��
			*	@param[out]	��
			*	@retval		STK_SUCCESS			���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS				SetLocation(STK_PNT32 i_pntLocation);

			/****************************************************************************
			*	@brief		��ȡͼƬλ��
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		ͼƬλ��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_PNT32				GetLocation();

			/****************************************************************************
			*	@brief		����ͼƬ����ͼ��ʾ�Ŀ��
			*	@param[in]	i_fDisplayWidth		ͼƬ����ͼ��ʾ�Ŀ��
			*	@param[out]	��
			*	@retval		STK_SUCCESS			���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS				SetDisplayWidth(STK_FLOAT32 i_fDisplayWidth);

			/****************************************************************************
			*	@brief		��ȡͼƬ����ͼ��ʾ�Ŀ��
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		ͼƬ����ͼ��ʾ�Ŀ��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_FLOAT32				GetDisplayWidth();

			/****************************************************************************
			*	@brief		����ͼƬ����ͼ��ʾ�ĸ߶�
			*	@param[in]	i_fDisplayHeight	ͼƬ����ͼ��ʾ�ĸ߶�
			*	@param[out]	��
			*	@retval		STK_SUCCESS			���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS				SetDisplayHeight(STK_FLOAT32 i_fDisplayHeight);

			/****************************************************************************
			*	@brief		��ȡͼƬ����ͼ��ʾ�ĸ߶�
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		ͼƬ����ͼ��ʾ�ĸ߶�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_FLOAT32				GetDisplayHeight();

			/****************************************************************************
			*	@brief		����ͼƬʵ�ʵĿ��
			*	@param[in]	i_fActualWidth		ͼƬʵ�ʵĿ��
			*	@param[out]	��
			*	@retval		STK_SUCCESS			���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS				SetActualWidth(STK_FLOAT32 i_fActualWidth);

			/****************************************************************************
			*	@brief		��ȡͼƬʵ�ʵĿ��
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		ͼƬʵ�ʵĿ��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_FLOAT32				GetActualWidth();

			/****************************************************************************
			*	@brief		��ȡͼƬʵ�ʵĸ߶�
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		ͼƬ����ͼ��ʾ�ĸ߶�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_FLOAT32				GetActualHeight();

			/****************************************************************************
			*	@brief		����ͼƬʵ�ʵĸ߶�
			*	@param[in]	i_fActualHeight		ͼƬʵ�ʵĸ߶�
			*	@param[out]	��
			*	@retval		STK_SUCCESS			���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS				SetActualHeight(STK_FLOAT32 i_fActualHeight);

			/****************************************************************************
			*	@brief		����ͼƬ���淽ʽ
			*	@param[in]	i_eSaveStyle		ͼƬ���淽ʽ��SVL�ⲿ/�ڲ�
			*	@param[out]	��
			*	@retval		STK_SUCCESS			���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS				SetPictureSaveStyle(StkPicSaveStyleEnum i_eSaveStyle);

			/****************************************************************************
			*	@brief		��ȡͼƬ���淽ʽ
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		ͼƬ���淽ʽ��SVL�ⲿ/�ڲ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			StkPicSaveStyleEnum		GetPictureSaveStyle();

			/****************************************************************************
			*	@brief		����ͼƬ�ı���·��
			*	@param[in]	i_wstrSavePath		ͼƬ�ı���·��
			*	@param[out]	��
			*	@retval		STK_SUCCESS			���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS				SetPictureSavePath(wstring i_wstrSavePath);

			/****************************************************************************
			*	@brief		��ȡͼƬ�ı���·��
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		ͼƬ�ı���·��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			wstring					GetPictureSavePath();

			/****************************************************************************
			*	@brief		����ͼƬλͼNC
			*	@param[in]	i_nNC				λͼNC
			*	@param[out]	��
			*	@retval		STK_SUCCESS			���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS				SetNC(STK_INT32 i_nNC);

			/****************************************************************************
			*	@brief		��ȡͼƬλͼNC
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		λͼNC
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_INT32				GetNC();

			/****************************************************************************
			*	@brief		����ͼƬ��������
			*	@param[in]	i_nudataSize		ͼƬ������������
			*	@param[in]	i_chDataP			ͼƬ��������
			*	@param[out]	��
			*	@retval		STK_SUCCESS			���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS				SetData(STK_UINT32 i_nudataSize, STK_CHAR* i_chDataP);

			/****************************************************************************
			*	@brief		��ȡͼƬ��������
			*	@param[in]	��
			*	@param[out]	o_nudataSize		ͼƬ������������
			*	@param[out]	o_chDataP			ͼƬ��������
			*	@retval		STK_SUCCESS			���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS				GetData(STK_UINT32& o_nudataSize, STK_CHAR*& o_chDataP);

			/****************************************************************************
			*	@brief		����X������������
			*	@param[in]	i_fScaleX			X������������
			*	@param[out]	��
			*	@retval		STK_SUCCESS			���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS				SetScaleX(STK_FLOAT32 i_fScaleX);

			/****************************************************************************
			*	@brief		��ȡX������������
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		X������������
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_FLOAT32				GetScaleX();

			/****************************************************************************
			*	@brief		����Y������������
			*	@param[in]	i_fScaleY			Y������������
			*	@param[out]	��
			*	@retval		STK_SUCCESS			���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS				SetScaleY(STK_FLOAT32 i_fScaleY);

			/****************************************************************************
			*	@brief		��ȡY������������
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		Y������������
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_FLOAT32				GetScaleY();

			/****************************************************************************
			*	@brief		д�ļ�
			*	@param[in]	i_DataP		����д��λ��
			*	@param[in]	i_pDoc		�ĵ�
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
