// All Rights Reserved. Copyright (C) 2017 HOTEAMSOFT, Ltd
/****************************************************************************
*	@file	Stk_Attribute.h
*
*	@brief	 Ù–‘¿‡
*
*	@par	¿˙ ∑:
*		2017/07/03	¥¥Ω®°£qinyp@HOTEAMSOFT
****************************************************************************/

#include "Stk_Attribute.h"
#include "Stk_ObjectImpl.h"
#include "Stk_AttributeCollection.h"
#include "Stk_DocumentImpl.h"
#include "ChineseCode.h"

#define  INSTANCEATTRIBUTE_NODE_NAME_ID  "id"
#define  INSTANCEATTRIBUTE_NODE_NAME_NAME    "name"
#define  INSTANCEATTRIBUTE_NODE_NAME_ATTRIBUTES "attributes"

#define  MODELATTRIBUTE_NODE_NAME_ID     "id"
#define  MODELATTRIBUTE_NODE_NAME_NAME   "name"
#define  MODELATTRIBUTE_NODE_NAME_ATTRIBUTES "attributes"

namespace HoteamSoft {
    namespace SVLLib {
        class Stk_AttributeCollection;
		class Stk_AttributeImpl : public Stk_ObjectImpl
		{
			STK_CLASS_IMPL_DECLARE(Stk_Attribute)
		public:
			Stk_AttributeImpl(Stk_AttributeCollection* i_attrDocP, int i_iModelID);
			virtual ~Stk_AttributeImpl(void);
		private:
			//Stk_AttributeCollectionPtr m_pAttributeDoc;
			//StkAttributeOwnerType    m_eAttributeOwnerType;
			//STK_UINT32		         m_iOwnerID;				//À˘ Ù Ù÷˜
			//map<STK_ID, map<string, string>>          m_mapInsAttr; // µ¿˝ Ù–‘
			//map<STK_ID, map<string, string>>          m_mapModelAttr;//ƒ£–Õ Ù–‘
		};

		Stk_AttributeImpl::Stk_AttributeImpl(Stk_AttributeCollection* i_attrDocP, int i_iOwnerID)
		{
			//m_pAttributeDoc = i_attrDocP;
		}

		Stk_AttributeImpl::~Stk_AttributeImpl()
		{

		}

		STK_CLASS_IMPL_IMPLEMENT(Stk_Attribute)

		Stk_Attribute::Stk_Attribute() :Stk_Object(new Stk_AttributeImpl(NULL, 0))
		{

		}

		Stk_Attribute::~Stk_Attribute()
		{
			DELETE_STK_CLASS_IMPL
		}
		STK_IMPLEMENT_CLASS(Stk_Attribute, Stk_Object)

		/*StkAttributeOwnerType Stk_Attribute::GetAttributeOwnerType()
		{
			return ((Stk_AttributeImpl*)m_pImpl)->m_eAttributeOwnerType;
		}

		STK_STATUS Stk_Attribute::SetAttributeOwnerType(const StkAttributeOwnerType& i_eAttributeOwnerType)
		{
			((Stk_AttributeImpl*)m_pImpl)->m_eAttributeOwnerType = i_eAttributeOwnerType;
			return STK_SUCCESS;
		}*/

		void Stk_Attribute::SetName(const wstring& i_wstrName)
		{

		}

		wstring  Stk_Attribute::GetName()
		{
			return L"";
		}

		STK_STATUS	Stk_Attribute::ToJson(void* i_DataP, void* i_JsonDocP)
		{
			STK_STATUS			ret = STK_SUCCESS;
			if (!i_DataP || !i_JsonDocP)
				return STK_ERROR;
			return ret;
		}

		STK_STATUS	Stk_Attribute::FromJson(void* i_DataP, void* i_JsonDocP)
		{
			STK_STATUS			ret = STK_SUCCESS;
			if (!i_DataP || !i_JsonDocP)
				return STK_ERROR;
			return ret;
		}

	}
}
