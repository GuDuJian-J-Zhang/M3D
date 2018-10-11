// All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
/****************************************************************************
*	@file	Stk_CompositeText.h
*
*	@brief	�ı��������ı��������
*
*	@par	��ʷ:
*		
****************************************************************************/
#ifndef _SVLLIB_COMPOSITETEXT_H_
#define _SVLLIB_COMPOSITETEXT_H_

#include "Stk_Object.h"
#include "Stk_PMIEntity.h"

namespace HoteamSoft {
	namespace SVLLib {
		
		/**************************************************************************************
		*	@class	Stk_ComText
		*	@brief	�����ı���
		*
		*	�Զ���ı�����������ϡ�
		***************************************************************************************/
		class Stk_Text;
		class STK_TOOLKIT_EXPORT Stk_ComText : public Stk_Object
		{
			STK_DECLARE_CLASS(Stk_ComText)
		protected:
			enum { _typeID = _SVLLIB_TYPE_PMI_COMTEXT };	//!< ���
		public:
			Stk_ComText();
			virtual ~Stk_ComText();
		public:
			/****************************************************************************
			*	@brief		���ø����ı���Χ��
			*	@param[in]	i_boxBounding		��Χ��
			*	@retval		STK_SUCCESS			���óɹ�
			*	@retval		STK_ERROR			����ʧ��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS						SetOuterBox(const STK_BOX32 &i_boxBounding);

			/****************************************************************************
			*	@brief		��ȡ�����ı���Χ��
			*	@param[in]	��
			*	@retval		m_boxBounding		�����ı���Χ��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_BOX32						GetOuterBox();

			/****************************************************************************
			*	@brief		�����������
			*	@param[in]	i_eFrameType		�������
			*	@param[out]	��
			*	@retval		STK_SUCCESS			���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS						SetFrameType(StkFrameTypeEnum i_eFrameType);

			/****************************************************************************
			*	@brief		��ȡ�������
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		�������
			*	@note		��
			*	@attention	��
			****************************************************************************/
			StkFrameTypeEnum				GetFrameType();

			/****************************************************************************
			*	@brief		��ȡ�ı���������Ĵ�С
			*	@param[in]	��
			*	@retval		m_vcTexts.size()		�ı���������Ĵ�С
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_SIZE_T						GetNumTexts();

			/****************************************************************************
			*	@brief		��ȡ�ı���������
			*	@param[in]	��
			*	@retval		m_vcTexts			�ı���������
			*	@note		��
			*	@attention	��
			****************************************************************************/
			vector<Stk_TextPtr>				GetTexts();

			/****************************************************************************
			*	@brief		�����ı���������
			*	@param[in]	i_vcTexts			�ı���������
			*	@retval		STK_SUCCESS			�ı����óɹ�
			*	@retval		STK_ERROR			�ı�����ʧ��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS						SetTexts(const vector<Stk_TextPtr>& i_vcTexts);

			/****************************************************************************
			*	@brief		д�ļ�
			*	@param[in]	i_DataP		����д��λ��
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

			/****************************************************************************
			*	@brief		���ð�������
			*	@param[in]	i_stkEnvelopeP		��������
			*	@param[out]	��
			*	@retval		STK_SUCCESS			���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS						SetEnvelope(Stk_EnvelopePtr i_stkEnvelopeP);

			/****************************************************************************
			*	@brief		��ȡ��������
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		��������
			*	@note		��
			*	@attention	��
			****************************************************************************/
			Stk_EnvelopePtr					GetEnvelope();
		};

		/**************************************************************************************
		*	@class	Stk_Text
		*	@brief	�ı���
		*
		*	��¼�����ı���������Ϣ��
		***************************************************************************************/
		class STK_TOOLKIT_EXPORT Stk_Text : public Stk_Object
		{
			STK_DECLARE_CLASS(Stk_Text)
		public:
			Stk_Text();
			virtual ~Stk_Text();
		public:
			/****************************************************************************
			*	@brief		��ȡ�ı���λ��
			*	@param[in]	��
			*	@retval		m_pntInnerLoc		�ı���λ��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_PNT32						GetInnerLoc();

			/****************************************************************************
			*	@brief		�����ı���λ��
			*	@param[in]	i_pntLocation		��λ��
			*	@retval		STK_SUCCESS			���óɹ�
			*	@retval		STK_ERROR			����ʧ��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS						SetInnerLoc(const STK_PNT32& i_pntLocation);

			/****************************************************************************
			*	@brief		��ȡ��ת����
			*	@param[out]	o_dirXAxis			X��������
			*	@param[out]	o_dirYAxis			Y��������
			*	@retval		��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			void							GetInnerXYAxis(STK_DIR32& o_dirXAxis, STK_DIR32& o_dirYAxis);

			/****************************************************************************
			*	@brief		������ת����
			*	@param[in]	i_dirXAxis			X��������
			*	@param[in]	i_dirYAxis			Y��������
			*	@param[out]	��
			*	@retval		STK_SUCCESS			���óɹ�
			*	@retval		STK_ERROR			����ʧ��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS						SetInnerXYAxis(const STK_DIR32& i_dirXAxis, const STK_DIR32& i_dirYAxis);
			
			/****************************************************************************
			*	@brief		�����ı���ת�Ƕ�
			*	@param[in]	i_fRotationAngle	��ת�Ƕ�
			*	@param[out] ��
			*	@retval		STK_SUCCESS			���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS						SetRotationAngle(STK_DOUBLE64 i_fRotationAngle);

			/****************************************************************************
			*	@brief		��ȡ�ı���ת�Ƕ�
			*	@param[in]	��
			*	@param[out] ��
			*	@retval		�ı���ת�Ƕ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_DOUBLE64					GetRotationAngle();

			/****************************************************************************
			*	@brief		��ȡ�ַ����
			*	@param[in]	��
			*	@param[out]	o_fWidth			�ַ���
			*	@param[out]	o_fHeight			�ַ���
			*	@retval		��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			void							GetCharWidthAndHeight(STK_FLOAT32& o_fWidth, STK_FLOAT32& o_fHeight);

			/****************************************************************************
			*	@brief		�����ַ����
			*	@param[in]	i_fWidth			�ַ���
			*	@param[in]	i_fHeight			�ַ���
			*	@param[out]	��
			*	@retval		STK_SUCCESS			���óɹ�
			*	@retval		STK_ERROR			����ʧ��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS						SetCharWidthAndHeight(const STK_FLOAT32& i_fWidth, const STK_FLOAT32& i_fHeight);
			
			/****************************************************************************
			*	@brief		��ȡ�ַ����
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_fCharDis			�ַ����
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_FLOAT32						GetCharSpace();

			/****************************************************************************
			*	@brief		�����ַ����
			*	@param[in]	i_fCharDis			�ַ����
			*	@param[out]	��
			*	@retval		STK_SUCCESS			���óɹ�
			*	@retval		STK_ERROR			����ʧ��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS						SetCharSpace(const STK_FLOAT32& i_fCharSpace);

			/****************************************************************************
			*	@brief		��ȡ�����ʽ
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		m_wstrFontName			����
			*	@note		��
			*	@attention	��
			****************************************************************************/
			wstring							GetFontName();

			/****************************************************************************
			*	@brief		�����ַ���ʽ
			*	@param[in]	i_wstrFontName			����
			*	@param[out]	��
			*	@retval		STK_SUCCESS			���óɹ�
			*	@retval		STK_ERROR			����ʧ��
			*	@note		��������63�ַ�
			*	@attention	��
			****************************************************************************/
			void							SetFontName(wstring i_wstrFontName);

			/****************************************************************************
			*	@brief		��ȡ�ı�����
			*	@param[in]	��
			*	@retval		m_wstrText			�ı�����
			*	@note		��
			*	@attention	��
			****************************************************************************/
			wstring							GetTextContent();

			/****************************************************************************
			*	@brief		�����ı�����
			*	@param[in]	i_wstrText			�ı�����
			*	@retval		STK_SUCCESS			���óɹ�
			*	@retval		STK_ERROR			����ʧ��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS						SetTextContent(wstring i_wstrText);

			/****************************************************************************
			*	@brief		�����ַ�Ӧ������
			*	@param[in]	i_Type			�ַ�Ӧ������
			*	@param[out]	��
			*	@retval		��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			void							SetUsageType(StkTextTypeEnum i_Type);

			/****************************************************************************
			*	@brief		��ȡ�ַ�Ӧ������
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			StkTextTypeEnum					GetUsageType();

			/****************************************************************************
			*	@brief		�����ı���Χ��
			*	@param[in]	i_boxBounding		��Χ��
			*	@param[out]	��
			*	@retval		STK_SUCCESS			���óɹ�
			*	@retval		STK_ERROR			����ʧ��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS						SetOuterBox(const STK_BOX32 &i_boxBounding);

			/****************************************************************************
			*	@brief		��ȡ�ı���Χ��
			*	@param[in]	��
			*	@param[out] ��
			*	@retval		m_boxBounding		�ı���Χ��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_BOX32						GetOuterBox();

			/****************************************************************************
			*	@brief		��ȡ�ı�������
			*	@param[in]	��
			*	@param[out] ��
			*	@retval		�ı�������
			*	@note		��
			*	@attention	��
			****************************************************************************/
			StkFontStyleTypeEnum			GetFontStyle();

			/****************************************************************************
			*	@brief		�����ı���������
			*	@param[in]	i_eFontType			��������
			*	@param[out] ��
			*	@retval		STK_SUCCESS			���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS						SetFontType(StkFontTypeEnum i_eFontType);

			/****************************************************************************
			*	@brief		��ȡ�ı���������
			*	@param[in]	��
			*	@param[out] ��
			*	@retval		�ı���������
			*	@note		��
			*	@attention	��
			****************************************************************************/
			StkFontTypeEnum					GetFontType();

			/****************************************************************************
			*	@brief		�����ı���������
			*	@param[in]	i_eAttributeType	�ı���������
			*	@param[out] ��
			*	@retval		STK_SUCCESS			���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS						SetAttributeType(StkTextAttributeTypeEnum i_eAttributeType);

			/****************************************************************************
			*	@brief		��ȡ�ı���������
			*	@param[in]	��
			*	@param[out] ��
			*	@retval		�ı���������
			*	@note		��
			*	@attention	��
			****************************************************************************/
			StkTextAttributeTypeEnum		GetAttributeType();

			/****************************************************************************
			*	@brief		��ȡ�������
			*	@param[in]	��
			*	@param[out] ��
			*	@retval		�������
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_UINT32						GetOutFrameNum();

			/****************************************************************************
			*	@brief		ͨ��������ȡ���ָ��
			*	@param[in]	����
			*	@param[out] ��
			*	@retval		�������
			*	@note		ͨ��GetOutFrameNum����ȡ������������Ϊ"0"��"����-1"
			*	@attention	��
			****************************************************************************/
			Stk_OutFramePtr					GetOutFrameByIndex(STK_UINT32 i_nuIndex);

			/****************************************************************************
			*	@brief		�������
			*	@param[in]	i_OutFrameP		���ָ��
			*	@param[out] ��
			*	@retval		״̬
			*	@note
			*	@attention	��
			****************************************************************************/
			STK_STATUS						AddOutFrame(Stk_OutFramePtr i_OutFrameP);

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
			STK_STATUS					FromJson(void* i_DataP, void* i_JsonDocP);
		};

		/**************************************************************************************
		*	@class	Stk_Envelope
		*	@brief	�������
		*
		*
		***************************************************************************************/
		class STK_TOOLKIT_EXPORT Stk_Envelope : public Stk_Object
		{
			STK_DECLARE_CLASS(Stk_Envelope)
		public:
			Stk_Envelope();
			virtual ~Stk_Envelope();
		public:
			/****************************************************************************
			*	@brief		���ð����ı���ɫ
			*	@param[in]	i_rgbaBackground	����򱳾�ɫ
			*	@param[out]	��
			*	@retval		STK_SUCCESS			���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS						SetBackgroundColor(STK_RGBA32 i_rgbaBackground);

			/****************************************************************************
			*	@brief		��ȡ�����ı���ɫ
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		�����ı���ɫ
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_RGBA32						GetBackgroundColor();

			/****************************************************************************
			*	@brief		���ð����߿����ɫ
			*	@param[in]	i_rgbaFrame			�����߿����ɫ
			*	@param[out]	��
			*	@retval		STK_SUCCESS			���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS						SetFrameColor(STK_RGBA32 i_rgbaFrame);

			/****************************************************************************
			*	@brief		��ȡ�����߿����ɫ
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		�����ı���ɫ
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_RGBA32						GetFrameColor();

			/****************************************************************************
			*	@brief		�����Ƿ��а����ı߿�ı�ʶ
			*	@param[in]	i_bHasFrame		�Ƿ��а����ı߿�ı�ʶ
			*	@param[out]	��
			*	@retval		STK_SUCCESS		���óɹ�
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS						SetStyle(STK_INT32 i_iStyle);

			/****************************************************************************
			*	@brief		��ȡ�����ı߿�ı�ʶ
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		�Ƿ��а����ı߿�ı�ʶ
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_BOOL						GetStyle();

			/****************************************************************************
			*	@brief		����ê��
			*	@param[in]	i_eType				ê������
			*	@param[in]	i_pntAnchor			ê��
			*	@param[out]	��
			*	@retval		STK_SUCCESS			���óɹ�
			*	@retval		����				����ʧ��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS						SetAnchorPoint(StkAnchorTypeEnum i_eType, STK_PNT32 i_pntAnchor);

			/****************************************************************************
			*	@brief		��ȡ���ڵ�����ê������
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		���ڵ�����ê������
			*	@note		��
			*	@attention	��
			****************************************************************************/
			vector<StkAnchorTypeEnum>&		GetAllAnchorTypes();

			/****************************************************************************
			*	@brief		��ȡê��
			*	@param[in]	i_eType				ê������
			*	@param[out]	o_pntAnchor			ê��
			*	@retval		STK_SUCCESS			��ȡ�ɹ�
			*	@retval		����				��ȡʧ��
			*	@note		��
			*	@attention	��
			****************************************************************************/
			STK_STATUS						GetAnchorPointByType(StkAnchorTypeEnum i_eType, STK_PNT32& o_pntAnchor);
			
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
			STK_STATUS					FromJson(void* i_DataP, void* i_JsonDocP);
		};
	}
}
#endif // !_STK_COMPOSITETEXT_H