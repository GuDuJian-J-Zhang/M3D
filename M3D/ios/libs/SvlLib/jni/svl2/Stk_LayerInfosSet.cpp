// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_LayerInfosSet.cpp
 *
 *	@brief	图层信息集、图层过滤器类 
 *
 *	@par	历史:
 *		2014/05/16	创建。WangY@HOTEAMSOFT
****************************************************************************/

#include "StdAfx.h"
#include "Stk_LayerInfosSet.h"
#include "Stk_Manager.h"
#include "Stk_Enum.h"
#include "Stk_Platform.h"

/************************************************************************//**
 *	@brief		Stk_LayerInfosSet构造函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_LayerInfosSet::Stk_LayerInfosSet()
{
	m_stkProtoP = NULL;
	m_nDefaultLayer = -1;
	m_nDefLayerFilter = -1;
	m_NodeP = NULL;
}

/************************************************************************//**
 *	@brief		Stk_LayerInfosSet构造函数
 *	@param[in]	i_stkProtoTypeP	Prototype对象指针
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_LayerInfosSet::Stk_LayerInfosSet(Stk_ProtoType* i_stkProtoTypeP)
{
	m_stkProtoP = i_stkProtoTypeP;
	m_nDefaultLayer = -1;
	m_nDefLayerFilter = -1;
	m_NodeP = NULL;
}

/****************************************************************************
 *	@brief		Stk_LayerInfosSet拷贝构造函数
 *	@param[in]	i_stkLayerInfosSet	图层信息集
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

//Stk_LayerInfosSet::Stk_LayerInfosSet(const Stk_LayerInfosSet& i_stkLayerInfosSet)
//{
//	vector<Stk_LayerFilter*> vcLayerFilters = i_stkLayerInfosSet.m_vcLayerFilters;
//
//	m_nuDefaultLayer = i_stkLayerInfosSet.m_nuDefaultLayer;
//	m_mapLayerAndName = i_stkLayerInfosSet.m_mapLayerAndName;
//	//默认图层过滤器进行浅拷贝，原因在于内存的分配是在图层过滤器数组中进行
//	m_nuDefLayerFilter = i_stkLayerInfosSet.m_nuDefLayerFilter;
//
//	m_stkProtoP = new Stk_ProtoType();
//	if (m_stkProtoP != NULL)
//	{
//		memcpy(m_stkProtoP, i_stkLayerInfosSet.m_stkProtoP, sizeof(*i_stkLayerInfosSet.m_stkProtoP));
//	}
//	for (int ii = 0; ii < vcLayerFilters.size(); ii++)
//	{
//		Stk_LayerFilter* stkLayerFilter = NULL;
//		stkLayerFilter = new Stk_LayerFilter();
//		if (stkLayerFilter != NULL)
//		{
//			memcpy(stkLayerFilter, vcLayerFilters[ii], sizeof(*vcLayerFilters[ii]));
//			m_vcLayerFilters.push_back(stkLayerFilter);
//		}
//	}
//}

/************************************************************************//**
 *	@brief		Stk_LayerInfosSet析构函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		如果已关联到Node，会自动析构Node并解除Node父子关联
 *	@attention	无
 ****************************************************************************/

Stk_LayerInfosSet::~Stk_LayerInfosSet()
{
	for (int ii = 0; ii < m_vcLayerFilters.size(); ii++)
	{
		if (m_vcLayerFilters[ii] != NULL)
		{
			delete m_vcLayerFilters[ii];
			m_vcLayerFilters[ii] = NULL;
		}
	}
	// 删除所从属的Node节点
	if (m_NodeP!=NULL)
	{
		m_NodeP->SetLayerInfosSet(NULL); //防止析构Node时再次析构LayerInfosSet造成死循环
		delete m_NodeP;
		m_NodeP = NULL;
	}
}

/****************************************************************************
 *	@brief		Stk_LayerInfosSet赋值构造函数
 *	@param[in]	i_stkLayerInfosSet	图层信息集
 *	@param[out]	无
 *	@retval		*this				图层信息集
 *	@note		无
 *	@attention	无
 ****************************************************************************/

//Stk_LayerInfosSet& Stk_LayerInfosSet::operator =(const Stk_LayerInfosSet& i_stkLayerInfosSet)
//{
//	if (this == &i_stkLayerInfosSet)
//	{
//		return *this;
//	}
//
//	vector<Stk_LayerFilter*> vcLayerFilters = i_stkLayerInfosSet.m_vcLayerFilters;
//
//	m_nuDefaultLayer = i_stkLayerInfosSet.m_nuDefaultLayer;
//	m_mapLayerAndName = i_stkLayerInfosSet.m_mapLayerAndName;
//	//默认图层过滤器进行浅拷贝，原因在于内存的分配是在图层过滤器数组中进行
//	m_nuDefLayerFilter = i_stkLayerInfosSet.m_nuDefLayerFilter;
//
//	m_stkProtoP = new Stk_ProtoType();
//	if (m_stkProtoP != NULL)
//	{
//		memcpy(m_stkProtoP, i_stkLayerInfosSet.m_stkProtoP, sizeof(*i_stkLayerInfosSet.m_stkProtoP));
//	}
//	for (int ii = 0; ii < vcLayerFilters.size(); ii++)
//	{
//		Stk_LayerFilter* stkLayerFilter = NULL;
//		stkLayerFilter = new Stk_LayerFilter();
//		if (stkLayerFilter != NULL)
//		{
//			memcpy(stkLayerFilter, vcLayerFilters[ii], sizeof(*vcLayerFilters[ii]));
//			m_vcLayerFilters.push_back(stkLayerFilter);
//		}
//	}
//
//	return *this;
//}

/************************************************************************//**
 *	@brief		设置默认图层
 *	@param[in]	i_nLayer		图层
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_LayerInfosSet::SetDefaultLayer(STK_INT32 i_nLayer)
{
	m_nDefaultLayer = i_nLayer;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取默认图层
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_nuDefaultLayer	默认图层
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_INT32 Stk_LayerInfosSet::GetDefaultLayer()
{
	return m_nDefaultLayer;
}

/************************************************************************//**
 *	@brief		添加图层编号和图层名称的映射
 *	@param[in]	i_nuLayer			图层编号
 *	@param[in]	i_wstrName			图层名称
 *	@param[out]	无
 *	@retval		STK_SUCCESS			添加成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/

//STK_STATUS Stk_LayerInfosSet::AddLayerAndName(STK_UINT32 i_nuLayer, wstring i_wstrName)
//{
//	m_mapLayerAndName.insert(pair<STK_UINT32, wstring>(i_nuLayer, i_wstrName));
//	return STK_SUCCESS;
//}

/************************************************************************//**
 *	@brief		获取所有图层编号和图层名称的映射
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_mapIndexAndName		所有图层编号和图层名称的映射
 *	@note		无
 *	@attention	无
 ****************************************************************************/

//map<STK_UINT32, wstring>& Stk_LayerInfosSet::GetAllLayerAndName()
//{
//	return m_mapLayerAndName;
//}

/************************************************************************//**
 *	@brief		添加图层
 *	@param[in]	i_stkLayerP			图层对象指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS			添加成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_LayerInfosSet::AddLayer(Stk_Layer* i_stkLayerP)
{
	if (i_stkLayerP != NULL)
	{
		m_vcLayers.push_back(i_stkLayerP);
	}
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取图层数量
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		图层数量
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_UINT32 Stk_LayerInfosSet::GetLayerNum()
{
	return m_vcLayers.size();
}

/************************************************************************//**
 *	@brief		获取指定的图层
 *	@param[in]	i_nuIndex		指定图层索引
 *	@param[out]	无
 *	@retval		指定的图层
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_Layer* Stk_LayerInfosSet::GetLayerByIndex(STK_UINT32 i_nuIndex)
{
	return m_vcLayers[i_nuIndex];
}

/************************************************************************//**
 *	@brief		设置默认图层过滤器
 *	@param[in]	i_nLayerFilter			默认图层过滤器编号
 *	@param[out]	无
 *	@retval		STK_SUCCESS				设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_LayerInfosSet::SetDefaultFilter(STK_INT32 i_nLayerFilter)
{
	m_nDefLayerFilter = i_nLayerFilter;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取默认图层过滤器
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		默认图层过滤器
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_LayerFilter* Stk_LayerInfosSet::GetDefaultFilter()
{
	if (m_nDefLayerFilter < 0)
	{
		return NULL;
	}

	return m_vcLayerFilters.at(m_nDefLayerFilter);
}

/************************************************************************//**
 *	@brief		添加图层过滤器
 *	@param[in]	i_stkLayerFilter		图层过滤器
 *	@param[out]	无
 *	@retval		STK_SUCCESS				添加成功
 *	@retval		STK_ERROR				添加失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_LayerInfosSet::AddLayerFilter(Stk_LayerFilter* i_stkLayerFilter)
{
	if (i_stkLayerFilter != NULL)
	{
		m_vcLayerFilters.push_back(i_stkLayerFilter);
		return STK_SUCCESS;
	}
	else
	{
		return STK_ERROR;
	}
}

/************************************************************************//**
 *	@brief		获取指定索引对应的图层过滤器
 *	@param[in]	i_nuIndex				索引号
 *	@param[out]	无
 *	@retval		图层过滤器
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_LayerFilter* Stk_LayerInfosSet::GetLayerFilterByIndex(STK_UINT32 i_nuIndex)
{
	for (int ii = 0; ii < m_vcLayerFilters.size(); ii++)
	{
		if (ii == i_nuIndex)
		{
			return m_vcLayerFilters[ii];
		}
	}

	return NULL;
}

/************************************************************************//**
 *	@brief		获取所有的图层过滤器
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_vcLayerFilters		所有图层过滤器
 *	@note		无
 *	@attention	无
 ****************************************************************************/

vector<Stk_LayerFilter*> Stk_LayerInfosSet::GetAllLayerFilters()
{
	return m_vcLayerFilters;
}

/*************************************************************************//**
 *	@brief		设置所对应的Node
 *	@param[in]	i_NodeP				所对应的Node
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_LayerInfosSet::SetNode(Stk_Node* i_NodeP)
{
	m_NodeP = i_NodeP;
	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		获取所对应的Node
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		所对应的Node
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
Stk_Node* Stk_LayerInfosSet::GetNode()
{
	return m_NodeP;
}

/************************************************************************//**
 *	@brief		读取文件中的图层信息集数据
 *	@param[in]	i_FileP				文件对象指针
 *	@param[in]	i_LayerSetHeadP		图层信息集记录头指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS			读取成功
 *	@retval		STK_ERROR_MEMORY	内存申请失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_LayerInfosSet::ReadFile(Stk_File *i_FileP, StkSegmentHead *i_LayerSetHeadP)
{
	StkSegmentHead			head={0};
	StkControl**			RecordPP = NULL;
	StkLayerInfosSetRec*	LayerInfosSetRecP = NULL;
	StkLayerFilterRec*		LayerFilterRecP = NULL;
	Stk_LayerFilter*		stkLayerFilterP = NULL;
	Stk_Layer*				stkLayerP = NULL;
	STK_STATUS				eState = STK_SUCCESS;
	STK_UINT32				nuLayerNameSize = 0;
	STK_UINT32				nuFilterNameSize = 0;
	STK_UINT32				nuDescription = 0;
	STK_INT32				nSelectLayer = -1;
	STK_BOOL				bSelectState = false;
	wstring					wstrLayerName = L"";
	CHAR*					chItorP = NULL;

	/*
	 *	读取图层信息集数据
	 */
	RecordPP = (StkControl **)calloc(i_LayerSetHeadP->recnum, sizeof(StkControl*));
	if(i_LayerSetHeadP->recnum !=0 && RecordPP == NULL)
	{
		return STK_ERROR_MEMORY;
	}
	eState = i_FileP->ReadSegmentAllRecordData(i_LayerSetHeadP, (StkControl** )RecordPP);
	if (eState != STK_SUCCESS)
	{
		return eState;
	}
	/*
	 *	读取图层信息集各记录
	 */
	for (int ii = 0; ii < i_LayerSetHeadP->recnum; ii++)
	{
		if (RecordPP[ii]->type == REC_TYPE_LAYERINFOSSET)
		{
			//版本 1
			if (RecordPP[ii]->version == 1)
			{
				LayerInfosSetRecP = (StkLayerInfosSetRec*)RecordPP[ii];
				m_nDefaultLayer = LayerInfosSetRecP->nDefaultLayer;
			}
		}
		else if (RecordPP[ii]->type == REC_TYPE_LAYERFILTER)
		{
			//版本 1
			if (RecordPP[ii]->version == 1)
			{
				LayerFilterRecP = (StkLayerFilterRec*)RecordPP[ii];
				stkLayerFilterP = new Stk_LayerFilter();
				nuFilterNameSize = LayerFilterRecP->nuNameNum;
				nuDescription = LayerFilterRecP->nuDescriptionNum;
				STK_WCHAR*	wchFilterName = new STK_WCHAR[nuFilterNameSize];
				if (nuFilterNameSize > 0 && wchFilterName == NULL)
				{
					return STK_ERROR_MEMORY;
				}
				STK_WCHAR*	wchDescription = new STK_WCHAR[nuDescription];
				if (nuDescription > 0 && wchDescription == NULL)
				{
					return STK_ERROR_MEMORY;
				}
				chItorP = LayerFilterRecP->atr;
				memcpy(wchFilterName, chItorP, sizeof(STK_WCHAR)*nuFilterNameSize);
				chItorP += sizeof(STK_WCHAR)*nuFilterNameSize;
#ifdef __MOBILE__
				stkLayerFilterP->SetLayerFilterName(Stk_Platform::STKWCHARStrToWString(wchFilterName));
#else
				stkLayerFilterP->SetLayerFilterName(wchFilterName);
#endif
				memcpy(wchDescription, chItorP, sizeof(STK_WCHAR)*nuDescription);
				chItorP += sizeof(STK_WCHAR)*nuDescription;
#ifdef __MOBILE__
				stkLayerFilterP->SetDescription(Stk_Platform::STKWCHARStrToWString(wchDescription));
#else
				stkLayerFilterP->SetDescription(wchDescription);
#endif
				for (int jj = 0; jj < LayerFilterRecP->nuSelectedLayerNum; jj++)
				{
					memcpy(&nSelectLayer, chItorP, sizeof(STK_INT32));
					chItorP += sizeof(STK_INT32);
					memcpy(&bSelectState, chItorP, sizeof(STK_BOOL));
					chItorP += sizeof(STK_UINT32);
					stkLayerFilterP->AddMapLayerState(nSelectLayer, bSelectState);
				}
				
				if (stkLayerFilterP != NULL)
				{
					m_vcLayerFilters.push_back(stkLayerFilterP);
				}

				// 删除临时分配的内存
				if (wchFilterName != NULL)
				{
					delete [] wchFilterName;
					wchFilterName = NULL;
				}
				if (wchDescription != NULL)
				{
					delete [] wchDescription;
					wchDescription = NULL;
				}
			}
		}
		else
		{

		}
	}

	// 释放内存
	eState = i_FileP->ReadSegmentFinishData(i_LayerSetHeadP, (StkControl**)RecordPP);
	if (eState != STK_SUCCESS)
	{
		return eState;
	}
	if (RecordPP != NULL)
	{
		free(RecordPP);
		RecordPP = NULL;
	}

	/*
	 *	读取下一段的数据
	 */
	while (1)
	{
		eState = i_FileP->ReadSegmentHeadData(&head);
		if (eState!=STK_SUCCESS)
		{
			return STK_ERROR_HEADRD;
		}
		switch(head.kind)
		{
			//当下一段是NodeConnector段时
		case SEG_TYPE_LAYER:
			stkLayerP = new Stk_Layer(m_stkProtoP);
			eState = stkLayerP->ReadFile(i_FileP, &head);
			if (eState != STK_SUCCESS)
			{
				delete stkLayerP;
				return eState;
			}

			AddLayer(stkLayerP);
			break;
			//当下一段是图层信息集-END段时
		case SEG_TYPE_LAYERSET_END:
			goto rtn;
			break;
		default:
			StkControl **TmpRecordPP = NULL;
			TmpRecordPP = (StkControl **)calloc(head.recnum, sizeof(StkControl*));
			if (head.recnum !=0 && TmpRecordPP == NULL)
			{
				return STK_ERROR_MEMORY;
			}
			eState = i_FileP->ReadSegmentAllRecordData(&head, (StkControl **)TmpRecordPP);
			if (eState!=STK_SUCCESS)
			{
				return STK_ERROR_HEADRD;
			}
			eState = i_FileP->ReadSegmentFinishData(&head, (StkControl **)TmpRecordPP);// 释放内存
			if (eState!=STK_SUCCESS)
			{
				return STK_ERROR_HEADRD;
			}
			if (TmpRecordPP != NULL)
			{
				free(TmpRecordPP);
				TmpRecordPP = NULL;
			}
			break;
		}
	}

rtn:
	return eState;
}

/*************************************************************************//**
 *	@brief		写图层信息集数据
 *	@param[in]	i_stkFileP			文件对象指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS			写入成功
 *	@retval		其它				写入失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_LayerInfosSet::WriteFile(Stk_File *i_stkFileP)
{
	StkSegmentHead		LayerSetHead = {0};
	StkSegmentHead		endHead = {0};
	StkLayerInfosSetRec	LayerInfosSetRec;
	STK_STATUS			ret = STK_SUCCESS;

	if (i_stkFileP == NULL)
	{
		return STK_ERROR;
	}

	/*
	 *	【第一步】输出图层信息集段
	 */
	// 【1.1】 输出图层信息集记录头
	LayerSetHead.kind = SEG_TYPE_LAYERSET;
	ret = i_stkFileP->WriteSegmentHeadData(&LayerSetHead);
	if(ret < STK_SUCCESS)
	{
		return STK_ERROR;
	}
	
	// 【1.2】 输出图层信息集记录
	LayerInfosSetRec.Control.type = REC_TYPE_LAYERINFOSSET;
	LayerInfosSetRec.Control.length = sizeof(StkLayerInfosSetRec) - sizeof(StkControl);
	LayerInfosSetRec.Control.version = _VER_LAYERINFOSSET_RECORD;

	LayerInfosSetRec.nDefaultLayer = m_nDefaultLayer;
	LayerInfosSetRec.nDefLayerFilter = m_nDefLayerFilter;
	LayerInfosSetRec.nuLayerNum = m_vcLayers.size();
	LayerInfosSetRec.nuLayerFilterNum = m_vcLayerFilters.size();
	
	// 【1.3】 完成图层信息集记录的输出
	ret = i_stkFileP->WriteSegmentRecordData(&LayerSetHead, (StkControl *)&LayerInfosSetRec);
	if(ret < STK_SUCCESS)
	{
		return STK_ERROR;
	}

	// 【1.4】 输出图层过滤器记录数组
	for (int ii = 0; ii < m_vcLayerFilters.size(); ii++)
	{
		if(m_vcLayerFilters[ii] != NULL)
		{
			m_vcLayerFilters[ii]->WriteLayerFilter(&LayerSetHead, i_stkFileP);
		}
	}

	// 【1.5】 完成图层信息集段的输出
	ret = i_stkFileP->WriteSegmentFinishData(&LayerSetHead);
	if(ret < STK_SUCCESS)
	{
		return STK_ERROR;
	}

	/*
	 *	【第二步】输出图层段
	 */
	for (int ii = 0; ii < m_vcLayers.size(); ii++)
	{
		if (m_vcLayers[ii] != NULL)
		{
			m_vcLayers[ii]->WriteFile(i_stkFileP);
		}
	}

	/*
	 *	【第三步】 输出图层信息集-END段
	 */
	endHead.id = m_nDefaultLayer;
	endHead.kind = SEG_TYPE_LAYERSET_END;
	ret = i_stkFileP->WriteSegmentHeadData(&endHead);
	if(ret < STK_SUCCESS)
	{
		return STK_ERROR;
	}

	return ret;
}

/*************************************************************************//**
 *	@brief		图层过滤器构造函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 *****************************************************************************/

Stk_LayerFilter::Stk_LayerFilter()
{
	m_wstrName = L"";
	m_wstrDescription = L"";
}

/*************************************************************************//**
 *	@brief		图层过滤器拷贝构造函数
 *	@param[in]	i_stkLayerFilter		图层过滤器
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 *****************************************************************************/

Stk_LayerFilter::Stk_LayerFilter(const Stk_LayerFilter& i_stkLayerFilter)
{
	m_wstrName = i_stkLayerFilter.m_wstrName;
	m_wstrDescription = i_stkLayerFilter.m_wstrDescription;
	//m_vcSelected = i_stkLayerFilter.m_vcSelected;
	//m_vcSelectedLayers = i_stkLayerFilter.m_vcSelectedLayers;
	m_mapSelectedLayers = i_stkLayerFilter.m_mapSelectedLayers;
}

/*************************************************************************//**
 *	@brief		图层过滤器析构函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 *****************************************************************************/

Stk_LayerFilter::~Stk_LayerFilter()
{

}

/*************************************************************************//**
 *	@brief		图层过滤器赋值构造函数
 *	@param[in]	i_stkLayerFilter		图层过滤器
 *	@param[out]	无
 *	@retval		图层过滤器对象
 *	@note		无
 *	@attention	无
 *****************************************************************************/

Stk_LayerFilter& Stk_LayerFilter::operator = (const Stk_LayerFilter& i_stkLayerFilter)
{
	if (this == &i_stkLayerFilter)
	{
		return *this;
	}
	m_wstrName = i_stkLayerFilter.m_wstrName;
	m_wstrDescription = i_stkLayerFilter.m_wstrDescription;
	//m_vcSelected = i_stkLayerFilter.m_vcSelected;
	//m_vcSelectedLayers = i_stkLayerFilter.m_vcSelectedLayers;
	m_mapSelectedLayers = i_stkLayerFilter.m_mapSelectedLayers;

	return *this;
}

/*************************************************************************//**
 *	@brief		设置图层过滤器名称
 *	@param[in]	i_wstrName		图层过滤器名称
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 *****************************************************************************/

STK_STATUS Stk_LayerFilter::SetLayerFilterName(wstring i_wstrName)
{
	m_wstrName = i_wstrName;
	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		获取图层过滤器名称
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_wstrName		图层过滤器名称
 *	@note		无
 *	@attention	无
 *****************************************************************************/

wstring Stk_LayerFilter::GetLayerFilterName()
{
	return m_wstrName;
}

/*************************************************************************//**
 *	@brief		设置图层过滤器描述
 *	@param[in]	i_wstrDescription	图层过滤器描述
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 *****************************************************************************/

STK_STATUS Stk_LayerFilter::SetDescription(wstring i_wstrDescription)
{
	m_wstrDescription = i_wstrDescription;
	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		获取图层过滤器描述
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_wstrDescription		图层过滤器描述
 *	@note		无
 *	@attention	无
 *****************************************************************************/

wstring Stk_LayerFilter::GetDescription()
{
	return m_wstrDescription;
}

//STK_STATUS Stk_LayerFilter::AddSelectedState(STK_BOOL i_bSelected)
//{
//	m_vcSelected.push_back(i_bSelected);
//	return STK_SUCCESS;
//}

/*****************************************************************************
 *	@brief		判断指定图层的状态
 *	@param[in]	i_nuLayer					指定的图层
 *	@param[out]	无
 *	@retval		指定的图层
 *	@note		无
 *	@attention	无
 *****************************************************************************/

//STK_BOOL Stk_LayerFilter::IsSelected(STK_UINT32 i_nuLayer)
//{
//	return m_vcSelected[i_nuLayer];
//}

/*****************************************************************************
 *	@brief		添加符合图层过滤器的图层
 *	@param[in]	i_nuLayer					符合图层过滤器的图层
 *	@param[out]	无
 *	@retval		STK_SUCCESS					添加成功
 *	@note		无
 *	@attention	无
 *****************************************************************************/

//STK_STATUS Stk_LayerFilter::AddSelectedLayer(STK_UINT32 i_nuLayer)
//{
//	m_vcSelectedLayers.push_back(i_nuLayer);
//	return STK_SUCCESS;
//}

/*****************************************************************************
 *	@brief		获取所有的符合图层过滤器的图层
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_vcSelectedLayers			符合图层过滤器的图层
 *	@note		无
 *	@attention	无
 *****************************************************************************/

//vector<STK_UINT32>& Stk_LayerFilter::GetAllSelectedLayers()
//{
//	return m_vcSelectedLayers;
//}

/*************************************************************************//**
 *	@brief		设置被选择图层与选择状态之间的映射
 *	@param[in]	i_nSelectedLayer			被选择的图层
 *	@param[in]	i_bSelectedState			被选择的图层的状态
 *	@param[out]	无
 *	@retval		STK_SUCCESS					设置成功
 *	@note		无
 *	@attention	无
 *****************************************************************************/

STK_STATUS Stk_LayerFilter::AddMapLayerState(STK_INT32 i_nSelectedLayer, STK_BOOL i_bSelectedState)
{
	m_mapSelectedLayers.insert(pair<STK_INT32, STK_BOOL>(i_nSelectedLayer, i_bSelectedState));
	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		获取被选择图层与选择状态之间的映射
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_mapSelectedLayers		映射关系
 *	@note		无
 *	@attention	无
 *****************************************************************************/

map<STK_INT32, STK_BOOL>& Stk_LayerFilter::GetAllMapLayerStates()
{
	return m_mapSelectedLayers;
}

/*************************************************************************//**
 *	@brief		写图层过滤器数据
 *	@param[in]	i_head				数据段头指针
 *	@param[in]	i_stkFileP			文件对象指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS			写入成功
 *	@retval		其它				写入失败
 *	@note		无
 *	@attention	内部接口
 *****************************************************************************/

STK_STATUS Stk_LayerFilter::WriteLayerFilter(StkSegmentHead *i_head, Stk_File *i_stkFileP)
{
	STK_STATUS			ret = STK_SUCCESS;
	StkLayerFilterRec	*LayerFilterRecP = NULL;	// 图层过滤器记录指针
	wstring				wstrLayerFilterName = L"";	// 图层过滤器名称
	wstring				wstrDescription = L"";		// 图层过滤器描述

	STK_UINT32			nuFilterNameNum = 0;		// 图层过滤器名称字符数
	STK_UINT32			nuDescriptionNum = 0;		// 图层过滤器描述内容字符数
	STK_UINT32			nuSelectedLayerNum = 0;		// 符合过滤器条件的图层数量
	CHAR*				chItorP = NULL;				// 游标

	map<STK_INT32, STK_BOOL>::iterator itor = m_mapSelectedLayers.begin();

	wstrLayerFilterName = m_wstrName;
	nuFilterNameNum = wstrLayerFilterName.size() + 1;
	wstrDescription = m_wstrDescription;
	nuDescriptionNum = wstrDescription.size() + 1;
	nuSelectedLayerNum = m_mapSelectedLayers.size();

	LayerFilterRecP = (StkLayerFilterRec *)calloc(1, sizeof(StkLayerFilterRec) - sizeof(CHAR)
		+ nuFilterNameNum*sizeof(STK_WCHAR) + nuDescriptionNum*sizeof(STK_WCHAR) + 2*nuSelectedLayerNum*sizeof(STK_UINT32)); //乘2的原因：包括图层和图层状态
	if (LayerFilterRecP == NULL)
	{
		return STK_ERROR_MEMORY;
	}
	LayerFilterRecP->Control.type = REC_TYPE_LAYERFILTER;
	LayerFilterRecP->Control.length =  sizeof(StkLayerFilterRec) - sizeof(CHAR)
		+ nuFilterNameNum*sizeof(STK_WCHAR) + nuDescriptionNum*sizeof(STK_WCHAR) + 2*nuSelectedLayerNum*sizeof(STK_UINT32) - sizeof(StkControl);
	LayerFilterRecP->Control.version = _VER_LAYERFILTER_RECORD;

	LayerFilterRecP->nuNameNum = nuFilterNameNum;
	LayerFilterRecP->nuDescriptionNum = nuDescriptionNum;
	LayerFilterRecP->nuSelectedLayerNum = nuSelectedLayerNum;

	chItorP = LayerFilterRecP->atr;
	// 过滤器名称
#ifdef __MOBILE__
	memcpy(chItorP, Stk_Platform::WStringToSTKWCHARStr(wstrLayerFilterName), (wstrLayerFilterName.size()+1)*sizeof(STK_WCHAR));
#else
	memcpy(chItorP, wstrLayerFilterName.c_str(), sizeof(STK_WCHAR)*nuFilterNameNum);
#endif
	chItorP += sizeof(STK_WCHAR)*nuFilterNameNum;

	// 过滤器描述
#ifdef __MOBILE__
	memcpy(chItorP, Stk_Platform::WStringToSTKWCHARStr(wstrDescription), nuDescriptionNum*sizeof(STK_WCHAR));
#else
	memcpy(chItorP, wstrDescription.c_str(), sizeof(STK_WCHAR)*nuDescriptionNum);
#endif
	chItorP += sizeof(STK_WCHAR)*nuDescriptionNum;

	// 符合过滤条件的图层及图层状态，格式为：图层+状态，图层+状态，...
	while(itor != m_mapSelectedLayers.end())
	{
		memcpy(chItorP, &itor->first, sizeof(STK_INT32));
		chItorP += sizeof(STK_INT32);
		memcpy(chItorP, &itor->second, sizeof(STK_UINT32));
		chItorP += sizeof(STK_UINT32);
		itor++;
	}

	ret = i_stkFileP->WriteSegmentRecordData(i_head, (StkControl *)LayerFilterRecP);
	if(ret < STK_SUCCESS)
	{
		return STK_ERROR;
	}

	return ret;
}

/*************************************************************************//**
*	@brief		图层构造函数
*	@param[in]	i_stkProtoP		所属组件
*	@param[out]	无
*	@retval		无
*	@note		无
*	@attention	无
*****************************************************************************/
Stk_Layer::Stk_Layer(Stk_ProtoType* i_stkProtoP)
{
	m_stkProtoP = i_stkProtoP;
	m_nLayer = -1;
	m_wstrName = L"";
	m_bIsDisplay = true;
}

/*************************************************************************//**
 *	@brief		图层析构函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 *****************************************************************************/
Stk_Layer::~Stk_Layer()
{
	for (int ii = 0; ii < m_vcNodeConnectors.size(); ii++)
	{
		if (m_vcNodeConnectors[ii] != NULL)
		{
			delete m_vcNodeConnectors[ii];
			m_vcNodeConnectors[ii] = NULL;
		}
	}
}

/*************************************************************************//**
 *	@brief		设置图层编号
 *	@param[in]	i_nLayer				图层编号
 *	@param[out]	无
 *	@retval		STK_SUCCESS				设置成功
 *	@note		该接口为设置图层号，不是设置图层ID
 *	@attention	无
 *****************************************************************************/
STK_STATUS Stk_Layer::SetLayer(STK_INT32 i_nLayer)
{
	m_nLayer = i_nLayer;

	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		获取图层编号
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		图层编号
 *	@note		该接口为获取图层号，不是获取图层ID
 *	@attention	无
 *****************************************************************************/
STK_INT32 Stk_Layer::GetLayer()
{
	return m_nLayer;
}

/*************************************************************************//**
 *	@brief		设置图层名称
 *	@param[in]	i_wstrName				图层名称
 *	@param[out]	无
 *	@retval		STK_SUCCESS				设置成功
 *	@note		无
 *	@attention	无
 *****************************************************************************/
STK_STATUS Stk_Layer::SetLayerName(wstring i_wstrName)
{
	m_wstrName = i_wstrName;

	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		获取图层名称
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		图层名称
 *	@note		无
 *	@attention	无
 *****************************************************************************/
wstring Stk_Layer::GetLayerName()
{
	return m_wstrName;
}

/************************************************************************//**
 *	@brief		设置图层显隐状态
 *	@param[in]	i_bIsDisplay	显隐状态
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Layer::SetDisplay(STK_BOOL i_bIsDisplay)
{
	m_bIsDisplay = i_bIsDisplay;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		判断图层显隐状态
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		显隐状态
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_BOOL Stk_Layer::IsDisplay()
{
	return m_bIsDisplay;
}

/*************************************************************************//**
 *	@brief		设置图层与要素的关联
 *	@param[in]	i_stkNodeConnectorP		图层与要素的关联
 *	@param[out]	无
 *	@retval		STK_SUCCESS				设置成功
 *	@note		无
 *	@attention	无
 *****************************************************************************/
STK_STATUS Stk_Layer::AddNodeConnector(Stk_NodeConnector* i_stkNodeConnectorP)
{
	if (i_stkNodeConnectorP != NULL)
	{
		m_vcNodeConnectors.push_back(i_stkNodeConnectorP);
	}
	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		获取该图层中与要素的关联数目
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		关联数目
 *	@note		无
 *	@attention	无
 *****************************************************************************/
STK_SIZE_T Stk_Layer::GetNodeConnectorNum()
{
	return m_vcNodeConnectors.size();
}

/*************************************************************************//**
 *	@brief		获取指定的图层与要素的关联
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		指定的图层与要素的关联
 *	@note		无
 *	@attention	无
 *****************************************************************************/
Stk_NodeConnector* Stk_Layer::GetNodeConnectorByIndex(STK_UINT32 i_nuIndex)
{
	return m_vcNodeConnectors[i_nuIndex];
}

/************************************************************************//**
 *	@brief		读取文件中的图层类数据
 *	@param[in]	i_FileP				文件对象指针
 *	@param[in]	i_HeadP				图层记录头指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS			读取成功
 *	@retval		STK_ERROR_MEMORY	内存申请失败
 *	@retval		其他				读取失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_Layer::ReadFile(Stk_File *i_FileP, StkSegmentHead *i_HeadP)
{
	StkSegmentHead		head={0};
	StkControl**		RecordPP = NULL;
	StkLayerRec*		layerRecP = NULL;
	Stk_NodeConnector*	stkNodeConnectorP = NULL;
	STK_UINT32			nuNameSize = 0;
	STK_STATUS			eState;

	/*
	 * 读取图层段的数据
	 */
	RecordPP = (StkControl **)calloc(i_HeadP->recnum, sizeof(StkControl*));
	if (i_HeadP->recnum != 0 && RecordPP == NULL)
	{
		return STK_ERROR_MEMORY;
	}
	eState = i_FileP->ReadSegmentAllRecordData(i_HeadP, (StkControl **)RecordPP);
	if (eState!=STK_SUCCESS)
	{
		return eState;
	}

	/*
	 * 读取图层段的各记录
	 */
	for (int ii = 0; ii < i_HeadP->recnum; ii++)
	{
		if (RecordPP[ii]->type == REC_TYPE_LAYERINFO)
		{
			if (RecordPP[ii]->version == 1)
			{
				layerRecP = (StkLayerRec*)RecordPP[ii];
				m_nLayer = layerRecP->nLayer;
				nuNameSize = layerRecP->nuNameSize;
				if (!((layerRecP->IsItem) & STK_LAYER_IS_DISPLAY))
				{
					SetDisplay(false);
				}

				STK_WCHAR* wchLayerName = new STK_WCHAR[nuNameSize];
				if (nuNameSize > 0 && wchLayerName == NULL)
				{
					return STK_ERROR_MEMORY;
				}
				for (int jj = 0; jj < nuNameSize; jj++)
				{
					wchLayerName[jj] = layerRecP->atr[jj];
				}
#ifdef __MOBILE__
				m_wstrName = Stk_Platform::STKWCHARStrToWString(wchLayerName);
#else
				m_wstrName = wchLayerName;
#endif
				//删除临时分配的内存
				if (wchLayerName != NULL)
				{
					delete [] wchLayerName;
					wchLayerName = NULL;
				}
			}
		}
		else
		{

		}
	}

	// 释放内存
	eState = i_FileP->ReadSegmentFinishData(i_HeadP, (StkControl **)RecordPP);
	if (eState!=STK_SUCCESS)
	{
		return STK_ERROR_HEADRD;
	}
	if (RecordPP != NULL)
	{
		free(RecordPP);
		RecordPP = NULL;
	}

	/*
	 *	读取下一段的数据
	 */
	while(1)
	{
		eState = i_FileP->ReadSegmentHeadData(&head);
		if (eState!=STK_SUCCESS)
		{
			return STK_ERROR_HEADRD;
		}
		switch (head.kind)
		{
			//当下一段是NodeConnector段时
		case SEG_TYPE_CONNECTOR:
			stkNodeConnectorP = new Stk_NodeConnector(m_stkProtoP);
			eState=stkNodeConnectorP->ReadFile(i_FileP, &head);
			if (eState!=STK_SUCCESS)
			{
				delete stkNodeConnectorP;
				return eState;
			}

			AddNodeConnector(stkNodeConnectorP);
			break;
			//当下一段是图层-END段时
		case SEG_TYPE_LAYER_END:
			goto rtn;
			break;
		default:
			StkControl **TmpRecordPP = NULL;
			TmpRecordPP = (StkControl **)calloc(head.recnum, sizeof(StkControl*));
			if (head.recnum !=0 && TmpRecordPP == NULL)
			{
				return STK_ERROR_MEMORY;
			}
			eState = i_FileP->ReadSegmentAllRecordData(&head, (StkControl **)TmpRecordPP);
			if (eState!=STK_SUCCESS)
			{
				return STK_ERROR_HEADRD;
			}
			eState = i_FileP->ReadSegmentFinishData(&head, (StkControl **)TmpRecordPP);// 释放内存
			if (eState!=STK_SUCCESS)
			{
				return STK_ERROR_HEADRD;
			}
			if (TmpRecordPP != NULL)
			{
				free(TmpRecordPP);
				TmpRecordPP = NULL;
			}
			break;
		}
	}

rtn:
	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		写图层数据
 *	@param[in]	i_stkFileP			文件对象指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS			写入成功
 *	@retval		其它				写入失败
 *	@note		无
 *	@attention	内部接口
 *****************************************************************************/
STK_STATUS Stk_Layer::WriteFile(Stk_File *i_stkFileP)
{
	StkSegmentHead		head = {0};
	StkSegmentHead		endHead = {0};
	StkLayerRec*		layerRecP = NULL;
	STK_UINT32			nuNameSize = 0;
	STK_STATUS			ret = STK_SUCCESS;

	if(i_stkFileP == NULL)
	{
		return STK_ERROR;
	}

	/*
	 *	【第1步】 输出图层段
	 */
	// 【1.1】 输出Table HEAD Record
	head.id = 0;
	head.kind = SEG_TYPE_LAYER;
	ret = i_stkFileP->WriteSegmentHeadData(&head);
	if(ret < STK_SUCCESS)
	{
		return STK_ERROR;
	}
	// 【1.2】 输出图层记录
	nuNameSize = m_wstrName.size() + 1;
	layerRecP = (StkLayerRec *)calloc(1, sizeof(StkLayerRec) - sizeof(STK_WCHAR) + nuNameSize*sizeof(STK_WCHAR));
	if(layerRecP == NULL)
	{
		return STK_ERROR;
	}
	layerRecP->Control.type = REC_TYPE_LAYERINFO;
	layerRecP->Control.version = _VER_LAYERINFO_RECORD;
	layerRecP->Control.length = sizeof(StkLayerRec) - sizeof(STK_WCHAR) + nuNameSize*sizeof(STK_WCHAR) - sizeof(StkControl);
	layerRecP->nLayer = m_nLayer;
	layerRecP->nuNameSize = nuNameSize;
	layerRecP->IsItem = 0;
	if (m_bIsDisplay)
	{
		layerRecP->IsItem |= STK_LAYER_IS_DISPLAY;
	}

#ifdef __MOBILE__
	memcpy(layerRecP->atr, Stk_Platform::WStringToSTKWCHARStr(m_wstrName), nuNameSize*sizeof(STK_WCHAR));
#else
	wcscpy_s(layerRecP->atr, nuNameSize, m_wstrName.c_str());	//把结束符L'\0'同时加进去
#endif
	// 【1.3】 完成图层记录的输出
	ret = i_stkFileP->WriteSegmentRecordData(&head, (StkControl *)layerRecP);
	if(ret < STK_SUCCESS)
	{
		return STK_ERROR;
	}

	if (layerRecP != NULL)
	{
		free(layerRecP);
		layerRecP = NULL;
	}

	// 【1.4】 完成图层段的输出
	ret = i_stkFileP->WriteSegmentFinishData(&head);
	if(ret < STK_SUCCESS)
	{
		return STK_ERROR;
	}

	/*
	 *	【第2步】 输出图层与要素关联段
	 */
	for (int ii = 0; ii < m_vcNodeConnectors.size(); ii++)
	{
		if (m_vcNodeConnectors[ii] != NULL)
		{
			m_vcNodeConnectors[ii]->WriteFile(i_stkFileP);
		}
	}

	/*
	 *	【第三步】 输出图层-END段
	 */
	endHead.id = 0;
	endHead.kind = SEG_TYPE_LAYER_END;
	ret = i_stkFileP->WriteSegmentHeadData(&endHead);
	if(ret < STK_SUCCESS)
	{
		return STK_ERROR;
	}

	return STK_SUCCESS;
}