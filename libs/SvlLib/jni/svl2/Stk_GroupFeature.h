// All Rights Reserved. Copyright (C) 2014 HOTEAMSOFT, Ltd
/*****************************************************************************
 *	@file	Stk_GroupFeature.h
 *
 *	@brief	组特征类
 *
 *	@par	历史:
 *		2014/11/17	创建。WangY@HOTEAMSOFT
****************************************************************************/

#ifndef _STK_GROUPFEATURE_ENTITY_H_
#define _STK_GROUPFEATURE_ENTITY_H_

#ifndef __MOBILE__
#pragma once
#endif

#include "Stk_Object.h"
#include "Stk_ComponentFeature.h"

/****************************************************************************
	 *	@class	Stk_GroupFeature
	 *	@brief	组特征类
	 *
	 *	区别于DTK中有关Node下的特征，该特征在装配级。<br>
	 *	继承自Stk_ComponentFeature 类。
 ****************************************************************************/
class DATAMANAGER_API Stk_GroupFeature :
	public Stk_ComponentFeature
{
private:
	//STK_ID							m_idPlacement;			// 当前特征相对上级的配置ID
	vector<STK_ID>					m_vcInstancePlcIDs;		// 组件特征下所包含的实例配置ID
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
	Stk_GroupFeature(Stk_DocumentManager* i_stkDocManagerP, Stk_ProtoType* i_stkProtoP);

	/*****************************************************************************
	 *	@brief		析构函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	~Stk_GroupFeature();
public:
	/****************************************************************************
	 *	@brief		设置当前特征相对上级的配置ID
	 *	@param[in]	i_idPlacement		配置ID
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	//STK_STATUS				SetPlcID(STK_ID i_idPlacement);

	/****************************************************************************
	 *	@brief		获取当前特征相对上级的配置ID
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		配置ID号
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	//STK_ID					GetPlcID();

	/****************************************************************************
	 *	@brief		往特征下添加包含的实例配置ID
	 *	@param[in]	i_idPlacement		配置ID
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				AddChildInsPlcID(STK_ID i_idPlacement);

	/****************************************************************************
	 *	@brief		获取特征下包含的实例数量
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		实例数量
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_UINT32				GetChildInsNum();

	/****************************************************************************
	 *	@brief		获取特征下指定的实例配置ID
	 *	@param[in]	i_nuIndex			索引
	 *	@param[out]	无
	 *	@retval		指定的实例配置ID
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_ID					GetChildInsPlcIDByIndex(STK_UINT32 i_nuIndex);

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