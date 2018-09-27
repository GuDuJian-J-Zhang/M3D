// All Rights Reserved. Copyright (C) 2017 Hoteamsoft. 
/**************************************************************************
 *	@class	Stk_BaseDocument
 *	@brief	Œƒµµπ‹¿Ìª˘¿‡
 *	@par ¿˙ ∑º«¬º
 *				2017/06/09 ¥¥Ω®°£likun(Hoteamsoft)
**************************************************************************/
#include <fstream>
#include "Stk_BaseDocument.h"
#include "Stk_BaseDocumentImpl.h"
#include "ChineseCode.h"



using namespace std;

namespace HoteamSoft {
    namespace SVLLib {
		STK_IMPLEMENT_CLASS(Stk_BaseDocument, Stk_Object)
		Stk_BaseDocument::Stk_BaseDocument(Stk_BaseDocumentImpl *pImpl)
		{
			m_pImpl = pImpl;
			m_pImpl->m_pDocument = NULL;
			m_pImpl->m_pPubl = this;
		}

		Stk_BaseDocument::~Stk_BaseDocument(void)
		{
			if (m_pImpl)
			{
				delete m_pImpl;
				m_pImpl = NULL;
			}
		}

		void Stk_BaseDocument::SetDocument(Stk_Document* pDocument)
		{
			m_pImpl->m_pDocument = pDocument;
		}

		Stk_Document* Stk_BaseDocument::GetDocument()
		{
			return m_pImpl->m_pDocument;
		}

		const std::wstring Stk_BaseDocument::GetFileExt(const std::wstring& strFilePath)
		{
			wstring strExt = m_pImpl->GetFileExt(strFilePath);
			return strExt;
		}

		STK_STATUS Stk_BaseDocument::OpenFile(const std::wstring& strFilePath)
		{
			STK_STATUS result = STK_ERROR;
			if (!strFilePath.empty()) {
				m_pImpl->m_strFilePath = strFilePath;
			}
			//∂¡Œƒº˛
			wstring strExt = GetFileExt(m_pImpl->m_strFilePath);
			if (strExt == L"json" || strExt == L"pmi" || strExt == L"model" || strExt == L"view")
			{
				result = m_pImpl->LoadJsonFile(m_pImpl->m_strFilePath);
			}
			return result;
		}

		STK_STATUS Stk_BaseDocument::WriteFile(const std::wstring& strFilePath)
		{
			STK_STATUS result = STK_SUCCESS;
			
			if (!strFilePath.empty()) {
				m_pImpl->m_strFilePath = strFilePath;
			}
			wstring outputFile = m_pImpl->m_strFilePath;
			//–¥Œƒº˛
			wstring strExt = GetFileExt(m_pImpl->m_strFilePath);
		/*	if (_tcsicmp(strExt.c_str(), L"json") == 0)
			{*/
				result = m_pImpl->WriteJsonFile(m_pImpl->m_strFilePath);
			//}
			return result;
		}

		void Stk_BaseDocument::SetFilePath(const std::wstring& strFilePath)
		{ 
			m_pImpl->m_strFilePath = strFilePath; 
		}

		STK_STATUS Stk_BaseDocument::ToJsonString(string& strJson)
		{
			return  m_pImpl->ToJsonString(strJson);
		}

		void* Stk_BaseDocument::GetJsonDocP()
		{
			return &m_pImpl->m_jsonDoc;
		}

		void Stk_BaseDocument::unLoad()
		{
			m_pImpl->unLoad();
		}

	}//svllib
}//hoteamsoft
