// All Rights Reserved. Copyright (C) 2017 Hoteamsoft. 
/**************************************************************************
*	@class	Stk_ViewDocument
*	@brief	��ͼ�ĵ�������
*	@par ��ʷ��¼
*				2017/06/09 ������likun(Hoteamsoft)
**************************************************************************/
#ifndef _SVLLIB_VIEWDOCUMENT_H_
#define _SVLLIB_VIEWDOCUMENT_H_
#include "Stk_BaseDocument.h"
#include "Stk_ObjectImpl.h"
//#include "Stk_View.h"

namespace HoteamSoft {
	namespace SVLLib {
		class Stk_View;
		class Stk_ViewDocument : public Stk_BaseDocument
		{
			STK_DECLARE_CLASS(Stk_ViewDocument)
		protected:
			enum { _typeID = _SVLLIB_TYPE_VIEWDOCUMENT };	//!< ���
		public:
			Stk_ViewDocument();
			virtual ~Stk_ViewDocument(void);
		public:
			void                       SetVersion(const wstring wstrVersion);
			const   wstring            GetVersion();
			/****************************************************************************
			*	@brief		��ȡָ���������ͼ�б�
			*	@param[in]	modelID			���ID
			*	@param[out]	��
			*	@retval		vector<Stk_ViewPtr>			��ͼ�б�
			*	@note		��
			*	@attention
			****************************************************************************/
			void GetViewListByModelId(unsigned int modelID, vector<Stk_ViewPtr>& vcViews);

			/****************************************************************************
			*	@brief		�������ID����ͼID��ȡ��ͼָ��
			*	@param[in]	modelID			���ID
			*	@param[in]	viewID			��ͼID
			*	@param[out]	��
			*	@retval		Stk_ViewPtr		��ͼָ��
			*	@note		��
			*	@attention
			****************************************************************************/
			Stk_ViewPtr			GetView(unsigned int modelID, unsigned int viewID);

			/****************************************************************************
			*	@brief		�����ͼ
			*	@param[in]	modelID			���ID
			*	@param[in]	viewP			��ͼ
			*	@param[out]	��
			*	@retval		STK_SUCCESS			д��ɹ�
			*	@retval		����				д��ʧ��
			*	@note		��
			*	@attention
			****************************************************************************/
			STK_STATUS          AddView(unsigned int modelID, Stk_ViewPtr viewP);

			/****************************************************************************
			*	@brief		�����ͼ
			*	@param[in]	viewP			��ͼ
			*	@param[out]	��
			*	@retval		STK_SUCCESS			д��ɹ�
			*	@retval		����				д��ʧ��
			*	@note		��
			*	@attention
			****************************************************************************/
			STK_STATUS          AddView(Stk_ViewPtr viewP);

			/****************************************************************************
			*	@brief		�����ͼ
			*	@param[in]	modelID			���ID
			*	@param[in]	vcViews		    View ID����
			*	@param[out]	��
			*	@retval		STK_SUCCESS			��ӳɹ�
			*	@retval		����				���ʧ��
			*	@note		��
			*	@attention
			****************************************************************************/
			STK_STATUS          AddViews(unsigned int modelID, vector<STK_ID> vcViews);

			/****************************************************************************
			*	@brief		�������ID����ͼIDɾ����ͼ
			*	@param[in]	modelID			���ID
			*	@param[in]	viewID			��ͼID
			*	@param[out]	��
			*	@retval		STK_SUCCESS			д��ɹ�
			*	@retval		����				д��ʧ��
			*	@note		��
			*	@attention
			****************************************************************************/
			STK_STATUS          DelView(unsigned int modelID, unsigned int viewID);

			map<STK_ID, Stk_ViewPtr>            GetViewMap();

			/****************************************************************************
			*	@brief		ж����ͼ
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		����				ɾ��ʧ��
			*	@attention
			****************************************************************************/
			void UnLoad();
		};
	}//svllib
}//hoteamsoft

#endif
