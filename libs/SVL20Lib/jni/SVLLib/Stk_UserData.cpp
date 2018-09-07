// All Rights Reserved. Copyright (C) 2017 HOTEAMSOFT, Ltd
/****************************************************************************
*	@file	Stk_UserData.cpp
*
*	@brief	”√ªß ˝æ›¿‡
*
*	@par	¿˙ ∑:
*		2017/06/10	¥¥Ω®°£qinyp@HOTEAMSOFT
****************************************************************************/

#include "Stk_UserData.h"
#include "Stk_ObjectImpl.h"
#include "ChineseCode.h"
#include "Stk_BaseDocumentImpl.h"

#define  USERDATA_NODE_NAME_NAME "name"
#define  USERDATA_NODE_NAME_ID   "id"
#define  USERDATA_NODE_NAME_FILENAME "fileName"
#define  USERDATA_NODE_NAME_DATASIZE "dataSize"
#define  USERDATA_NODE_NAME_DATA     "data"

namespace HoteamSoft {
	namespace SVLLib {
        
        class Stk_UserDataImpl : public Stk_ObjectImpl
		{
			STK_CLASS_IMPL_DECLARE(Stk_UserData)
		public:
			Stk_UserDataImpl(void);
			virtual ~Stk_UserDataImpl(void);
		public:
			wstring                     m_wstrFileName;        //Œƒº˛√˚≥∆
			wstring                     m_wstrDataName;        // ˝æ›√˚≥∆
			STK_ID                      m_uiID;                //ID
			STK_UINT32                  m_uiDataSize;          // ˝æ›≥§∂»
			STK_CHAR                    *m_pszData;            // ˝æ›ƒ⁄»›
		public:
			/****************************************************************************
			*	@brief		∂¡»°Œƒº˛÷–µƒBody ˝æ›
			*	@param[in]	i_pJsonNode			View ∏∏JsonΩ⁄µ„
			*	@param[out]	Œﬁ
			*	@retval		STK_SUCCESS			∂¡»°≥…π¶
			*	@retval		STK_ERROR			∂¡»° ß∞‹
			*	@retval		STK_ERROR_MEMORY	ƒ⁄¥Ê…Í«Î ß∞‹
			*	@note		Œﬁ
			*	@attention	ƒ⁄≤øΩ”ø⁄
			****************************************************************************/
			STK_STATUS					FromJson(Value* i_pJsonNode);

			/*****************************************************************************
			*	@brief		–¥View ˝æ›
			*	@param[in]	Value* i_pJsonParentNode			View ∏∏JsonΩ⁄µ„
			*	@param[out]	Œﬁ
			*	@retval		STK_SUCCESS			–¥»Î≥…π¶
			*	@retval		∆‰À¸				–¥»Î ß∞‹
			*	@note		Œﬁ
			*	@attention	ƒ⁄≤øΩ”ø⁄
			****************************************************************************/
			STK_STATUS					ToJson(Value* i_pJsonNode, Document* pDoc);
		};

		STK_STATUS	Stk_UserDataImpl::ToJson(Value* i_pJsonNode, Document* pDoc)
		{
			STK_STATUS			ret = STK_SUCCESS;
			if (!i_pJsonNode || !pDoc)
				return STK_ERROR;
			i_pJsonNode->SetObject();
			//ID
			i_pJsonNode->AddMember(USERDATA_NODE_NAME_ID, m_uiID, pDoc->GetAllocator());
			string strValue;
			// ˝æ›√˚
			strValue = ChineseCode::WStringToString(m_wstrDataName.c_str());
			i_pJsonNode->AddMember(USERDATA_NODE_NAME_NAME, strValue, pDoc->GetAllocator());
			//Œƒº˛√˚
			strValue = ChineseCode::WStringToString(m_wstrFileName.c_str());
			i_pJsonNode->AddMember(USERDATA_NODE_NAME_FILENAME, strValue, pDoc->GetAllocator());
			//dataSize
			i_pJsonNode->AddMember(USERDATA_NODE_NAME_DATASIZE, m_uiDataSize, pDoc->GetAllocator());
			//data
			i_pJsonNode->AddMember(USERDATA_NODE_NAME_DATA, (string)m_pszData, pDoc->GetAllocator());
			return ret;
		}

		STK_STATUS	Stk_UserDataImpl::FromJson(Value* i_pJsonNode)
		{
			STK_STATUS			ret = STK_SUCCESS;
			if (!i_pJsonNode)
				return STK_ERROR;
			m_uiID = (*i_pJsonNode)[USERDATA_NODE_NAME_ID].GetInt();
			m_wstrDataName = ChineseCode::StringToWString((*i_pJsonNode)[USERDATA_NODE_NAME_NAME].GetString());
			m_wstrFileName = ChineseCode::StringToWString((*i_pJsonNode)[USERDATA_NODE_NAME_FILENAME].GetString());
			m_uiDataSize = (*i_pJsonNode)[USERDATA_NODE_NAME_DATASIZE].GetInt();
			m_pszData = (STK_CHAR *)(*i_pJsonNode)[USERDATA_NODE_NAME_DATA].GetString();
			return ret;
		}

		STK_CLASS_IMPL_IMPLEMENT(Stk_UserData)

		Stk_UserDataImpl::Stk_UserDataImpl(void)
		{
			m_wstrDataName = L"";
			m_wstrFileName = L"";
			m_uiID = 0;
			m_pszData = NULL;
			m_uiDataSize = 0;
		}

		Stk_UserDataImpl::~Stk_UserDataImpl(void)
		{

		}

		STK_IMPLEMENT_CLASS(Stk_UserData, Stk_Object)
		
		Stk_UserData::Stk_UserData() : Stk_Object(new Stk_UserDataImpl())
		{
		}


		Stk_UserData::~Stk_UserData()
		{
		}

		STK_STATUS	Stk_UserData::SetUserData(wstring i_wstrDataName, STK_ID i_id,
			STK_CHAR *i_dataP, STK_SIZE_T i_dataSize, wstring i_wstrFileName)
		{
			((Stk_UserDataImpl*)m_pImpl)->m_wstrDataName = i_wstrDataName;
			((Stk_UserDataImpl*)m_pImpl)->m_uiDataSize = i_dataSize;
			((Stk_UserDataImpl*)m_pImpl)->m_wstrFileName = i_wstrFileName;
			((Stk_UserDataImpl*)m_pImpl)->m_uiID = i_id;

			if (i_dataSize > 0)
			{
				((Stk_UserDataImpl*)m_pImpl)->m_pszData = (STK_CHAR *)malloc(i_dataSize);
				if (((Stk_UserDataImpl*)m_pImpl)->m_pszData == NULL)
				{
					return STK_ERROR;
				}
				memcpy(((Stk_UserDataImpl*)m_pImpl)->m_pszData, i_dataP, i_dataSize);
			}

			return STK_SUCCESS;
		}

		STK_STATUS Stk_UserData::GetUserData(wstring &o_wstrDataName, STK_ID &o_id,
			STK_CHAR* &o_dataP, STK_SIZE_T &o_dataSize, wstring &o_wstrFileName)
		{
			o_wstrDataName = ((Stk_UserDataImpl*)m_pImpl)->m_wstrDataName;
			o_id = ((Stk_UserDataImpl*)m_pImpl)->m_uiID;
			o_dataSize = ((Stk_UserDataImpl*)m_pImpl)->m_uiDataSize;
			o_wstrFileName = ((Stk_UserDataImpl*)m_pImpl)->m_wstrFileName;

			if (((Stk_UserDataImpl*)m_pImpl)->m_uiDataSize > 0)
			{
				o_dataP = (STK_CHAR *)malloc(((Stk_UserDataImpl*)m_pImpl)->m_uiDataSize);
				if (((Stk_UserDataImpl*)m_pImpl)->m_pszData == NULL)
				{
					return STK_ERROR;
				}
				memcpy(o_dataP, ((Stk_UserDataImpl*)m_pImpl)->m_pszData, ((Stk_UserDataImpl*)m_pImpl)->m_uiDataSize);
			}

			return STK_SUCCESS;
		}

		STK_STATUS	Stk_UserData::ToJson(void* i_DataP, void* pDoc)
		{
			return ((Stk_UserDataImpl*)m_pImpl)->ToJson((Value*)i_DataP, (Document*)pDoc);
		}

		STK_STATUS	Stk_UserData::FromJson(void* i_DataP)
		{
			return ((Stk_UserDataImpl*)m_pImpl)->FromJson((Value*)i_DataP);
		}
	}
}
