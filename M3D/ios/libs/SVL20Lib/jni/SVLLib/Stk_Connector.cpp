#include "Stk_Connector.h"
#include "Stk_ObjectImpl.h"
#include "Stk_Model.h"
#include "Stk_BaseDocumentImpl.h"
#include "ChineseCode.h"
#include "StringTools.h"

#define CONNECTOR_NODE_NAME_ID "id"
#define CONNECTOR_NODE_NAME_POINTEDENTITYTYPE "pointedEntityType"
#define CONNECTOR_NODE_NAME_ISLINKPROTO "isLinkProto"
#define CONNECTOR_NODE_NAME_LINKPLCPATH  "linkPlcPath"
#define CONNECTOR_NODE_NAME_ISLINKENTITY "isLinkEntity"
#define CONNECTOR_NODE_NAME_LINKENTITY "linkEntity"
#define CONNECTOR_NODE_NAME_ISLINKENTITIES "isLinkEntities"
#define CONNECTOR_NODE_NAME_LINKENTITIES "linkEntities"

#define LINKENTITIES_NODE_NAME_ENTITYID "entityID"
#define LINKENTITIES_NODE_NAME_ENTITYTYPE "entityType"
#define LINKENTITIES_NODE_NAME_PICKPNT "pickPnt"
#define LINKENTITIES_NODE_NAME_PICKDIR "pickDir"

namespace HoteamSoft {
    namespace SVLLib {
        class Stk_ConnectorImpl : public Stk_ObjectImpl
		{
			STK_CLASS_IMPL_DECLARE(Stk_Connector)
		public:
			Stk_ConnectorImpl();
			virtual ~Stk_ConnectorImpl();
		private:
			Stk_ModelPtr				m_stkProtoP;			//!< À˘ Ù¡„º˛
			StkEntityTypeEnum			m_ePointingEntityType;	//!< “™Àÿ¿‡–Õ
			StkEntityTypeEnum			m_ePointedEntityType;	//!< ±ª÷∏œÚ“™Àÿµƒ¿‡–Õ
			STK_BOOL					m_bIsLinkProto;			//!<  «∑Ò÷∏œÚ∆‰À¸ProtoType
			wstring						m_wstrPlcPath;			//!< ≈‰÷√¬∑æ∂
			STK_BOOL					m_bIsLinkEntity;			//!<  «∑Ò÷∏œÚNode
			STK_ID						m_idEntity;				//!< NodeConnectorID
			STK_BOOL					m_bIsLinkEntities;		//!<  «∑Ò÷∏œÚNodeœ¬µƒ“™Àÿ
			vector<SubEntity>			m_vcEntities;			//!< Nodeœ¬µƒ“™Àÿ¡–±Ì£®¿˝»Á±ﬂ°¢√Ê°¢Õº≤„µ»£©
		public:
			/****************************************************************************
			*	@brief		∂¡»°Œƒº˛÷–µƒ ˝æ›
			*	@param[in]	i_pJsonNode		 ∏∏JsonΩ⁄µ„
			*	@param[out]	Œﬁ
			*	@retval		STK_SUCCESS			∂¡»°≥…π¶
			*	@retval		STK_ERROR			∂¡»° ß∞‹
			*	@retval		STK_ERROR_MEMORY	ƒ⁄¥Ê…Í«Î ß∞‹
			*	@note		Œﬁ
			*	@attention	ƒ⁄≤øΩ”ø⁄
			****************************************************************************/
			STK_STATUS					FromJson(Value* i_pJsonNode, Document*i_JsonDocP);

			/*****************************************************************************
			*	@brief		–¥ ˝æ›
			*	@param[in]	Value* i_pJsonNode			View ∏∏JsonΩ⁄µ„
			*	@param[out]	Œﬁ
			*	@retval		STK_SUCCESS			–¥»Î≥…π¶
			*	@retval		∆‰À¸				–¥»Î ß∞‹
			*	@note		Œﬁ
			*	@attention	ƒ⁄≤øΩ”ø⁄
			****************************************************************************/
			STK_STATUS					ToJson(Value* i_pJsonNode, Document* pDoc);
		};

		STK_STATUS	Stk_ConnectorImpl::ToJson(Value* i_pJsonNode, Document* i_JsonDocP)
		{
			STK_STATUS			ret = STK_SUCCESS;
			if (!i_pJsonNode)
				return STK_ERROR;
			i_pJsonNode->SetObject();
			i_pJsonNode->AddMember(CONNECTOR_NODE_NAME_ID, m_ID, i_JsonDocP->GetAllocator());
			i_pJsonNode->AddMember(CONNECTOR_NODE_NAME_ISLINKPROTO, m_bIsLinkProto, i_JsonDocP->GetAllocator());
			string strValue = ChineseCode::WStringToString(m_wstrPlcPath);
			i_pJsonNode->AddMember(CONNECTOR_NODE_NAME_LINKPLCPATH, strValue, i_JsonDocP->GetAllocator());
			i_pJsonNode->AddMember(CONNECTOR_NODE_NAME_ISLINKENTITY, m_bIsLinkEntity, i_JsonDocP->GetAllocator());
			i_pJsonNode->AddMember(CONNECTOR_NODE_NAME_LINKENTITY, m_idEntity, i_JsonDocP->GetAllocator());
			i_pJsonNode->AddMember(CONNECTOR_NODE_NAME_POINTEDENTITYTYPE, m_ePointingEntityType, i_JsonDocP->GetAllocator());
			i_pJsonNode->AddMember(CONNECTOR_NODE_NAME_ISLINKENTITIES, m_bIsLinkEntities, i_JsonDocP->GetAllocator());
			//linkEntities
			char szValue[256] = { 0 };
			Value  valLinkEntities;
			valLinkEntities.SetArray();
			vector<SubEntity>::iterator itorEntity = m_vcEntities.begin();
			while (itorEntity != m_vcEntities.end())
			{
				Value valEntity;
				valEntity.SetObject();
				valEntity.AddMember(LINKENTITIES_NODE_NAME_ENTITYID, (*itorEntity).idEntity, i_JsonDocP->GetAllocator());
				valEntity.AddMember(LINKENTITIES_NODE_NAME_ENTITYTYPE, (*itorEntity).nuEntityType, i_JsonDocP->GetAllocator());
				strValue = "";
				//pickPnt
				sprintf(szValue, "%.8f", (*itorEntity).pntPick.X);
				strValue += szValue;
				strValue += " ";
				sprintf(szValue, "%.8f", (*itorEntity).pntPick.Y);
				strValue += szValue;
				strValue += " ";
				sprintf(szValue, "%.8f", (*itorEntity).pntPick.Z);
				strValue += szValue;
				valEntity.AddMember(LINKENTITIES_NODE_NAME_PICKPNT, strValue, i_JsonDocP->GetAllocator());
				//pickDir
				strValue = "";
				sprintf(szValue, "%.8f", (*itorEntity).dirPick.X);
				strValue += szValue;
				strValue += " ";
				sprintf(szValue, "%.8f", (*itorEntity).dirPick.Y);
				strValue += szValue;
				strValue += " ";
				sprintf(szValue, "%.8f", (*itorEntity).dirPick.Z);
				strValue += szValue;
				valEntity.AddMember(LINKENTITIES_NODE_NAME_PICKDIR, strValue, i_JsonDocP->GetAllocator());
				valLinkEntities.PushBack(valEntity, i_JsonDocP->GetAllocator());
				itorEntity++;
			}
			if(valLinkEntities.Size() != 0)
				i_pJsonNode->AddMember(CONNECTOR_NODE_NAME_LINKENTITIES, valLinkEntities, i_JsonDocP->GetAllocator());
			return ret;
		}

		STK_STATUS	Stk_ConnectorImpl::FromJson(Value* i_pJsonNode, Document*i_JsonDocP)
		{
			STK_STATUS			ret = STK_SUCCESS;
			if (!i_pJsonNode)
				return STK_ERROR;
			Value::MemberIterator valueID = i_pJsonNode->FindMember(CONNECTOR_NODE_NAME_ID);
			if (valueID != i_pJsonNode->MemberEnd())
			{
				m_ID = valueID->value.GetInt();
			}
			Value::MemberIterator valuePointedEntityType = i_pJsonNode->FindMember(CONNECTOR_NODE_NAME_POINTEDENTITYTYPE);
			if (valuePointedEntityType != i_pJsonNode->MemberEnd())
			{
				m_ePointedEntityType = (StkEntityTypeEnum)valuePointedEntityType->value.GetInt();
			}
			Value::MemberIterator valueIsLinkProto = i_pJsonNode->FindMember(CONNECTOR_NODE_NAME_ISLINKPROTO);
			if (valueIsLinkProto != i_pJsonNode->MemberEnd())
			{
				m_bIsLinkProto = valueIsLinkProto->value.GetBool();
			}
			Value::MemberIterator valueLinkPlcPath = i_pJsonNode->FindMember(CONNECTOR_NODE_NAME_LINKPLCPATH);
			if (valueLinkPlcPath != i_pJsonNode->MemberEnd())
			{
				m_wstrPlcPath = ChineseCode::StringToWString(valueLinkPlcPath->value.GetString());
			}
			Value::MemberIterator valueIsLinkEntity = i_pJsonNode->FindMember(CONNECTOR_NODE_NAME_ISLINKENTITY);
			if (valueIsLinkEntity != i_pJsonNode->MemberEnd())
			{
				m_bIsLinkEntity = valueIsLinkEntity->value.GetBool();
			}
			Value::MemberIterator valueLinkEntity = i_pJsonNode->FindMember(CONNECTOR_NODE_NAME_LINKENTITY);
			if (valueLinkEntity != i_pJsonNode->MemberEnd())
			{
				m_idEntity = valueLinkEntity->value.GetInt();
			}
			Value::MemberIterator valueIsLinkEntities = i_pJsonNode->FindMember(CONNECTOR_NODE_NAME_ISLINKENTITIES);
			if (valueIsLinkEntities != i_pJsonNode->MemberEnd())
			{
				m_bIsLinkEntities = valueIsLinkEntities->value.GetBool();
			}
			Value::MemberIterator valueLinkEntities = i_pJsonNode->FindMember(CONNECTOR_NODE_NAME_LINKENTITIES);
			if(valueLinkEntities != i_pJsonNode->MemberEnd())
			{
				Value valEntities = valueIsLinkEntities->value.GetArray();
				Value valEntity;
				for (int i = 0; i < valEntities.Size(); i++)
				{
					valEntity = valEntities[i];
					SubEntity subEntity;
					Value::MemberIterator valueEntityID = valEntity.FindMember(LINKENTITIES_NODE_NAME_ENTITYID);
					if (valueEntityID != valEntity.MemberEnd())
					{
						subEntity.idEntity = valueEntityID->value.GetInt();
					}
					Value::MemberIterator valueEntityType = valEntity.FindMember(LINKENTITIES_NODE_NAME_ENTITYTYPE);
					if (valueEntityType != valEntity.MemberEnd())
					{
						subEntity.nuEntityType = (StkEntityTypeEnum)valueEntityType->value.GetInt();
					}
					string strValue;
					std::vector<std::string> vecValues;
					Value::MemberIterator valuePickPnt = valEntity.FindMember(LINKENTITIES_NODE_NAME_PICKPNT);
					if (valuePickPnt != valEntity.MemberEnd())
					{
						strValue = valuePickPnt->value.GetString();
						CStringTools::StringSplit(strValue, " ", vecValues);
						subEntity.pntPick.X = atof(vecValues[0].c_str());
						subEntity.pntPick.Y = atof(vecValues[1].c_str());
						subEntity.pntPick.Z = atof(vecValues[2].c_str());
					}
					vecValues.clear();
					Value::MemberIterator valuePickDir = valEntity.FindMember(LINKENTITIES_NODE_NAME_PICKDIR);
					if (valuePickDir != valEntity.MemberEnd())
					{
						strValue = valuePickDir->value.GetString();
						CStringTools::StringSplit(strValue, " ", vecValues);
						subEntity.dirPick.X = atof(vecValues[0].c_str());
						subEntity.dirPick.Y = atof(vecValues[1].c_str());
						subEntity.dirPick.Z = atof(vecValues[2].c_str());
					}
					m_vcEntities.push_back(subEntity);
				}
			}
			return ret;
		}

		STK_CLASS_IMPL_IMPLEMENT(Stk_Connector)

		Stk_ConnectorImpl::Stk_ConnectorImpl()
		{
			m_ePointedEntityType = ENTITY_TYPE_UNKNOWN;
			m_ePointingEntityType = ENTITY_TYPE_VIEW;
			m_idEntity = -1;
			m_vcEntities.clear();
			m_wstrPlcPath = L"";
		}

		Stk_ConnectorImpl::~Stk_ConnectorImpl()
		{

		}
		STK_IMPLEMENT_CLASS(Stk_Connector, Stk_Object)

		Stk_Connector::Stk_Connector() : Stk_Object(new Stk_ConnectorImpl())
		{

		}
		Stk_Connector::~Stk_Connector()
		{

		}

		void Stk_Connector::SetEntityType(StkEntityTypeEnum i_ePointingEnt, StkEntityTypeEnum i_ePointedEnt)
		{
			((Stk_ConnectorImpl*)m_pImpl)->m_ePointingEntityType = i_ePointingEnt;
			((Stk_ConnectorImpl*)m_pImpl)->m_ePointedEntityType = i_ePointedEnt;
		}

		void Stk_Connector::GetEntityType(StkEntityTypeEnum& o_ePointingEnt, StkEntityTypeEnum& o_ePointedEnt)
		{
			o_ePointingEnt = ((Stk_ConnectorImpl*)m_pImpl)->m_ePointingEntityType;
			o_ePointedEnt = ((Stk_ConnectorImpl*)m_pImpl)->m_ePointedEntityType;
		}

		void Stk_Connector::SetLinkProto(const wstring &i_wstrPlcPath)
		{
			((Stk_ConnectorImpl*)m_pImpl)->m_wstrPlcPath = i_wstrPlcPath;
		}

		wstring	Stk_Connector::GetLinkProto()
		{
			return ((Stk_ConnectorImpl*)m_pImpl)->m_wstrPlcPath;
		}

		STK_BOOL	Stk_Connector::IsLinkProto()
		{
			return ((Stk_ConnectorImpl*)m_pImpl)->m_bIsLinkProto;
		}

		void	Stk_Connector::SetLinkNode(STK_ID i_idNode)
		{
			((Stk_ConnectorImpl*)m_pImpl)->m_idEntity = i_idNode;
		}

		STK_ID	Stk_Connector::GetLinkNode()
		{
			return ((Stk_ConnectorImpl*)m_pImpl)->m_idEntity;
		}

		STK_BOOL Stk_Connector::IsLinkNode()
		{
			return ((Stk_ConnectorImpl*)m_pImpl)->m_bIsLinkEntity;
		}

		void	Stk_Connector::AddSubEntity(SubEntity i_SubEntity)
		{
			((Stk_ConnectorImpl*)m_pImpl)->m_vcEntities.push_back(i_SubEntity);
			((Stk_ConnectorImpl*)m_pImpl)->m_bIsLinkEntities = true;
		}

		vector<SubEntity>	Stk_Connector::GetSubEntity()
		{
			return ((Stk_ConnectorImpl*)m_pImpl)->m_vcEntities;
		}

		STK_BOOL	Stk_Connector::IsLinkEntities()
		{
			return ((Stk_ConnectorImpl*)m_pImpl)->m_bIsLinkEntities;
		}

		STK_STATUS	Stk_Connector::ToJson(void* i_DataP, void* i_JsonDocP)
		{
			return ((Stk_ConnectorImpl*)m_pImpl)->ToJson((Value*)i_DataP, (Document*)i_JsonDocP);
		}

		STK_STATUS	Stk_Connector::FromJson(void* i_DataP, void* i_JsonDocP)
		{
			return ((Stk_ConnectorImpl*)m_pImpl)->FromJson((Value*)i_DataP, (Document*)i_JsonDocP);
		}

		void   Stk_Connector::SetIsLinkProto(STK_BOOL i_bLinkProto)
		{
			((Stk_ConnectorImpl*)m_pImpl)->m_bIsLinkProto = i_bLinkProto;
		}

		void   Stk_Connector::SetIsLinkNode(STK_BOOL i_bLinkNode)
		{
			((Stk_ConnectorImpl*)m_pImpl)->m_bIsLinkEntity = i_bLinkNode;
		}
	}
}
