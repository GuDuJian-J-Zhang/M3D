// All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
/****************************************************************************
*	@file	Stk_Entity.h
*
*	@brief	Ҫ�ػ���
*
*	@par	��ʷ:
*
****************************************************************************/
#ifndef _SVLLIB_CURVEENTITY_H_
#define _SVLLIB_CURVEENTITY_H_ 

#pragma once
#include "Stk_Object.h"
namespace HoteamSoft {
	namespace SVLLib {
		class STK_TOOLKIT_EXPORT Stk_CurveEntity : public Stk_Object
		{
			STK_DECLARE_CLASS(Stk_CurveEntity)
		protected:
			enum { _typeID = _SVLLIB_TYPE_CURVE };	//!< ���
		public:
			Stk_CurveEntity();
			virtual ~Stk_CurveEntity();
		public:
			/****************************************************************************
			*	@brief		��ȡCurve����ɫ
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_rgbaCurve			��ɫ
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_RGBA32					GetColor();

			/****************************************************************************
			*	@brief		����Curve����ɫ
			*	@param[in]	i_rgbaCurve			Curve����ɫ
			*	@retval		STK_SUCCESS			���óɹ�
			*	@retval		STK_ERROR			����ʧ��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS					SetColor(const STK_RGBA32& i_rgbaCurve);

			/****************************************************************************
			*	@brief		��ȡCurve������
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_eCurveType	Curve������
			*	@note		��
			*	@attention	��
			****************************************************************************/
			virtual StkCurveTypeEnum			GetCurveType();

			/****************************************************************************
			*	@brief		����Curve������
			*	@param[in]	i_eCurveType	Curve������
			*	@param[out]	��
			*	@retval		��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			void 			SetCurveType(StkCurveTypeEnum i_eCurveType);

			/****************************************************************************
			*	@brief		��ȡCurve��Ӧ������
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_eUsageType		Curve�Ķ��峡������
			*	@note		��������Curve�Ķ��峡������
			*	@attention	��
			****************************************************************************/
			StkCurveUsageEnum			GetUsageType();

			/****************************************************************************
			*	@brief		����Curve�Ķ��峡������
			*	@param[in]	i_eUsageType		Curve�Ķ��峡������
			*	@param[out]	��
			*	@retval		��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			void						SetUsageType(StkCurveUsageEnum i_eUsageType);

			/*****************************************************************************
			*	@brief		���������Ƿ�����
			*	@param[in]	i_bDispStat			�����Ƿ�����
			*	@param[out]	��
			*	@retval		STK_SUCCESS			���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS					SetCurveDispStat(STK_BOOL i_bDispStat);

			/*****************************************************************************
			*	@brief		��ȡ����������״̬
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_bDispStat
			*				= TRUE		��ʾ
			*				= FALSE		����
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_BOOL					GetCurveDispStat();

			/*****************************************************************************
			*	@brief		��ȡ��������ʾ����
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_nuDispType			������ʾ����
			*	@note		��
			*	@attention	��
			****************************************************************************/
			StkCurveDispTypeEnum		GetCurveDispType();
			
			/*****************************************************************************
			*	@brief		������������ʾ����
			*	@param[in]	i_eDispType				������ʾ����
			*	@param[out]	��
			*	@retval		STK_SUCCESS				���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS					SetCurveDispType(StkCurveDispTypeEnum i_eDispType);

			/****************************************************************************
			*	@brief		��ȡ�ļ��е�Stk_line����
			*	@param[in]	i_pJsonNode			��Json�ڵ�
			*	@param[in]	i_JsonDocP			Json�ĵ�����
			*	@param[out]	��
			*	@retval		STK_SUCCESS			��ȡ�ɹ�
			*	@retval		STK_ERROR			��ȡʧ��
			*	@retval		STK_ERROR_MEMORY	�ڴ�����ʧ��
			*	@note		��
			*	@attention	�ڲ��ӿ�
			****************************************************************************/
			virtual     STK_STATUS		FromJson(void* i_pJsonNode, void* i_JsonDocP);

			/*****************************************************************************
			*	@brief		дStk_line����
			*	@param[in]	Value* i_pJsonNode			��Json�ڵ�
			*	@param[in]	i_JsonDocP			Json�ĵ�����
			*	@param[out]	��
			*	@retval		STK_SUCCESS			д��ɹ�
			*	@retval		����				д��ʧ��
			*	@note		��
			*	@attention	�ڲ��ӿ�
			****************************************************************************/
			virtual      STK_STATUS		ToJson(void* i_DataP, void* i_JsonDocP);
		};

		/**************************************************************************************
		*	@class	Stk_Line
		*	@brief	��������
		*
		*	��¼PMI��������ȷ��һ���ߵ����������Ϣ��<br>
		*	�̳���Stk_CurveEntity�ࡣ
		***************************************************************************************/
		class STK_TOOLKIT_EXPORT Stk_Line : public Stk_CurveEntity
		{
			STK_DECLARE_CLASS(Stk_Line)
		protected:
			enum { _typeID = _SVLLIB_TYPE_CURVE_LINE };	//!< ���
		public:
			/****************************************************************************
			*	@brief		Stk_Line���캯��
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			Stk_Line(void);
			/****************************************************************************
			*	@brief		Stk_Line��������
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			virtual ~Stk_Line(void);
		public:
			/****************************************************************************
			 *	@brief		��ȡ��ʼ��
			 *	@param[in]	��
			 *	@param[out]	��
			 *	@retval		m_pntStart	��ʼ��
			 *	@note		��
			 *	@attention	��
			 ****************************************************************************/
			STK_PNT32				GetStartPoint();
	
			/****************************************************************************
			 *	@brief		��ȡ��ֹ��
			 *	@param[in]	��
			 *	@param[out]	��
			 *	@retval		m_pntEnd	��ֹ��
			 *	@note		��
			 *	@attention	��
			 ****************************************************************************/
			STK_PNT32				GetEndPoint();

			/****************************************************************************
			 *	@brief		������ʼ��
			 *	@param[in]	i_pntStart	��ʼ��
			 *	@param[out]	��
			 *	@retval		��
			 *	@note		��
			 *	@attention	��
			 ****************************************************************************/
			void					SetStartPoint(const STK_PNT32& i_pntStart);

			/****************************************************************************
			 *	@brief		������ֹ��
			 *	@param[in]	i_pntEnd	��ֹ��
			 *	@param[out]	��
			 *	@retval		��
			 *	@note		��
			 *	@attention	��
			 ****************************************************************************/
			void					SetEndPoint(const STK_PNT32& i_pntEnd);
			
			/****************************************************************************
			*	@brief		��ȡCurve������
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_eCurveType	Curve������
			*	@note		��
			*	@attention	��
			****************************************************************************/
			virtual StkCurveTypeEnum			GetCurveType();

			/****************************************************************************
			*	@brief		��ȡ�ļ��е�Stk_line����
			*	@param[in]	i_pJsonNode			��Json�ڵ�
			*	@param[in]	i_JsonDocP			Json�ĵ�����
			*	@param[out]	��
			*	@retval		STK_SUCCESS			��ȡ�ɹ�
			*	@retval		STK_ERROR			��ȡʧ��
			*	@retval		STK_ERROR_MEMORY	�ڴ�����ʧ��
			*	@note		��
			*	@attention	�ڲ��ӿ�
			****************************************************************************/
			STK_STATUS					FromJson(void* i_pJsonNode, void* i_JsonDocP);
			
			/*****************************************************************************
			*	@brief		дStk_line����
			*	@param[in]	Value* i_pJsonNode			��Json�ڵ�
			*	@param[in]	i_JsonDocP			Json�ĵ�����
			*	@param[out]	��
			*	@retval		STK_SUCCESS			д��ɹ�
			*	@retval		����				д��ʧ��
			*	@note		��
			*	@attention	�ڲ��ӿ�
			****************************************************************************/
			STK_STATUS					ToJson(void* i_DataP, void* i_JsonDocP);
		};

		/**************************************************************************************
		*	@class	Stk_Polyline
		*	@brief	������
		*
		*	��¼PMI������Ϊ���ߵ�������Ϣ��<br>
		*	�̳���Stk_CurveEntity�ࡣ
		***************************************************************************************/

		class STK_TOOLKIT_EXPORT Stk_PolyLine : public Stk_CurveEntity
		{
			STK_DECLARE_CLASS(Stk_PolyLine)
		protected:
			enum { _typeID = _SVLLIB_TYPE_CURVE_POLYLINE };	//!< ���
		public:
			/****************************************************************************
			*	@brief		Stk_PolyLine���캯��
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			Stk_PolyLine(void);
			/****************************************************************************
			*	@brief		Stk_PolyLine��������
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			virtual ~Stk_PolyLine(void);
		public:
			/****************************************************************************
			*	@brief		��ȡ���ߵ���
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_pntPoints			���ߵ���
			*	@note		��
			*	@attention	��
			****************************************************************************/
			const vector<STK_PNT32>&	GetPoints();

			/****************************************************************************
			*	@brief		�������ߵ���
			*	@param[in]	i_pntPoints			���ߵ���
			*	@param[out]	��
			*	@retval		STK_SUCCESS			���óɹ�
			*	@retval		STK_ERROR			����ʧ��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS					SetPoints(const vector<STK_PNT32>& i_pntPoints);

			/****************************************************************************
			*	@brief		��ȡ������ʼ��ֹ��Χ
			*	@param[in]	��
			*	@param[out]	o_fUMin				��ʼ��Χ
			*	@param[out]	o_fUMax				��ֹ��Χ
			*	@retval		��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			void						GetRange(STK_FLOAT32& o_fUMin, STK_FLOAT32& o_fUMax);

			/****************************************************************************
			*	@brief		��ȡCurve������
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_eCurveType	Curve������
			*	@note		��
			*	@attention	��
			****************************************************************************/
			virtual StkCurveTypeEnum			GetCurveType();
			
			/****************************************************************************
			*	@brief		����������ʼ��ֹ��Χ
			*	@param[in]	i_fUMin				��ʼ��Χ
			*	@param[in]	i_fUMax				��ֹ��Χ
			*	@param[out]	��
			*	@retval		STK_SUCCESS			���óɹ�
			*	@retval		STK_ERROR			����ʧ��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS					SetRange(STK_FLOAT32 i_fUMin, STK_FLOAT32 i_fUMax);
			
			/****************************************************************************
			*	@brief		��ȡ�ļ��е�Stk_PolyLine����
			*	@param[in]	i_pJsonNode			��Json�ڵ�
			*	@param[in]	i_JsonDocP			Json�ĵ�����
			*	@param[out]	��
			*	@retval		STK_SUCCESS			��ȡ�ɹ�
			*	@retval		STK_ERROR			��ȡʧ��
			*	@retval		STK_ERROR_MEMORY	�ڴ�����ʧ��
			*	@note		��
			*	@attention	�ڲ��ӿ�
			****************************************************************************/
			STK_STATUS					FromJson(void* i_pJsonNode, void* i_JsonDocP);
			
			/*****************************************************************************
			*	@brief		дStk_line����
			*	@param[in]	Value* i_pJsonNode			��Json�ڵ�
			*	@param[in]	i_JsonDocP			Json�ĵ�����
			*	@param[out]	��
			*	@retval		STK_SUCCESS			д��ɹ�
			*	@retval		����				д��ʧ��
			*	@note		��
			*	@attention	�ڲ��ӿ�
			****************************************************************************/
			STK_STATUS					ToJson(void* i_DataP, void* i_JsonDocP);
		};

		/**************************************************************************************
		*	@class	Stk_Arc
		*	@brief	������
		*
		*	��¼PMI������Ϊ���ߵ�������Ϣ��<br>
		*	�̳���Stk_CurveEntity�ࡣ
		***************************************************************************************/

		class STK_TOOLKIT_EXPORT Stk_Arc
			: public Stk_CurveEntity
		{
			STK_DECLARE_CLASS(Stk_Arc)
		protected:
			enum { _typeID = _SVLLIB_TYPE_CURVE_ELLIPSE };	//!< ���
		public:
			/****************************************************************************
			 *	@brief		Stk_Arc���캯��
			 *	@param[in]	��
			 *	@param[out]	��
			 *	@retval		��
			 *	@note		��
			 *	@attention	��
			 ****************************************************************************/
			Stk_Arc(void);
			/****************************************************************************
			*	@brief		Stk_Arc��������
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			virtual ~Stk_Arc(void);
		public:
			/****************************************************************************
			*	@brief		��ȡ������ʼ��ֹ��Χ
			*	@param[in]	��
			*	@param[out]	o_fUMin				��ʼ��Χ
			*	@param[out]	o_fUMax				��ֹ��Χ
			*	@retval		��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			void					GetRange(STK_FLOAT32& o_fUMin, STK_FLOAT32& o_fUMax);

			/****************************************************************************
			*	@brief		���û�����ʼ��ֹ��Χ
			*	@param[in]	i_fUMin				��ʼ��Χ
			*	@param[in]	i_fUMax				��ֹ��Χ
			*	@param[out]	��
			*	@retval		STK_SUCCESS			���óɹ�
			*	@retval		STK_ERROR			����ʧ��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS				SetRange(STK_FLOAT32 i_fUMin, STK_FLOAT32 i_fUMax);

			/****************************************************************************
			*	@brief		��ȡ�������ĵ�
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_pntCenter			���ĵ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_PNT32				GetCenterPoint();

			/****************************************************************************
			*	@brief		���û������ĵ�
			*	@param[in]	i_pntCenter			���ĵ�
			*	@param[out]	��
			*	@retval		STK_SUCCESS			���óɹ�
			*	@retval		STK_ERROR			����ʧ��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS				SetCenterPoint(const STK_PNT32& i_pntCenter);

			/****************************************************************************
			*	@brief		��ȡ���߷�������
			*	@param[in]	��
			*	@param[out]	o_dirX				X��������
			*	@param[out]	o_dirY				Y��������
			*	@param[out]	o_dirZ				Z��������
			*	@retval		��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			void					GetXYZDir(STK_DIR32& o_dirX, STK_DIR32& o_dirY, STK_DIR32& o_dirZ);

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
			STK_STATUS				SetXYZDir(const STK_DIR32& i_dirX, const STK_DIR32& i_dirY, const STK_DIR32& i_dirZ);
			
			/****************************************************************************
			*	@brief		����Բ������
			*	@param[in]	i_dirNormal	Բ������
			*	@param[out]	��
			*	@retval		��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			void					SetNormal(const STK_DIR32& i_dirNormal);

			/****************************************************************************
			*	@brief		��ȡԲ������
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_dirNormal	Բ������
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_DIR32				GetNormal();

			/****************************************************************************
			*	@brief		��ȡCurve������
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_eCurveType	Curve������
			*	@note		��
			*	@attention	��
			****************************************************************************/
			virtual StkCurveTypeEnum			GetCurveType();
			
			/****************************************************************************
			*	@brief		����ԭ�㷽��
			*	@param[in]	i_dirOrigin	ԭ�㷽��
			*	@param[out]	��
			*	@retval		��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			void					SetOriginDir(const STK_DIR32& i_dirOrigin);

			/****************************************************************************
			*	@brief		��ȡԭ�㷽��
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_dirOrigin	ԭ�㷽��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_DIR32				GetOriginDir();

			/****************************************************************************
			*	@brief		��ȡ���߰뾶
			*	@param[in]	��
			*	@param[out]	o_fMajorR			���뾶
			*	@param[out]	o_fMinorR			�̰뾶
			*	@retval		��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			void					GetRadius(STK_FLOAT32& o_fMajorR, STK_FLOAT32& o_fMinorR);

			/****************************************************************************
			*	@brief		���û��߰뾶
			*	@param[in]	i_fMajorR			���뾶
			*	@param[in]	i_fMinorR			�̰뾶
			*	@param[out]	��
			*	@retval		STK_SUCCESS			���óɹ�
			*	@retval		STK_ERROR			����ʧ��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS				SetRadius(STK_FLOAT32 i_fMajorR, STK_FLOAT32 i_fMinorR);

			/****************************************************************************
			*	@brief		��ȡ���ߵ�����
			*	@param[in]	��
			*	@param[out]	o_pntStart			�������
			*	@param[out]	o_pntEnd			�յ�����
			*	@retval		��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			void					GetCoordinatePnt(STK_PNT32& o_pntStart, STK_PNT32& o_pntEnd);

			/****************************************************************************
			*	@brief		���û��ߵ�����
			*	@param[in]	i_pntStart			�������
			*	@param[in]	i_pntEnd			�յ�����
			*	@param[out]	��
			*	@retval		STK_SUCCESS			���óɹ�
			*	@retval		STK_ERROR			����ʧ��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS				SetCoordiantePnt(const STK_PNT32& i_pntStart, const STK_PNT32& i_pntEnd);
			
			/****************************************************************************
			*	@brief		��ȡ���ߵ����λ��
			*	@param[in]	��
			*	@param[out]	o_fStartPar			������λ��
			*	@param[out]	o_fEndPar			�յ����λ��
			*	@retval		��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			void					GetParameter(STK_FLOAT32& o_fStartPar, STK_FLOAT32& o_fEndPar);

			/****************************************************************************
			*	@brief		���û��ߵ����λ��
			*	@param[in]	i_fStartPar			������λ��
			*	@param[in]	i_fEndPar			�յ����λ��
			*	@param[out]	��
			*	@retval		STK_SUCCESS			���óɹ�
			*	@retval		STK_ERROR			����ʧ��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS				SetParameter(STK_FLOAT32 i_fStartPar, STK_FLOAT32 i_fEndPar);
			/****************************************************************************
			*	@brief		��ȡ�ļ��е�Stk_Arc����
			*	@param[in]	i_pJsonNode			��Json�ڵ�
			*	@param[in]	i_JsonDocP			Json�ĵ�����
			*	@param[out]	��
			*	@retval		STK_SUCCESS			��ȡ�ɹ�
			*	@retval		STK_ERROR			��ȡʧ��
			*	@retval		STK_ERROR_MEMORY	�ڴ�����ʧ��
			*	@note		��
			*	@attention	�ڲ��ӿ�
			****************************************************************************/
			STK_STATUS					FromJson(void* i_pJsonNode, void* i_JsonDocP);
			/*****************************************************************************
			*	@brief		дStk_line����
			*	@param[in]	Value* i_pJsonNode			��Json�ڵ�
			*	@param[in]	i_JsonDocP			Json�ĵ�����
			*	@param[out]	��
			*	@retval		STK_SUCCESS			д��ɹ�
			*	@retval		����				д��ʧ��
			*	@note		��
			*	@attention	�ڲ��ӿ�
			****************************************************************************/
			STK_STATUS					ToJson(void* i_DataP, void* i_JsonDocP);
		};


		/****************************************************************************************
		*	@class	Stk_NurbsCurve
		*	@brief	NURBS������
		*
		*	��¼NURBS���ߵ�������Ϣ��<br>
		*	�̳���Stk_CurveEntity�ࡣ
		***************************************************************************************/

		class STK_TOOLKIT_EXPORT Stk_NurbsCurve
			: public Stk_CurveEntity
		{
			STK_DECLARE_CLASS(Stk_NurbsCurve)
		protected:
			enum { _typeID = _SVLLIB_TYPE_CURVE_NURBS };	//!< ���
		public:
			/****************************************************************************
			*	@brief		Stk_Arc���캯��
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			Stk_NurbsCurve(void);
			/****************************************************************************
			*	@brief		Stk_Arc��������
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			virtual ~Stk_NurbsCurve(void);
		public:
			/*****************************************************************************
			*	@brief		��ȡ��ʼ����ֹ��Χ
			*	@param[in]	��
			*	@param[out]	o_fMin					��ʼ��Χ
			*	@param[out]	o_fMax					��ֹ��Χ
			*	@retval		STK_SUCCESS				��ȡ�ɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS				GetDomain(STK_FLOAT32& o_fMin, STK_FLOAT32& o_fMax);

			/*****************************************************************************
			*	@brief		������ʼ����ֹ��Χ
			*	@param[in]	i_fMin					��ʼ��Χ
			*	@param[in]	i_fMax					��ֹ��Χ
			*	@param[out]	��
			*	@retval		STK_SUCCESS				���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS				SetDomain(STK_FLOAT32 i_fMin, STK_FLOAT32 i_fMax);

			/*****************************************************************************
			*	@brief		��ȡDegree
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_nuDegree				Degreeֵ
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_UINT32				GetDegree();

			/*****************************************************************************
			*	@brief		����Degree
			*	@param[in]	i_nuDegree				Degree����
			*	@param[out]	��
			*	@retval		STK_SUCCESS				���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS				SetDegree(STK_UINT32 i_nuDegree);

			/*****************************************************************************
			*	@brief		��ȡKnot�Ķ�����
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_vecKnotPlicity		Knot�Ķ�����
			*	@note		��
			*	@attention	��
			****************************************************************************/
			vector<STK_UINT32>		GetKnotMultiplicity();

			/*****************************************************************************
			*	@brief		���Knot�Ķ�����
			*	@param[in]	i_nuKnotPlicity			Knot�Ķ�����
			*	@param[out]	��
			*	@retval		STK_SUCCESS				��ӳɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS				AddKnotMultiplicity(STK_UINT32 i_nuKnotPlicity);

			/*****************************************************************************
			*	@brief		��ȡKnot�ڵ�
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_vecKnots				Knots�ڵ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			vector<STK_FLOAT32>		GetKnots();

			/*****************************************************************************
			*	@brief		���Knot�ڵ�
			*	@param[in]	i_fKnot					Knot�ڵ�
			*	@param[out]	��
			*	@retval		STK_SUCCESS				��ӳɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS				AddKnot(STK_FLOAT32 i_fKnot);

			/*****************************************************************************
			*	@brief		��ȡ���Ƶ�
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_vecPoints				Nurbs���ߵĿ��Ƶ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			vector<STK_PNT32>		GetPoints();

			/*****************************************************************************
			*	@brief		���Nurbs���߿��Ƶ�
			*	@param[in]	i_pntCtrl				Nurbs���߿��Ƶ�
			*	@param[out]	��
			*	@retval		STK_SUCCESS				��ӳɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS				AddPoint(STK_PNT32 i_pntCtrl);

			/*****************************************************************************
			*	@brief		��ȡNurbs���߿��Ƶ��Ȩ��
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_vecWeights				Nurbs���߿��Ƶ��Ȩ��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			vector<STK_FLOAT32>		GetWeights();

			/*****************************************************************************
			*	@brief		���Nurbs���߿��Ƶ��Ȩ��
			*	@param[in]	i_fWeight					Nurbs���߿��Ƶ��Ȩ��
			*	@param[out]	��
			*	@retval		STK_SUCCESS					��ӳɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS				AddWeight(STK_FLOAT32 i_fWeight);

			/*****************************************************************************
			*	@brief		��дNurbs���߿��Ƶ�
			*	@param[in]	i_Point					���Ƶ�
			*	@param[out]	o_fAtr					ռλ��λ��
			*	@retval		��
			*	@note		��
			*	@attention	�ڲ��ӿ�
			****************************************************************************/
			STK_STATUS				WriteVertex(STK_FLOAT32* o_fAtr, STK_PNT32 i_Point);

			/****************************************************************************
			*	@brief		��ȡ�ļ��е�Nurbs����
			*	@param[in]	i_strJson			Json��ʽ�ַ���
			*	@param[out]	��
			*	@retval		STK_SUCCESS			��ȡ�ɹ�
			*	@retval		STK_ERROR			��ȡʧ��
			*	@retval		STK_ERROR_MEMORY	�ڴ�����ʧ��
			*	@note		��
			*	@attention	�ڲ��ӿ�
			****************************************************************************/
			STK_STATUS					FromJson(string i_strJson);
			/*****************************************************************************
			*	@brief		дStk_line����
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		Json��ʽ�ַ���
			*	@note		��
			*	@attention	�ڲ��ӿ�
			****************************************************************************/
			string					ToJson();
		};



		/****************************************************************************************
		*	@class	Stk_Hyperbola
		*	@brief	˫������
		*
		*	��¼˫���ߵ�������Ϣ��<br>
		*	�̳���Stk_CurveEntity�ࡣ
		***************************************************************************************/
		class STK_TOOLKIT_EXPORT Stk_Hyperbola : public Stk_CurveEntity
		{
			STK_DECLARE_CLASS(Stk_Hyperbola)
		protected:
			enum {
				_typeID = _SVLLIB_TYPE_CURVE_HYPERBOLA
			};	//!< ���

		public:
			/*****************************************************************************
			*	@brief		Stk_Hyperbola���캯��
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			Stk_Hyperbola(void);

			/*****************************************************************************
			*	@brief		Stk_Hyperbola��������
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			~Stk_Hyperbola(void);

			/*****************************************************************************
			*	@brief		��ȡ��ʼ����ֹ��Χ
			*	@param[in]	��
			*	@param[out]	o_fMin					��ʼ��Χ
			*	@param[out]	o_fMax					��ֹ��Χ
			*	@retval		STK_SUCCESS				��ȡ�ɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS				GetDomain(STK_FLOAT32& o_fMin, STK_FLOAT32& o_fMax);

			/*****************************************************************************
			*	@brief		������ʼ����ֹ��Χ
			*	@param[in]	i_fMin					��ʼ��Χ
			*	@param[in]	i_fMax					��ֹ��Χ
			*	@param[out]	��
			*	@retval		STK_SUCCESS				���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS				SetDomain(STK_FLOAT32 i_fMin, STK_FLOAT32 i_fMax);

			/*****************************************************************************
			*	@brief		��ȡ˫���ߵ����ĵ�
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_pntCenter				˫�������ĵ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_PNT32				GetCenterPoint();

			/*****************************************************************************
			*	@brief		����˫���ߵ����ĵ�
			*	@param[in]	i_pntCenter				˫���ߵ����ĵ�
			*	@param[out]	��
			*	@retval		STK_SUCCESS				���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS				SetCenterPoint(STK_PNT32& i_pntCenter);

			/*****************************************************************************
			*	@brief		��ȡ˫���ߵ�XYZ��������
			*	@param[in]	��
			*	@param[out]	o_dirX					X��������
			*	@param[out]	o_dirY					Y��������
			*	@param[out]	o_dirZ					Z��������
			*	@retval		STK_SUCCESS				��ȡ�ɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS				GetXYZDir(STK_DIR32& o_dirX, STK_DIR32& o_dirY, STK_DIR32& o_dirZ);

			/*****************************************************************************
			*	@brief		����˫���ߵ�XYZ��������
			*	@param[in]	i_dirX					X��������
			*	@param[in]	i_dirY					Y��������
			*	@param[in]	i_dirZ					Z��������
			*	@param[out]	��
			*	@retval		STK_SUCCESS				���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS				SetXYZDir(STK_DIR32& i_dirX, STK_DIR32& i_dirY, STK_DIR32& i_dirZ);

			/*****************************************************************************
			*	@brief		��ȡ˫���ߵİ���
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_fSemiAxis				��ȡ�ɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_FLOAT32				GetSemiAxis();

			/*****************************************************************************
			*	@brief		����˫���ߵİ���
			*	@param[in]	i_fSemiAxis				˫���ߵİ���
			*	@param[out]	��
			*	@retval		STK_SUCCESS				���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS				SetSemiAxis(STK_FLOAT32 i_fSemiAxis);

			/*****************************************************************************
			*	@brief		��ȡ˫���ߵ�SemiImgAxis
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_fSemiAxis				��ȡ�ɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_FLOAT32				GetSemiImgAxis();

			/*****************************************************************************
			*	@brief		����˫���ߵ�SemiImgAxis
			*	@param[in]	i_fSemiImgAxis			˫���ߵ�SemiImgAxis
			*	@param[out]	��
			*	@retval		STK_SUCCESS				���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS				SetSemiImgAxis(STK_FLOAT32 i_fSemiImgAxis);

			/*****************************************************************************
			*	@brief		��ȡ�������
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_pntStart				�������
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_PNT32				GetStartPoint();

			/*****************************************************************************
			*	@brief		�����������
			*	@param[in]	i_pntStart				�������
			*	@param[out]	��
			*	@retval		STK_SUCCESS				���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS				SetStartPoint(STK_PNT32 i_pntStart);

			/*****************************************************************************
			*	@brief		��ȡ�յ�����
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_pntEnd				�յ�����
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_PNT32				GetEndPoint();

			/*****************************************************************************
			*	@brief		�����յ�����
			*	@param[in]	i_pntEnd				�յ�����
			*	@param[out]	��
			*	@retval		STK_SUCCESS				���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS				SetEndPoint(STK_PNT32 i_pntEnd);

			/****************************************************************************
			*	@brief		��ȡ�ļ��е�Stk_Hyperbola����
			*	@param[in]	i_strJson			Json��ʽ�ַ���
			*	@param[out]	��
			*	@retval		STK_SUCCESS			��ȡ�ɹ�
			*	@retval		STK_ERROR			��ȡʧ��
			*	@retval		STK_ERROR_MEMORY	�ڴ�����ʧ��
			*	@note		��
			*	@attention	�ڲ��ӿ�
			****************************************************************************/
			STK_STATUS					FromJson(string i_strJson);
			/*****************************************************************************
			*	@brief		дStk_line����
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		Json��ʽ�ַ���
			*	@note		��
			*	@attention	�ڲ��ӿ�
			****************************************************************************/
			string					ToJson();
		};


		/****************************************************************************************
		*	@class	Stk_Parabola
		*	@brief	��������
		*
		*	��¼�����ߵ�������Ϣ��<br>
		*	�̳���Stk_CurveEntity�ࡣ
		***************************************************************************************/
		class STK_TOOLKIT_EXPORT Stk_Parabola : public Stk_CurveEntity
		{
			STK_DECLARE_CLASS(Stk_Parabola)
		protected:
			enum {
				_typeID = _SVLLIB_TYPE_CURVE_PARABOLA
			};	//!< ���
		public:
			/*****************************************************************************
			*	@brief		Stk_Parabola���캯��
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			Stk_Parabola(void);

			/*****************************************************************************
			*	@brief		Stk_Parabola��������
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			~Stk_Parabola(void);

			/*****************************************************************************
			*	@brief		��ȡ��ʼ����ֹ��Χ
			*	@param[in]	��
			*	@param[out]	o_fMin					��ʼ��Χ
			*	@param[out]	o_fMax					��ֹ��Χ
			*	@retval		STK_SUCCESS				��ȡ�ɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS				GetDomain(STK_FLOAT32& o_fMin, STK_FLOAT32& o_fMax);

			/*****************************************************************************
			*	@brief		������ʼ����ֹ��Χ
			*	@param[in]	i_fMin					��ʼ��Χ
			*	@param[in]	i_fMax					��ֹ��Χ
			*	@param[out]	��
			*	@retval		STK_SUCCESS				���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS				SetDomain(STK_FLOAT32 i_fMin, STK_FLOAT32 i_fMax);

			/*****************************************************************************
			*	@brief		��ȡ�������
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_pntStart				�������
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_PNT32				GetStartPoint();

			/*****************************************************************************
			*	@brief		�����������
			*	@param[in]	i_pntStart				�������
			*	@param[out]	��
			*	@retval		STK_SUCCESS				���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS				SetStartPoint(STK_PNT32 i_pntStart);

			/*****************************************************************************
			*	@brief		��ȡ�յ�����
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_pntEnd				�յ�����
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_PNT32				GetEndPoint();

			/*****************************************************************************
			*	@brief		�����յ�����
			*	@param[in]	i_pntEnd				�յ�����
			*	@param[out]	��
			*	@retval		STK_SUCCESS				���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS				SetEndPoint(STK_PNT32 i_pntEnd);

			/*****************************************************************************
			*	@brief		��ȡ���ĵ�
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_pntCenter				���ĵ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_PNT32				GetCenterPoint();

			/*****************************************************************************
			*	@brief		�������ĵ�
			*	@param[in]	i_pntCenter				���ĵ�
			*	@param[out]	��
			*	@retval		STK_SUCCESS				���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS				SetCenterPoint(STK_PNT32& i_pntCenter);

			/*****************************************************************************
			*	@brief		��ȡ�����ߵ�XYZ��������
			*	@param[in]	��
			*	@param[out]	o_dirX					X��������
			*	@param[out]	o_dirY					Y��������
			*	@param[out]	o_dirZ					Z��������
			*	@retval		STK_SUCCESS				��ȡ�ɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS				GetXYZDir(STK_DIR32& o_dirX, STK_DIR32& o_dirY, STK_DIR32& o_dirZ);

			/*****************************************************************************
			*	@brief		���������ߵ�XYZ��������
			*	@param[in]	i_dirX					X��������
			*	@param[in]	i_dirY					Y��������
			*	@param[in]	i_dirZ					Z��������
			*	@param[out]	��
			*	@retval		STK_SUCCESS				���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS				SetXYZDir(STK_DIR32& i_dirX, STK_DIR32& i_dirY, STK_DIR32& i_dirZ);

			/*****************************************************************************
			*	@brief		��ȡFocal����
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_fFocalDis				Focal����
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_FLOAT32				GetFocalDistance();

			/*****************************************************************************
			*	@brief		����Focal����
			*	@param[in]	i_fDistance				Focal����
			*	@param[out]	��
			*	@retval		STK_SUCCESS				���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS				SetFocalDistance(STK_FLOAT32 i_fDistance);

			/****************************************************************************
			*	@brief		��ȡ�ļ��е�Stk_Hyperbola����
			*	@param[in]	i_strJson			Json��ʽ�ַ���
			*	@param[out]	��
			*	@retval		STK_SUCCESS			��ȡ�ɹ�
			*	@retval		STK_ERROR			��ȡʧ��
			*	@retval		STK_ERROR_MEMORY	�ڴ�����ʧ��
			*	@note		��
			*	@attention	�ڲ��ӿ�
			****************************************************************************/
			STK_STATUS					FromJson(string i_strJson);
			/*****************************************************************************
			*	@brief		дStk_line����
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		Json��ʽ�ַ���
			*	@note		��
			*	@attention	�ڲ��ӿ�
			****************************************************************************/
			string					ToJson();
		};


	}
}
#endif