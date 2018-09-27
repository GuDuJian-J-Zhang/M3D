// All Rights Reserved. Copyright (C) 2016 Hoteamsoft. 
/**************************************************************************
*	@class	Stk_PMIEntity
*			������
*
**************************************************************************/
#ifndef _SVLLIB_PMIENTITY_H_
#define _SVLLIB_PMIENTITY_H_

#pragma once
#include "Stk_Object.h"
#include "Stk_CurveEntity.h"
namespace HoteamSoft {
	namespace SVLLib {
		class STK_TOOLKIT_EXPORT Stk_Leader : public Stk_Object
		{
			STK_DECLARE_CLASS(Stk_Leader)
		protected:
			enum { _typeID = _SVLLIB_TYPE_PMI_LEADER };	//!< ���
		public:
			Stk_Leader();
			virtual ~Stk_Leader();
		public:
			friend class Stk_PMI;
		public:
			/****************************************************************************
			*	@brief		��ȡ����������
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_eLeaderType	����������
			*	@note		��
			*	@attention	��
			****************************************************************************/
			StkLeaderTypeEnum			GetLeaderType();

			/****************************************************************************
			*	@brief		��������������
			*	@param[in]	i_eType			����������
			*	@param[out]	��
			*	@retval		STK_SUCCESS		���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS					SetLeaderType(StkLeaderTypeEnum i_eType);

			/****************************************************************************
			*	@brief		��ȡ������ĩ�����͡���ߡ���ʼλ�úͷ���
			*	@param[in]	��
			*	@param[out]	o_eTermType		ĩ������
			*	@param[out]	o_SizeA			ĩ�˿��
			*	@param[out]	o_pntLoc		ĩ����ʼλ��
			*	@param[out]	o_dirTerm		ĩ�˷���
			*	@retval		��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			void						GetTerminator(StkTermTypeEnum& o_eTermType, STK_FLOAT32 o_SizeA[2], STK_PNT32& o_pntLoc, STK_DIR32& o_dirTerm);

			/****************************************************************************
			*	@brief		����������ĩ�����͡���ߡ���ʼλ�úͷ���
			*	@param[in]	i_eTermType		ĩ������
			*	@param[in]	i_SizeA			ĩ�˿��
			*	@param[in]	i_pntLoc		ĩ����ʼλ��
			*	@param[in]	i_dirTerm		ĩ�˷���
			*	@param[out]	��
			*	@retval		STK_SUCCESS		���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS					SetTerminator(StkTermTypeEnum i_eTermType, STK_FLOAT32 i_SizeA[2], const STK_PNT32& i_pntLoc, const STK_DIR32& i_dirTerm);

			/****************************************************************************
			*	@brief		���ö�ά�����߱�ʶ
			*	@param[in]	i_bStub		��ά�����߱�ʶ
			*	@param[out]	��
			*	@retval		STK_SUCCESS	���óɹ�
			*	@retval		STK_ERROR	����ʧ��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS					SetBendLineFlag(STK_BOOL i_bIsBendLine);

			/****************************************************************************
			*	@brief		�Ƿ��Ƕ�ά������
			*	@param[in]	i_bStub		��ά�����߱�ʶ
			*	@param[out]	��
			*	@retval		STK_SUCCESS	���óɹ�
			*	@retval		STK_ERROR	����ʧ��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_BOOL					GetBendLineFlag();

			/****************************************************************************
			*	@brief		��ȡ���е�������
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_vcCurves	����������
			*	@note		��
			*	@attention	��
			****************************************************************************/
			vector<Stk_CurveEntityPtr>	GetAllCurve();

			/****************************************************************************
			*	@brief		��ȡ��������
			*	@param[in]	iIndex	����
			*	@param[out]	��
			*	@retval		Stk_CurveEntityPtr	������
			*	@note		��
			*	@attention	��
			****************************************************************************/
			Stk_CurveEntityPtr	GetCurve(int iIndex);

			/****************************************************************************
			*	@brief		�������������
			*	@param[in]	i_CurveP	Curve����ָ��
			*	@param[out]	��
			*	@retval		STK_SUCCESS	��ӳɹ�
			*	@retval		STK_ERROR	���ʧ��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS					AddCurve(Stk_CurveEntityPtr i_CurveP);

			/****************************************************************************
			*	@brief		д�ļ�
			*	@param[in]	i_DataP		����д��λ��
			*   @param[in]  i_pDoc     �ĵ�	
			*	@param[out]	��
			*	@retval		STK_SUCCESS	д��ɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS					ToJson(void* i_DataP, void* i_pDoc);

			/****************************************************************************
			*	@brief		���ļ�
			*	@param[in]	i_DataP		���ݶ���λ��
			*	@param[out]	��
			*	@retval		STK_SUCCESS	����ɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS					FromJson(void* i_DataP, void* i_JsonDocP);
		};

		/**************************************************************************************
		*	@class	Stk_Extensionline
		*	@brief	�ӳ�����
		*
		***************************************************************************************/
		class STK_TOOLKIT_EXPORT Stk_Extensionline : public Stk_Object
		{
			STK_DECLARE_CLASS(Stk_Extensionline)
		protected:
			enum { _typeID = _SVLLIB_TYPE_PMI_EXLINE };	//!< ���
		public:
			/****************************************************************************
			*	@brief		Stk_Extensionline���캯��
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			Stk_Extensionline(void);

			/****************************************************************************
			*	@brief		Stk_Extensionline��������
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			virtual ~Stk_Extensionline(void);

		public:
			/****************************************************************************
			*	@brief		��ȡ�ӳ��ߵĶ�������
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_eExLineType	�ӳ��ߵĶ�������
			*	@note		��
			*	@attention	��
			****************************************************************************/
			StkExtensTypeEnum			GetExLineType();

			/****************************************************************************
			*	@brief		�����ӳ��ߵĶ�������
			*	@param[in]	i_eType		�ӳ��ߵĶ�������
			*	@param[out]	��
			*	@retval		STK_SUCCESS	���óɹ�
			*	@retval		STK_SUCCESS	����ʧ��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS					SetExLineType(StkExtensTypeEnum i_nuType);

			/****************************************************************************
			*	@brief		�ж��ӳ�����ʽ
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_uiStyle	��ʽ
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_UINT32					GetStyle();

			/****************************************************************************
			*	@brief		�����ӳ�����ʽ
			*	@param[in]	m_uiStyle	��ʽ	
			*	@param[out]	��
			*	@retval		��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			void						SetStyle(STK_UINT32 i_uiStyle);

			/****************************************************************************
			*	@brief		��������
			*	@param[in]	i_curveP  ����
			*	@param[out]	��
			*	@retval		�ɹ����
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS                  SetCurve(Stk_CurveEntityPtr i_curveP);

			/****************************************************************************
			*	@brief		��ȡ����
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		
			*	@note		��
			*	@attention	��
			****************************************************************************/
			Stk_CurveEntityPtr          GetCurve();

			/****************************************************************************
			*	@brief		д�ļ�
			*	@param[in]	i_DataP		����д��λ��
			*	@param[in]	i_pDoc		�ĵ�
			*	@param[out]	��
			*	@retval		STK_SUCCESS	д��ɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS					ToJson(void* i_DataP, void* i_JsonDocP);

			/****************************************************************************
			*	@brief		���ļ�
			*	@param[in]	i_DataP		���ݶ���λ��
			*	@param[out]	��
			*	@retval		STK_SUCCESS	����ɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS					FromJson(void* i_DataP, void* i_JsonDocP);
		};

		/**************************************************************************************
		*	@class	Stk_SpecialLine
		*	@brief	��������
		*
		***************************************************************************************/

		class STK_TOOLKIT_EXPORT Stk_SpecialLine : public Stk_Object
		{
			STK_DECLARE_CLASS(Stk_SpecialLine)
		protected:
			enum { _typeID = _SVLLIB_TYPE_PMI_SPECIALLINE };	//!< ���
		public:
			/****************************************************************************
			*	@brief		Stk_SpecialLine���캯��
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			Stk_SpecialLine(void);

			/****************************************************************************
			*	@brief		Stk_SpecialLine��������
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			virtual ~Stk_SpecialLine(void);
		public:
			/****************************************************************************
			*	@brief		��ȡ��������;
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_eLineUsage	��������;
			*	@note		��
			*	@attention	��
			****************************************************************************/
			StkSpecLineUsageEnum		GetSpecLinUse();

			/****************************************************************************
			*	@brief		������������;
			*	@param[in]	i_eLineType	��������;
			*	@param[out]	��
			*	@retval		STK_SUCCESS		���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS					SetSpecLinUse(StkSpecLineUsageEnum i_eLineType);

			/****************************************************************************
			*	@brief		��ȡ����������
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_vcCurves		����������
			*	@note		��
			*	@attention	��
			****************************************************************************/
			vector<Stk_CurveEntityPtr>	GetCurves();

			/****************************************************************************
			*	@brief		��������ߵ�������������
			*	@param[in]	i_CurveP		Curve����
			*	@param[out]	��
			*	@retval		STK_SUCCESS
			*	@retval		STK_ERROR
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS					AddCurve(Stk_CurveEntityPtr i_CurveP);

			/****************************************************************************
			*	@brief		д�ļ�
			*	@param[in]	i_DataP		����д��λ��
			*	@param[in]	i_pDoc		�ĵ�
			*	@param[out]	��
			*	@retval		STK_SUCCESS	д��ɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS					ToJson(void* i_DataP, void* i_JsonDocP);

			/****************************************************************************
			*	@brief		���ļ�
			*	@param[in]	i_DataP		���ݶ���λ��
			*	@param[out]	��
			*	@retval		STK_SUCCESS	����ɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS					FromJson(void* i_DataP, void* i_JsonDocP);
		};

		/**************************************************************************************
		*	@class	Stk_OutFrame
		*	@brief	OutFrame�����
		*
		*	PMI�����
		***************************************************************************************/
		class STK_TOOLKIT_EXPORT Stk_OutFrame : public Stk_Object
		{
			STK_DECLARE_CLASS(Stk_OutFrame)
		protected:
			enum { _typeID = _SVLLIB_TYPE_CURVE_ELLIPSE };	//!< ���
		public:
			Stk_OutFrame();
			virtual ~Stk_OutFrame();
		public:
			/****************************************************************************
			*	@brief		��ȡ����Ӧ������
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_eFrameType		�������
			*	@note		��
			*	@attention	��
			****************************************************************************/
			StkOutFrameUsageEnum			GetFrameType();

			/****************************************************************************
			*	@brief		��������Ӧ������
			*	@param[in]	i_eType				�������
			*	@param[out]	��
			*	@retval		STK_SUCCESS			���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS						SetFrameType(StkOutFrameUsageEnum i_eType);

			/****************************************************************************
			*	@brief		��ȡ���Ķ�������
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_eFrameType		�������
			*	@note		��
			*	@attention	��
			****************************************************************************/
			StkOutFrameDefEnum				GetDefineType();

			/****************************************************************************
			*	@brief		�������Ķ�������
			*	@param[in]	i_eType				�������
			*	@param[out]	��
			*	@retval		STK_SUCCESS			���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS						SetDefineType(StkOutFrameDefEnum i_eType);

			/****************************************************************************
			*	@brief		��ȡ���е����������
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_vcCurves			���е����������
			*	@note		���ٽ���ʹ��,ʹ��GetFrameLineNum()��GetCurve()���
			*	@attention	��
			****************************************************************************/
			const vector<Stk_CurveEntityPtr>& GetFrameData();

			/****************************************************************************
			*	@brief		�������������
			*	@param[in]	i_CurveP			���������
			*	@param[out]	��
			*	@retval		STK_SUCCESS			���óɹ�
			*	@note		���ٽ���ʹ�ã�ʹ��AddCurve()���
			*	@attention	��
			****************************************************************************/
			STK_STATUS						SetFrameData(Stk_CurveEntityPtr i_CurveP);

			/****************************************************************************
			*	@brief		д�ļ�
			*	@param[in]	i_DataP		����д��λ��
			*	@param[in]	i_pDoc		�ĵ�
			*	@param[out]	��
			*	@retval		STK_SUCCESS	д��ɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS					ToJson(void* i_DataP, void* i_pDoc);

			/****************************************************************************
			*	@brief		���ļ�
			*	@param[in]	i_DataP		���ݶ���λ��
			*	@param[out]	��
			*	@retval		STK_SUCCESS	����ɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS					FromJson(void* i_DataP, void* i_JsonDocP);
		};
	}
}
#endif