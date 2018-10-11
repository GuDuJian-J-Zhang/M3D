// All Rights Reserved. Copyright (C) 2017 Hoteamsoft. 
/**************************************************************************
*	@class	Stk_PMIDocument
*	@brief	PMI������
*	@par ��ʷ��¼
*				2017/06/14 ������qinyp(Hoteamsoft)
**************************************************************************/

#ifndef _SVLLIB_PMIDOCUMENT_H_
#define _SVLLIB_PMIDOCUMENT_H_
#pragma once
#include "Stk_BaseDocument.h"

namespace HoteamSoft {
	namespace SVLLib {
		class Stk_PMI;
		class Stk_PMIDocument : public Stk_BaseDocument
		{
			STK_DECLARE_CLASS(Stk_PMIDocument)
		public:
			Stk_PMIDocument();
			virtual ~Stk_PMIDocument();
		protected:
			enum { _typeID = _SVLLIB_TYPE_PMIDOCUMENT };	//!< ���
			
		public:
			void                       SetVersion(const wstring wstrVersion);
			const   wstring            GetVersion();
			/****************************************************************************
			*	@brief		��ȡָ�������PMI�б�
			*	@param[in]	modelID			���ID
			*	@param[in][out]	vcPMIs			PMI�б�
			*	@note		��
			*	@attention
			****************************************************************************/
			void			GetPMIListByModelID(unsigned int modelID, vector<Stk_PMIPtr>& vcPMIs);

			/****************************************************************************
			*	@brief		�������ID��PMI ID��ȡPMIָ��
			*	@param[in]	modelID			���ID
			*	@param[in]	pmiID			PMI ID
			*	@param[out]	��
			*	@retval		Stk_PMIPtr		��ͼָ��
			*	@note		��
			*	@attention
			****************************************************************************/
			Stk_PMIPtr			GetPMI(unsigned int modelID, unsigned int pmiID);

			/****************************************************************************
			*	@brief		���PMI
			*	@param[in]	modelID			���ID
			*	@param[in]	pmiID		    PMI ID
			*	@param[out]	��
			*	@retval		STK_SUCCESS			��ӳɹ�
			*	@retval		����				���ʧ��
			*	@note		��
			*	@attention
			****************************************************************************/
			STK_STATUS          AddPMI(unsigned int modelID, Stk_PMIPtr pmiP);

			/****************************************************************************
			*	@brief		���PMI
			*	@param[in]	modelID			���ID
			*	@param[in]	vcPMI		    PMI ID����
			*	@param[out]	��
			*	@retval		STK_SUCCESS			��ӳɹ�
			*	@retval		����				���ʧ��
			*	@note		��
			*	@attention
			****************************************************************************/
			STK_STATUS          AddPMI(unsigned int modelID, vector<STK_ID> vcPMI);

			/****************************************************************************
			*	@brief		���PMI
			*	@param[in]	pmiID		    PMI ID
			*	@param[out]	��
			*	@retval		STK_SUCCESS			��ӳɹ�
			*	@retval		����				���ʧ��
			*	@note		��
			*	@attention
			****************************************************************************/
			STK_STATUS          AddPMI(Stk_PMIPtr pmiP);

			/****************************************************************************
			*	@brief		�������ID��PMI IDɾ��PMI
			*	@param[in]	modelID			���ID
			*	@param[in]	pmiID			PMI ID
			*	@param[out]	��
			*	@retval		STK_SUCCESS			ɾ���ɹ�
			*	@retval		����				ɾ��ʧ��
			*	@note		��
			*	@attention
			****************************************************************************/
			STK_STATUS          DelPMI(unsigned int modelID, unsigned int pmiID);
			
			/****************************************************************************
			*	@brief		ж��PMI
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		����				ɾ��ʧ��
			*	@attention
			****************************************************************************/
			void UnLoad();
		};
	}
}

#endif