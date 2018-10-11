// All Rights Reserved. Copyright (C) 2017 Hoteamsoft. 
/**************************************************************************
*	@class	Stk_ViewDocument
*	@brief	 ”ÕºŒƒµµπ‹¿Ì¿‡
*	@par ¿˙ ∑º«¬º
*				2017/06/09 ¥¥Ω®°£likun(Hoteamsoft)
**************************************************************************/

#include "Stk_ViewDocument.h"
#include "Stk_View.h"
#include "Stk_BaseDocumentImpl.h"
#include "ChineseCode.h"

#define VIEWDOC_NODE_NAME_PROTOTYPEID	"modelID"
#define VIEWDOC_NODE_NAME_VERSION		"version"
#define VIEWDOC_NODE_NAME_VIEWS			"views"
#define VIEWDOC_NODE_NAME_MODEL_VIEW	"model_views"
#define VIEWDOC_NODE_NAME_ID   "id"
#define VIEWDOC_NODE_NAME_VIEW_PMI   "view_pmis"
#define VIEWDOC_NODE_NAME_PMI  "pmis"
#define VIEWDOC_NODE_NAME_VIEW_VIEW   "view_views"
#define VIEWDOC_NODE_NAME_VIEW_LINKVIEW  "views"

namespace HoteamSoft {
	namespace SVLLib {
		class Stk_ViewDocumentImpl : public Stk_BaseDocumentImpl
		{
			STK_CLASS_IMPL_DECLARE(Stk_ViewDocument)
		public:
			Stk_ViewDocumentImpl(void);
			virtual ~Stk_ViewDocumentImpl(void);
		public:
			map<STK_ID, vector<STK_ID>> m_mapModelIdToViewList;	//!< ¡„º˛”Î ”Õº¡–±Ìπÿ¡™µƒMap
			map<STK_ID, Stk_ViewPtr> m_mapViewIdToViewPtr;				//!<  ”Õº±‡∫≈”Î ”Õº÷∏’Îπÿ¡™Map
			/****************************************************************************
			*	@brief		≥ı ºªØ∂‘œÛ ˝æ›
			*	@param[in]	Œﬁ
			*	@param[out]	Œﬁ
			*	@retval		Œﬁ
			*	@note		Œﬁ
			*	@attention
			****************************************************************************/
			virtual STK_STATUS FromJson(Document& jsonDoc);

			/****************************************************************************
			*	@brief		Ω´ ˝æ›◊™ªªŒ™Json∂‘œÛ
			*	@param[in]	Œﬁ
			*	@param[out]	jsonDoc		JsonŒƒµµ∂‘œÛ
			*	@retval		STK_SUCCESS			∂¡»°≥…π¶
			*	@retval		∆‰À˚				∂¡»° ß∞‹
			*	@note		Œﬁ
			*	@attention
			****************************************************************************/
			virtual STK_STATUS ToJson(Document& jsonDoc);

			/****************************************************************************
			*	@brief		ªÒ»°÷∏∂®¡„º˛µƒ ”Õº¡–±Ì
			*	@param[in]	modelID			¡„º˛ID
			*	@param[out]	Œﬁ
			*	@retval		vector<Stk_ViewPtr>			 ”Õº¡–±Ì
			*	@note		Œﬁ
			*	@attention
			****************************************************************************/
			void			GetViewListByModelId(unsigned int modelID, vector<Stk_ViewPtr>& vcViews);
			Stk_ViewPtr     GetView(unsigned int modelID, unsigned int viewID);
			map<STK_ID, Stk_ViewPtr>            GetViewMap();
		};

		STK_CLASS_IMPL_IMPLEMENT(Stk_ViewDocument)
		Stk_ViewDocumentImpl::Stk_ViewDocumentImpl(void)
		{
			m_strVersion = L"0.1";
		}

		Stk_ViewDocumentImpl::~Stk_ViewDocumentImpl(void)
		{

		}

		STK_STATUS Stk_ViewDocumentImpl::FromJson(Document& jsonDoc)
		{
			m_mapViewIdToViewPtr.clear();
			m_mapModelIdToViewList.clear();
			//∏˘æ›jsonDoc÷–µƒƒ⁄»›≥ı ºªØm_mapmodelIDToViewList
			Value::MemberIterator VersionNodeIter = jsonDoc.FindMember(VIEWDOC_NODE_NAME_VERSION);
			if (VersionNodeIter != jsonDoc.MemberEnd())
			{
				string strVersion = VersionNodeIter->value.GetString();
				//m_strVersion = ChineseCode::StringToWString(strVersion);
				m_strVersion = ChineseCode::UTF_8ToUnicode(strVersion.c_str());
			}

			Value::MemberIterator viewsNodeIter = jsonDoc.FindMember(VIEWDOC_NODE_NAME_VIEWS);
			if (viewsNodeIter == jsonDoc.MemberEnd())
				return STK_ERROR;;
			Value viewsValue = viewsNodeIter->value.GetArray();
			Value valView;
			for (int i = 0; i < viewsValue.Size(); i++)
			{
				valView = viewsValue[i];
				Stk_ViewPtr pView = Stk_ViewPtr::CreateObject();
				pView->FromJson(&valView, &jsonDoc);
				m_mapViewIdToViewPtr.insert(make_pair(pView->GetID(), pView));
			}

			Value::MemberIterator model_viewsNodeIter = jsonDoc.FindMember(VIEWDOC_NODE_NAME_MODEL_VIEW);
			if (model_viewsNodeIter != jsonDoc.MemberEnd())
			{
				rapidjson::Value::Array valModels = model_viewsNodeIter->value.GetArray();
				Value valModel;
				for (int i = 0; i < valModels.Size(); i++)
				{
					valModel = valModels[i];
					STK_ID modelID = -1;
					vector<STK_ID> vcView;
					Value::MemberIterator idIter = valModel.FindMember(VIEWDOC_NODE_NAME_ID);
					if (idIter != valModel.MemberEnd())
						modelID = idIter->value.GetInt();
					Value::MemberIterator viewsIter = valModel.FindMember(VIEWDOC_NODE_NAME_VIEWS);
					if (viewsIter != valModel.MemberEnd())
					{
						Value viewIDs = viewsIter->value.GetArray();
						Value viewID;
						for (int j = 0; j < viewIDs.Size(); j++)
						{
							viewID = viewIDs[j];
							vcView.push_back(viewID.GetInt());
						}
					}
					m_mapModelIdToViewList.insert(make_pair(modelID, vcView));
				}
			}
			//instance_views ‘› ±»± °

			//view_pmis
			Value::MemberIterator view_pmisNodeIter = jsonDoc.FindMember(VIEWDOC_NODE_NAME_VIEW_PMI);
			if (view_pmisNodeIter != jsonDoc.MemberEnd())
			{
				rapidjson::Value::Array valViews = view_pmisNodeIter->value.GetArray();
				Value valView;
				for (int i = 0; i < valViews.Size(); i++)
				{
					valView = valViews[i];
					STK_ID stkViewID = -1;
					Value::MemberIterator idIter = valView.FindMember(VIEWDOC_NODE_NAME_ID);
					if (idIter != valView.MemberEnd())
						stkViewID = idIter->value.GetInt();
					map<STK_ID, Stk_ViewPtr>::iterator itorViewIdToViewPtr = m_mapViewIdToViewPtr.find(stkViewID);
					if(itorViewIdToViewPtr == m_mapViewIdToViewPtr.end())
						continue;
					Stk_ViewPtr pView = itorViewIdToViewPtr->second;
					if(pView == nullptr)
						continue;
					Value::MemberIterator pmisIter = valView.FindMember(VIEWDOC_NODE_NAME_PMI);
					if (pmisIter != valView.MemberEnd())
					{
						Value valPmiIDs = pmisIter->value.GetArray();
						Value valPmiID;
						for (int j = 0; j < valPmiIDs.Size(); j++)
						{
							valPmiID = valPmiIDs[j];
							pView->AddConnector(ENTITY_TYPE_PMI, valPmiID.GetInt());
						}
					}
				}
			}

			//view_views
			Value::MemberIterator view_viewsNodeIter = jsonDoc.FindMember(VIEWDOC_NODE_NAME_VIEW_VIEW);
			if (view_viewsNodeIter != jsonDoc.MemberEnd())
			{
				rapidjson::Value::Array valViews = view_viewsNodeIter->value.GetArray();
				Value valView;
				for (int i = 0; i < valViews.Size(); i++)
				{
					valView = valViews[i];
					STK_ID stkViewID = -1;
					Value::MemberIterator idIter = valView.FindMember(VIEWDOC_NODE_NAME_ID);
					if (idIter != valView.MemberEnd())
						stkViewID = idIter->value.GetInt();
					map<STK_ID, Stk_ViewPtr>::iterator itorViewIdToViewPtr = m_mapViewIdToViewPtr.find(stkViewID);
					if (itorViewIdToViewPtr == m_mapViewIdToViewPtr.end())
						continue;
					Stk_ViewPtr pView = itorViewIdToViewPtr->second;
					if (pView == nullptr)
						continue;
					Value::MemberIterator linkViewIter = valView.FindMember(VIEWDOC_NODE_NAME_VIEW_LINKVIEW);
					if (linkViewIter != valView.MemberEnd())
					{
						Value valLinkVIewIDs = linkViewIter->value.GetArray();
						Value valLinkViewID;
						for (int j = 0; j < valLinkVIewIDs.Size(); j++)
						{
							valLinkViewID = valLinkVIewIDs[j];
							pView->AddConnector(ENTITY_TYPE_VIEW, valLinkViewID.GetInt());
						}
					}
				}
			}

			return STK_SUCCESS;
		}

		STK_STATUS Stk_ViewDocumentImpl::ToJson(Document& jsonDoc)
		{
			//∏˘æ›m_mapmodelIDToViewListÃÓ≥‰jsonDoc
			STK_STATUS ret = STK_ERROR;
			jsonDoc.RemoveAllMembers();
			if (m_mapModelIdToViewList.size() <= 0)
				return ret;

			string strVersion;
			//strVersion = ChineseCode::WStringToString(m_strVersion);
			ChineseCode::UnicodeToUTF_8(m_strVersion.c_str(), strVersion);
			Value valVersion;
			valVersion.SetString(strVersion.c_str(), jsonDoc.GetAllocator());
			jsonDoc.AddMember(VIEWDOC_NODE_NAME_VERSION, valVersion, jsonDoc.GetAllocator());

			// ”Õº¡–±ÌΩ⁄µ„
			Value viewsValue;
			viewsValue.SetArray();
			
			//¡„º˛- ”Õºπÿ¡™Ω⁄µ„
			Value model_viewsValue;
			model_viewsValue.SetArray();

			map<STK_ID, vector<STK_ID>>::iterator itorModelIDToViewList = m_mapModelIdToViewList.begin();
			while (itorModelIDToViewList != m_mapModelIdToViewList.end())
			{
				Value valModel;
				valModel.SetObject();
				valModel.AddMember(VIEWDOC_NODE_NAME_ID, itorModelIDToViewList->first, jsonDoc.GetAllocator());
				Value ViewIdsValue;
				ViewIdsValue.SetArray();
				
				vector<STK_ID>& vecView = itorModelIDToViewList->second;
				for (int i = 0; i < vecView.size(); i++)
				{
					STK_ID viewID = vecView[i];
					//ÃÌº”ƒ£–Õ”Î ”Õºµƒπÿ¡™ ˝æ›
					ViewIdsValue.PushBack(viewID, jsonDoc.GetAllocator());

					map<STK_ID, Stk_ViewPtr>::iterator itorView = m_mapViewIdToViewPtr.find(viewID);
					if(itorView == m_mapViewIdToViewPtr.end())
						continue;
					Stk_ViewPtr pView = itorView->second;
					if(pView == nullptr)
						continue;
					//ÃÌº” ”Õº ˝æ›
					Value valView;
					if(pView->ToJson(&valView, &jsonDoc) == STK_SUCCESS)
					{
						viewsValue.PushBack(valView, jsonDoc.GetAllocator());
					}
				}
				valModel.AddMember(VIEWDOC_NODE_NAME_VIEWS, ViewIdsValue, jsonDoc.GetAllocator());
				model_viewsValue.PushBack(valModel, jsonDoc.GetAllocator());
				itorModelIDToViewList++;
			}
			jsonDoc.AddMember(VIEWDOC_NODE_NAME_VIEWS, viewsValue, jsonDoc.GetAllocator());
			jsonDoc.AddMember(VIEWDOC_NODE_NAME_MODEL_VIEW, model_viewsValue, jsonDoc.GetAllocator());

			Value valView_pmis;
			valView_pmis.SetArray();
			Value valView_Views;
			valView_Views.SetArray();
			map<STK_ID, Stk_ViewPtr>::iterator itorViewMap = m_mapViewIdToViewPtr.begin();
			while (itorViewMap != m_mapViewIdToViewPtr.end())
			{
				Stk_ViewPtr viewP = itorViewMap->second;
				if (viewP == nullptr)
				{
					itorViewMap++;
					continue;
				}
				map<StkEntityTypeEnum, vector<STK_ID>> vcConnector = viewP->GetConnector();
				map<StkEntityTypeEnum, vector<STK_ID>>::iterator itorConnector = vcConnector.begin();
				while (itorConnector != vcConnector.end())
				{
					StkEntityTypeEnum eEntityType = itorConnector->first;
					//view_pmis
					if (eEntityType == ENTITY_TYPE_PMI)
					{
						Value valView_pmi;
						valView_pmi.SetObject();
						valView_pmi.AddMember(VIEWDOC_NODE_NAME_ID, viewP->GetID(), jsonDoc.GetAllocator());
						Value valPmiIds;
						valPmiIds.SetArray();
						vector<STK_ID> vcPmiId = itorConnector->second;
						vector<STK_ID>::iterator itorPmiId = vcPmiId.begin();
						while (itorPmiId != vcPmiId.end())
						{
							valPmiIds.PushBack(*itorPmiId, jsonDoc.GetAllocator());
							itorPmiId++;
						}
						valView_pmi.AddMember(VIEWDOC_NODE_NAME_PMI, valPmiIds, jsonDoc.GetAllocator());
						valView_pmis.PushBack(valView_pmi, jsonDoc.GetAllocator());
					}
					//view_pmis
					if (eEntityType == ENTITY_TYPE_VIEW)
					{
						Value valView_View;
						valView_View.SetObject();
						valView_View.AddMember(VIEWDOC_NODE_NAME_ID, viewP->GetID(), jsonDoc.GetAllocator());
						Value valViewIds;
						valViewIds.SetArray();
						vector<STK_ID> vcViewId = itorConnector->second;
						vector<STK_ID>::iterator itorViewId = vcViewId.begin();
						while (itorViewId != vcViewId.end())
						{
							valViewIds.PushBack(*itorViewId, jsonDoc.GetAllocator());
							itorViewId++;
						}
						valView_View.AddMember(VIEWDOC_NODE_NAME_VIEW_LINKVIEW, valViewIds, jsonDoc.GetAllocator());
						valView_Views.PushBack(valView_View, jsonDoc.GetAllocator());
					}
					itorConnector++;
				}
				itorViewMap++;
			}
			jsonDoc.AddMember(VIEWDOC_NODE_NAME_VIEW_PMI, valView_pmis, jsonDoc.GetAllocator());
			jsonDoc.AddMember(VIEWDOC_NODE_NAME_VIEW_VIEW, valView_Views, jsonDoc.GetAllocator());
			
			return STK_SUCCESS;
		}

		void Stk_ViewDocumentImpl::GetViewListByModelId(unsigned int modelID, vector<Stk_ViewPtr>& vcViews)
		{
			map<STK_ID, vector<STK_ID>>::iterator itorModelIdToViewList = m_mapModelIdToViewList.find(modelID);
			if (itorModelIdToViewList != m_mapModelIdToViewList.end())
			{
				vector<STK_ID> vcViewID = itorModelIdToViewList->second;
				vector<STK_ID>::iterator itorViewID = vcViewID.begin();
				while (itorViewID != vcViewID.end())
                {
                    map<STK_ID, Stk_ViewPtr>::iterator itorViewIdToViewPtr = m_mapViewIdToViewPtr.find(*itorViewID);
					if (itorViewIdToViewPtr != m_mapViewIdToViewPtr.end() && itorViewIdToViewPtr->second != nullptr)
						vcViews.push_back(itorViewIdToViewPtr->second);
					itorViewID++;
				}
			}
		}

		Stk_ViewPtr Stk_ViewDocumentImpl::GetView(unsigned int modelID, unsigned int viewID)
		{
			Stk_ViewPtr pView = nullptr;
			map<STK_ID, vector<STK_ID>>::iterator itorModelIdToViewList = m_mapModelIdToViewList.find(modelID);
			if (itorModelIdToViewList != m_mapModelIdToViewList.end())
			{
				map<STK_ID, Stk_ViewPtr>::iterator itorViewIdToViewPtr = m_mapViewIdToViewPtr.find(viewID);
				if (itorViewIdToViewPtr != m_mapViewIdToViewPtr.end())
					pView = itorViewIdToViewPtr->second;
			}
			return pView;
		}

		map<STK_ID, Stk_ViewPtr> Stk_ViewDocumentImpl::GetViewMap()
		{
			return m_mapViewIdToViewPtr;
		}

		STK_IMPLEMENT_CLASS(Stk_ViewDocument, Stk_BaseDocument)

		Stk_ViewDocument::Stk_ViewDocument() :
			Stk_BaseDocument(new Stk_ViewDocumentImpl())
		{
			
		}

		Stk_ViewDocument::~Stk_ViewDocument(void)
		{

		}

		void  Stk_ViewDocument::SetVersion(const wstring wstrVersion)
		{
			m_pImpl->m_strVersion = wstrVersion;
		}

		const wstring   Stk_ViewDocument::GetVersion()
		{
			return m_pImpl->m_strVersion;
		}

		void Stk_ViewDocument::GetViewListByModelId(unsigned int modelID, vector<Stk_ViewPtr>& vcViews)
		{
			((Stk_ViewDocumentImpl*)m_pImpl)->GetViewListByModelId(modelID, vcViews);
		}

		Stk_ViewPtr Stk_ViewDocument::GetView(unsigned int modelID, unsigned int viewID)
		{
			return ((Stk_ViewDocumentImpl*)m_pImpl)->GetView(modelID, viewID);
		}

		STK_STATUS Stk_ViewDocument::AddView(unsigned int modelID, Stk_ViewPtr viewP)
		{
			map<STK_ID, Stk_ViewPtr>::iterator itorView = ((Stk_ViewDocumentImpl*)m_pImpl)->m_mapViewIdToViewPtr.find(viewP->GetID());
			if(itorView == ((Stk_ViewDocumentImpl*)m_pImpl)->m_mapViewIdToViewPtr.end())
				((Stk_ViewDocumentImpl*)m_pImpl)->m_mapViewIdToViewPtr.insert(make_pair(viewP->GetID(), viewP));
			map<STK_ID, vector<STK_ID>>::iterator itorMap = ((Stk_ViewDocumentImpl*)m_pImpl)->m_mapModelIdToViewList.find(modelID);
			if (itorMap != ((Stk_ViewDocumentImpl*)m_pImpl)->m_mapModelIdToViewList.end())
			{
				bool bFlag = false;
				vector<STK_ID>::iterator itorView =  itorMap->second.begin();
				while (itorView != itorMap->second.end())
				{
					if (*itorView == viewP->GetID())
					{
						bFlag = true;
						break;
					}
					itorView++;
				}
				if(!bFlag)
					itorMap->second.push_back(viewP->GetID());
			}else
			{
				vector<STK_ID> vcView;
				vcView.push_back(viewP->GetID());
				((Stk_ViewDocumentImpl*)m_pImpl)->m_mapModelIdToViewList.insert(make_pair(modelID, vcView));
			}
			return STK_SUCCESS;
		}

		STK_STATUS Stk_ViewDocument::AddView(Stk_ViewPtr viewP)
		{
			if (viewP == nullptr)
				return STK_ERROR;
			map<STK_ID, Stk_ViewPtr>::iterator itorView = ((Stk_ViewDocumentImpl*)m_pImpl)->m_mapViewIdToViewPtr.find(viewP->GetID());
			if (itorView == ((Stk_ViewDocumentImpl*)m_pImpl)->m_mapViewIdToViewPtr.end())
				((Stk_ViewDocumentImpl*)m_pImpl)->m_mapViewIdToViewPtr.insert(make_pair(viewP->GetID(), viewP));
			return STK_SUCCESS;
		}

		STK_STATUS  Stk_ViewDocument::AddViews(unsigned int modelID, vector<STK_ID> vcViewsID)
		{
			map<STK_ID, vector<STK_ID>>::iterator itorListMap = ((Stk_ViewDocumentImpl*)m_pImpl)->m_mapModelIdToViewList.find(modelID);
			if (itorListMap == ((Stk_ViewDocumentImpl*)m_pImpl)->m_mapModelIdToViewList.end())
			{
				((Stk_ViewDocumentImpl*)m_pImpl)->m_mapModelIdToViewList.insert(make_pair(modelID, vcViewsID));
			}
			else
			{
				vector<STK_ID>::iterator itorViewID = vcViewsID.begin();
				while (itorViewID != vcViewsID.begin())
				{
					STK_ID ViewID = *itorViewID;
					bool bFlag = false;
					vector<STK_ID>::iterator itorView = itorListMap->second.begin();
					while (itorView != itorListMap->second.end())
					{
						if (*itorView == ViewID)
						{
							bFlag = true;
							break;
						}
						itorView++;
					}
					if (!bFlag)
						itorListMap->second.push_back(ViewID);
					itorViewID++;
				}
			}
			return STK_SUCCESS;
		}

		STK_STATUS Stk_ViewDocument::DelView(unsigned int modelID, unsigned int viewID)
		{
			map<STK_ID, vector<STK_ID>>::iterator itorMapModelView = ((Stk_ViewDocumentImpl*)m_pImpl)->m_mapModelIdToViewList.find(modelID);
			if (itorMapModelView == ((Stk_ViewDocumentImpl*)m_pImpl)->m_mapModelIdToViewList.end())
				return STK_ERROR;
			vector<STK_ID> vecViewPtr = itorMapModelView->second;
			vector<STK_ID>::iterator itVecViewPtr = vecViewPtr.begin();
			while (itVecViewPtr != vecViewPtr.end())
			{
				if ((*itVecViewPtr) == viewID)
				{
					vecViewPtr.erase(itVecViewPtr);
					break;
				}
				itVecViewPtr++;
			}
			return STK_SUCCESS;
		}

		map<STK_ID, Stk_ViewPtr>  Stk_ViewDocument::GetViewMap()
		{
			return ((Stk_ViewDocumentImpl*)m_pImpl)->GetViewMap();
		}
	}//svllib
}//hoteamsoft
