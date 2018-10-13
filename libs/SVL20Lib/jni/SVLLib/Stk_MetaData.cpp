// All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
/*****************************************************************************
*	@file	Stk_MetaData.cpp
*
****************************************************************************/
#include "Stk_ObjectImpl.h"
#include "Stk_MetaData.h"
#include "Stk_DocumentImpl.h"
#include "Stk_Document.h"
#include "Stk_MetaDataDocument.h"
#include "Stk_Declare_Ptr.h"
#include "ChineseCode.h"

#define  METADATA_NODE_NAME_NAME "name"
#define  METADATA_NODE_NAME_VALUE "value"
#define  METADATA_NODE_NAME_TYPE  "type"
#define  METADATA_NODE_NAME_UNIT  "unit"

namespace HoteamSoft {
	namespace SVLLib {
		class Stk_MetaDataImpl : public Stk_ObjectImpl
        {
            STK_CLASS_IMPL_DECLARE(Stk_MetaData)
		public:
			Stk_MetaDataImpl(Stk_MetaDataDocument* i_MetaDataDocP, int i_iPartID);
			virtual ~Stk_MetaDataImpl(void);
		public:
			Stk_MetaDataDocumentPtr   m_pMetaDataDoc;						//÷∏’Î
			STK_UINT32		  m_iPartID;				            //À˘ Ù¡„º˛
			wstring m_wstrVersion;									// ∞Ê±æ∫≈
			STK_UINT32 m_uiID;										//ID
			wstring m_wstrName;										// Ù–‘√˚≥∆	
			wstring m_wstrValue;								    // Ù–‘÷µ
			StkMetaDataValueTypeEnum m_eValueType;					// Ù–‘¿‡–Õ
			wstring m_wstrUnit;										// Ù–‘µ•‘™
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
			STK_STATUS					ToJson(Value* i_pJsonNode);
		};
		
		STK_CLASS_IMPL_IMPLEMENT(Stk_MetaData)

		STK_STATUS	Stk_MetaDataImpl::FromJson(Value* i_pJsonNode)
		{
			STK_STATUS			ret = STK_SUCCESS;
			if (!i_pJsonNode)
				return STK_ERROR;
			ChineseCode::UTF_8ToUnicode(m_wstrName, (*i_pJsonNode)[METADATA_NODE_NAME_NAME].GetString());
			ChineseCode::UTF_8ToUnicode(m_wstrValue, (*i_pJsonNode)[METADATA_NODE_NAME_VALUE].GetString());
			m_eValueType = (StkMetaDataValueTypeEnum)(*i_pJsonNode)[METADATA_NODE_NAME_TYPE].GetInt();
			ChineseCode::UTF_8ToUnicode(m_wstrUnit, (*i_pJsonNode)[METADATA_NODE_NAME_UNIT].GetString());
			return STK_SUCCESS;
		}

		STK_STATUS Stk_MetaDataImpl::ToJson(Value* i_pJsonNode)
		{
			STK_STATUS			ret = STK_ERROR;
			if (!i_pJsonNode)
				return ret;
			Document* pDoc = (Document*)m_pMetaDataDoc->GetJsonDocP();
			i_pJsonNode->SetObject();
			
			string strUTF8;
			ChineseCode::UnicodeToUTF_8(m_wstrName.c_str(), strUTF8);
			i_pJsonNode->AddMember(METADATA_NODE_NAME_NAME, strUTF8, pDoc->GetAllocator());
			ChineseCode::UnicodeToUTF_8(m_wstrValue.c_str(), strUTF8);
			i_pJsonNode->AddMember(METADATA_NODE_NAME_VALUE, strUTF8, pDoc->GetAllocator());
			i_pJsonNode->AddMember(METADATA_NODE_NAME_TYPE, (int)m_eValueType, pDoc->GetAllocator());
			ChineseCode::UnicodeToUTF_8(m_wstrUnit.c_str(), strUTF8);
			i_pJsonNode->AddMember(METADATA_NODE_NAME_UNIT, strUTF8, pDoc->GetAllocator());
			return STK_SUCCESS;
		}

		Stk_MetaDataImpl::Stk_MetaDataImpl(Stk_MetaDataDocument* i_ViewDocP, int i_iPartID)
		{
			m_pMetaDataDoc = i_ViewDocP;
			m_iPartID = i_iPartID;
			m_eValueType = StkMetaDataValueTypeEnum::TypeValueUnknown;
		}

		Stk_MetaDataImpl::~Stk_MetaDataImpl(void)
		{
			if (m_pMetaDataDoc != nullptr)
			{
				m_pMetaDataDoc->DelMetaData(m_iPartID, m_ID);
			}
		}

		STK_IMPLEMENT_CLASS(Stk_MetaData, Stk_Object)
		
		Stk_MetaData::Stk_MetaData():Stk_Object(new Stk_MetaDataImpl(NULL, 0))
		{
		}

		Stk_MetaData::~Stk_MetaData()
		{
			DELETE_STK_CLASS_IMPL
		}

		void Stk_MetaData::Init(Stk_MetaDataDocument* pDoc, unsigned int parentID/*, bool bForWrite = true*/)
		{
			((Stk_MetaDataImpl*)m_pImpl)->m_pMetaDataDoc = pDoc;
			((Stk_MetaDataImpl*)m_pImpl)->m_iPartID = parentID;
		}

		STK_UINT32  Stk_MetaData::GetID()
		{
			return ((Stk_MetaDataImpl*)m_pImpl)->m_uiID;
		}

		STK_STATUS Stk_MetaData::SetName(wstring i_wstrName)
		{
			((Stk_MetaDataImpl*)m_pImpl)->m_wstrName = i_wstrName;
			return STK_SUCCESS;
		}

		wstring	Stk_MetaData::GetName()
		{
			return ((Stk_MetaDataImpl*)m_pImpl)->m_wstrName;
		}

		STK_STATUS Stk_MetaData::SetType(StkMetaDataValueTypeEnum i_eValueType)
		{
			((Stk_MetaDataImpl*)m_pImpl)->m_eValueType = i_eValueType;
			return STK_SUCCESS;
		}

		StkMetaDataValueTypeEnum Stk_MetaData::GetType()
		{
			return ((Stk_MetaDataImpl*)m_pImpl)->m_eValueType;
		}

		STK_STATUS Stk_MetaData::SetValue(wstring i_wstrValue)
		{
			((Stk_MetaDataImpl*)m_pImpl)->m_wstrValue = i_wstrValue;
			return STK_SUCCESS;
		}

		wstring Stk_MetaData::GetValue()
		{
			return ((Stk_MetaDataImpl*)m_pImpl)->m_wstrValue;
		}

		STK_STATUS Stk_MetaData::SetUnit(wstring i_wstrUnit)
		{
			((Stk_MetaDataImpl*)m_pImpl)->m_wstrUnit = i_wstrUnit;
			return STK_SUCCESS;
		}

		wstring	Stk_MetaData::GetUnit()
		{
			return ((Stk_MetaDataImpl*)m_pImpl)->m_wstrUnit;
		}
	}
}
