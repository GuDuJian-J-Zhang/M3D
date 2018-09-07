// All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
/****************************************************************************
*	@file	Stk_PlaneEntity.h
*
*	@brief	��Ҫ����
*
*	@par	��ʷ:
*
****************************************************************************/
#ifndef _SVLLIB_PLANEENTITY_H_
#define _SVLLIB_PLANEENTITY_H_
#pragma once
#include "Stk_Object.h"
namespace HoteamSoft {
	namespace SVLLib {
		class STK_TOOLKIT_EXPORT Stk_PlaneEntity : public Stk_Object
		{
			STK_DECLARE_CLASS(Stk_PlaneEntity)
		protected:
			enum { _typeID = _SVLLIB_TYPE_PLANE };	//!< ���
		public:
			Stk_PlaneEntity();
			virtual ~Stk_PlaneEntity();
		protected:
			StkPlaneTypeEnum	m_ePlaneType;	     //!< ������
		public:
			/****************************************************************************
			*	@brief		��ȡ��������
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_nuType		��������
			*	@note		��
			*	@attention	��
			****************************************************************************/
			StkPlaneTypeEnum	GetType();

			/****************************************************************************
			*	@brief		��ȡ������ɫ
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_rgbaSurface	������ɫ
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_RGBA32			GetColor();

			/****************************************************************************
			*	@brief		����������ɫ
			*	@param[in]	i_rgbaSurface	������ɫ
			*	@param[out]	��
			*	@retval		STK_SUCCESS		���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS			SetColor(const STK_RGBA32& i_rgbaSurface);

			/****************************************************************************
			*	@brief		д�ļ�
			*	@param[in]	i_DataP		����д��λ��
			*	@param[in]	i_JsonDocP	Json�ĵ�����
			*	@param[out]	��
			*	@retval		STK_SUCCESS	д��ɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			virtual    STK_STATUS					ToJson(void* i_DataP, void* i_JsonDocP);

			/****************************************************************************
			*	@brief		���ļ�
			*	@param[in]	i_DataP		���ݶ���λ��
			*	@param[in]	i_JsonDocP	Json�ĵ�����
			*	@param[out]	��
			*	@retval		STK_SUCCESS	����ɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			virtual     STK_STATUS					FromJson(void* i_DataP, void* i_JsonDocP);
		};

		/**************************************************************************************
		*	@class	Stk_ClipPlane
		*	@brief	Stk_ClipPlane ��
		*
		*	�̳���Stk_SurfaceEntity�ࡣ
		***************************************************************************************/
		class STK_TOOLKIT_EXPORT Stk_ClipPlane : public Stk_PlaneEntity
		{
			STK_DECLARE_CLASS(Stk_ClipPlane)
		protected:
			enum { _typeID = _SVLLIB_TYPE_PLANE_CLIP };	//!< ���
		public:
			Stk_ClipPlane();
			virtual ~Stk_ClipPlane();
		public:
			/****************************************************************************
			*	@brief		��ȡ�����ĳ��λ��
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_pntOrigin		������ĳ��λ��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_PNT32			GetOrigin();

			/****************************************************************************
			*	@brief		���������ĳ��λ��
			*	@param[in]	i_pntOrigin		������ĳ��λ��
			*	@param[out]	��
			*	@retval		��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			void				SetOrigin(const STK_PNT32 &i_pntOrigin);

			/****************************************************************************
			*	@brief		��ȡ���淨����
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_dirNormal		���淨����
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_DIR32			GetNormal();

			/****************************************************************************
			*	@brief		�������淨����
			*	@param[in]	i_dirNormal		���淨����
			*	@param[out]	��
			*	@retval		��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			void				SetNormal(const STK_DIR32 &i_dirNormal);

			/****************************************************************************
			*	@brief		��ȡ����UV����
			*	@param[in]	o_dirU		����U����
			*	@param[in]	o_dirV		����V����
			*	@param[out]	��
			*	@retval		STK_SUCCESS	��ȡ�ɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS			GetUVDir(STK_DIR32 &o_dirU, STK_DIR32 &o_dirV);

			/****************************************************************************
			*	@brief		��������UV����
			*	@param[in]	i_dirU		����U����
			*	@param[in]	i_dirV		����V����
			*	@param[out]	��
			*	@retval		��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			void				SetUVDir(const STK_DIR32 &i_dirU, const STK_DIR32 &i_dirV);

			/****************************************************************************
			*	@brief		��ȡ�������:ԭ�������������
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		���
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_FLOAT32			GetDistance();

			/****************************************************************************
			*	@brief		�����������:ԭ�������������
			*	@param[in]	i_fDistance		���
			*	@param[out]	��
			*	@retval		STK_SUCCESS		���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS			SetDistance(STK_FLOAT32 i_fDistance);

			/****************************************************************************
			*	@brief		��ȡ�����߶���
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		�����߶���
			*	@note		��
			*	@attention	��
			****************************************************************************/
			Stk_ClippingCurvePtr	GetClippingCurve();

			/****************************************************************************
			*	@brief		���������߶���
			*	@param[in]	i_stkClippingCurveP	�����߶���
			*	@param[out]	��
			*	@retval		STK_SUCCESS			���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS			SetClippingCurve(Stk_ClippingCurvePtr i_stkClippingCurveP);

			/****************************************************************************
			*	@brief		д�ļ�
			*	@param[in]	i_DataP		����д��λ��
			*	@param[in]	i_JsonDocP	Json�ĵ�����
			*	@param[out]	��
			*	@retval		STK_SUCCESS	д��ɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS					ToJson(void* i_DataP, void* i_JsonDocP);

			/****************************************************************************
			*	@brief		���ļ�
			*	@param[in]	i_DataP		���ݶ���λ��
			*	@param[in]	i_JsonDocP	Json�ĵ�����
			*	@param[out]	��
			*	@retval		STK_SUCCESS	����ɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS					FromJson(void* i_DataP, void* i_JsonDocP);
		};

		/**************************************************************************************
		*	@class	Stk_ClippingCurve
		*	@brief	������Ӱ����
		*
		*	�̳���Stk_Entity�ࡣ
		***************************************************************************************/
		class STK_TOOLKIT_EXPORT Stk_ClippingCurve : public Stk_Object
		{
			STK_DECLARE_CLASS(Stk_ClippingCurve)
		protected:
			enum {      _typeID = _SVLLIB_TYPE_CLIPPINGCURVE     };	//!< ���
		public:
			Stk_ClippingCurve();
			~Stk_ClippingCurve();
		public:
			/****************************************************************************
			*	@brief		��������������״̬
			*	@param[in]	i_bDisplay		����״̬
			*	@param[out]	��
			*	@retval		STK_SUCCESS		���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS		SetDisplay(STK_BOOL i_bDisplay);

			/****************************************************************************
			*	@brief		��ȡ����������״̬
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		����״̬
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_BOOL		GetDisplay();

			/****************************************************************************
			*	@brief		���������߻��Ʒ�ʽ
			*	@param[in]	i_eDrawStyle	���Ʒ�ʽ
			*	@param[out]	��
			*	@retval		STK_SUCCESS		���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS		SetDrawStyle(StkClippingCurveDrawStyleEnum i_eDrawStyle);

			/****************************************************************************
			*	@brief		��ȡ�����߻��Ʒ�ʽ
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		���Ʒ�ʽ
			*	@note		��
			*	@attention	��
			****************************************************************************/
			StkClippingCurveDrawStyleEnum	GetDrawStyle();

			/****************************************************************************
			*	@brief		���������߻�����ʾ����
			*	@param[in]	i_eDrawType		�����߻�����ʾ����
			*	@param[out]	��
			*	@retval		STK_SUCCESS		���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS		SetDrawType(StkClippingCurveDrawTypeEnum i_eDrawType);

			/****************************************************************************
			*	@brief		��ȡ�����߻�����ʾ����
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		�����߻�����ʾ����
			*	@note		��
			*	@attention	��
			****************************************************************************/
			StkClippingCurveDrawTypeEnum	GetDrawType();

			/****************************************************************************
			*	@brief		���������߽Ƕ�
			*	@param[in]	i_fAngle		�����߽Ƕ�
			*	@param[out]	��
			*	@retval		STK_SUCCESS		���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS		SetAngle(STK_FLOAT32 i_fAngle);

			/****************************************************************************
			*	@brief		��ȡ�����߽Ƕ�
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		�����߽Ƕ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_FLOAT32		GetAngle();

			/****************************************************************************
			*	@brief		������������ʾ����
			*	@param[in]	i_fRatio		��������ʾ����
			*	@param[out]	��
			*	@retval		STK_SUCCESS		���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS		SetRatio(STK_FLOAT32 i_fRatio);

			/****************************************************************************
			*	@brief		��ȡ��������ʾ����
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		��������ʾ����
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_FLOAT32		GetRatio();

			/****************************************************************************
			*	@brief		������������ʾ��ɫ
			*	@param[in]	i_rgbaCurve		��������ʾ��ɫ
			*	@param[out]	��
			*	@retval		STK_SUCCESS		���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS		SetColor(STK_RGBA32 i_rgbaCurve);

			/****************************************************************************
			*	@brief		��ȡ��������ʾ��ɫ
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		��������ʾ��ɫ
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_RGBA32		GetColor();

			/****************************************************************************
			*	@brief		д�ļ�
			*	@param[in]	i_DataP		����д��λ��
			*	@param[in]	i_JsonDocP	Json�ĵ�����
			*	@param[out]	��
			*	@retval		STK_SUCCESS	д��ɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS					ToJson(void* i_DataP, void* i_JsonDocP);

			/****************************************************************************
			*	@brief		���ļ�
			*	@param[in]	i_DataP		���ݶ���λ��
			*	@param[in]	i_JsonDocP	Json�ĵ�����
			*	@param[out]	��
			*	@retval		STK_SUCCESS	����ɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS					FromJson(void* i_DataP, void* i_JsonDocP);
		};
	}
}

#endif // _SVLLIB_PLANEENTITY_H_