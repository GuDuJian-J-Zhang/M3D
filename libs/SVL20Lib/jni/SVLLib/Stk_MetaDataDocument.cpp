#include "Stk_MetaDataDocument.h"
#include "Stk_BaseDocumentImpl.h"
#include "Stk_MetaData.h"

#define METADATADOC_NODE_NAME_ROOT			"root"
#define METADATADOC_NODE_NAME_PROTOTYPES	"protoTypes"

namespace HoteamSoft {
	namespace SVLLib {

		class Stk_MetaDataDocumentImpl : public Stk_BaseDocumentImpl
		{
			STK_CLASS_IMPL_DECLARE(Stk_MetaDataDocument)
		public:
			Stk_MetaDataDocumentImpl(void);
			virtual ~Stk_MetaDataDocumentImpl(void);
		public:
			map<int, vector<Stk_MetaDataPtr>> m_mapModelIdToMetaDataList;	//!< ¡„º˛”ÎMetaData¡–±Ìπÿ¡™µƒMap
		public:
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
			vector<Stk_MetaDataPtr>&			GetViewListByModelId(unsigned int modelID);
		
		};

		STK_CLASS_IMPL_IMPLEMENT(Stk_MetaDataDocument)
		Stk_MetaDataDocumentImpl::Stk_MetaDataDocumentImpl(void)
		{
			
		}

		Stk_MetaDataDocumentImpl::~Stk_MetaDataDocumentImpl(void)
		{

		}

		STK_STATUS Stk_MetaDataDocumentImpl::FromJson(Document& jsonDoc)
		{
			//∏˘æ›jsonDoc÷–µƒƒ⁄»›≥ı ºªØm_mapPartIdToMetaDataList
			Value::MemberIterator rootNodeIter = jsonDoc.FindMember(METADATADOC_NODE_NAME_ROOT);
			if (rootNodeIter == jsonDoc.MemberEnd())
				return STK_ERROR;
			Value::MemberIterator ProtoTypesNodeIter = rootNodeIter->value.FindMember(METADATADOC_NODE_NAME_PROTOTYPES);
			if (ProtoTypesNodeIter == rootNodeIter->value.MemberEnd())
				return STK_ERROR;
			Value ProtoTypes = ProtoTypesNodeIter->value.GetArray();
			Value ProtoType;
			while (ProtoTypes.Size()>0)
			{
				ProtoType = ProtoTypes.PopBack();
				int iProtoTypeID = ProtoType.GetInt();

				//ValueIterator itor = ProtoType.Begin(); 
				//Value ProtoType.GetArray();

				/*Value::MemberIterator nodeIDIter = ProtoType.FindMember(VIEWDOC_NODE_NAME_PROTOTYPEID);
				if (nodeIDIter == ProtoType.MemberEnd())
					continue;
				int iProtoTypeID = nodeIDIter->value.GetInt();

				Value::MemberIterator ViewsNodeIter = ProtoType.FindMember(VIEWDOC_NODE_NAME_VIEWS);
				if (ViewsNodeIter == ProtoType.MemberEnd())
					continue;
				Value Views = ViewsNodeIter->value.GetArray();
				vector<Stk_ViewPtr> vecView;
				Value View;
				while (Views.Size()>0)
				{
					View = Views.PopBack();
					Stk_ViewPtr viewP = Stk_ViewPtr::CreateObject();
					viewP->Init((Stk_ViewDocument*)m_pPubl, iProtoTypeID);
					viewP->ReadFile(&View);
					vecView.push_back(viewP);
				}
				m_mapPartIdToViewList.insert(pair<int, vector<Stk_ViewPtr>>(iProtoTypeID, vecView));*/
				
			}

			return STK_SUCCESS;
		}

		STK_STATUS Stk_MetaDataDocumentImpl::ToJson(Document& jsonDoc)
		{
			//∏˘æ›m_mapPartIdToMetaDataListÃÓ≥‰jsonDoc
			jsonDoc.Clear();
			if (m_mapModelIdToMetaDataList.size() <= 0)
				return STK_ERROR;
			Value valRoot;
			valRoot.SetObject();
			jsonDoc.AddMember(METADATADOC_NODE_NAME_ROOT, valRoot, jsonDoc.GetAllocator());
			Value valProtoTypes;
			valProtoTypes.SetArray();

			map<int, vector<Stk_MetaDataPtr>>::iterator itorPartIdToMetaDataList = m_mapModelIdToMetaDataList.begin();
			while (itorPartIdToMetaDataList != m_mapModelIdToMetaDataList.end())
			{
				/*Value valProtoType;
				valProtoType.SetObject();
				valProtoType.AddMember(VIEWDOC_NODE_NAME_PROTOTYPEID, itorPartIdToViewList->first, jsonDoc.GetAllocator());
				Value valViews;
				valViews.SetArray();
				valProtoType.AddMember(VIEWDOC_NODE_NAME_VIEWS, valViews, jsonDoc.GetAllocator());

				vector<Stk_ViewPtr>& vecView = itorPartIdToViewList->second;
				for (int i = 0; i < vecView.size(); i++)
				{
					if(vecView[i] == NULL)
						continue;
					vecView[i]->WriteFile(&valViews);
				}
				valProtoTypes.PushBack(valProtoType, jsonDoc.GetAllocator());*/
				itorPartIdToMetaDataList++;
			}
			valRoot.AddMember(METADATADOC_NODE_NAME_PROTOTYPES, valProtoTypes, jsonDoc.GetAllocator());
			return STK_SUCCESS;
		}

		vector<Stk_MetaDataPtr>& Stk_MetaDataDocumentImpl::GetViewListByModelId(unsigned int modelID)
        {
            return m_mapModelIdToMetaDataList.find(modelID)->second;
		}

		STK_IMPLEMENT_CLASS(Stk_MetaDataDocument, Stk_BaseDocument)

		Stk_MetaDataDocument::Stk_MetaDataDocument(void) :
		Stk_BaseDocument(new Stk_MetaDataDocumentImpl())
		{

		}

		Stk_MetaDataDocument::~Stk_MetaDataDocument(void)
		{

		}

		vector<Stk_MetaDataPtr>& Stk_MetaDataDocument::GetMetaDataListByModelId(unsigned int modelID)
		{
			return ((Stk_MetaDataDocumentImpl*)m_pImpl)->GetViewListByModelId(modelID);
		}

		Stk_MetaDataPtr	Stk_MetaDataDocument::GetMetaData(unsigned int modelID, unsigned int metadataID)
		{
			Stk_MetaDataPtr ptrMetaData = nullptr;
			vector<Stk_MetaDataPtr> vecMetaData = ((Stk_MetaDataDocumentImpl*)m_pImpl)->m_mapModelIdToMetaDataList.find(modelID)->second;
			vector<Stk_MetaDataPtr>::iterator itorMetaDataPtr = vecMetaData.begin();
			while (vecMetaData.end() != itorMetaDataPtr)
			{
				if ((*itorMetaDataPtr) != nullptr && (*itorMetaDataPtr)->GetID() == metadataID)
				{
					ptrMetaData = *itorMetaDataPtr;
					break;
				}
				itorMetaDataPtr++;
			}
			return ptrMetaData;
		}

		STK_STATUS  Stk_MetaDataDocument::AddMetaData(unsigned int modelID, Stk_MetaDataPtr metadataP)
		{
			map<int, vector<Stk_MetaDataPtr>>::iterator itorMap = ((Stk_MetaDataDocumentImpl*)m_pImpl)->m_mapModelIdToMetaDataList.find(modelID);
			if (itorMap == ((Stk_MetaDataDocumentImpl*)m_pImpl)->m_mapModelIdToMetaDataList.end())
			{
				vector<Stk_MetaDataPtr> vecMetaDataPtr;
				vecMetaDataPtr.push_back(metadataP);
				((Stk_MetaDataDocumentImpl*)m_pImpl)->m_mapModelIdToMetaDataList.insert(make_pair(modelID, vecMetaDataPtr));
			}
			else
			{
				itorMap->second.push_back(metadataP);
			}
	
			return	STK_SUCCESS;
		}

		STK_STATUS Stk_MetaDataDocument::DelMetaData(unsigned int modelID, unsigned int metadataID)
		{
			map<int, vector<Stk_MetaDataPtr>>::iterator itorMapPartMetaData = ((Stk_MetaDataDocumentImpl*)m_pImpl)->m_mapModelIdToMetaDataList.find(modelID);
			if (itorMapPartMetaData == ((Stk_MetaDataDocumentImpl*)m_pImpl)->m_mapModelIdToMetaDataList.end())
				return STK_ERROR_ID_NOT_FOUND;
			vector<Stk_MetaDataPtr> vecMetaDataPtr = itorMapPartMetaData->second;
			vector<Stk_MetaDataPtr>::iterator itVecMetaDataPtr = vecMetaDataPtr.begin();
			while (itVecMetaDataPtr != vecMetaDataPtr.end())
			{
				if ((*itVecMetaDataPtr) != nullptr && (*itVecMetaDataPtr)->GetID() == metadataID)
				{
					vecMetaDataPtr.erase(itVecMetaDataPtr);
					break;
				}
				itVecMetaDataPtr++;
			}
			return STK_SUCCESS;
		}
	}
}
