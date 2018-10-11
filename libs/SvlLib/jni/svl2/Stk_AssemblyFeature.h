// All Rights Reserved. Copyright (C) 2014 HOTEAMSOFT, Ltd
/*****************************************************************************
 *	@file	Stk_AssemblyFeature.h
 *
 *	@brief	装配特征类
 *
 *	@par	历史:
 *		2014/11/17	创建。WangY@HOTEAMSOFT
****************************************************************************/

#ifndef _STK_ASSEMBLYFEATURE_ENTITY_H_
#define _STK_ASSEMBLYFEATURE_ENTITY_H_

#ifndef __MOBILE__
#pragma once
#endif

#include "Stk_Object.h"
#include "Stk_ComponentFeature.h"

/****************************************************************************
	 *	@class	Stk_AssemblyFeature
	 *	@brief	装配特征类
	 *
	 *	区别于DTK中有关Node下的特征，该特征在装配级。<br>
	 *	继承自Stk_ComponentFeature 类。
 ****************************************************************************/
class DATAMANAGER_API Stk_AssemblyFeature :
	public Stk_ComponentFeature
{
private:
	vector<STK_ID>				m_vcNodeIds;			//!< 装配特征关联的Node ID
	wstring						m_wstrPlcPath;			//!< 实例相对于顶级Proto的配置路径
public:
	/*****************************************************************************
	 *	@brief		构造函数
	 *	@param[in]	i_stkDocManagerP		Document管理器对象指针
	 *	@param[in]	i_stkProtoP				ProtoType
	 *	@param[out]	无
	 *	@retval		无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_AssemblyFeature(Stk_DocumentManager* i_stkDocManagerP, Stk_ProtoType* i_stkProtoP);

	/*****************************************************************************
	 *	@brief		析构函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	~Stk_AssemblyFeature();
public:
	/****************************************************************************
	 *	@brief		添加装配特征中所包含的要素Node ID
	 *	@param[in]	i_idPlacement		配置ID
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				AddNodeID(STK_ID i_idPlacement);

	/****************************************************************************
	 *	@brief		获取装配特征中所包含的要素Node数量
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		Node数量
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_UINT32				GetNodeNum();

	/****************************************************************************
	 *	@brief		获取指定的Node ID
	 *	@param[in]	i_nuIndex			索引
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_ID					GetNodeIDByIndex(STK_UINT32 i_nuIndex);

	/****************************************************************************
	 *	@brief		设置装配特征关联的实例的配置路径
	 *	@param[in]	i_wstrPlcPath		配置路径
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetPlcPath(wstring i_wstrPlcPath);

	/****************************************************************************
	 *	@brief		获取装配特征关联的实例的配置路径
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		配置路径
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	wstring					GetPlcPath();

	/*****************************************************************************
	 *	@brief		写组件特征文件
	 *	@param[in]	i_stkFileP			文件对象指针
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			保存成功
	 *	@retval		STK_ERROR			保存失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				WriteFile(Stk_File *i_stkFileP);
	
	/****************************************************************************
	 *	@brief		读取文件中的ProtoType数据
	 *	@param[in]	i_stkFileP			文件对象指针
	 *	@param[in]	i_FeatHeadP			组件特征记录头指针
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			读取成功
	 *	@retval		STK_ERROR			读取失败
	 *	@retval		STK_ERROR_MEMORY	内存申请失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				ReadFile(Stk_File *i_stkFileP, StkSegmentHead *i_FeatHeadP);
};

#endif