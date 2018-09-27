// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_View.cpp
 *
 *	@brief	视图类
 *
 *	@par	历史:
 *		2013/08/10	创建。Gaoqq@HOTEAMSOFT
****************************************************************************/

#include "Stk_View.h"
#include "Stk_ViewDocument.h"
#include "Stk_BaseDocumentImpl.h"
#ifdef _WINDOWS_
#include "Stk_Log.h"
#endif
#include "Stk_ObjectImpl.h"
#include "Stk_Declare_Ptr.h"
#include "ChineseCode.h"
#include "Stk_PlaneEntity.h"
#include "Stk_Connector.h"

#define	VIEW_VERSION	0.1f

#define VIEW_NODE_NAME_ID				"id"
#define VIEW_NODE_NAME_NAME				"name"
#define VIEW_NODE_NAME_USAGETYPE		"usageType"
#define VIEW_NODE_NAME_ISACTIVATED		"isActivated"
#define VIEW_NODE_NAME_TRANSPARENCY		"transparency"
#define VIEW_NODE_NAME_CAMERA			"camera"
#define VIEW_NODE_NAME_CLIPPLANES		"clipPlanes"
#define VIEW_NODE_NAME_CONNECTORS		"connectors"
#define VIEW_NODE_NAME_INSATTRIBUTES	"insAttributes"

namespace HoteamSoft {
	namespace SVLLib {
		class Stk_ViewImpl : public Stk_ObjectImpl
		{
			STK_CLASS_IMPL_DECLARE(Stk_View)
		public:
			Stk_ViewImpl(Stk_ViewDocument* i_ViewDocP, int i_iModelID);
			virtual ~Stk_ViewImpl(void);

			/****************************************************************************
			*	@brief		读取文件中的Body数据
			*	@param[in]	i_pJsonNode			View 父Json节点
			*	@param[in]	i_JsonDocP			Json文档对象
			*	@param[out]	无
			*	@retval		STK_SUCCESS			读取成功
			*	@retval		STK_ERROR			读取失败
			*	@retval		STK_ERROR_MEMORY	内存申请失败
			*	@note		无
			*	@attention	内部接口
			****************************************************************************/
			STK_STATUS					FromJson(Value* i_pJsonNode, Document* i_JsonDocP);

			/*****************************************************************************
			*	@brief		写View数据
			*	@param[in]	Value* i_pJsonParentNode			View 父Json节点
			*	@param[in]	i_JsonDocP			Json文档对象
			*	@param[out]	无
			*	@retval		STK_SUCCESS			写入成功
			*	@retval		其它				写入失败
			*	@note		无
			*	@attention	内部接口
			****************************************************************************/
			STK_STATUS					ToJson(Value* i_pJsonNode, Document* i_JsonDocP);

			/****************************************************************************
			*	@brief		添加子实例的显隐状态
			*	@param[in]	i_wstrInsPlcPath	子实例的配置路径
			*	@param[in]	i_eDisplayState		显隐状态
			*	@param[out]	无
			*	@retval		STK_SUCCESS			添加成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS					AddInsDisplayState(wstring i_wstrInsPlcPath, StkDisplayStateEnum i_eDisplayState);

			/****************************************************************************
			*	@brief		获取指定配置路径下子实例的显隐状态
			*	@param[in]	i_wstrInsPlcPath	子实例的配置路径
			*	@param[out]	o_eDisplayState		子实例的显隐状态
			*	@retval		子实例的显隐状态
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS					GetInsDisplayStateByPlcPath(wstring i_wstrInsPlcPath, StkDisplayStateEnum& o_eDisplayState);

			/****************************************************************************
			*	@brief		添加子实例的颜色
			*	@param[in]	i_wstrInsPlcPath	子实例的配置路径
			*	@param[in]	i_rgbaIns			子实例的颜色
			*	@param[out]	无
			*	@retval		STK_SUCCESS			添加成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS					AddInsColor(wstring i_wstrInsPlcPath, STK_RGBA32 i_rgbaIns);

			/****************************************************************************
			*	@brief		获取指定配置路径下的子实例的颜色
			*	@param[in]	i_wstrInsPlcPath	子实例的配置路径
			*	@param[out]	o_rgbaIns			子实例的颜色
			*	@retval		指定的子实例的颜色
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS					GetInsColorByPlcPath(wstring i_wstrInsPlcPath, STK_RGBA32& o_rgbaIns);

			/****************************************************************************
			*	@brief		添加配置路径下子实例的材质
			*	@param[in]	i_wstrInsPlcPath	子实例的配置路径
			*	@param[in]	i_matIns			子实例的材质
			*	@param[out]	无
			*	@retval		STK_SUCCESS			添加成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS					AddInsMaterial(wstring i_wstrInsPlcPath, STK_MAT32 i_matIns);

			/****************************************************************************
			*	@brief		获取指定的子实例的材质
			*	@param[in]	i_wstrInsPlcPath	子实例的配置路径
			*	@param[out]	o_matIns			子实例的材质
			*	@retval		指定的子实例的材质
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS					GetInsMaterialByPlcPath(wstring i_wstrInsPlcPath, STK_MAT32& o_matIns);

			/****************************************************************************
			*	@brief		添加配置路径下子实例相对于父级的配置矩阵
			*	@param[in]	i_wstrInsPlcPath	子实例的配置路径
			*	@param[in]	i_idPlacement		子实例的配置ID
			*	@param[in]	i_mtxInsPlc			子实例的配置矩阵
			*	@param[out]	无
			*	@retval		STK_SUCCESS			添加成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS					AddInsPlacement(wstring i_wstrInsPlcPath, STK_ID i_idPlacement, STK_MTX32 i_mtxInsPlc);

			/****************************************************************************
			*	@brief		获取指定的子实例相对于父级的配置矩阵
			*	@param[in]	i_wstrInsPlcPath	子实例的配置路径
			*	@param[out]	o_idPlacement		子实例的配置ID
			*	@param[out]	o_mtxInsPlc			子实例的配置矩阵
			*	@retval		指定的子实例的配置矩阵
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS					GetInsPlacementByPlcPath(wstring i_wstrInsPlcPath, STK_ID& o_idPlacement, STK_MTX32& o_mtxInsPlc);

			/****************************************************************************
			*	@brief		获取包含实例属性的所有配置路径
			*	@param[in]	无
			*	@param[out]	vcInsPlcPaths	所有配置路径
			*	@retval		无
			*	@note		无
			*	@attention	无
			****************************************************************************/
			void GetAllInsAttrPlcPath(vector<wstring>& vcInsPlcPaths);

			/****************************************************************************
			*	@brief		删除指定配置路径的实例属性
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		STK_SUCCESS	删除成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS					DeleteInsAttrByPlcPath(wstring i_wstrInsPlcPath);

			/****************************************************************************
			*	@brief		删除当前视图中所有的实例属性
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		STK_SUCCESS	删除成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS					DeleteAllInsAttribute();
		public:
			Stk_ViewDocumentPtr m_pViewDoc;
			STK_ID			m_iIdenty;								//!< 视图唯一标识
			STK_ID			m_iViewID;								//!< 视图ID号
			StkViewUsageEnum			m_eUsageType;				//!< 应用类型
			wstring						m_wstrName;					//!< 视图名称
			STK_BOOL					m_bIsActivated;				//!< 是否活动
			Stk_CameraPtr				m_CameraP;					//!< 照相机
			STK_BOOL					m_bHasClipPlane;				//!< 是否有剖面
			vector<Stk_PlaneEntityPtr>	m_vcPlaneEntity;			//!< 多个面
			STK_FLOAT32					m_fTransparency;			//!< 透明度
			map<wstring, Stk_InsAttributePtr>	m_mapInsAttribute;		//!< 子实例属性
			int				m_iModelID;						//!< 所属零件
			map<StkEntityTypeEnum, vector<STK_ID>>    m_mapConnector;  //链接
		};

		STK_CLASS_IMPL_IMPLEMENT(Stk_View)
			Stk_ViewImpl::Stk_ViewImpl(Stk_ViewDocument* i_ViewDocP, int i_iModelID)
		{
			m_pViewDoc = i_ViewDocP;
			m_CameraP = nullptr;
			m_bHasClipPlane = FALSE;
			m_bIsActivated = TRUE;
			m_iModelID = i_iModelID;
			m_eUsageType = VIEW_USAGE_UNKNOWN;
			m_fTransparency = 1.0;
			m_iIdenty = 0;
			m_iViewID = 0;
			m_mapConnector.clear();
		}

		Stk_ViewImpl::~Stk_ViewImpl(void)
		{
			//if(m_pClippingPlan != NULL){
			//	delete m_pClippingPlan;
			//	m_pClippingPlan = NULL;
			//}
#if 0
			for (int ii = 0; ii < m_vcNodeConnectors.size(); ii++)
			{
				if (m_vcNodeConnectors[ii] != NULL) {
					delete m_vcNodeConnectors[ii];
					m_vcNodeConnectors[ii] = NULL;
				}
			}
			for (int ii = 0; ii < m_vcClippingPlans.size(); ii++)
			{
				if (m_vcClippingPlans[ii] != NULL)
				{
					delete m_vcClippingPlans[ii];
					m_vcClippingPlans[ii] = NULL;
				}
			}
#endif
			// 实例属性释放
			m_mapInsAttribute.clear();
			if(m_pViewDoc != nullptr)
			{
				m_pViewDoc->DelView(m_iModelID, m_ID);
			}
		}

		STK_STATUS Stk_ViewImpl::FromJson(Value* i_pJsonNode, Document* i_JsonDocP)
		{
			STK_STATUS			ret = STK_SUCCESS;
			if (!i_pJsonNode)
				return STK_ERROR;

			m_ID = (*i_pJsonNode)[VIEW_NODE_NAME_ID].GetInt();
			m_wstrName = ChineseCode::UTF_8ToUnicode((*i_pJsonNode)[VIEW_NODE_NAME_NAME].GetString());
			m_bIsActivated = (*i_pJsonNode)[VIEW_NODE_NAME_ISACTIVATED].GetBool();
			m_fTransparency = (*i_pJsonNode)[VIEW_NODE_NAME_TRANSPARENCY].GetFloat();
			m_eUsageType = (StkViewUsageEnum)(*i_pJsonNode)[VIEW_NODE_NAME_USAGETYPE].GetInt();

			//读取Camera信息
			Value::MemberIterator cameraNodeIter = i_pJsonNode->FindMember(VIEW_NODE_NAME_CAMERA);
			if (cameraNodeIter != i_pJsonNode->MemberEnd())
			{
				if(m_CameraP == nullptr)
				{
					m_CameraP = Stk_CameraPtr::CreateObject();
				}
				m_CameraP->FromJson(&cameraNodeIter->value, i_JsonDocP);
			}

			//读取剖面信息
			m_vcPlaneEntity.clear();
			Value::MemberIterator clipNodeIter = i_pJsonNode->FindMember(VIEW_NODE_NAME_CLIPPLANES);
			if (clipNodeIter != i_pJsonNode->MemberEnd())
			{
				Value valClipPlanes = clipNodeIter->value.GetArray();
				Value valClipPlane;
				for (int i = 0; i < valClipPlanes.Size(); i++)
				{
					valClipPlane = valClipPlanes[i];
					Stk_ClipPlanePtr clipPlaneP = Stk_ClipPlanePtr::CreateObject();
					clipPlaneP->FromJson(&valClipPlane, i_JsonDocP);
					m_vcPlaneEntity.push_back(clipPlaneP);
				}
			}

			//读取模型状态信息
			Value::MemberIterator insAttrsNodeIter = i_pJsonNode->FindMember(VIEW_NODE_NAME_INSATTRIBUTES);
			if (insAttrsNodeIter != i_pJsonNode->MemberEnd())
			{
				m_mapInsAttribute.clear();
				Value insAttributesValue = insAttrsNodeIter->value.GetObject();
				
				Value::MemberIterator insAttrNodeIter = insAttributesValue.MemberBegin();
				while (insAttrNodeIter != insAttributesValue.MemberEnd())
				{
					Value insAttributeValue = insAttrNodeIter->value.GetObject();
					Stk_InsAttributePtr insAttributeP = Stk_InsAttributePtr::CreateObject();
					insAttributeP->FromJson(&insAttributeValue, i_JsonDocP);
					m_mapInsAttribute.insert(pair<wstring, Stk_InsAttributePtr>(ChineseCode::UTF_8ToUnicode(insAttrNodeIter->name.GetString()), insAttributeP));
					insAttrNodeIter++;
				}
			}

			return ret;
		}

		STK_STATUS Stk_ViewImpl::ToJson(Value* i_pJsonNode, Document* i_JsonDocP)
		{
			STK_STATUS			ret = STK_ERROR;
			if (!i_pJsonNode || !i_JsonDocP)
				return ret;
			i_pJsonNode->SetObject();
			
			i_pJsonNode->AddMember(VIEW_NODE_NAME_ID, m_ID, i_JsonDocP->GetAllocator());
			string strName;
			ChineseCode::UnicodeToUTF_8(m_wstrName.c_str(), strName);
			i_pJsonNode->AddMember(VIEW_NODE_NAME_NAME, strName, i_JsonDocP->GetAllocator());
			i_pJsonNode->AddMember(VIEW_NODE_NAME_USAGETYPE, m_eUsageType, i_JsonDocP->GetAllocator());
			i_pJsonNode->AddMember(VIEW_NODE_NAME_ISACTIVATED, m_bIsActivated, i_JsonDocP->GetAllocator());
			i_pJsonNode->AddMember(VIEW_NODE_NAME_TRANSPARENCY, m_fTransparency, i_JsonDocP->GetAllocator());

			//输出Camera信息
			if (m_CameraP != nullptr)
			{
				Value cameraValue;
				cameraValue.SetObject();
				m_CameraP->ToJson(&cameraValue, i_JsonDocP);
				i_pJsonNode->AddMember(VIEW_NODE_NAME_CAMERA, cameraValue, i_JsonDocP->GetAllocator());
			}

			//clipPlanes
			Value valPlaneEntities;
			valPlaneEntities.SetArray();
			vector<Stk_PlaneEntityPtr>::iterator itorPlaneEntity = m_vcPlaneEntity.begin();
			while (itorPlaneEntity != m_vcPlaneEntity.end())
			{
				if (*itorPlaneEntity == nullptr)
				{
					itorPlaneEntity++;
					continue;
				}
				Value valPlaneEntity;
				(*itorPlaneEntity)->ToJson(&valPlaneEntity, i_JsonDocP);
				valPlaneEntities.PushBack(valPlaneEntity, i_JsonDocP->GetAllocator());
				itorPlaneEntity++;
			}
			if(m_vcPlaneEntity.size() != 0)
				i_pJsonNode->AddMember(VIEW_NODE_NAME_CLIPPLANES, valPlaneEntities, i_JsonDocP->GetAllocator());
			//输出模型状态信息，ProtoType下的所有子实例(包括子级ProtoType的子实例)属性(颜色、显隐、材质)
			Value insAttrs;
			insAttrs.SetObject();
			
			map<wstring, Stk_InsAttributePtr>::iterator itorInsAttr = m_mapInsAttribute.begin();
			while (itorInsAttr != m_mapInsAttribute.end())
			{
				Value insAttrNode; 
				ret = itorInsAttr->second->ToJson(&insAttrNode, i_JsonDocP);
				if (ret == STK_SUCCESS)
				{
					string strAttrNameUTF8 = "";
					ChineseCode::UnicodeToUTF_8(itorInsAttr->first.c_str(), strAttrNameUTF8);
					Value key(StringRef(strAttrNameUTF8), i_JsonDocP->GetAllocator());
					insAttrs.AddMember(key,insAttrNode, i_JsonDocP->GetAllocator());
				}
				itorInsAttr++;
			}
			if(m_mapInsAttribute.size() != 0)
				i_pJsonNode->AddMember(VIEW_NODE_NAME_INSATTRIBUTES, insAttrs, i_JsonDocP->GetAllocator());
			ret = STK_SUCCESS;
			return ret;
		}

		STK_STATUS Stk_ViewImpl::AddInsDisplayState(wstring i_wstrInsPlcPath, StkDisplayStateEnum i_eDisplayState)
		{
			Stk_InsAttributePtr	stkInsAttributeP = NULL;

			if (i_wstrInsPlcPath == L"")
			{
				return STK_ERROR;
			}

			map<wstring, Stk_InsAttributePtr>::iterator itor = m_mapInsAttribute.find(i_wstrInsPlcPath);

			if (itor != m_mapInsAttribute.end())
			{
				stkInsAttributeP = itor->second;
				stkInsAttributeP->SetDisplayState(i_eDisplayState);
			}
			else
			{
				stkInsAttributeP = Stk_InsAttributePtr::CreateObject();
				stkInsAttributeP->SetDisplayState(i_eDisplayState);
				m_mapInsAttribute.insert(pair<wstring, Stk_InsAttributePtr>(i_wstrInsPlcPath, stkInsAttributeP));
			}

			return STK_SUCCESS;
		}

		STK_STATUS Stk_ViewImpl::GetInsDisplayStateByPlcPath(wstring i_wstrInsPlcPath, StkDisplayStateEnum& o_eDisplayState)
		{
			Stk_InsAttributePtr	stkInsAttributeP = NULL;
			map<wstring, Stk_InsAttributePtr>::iterator itor = m_mapInsAttribute.find(i_wstrInsPlcPath);
			if (itor != m_mapInsAttribute.end())
			{
				stkInsAttributeP = itor->second;
				o_eDisplayState = stkInsAttributeP->GetDisplayState();
				return STK_SUCCESS;
			}
			else
			{
				return STK_ERROR;
			}
		}

		STK_STATUS Stk_ViewImpl::AddInsColor(wstring i_wstrInsPlcPath, STK_RGBA32 i_rgbaIns)
		{
			Stk_InsAttributePtr	stkInsAttributeP = NULL;

			if (i_wstrInsPlcPath == L"")
			{
				return STK_ERROR;
			}

			map<wstring, Stk_InsAttributePtr>::iterator itor = m_mapInsAttribute.find(i_wstrInsPlcPath);
			if (itor != m_mapInsAttribute.end())
			{
				stkInsAttributeP = itor->second;
				stkInsAttributeP->SetColor(i_rgbaIns);
			}
			else
			{
				stkInsAttributeP = Stk_InsAttributePtr::CreateObject();
				stkInsAttributeP->SetColor(i_rgbaIns);
				m_mapInsAttribute.insert(pair<wstring, Stk_InsAttributePtr>(i_wstrInsPlcPath, stkInsAttributeP));
			}

			return STK_SUCCESS;
		}

		STK_STATUS Stk_ViewImpl::GetInsColorByPlcPath(wstring i_wstrInsPlcPath, STK_RGBA32& o_rgbaIns)
		{
			Stk_InsAttributePtr	stkInsAttributeP = NULL;
			map<wstring, Stk_InsAttributePtr>::iterator itor = m_mapInsAttribute.find(i_wstrInsPlcPath);
			if (itor != m_mapInsAttribute.end())
			{
				stkInsAttributeP = itor->second;
				o_rgbaIns = stkInsAttributeP->GetColor();
				return STK_SUCCESS;
			}
			else
			{
				return STK_ERROR;
			}
		}

		STK_STATUS Stk_ViewImpl::AddInsMaterial(wstring i_wstrInsPlcPath, STK_MAT32 i_matIns)
		{
			Stk_InsAttributePtr	stkInsAttributeP = NULL;

			if (i_wstrInsPlcPath == L"")
			{
				return STK_ERROR;
			}

			map<wstring, Stk_InsAttributePtr>::iterator itor = m_mapInsAttribute.find(i_wstrInsPlcPath);

			if (itor != m_mapInsAttribute.end())
			{
				stkInsAttributeP = itor->second;
				stkInsAttributeP->SetMaterial(i_matIns);
			}
			else
			{
				stkInsAttributeP = Stk_InsAttributePtr::CreateObject();
				stkInsAttributeP->SetMaterial(i_matIns);
				m_mapInsAttribute.insert(pair<wstring, Stk_InsAttributePtr>(i_wstrInsPlcPath, stkInsAttributeP));
			}

			return STK_SUCCESS;

			// 将STK_MAT32更改为Stk_Material后，需要将Stk_Material增加到Stk_File中。
			// GetProtoType()->GetFile()->AddMaterial(Stk_Material* i_stkMaterialP);
		}

		STK_STATUS Stk_ViewImpl::GetInsMaterialByPlcPath(wstring i_wstrInsPlcPath, STK_MAT32& o_matIns)
		{
			Stk_InsAttributePtr	stkInsAttributeP = NULL;
			map<wstring, Stk_InsAttributePtr>::iterator itor = m_mapInsAttribute.find(i_wstrInsPlcPath);
			if (itor != m_mapInsAttribute.end())
			{
				stkInsAttributeP = itor->second;
				o_matIns = stkInsAttributeP->GetMaterial();
				return STK_SUCCESS;
			}
			else
			{
				return STK_ERROR;
			}
		}

		STK_STATUS Stk_ViewImpl::AddInsPlacement(wstring i_wstrInsPlcPath, STK_ID i_idPlacement, STK_MTX32 i_mtxInsPlc)
		{
			Stk_InsAttributePtr	stkInsAttributeP = NULL;

			if (i_wstrInsPlcPath == L"")
			{
				return STK_ERROR;
			}

			map<wstring, Stk_InsAttributePtr>::iterator itor = m_mapInsAttribute.find(i_wstrInsPlcPath);
			if (itor != m_mapInsAttribute.end())
			{
				stkInsAttributeP = itor->second;
				stkInsAttributeP->SetPlacement(i_idPlacement, i_mtxInsPlc);
			}
			else
			{
				stkInsAttributeP = Stk_InsAttributePtr::CreateObject();
				stkInsAttributeP->SetPlacement(i_idPlacement, i_mtxInsPlc);
				m_mapInsAttribute.insert(pair<wstring, Stk_InsAttributePtr>(i_wstrInsPlcPath, stkInsAttributeP));
			}

			return STK_SUCCESS;
		}

		STK_STATUS Stk_ViewImpl::GetInsPlacementByPlcPath(wstring i_wstrInsPlcPath, STK_ID& o_idPlacement, STK_MTX32& o_mtxInsPlc)
		{
			Stk_InsAttributePtr	stkInsAttributeP = NULL;
			map<wstring, Stk_InsAttributePtr>::iterator itor = m_mapInsAttribute.find(i_wstrInsPlcPath);
			if (itor != m_mapInsAttribute.end())
			{
				stkInsAttributeP = itor->second;
				stkInsAttributeP->GetPlacement(o_idPlacement, o_mtxInsPlc);
				return STK_SUCCESS;
			}
			else
			{
				return STK_ERROR;
			}
		}
		
		void Stk_ViewImpl::GetAllInsAttrPlcPath(vector<wstring>& vcInsPlcPaths)
		{
			vcInsPlcPaths.clear();
			map<wstring, Stk_InsAttributePtr>::iterator itor = m_mapInsAttribute.begin();
			while (itor != m_mapInsAttribute.end())
			{
				vcInsPlcPaths.push_back(itor->first);
				itor++;
			}
		}

		STK_STATUS Stk_ViewImpl::DeleteInsAttrByPlcPath(wstring i_wstrInsPlcPath)
		{
			map<wstring, Stk_InsAttributePtr>::iterator itor = m_mapInsAttribute.find(i_wstrInsPlcPath);
			if (itor != m_mapInsAttribute.end())
			{
				m_mapInsAttribute.erase(itor);
				return STK_SUCCESS;
			}
			return STK_ERROR;
		}

		STK_STATUS Stk_ViewImpl::DeleteAllInsAttribute()
		{
			map<wstring, Stk_InsAttributePtr>::iterator itor = m_mapInsAttribute.begin();

			while (itor != m_mapInsAttribute.end())
			{
				if (itor->second != NULL)
				{
					m_mapInsAttribute.erase(itor);
				}
				itor++;
			}

			return STK_SUCCESS;
		}


		STK_IMPLEMENT_CLASS(Stk_View, Stk_Object)

		Stk_View::Stk_View():Stk_Object(new Stk_ViewImpl(NULL,NULL))
		{
			
		}

		Stk_View::~Stk_View(void)
		{
			DELETE_STK_CLASS_IMPL
		}

		void Stk_View::Init(Stk_ViewDocument* i_ViewDocP, int i_iModelID)
		{
			((Stk_ViewImpl*)m_pImpl)->m_pViewDoc = i_ViewDocP;
			((Stk_ViewImpl*)m_pImpl)->m_iModelID = i_iModelID;
		}

		/*void Stk_View::SetIdenty(STK_ID i_Identy)
		{
			((Stk_ViewImpl*)m_pImpl)->m_iIdenty = i_Identy;
		}

		STK_ID Stk_View::GetIdenty()
		{
			return ((Stk_ViewImpl*)m_pImpl)->m_iIdenty;
		}*/

		void Stk_View::SetViewID(STK_ID i_ID)
		{
			((Stk_ViewImpl*)m_pImpl)->m_iViewID = i_ID;
		}

		STK_ID Stk_View::GetViewID()
		{
			return ((Stk_ViewImpl*)m_pImpl)->m_iViewID;
		}

		void Stk_View::SetTransparency(STK_FLOAT32 i_fTrans)
		{
			((Stk_ViewImpl*)m_pImpl)->m_fTransparency = i_fTrans;
		}

		STK_FLOAT32 Stk_View::GetTransparency()
		{
			return ((Stk_ViewImpl*)m_pImpl)->m_fTransparency;
		}

		void Stk_View::SetUsageType(StkViewUsageEnum i_eUsageType)
		{
			((Stk_ViewImpl*)m_pImpl)->m_eUsageType = i_eUsageType;
		}

		StkViewUsageEnum Stk_View::GetUsageType()
		{
			return ((Stk_ViewImpl*)m_pImpl)->m_eUsageType;
		}

		void Stk_View::SetName(const wstring &inName)
		{
			((Stk_ViewImpl*)m_pImpl)->m_wstrName = inName;
		}

		wstring Stk_View::GetName(void)
		{
			return ((Stk_ViewImpl*)m_pImpl)->m_wstrName;
		}

		STK_STATUS Stk_View::ChangeName(const wstring &i_strName)
		{
			((Stk_ViewImpl*)m_pImpl)->m_wstrName = i_strName;
			return STK_SUCCESS;
		}

		void Stk_View::SetCamera(Stk_CameraPtr i_CameraP)
		{
			((Stk_ViewImpl*)m_pImpl)->m_CameraP = i_CameraP;
		}

		Stk_CameraPtr Stk_View::GetCamera(void)
		{
			return ((Stk_ViewImpl*)m_pImpl)->m_CameraP;
		}

		const vector<Stk_PlaneEntityPtr>&  Stk_View::GetAllPlaneEntity()
		{
			return ((Stk_ViewImpl*)m_pImpl)->m_vcPlaneEntity;
		}

		void Stk_View::AddPlaneEntity(Stk_PlaneEntityPtr i_PlaneEntityP)
		{
			((Stk_ViewImpl*)m_pImpl)->m_vcPlaneEntity.push_back(i_PlaneEntityP);
		}

		void Stk_View::SetActivated(STK_BOOL i_bActivated)
		{
			((Stk_ViewImpl*)m_pImpl)->m_bIsActivated = i_bActivated;
		}

		STK_BOOL Stk_View::IsActivated()
		{
			return ((Stk_ViewImpl*)m_pImpl)->m_bIsActivated;
		}

		STK_STATUS Stk_View::AddInsDisplayState(wstring i_wstrInsPlcPath, StkDisplayStateEnum i_eDisplayState)
		{
			return ((Stk_ViewImpl*)m_pImpl)->AddInsDisplayState(i_wstrInsPlcPath, i_eDisplayState);
		}

		STK_STATUS Stk_View::GetInsDisplayStateByPlcPath(wstring i_wstrInsPlcPath, StkDisplayStateEnum& o_eDisplayState)
		{
			return ((Stk_ViewImpl*)m_pImpl)->GetInsDisplayStateByPlcPath(i_wstrInsPlcPath, o_eDisplayState);
		}

		STK_STATUS Stk_View::AddInsColor(wstring i_wstrInsPlcPath, STK_RGBA32 i_rgbaIns)
		{
			return ((Stk_ViewImpl*)m_pImpl)->AddInsColor(i_wstrInsPlcPath, i_rgbaIns);
		}

		STK_STATUS Stk_View::GetInsColorByPlcPath(wstring i_wstrInsPlcPath, STK_RGBA32& o_rgbaIns)
		{
			return ((Stk_ViewImpl*)m_pImpl)->GetInsColorByPlcPath(i_wstrInsPlcPath, o_rgbaIns);
		}

		STK_STATUS Stk_View::AddInsMaterial(wstring i_wstrInsPlcPath, STK_MAT32 i_matIns)
		{
			return ((Stk_ViewImpl*)m_pImpl)->AddInsMaterial(i_wstrInsPlcPath, i_matIns);
		}

		STK_STATUS Stk_View::GetInsMaterialByPlcPath(wstring i_wstrInsPlcPath, STK_MAT32& o_matIns)
		{
			return ((Stk_ViewImpl*)m_pImpl)->GetInsMaterialByPlcPath(i_wstrInsPlcPath, o_matIns);
		}
		
		STK_STATUS Stk_View::AddInsPlacement(wstring i_wstrInsPlcPath, STK_ID i_idPlacement, STK_MTX32 i_mtxInsPlc)
		{
			return ((Stk_ViewImpl*)m_pImpl)->AddInsPlacement(i_wstrInsPlcPath, i_idPlacement, i_mtxInsPlc);
		}

		STK_STATUS Stk_View::GetInsPlacementByPlcPath(wstring i_wstrInsPlcPath, STK_ID& o_idPlacement, STK_MTX32& o_mtxInsPlc)
		{
			return ((Stk_ViewImpl*)m_pImpl)->GetInsPlacementByPlcPath(i_wstrInsPlcPath, o_idPlacement, o_mtxInsPlc);
		}

		void Stk_View::GetAllInsAttrPlcPath(vector<wstring>& vcInsPlcPaths)
		{
			((Stk_ViewImpl*)m_pImpl)->GetAllInsAttrPlcPath(vcInsPlcPaths);
		}

		STK_STATUS Stk_View::DeleteInsAttrByPlcPath(wstring i_wstrInsPlcPath)
		{
			return ((Stk_ViewImpl*)m_pImpl)->DeleteInsAttrByPlcPath(i_wstrInsPlcPath);
		}

		STK_STATUS Stk_View::DeleteAllInsAttribute()
		{
			return ((Stk_ViewImpl*)m_pImpl)->DeleteAllInsAttribute();
		}

		STK_STATUS Stk_View::ToJson(void* i_DataP, void* i_JsonDocP)
		{
			return ((Stk_ViewImpl*)m_pImpl)->ToJson((Value*)i_DataP, (Document*)i_JsonDocP);
		}

		STK_STATUS Stk_View::FromJson(void* i_DataP, void* i_JsonDocP)
		{
			return ((Stk_ViewImpl*)m_pImpl)->FromJson((Value*)i_DataP, (Document*)i_JsonDocP);
		}

		STK_STATUS	Stk_View::AddConnector(StkEntityTypeEnum eEntityType, STK_ID entityID)
		{
			map<StkEntityTypeEnum, vector<STK_ID>>::iterator itorMap = ((Stk_ViewImpl*)m_pImpl)->m_mapConnector.find(eEntityType);
			if (itorMap != ((Stk_ViewImpl*)m_pImpl)->m_mapConnector.end())
			{
				vector<STK_ID> vcEntityID = itorMap->second;
				bool bFlag = false;
				vector<STK_ID>::iterator itorEntityID = vcEntityID.begin();
				while (itorEntityID != vcEntityID.end())
				{
					if (*itorEntityID == entityID)
					{
						bFlag = true;
						break;
					}
					itorEntityID++;
				}
				if (!bFlag)
					itorMap->second.push_back(entityID);
			}else
			{
				vector<STK_ID> vcEntityID;
				vcEntityID.push_back(entityID);
				((Stk_ViewImpl*)m_pImpl)->m_mapConnector.insert(make_pair(eEntityType, vcEntityID));
			}
			return STK_SUCCESS;
		}

		const map<StkEntityTypeEnum, vector<STK_ID>>& Stk_View::GetConnector()
		{
			return ((Stk_ViewImpl*)m_pImpl)->m_mapConnector;
		}
	}//SVLLib
}//HoteamSoft