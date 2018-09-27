// All Rights Reserved. Copyright (C) 2017 Hoteamsoft. 
/**************************************************************************
*	@class	Stk_AttributeDocument
*	@brief	属性文档类
*	@par 历史记录
*				2017/07/03 创建。qinyp(Hoteamsoft)
**************************************************************************/
#include "Stk_AttributeCollection.h"
#include "Stk_BaseDocumentImpl.h"
#include "Stk_Attribute.h"
#include "ChineseCode.h"
#include "Stk_MeshAttribute.h"

#define  ATTRIBUTEDOC_NODE_NAME_VERSION  "version"
#define  ATTRIBUTEDOC_NODE_NAME_INSTANCES "instances"
#define  ATTRIBUTEDOC_NODE_NAME_MODELS    "models"
#define  ATTRIBUTEDOC_NODE_NAME_MESHES    "meshes"
#define  ATTRIBUTEDOC_NODE_NAME_MESHFACES "meshFaces"
#define  ATTRIBUTEDOC_NODE_NAME_MESHEDGES "meshEdges"

#define  ATTRIBUTE_NODE_NAME_NAME "name"
#define  ATTRIBUTE_NODE_NAME_ID   "id"
#define  ATTRIBUTE_NODE_NAME_ATTRIBUTES "attributes"

namespace HoteamSoft {
	namespace SVLLib {
		class Stk_Attribute;
		class Stk_AttributeCollectionImpl : public Stk_BaseDocumentImpl
		{
			STK_CLASS_IMPL_DECLARE(Stk_AttributeCollection)
		public:
			Stk_AttributeCollectionImpl(void);
			virtual ~Stk_AttributeCollectionImpl(void);
		public:
			map<STK_ID, map<wstring, wstring>>          m_mapInsAttr; //实例属性
			map<STK_ID, map<wstring, wstring>>          m_mapModelAttr;//模型属性
			map<STK_ID, map<wstring, wstring>>       m_mapMeshFaceAttr;//MeshFace属性
			map<STK_ID, map<wstring, wstring>>       m_mapMeshEdgeAttr; //MeshEdge属性
		public:
			/****************************************************************************
			*	@brief		初始化对象数据
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		无
			*	@note		无
			*	@attention
			****************************************************************************/
			virtual STK_STATUS FromJson(Document& jsonDoc);

			/****************************************************************************
			*	@brief		将数据转换为Json对象
			*	@param[in]	无
			*	@param[out]	jsonDoc		Json文档对象
			*	@retval		STK_SUCCESS			读取成功
			*	@retval		其他				读取失败
			*	@note		无
			*	@attention
			****************************************************************************/
			virtual STK_STATUS ToJson(Document& jsonDoc);

			/****************************************************************************
			*	@brief		添加实例属性
			*	@param[in]  i_iOwnerID          属主ID
			*	@param[in]  i_strName           属性名称
			*	@param[in]  i_strValue          属性值
			*	@param[out]	无
			*	@retval		STK_STATUS 成功与否
			*	@note		无
			*	@attention
			****************************************************************************/
			STK_STATUS AddInstanceAttribute(const STK_ID& i_iOwnerID, const wstring& i_strName, const wstring& i_strValue);

			/****************************************************************************
			*	@brief		获取实例属性
			*	@param[in]  i_iOwnerID          属主ID
			*	@param[in]  i_strName           属性名称
			*	@param[out]	无
			*	@retval		属性值
			*	@note		无
			*	@attention
			****************************************************************************/
			 wstring GetInstanceAttribute(const STK_ID& i_iOwnerID, const wstring& i_strName);
			
			/****************************************************************************
			*	@brief		获取实例所有属性
			*	@param[in]  i_iOwnerID          属主ID
			*	@param[out]	无
			*	@retval		属性值
			*	@note		无
			*	@attention
			****************************************************************************/
			map<wstring, wstring>  GetAllInstanceAttributes(const STK_ID& i_iOwner);
			
			/****************************************************************************
			*	@brief		获取实例所有属性
			*	@param[in]  i_iOwnerID          属主ID
			*	@param[out]	无
			*	@retval		属性值
			*	@note		无
			*	@attention
			****************************************************************************/
			//map<wstring, wstring>  GetMeshAllAttributes(const STK_ID& i_iOwner);

			/****************************************************************************
			*	@brief		添加模型属性
			*	@param[in]  i_iOwnerID          属主ID
			*	@param[in]  i_strName           属性名称
			*	@param[in]  i_strValue          属性值
			*	@param[out]	无
			*	@retval		STK_STATUS 成功与否
			*	@note		无
			*	@attention
			****************************************************************************/
			STK_STATUS AddModelAttribute(const STK_ID& i_iOwnerID, const wstring& i_strName, const wstring& i_strValue);
			
			/****************************************************************************
			*	@brief		获取实例属性
			*	@param[in]  i_iOwnerID          属主ID
			*	@param[in]  i_strName           属性名称
			*	@param[out]	无
			*	@retval		属性值
			*	@note		无
			*	@attention
			****************************************************************************/
			const wstring& GetModelAttribute(const STK_ID& i_iOwnerID, const wstring& i_strName);

			/****************************************************************************
			*	@brief		获取实例所有属性
			*	@param[in]  i_iOwnerID          属主ID
			*	@param[out]	无
			*	@retval		属性值
			*	@note		无
			*	@attention
			****************************************************************************/
			map<wstring, wstring>  GetAllModelAttributes(const STK_ID& i_iOwner);
			
			/****************************************************************************
			*	@brief		根据类型和id查找属性对象
			*	@param[in]	i_eAttrOwnerType 属主类型
			*	@param[in]	i_iOwnerID 属主ID
			*	@retval		Stk_AttributePtr 属性对象
			*	@note		无
			*	@attention
			****************************************************************************/
			STK_BOOL FindAttribute(const StkAttributeOwnerType& i_eAttrOwnerType, const STK_ID& i_iID);
#if 0
			/****************************************************************************
			*	@brief		获取指定ID的MeshFace的属性
			*	@param[in]	i_iOwnerID 属主ID
			*	@retval		Stk_AttributePtr 属性对象
			*	@note		无
			*	@attention
			****************************************************************************/
			map<wstring, wstring>     GetMeshFaceAllAttribute(const STK_ID& i_iOwnerID);

			/****************************************************************************
			*	@brief	    获取MeshFace的属性
			*	@param[in]	
			*	@retval		成功与否
			*	@note		无
			*	@attention
			****************************************************************************/
			STK_STATUS               AddMeshFaceAttribute(const STK_ID& i_iOwnerID, const wstring& i_strName, const wstring& i_strValue);

			/****************************************************************************
			*	@brief		获取指定ID的MeshEdge的属性
			*	@param[in]	i_iOwnerID 属主ID
			*	@retval		Stk_AttributePtr 属性对象
			*	@note		无
			*	@attention
			****************************************************************************/
			map<wstring, wstring>      GetMeshEdgeAllAttribute(const STK_ID& i_iOwnerID);

			/****************************************************************************
			*	@brief	    设置MeshEdge的属性
			*	@param[in]	i_MeshAttrP
			*	@retval		成功与否
			*	@note		无
			*	@attention
			****************************************************************************/
			STK_STATUS                AddMeshEdgeAttribute(const STK_ID& i_iOwnerID, const wstring& i_strName, const wstring& i_strValue);

			/****************************************************************************
			*	@brief		获取MeshFace属性
			*	@param[in]  i_iOwnerID          属主ID
			*	@param[in]  i_strName           属性名称
			*	@param[out]	无
			*	@retval		属性值
			*	@note		无
			*	@attention
			****************************************************************************/
			wstring GetMeshFaceAttribute(const STK_ID& i_iOwnerID, const wstring& i_strName);

			/****************************************************************************
			*	@brief		获取MeshEdge属性
			*	@param[in]  i_iOwnerID          属主ID
			*	@param[in]  i_strName           属性名称
			*	@param[out]	无
			*	@retval		属性值
			*	@note		无
			*	@attention
			****************************************************************************/
			wstring GetMeshEdgeAttribute(const STK_ID& i_iOwnerID, const wstring& i_strName);
#endif
			void unLoad();
		};

#if 0
		wstring Stk_AttributeCollectionImpl::GetMeshFaceAttribute(const STK_ID& i_iOwnerID, const wstring& i_strName)
		{
			map<STK_ID, map<wstring, wstring>>::iterator itorMap = m_mapMeshFaceAttr.find(i_iOwnerID);
			if (itorMap == m_mapMeshFaceAttr.end())
			{
				return L"";
			}
			else
			{
				map<wstring, wstring>::iterator itorAttr = itorMap->second.find(i_strName);
				if (itorAttr == itorMap->second.end())
					return L"";
				else
					return itorAttr->second;
			}
		}

		wstring Stk_AttributeCollectionImpl::GetMeshEdgeAttribute(const STK_ID& i_iOwnerID, const wstring& i_strName)
		{
			map<STK_ID, map<wstring, wstring>>::iterator itorMap = m_mapMeshEdgeAttr.find(i_iOwnerID);
			if (itorMap == m_mapMeshEdgeAttr.end())
			{
				return L"";
			}
			else
			{
				map<wstring, wstring>::iterator itorAttr = itorMap->second.find(i_strName);
				if (itorAttr == itorMap->second.end())
					return L"";
				else
					return itorAttr->second;
			}
		}

		STK_STATUS Stk_AttributeCollectionImpl::AddMeshEdgeAttribute(const STK_ID& i_iOwnerID, const wstring& i_strName, const wstring& i_strValue)
		{
			map<STK_ID, map<wstring, wstring>>::iterator itorMap = m_mapMeshEdgeAttr.find(i_iOwnerID);
			if (itorMap == m_mapMeshEdgeAttr.end())
			{
				map<wstring, wstring> mapAttr;
				mapAttr.insert(make_pair(i_strName, i_strValue));
				m_mapMeshEdgeAttr.insert(make_pair(i_iOwnerID, mapAttr));
			}
			else
			{
				itorMap->second.insert(make_pair(i_strName, i_strValue));
			}
			return STK_SUCCESS;
		}

		map<wstring, wstring> Stk_AttributeCollectionImpl::GetMeshEdgeAllAttribute(const STK_ID& i_iOwnerID)
		{
			map<wstring, wstring> mapAttribute;
			map<STK_ID, map<wstring, wstring>>::iterator itorMap = m_mapMeshEdgeAttr.find(i_iOwnerID);
			if (itorMap == m_mapMeshEdgeAttr.end())
				return mapAttribute;
			else
				mapAttribute = itorMap->second;
			return mapAttribute;
		}

		STK_STATUS   Stk_AttributeCollectionImpl::AddMeshFaceAttribute(const STK_ID& i_iOwnerID, const wstring& i_strName, const wstring& i_strValue)
		{
			map<STK_ID, map<wstring, wstring>>::iterator itorMap = m_mapMeshFaceAttr.find(i_iOwnerID);
			if (itorMap == m_mapMeshFaceAttr.end())
			{
				map<wstring, wstring> mapAttr;
				mapAttr.insert(make_pair(i_strName, i_strValue));
				m_mapMeshFaceAttr.insert(make_pair(i_iOwnerID, mapAttr));
			}
			else
			{
				itorMap->second.insert(make_pair(i_strName, i_strValue));
			}
			return STK_SUCCESS;
		}

		map<wstring, wstring>     Stk_AttributeCollectionImpl::GetMeshFaceAllAttribute(const STK_ID& i_iOwnerID)
		{
			map<wstring, wstring> mapAttribute;
			map<STK_ID, map<wstring, wstring>>::iterator itorMap = m_mapMeshFaceAttr.find(i_iOwnerID);
			if (itorMap == m_mapInsAttr.end())
				return mapAttribute;
			else
				mapAttribute = itorMap->second;
			return mapAttribute;
		}
#endif
		void Stk_AttributeCollectionImpl::unLoad()
		{
			map<STK_ID, map<wstring, wstring>> emptyMap;
			map<STK_ID, map<wstring, wstring>>::iterator it;
			//卸载实例属性信息
			for (it = m_mapInsAttr.begin(); it != m_mapInsAttr.end(); it++)
			{
				map<wstring, wstring> subEmptyMap;
				it->second.swap(subEmptyMap);
				it->second.clear();
			}
			m_mapInsAttr.swap(emptyMap);
			m_mapInsAttr.clear();
			//卸载Model属性
			for (it = m_mapModelAttr.begin(); it != m_mapModelAttr.end(); it++)
			{
				map<wstring, wstring> subEmptyMap;
				it->second.swap(subEmptyMap);
				it->second.clear();
			}
			m_mapModelAttr.swap(emptyMap);
			m_mapModelAttr.clear();
		}

		STK_BOOL Stk_AttributeCollectionImpl::FindAttribute(const StkAttributeOwnerType& i_eAttrOwnerType, const STK_ID& i_iID)
		{
			STK_BOOL bRet = false;
			if (i_eAttrOwnerType == STKAttributeOwnerType_Instance)
			{
				map<STK_ID, map<wstring, wstring>>::iterator itorInsAttr = m_mapInsAttr.find(i_iID);
				if (itorInsAttr != m_mapInsAttr.end())
					bRet = true;
			}
			else if (i_eAttrOwnerType == STKAttributeOwnerType_Model)
			{
				map<STK_ID, map<wstring, wstring>>::iterator itorModelAttr = m_mapModelAttr.find(i_iID);
				if (itorModelAttr != m_mapModelAttr.end())
					bRet = true;
			}
			return	bRet;
		}

		STK_STATUS Stk_AttributeCollectionImpl::AddInstanceAttribute(const STK_ID& i_iOwnerID, const wstring& i_strName, const wstring& i_strValue)
		{
			map<STK_ID, map<wstring, wstring>>::iterator itorMap = m_mapInsAttr.find(i_iOwnerID);
			if (itorMap == m_mapInsAttr.end())
			{
				map<wstring, wstring> mapAttr;
				mapAttr.insert(make_pair(i_strName, i_strValue));
				m_mapInsAttr.insert(make_pair(i_iOwnerID, mapAttr));
			}
			else
			{
				itorMap->second.insert(make_pair(i_strName, i_strValue));
			}
			return STK_SUCCESS;
		}

		wstring Stk_AttributeCollectionImpl::GetInstanceAttribute(const STK_ID& i_iOwnerID, const wstring& i_strName)
		{
			map<STK_ID, map<wstring, wstring>>::iterator itorMap = m_mapInsAttr.find(i_iOwnerID);
			if (itorMap == m_mapInsAttr.end())
			{
				return L"";
			}
			else
			{
				map<wstring, wstring>::iterator itorAttr = itorMap->second.find(i_strName);
				if (itorAttr == itorMap->second.end())
					return nullptr;
				else
					return itorAttr->second;
			}
		}

		map<wstring, wstring>  Stk_AttributeCollectionImpl::GetAllInstanceAttributes(const STK_ID& i_iOwnerID)
		{
			map<wstring, wstring> mapAttribute;
			map<STK_ID, map<wstring, wstring>>::iterator itorMap = m_mapInsAttr.find(i_iOwnerID);
			if (itorMap == m_mapInsAttr.end())
				return mapAttribute;
			else
				mapAttribute = itorMap->second;
			return mapAttribute;
		}

		const wstring& Stk_AttributeCollectionImpl::GetModelAttribute(const STK_ID& i_iOwnerID, const wstring& i_strName)
		{
			map<STK_ID, map<wstring, wstring>>::iterator itorMap = m_mapModelAttr.find(i_iOwnerID);
			if (itorMap == m_mapModelAttr.end())
			{
				return L"";
			}
			else
			{
				map<wstring, wstring>::iterator itorAttr = itorMap->second.find(i_strName);
				if (itorAttr == itorMap->second.end())
					return L"";
				else
					return itorAttr->second;
			}
		}

		map<wstring, wstring>  Stk_AttributeCollectionImpl::GetAllModelAttributes(const STK_ID& i_iOwnerID)
		{
			map<wstring, wstring> mapAttribute;
			map<STK_ID, map<wstring, wstring>>::iterator itorMap = m_mapModelAttr.find(i_iOwnerID);
			if (itorMap == m_mapModelAttr.end())
				return mapAttribute;
			else
				mapAttribute = itorMap->second;
			return mapAttribute;
		}

		STK_STATUS Stk_AttributeCollectionImpl::AddModelAttribute(const STK_ID& i_iOwnerID, const wstring& i_strName, const wstring& i_strValue)
		{
			map<STK_ID, map<wstring, wstring>>::iterator itorMap = m_mapModelAttr.find(i_iOwnerID);
			if (itorMap == m_mapModelAttr.end())
			{
				map<wstring, wstring> mapAttr;
				mapAttr.insert(make_pair(i_strName, i_strValue));
				m_mapModelAttr.insert(make_pair(i_iOwnerID, mapAttr));
			}
			else
			{
				itorMap->second.insert(make_pair(i_strName, i_strValue));
			}
			return STK_SUCCESS;
		}

		STK_STATUS Stk_AttributeCollectionImpl::ToJson(Document& jsonDoc)
		{
			STK_STATUS			ret = STK_SUCCESS;
			jsonDoc.RemoveAllMembers();
			if (m_mapInsAttr.size() <= 0)
				return STK_ERROR;
			
			//version
			string strVersion;
			//strVersion = ChineseCode::WStringToString(m_strVersion);
			ChineseCode::UnicodeToUTF_8(m_strVersion.c_str(), strVersion);
			Value valVersion;
			jsonDoc.AddMember(ATTRIBUTEDOC_NODE_NAME_VERSION, strVersion, jsonDoc.GetAllocator());
			//instances
			Value valInstances;
			valInstances.SetArray();
			map<STK_ID, map<wstring, wstring>>::iterator itorInsMap = m_mapInsAttr.begin();
			while (itorInsMap != m_mapInsAttr.end())
			{
				STK_ID stkID = itorInsMap->first;
				Value valInstance;
				valInstance.SetObject();
				valInstance.AddMember(ATTRIBUTE_NODE_NAME_ID, stkID, jsonDoc.GetAllocator());
				map<wstring, wstring>::iterator itorAttrMap = itorInsMap->second.begin();
				while (itorAttrMap != itorInsMap->second.end())
				{
					string strName, strValue;
					//strName = ChineseCode::UnicodeToUTF_8(itorAttrMap->first);
					//strValue = ChineseCode::WStringToString(itorAttrMap->second);
					ChineseCode::UnicodeToUTF_8(itorAttrMap->first.c_str(), strName);
					ChineseCode::UnicodeToUTF_8(itorAttrMap->second.c_str(), strValue);
					Value name(StringRef(strName), jsonDoc.GetAllocator());
					valInstance.AddMember(name, strValue, jsonDoc.GetAllocator());
					itorAttrMap++;
				}
				valInstances.PushBack(valInstance, jsonDoc.GetAllocator());
				itorInsMap++;
			}
			jsonDoc.AddMember(ATTRIBUTEDOC_NODE_NAME_INSTANCES, valInstances, jsonDoc.GetAllocator());
			
			//models
			Value valModels;
			valModels.SetArray();
			map<STK_ID, map<wstring, wstring>>::iterator itorModelMap = m_mapModelAttr.begin();
			while (itorModelMap != m_mapModelAttr.end())
			{
				STK_ID stkID = itorModelMap->first;
				Value valModel;
				valModel.SetObject();
				valModel.AddMember(ATTRIBUTE_NODE_NAME_ID, stkID, jsonDoc.GetAllocator());
				map<wstring, wstring>::iterator itorAttrMap = itorModelMap->second.begin();
				while (itorAttrMap != itorModelMap->second.end())
				{
					string strName, strValue;
					//strName = ChineseCode::WStringToString(itorAttrMap->first);
					//strValue = ChineseCode::WStringToString(itorAttrMap->second);
					ChineseCode::UnicodeToUTF_8(itorAttrMap->first.c_str(), strName);
					ChineseCode::UnicodeToUTF_8(itorAttrMap->second.c_str(), strValue);
					Value name(StringRef(strName), jsonDoc.GetAllocator());
					valModel.AddMember(name, strValue, jsonDoc.GetAllocator());
					itorAttrMap++;
				}
				valModels.PushBack(valModel, jsonDoc.GetAllocator());
				itorModelMap++;
			}
			jsonDoc.AddMember(ATTRIBUTEDOC_NODE_NAME_MODELS , valModels, jsonDoc.GetAllocator());

			//meshes
#if 0
			//meshFaces
			Value valMeshFaces;
			valMeshFaces.SetArray();
			map<STK_ID, map<wstring, wstring>>::iterator itorMeshFaceMap = m_mapMeshFaceAttr.begin();
			while (itorMeshFaceMap != m_mapMeshFaceAttr.end())
			{
				STK_ID stkID = itorMeshFaceMap->first;
				Value valMeshFace;
				valMeshFace.SetObject();
				valMeshFace.AddMember(ATTRIBUTE_NODE_NAME_ID, stkID, jsonDoc.GetAllocator());
				map<wstring, wstring>::iterator itorAttrMap = itorMeshFaceMap->second.begin();
				while (itorAttrMap != itorMeshFaceMap->second.end())
				{
					string strName, strValue;
					//strName = ChineseCode::WStringToString(itorAttrMap->first);
					//strValue = ChineseCode::WStringToString(itorAttrMap->second);
					ChineseCode::UnicodeToUTF_8(itorAttrMap->first.c_str(), strName);
					ChineseCode::UnicodeToUTF_8(itorAttrMap->second.c_str(), strValue);
					Value name(StringRef(strName), jsonDoc.GetAllocator());
					valMeshFace.AddMember(name, strValue, jsonDoc.GetAllocator());
					itorAttrMap++;
				}
				valMeshFaces.PushBack(valMeshFace, jsonDoc.GetAllocator());
				itorMeshFaceMap++;
			}
			jsonDoc.AddMember(ATTRIBUTEDOC_NODE_NAME_MESHFACES, valMeshFaces, jsonDoc.GetAllocator());
			//meshEdges
			Value valMeshEdges;
			valMeshEdges.SetArray();
			map<STK_ID, map<wstring, wstring>>::iterator itorMeshEdgeMap = m_mapMeshEdgeAttr.begin();
			while (itorMeshEdgeMap != m_mapMeshEdgeAttr.end())
			{
				STK_ID stkID = itorMeshEdgeMap->first;
				Value valMeshEdge;
				valMeshEdge.SetObject();
				valMeshEdge.AddMember(ATTRIBUTE_NODE_NAME_ID, stkID, jsonDoc.GetAllocator());
				map<wstring, wstring>::iterator itorAttrMap = itorMeshEdgeMap->second.begin();
				while (itorAttrMap != itorMeshEdgeMap->second.end())
				{
					string strName, strValue;
					//strName = ChineseCode::WStringToString(itorAttrMap->first);
					//strValue = ChineseCode::WStringToString(itorAttrMap->second);
					ChineseCode::UnicodeToUTF_8(itorAttrMap->first.c_str(), strName);
					ChineseCode::UnicodeToUTF_8(itorAttrMap->second.c_str(), strValue);
					Value name(StringRef(strName), jsonDoc.GetAllocator());
					valMeshEdge.AddMember(name, strValue, jsonDoc.GetAllocator());
					itorAttrMap++;
				}
				valMeshEdges.PushBack(valMeshEdge, jsonDoc.GetAllocator());
				itorMeshEdgeMap++;
			}
			jsonDoc.AddMember(ATTRIBUTEDOC_NODE_NAME_MESHEDGES, valMeshEdges, jsonDoc.GetAllocator());
#endif
			return ret;
		}

		STK_STATUS Stk_AttributeCollectionImpl::FromJson(Document& jsonDoc)
		{
			//version
			Value::MemberIterator versionNodeIter = jsonDoc.FindMember(ATTRIBUTEDOC_NODE_NAME_VERSION);
			if (versionNodeIter != jsonDoc.MemberEnd())
			{
				string strVersion = versionNodeIter->value.GetString();
				m_strVersion = ChineseCode::UTF_8ToUnicode(strVersion.c_str());
			}
			//instances
			Value::MemberIterator instancesNodeIter = jsonDoc.FindMember(ATTRIBUTEDOC_NODE_NAME_INSTANCES);
			if (instancesNodeIter != jsonDoc.MemberEnd())
			{
				Value valInstances = instancesNodeIter->value.GetArray();
				Value valInstance;
				for (int i = 0; i < valInstances.Size(); i++)
				{
					valInstance = valInstances[i];
					STK_ID stkID;
					Value::MemberIterator memberIter = valInstance.MemberBegin();
					while (memberIter != valInstance.MemberEnd())
					{
						wstring strName = ChineseCode::UTF_8ToUnicode(memberIter->name.GetString());
						if (strName == L"id")
							stkID = memberIter->value.GetInt();
						else{
							wstring strValue = ChineseCode::UTF_8ToUnicode(memberIter->value.GetString());

							AddInstanceAttribute(stkID, strName, strValue);
						}
						memberIter++;
					}
				}
			}
			//models
			Value::MemberIterator modelsNodeIter = jsonDoc.FindMember(ATTRIBUTEDOC_NODE_NAME_MODELS);
			if (modelsNodeIter != jsonDoc.MemberEnd())
			{
				Value valModels = modelsNodeIter->value.GetArray();
				Value valModel;
				for (int i = 0; i < valModels.Size(); i++)
				{
					valModel = valModels[i];
					STK_ID stkID;
					Value::MemberIterator memberIter = valModel.MemberBegin();
					while (memberIter != valModel.MemberEnd())
					{
						wstring strName = ChineseCode::UTF_8ToUnicode(memberIter->name.GetString());
						if (strName == L"id")
							stkID = memberIter->value.GetInt();
						else {
							wstring strValue = ChineseCode::UTF_8ToUnicode(memberIter->value.GetString());
							AddModelAttribute(stkID, strName, strValue);
						}
						memberIter++;
					}
				}
			}
			//meshes
			
#if 0
			//meshFaces
			Value::MemberIterator meshFacesNodeIter = jsonDoc.FindMember(ATTRIBUTEDOC_NODE_NAME_MESHFACES);
			if (meshFacesNodeIter != jsonDoc.MemberEnd())
			{
				Value valMeshFaces = meshFacesNodeIter->value.GetArray();
				Value valMeshFace;
				for (int i = 0; i < valMeshFaces.Size(); i++)
				{
					valMeshFace = valMeshFaces[i];
					STK_ID stkID;
					Value::MemberIterator memberIter = valMeshFace.MemberBegin();
					while (memberIter != valMeshFace.MemberEnd())
					{
						wstring strName = ChineseCode::UTF_8ToUnicode(memberIter->name.GetString());
						if (strName == L"id")
							stkID = memberIter->value.GetInt();
						else {
							wstring strValue = ChineseCode::UTF_8ToUnicode(memberIter->value.GetString());
							AddMeshFaceAttribute(stkID, strName, strValue);
						}
						memberIter++;
					}
				}
			}
			//meshEdges
			Value::MemberIterator meshEdgesNodeIter = jsonDoc.FindMember(ATTRIBUTEDOC_NODE_NAME_MESHEDGES);
			if (meshEdgesNodeIter != jsonDoc.MemberEnd())
			{
				Value valMeshEdges = meshEdgesNodeIter->value.GetArray();
				Value valMeshEdge;
				for (int i = 0; i < valMeshEdges.Size(); i++)
				{
					valMeshEdge = valMeshEdges[i];
					STK_ID stkID;
					Value::MemberIterator memberIter = valMeshEdge.MemberBegin();
					while (memberIter != valMeshEdge.MemberEnd())
					{
						wstring strName = ChineseCode::UTF_8ToUnicode(memberIter->name.GetString());
						if (strName == L"id")
							stkID = memberIter->value.GetInt();
						else {
							wstring strValue = ChineseCode::UTF_8ToUnicode(memberIter->value.GetString());
							AddMeshEdgeAttribute(stkID, strName, strValue);
						}
						memberIter++;
					}
				}
			}
#endif
			return STK_SUCCESS;
		}

		Stk_AttributeCollectionImpl::Stk_AttributeCollectionImpl()
		{
			m_strVersion = L"2.0.0";
		}

		Stk_AttributeCollectionImpl::~Stk_AttributeCollectionImpl()
		{

		}

		STK_CLASS_IMPL_IMPLEMENT(Stk_AttributeCollection)

		Stk_AttributeCollection::Stk_AttributeCollection() :
			Stk_BaseDocument(new Stk_AttributeCollectionImpl())
		{

		}

		Stk_AttributeCollection::~Stk_AttributeCollection()
		{
			DELETE_STK_CLASS_IMPL
		}

		STK_IMPLEMENT_CLASS(Stk_AttributeCollection, Stk_BaseDocument)

		void Stk_AttributeCollection::SetVersion(const wstring wstrVersion)
		{
			m_pImpl->m_strVersion = wstrVersion;
		}

		const wstring&   Stk_AttributeCollection::GetVersion()
		{
			return m_pImpl->m_strVersion;
		}

		STK_STATUS Stk_AttributeCollection::AddInstanceAttribute(const STK_ID& i_iOwnerID, const wstring& i_strName, const wstring& i_strValue)
		{
			return ((Stk_AttributeCollectionImpl*)m_pImpl)->AddInstanceAttribute(i_iOwnerID, i_strName, i_strValue);
		}

		wstring Stk_AttributeCollection::GetInstanceAttribute(const STK_ID& i_iOwnerID, const wstring& i_strName)
		{
			return ((Stk_AttributeCollectionImpl*)m_pImpl)->GetInstanceAttribute(i_iOwnerID, i_strName);
		}

		map<wstring, wstring>   Stk_AttributeCollection::GetInstanceAllAttributes(const STK_ID& i_iOwnerID)
		{
			return ((Stk_AttributeCollectionImpl*)m_pImpl)->GetAllInstanceAttributes(i_iOwnerID);
		}

		const wstring& Stk_AttributeCollection::GetModelAttribute(const STK_ID& i_iOwnerID, const wstring& i_strName)
		{
			return ((Stk_AttributeCollectionImpl*)m_pImpl)->GetModelAttribute(i_iOwnerID, i_strName);
		}

		map<wstring, wstring>   Stk_AttributeCollection::GetModelAllAttributes(const STK_ID& i_iOwnerID)
		{
			return ((Stk_AttributeCollectionImpl*)m_pImpl)->GetAllModelAttributes(i_iOwnerID);
		}

		STK_STATUS Stk_AttributeCollection::AddModelAttribute(const STK_ID& i_iOwnerID, const wstring& i_strName, const wstring& i_strValue)
		{
			return ((Stk_AttributeCollectionImpl*)m_pImpl)->AddModelAttribute(i_iOwnerID, i_strName, i_strValue);
		}

		STK_BOOL Stk_AttributeCollection::FindAttribute(const StkAttributeOwnerType& i_eAttrOwnerType, const STK_ID& i_iOwnerID)
		{
			return ((Stk_AttributeCollectionImpl*)m_pImpl)->FindAttribute(i_eAttrOwnerType, i_iOwnerID);
		}

		void Stk_AttributeCollection::unLoad()
		{
			((Stk_AttributeCollectionImpl*)m_pImpl)->unLoad();
			Stk_BaseDocument::unLoad();
		}
#if 0
		map<wstring, wstring>   Stk_AttributeCollection::GetMeshFaceAllAttribute(const STK_ID& i_iOwnerID)
		{
			return ((Stk_AttributeCollectionImpl*)m_pImpl)->GetMeshFaceAllAttribute(i_iOwnerID);
		}

		STK_STATUS  Stk_AttributeCollection::AddMeshFaceAttribute(const STK_ID& i_iOwnerID, const wstring& i_strName, const wstring& i_strValue)
		{
			return ((Stk_AttributeCollectionImpl*)m_pImpl)->AddMeshFaceAttribute(i_iOwnerID, i_strName, i_strValue);
		}

		map<wstring, wstring> Stk_AttributeCollection::GetMeshEdgeAllAttribute(const STK_ID& i_iOwnerID)
		{
			return ((Stk_AttributeCollectionImpl*)m_pImpl)->GetMeshEdgeAllAttribute(i_iOwnerID);
		}

		STK_STATUS Stk_AttributeCollection::AddMeshEdgeAttribute(const STK_ID& i_iOwnerID, const wstring& i_strName, const wstring& i_strValue)
		{
			return ((Stk_AttributeCollectionImpl*)m_pImpl)->AddMeshEdgeAttribute(i_iOwnerID, i_strName, i_strValue);
		}

		wstring Stk_AttributeCollection::GetMeshFaceAttribute(const STK_ID& i_iOwnerID, const wstring& i_strName)
		{
			return ((Stk_AttributeCollectionImpl*)m_pImpl)->GetMeshFaceAttribute(i_iOwnerID, i_strName);
		}

		wstring Stk_AttributeCollection::GetMeshEdgeAttribute(const STK_ID& i_iOwnerID, const wstring& i_strName)
		{
			return ((Stk_AttributeCollectionImpl*)m_pImpl)->GetMeshEdgeAttribute(i_iOwnerID, i_strName);
		}
#endif

	}
}