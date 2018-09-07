// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_LayerInfosSet.cpp
 *
 *	@brief	图层信息集、图层过滤器类 
 *
 *	@par	历史:
 *		2014/05/16	创建。WangY@HOTEAMSOFT
****************************************************************************/

#include "Stk_LayerInfosSet.h"

namespace HoteamSoft {
	namespace SVLLib {
		STK_IMPLEMENT_CLASS(Stk_LayerInfosSet, Stk_Object)
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
			m_iModelId = 0;
			m_nDefaultLayer = -1;
			m_nDefLayerFilter = -1;
		}


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
			m_vcLayerFilters.clear();

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
		STK_STATUS Stk_LayerInfosSet::AddLayer(Stk_LayerPtr i_stkLayerP)
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
		Stk_LayerPtr Stk_LayerInfosSet::GetLayerByIndex(STK_UINT32 i_nuIndex)
		{
			return m_vcLayers[i_nuIndex];
		}

		/************************************************************************
		*	@brief		获取指定的图层
		*	@param[in]	i_iID		指定图层ID
		*	@param[out]	无
		*	@retval		指定的图层
		*	@note		无
		*	@attention	无
		****************************************************************************/
		Stk_LayerPtr Stk_LayerInfosSet::GetLayerById(STK_ID i_iID)
		{
			Stk_LayerPtr pLayer = nullptr;
			for (int i = 0; i < m_vcLayers.size(); i++)
			{
				if(m_vcLayers[i] == nullptr)
					continue;
				if (m_vcLayers[i]->GetLayer() == i_iID)
				{
					pLayer = m_vcLayers[i];
					break;
				}
			}
			return pLayer;
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

		Stk_LayerFilterPtr Stk_LayerInfosSet::GetDefaultFilter()
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

		STK_STATUS Stk_LayerInfosSet::AddLayerFilter(Stk_LayerFilterPtr i_stkLayerFilter)
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

		Stk_LayerFilterPtr Stk_LayerInfosSet::GetLayerFilterByIndex(STK_UINT32 i_nuIndex)
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

		vector<Stk_LayerFilterPtr> Stk_LayerInfosSet::GetAllLayerFilters()
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
		STK_STATUS Stk_LayerInfosSet::SetModelId(STK_ID i_ModelId)
		{
			m_iModelId = i_ModelId;
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
		STK_ID Stk_LayerInfosSet::GetModelId()
		{
			return m_iModelId;
		}


		STK_IMPLEMENT_CLASS(Stk_LayerFilter, Stk_Object)
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


		STK_IMPLEMENT_CLASS(Stk_Layer, Stk_Object)
		/*************************************************************************//**
		*	@brief		图层构造函数
		*	@param[in]	i_stkProtoP		所属组件
		*	@param[out]	无
		*	@retval		无
		*	@note		无
		*	@attention	无
		*****************************************************************************/
		Stk_Layer::Stk_Layer()
		{
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
			m_vcNodeConnectors.clear();
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
		STK_STATUS Stk_Layer::AddNodeConnector(Stk_ConnectorPtr i_stkNodeConnectorP)
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
		Stk_ConnectorPtr Stk_Layer::GetNodeConnectorByIndex(STK_UINT32 i_nuIndex)
		{
			return m_vcNodeConnectors[i_nuIndex];
		}

	}
}