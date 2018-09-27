#include "Stk_GeometryAttributeCollection.h"
#include "Stk_BaseDocumentImpl.h"
#include "ChineseCode.h"

#define  GEOATTRIBUTEDOC_NODE_NAME_VERSION  "version"
#define  GEOATTRIBUTEDOC_NODE_NAME_MODELS    "models"
#define  GEOATTRIBUTEDOC_NODE_NAME_MESHFACES "meshFaces"
#define  GEOATTRIBUTEDOC_NODE_NAME_MESHEDGES "meshEdges"
#define  GEOATTRIBUTEDOC_NODE_NAME_GEOMETRIES  "geometries"
#define  GEOATTRIBUTEDOC_NODE_NAME_ID         "id"

namespace HoteamSoft {
	namespace SVLLib {

		class Stk_GeometryAttributeCollectionImpl : public Stk_BaseDocumentImpl
		{
			STK_CLASS_IMPL_DECLARE(Stk_GeometryAttributeCollection)
		public:
			Stk_GeometryAttributeCollectionImpl(void);
			virtual ~Stk_GeometryAttributeCollectionImpl(void);
		public:
			map<STK_ID, map<STK_ID, map<wstring, wstring>>>      m_mapMeshFaceAttr;//MeshFace��������
			map<STK_ID, map<STK_ID, map<wstring, wstring>>>      m_mapMeshEdgeAttr;//MeshEdge��������
		public:
			/****************************************************************************
			*	@brief		��ʼ����������
			*	@param[in]	��
			*	@param[out]	��
			*	@retval		��
			*	@note		��
			*	@attention
			****************************************************************************/
			//virtual STK_STATUS FromJson(Document& jsonDoc);

			/****************************************************************************
			*	@brief		������ת��ΪJson����
			*	@param[in]	��
			*	@param[out]	jsonDoc		Json�ĵ�����
			*	@retval		STK_SUCCESS			��ȡ�ɹ�
			*	@retval		����				��ȡʧ��
			*	@note		��
			*	@attention
			****************************************************************************/
			virtual STK_STATUS ToJson(Document& jsonDoc);

			/****************************************************************************
			*	@brief		��ȡָ��ID��MeshFace������
			*   @param[in]  i_iModelID ģ��ID
			*	@param[in]	i_iOwnerID ����ID
			*   @param[in]  attrType ��������
			*	@retval		Stk_AttributePtr ���Զ���
			*	@note		��
			*	@attention
			****************************************************************************/
			map<wstring, wstring>     GetMeshFaceAllAttribute(const STK_ID& i_iModelID, 
				const STK_ID& i_iOwnerID, StkAttributeType attrType);

			/****************************************************************************
			*	@brief		��ȡָ��Model�µ�MeshFace��ָ������
			*   @param[in]  i_iModelID ģ��ID
			*	@param[in]	i_iOwnerID ����ID
			*   @param[in]  i_strName  ��������
			*   @param[in]  attrType ��������
			*	@retval		Stk_AttributePtr ���Զ���
			*	@note		��
			*	@attention
			****************************************************************************/
			wstring           GetMeshFaceAttribute(const STK_ID& i_iModelID, const STK_ID& i_iOwnerID,
				const wstring& i_strName, StkAttributeType attrType);

			/****************************************************************************
			*	@brief	   ���MeshFace������
			*	@param[in] i_iModel  ģ��ID
			*	@param[in]	i_iOwnerID ����ID
			*   @param[in]  i_strName  ��������
			*   @param[in]  i_strValue ����ֵ
			*   @param[in]  attrType ��������
			*	@retval		�ɹ����
			*	@note		��
			*	@attention
			****************************************************************************/
			STK_STATUS       AddMeshFaceAttribute(const STK_ID& i_iModelID, const STK_ID& i_iOwnerID,
				const wstring& i_strName, const wstring& i_strValue, StkAttributeType attrType/* = StkAttributeType_Geo*/);

			/****************************************************************************
			*	@brief		��ȡָ��ID��MeshEdge������
			*   @param[in]  i_iModelID ģ��ID
			*	@param[in]	i_iOwnerID ����ID
			*   @param[in]  attrType ��������
			*	@retval		Stk_AttributePtr ���Զ���
			*	@note		��
			*	@attention
			****************************************************************************/
			map<wstring, wstring>     GetMeshEdgeAllAttribute(const STK_ID& i_iModelID,
				const STK_ID& i_iOwnerID, StkAttributeType attrType);

			/****************************************************************************
			*	@brief		��ȡָ��Model�µ�MeshEdge��ָ������
			*   @param[in]  i_iModelID ģ��ID
			*	@param[in]	i_iOwnerID ����ID
			*   @param[in]  i_strName  ��������
			*   @param[in]  attrType ��������
			*	@retval		Stk_AttributePtr ���Զ���
			*	@note		��
			*	@attention
			****************************************************************************/
			wstring           GetMeshEdgeAttribute(const STK_ID& i_iModelID, const STK_ID& i_iOwnerID,
				const wstring& i_strName, StkAttributeType attrType);

			/****************************************************************************
			*	@brief	   ���MeshEdge������
			*	@param[in] i_iModel  ģ��ID
			*	@param[in]	i_iOwnerID ����ID
			*   @param[in]  i_strName  ��������
			*   @param[in]  i_strValue ����ֵ
			*   @param[in]  attrType ��������
			*	@retval		�ɹ����
			*	@note		��
			*	@attention
			****************************************************************************/
			STK_STATUS       AddMeshEdgeAttribute(const STK_ID& i_iModelID, const STK_ID& i_iOwnerID,
				const wstring& i_strName, const wstring& i_strValue, StkAttributeType attrType/* = StkAttributeType_Geo*/);
			/****************************************************************************
			*	@brief		ж��
			*	@retval		��
			*	@note		��
			*	@attention
			****************************************************************************/
			void UnLoad();
		};

		void Stk_GeometryAttributeCollectionImpl::UnLoad()
		{
			//ж��MeshFace
			map<STK_ID, map<STK_ID, map<wstring, wstring>>> modelEmptyMap;
			map<STK_ID, map<STK_ID, map<wstring, wstring>>>::iterator itorModel = m_mapMeshFaceAttr.begin();
			while (itorModel != m_mapMeshFaceAttr.end())
			{
				map<STK_ID, map<wstring, wstring>> meshFaceEmptyMap;
				map<STK_ID, map<wstring, wstring>>::iterator itorMeshFace = itorModel->second.begin();
				while (itorMeshFace != itorModel->second.end())
				{
					map<wstring, wstring> attrEmptyMap;
					itorMeshFace->second.swap(attrEmptyMap);
					itorMeshFace->second.clear();
					itorMeshFace++;
				}
				itorModel->second.swap(meshFaceEmptyMap);
				itorModel->second.clear();
				itorModel++;
			}
			m_mapMeshFaceAttr.swap(modelEmptyMap);
			m_mapMeshFaceAttr.clear();

			//ж��MeshEdge
			//map<STK_ID, map<STK_ID, map<wstring, wstring>>> modelEmptyMap;
			/*map<STK_ID, map<STK_ID, map<wstring, wstring>>>::iterator */itorModel = m_mapMeshEdgeAttr.begin();
			while (itorModel != m_mapMeshEdgeAttr.end())
			{
				map<STK_ID, map<wstring, wstring>> meshEdgeEmptyMap;
				map<STK_ID, map<wstring, wstring>>::iterator itorMeshEdge = itorModel->second.begin();
				while (itorMeshEdge != itorModel->second.end())
				{
					map<wstring, wstring> attrEmptyMap;
					itorMeshEdge->second.swap(attrEmptyMap);
					itorMeshEdge->second.clear();
					itorMeshEdge++;
				}
				itorModel->second.swap(meshEdgeEmptyMap);
				itorModel->second.clear();
				itorModel++;
			}
			m_mapMeshEdgeAttr.swap(modelEmptyMap);
			m_mapMeshEdgeAttr.clear();
		}
		

		STK_STATUS Stk_GeometryAttributeCollectionImpl::ToJson(Document& jsonDoc)
		{
			STK_STATUS			ret = STK_SUCCESS;
			jsonDoc.RemoveAllMembers();
			if (m_mapMeshFaceAttr.size() <= 0 && m_mapMeshEdgeAttr.size() <= 0)
				return STK_ERROR;
			//version
			string strVersion;
			ChineseCode::UnicodeToUTF_8(m_strVersion.c_str(), strVersion);
			Value valVersion;
			jsonDoc.AddMember(GEOATTRIBUTEDOC_NODE_NAME_VERSION, strVersion, jsonDoc.GetAllocator());
			//geometries
			Value valGeometries;
			valGeometries.SetObject();
			//models
			Value valModels;
			valModels.SetArray();
			//meshFaces
			map<STK_ID, map<STK_ID, map<wstring, wstring>>>::iterator itorMeshFacesModelAttrMap = m_mapMeshFaceAttr.begin();
			while (itorMeshFacesModelAttrMap != m_mapMeshFaceAttr.end())
			{
				STK_ID stkModelID = itorMeshFacesModelAttrMap->first;
				Value valModel;
				valModel.SetObject();
				valModel.AddMember(GEOATTRIBUTEDOC_NODE_NAME_ID, stkModelID, jsonDoc.GetAllocator());
				
				//MeshFace
				Value valMeshFaces;
				valMeshFaces.SetObject(); 
	
				map<STK_ID, map<wstring, wstring>>::iterator itorMeshFaceAttrMap = itorMeshFacesModelAttrMap->second.begin();
				while (itorMeshFaceAttrMap != itorMeshFacesModelAttrMap->second.end())
				{	
					//key
					char szMeshFaceID[32] = { 0 };
					#ifdef _WIN32
                    sprintf(szMeshFaceID,"%d",itorMeshFaceAttrMap->first);
					#else
                    itoa(itorMeshFaceAttrMap->first, szMeshFaceID, 10);
					#endif
					string strMeshFaceID = szMeshFaceID;
					Value valMeshFaceKey(StringRef(strMeshFaceID), jsonDoc.GetAllocator());

					//values
					Value valMeshFaceValue;
					valMeshFaceValue.SetObject();
					map<wstring, wstring>::iterator itorMeshFaceValue = itorMeshFaceAttrMap->second.begin();
					while (itorMeshFaceValue != itorMeshFaceAttrMap->second.end())
					{
						string strName, strValue;
						ChineseCode::UnicodeToUTF_8(itorMeshFaceValue->first.c_str(), strName);
						ChineseCode::UnicodeToUTF_8(itorMeshFaceValue->second.c_str(), strValue);
						Value name(StringRef(strName), jsonDoc.GetAllocator());
						valMeshFaceValue.AddMember(name, strValue, jsonDoc.GetAllocator());
						itorMeshFaceValue++;
					}
					valMeshFaces.AddMember(valMeshFaceKey, valMeshFaceValue, jsonDoc.GetAllocator());
					itorMeshFaceAttrMap++;
				}
				if(valMeshFaces.MemberCount() > 0)
					valModel.AddMember(GEOATTRIBUTEDOC_NODE_NAME_MESHFACES, valMeshFaces, jsonDoc.GetAllocator());

				//MeshEdge
				//������ͬModelID��MeshEdge
				map<STK_ID, map<STK_ID, map<wstring, wstring>>>::iterator itorMeshEdgesModelAttrMap = m_mapMeshEdgeAttr.find(stkModelID);
				if (itorMeshEdgesModelAttrMap != m_mapMeshEdgeAttr.end())
				{
					Value valMeshEdges;
					valMeshEdges.SetObject();

					map<STK_ID, map<wstring, wstring>>::iterator itorMeshEdgeAttrMap = itorMeshEdgesModelAttrMap->second.begin();
					while (itorMeshEdgeAttrMap != itorMeshEdgesModelAttrMap->second.end())
					{
						//key
						char szMeshEdgeID[32] = { 0 };
											#ifdef _WIN32
                    	itoa(itorMeshEdgeAttrMap->first, szMeshEdgeID, 10);
					#else
                                           sprintf(szMeshEdgeID,"%d",itorMeshEdgeAttrMap->first);
					#endif
					
						string strMeshEdgeID = szMeshEdgeID;
						Value valMeshEdgeKey(StringRef(strMeshEdgeID), jsonDoc.GetAllocator());

						//values
						Value valMeshEdgeValue;
						valMeshEdgeValue.SetObject();
						map<wstring, wstring>::iterator itorMeshEdgeValue = itorMeshEdgeAttrMap->second.begin();
						while (itorMeshEdgeValue != itorMeshEdgeAttrMap->second.end())
						{
							string strName, strValue;
							ChineseCode::UnicodeToUTF_8(itorMeshEdgeValue->first.c_str(), strName);
							ChineseCode::UnicodeToUTF_8(itorMeshEdgeValue->second.c_str(), strValue);
							Value name(StringRef(strName), jsonDoc.GetAllocator());
							valMeshEdgeValue.AddMember(name, strValue, jsonDoc.GetAllocator());
							itorMeshEdgeValue++;
						}
						valMeshEdges.AddMember(valMeshEdgeKey, valMeshEdgeValue, jsonDoc.GetAllocator());
						itorMeshEdgeAttrMap++;
					}
					if (valMeshEdges.MemberCount() > 0)
						valModel.AddMember(GEOATTRIBUTEDOC_NODE_NAME_MESHEDGES, valMeshEdges, jsonDoc.GetAllocator());
				}

				valModels.PushBack(valModel, jsonDoc.GetAllocator());
				itorMeshFacesModelAttrMap++;
			}

			//ʣ��MeshEdge
			map<STK_ID, map<STK_ID, map<wstring, wstring>>>::iterator itorMeshEdgesModelAttrMap = m_mapMeshEdgeAttr.begin();
			while (itorMeshEdgesModelAttrMap != m_mapMeshEdgeAttr.end())
			{
				STK_ID stkModelID = itorMeshEdgesModelAttrMap->first;
				map<STK_ID, map<STK_ID, map<wstring, wstring>>>::iterator itorTmpMeshFacesModelAttrMap = m_mapMeshFaceAttr.find(stkModelID);;
				if (itorTmpMeshFacesModelAttrMap != m_mapMeshFaceAttr.end())
				{
					itorMeshEdgesModelAttrMap++;
					continue;
				}
				Value valModel;
				valModel.SetObject();
				valModel.AddMember(GEOATTRIBUTEDOC_NODE_NAME_ID, stkModelID, jsonDoc.GetAllocator());

				//MeshEdge
				Value valMeshEdges;
				valMeshEdges.SetObject();

				map<STK_ID, map<wstring, wstring>>::iterator itorMeshEdgeAttrMap = itorMeshEdgesModelAttrMap->second.begin();
				while (itorMeshEdgeAttrMap != itorMeshEdgesModelAttrMap->second.end())
				{
					//key
					char szMeshEdgeID[32] = { 0 };
										#ifdef _WIN32
                    					itoa(itorMeshEdgeAttrMap->first, szMeshEdgeID, 10);
					#else
                                       sprintf(szMeshEdgeID,"%d",itorMeshEdgeAttrMap->first);
					#endif

					string strMeshEdgeID = szMeshEdgeID;
					Value valMeshEdgeKey(StringRef(strMeshEdgeID), jsonDoc.GetAllocator());

					//values
					Value valMeshEdgeValue;
					valMeshEdgeValue.SetObject();
					map<wstring, wstring>::iterator itorMeshEdgeValue = itorMeshEdgeAttrMap->second.begin();
					while (itorMeshEdgeValue != itorMeshEdgeAttrMap->second.end())
					{
						string strName, strValue;
						ChineseCode::UnicodeToUTF_8(itorMeshEdgeValue->first.c_str(), strName);
						ChineseCode::UnicodeToUTF_8(itorMeshEdgeValue->second.c_str(), strValue);
						Value name(StringRef(strName), jsonDoc.GetAllocator());
						valMeshEdgeValue.AddMember(name, strValue, jsonDoc.GetAllocator());
						itorMeshEdgeValue++;
					}
					valMeshEdges.AddMember(valMeshEdgeKey, valMeshEdgeValue, jsonDoc.GetAllocator());
					itorMeshEdgeAttrMap++;
				}
				if (valMeshEdges.MemberCount() > 0)
					valModel.AddMember(GEOATTRIBUTEDOC_NODE_NAME_MESHEDGES, valMeshEdges, jsonDoc.GetAllocator());
				valModels.PushBack(valModel, jsonDoc.GetAllocator());
				itorMeshEdgesModelAttrMap++;
			}

			if(valModels.Size() > 0)
				valGeometries.AddMember(GEOATTRIBUTEDOC_NODE_NAME_MODELS, valModels, jsonDoc.GetAllocator());
			jsonDoc.AddMember(GEOATTRIBUTEDOC_NODE_NAME_GEOMETRIES, valGeometries, jsonDoc.GetAllocator());
			//���������������Կ���ͨ�����ҵķ�ʽ���

			return ret;
		}

		STK_STATUS Stk_GeometryAttributeCollectionImpl::AddMeshFaceAttribute(const STK_ID& i_iModelID, const STK_ID& i_iOwnerID,
			const wstring& i_strName, const wstring& i_strValue, StkAttributeType attrType)
		{
			if (attrType == StkAttributeType_Geo)
			{
				map<STK_ID, map<STK_ID, map<wstring, wstring>>>::iterator itorModelMap = m_mapMeshFaceAttr.find(i_iModelID);
				if (itorModelMap == m_mapMeshFaceAttr.end())
				{
					map<STK_ID, map<wstring, wstring>> mapMeshFace;
					map<wstring, wstring> mapAttr;
					mapAttr.insert(make_pair(i_strName, i_strValue));
					mapMeshFace.insert(make_pair(i_iOwnerID, mapAttr));
					m_mapMeshFaceAttr.insert(make_pair(i_iModelID, mapMeshFace));
				}else
				{
					map<STK_ID, map<wstring, wstring>>::iterator itorMeshFaceMap = itorModelMap->second.find(i_iOwnerID);
					if (itorMeshFaceMap == itorModelMap->second.end())
					{
						map<wstring, wstring> mapAttr;
						mapAttr.insert(make_pair(i_strName, i_strValue));
						itorModelMap->second.insert(make_pair(i_iOwnerID, mapAttr));
					}
					else
						itorMeshFaceMap->second.insert(make_pair(i_strName, i_strValue));
				}
			}
			return STK_SUCCESS;
		}

		STK_STATUS Stk_GeometryAttributeCollectionImpl::AddMeshEdgeAttribute(const STK_ID& i_iModelID, const STK_ID& i_iOwnerID,
			const wstring& i_strName, const wstring& i_strValue, StkAttributeType attrType)
		{
			if (attrType == StkAttributeType_Geo)
			{
				map<STK_ID, map<STK_ID, map<wstring, wstring>>>::iterator itorModelMap = m_mapMeshEdgeAttr.find(i_iModelID);
				if (itorModelMap == m_mapMeshEdgeAttr.end())
				{
					map<STK_ID, map<wstring, wstring>> mapMeshEdge;
					map<wstring, wstring> mapAttr;
					mapAttr.insert(make_pair(i_strName, i_strValue));
					mapMeshEdge.insert(make_pair(i_iOwnerID, mapAttr));
					m_mapMeshEdgeAttr.insert(make_pair(i_iModelID, mapMeshEdge));
				}
				else
				{
					map<STK_ID, map<wstring, wstring>>::iterator itorMeshEdgeMap = itorModelMap->second.find(i_iOwnerID);
					if (itorMeshEdgeMap == itorModelMap->second.end())
					{
						map<wstring, wstring> mapAttr;
						mapAttr.insert(make_pair(i_strName, i_strValue));
						itorModelMap->second.insert(make_pair(i_iOwnerID, mapAttr));
					}
					else
						itorMeshEdgeMap->second.insert(make_pair(i_strName, i_strValue));
				}
			}
			return STK_SUCCESS;
		}

		map<wstring, wstring>     Stk_GeometryAttributeCollectionImpl::GetMeshFaceAllAttribute(const STK_ID& i_iModelID, const STK_ID& i_iOwnerID, StkAttributeType attrType)
		{
			map<wstring, wstring> mapAttribute;
			if (attrType == StkAttributeType_Geo)
			{
				map<STK_ID, map<STK_ID, map<wstring, wstring>>>::iterator itorModelMap = m_mapMeshFaceAttr.find(i_iModelID);
				if (itorModelMap == m_mapMeshFaceAttr.end())
				{
					return mapAttribute;
				}
				else
				{
					map<STK_ID, map<wstring, wstring>>::iterator itorMeshFaceMap = itorModelMap->second.find(i_iOwnerID);
					if (itorMeshFaceMap == itorModelMap->second.end())
						return mapAttribute;
					else
						return itorMeshFaceMap->second;
				}
			}
            return mapAttribute;
		}

		map<wstring, wstring>     Stk_GeometryAttributeCollectionImpl::GetMeshEdgeAllAttribute(const STK_ID& i_iModelID, const STK_ID& i_iOwnerID, StkAttributeType attrType)
		{
			map<wstring, wstring> mapAttribute;
			if (attrType == StkAttributeType_Geo)
			{
				map<STK_ID, map<STK_ID, map<wstring, wstring>>>::iterator itorModelMap = m_mapMeshEdgeAttr.find(i_iModelID);
				if (itorModelMap == m_mapMeshEdgeAttr.end())
				{
					return mapAttribute;
				}
				else
				{
					map<STK_ID, map<wstring, wstring>>::iterator itorMeshEdgeMap = itorModelMap->second.find(i_iOwnerID);
					if (itorMeshEdgeMap == itorModelMap->second.end())
						return mapAttribute;
					else
						return itorMeshEdgeMap->second;
				}
			}
            return mapAttribute;
		}

		wstring  Stk_GeometryAttributeCollectionImpl::GetMeshFaceAttribute(const STK_ID& i_iModelID, const STK_ID& i_iOwnerID,
			const wstring& i_strName, StkAttributeType attrType)
		{
			if (attrType == StkAttributeType_Geo)
			{
				map<STK_ID, map<STK_ID, map<wstring, wstring>>>::iterator itorModelMap = m_mapMeshFaceAttr.find(i_iModelID);
				if (itorModelMap == m_mapMeshFaceAttr.end())
				{
					return L"";
				}
				else
				{
					map<STK_ID, map<wstring, wstring>>::iterator itorMeshFaceMap = itorModelMap->second.find(i_iOwnerID);
					if (itorMeshFaceMap == itorModelMap->second.end())
						return L"";
					else
					{
						map<wstring, wstring>::iterator itorAttr = itorMeshFaceMap->second.find(i_strName);
						if (itorAttr == itorMeshFaceMap->second.end())
							return L"";
						else
							return itorAttr->second;
					}
				}
			}
            return NULL;
		}

		wstring  Stk_GeometryAttributeCollectionImpl::GetMeshEdgeAttribute(const STK_ID& i_iModelID, const STK_ID& i_iOwnerID,
			const wstring& i_strName, StkAttributeType attrType)
		{
			if (attrType == StkAttributeType_Geo)
			{
				map<STK_ID, map<STK_ID, map<wstring, wstring>>>::iterator itorModelMap = m_mapMeshEdgeAttr.find(i_iModelID);
				if (itorModelMap == m_mapMeshEdgeAttr.end())
				{
					return L"";
				}
				else
				{
					map<STK_ID, map<wstring, wstring>>::iterator itorMeshEdgeMap = itorModelMap->second.find(i_iOwnerID);
					if (itorMeshEdgeMap == itorModelMap->second.end())
						return L"";
					else
					{
						map<wstring, wstring>::iterator itorAttr = itorMeshEdgeMap->second.find(i_strName);
						if (itorAttr == itorMeshEdgeMap->second.end())
							return L"";
						else
							return itorAttr->second;
					}
				}
			}
            return NULL;
		}

		STK_CLASS_IMPL_IMPLEMENT(Stk_GeometryAttributeCollection)

		Stk_GeometryAttributeCollection::Stk_GeometryAttributeCollection() :
			Stk_BaseDocument(new Stk_GeometryAttributeCollectionImpl())
		{
		}


		Stk_GeometryAttributeCollection::~Stk_GeometryAttributeCollection()
		{
			DELETE_STK_CLASS_IMPL
		}

		STK_IMPLEMENT_CLASS(Stk_GeometryAttributeCollection, Stk_BaseDocument)

		Stk_GeometryAttributeCollectionImpl::Stk_GeometryAttributeCollectionImpl()
		{
			m_strVersion = L"2.0.0";
		}

		Stk_GeometryAttributeCollectionImpl::~Stk_GeometryAttributeCollectionImpl()
		{

		}

		void  Stk_GeometryAttributeCollection::SetVersion(const wstring wstrVersion)
		{
			m_pImpl->m_strVersion = wstrVersion;
		}

		const wstring&  Stk_GeometryAttributeCollection::GetVersion()
		{
			return m_pImpl->m_strVersion;
		}

		map<wstring, wstring> Stk_GeometryAttributeCollection::GetMeshFaceAllAttribute(const STK_ID& i_iModelID, const STK_ID& i_iOwnerID, StkAttributeType attrType)
		{
			return ((Stk_GeometryAttributeCollectionImpl*)m_pImpl)->GetMeshFaceAllAttribute(i_iModelID, i_iOwnerID, attrType);
		}

		wstring  Stk_GeometryAttributeCollection::GetMeshFaceAttribute(const STK_ID& i_iModelID, const STK_ID& i_iOwnerID, const wstring& i_strName, StkAttributeType attrType)
		{
			return ((Stk_GeometryAttributeCollectionImpl*)m_pImpl)->GetMeshFaceAttribute(i_iModelID, i_iOwnerID, i_strName, attrType);
		}

		STK_STATUS       Stk_GeometryAttributeCollection::AddMeshFaceAttribute(const STK_ID& i_iModelID, const STK_ID& i_iOwnerID,
			const wstring& i_strName, const wstring& i_strValue, StkAttributeType attrType)
		{
			return ((Stk_GeometryAttributeCollectionImpl*)m_pImpl)->AddMeshFaceAttribute(i_iModelID, i_iOwnerID, i_strName, i_strValue, attrType);
		}

		map<wstring, wstring> Stk_GeometryAttributeCollection::GetMeshEdgeAllAttribute(const STK_ID& i_iModelID, const STK_ID& i_iOwnerID, StkAttributeType attrType)
		{
			return ((Stk_GeometryAttributeCollectionImpl*)m_pImpl)->GetMeshEdgeAllAttribute(i_iModelID, i_iOwnerID, attrType);
		}

		wstring  Stk_GeometryAttributeCollection::GetMeshEdgeAttribute(const STK_ID& i_iModelID, const STK_ID& i_iOwnerID, const wstring& i_strName, StkAttributeType attrType)
		{
			return ((Stk_GeometryAttributeCollectionImpl*)m_pImpl)->GetMeshEdgeAttribute(i_iModelID, i_iOwnerID, i_strName, attrType);
		}

		STK_STATUS       Stk_GeometryAttributeCollection::AddMeshEdgeAttribute(const STK_ID& i_iModelID, const STK_ID& i_iOwnerID,
			const wstring& i_strName, const wstring& i_strValue, StkAttributeType attrType)
		{
			return ((Stk_GeometryAttributeCollectionImpl*)m_pImpl)->AddMeshEdgeAttribute(i_iModelID, i_iOwnerID, i_strName, i_strValue, attrType);
		}

		void     Stk_GeometryAttributeCollection::UnLoad()
		{
			((Stk_GeometryAttributeCollectionImpl*)m_pImpl)->UnLoad();
			Stk_BaseDocument::unLoad();
		}
	}
}