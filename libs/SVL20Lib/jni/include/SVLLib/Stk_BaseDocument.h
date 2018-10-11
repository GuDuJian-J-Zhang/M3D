// All Rights Reserved. Copyright (C) 2017 Hoteamsoft. 
/**************************************************************************
*	@class	Stk_BaseDocument
*	@brief	�ĵ��������
*	@par ��ʷ��¼
*				2017/06/09 ������likun(Hoteamsoft)
**************************************************************************/
#ifndef _SVLLIB_BASEDOCUMENT_H_
#define _SVLLIB_BASEDOCUMENT_H_

#include "Stk_API.h"


using namespace std;

namespace HoteamSoft {
	namespace SVLLib {
		class Stk_BaseDocumentImpl;
		class Stk_BaseDocument : public Stk_Object
		{
			STK_DECLARE_CLASS(Stk_BaseDocument)
		protected:
			enum { _typeID = _SVLLIB_TYPE_DOCUMENT };	//!< ���
		public:
			Stk_BaseDocument(Stk_BaseDocumentImpl *pImpl);
			virtual ~Stk_BaseDocument(void);
		
			
		protected:
			Stk_BaseDocumentImpl *m_pImpl;			//!< ��������

		public:
			/****************************************************************************
			*	@brief		��ȡ�����ļ��������
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		�����ļ��������ָ��
			*	@note		��
			*	@attention
			****************************************************************************/
			Stk_Document* GetDocument();
			void SetDocument(Stk_Document* pDocument);
			/****************************************************************************
			*	@brief		��ȡ�ļ�����չ��
			*	@param[in]	strFilePath �ļ����ƻ�·��
			*	@param[out]	��
			*	@retval		�ļ�����չ��
			*	@note		��
			*	@attention
			****************************************************************************/
			const std::wstring GetFileExt(const std::wstring& strFilePath);
		
			/****************************************************************************
			*	@brief		�����ļ�
			*	@param[in]	strFilePath	�ļ�·��
			*	@param[out]	��
			*	@retval		STK_SUCCESS			д��ɹ�
			*	@retval		����				д��ʧ��
			*	@note		��
			*	@attention
			****************************************************************************/
			STK_STATUS	WriteFile(const std::wstring& strFilePath);

			/****************************************************************************
			*	@brief		��ȡ�ļ�
			*	@param[in]	strFilePath	�ļ�·��
			*	@param[out]	��
			*	@retval		STK_SUCCESS			��ȡ�ɹ�
			*	@retval		����				��ȡʧ��
			*	@note		��
			*	@attention
			****************************************************************************/
			STK_STATUS	OpenFile(const std::wstring& strFilePath);

			/****************************************************************************
			*	@brief		���õ�ǰ�ļ�·��
			*	@param[in]	strFilePath	�ļ�·��
			*	@param[out]	��
			*	@retval		��
			*	@note		��
			*	@attention
			****************************************************************************/
			void		SetFilePath(const std::wstring& strFilePath);

			/****************************************************************************
			*	@brief		����Json��ʽ�ַ���
			*	@param[in]	��
			*	@param[out]	strJson �ַ�������
			*	@retval		STK_SUCCESS			�ɹ�
			*	@retval		����				ʧ��
			*	@note		��
			*	@attention
			****************************************************************************/
			STK_STATUS ToJsonString(string& strJson);

			/****************************************************************************
			*	@brief		��ȡJson �ĵ�ָ��
			*	@param[out]	��
			*	@retval		void*
			*	@note		��
			*	@attention
			****************************************************************************/
			void* GetJsonDocP();

			void  unLoad();
		};
	}//svllib
}//hoteamsoft

#endif
