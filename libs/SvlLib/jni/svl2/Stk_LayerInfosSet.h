// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_LayerInfosSet.h
 *
 *	@brief	图层信息集类
 *
 *	@par	历史:
 *		2014/05/13	创建。WangY@HOTEAMSOFT
****************************************************************************/

#ifndef _Stk_LAYER_ENTITY_H_
#define _Stk_LAYER_ENTITY_H_

#ifndef __MOBILE__
#pragma once
#endif

#include "Stk_Entity.h"
#include "Stk_File.h"
#include "Stk_NodeConnector.h"

class Stk_Node;

/**************************************************************************************
	 *	@class	Stk_LayerFilter
	 *	@brief	图层过滤器类
	 *
	 *	继承自Stk_Entity类。
***************************************************************************************/

class DATAMANAGER_API Stk_LayerFilter :
	public Stk_Entity
{
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
	 *	@brief		图层过滤器拷贝构造函数
	 *	@param[in]	i_stkLayerFilter		图层过滤器
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 *****************************************************************************/
	Stk_LayerFilter(const Stk_LayerFilter& i_stkLayerFilter);

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

	/*****************************************************************************
	 *	@brief		写图层过滤器数据
	 *	@param[in]	i_head				数据段头指针
	 *	@param[in]	i_stkFileP			文件对象指针
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			写入成功
	 *	@retval		其它				写入失败
	 *	@note		无
	 *	@attention	内部接口
	 *****************************************************************************/
	STK_STATUS					WriteLayerFilter(StkSegmentHead *i_head, Stk_File *i_stkFileP);
};

/**************************************************************************************
	 *	@class	Stk_Layer
	 *	@brief	图层类
	 *
	 *	继承自Stk_Entity类。
***************************************************************************************/

class DATAMANAGER_API Stk_Layer :
	public Stk_Entity
{
private:
	STK_INT32					m_nLayer;				//!< 图层号
	wstring						m_wstrName;				//!< 图层名
	STK_BOOL					m_bIsDisplay;			//!< 显隐状态
	vector<Stk_NodeConnector*>	m_vcNodeConnectors;		//!< 图层与要素的关联
public:
	/*****************************************************************************
	 *	@brief		图层构造函数
	 *	@param[in]	i_stkProtoP		所属组件
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 *****************************************************************************/
	Stk_Layer(Stk_ProtoType* i_stkProtoP);
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
	STK_STATUS					AddNodeConnector(Stk_NodeConnector* i_stkNodeConnectorP);

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
	Stk_NodeConnector*			GetNodeConnectorByIndex(STK_UINT32 i_nuIndex);

	/****************************************************************************
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
	STK_STATUS					ReadFile(Stk_File *i_FileP, StkSegmentHead *i_HeadP);

	/*****************************************************************************
	 *	@brief		写图层数据
	 *	@param[in]	i_stkFileP			文件对象指针
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			写入成功
	 *	@retval		其它				写入失败
	 *	@note		无
	 *	@attention	内部接口
	 *****************************************************************************/
	STK_STATUS					WriteFile(Stk_File *i_stkFileP);
};

/**************************************************************************************
	 *	@class	Stk_LayerInfosSet
	 *	@brief	图层信息集类
	 *
	 *	继承自Stk_Entity类。
***************************************************************************************/

class DATAMANAGER_API Stk_LayerInfosSet :
	public Stk_Entity
{
private:
	Stk_Node*					m_NodeP;				//!< 所属Node
	STK_INT32					m_nDefaultLayer;		//!< 默认图层
	//map<STK_UINT32, wstring>	m_mapLayerAndName;		//!< 图层编号与图层名称之间的映射
	vector<Stk_Layer*>			m_vcLayers;				//!< 图层数组
	STK_INT32					m_nDefLayerFilter;		//!< 默认图层过滤器编号
	vector<Stk_LayerFilter*>	m_vcLayerFilters;		//!< 图层过滤器
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

	/****************************************************************************
	 *	@brief		Stk_LayerInfosSet构造函数
	 *	@param[in]	i_stkProtoTypeP	Prototype对象指针
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_LayerInfosSet(Stk_ProtoType* i_stkProtoTypeP);
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
	STK_STATUS						AddLayer(Stk_Layer* i_stkLayerP);

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
	Stk_Layer*						GetLayerByIndex(STK_UINT32 i_nuIndex);

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
	Stk_LayerFilter*			GetDefaultFilter();

	/****************************************************************************
	 *	@brief		添加图层过滤器
	 *	@param[in]	i_stkLayerFilter		图层过滤器
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS				添加成功
	 *	@retval		STK_ERROR				添加失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					AddLayerFilter(Stk_LayerFilter* i_stkLayerFilter);
	
	/****************************************************************************
	 *	@brief		获取指定索引对应的图层过滤器
	 *	@param[in]	i_nuIndex				索引号
	 *	@param[out]	无
	 *	@retval		图层过滤器
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_LayerFilter*			GetLayerFilterByIndex(STK_UINT32 i_nuIndex);
	
	/****************************************************************************
	 *	@brief		获取所有的图层过滤器
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_vcLayerFilters		所有图层过滤器
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	vector<Stk_LayerFilter*>	GetAllLayerFilters();

	/*****************************************************************************
	 *	@brief		设置所对应的Node
	 *	@param[in]	i_NodeP				所对应的Node
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					SetNode(Stk_Node* i_NodeP);

	/*****************************************************************************
	 *	@brief		获取所对应的Node
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		所对应的Node
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	Stk_Node*					GetNode();

	/****************************************************************************
	 *	@brief		读取文件中的图层信息集数据
	 *	@param[in]	i_FileP				文件对象指针
	 *	@param[in]	i_LayerSetHeadP		图层信息集记录头指针
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			读取成功
	 *	@retval		STK_ERROR_MEMORY	内存申请失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					ReadFile(Stk_File *i_FileP, StkSegmentHead *i_LayerSetHeadP);
	
	/*****************************************************************************
	 *	@brief		写图层信息集数据
	 *	@param[in]	i_stkFileP			文件对象指针
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			写入成功
	 *	@retval		其它				写入失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					WriteFile(Stk_File *i_stkFileP);

};

#endif