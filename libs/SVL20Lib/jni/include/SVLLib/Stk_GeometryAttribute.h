// All Rights Reserved. Copyright (C) 2017 Hoteamsoft. 
/**************************************************************************
*	@class	Stk_GeometryAttribute
*	@brief	����������
*	@par ��ʷ��¼
*				2017/07/14 ������qinyp(Hoteamsoft)
**************************************************************************/
#pragma once

#include "Stk_Object.h"

#ifndef _SVLLIB_GEOMETRYATTRIBUTE_H_
#define _SVLLIB_GEOMETRYATTRIBUTE_H_
namespace HoteamSoft {
	namespace SVLLib {
		class STK_TOOLKIT_EXPORT Stk_GeometryAttribute : public Stk_Object
		{
			STK_DECLARE_CLASS(Stk_GeometryAttribute)
		public:
			Stk_GeometryAttribute();
			virtual ~Stk_GeometryAttribute();
		protected:
			enum {
				_typeID = _SVLLIB_TYPE_GEOMETRYATTRIBUTE
			};
		public:
			/****************************************************************************
			*	@brief		��ȡ����Ҫ�ص�����
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_eGeoAttrType	����Ҫ�ص�����
			*	@note		��
			*	@attention	��
			****************************************************************************/
			virtual StkGeoAttrTypeEnum	GetGeoAttrType();

			/****************************************************************************
			*	@brief		��������Ҫ�ص�����
			*	@param[in]	i_eAttrType		����Ҫ�ص�����
			*	@param[out]	��
			*	@retval		STK_SUCCESS		���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			virtual STK_STATUS			SetGeoAttrType(StkGeoAttrTypeEnum i_eAttrType);

			/****************************************************************************
			*	@brief		д�ļ�
			*	@param[in]	i_DataP		����д��λ��
			*	@param[in]	i_JsonDocP	Json�ĵ�����
			*	@param[out]	��
			*	@retval		STK_SUCCESS	д��ɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			virtual STK_STATUS					ToJson(void* i_DataP, void* i_JsonDocP);

			/****************************************************************************
			*	@brief		���ļ�
			*	@param[in]	i_DataP		���ݶ���λ��
			*	@param[in]	i_JsonDocP	Json�ĵ�����
			*	@param[out]	��
			*	@retval		STK_SUCCESS	����ɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			virtual STK_STATUS					FromJson(void* i_DataP, void* i_JsonDocP);
		};

		/**************************************************************************************
		*	@class	Stk_PlaneFaceAttribute
		*	@brief	ƽ��������
		*
		*	��¼ƽ��Ļ���������Ϣ��<br>
		*	�̳���Stk_GeometryAttribute �ࡣ
		***************************************************************************************/
		class STK_TOOLKIT_EXPORT Stk_PlaneFaceAttribute : public Stk_GeometryAttribute
		{
			STK_DECLARE_CLASS(Stk_PlaneFaceAttribute)
		protected:
			enum {
				_typeID = _SVLLIB_TYPE_PLANEFACEATTRIBUTE
			};
		public:
			Stk_PlaneFaceAttribute();
			virtual ~Stk_PlaneFaceAttribute();
		public:
			/****************************************************************************
			*	@brief		д�ļ�
			*	@param[in]	i_DataP		����д��λ��
			*	@param[in]	i_JsonDocP	Json�ĵ�����
			*	@param[out]	��
			*	@retval		STK_SUCCESS	д��ɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			virtual STK_STATUS					ToJson(void* i_DataP, void* i_JsonDocP);

			/****************************************************************************
			*	@brief		���ļ�
			*	@param[in]	i_DataP		���ݶ���λ��
			*	@param[in]	i_JsonDocP	Json�ĵ�����
			*	@param[out]	��
			*	@retval		STK_SUCCESS	����ɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			virtual STK_STATUS					FromJson(void* i_DataP, void* i_JsonDocP);

			/****************************************************************************
			*	@brief		����ƽ��ķ���
			*	@param[in]	i_dirNormal		ƽ�淨��
			*	@param[out]	��
			*	@retval		STK_SUCCESS		����ƽ�淨��ɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS			SetNormal(const STK_DIR32 &i_dirNormal);

			/****************************************************************************
			*	@brief		��ȡƽ��ķ���
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_dirNormal		ƽ�淨��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_DIR32			GetNormal();

			/****************************************************************************
			*	@brief		����ƽ���ԭ��
			*	@param[in]	i_pntOrigin		ƽ��ԭ��
			*	@param[out]	��
			*	@retval		STK_SUCCESS		����ƽ��ԭ��ɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS			SetOrigin(const STK_PNT32 &i_pntOrigin);

			/****************************************************************************
			*	@brief		��ȡƽ���ԭ��
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_pntOrigin		ƽ��ԭ��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_PNT32			GetOrigin();
		};

		/**************************************************************************************
		*	@class	Stk_RevolutionFaceAttribute
		*	@brief	��ת�����Ի���
		*
		*	��¼��ת��Ļ���������Ϣ��<br>
		*	�̳���Stk_GeometryAttribute �ࡣ
		***************************************************************************************/
		class STK_TOOLKIT_EXPORT Stk_RevolutionFaceAttribute : public Stk_GeometryAttribute
		{
			STK_DECLARE_CLASS(Stk_RevolutionFaceAttribute)
		public:
			Stk_RevolutionFaceAttribute();
			virtual ~Stk_RevolutionFaceAttribute();
		protected:
			enum {
				_typeID = _SVLLIB_TYPE_REVOLUTIONATTRIBUTE
			};
		public:
			/****************************************************************************
			*	@brief		������ת��뾶
			*	@param[in]	i_dRadius		��ת��뾶
			*	@param[out]	��
			*	@retval		STK_SUCCESS		������ת��뾶�ɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS			SetOriginRadius(STK_DOUBLE64 i_dRadius);

			/****************************************************************************
			*	@brief		��ȡ��ת��뾶
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_dRadius		��ת��뾶
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_DOUBLE64		GetOriginRadius();

			/****************************************************************************
			*	@brief		������ת����ת��
			*	@param[in]	i_dirRevoAxis		��ת����ת�᷽������
			*	@param[out]	��
			*	@retval		STK_SUCCESS			������ת����ת��ɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS			SetRevoAxis(const STK_DIR32 &i_dirRevoAxis);

			/****************************************************************************
			*	@brief		��ȡ��ת����ת�᷽������
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_dirRevoAxis		��ת����ת�᷽������
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_DIR32			GetRevoAxis();

			/****************************************************************************
			*	@brief		������ת�����ת��ԭ��
			*	@param[in]	i_pntAxisOrigin		��ת�����ת��ԭ��
			*	@param[out]	��
			*	@retval		STK_SUCCESS			������ת���ԭ��ɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS			SetAxisOrigin(const STK_PNT32 &i_pntAxisOrigin);

			/****************************************************************************
			*	@brief		��ȡ��ת�����ת��ԭ��
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_pntAxisOrigin			��ת���ԭ��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_PNT32			GetAxisOrigin();

			/****************************************************************************
			*	@brief		д�ļ�
			*	@param[in]	i_DataP		����д��λ��
			*	@param[in]	i_JsonDocP	Json�ĵ�����
			*	@param[out]	��
			*	@retval		STK_SUCCESS	д��ɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			virtual STK_STATUS					ToJson(void* i_DataP, void* i_JsonDocP);

			/****************************************************************************
			*	@brief		���ļ�
			*	@param[in]	i_DataP		���ݶ���λ��
			*	@param[in]	i_JsonDocP	Json�ĵ�����
			*	@param[out]	��
			*	@retval		STK_SUCCESS	����ɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			virtual STK_STATUS					FromJson(void* i_DataP, void* i_JsonDocP);

		};

		/**************************************************************************************
		*	@class	Stk_CylinderFaceAttribute
		*	@brief	Բ�������Ի���
		*
		*	��¼Բ����Ļ���������Ϣ��<br>
		*	�̳���Stk_GeometryAttribute �ࡣ
		***************************************************************************************/
		class STK_TOOLKIT_EXPORT Stk_CylinderFaceAttribute : public Stk_GeometryAttribute
		{
			STK_DECLARE_CLASS(Stk_CylinderFaceAttribute)
		public:
			Stk_CylinderFaceAttribute();
			virtual ~Stk_CylinderFaceAttribute();
		protected:
			enum {
				_typeID = _SVLLIB_TYPE_CYLINDERATTRIBUTE
			};
		public:
			/****************************************************************************
			*	@brief		����Բ����뾶
			*	@param[in]	i_dRadius		Բ����뾶
			*	@param[out]	��
			*	@retval		STK_SUCCESS		����Բ����뾶�ɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS			SetOriginRadius(STK_DOUBLE64 i_dRadius);

			/****************************************************************************
			*	@brief		��ȡԲ����뾶
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_dRadius		Բ����뾶
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_DOUBLE64		GetOriginRadius();

			/****************************************************************************
			*	@brief		����Բ������ת��
			*	@param[in]	i_dirRevoAxis		Բ������ת�᷽������
			*	@param[out]	��
			*	@retval		STK_SUCCESS			����Բ������ת��ɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS			SetRevoAxis(const STK_DIR32 &i_dirRevoAxis);

			/****************************************************************************
			*	@brief		��ȡԲ������ת�᷽������
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_dirRevoAxis		Բ������ת�᷽������
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_DIR32			GetRevoAxis();

			/****************************************************************************
			*	@brief		����Բ�������ת��ԭ��
			*	@param[in]	i_pntAxisOrigin		Բ�������ת��ԭ��
			*	@param[out]	��
			*	@retval		STK_SUCCESS			������ת���ԭ��ɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS			SetAxisOrigin(const STK_PNT32 &i_pntAxisOrigin);

			/****************************************************************************
			*	@brief		��ȡԲ�������ת��ԭ��
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_pntAxisOrigin			��ת���ԭ��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_PNT32			GetAxisOrigin();

			/****************************************************************************
			*	@brief		д�ļ�
			*	@param[in]	i_DataP		����д��λ��
			*	@param[in]	i_JsonDocP	Json�ĵ�����
			*	@param[out]	��
			*	@retval		STK_SUCCESS	д��ɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			virtual STK_STATUS					ToJson(void* i_DataP, void* i_JsonDocP);

			/****************************************************************************
			*	@brief		���ļ�
			*	@param[in]	i_DataP		���ݶ���λ��
			*	@param[in]	i_JsonDocP	Json�ĵ�����
			*	@param[out]	��
			*	@retval		STK_SUCCESS	����ɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			virtual STK_STATUS					FromJson(void* i_DataP, void* i_JsonDocP);

		};

		/**************************************************************************************
		*	@class	Stk_ConeFaceAttribute
		*	@brief	Բ׶�����Ի���
		*
		*	��¼Բ׶��Ļ���������Ϣ��<br>
		*	�̳���Stk_GeometryAttribute �ࡣ
		***************************************************************************************/
		class STK_TOOLKIT_EXPORT Stk_ConeFaceAttribute : public Stk_GeometryAttribute
		{
			STK_DECLARE_CLASS(Stk_ConeFaceAttribute)
		public:
			Stk_ConeFaceAttribute();
			virtual ~Stk_ConeFaceAttribute();
		protected:
			enum {
				_typeID = _SVLLIB_TYPE_CONEATTRIBUTE
			};
		public:
			/****************************************************************************
			*	@brief		����Բ׶��뾶
			*	@param[in]	i_dRadius		Բ׶��뾶
			*	@param[out]	��
			*	@retval		STK_SUCCESS		����Բ׶��뾶�ɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS			SetOriginRadius(STK_DOUBLE64 i_dRadius);

			/****************************************************************************
			*	@brief		��ȡԲ׶��뾶
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_dRadius		Բ׶��뾶
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_DOUBLE64		GetOriginRadius();

			/****************************************************************************
			*	@brief		����Բ׶����ת��
			*	@param[in]	i_dirRevoAxis		Բ׶����ת�᷽������
			*	@param[out]	��
			*	@retval		STK_SUCCESS			������ת����ת��ɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS			SetRevoAxis(const STK_DIR32 &i_dirRevoAxis);

			/****************************************************************************
			*	@brief		��ȡԲ׶����ת�᷽������
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_dirRevoAxis		Բ׶����ת�᷽������
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_DIR32			GetRevoAxis();

			/****************************************************************************
			*	@brief		����Բ׶�����ת��ԭ��
			*	@param[in]	i_pntAxisOrigin		Բ׶�����ת��ԭ��
			*	@param[out]	��
			*	@retval		STK_SUCCESS			������ת���ԭ��ɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS			SetAxisOrigin(const STK_PNT32 &i_pntAxisOrigin);

			/****************************************************************************
			*	@brief		��ȡԲ׶�����ת��ԭ��
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_pntAxisOrigin			��ת���ԭ��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_PNT32			GetAxisOrigin();

			/****************************************************************************
			*	@brief		д�ļ�
			*	@param[in]	i_DataP		����д��λ��
			*	@param[in]	i_JsonDocP	Json�ĵ�����
			*	@param[out]	��
			*	@retval		STK_SUCCESS	д��ɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			virtual STK_STATUS					ToJson(void* i_DataP, void* i_JsonDocP);

			/****************************************************************************
			*	@brief		���ļ�
			*	@param[in]	i_DataP		���ݶ���λ��
			*	@param[in]	i_JsonDocP	Json�ĵ�����
			*	@param[out]	��
			*	@retval		STK_SUCCESS	����ɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			virtual STK_STATUS					FromJson(void* i_DataP, void* i_JsonDocP);

		};

		/**************************************************************************************
		*	@class	Stk_SphereFaceAttribute
		*	@brief	�������Ի���
		*
		*	��¼����Ļ���������Ϣ��<br>
		*	�̳���Stk_GeometryAttribute �ࡣ
		***************************************************************************************/
		class STK_TOOLKIT_EXPORT Stk_SphereFaceAttribute : public Stk_GeometryAttribute
		{
			STK_DECLARE_CLASS(Stk_SphereFaceAttribute)
		public:
			Stk_SphereFaceAttribute();
			virtual ~Stk_SphereFaceAttribute();
		protected:
			enum {
				_typeID = _SVLLIB_TYPE_SPHEREATTRIBUTE
			};
		public:
			/****************************************************************************
			*	@brief		��������뾶
			*	@param[in]	i_dRadius		��ת��뾶
			*	@param[out]	��
			*	@retval		STK_SUCCESS		������ת��뾶�ɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS			SetOriginRadius(STK_DOUBLE64 i_dRadius);

			/****************************************************************************
			*	@brief		��ȡ����뾶
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_dRadius		��ת��뾶
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_DOUBLE64		GetOriginRadius();

			/****************************************************************************
			*	@brief		����������ת��
			*	@param[in]	i_dirRevoAxis		��ת����ת�᷽������
			*	@param[out]	��
			*	@retval		STK_SUCCESS			������ת����ת��ɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS			SetRevoAxis(const STK_DIR32 &i_dirRevoAxis);

			/****************************************************************************
			*	@brief		��ȡ������ת�᷽������
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_dirRevoAxis		��ת����ת�᷽������
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_DIR32			GetRevoAxis();

			/****************************************************************************
			*	@brief		�����������ת��ԭ��
			*	@param[in]	i_pntAxisOrigin		��ת�����ת��ԭ��
			*	@param[out]	��
			*	@retval		STK_SUCCESS			������ת���ԭ��ɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS			SetAxisOrigin(const STK_PNT32 &i_pntAxisOrigin);

			/****************************************************************************
			*	@brief		��ȡ�������ת��ԭ��
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_pntAxisOrigin			��ת���ԭ��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_PNT32			GetAxisOrigin();

			/****************************************************************************
			*	@brief		д�ļ�
			*	@param[in]	i_DataP		����д��λ��
			*	@param[in]	i_JsonDocP	Json�ĵ�����
			*	@param[out]	��
			*	@retval		STK_SUCCESS	д��ɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			virtual STK_STATUS					ToJson(void* i_DataP, void* i_JsonDocP);

			/****************************************************************************
			*	@brief		���ļ�
			*	@param[in]	i_DataP		���ݶ���λ��
			*	@param[in]	i_JsonDocP	Json�ĵ�����
			*	@param[out]	��
			*	@retval		STK_SUCCESS	����ɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			virtual STK_STATUS					FromJson(void* i_DataP, void* i_JsonDocP);

		};

		/**************************************************************************************
		*	@class	Stk_ToroidalFaceAttribute
		*	@brief	��̥��������
		*
		*	��¼��̥��Ļ���������Ϣ��<br>
		*	�̳���Stk_RevolutionFaceAttribute �ࡣ
		***************************************************************************************/
		class STK_TOOLKIT_EXPORT Stk_ToroidalFaceAttribute : public Stk_RevolutionFaceAttribute
		{
			STK_DECLARE_CLASS(Stk_ToroidalFaceAttribute)
		public:
			Stk_ToroidalFaceAttribute();
			virtual ~Stk_ToroidalFaceAttribute();
		protected:
			enum {
				_typeID = _SVLLIB_TYPE_TOROIDALFACEATTRIBUTE
			};
		public:
			/****************************************************************************
			*	@brief		��ȡ��̥����⾶
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_fMajorRadius			��̥����⾶
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_FLOAT32		GetMajorRadius();

			/****************************************************************************
			*	@brief		������̥����⾶
			*	@param[in]	i_fRadius			��̥����⾶
			*	@param[out]	��
			*	@retval		STK_SUCCESS			���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS		SetMajorRaius(STK_FLOAT32 i_fRadius);

			/****************************************************************************
			*	@brief		��ȡ��̥����ھ�
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_fMinorRadius			��̥����ھ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_FLOAT32		GetMinorRadius();

			/****************************************************************************
			*	@brief		������̥����ھ�
			*	@param[in]	i_fRadius			��̥����ھ�
			*	@param[out]	��
			*	@retval		STK_SUCCESS			���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS		SetMinorRadius(STK_FLOAT32 i_fRdius);

			/****************************************************************************
			*	@brief		д�ļ�
			*	@param[in]	i_DataP		����д��λ��
			*	@param[in]	i_JsonDocP	Json�ĵ�����
			*	@param[out]	��
			*	@retval		STK_SUCCESS	д��ɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			virtual STK_STATUS					ToJson(void* i_DataP, void* i_JsonDocP);

			/****************************************************************************
			*	@brief		���ļ�
			*	@param[in]	i_DataP		���ݶ���λ��
			*	@param[in]	i_JsonDocP	Json�ĵ�����
			*	@param[out]	��
			*	@retval		STK_SUCCESS	����ɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			virtual STK_STATUS					FromJson(void* i_DataP, void* i_JsonDocP);
		};

		/****************************************************************************************
		*	@class	Stk_LineAttribute
		*	@brief	Line������
		*
		*	��¼Line�Ļ���������Ϣ��<br>
		***************************************************************************************/
		class STK_TOOLKIT_EXPORT Stk_LineAttribute : public Stk_GeometryAttribute
		{
			STK_DECLARE_CLASS(Stk_LineAttribute)
		public:
			Stk_LineAttribute();
			virtual ~Stk_LineAttribute();
		protected:
			enum {
				_typeID = _SVLLIB_TYPE_LINEATTRIBUTE
			};
		public:
			/*****************************************************************************
			*	@brief		��ȡֱ���е�
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_pntCenterPoint	ֱ���е�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_PNT32		GetCenterPoint();

			/*****************************************************************************
			*	@brief		����ֱ���е�
			*	@param[in]	i_pntCenter	ֱ���е�
			*	@param[out]	��
			*	@retval		STK_SUCCESS	���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS		SetCenterPoint(STK_PNT32 &i_pntCenter);

			/*****************************************************************************
			*	@brief		��ȡֱ�߷�������
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_dirDirection	ֱ�߷�������
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_DIR32		GetDirection();

			/*****************************************************************************
			*	@brief		����ֱ�߷�������
			*	@param[in]	i_dirDirect	ֱ�߷�������
			*	@param[out]	��
			*	@retval		STK_SUCCESS	���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS		SetDirection(STK_DIR32 &i_dirDirect);

			/*****************************************************************************
			*	@brief		��ȡ�߽�����ʼ��
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_pntStartPoint	�߽�����ʼ��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_PNT32		GetStartPoint();

			/*****************************************************************************
			*	@brief		���ñ߽�����ʼ��
			*	@param[in]	i_pntStart	�߽�����ʼ��
			*	@param[out]	��
			*	@retval		STK_SUCCESS	���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS		SetStartPoint(STK_PNT32 &i_pntStart);

			/*****************************************************************************
			*	@brief		��ȡ�߽����յ�
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_pntEndPoint	�߽����յ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_PNT32		GetEndPoint();

			/*****************************************************************************
			*	@brief		���ñ߽����յ�
			*	@param[in]	i_pntEnd	�߽����յ�
			*	@param[out]	��
			*	@retval		STK_SUCCESS	���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS		SetEndPoint(STK_PNT32 &i_pntEnd);

			/*****************************************************************************
			*	@brief		��ȡ�߽��߳���
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_fLength	�߽��߳���
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_FLOAT32		GetLength();

			/*****************************************************************************
			*	@brief		���ñ߽��߳���
			*	@param[in]	i_fLength	�߽��߳���
			*	@param[out]	��
			*	@retval		STK_SUCCESS	���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS		SetLength(STK_FLOAT32 i_fLength);

			/****************************************************************************
			*	@brief		д�ļ�
			*	@param[in]	i_DataP		����д��λ��
			*	@param[in]	i_JsonDocP	Json�ĵ�����
			*	@param[out]	��
			*	@retval		STK_SUCCESS	д��ɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			virtual STK_STATUS					ToJson(void* i_DataP, void* i_JsonDocP);

			/****************************************************************************
			*	@brief		���ļ�
			*	@param[in]	i_DataP		���ݶ���λ��
			*	@param[in]	i_JsonDocP	Json�ĵ�����
			*	@param[out]	��
			*	@retval		STK_SUCCESS	����ɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			virtual STK_STATUS					FromJson(void* i_DataP, void* i_JsonDocP);
		};

		/****************************************************************************************
		*	@class	Stk_EllipseAttribute
		*	@brief	Ellipse������
		*
		*	��¼Ellipse�Ļ���������Ϣ��<br>
		***************************************************************************************/
		class STK_TOOLKIT_EXPORT Stk_EllipseAttribute : public Stk_GeometryAttribute
		{
			STK_DECLARE_CLASS(Stk_EllipseAttribute)
		public:
			Stk_EllipseAttribute();
			virtual ~Stk_EllipseAttribute();
		protected:
			enum {
				_typeID = _SVLLIB_TYPE_ELLIPSEATTRIBUTE
			};
		public:
			/*****************************************************************************
			*	@brief		��ȡ��Բ�����ĵ�
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_pntCenterPoint	��Բ�����ĵ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_PNT32		GetCenterPoint();

			/*****************************************************************************
			*	@brief		������Բ�����ĵ�
			*	@param[in]	i_pntCenter	��Բ�����ĵ�
			*	@param[out]	��
			*	@retval		STK_SUCCESS	���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS		SetCenterPoint(STK_PNT32 &i_pntCenter);

			/*****************************************************************************
			*	@brief		��ȡ��Բ������뾶
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_fMajorRadius	��Բ������뾶
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_FLOAT32		GetMajorRadius();

			/*****************************************************************************
			*	@brief		������Բ������뾶
			*	@param[in]	i_fRadius	��Բ������뾶
			*	@param[out]	��
			*	@retval		STK_SUCCESS	���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS		SetMajorRadius(STK_FLOAT32 i_fRadius);

			/*****************************************************************************
			*	@brief		��ȡ��Բ������뾶
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_fMinorRadius	��Բ������뾶
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_FLOAT32		GetMinorRadius();

			/*****************************************************************************
			*	@brief		������Բ������뾶
			*	@param[in]	i_fRadius	��Բ������뾶
			*	@param[out]	��
			*	@retval		STK_SUCCESS	���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS		SetMinorRadius(STK_FLOAT32 i_fRadius);

			/*****************************************************************************
			*	@brief		��ȡ�߽�����ʼ��
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_pntStartPoint	�߽�����ʼ��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_PNT32		GetStartPoint();

			/*****************************************************************************
			*	@brief		���ñ߽�����ʼ��
			*	@param[in]	i_pntStart	�߽�����ʼ��
			*	@param[out]	��
			*	@retval		STK_SUCCESS	���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS		SetStartPoint(STK_PNT32 &i_pntStart);

			/*****************************************************************************
			*	@brief		��ȡ�߽����յ�
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_pntEndPoint	�߽����յ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_PNT32		GetEndPoint();

			/*****************************************************************************
			*	@brief		���ñ߽����յ�
			*	@param[in]	i_pntEnd	�߽����յ�
			*	@param[out]	��
			*	@retval		STK_SUCCESS	���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS		SetEndPoint(STK_PNT32 &i_pntEnd);

			/****************************************************************************
			*	@brief		��ȡ��������
			*	@param[in]	��
			*	@param[out]	o_dirX				X��������
			*	@param[out]	o_dirY				Y��������
			*	@param[out]	o_dirZ				Z��������
			*	@retval		��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			void			GetXYZDir(STK_DIR32& o_dirX, STK_DIR32& o_dirY, STK_DIR32& o_dirZ);

			/****************************************************************************
			*	@brief		���û�������
			*	@param[in]	i_dirX				X��������
			*	@param[in]	i_dirY				Y��������
			*	@param[in]	i_dirZ				Z��������
			*	@param[out]	��
			*	@retval		STK_SUCCESS			���óɹ�
			*	@retval		STK_ERROR			����ʧ��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS		SetXYZDir(const STK_DIR32& i_dirX, const STK_DIR32& i_dirY, const STK_DIR32& i_dirZ);

			/****************************************************************************
			*	@brief		д�ļ�
			*	@param[in]	i_DataP		����д��λ��
			*	@param[in]	i_JsonDocP	Json�ĵ�����
			*	@param[out]	��
			*	@retval		STK_SUCCESS	д��ɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			virtual STK_STATUS					ToJson(void* i_DataP, void* i_JsonDocP);

			/****************************************************************************
			*	@brief		���ļ�
			*	@param[in]	i_DataP		���ݶ���λ��
			*	@param[in]	i_JsonDocP	Json�ĵ�����
			*	@param[out]	��
			*	@retval		STK_SUCCESS	����ɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			virtual STK_STATUS					FromJson(void* i_DataP, void* i_JsonDocP);
		};
	}
}
#endif