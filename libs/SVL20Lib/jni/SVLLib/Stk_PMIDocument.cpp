#include "Stk_PMIDocument.h"
#include "Stk_BaseDocumentImpl.h"
#include "Stk_PMI.h"
#include "ChineseCode.h"

#define PMIDOC_NODE_NAME_VERSION			"version"
#define PMIDOC_NODE_NAME_PMIS                "pmis"
#define PMIDOC_NODE_NAME_PROTOTYPES	"protoTypes"
#define PMIDOC_NODE_NAME_INSTANCES   "instances"
#define PMIDOC_NODE_NAME_PROTOTYPEID	"protoTypeID"
#define PMIDOC_NODE_NAME_MODEL_PMI     "model_pmis"
#define PMIDOC_NODE_NAME_INSTANCE_PMI "instance_pmis"
#define PMIDOC_NODE_NAME_ID   "id"


namespace HoteamSoft {
	namespace SVLLib {

		class Stk_PMIDocumentImpl : public Stk_BaseDocumentImpl
		{
			STK_CLASS_IMPL_DECLARE(Stk_PMIDocument)
		public:
			Stk_PMIDocumentImpl(void);
			virtual ~Stk_PMIDocumentImpl(void);
		public:
			map<STK_ID, vector<STK_ID>> m_mapModelIdToPMIList;	  // µ¿˝”ÎPMI¡–±Ìπÿ¡™µƒMap
			map<STK_ID, Stk_PMIPtr> m_mapPMIIdToPMIPtr;			  //PMI±‡∫≈”ÎPMI÷∏’Îπÿ¡™Map
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
			*	@brief		ªÒ»°÷∏∂®¡„º˛µƒPMI¡–±Ì
			*	@param[in]	modelID			¡„º˛ID
			*	@param[out]	Œﬁ
			*	@retval		vector<Stk_ViewPtr>			 ”Õº¡–±Ì
			*	@note		Œﬁ
			*	@attention
			****************************************************************************/
			void			GetPMIListByModelID(unsigned int modelID, vector<Stk_PMIPtr>& vePMIs);
		
		};

		STK_CLASS_IMPL_IMPLEMENT(Stk_PMIDocument)
		Stk_PMIDocumentImpl::Stk_PMIDocumentImpl(void)
		{
			m_strVersion = L"2.0.0";
		}

		Stk_PMIDocumentImpl::~Stk_PMIDocumentImpl(void)
		{

		}

		STK_STATUS Stk_PMIDocumentImpl::FromJson(Document& jsonDoc)
		{
			m_mapModelIdToPMIList.clear();
			m_mapPMIIdToPMIPtr.clear();
			//∏˘æ›jsonDoc÷–µƒƒ⁄»›≥ı ºªØm_mapModelIDToPMIList
			Value::MemberIterator versionNodeIter = jsonDoc.FindMember(PMIDOC_NODE_NAME_VERSION);
			if (versionNodeIter == jsonDoc.MemberEnd())
				return STK_ERROR;
			string strVersion = versionNodeIter->value.GetString();
			//m_strVersion = ChineseCode::StringToWString(strVersion);
			m_strVersion = ChineseCode::UTF_8ToUnicode(strVersion.c_str());
			//PMIS
			Value::MemberIterator PMIsNodeIter = jsonDoc.FindMember(PMIDOC_NODE_NAME_PMIS);
			if (PMIsNodeIter == jsonDoc.MemberEnd())
				return STK_ERROR;
			Value PMIsValue = PMIsNodeIter->value.GetArray();
			Value valPmi;
			for (int i = 0; i < PMIsValue.Size(); i++)
			{
				valPmi = PMIsValue[i];
				Stk_PMIPtr pPmi = Stk_PMIPtr::CreateObject();
				pPmi->FromJson(&valPmi, &jsonDoc);
				m_mapPMIIdToPMIPtr.insert(make_pair(pPmi->GetID(), pPmi));
			}
			//model_pmi
			Value::MemberIterator model_pmiNodeIter = jsonDoc.FindMember(PMIDOC_NODE_NAME_MODEL_PMI);
			if (model_pmiNodeIter != jsonDoc.MemberEnd())
			{
				rapidjson::Value::Array valModels = model_pmiNodeIter->value.GetArray();
				Value valModel;
				for (int i = 0; i < valModels.Size(); i++)
				{
					valModel = valModels[i];
					STK_ID modelID = -1;
					vector<STK_ID> vcPmi;
					Value::MemberIterator idIter = valModel.FindMember(PMIDOC_NODE_NAME_ID);
					if (idIter != valModel.MemberEnd())
						modelID = idIter->value.GetInt();
					Value::MemberIterator pmisIter = valModel.FindMember(PMIDOC_NODE_NAME_PMIS);
					if (pmisIter != valModel.MemberEnd())
					{
						Value pmiIDs = pmisIter->value.GetArray();
						Value pmiID;
						for (int j = 0; j < pmiIDs.Size(); j++)
						{
							pmiID = pmiIDs[j];
							vcPmi.push_back(pmiID.GetInt());
						}
					}
					m_mapModelIdToPMIList.insert(make_pair(modelID, vcPmi));
				}
			}

			return STK_SUCCESS;
		}

		STK_STATUS Stk_PMIDocumentImpl::ToJson(Document& jsonDoc)
		{
			//∏˘æ›m_mapmodelIDToPMIListÃÓ≥‰jsonDoc
			STK_STATUS ret = STK_ERROR;
			jsonDoc.RemoveAllMembers();
			if (m_mapModelIdToPMIList.size() <= 0)
				return ret;
			
			string strVersion;
			//strVersion = ChineseCode::WStringToString(m_strVersion);
			ChineseCode::UnicodeToUTF_8(m_strVersion.c_str(), strVersion);
			Value valVersion;
			valVersion.SetString(strVersion.c_str(), jsonDoc.GetAllocator());
			jsonDoc.AddMember(PMIDOC_NODE_NAME_VERSION, valVersion, jsonDoc.GetAllocator());

			Value valPMIs;
			valPMIs.SetArray();

			Value valModels;
			valModels.SetArray();
			
			//instances Ω⁄µ„‘› ±»± °
			Value valInstances;
			valInstances.SetArray();

			map<STK_ID, vector<STK_ID>>::iterator itormodelIDToPMIList = m_mapModelIdToPMIList.begin();
			while (itormodelIDToPMIList != m_mapModelIdToPMIList.end())
			{
				Value valModel;
				valModel.SetObject();
				valModel.AddMember(PMIDOC_NODE_NAME_ID, itormodelIDToPMIList->first, jsonDoc.GetAllocator());
				Value valPMIsID;
				valPMIsID.SetArray();
				
				//PMIΩ⁄µ„µƒ¥¥Ω®
				vector<STK_ID> vecPMI = itormodelIDToPMIList->second;
				for (int i = 0; i < vecPMI.size(); i++)
				{
					STK_ID pmiID = vecPMI[i];
					valPMIsID.PushBack(pmiID, jsonDoc.GetAllocator());
					
					Stk_PMIPtr pPMI = nullptr;
					map<STK_ID, Stk_PMIPtr>::iterator itorMap = m_mapPMIIdToPMIPtr.find(pmiID);
					if (itorMap != m_mapPMIIdToPMIPtr.end())
						pPMI = itorMap->second;
					if(pPMI == nullptr)
						continue;
					Value valPMI;
					if (pPMI->ToJson(&valPMI, &jsonDoc) == STK_SUCCESS)
					{
						valPMIs.PushBack(valPMI, jsonDoc.GetAllocator());
					}
				}
				valModel.AddMember(PMIDOC_NODE_NAME_PMIS, valPMIsID, jsonDoc.GetAllocator());
				valModels.PushBack(valModel, jsonDoc.GetAllocator());
				itormodelIDToPMIList++;
			}
			jsonDoc.AddMember(PMIDOC_NODE_NAME_PMIS, valPMIs, jsonDoc.GetAllocator());
			jsonDoc.AddMember(PMIDOC_NODE_NAME_MODEL_PMI, valModels, jsonDoc.GetAllocator());
			return STK_SUCCESS;
		}

		void	Stk_PMIDocumentImpl::GetPMIListByModelID(unsigned int modelID, vector<Stk_PMIPtr>& vcPMIs)
        {
            map<STK_ID, vector<STK_ID>>::iterator itorModelIdToPMIlist = m_mapModelIdToPMIList.find(modelID);
			if (itorModelIdToPMIlist == m_mapModelIdToPMIList.end())
				return;
			vector<STK_ID> vcPmiId = itorModelIdToPMIlist->second;
			vector<STK_ID>::iterator itorId = vcPmiId.begin();
			while (itorId != vcPmiId.end())
			{
				map<STK_ID, Stk_PMIPtr>::iterator itorPmi = m_mapPMIIdToPMIPtr.find(*itorId);
				if (itorPmi != m_mapPMIIdToPMIPtr.end())
					vcPMIs.push_back(itorPmi->second);
				itorId++;
			}
		}

		STK_IMPLEMENT_CLASS(Stk_PMIDocument, Stk_BaseDocument)

		Stk_PMIDocument::Stk_PMIDocument() :
			Stk_BaseDocument(new Stk_PMIDocumentImpl())
		{
			
		}

		Stk_PMIDocument::~Stk_PMIDocument()
		{
			DELETE_STK_CLASS_IMPL
		}

		void  Stk_PMIDocument::SetVersion(const wstring wstrVersion)
		{
			m_pImpl->m_strVersion = wstrVersion;
		}

		const   wstring  Stk_PMIDocument::GetVersion()
		{
			return m_pImpl->m_strVersion;
		}

		void Stk_PMIDocument::GetPMIListByModelID(unsigned int modelID, vector<Stk_PMIPtr>& vcPMIs)
		{
			((Stk_PMIDocumentImpl*)m_pImpl)->GetPMIListByModelID(modelID, vcPMIs);
		}

		Stk_PMIPtr Stk_PMIDocument::GetPMI(unsigned int modelID, unsigned int pmiID)
		{
			Stk_PMIPtr pPmi = nullptr;
			bool bFlag = false;
			vector<STK_ID> vecPMI = ((Stk_PMIDocumentImpl*)m_pImpl)->m_mapModelIdToPMIList.find(modelID)->second;
			vector<STK_ID>::iterator itorPMIPtr = vecPMI.begin();
			while (vecPMI.end() != itorPMIPtr)
			{
				if ((*itorPMIPtr) == pmiID)
				{
					bFlag = true;
					break;
				}
				itorPMIPtr++;
			}
			if (bFlag)
			{
				map<STK_ID, Stk_PMIPtr>::iterator itorMap = ((Stk_PMIDocumentImpl*)m_pImpl)->m_mapPMIIdToPMIPtr.find(pmiID);
			}
			return pPmi;
		}

		STK_STATUS Stk_PMIDocument::AddPMI(unsigned int modelID, Stk_PMIPtr pmiP)
		{
			map<STK_ID, vector<STK_ID>>::iterator itorListMap = ((Stk_PMIDocumentImpl*)m_pImpl)->m_mapModelIdToPMIList.find(modelID);
			if (itorListMap == ((Stk_PMIDocumentImpl*)m_pImpl)->m_mapModelIdToPMIList.end())
			{
				vector<STK_ID> vecPMIIdPtr;
				vecPMIIdPtr.push_back(pmiP->GetID());
				((Stk_PMIDocumentImpl*)m_pImpl)->m_mapModelIdToPMIList.insert(make_pair(modelID, vecPMIIdPtr));
			}
			else
			{
				bool bFlag = false;
				vector<STK_ID>::iterator itorPMI = itorListMap->second.begin();
				while (itorPMI != itorListMap->second.end())
				{
					if (*itorPMI == pmiP->GetID())
					{
						bFlag = true;
						break;
					}
					itorPMI++;	
				}
				if(!bFlag)
					itorListMap->second.push_back(pmiP->GetID());
			}
			AddPMI(pmiP);
			return	STK_SUCCESS;
		}

		STK_STATUS  Stk_PMIDocument::AddPMI(unsigned int modelID, vector<STK_ID> vcPMIId)
		{
			map<STK_ID, vector<STK_ID>>::iterator itorListMap = ((Stk_PMIDocumentImpl*)m_pImpl)->m_mapModelIdToPMIList.find(modelID);
			if (itorListMap == ((Stk_PMIDocumentImpl*)m_pImpl)->m_mapModelIdToPMIList.end())
			{
				((Stk_PMIDocumentImpl*)m_pImpl)->m_mapModelIdToPMIList.insert(make_pair(modelID, vcPMIId));
			}
			else
			{
				vector<STK_ID>::iterator itorPMIID = vcPMIId.begin();
				while (itorPMIID != vcPMIId.begin())
				{
					STK_ID pmiID = *itorPMIID;
					bool bFlag = false;
					vector<STK_ID>::iterator itorPMI = itorListMap->second.begin();
					while (itorPMI != itorListMap->second.end())
					{
							if (*itorPMI == pmiID)
							{
								bFlag = true;
								break;
							}
							itorPMI++;
						}
						if (!bFlag)
							itorListMap->second.push_back(pmiID);
					itorPMIID++;
				}
			}
			return STK_SUCCESS;
		}

		STK_STATUS Stk_PMIDocument::AddPMI(Stk_PMIPtr pmiP)
		{
			map<STK_ID, Stk_PMIPtr>::iterator itorMap = ((Stk_PMIDocumentImpl*)m_pImpl)->m_mapPMIIdToPMIPtr.find(pmiP->GetID());
			if (itorMap == ((Stk_PMIDocumentImpl*)m_pImpl)->m_mapPMIIdToPMIPtr.end())
			{
				((Stk_PMIDocumentImpl*)m_pImpl)->m_mapPMIIdToPMIPtr.insert(make_pair(pmiP->GetID(), pmiP));
			}
			return	STK_SUCCESS;
		}

		STK_STATUS Stk_PMIDocument::DelPMI(unsigned int modelID, unsigned int pmiID)
		{
			map<STK_ID, vector<STK_ID>>::iterator itorMapModelPMI = ((Stk_PMIDocumentImpl*)m_pImpl)->m_mapModelIdToPMIList.find(modelID);
			if (itorMapModelPMI == ((Stk_PMIDocumentImpl*)m_pImpl)->m_mapModelIdToPMIList.end())
				return STK_ERROR;
			vector<STK_ID> vecPMIPtr = itorMapModelPMI->second;
			vector<STK_ID>::iterator itVecPMIPtr = vecPMIPtr.begin();
			while (itVecPMIPtr != vecPMIPtr.end())
			{
				if ((*itVecPMIPtr) == pmiID)
				{
					vecPMIPtr.erase(itVecPMIPtr);
					break;
				}
				itVecPMIPtr++;
			} 
			return STK_SUCCESS;
		}
	}
}
