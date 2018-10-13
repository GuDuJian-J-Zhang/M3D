// All Rights Reserved. Copyright (C) 2017 Hoteamsoft. 
/**************************************************************************
*	@class	Stk_MetaDataDocument
*	@brief	�Զ������Թ�����
*	@par ��ʷ��¼
*				2017/06/14 ������qinyp(Hoteamsoft)
**************************************************************************/

#ifndef _SVLLIB_METADATADOCUMENT_H_
#define _SVLLIB_METADATADOCUMENT_H_
#pragma once
#include "Stk_BaseDocument.h"

namespace HoteamSoft {
	namespace SVLLib {
		class Stk_MetaData;
		class Stk_MetaDataDocument : public Stk_BaseDocument
		{
			STK_DECLARE_CLASS(Stk_MetaDataDocument)
		public:
			Stk_MetaDataDocument(void);
			virtual ~Stk_MetaDataDocument(void);
		protected:
			enum { _typeID = _SVLLIB_TYPE_METADATADOCUMENT };	//!< ���

		public:
			/****************************************************************************
			*	@brief		��ȡָ������������б�
			*	@param[in]	modelID			���ID
			*	@param[out]	��
			*	@retval		vector<Stk_MetaDataPtr>			��ͼ�б�
			*	@note		��
			*	@attention
			****************************************************************************/
			vector<Stk_MetaDataPtr>&			GetMetaDataListByModelId(unsigned int modelID);

			/****************************************************************************
			*	@brief		�������ID������ID��ȡ����ָ��
			*	@param[in]	modelID			���ID
			*	@param[in]	metadataD	    ����ID
			*	@param[out]	��
			*	@retval		Stk_MetaDataPtr		����ָ��
			*	@note		��
			*	@attention
			****************************************************************************/
			Stk_MetaDataPtr			GetMetaData(unsigned int modelID, unsigned int metadataID);

			/****************************************************************************
			*	@brief		�������
			*	@param[in]	modelID			���ID
			*	@param[in]	metadataID			����ID
			*	@param[out]	��
			*	@retval		STK_SUCCESS			д��ɹ�
			*	@retval		����				д��ʧ��
			*	@note		��
			*	@attention
			****************************************************************************/
			STK_STATUS          AddMetaData(unsigned int modelID, Stk_MetaDataPtr metadataP);

			/****************************************************************************
			*	@brief		�������ID������IDɾ������
			*	@param[in]	modelID			���ID
			*	@param[in]	metadataID			����ID
			*	@param[out]	��
			*	@retval		STK_SUCCESS			д��ɹ�
			*	@retval		����				д��ʧ��
			*	@note		��
			*	@attention
			****************************************************************************/
			STK_STATUS          DelMetaData(unsigned int modelID, unsigned int metadataID);
		};
	}
}

#endif //_SVLLIB_METADATADOCUMENT_H_