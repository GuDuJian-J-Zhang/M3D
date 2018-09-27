// All Rights Reserved. Copyright (C) 2017 Hoteamsoft. 
/**************************************************************************
 *	@class	Stk_BaseDocumentImpl
 *	@brief	Œƒµµπ‹¿Ìª˘¿‡
 *	@par ¿˙ ∑º«¬º
 *				2017/06/09 ¥¥Ω®°£likun(Hoteamsoft)
**************************************************************************/
#include <fstream>
#include "Stk_BaseDocumentImpl.h"
#include "ChineseCode.h"

namespace HoteamSoft {
	namespace SVLLib {

		Stk_BaseDocumentImpl::Stk_BaseDocumentImpl()
		{
			m_jsonDoc.SetObject();
			m_pDocument = NULL;
			m_pPubl = NULL;
			m_strVersion = L"";
		}

		Stk_BaseDocumentImpl::~Stk_BaseDocumentImpl(void)
		{

		}

		const std::wstring Stk_BaseDocumentImpl::GetFileExt(const std::wstring& strFilePath)
		{
			wstring strExt = L"";
			int iWhich = strFilePath.find_last_of('.');
			if (iWhich == -1)
				return strExt;
			strExt = strFilePath.substr(iWhich + 1, strFilePath.size() - iWhich - 1);
			return strExt;
		}

		STK_STATUS Stk_BaseDocumentImpl::ToJsonString(string& strJson)
		{
			STK_STATUS result = STK_ERROR;
			strJson = "";
			//∏˘æ›Œƒº˛ƒ⁄»›ÃÓ–¥JsonŒƒµµµƒƒ⁄»›£¨∏√Ω”ø⁄ª·‘⁄∏˜◊”¿‡÷–÷ÿ–¥“‘ µœ÷∏˜◊‘µƒπ¶ƒ‹
			result = ToJson(m_jsonDoc);
			if (result != STK_SUCCESS)
				return result;

			StringBuffer buffer;
			Writer<StringBuffer> writer(buffer);
			m_jsonDoc.Accept(writer);
			strJson.append(buffer.GetString(), buffer.GetSize());
			
			m_jsonDoc.RemoveAllMembers();
			return result;
		}

		STK_STATUS Stk_BaseDocumentImpl::FromJsonString(string& strJson)
		{
			STK_STATUS result = STK_SUCCESS;
			m_jsonDoc.Parse<0>(strJson.c_str());

			if (m_jsonDoc.HasParseError()) {
				return STK_ERROR;
            }
            if (!m_jsonDoc.IsObject()) {
				return STK_ERROR;
			}
			FromJson(m_jsonDoc);
			m_jsonDoc.RemoveAllMembers();
			return result;
		}

		STK_STATUS Stk_BaseDocumentImpl::WriteJsonFile(const std::wstring& strFilePath)
		{
			STK_STATUS result = STK_SUCCESS;
			//≈–∂œ¿©’π√˚ «∑Ò’˝»∑
			int iWhich = strFilePath.find_last_of('.');
			if (-1 == iWhich) {
				return STK_ERROR_FILERD;
			}
			wstring strExt = GetFileExt(strFilePath);
			if (strExt.empty()/* || _tcsicmp(strExt.c_str(), L"json") != 0*/)
			{
				return STK_ERROR_FILERD;
			}
			wstring outputFile = strFilePath;
			//–¥Œƒº˛

			//∏˘æ›Œƒº˛ƒ⁄»›ÃÓ–¥JsonŒƒµµµƒƒ⁄»›£¨∏√Ω”ø⁄ª·‘⁄∏˜◊”¿‡÷–÷ÿ–¥“‘ µœ÷∏˜◊‘µƒπ¶ƒ‹
			string strJson = "";
			result = ToJsonString(strJson);
			if (result != STK_SUCCESS)
				return result;
			std::ofstream ofile;
#ifdef __MOBILE__
			string strPath;
			ChineseCode::UnicodeToUTF_8(outputFile.c_str(), strPath);
			ofile.open(strPath.c_str());
#else
			ofile.open(outputFile.c_str());
#endif
			if (ofile.fail())
			{
				return STK_ERROR_FILEWR;
			}
			//char UTF8BOM[4] = { '\xEF','\xBB','\xBF','\x0' };
			//ofile << UTF8BOM;
			ofile << strJson;
			ofile.close();
			return result;
		}

		void Stk_BaseDocumentImpl::unLoad()
		{
			
		}

		STK_STATUS Stk_BaseDocumentImpl::LoadJsonFile(const std::wstring& strFilePath)
		{
			STK_STATUS result = STK_SUCCESS;
			MutexLock locked(this->m_mutexDocRead);

			//≈–∂œ¿©’π√˚ «∑Ò’˝»∑
			wstring strExt = GetFileExt(strFilePath);
			if (strExt.empty() || strExt == L"json")
			{
				return STK_ERROR_FILERD;
			}

			//º”‘ÿJsonŒƒº˛¡˜
			string strStream;
			std::ifstream ifile;
#ifdef __MOBILE__
			string strPath;
			ChineseCode::UnicodeToUTF_8(strFilePath.c_str(), strPath);
			ifile.open(strPath.c_str(), ios_base::in);
#else
			ifile.open(strFilePath.c_str(), ios_base::in);
#endif

			if (!ifile.is_open())
			{
				return STK_ERROR;
			}
			string strLine;
			while (getline(ifile, strLine))
			{
				strStream.append(strLine + "\n");
			}
			ifile.close();
			result = FromJsonString(strStream);
			if(result != STK_SUCCESS)
			{
				return result;
			}

			return STK_SUCCESS;
		}

	}//svllib
}//hoteamsoft
