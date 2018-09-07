// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_LayerInfosSet.h
 *
 *	@brief	图层信息集类
 *
 *	@par	历史:
 *		2014/05/13	创建。WangY@HOTEAMSOFT
****************************************************************************/

#ifndef _SVLLIB_LAYER_ENTITY_H_
#define _SVLLIB_LAYER_ENTITY_H_

#ifndef __MOBILE__
#pragma once
#endif

#include "Stk_Object.h"
#include "Stk_Connector.h"

namespace HoteamSoft {
	namespace SVLLib {

		/**************************************************************************************
			 *	@class	Stk_LayerFilter
			 *	@brief	图层过滤器类
			 *
			 *	继承自Stk_Entity类。
		***************************************************************************************/

		class STK_TOOLKIT_EXPORT Stk_LayerFilter :public Stk_Object
		{
			STK_DECLARE_CLASS(Stk_LayerFilter)
		protected:
			enum { _typeID = _SVLLIB_TYPE_LAYERFILTER };	//!< 类别
		private:
			wstring						m_wstrName;					//!< 过滤器名称
			wstring						m_wstrDescription;			//!< 过滤器描述
			//vector<STK_BOOL>			m_vcSelected;				//!< 图层选择标志
			//vector<STK_UINT32>		m_vcSelectedLayers;			//!< 选择的图层
			map<STK_INT32, STK_BOOL>	m_mapSelectedLayers;		//!< 符合图层过滤器条件的图层与选择状态之间的映射
		public:
			/*****************************************************************************
			 *	@brief		图层过滤器构造函数
			 *	@param[in]	无
			 *	@param[out]	无
			 *	@retval		无
			 *	@note		无
			 *	@attention	无
			 *****************************************************************************/
			Stk_LayerFilter();

			/*****************************************************************************
			 *	@brief		图层过滤器析构函数
			 *	@param[in]	无
			 *	@param[out]	无
			 *	@retval		无
			 *	@note		无
			 *	@attention	无
			 *****************************************************************************/
			~Stk_LayerFilter();

			/*****************************************************************************
			 *	@brief		图层过滤器赋值构造函数
			 *	@param[in]	i_stkLayerFilter		图层过滤器
			 *	@param[out]	无
			 *	@retval		图层过滤器对象
			 *	@note		无
			 *	@attention	无
			 *****************************************************************************/
			Stk_LayerFilter& operator = (const Stk_LayerFilter& i_stkLayerFilter);

			/*****************************************************************************
			 *	@brief		设置图层过滤器名称
			 *	@param[in]	i_wstrName		图层过滤器名称
			 *	@param[out]	无
			 *	@retval		STK_SUCCESS		设置成功
			 *	@note		无
			 *	@attention	无
			 *****************************************************************************/
			STK_STATUS					SetLayerFilterName(wstring i_wstrName);

			/*****************************************************************************
			 *	@brief		获取图层过滤器名称
			 *	@param[in]	无
			 *	@param[out]	无
			 *	@retval		m_wstrName		图层过滤器名称
			 *	@note		无
			 *	@attention	无
			 *****************************************************************************/
			wstring						GetLayerFilterName();

			/*****************************************************************************
			 *	@brief		设置图层过滤器描述
			 *	@param[in]	i_wstrDescription	图层过滤器描述
			 *	@param[out]	无
			 *	@retval		STK_SUCCESS			设置成功
			 *	@note		无
			 *	@attention	无
			 *****************************************************************************/
			STK_STATUS					SetDescription(wstring i_wstrDescription);

			/*****************************************************************************
			 *	@brief		获取图层过滤器描述
			 *	@param[in]	无
			 *	@param[out]	无
			 *	@retval		m_wstrDescription		图层过滤器描述
			 *	@note		无
			 *	@attention	无
			 *****************************************************************************/
			wstring						GetDescription();

			//STK_STATUS				AddSelectedState(STK_BOOL i_bSelected);
			//STK_BOOL					IsSelected(STK_UINT32 i_nuLayer);

			//STK_STATUS				AddSelectedLayer(STK_UINT32 i_nuLayer);
			//vector<STK_UINT32>&		GetAllSelectedLayers();

			/*****************************************************************************
			 *	@brief		设置被选择图层与选择状态之间的映射
			 *	@param[in]	i_nSelectedLayer			被选择的图层
			 *	@param[in]	i_bSelectedState			被选择的图层的状态
			 *	@param[out]	无
			 *	@retval		STK_SUCCESS					设置成功
			 *	@note		无
			 *	@attention	无
			 *****************************************************************************/
			STK_STATUS					AddMapLayerState(STK_INT32 i_nSelectedLayer, STK_BOOL i_bSelectedState);

			/*****************************************************************************
			 *	@brief		获取被选择图层与选择状态之间的映射
			 *	@param[in]	无
			 *	@param[out]	无
			 *	@retval		m_mapSelectedLayers		映射关系
			 *	@note		无
			 *	@attention	无
			 *****************************************************************************/
			map<STK_INT32, STK_BOOL>&	GetAllMapLayerStates();

		};

		/**************************************************************************************
			 *	@class	Stk_Layer
			 *	@brief	图层类
			 *
			 *	继承自Stk_Entity类。
		***************************************************************************************/

		class STK_TOOLKIT_EXPORT Stk_Layer :
			public Stk_Object
		{
			STK_DECLARE_CLASS(Stk_Layer)
		protected:
			enum { _typeID = _SVLLIB_TYPE_LAYER };	//!< 类别
		private:
			STK_INT32					m_nLayer;				//!< 图层号
			wstring						m_wstrName;				//!< 图层名
			STK_BOOL					m_bIsDisplay;			//!< 显隐状态
			vector<Stk_ConnectorPtr>	m_vcNodeConnectors;			//!< 图层与要素的关联
		public:
			/*****************************************************************************
			 *	@brief		图层构造函数
			 *	@param[in]	无
			 *	@param[out]	无
			 *	@retval		无
			 *	@note		无
			 *	@attention	无
			 *****************************************************************************/
			Stk_Layer();
			/*****************************************************************************
			 *	@brief		图层析构函数
			 *	@param[in]	无
			 *	@param[out]	无
			 *	@retval		无
			 *	@note		无
			 *	@attention	无
			 *****************************************************************************/
			~Stk_Layer();
		public:
			/*****************************************************************************
			 *	@brief		设置图层编号
			 *	@param[in]	i_nLayer				图层编号
			 *	@param[out]	无
			 *	@retval		STK_SUCCESS				设置成功
			 *	@note		该接口为设置图层号，不是设置图层ID
			 *	@attention	无
			 *****************************************************************************/
			STK_STATUS					SetLayer(STK_INT32 i_nLayer);

			/*****************************************************************************
			 *	@brief		获取图层编号
			 *	@param[in]	无
			 *	@param[out]	无
			 *	@retval		图层编号
			 *	@note		该接口为获取图层号，不是获取图层ID
			 *	@attention	无
			 *****************************************************************************/
			STK_INT32					GetLayer();

			/*****************************************************************************
			 *	@brief		设置图层名称
			 *	@param[in]	i_wstrName				图层名称
			 *	@param[out]	无
			 *	@retval		STK_SUCCESS				设置成功
			 *	@note		无
			 *	@attention	无
			 *****************************************************************************/
			STK_STATUS					SetLayerName(wstring i_wstrName);

			/*****************************************************************************
			 *	@brief		获取图层名称
			 *	@param[in]	无
			 *	@param[out]	无
			 *	@retval		图层名称
			 *	@note		无
			 *	@attention	无
			 *****************************************************************************/
			wstring						GetLayerName();

			/****************************************************************************
			 *	@brief		设置图层显隐状态
			 *	@param[in]	i_bIsDisplay	显隐状态
			 *	@param[out]	无
			 *	@retval		STK_SUCCESS		设置成功
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			STK_STATUS					SetDisplay(STK_BOOL i_bIsDisplay);

			/****************************************************************************
			 *	@brief		判断图层显隐状态
			 *	@param[in]	无
			 *	@param[out]	无
			 *	@retval		显隐状态
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			STK_BOOL					IsDisplay();

			/*****************************************************************************
			 *	@brief		设置图层与要素的关联
			 *	@param[in]	i_stkNodeConnectorP		图层与要素的关联
			 *	@param[out]	无
			 *	@retval		STK_SUCCESS				设置成功
			 *	@note		无
			 *	@attention	无
			 *****************************************************************************/
			STK_STATUS					AddNodeConnector(Stk_ConnectorPtr i_stkNodeConnectorP);

			/*****************************************************************************
			 *	@brief		获取该图层中与要素的关联数目
			 *	@param[in]	无
			 *	@param[out]	无
			 *	@retval		关联数目
			 *	@note		无
			 *	@attention	无
			 *****************************************************************************/
			STK_SIZE_T					GetNodeConnectorNum();

			/*****************************************************************************
			 *	@brief		获取指定的图层与要素的关联
			 *	@param[in]	无
			 *	@param[out]	无
			 *	@retval		指定的图层与要素的关联
			 *	@note		无
			 *	@attention	无
			 *****************************************************************************/
			Stk_ConnectorPtr			GetNodeConnectorByIndex(STK_UINT32 i_nuIndex);
		};

		/**************************************************************************************
			 *	@class	Stk_LayerInfosSet
			 *	@brief	图层信息集类
			 *
			 *	继承自Stk_Entity类。
		***************************************************************************************/

		class STK_TOOLKIT_EXPORT Stk_LayerInfosSet :
			public Stk_Object
		{
			STK_DECLARE_CLASS(Stk_LayerInfosSet)
		protected:
			enum { _typeID = _SVLLIB_TYPE_LAYERINFOS };	//!< 类别
		private:
			STK_ID						m_iModelId;				//!< 所属模型编号
			STK_INT32					m_nDefaultLayer;		//!< 默认图层
			//map<STK_UINT32, wstring>	m_mapLayerAndName;		//!< 图层编号与图层名称之间的映射
			vector<Stk_LayerPtr>			m_vcLayers;			//!< 图层数组
			STK_INT32					m_nDefLayerFilter;		//!< 默认图层过滤器编号
			vector<Stk_LayerFilterPtr>	m_vcLayerFilters;		//!< 图层过滤器
		public:
			/****************************************************************************
			 *	@brief		Stk_LayerInfosSet构造函数
			 *	@param[in]	无
			 *	@param[out]	无
			 *	@retval		无
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			Stk_LayerInfosSet();

			//Stk_LayerInfosSet(const Stk_LayerInfosSet& i_stkLayerInfosSet);

			/****************************************************************************
			 *	@brief		Stk_LayerInfosSet析构函数
			 *	@param[in]	无
			 *	@param[out]	无
			 *	@retval		无
			 *	@note		如果已关联到Node，会自动析构Node并解除Node父子关联
			 *	@attention	无
			 ****************************************************************************/
			~Stk_LayerInfosSet();
			//Stk_LayerInfosSet& operator = (const Stk_LayerInfosSet& i_stkLayerInfosSet);

			/****************************************************************************
			 *	@brief		设置默认图层
			 *	@param[in]	i_nLayer		图层
			 *	@param[out]	无
			 *	@retval		STK_SUCCESS		设置成功
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			STK_STATUS					SetDefaultLayer(STK_INT32 i_nLayer);

			/****************************************************************************
			 *	@brief		获取默认图层
			 *	@param[in]	无
			 *	@param[out]	无
			 *	@retval		m_nuDefaultLayer	默认图层
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			STK_INT32					GetDefaultLayer();

			/****************************************************************************
			 *	@brief		添加图层编号和图层名称的映射
			 *	@param[in]	i_nuLayer			图层编号
			 *	@param[in]	i_wstrName			图层名称
			 *	@param[out]	无
			 *	@retval		STK_SUCCESS			添加成功
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			 //STK_STATUS					AddLayerAndName(STK_UINT32 i_nuLayer, wstring i_wstrName);

			 /****************************************************************************
			  *	@brief		获取所有图层编号和图层名称的映射
			  *	@param[in]	无
			  *	@param[out]	无
			  *	@retval		m_mapIndexAndName		所有图层编号和图层名称的映射
			  *	@note		无
			  *	@attention	无
			  ****************************************************************************/
			  //map<STK_UINT32, wstring>&	GetAllLayerAndName();

			  /****************************************************************************
			   *	@brief		添加图层
			   *	@param[in]	i_stkLayerP			图层对象指针
			   *	@param[out]	无
			   *	@retval		STK_SUCCESS			添加成功
			   *	@note		无
			   *	@attention	无
			   ****************************************************************************/
			STK_STATUS						AddLayer(Stk_LayerPtr i_stkLayerP);

			/****************************************************************************
			 *	@brief		获取图层数量
			 *	@param[in]	无
			 *	@param[out]	无
			 *	@retval		图层数量
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			STK_UINT32						GetLayerNum();

			/****************************************************************************
			 *	@brief		获取指定的图层
			 *	@param[in]	i_nuIndex		指定图层索引
			 *	@param[out]	无
			 *	@retval		指定的图层
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			Stk_LayerPtr						GetLayerByIndex(STK_UINT32 i_nuIndex);

			/****************************************************************************
			*	@brief		获取指定的图层
			*	@param[in]	i_iID		指定图层ID
			*	@param[out]	无
			*	@retval		指定的图层
			*	@note		无
			*	@attention	无
			****************************************************************************/
			Stk_LayerPtr GetLayerById(STK_ID i_iID);

			/****************************************************************************
			 *	@brief		设置默认图层过滤器
			 *	@param[in]	i_nLayerFilter			默认图层过滤器编号
			 *	@param[out]	无
			 *	@retval		STK_SUCCESS				设置成功
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			STK_STATUS					SetDefaultFilter(STK_INT32 i_nLayerFilter);

			/****************************************************************************
			 *	@brief		获取默认图层过滤器
			 *	@param[in]	无
			 *	@param[out]	无
			 *	@retval		m_stkDefFilterP			默认图层过滤器
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			Stk_LayerFilterPtr			GetDefaultFilter();

			/****************************************************************************
			 *	@brief		添加图层过滤器
			 *	@param[in]	i_stkLayerFilter		图层过滤器
			 *	@param[out]	无
			 *	@retval		STK_SUCCESS				添加成功
			 *	@retval		STK_ERROR				添加失败
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			STK_STATUS					AddLayerFilter(Stk_LayerFilterPtr i_stkLayerFilter);

			/****************************************************************************
			 *	@brief		获取指定索引对应的图层过滤器
			 *	@param[in]	i_nuIndex				索引号
			 *	@param[out]	无
			 *	@retval		图层过滤器
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			Stk_LayerFilterPtr			GetLayerFilterByIndex(STK_UINT32 i_nuIndex);

			/****************************************************************************
			 *	@brief		获取所有的图层过滤器
			 *	@param[in]	无
			 *	@param[out]	无
			 *	@retval		m_vcLayerFilters		所有图层过滤器
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			vector<Stk_LayerFilterPtr>	GetAllLayerFilters();

			/*****************************************************************************
			 *	@brief		设置所属模型的ID
			 *	@param[in]	i_ModelId				模型的ID
			 *	@param[out]	无
			 *	@retval		STK_SUCCESS			设置成功
			 *	@note		无
			 *	@attention	内部接口
			 ****************************************************************************/
			STK_STATUS					SetModelId(STK_ID i_ModelId);

			/*****************************************************************************
			 *	@brief		获取所对应的模型的ID
			 *	@param[in]	无
			 *	@param[out]	无
			 *	@retval		模型的ID
			 *	@note		无
			 *	@attention	内部接口
			 ****************************************************************************/
			STK_ID					GetModelId();


		};
	}
}

#endif