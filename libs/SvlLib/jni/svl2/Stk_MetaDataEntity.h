// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_MetaDataEntity.h
 *
 *	@brief	Stk_MetaData自定义属性类
 *
 *	@par	历史:
 *		2014/01/08	创建。WangY@HOTEAMSOFT
****************************************************************************/

#ifndef _Stk_METADATA_ENTITY_H_
#define _Stk_METADATA_ENTITY_H_

#ifndef __MOBILE__
#pragma once
#endif

#include "DataManager.h"
#include "Stk_File.h"
#include "Stk_Entity.h"

class Stk_Node;

/**************************************************************************************
	 *	@class	Stk_MetaData
	 *	@brief	自定义属性类
	 *
	 *	自定义属性。<br>
	 *	继承自Stk_Entity类。
***************************************************************************************/

class DATAMANAGER_API  Stk_MetaData :
	public Stk_Entity
{
private:
	Stk_Node*					m_NodeP;						//!< 所属Node
	wstring						m_wstrTitle;					//!< 自定义属性名
	StkMetaDataValueTypeEnum	m_eValueType;					//!< 自定义属性值的类型（枚举表示）
	wstring						m_wstrValue;					//!< 自定义属性值
	wstring						m_wstrUnits;					//!< 自定义属性单位
public:
	/****************************************************************************
	 *	@brief		Stk_MetaData构造函数
	 *	@param[in]	i_stkProtoP			组件ProtoType对象指针
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_MetaData(Stk_ProtoType* i_stkProtoP);

	/****************************************************************************
	 *	@brief		Stk_MetaData析构函数
	 *	@param[in]	无
	 *	@retval		无
	 *	@note		如果已关联到Node，会自动析构Node并解除Node父子关联
	 *	@attention	无
	 ****************************************************************************/
	~Stk_MetaData();
public:
	/****************************************************************************
	 *	@brief		设置自定义属性名称
	 *	@param[in]	i_wstrTitle			自定义属性名称
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					SetTitle(wstring i_wstrTitle);

	/****************************************************************************
	 *	@brief		获取自定义属性名称
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_wstrTitle		自定义属性名称
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	wstring						GetTitle();

	/****************************************************************************
	 *	@brief		设置自定义属性值的类型
	 *	@param[in]	i_eValueType	自定义属性值的类型
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					SetType(StkMetaDataValueTypeEnum i_eValueType);
	
	/****************************************************************************
	 *	@brief		获取自定义属性值的类型
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_eValueType		自定义属性值的类型
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	StkMetaDataValueTypeEnum	GetType();

	/****************************************************************************
	 *	@brief		设置自定义属性值
	 *	@param[in]	i_wstrValue		自定义属性值
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					SetValue(wstring i_wstrValue);

	/****************************************************************************
	 *	@brief		获取自定义属性值
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_wstrValue		自定义属性值
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	wstring						GetValue();

	/****************************************************************************
	 *	@brief		设置自定义属性单位
	 *	@param[in]	i_wstrUnits		自定义属性单位
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS					SetUnits(wstring i_wstrUnits);

	/****************************************************************************
	 *	@brief		获取自定义属性单位
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		m_wstrUnits		自定义属性单位
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	wstring						GetUnits();

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
	 *	@brief		从SVL中读取自定义属性
	 *	@param[in]	i_FileP				SVL文件指针
	 *	@param[in]	i_PropHeadP			自定义属性段头
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			读取成功
	 *	@retval		STK_ERROR_MEMORY	内存申请失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					ReadMetaData(Stk_File *i_FileP, StkSegmentHead *i_PropHeadP);
	
	/****************************************************************************
	 *	@brief		往SVL文件读取自定义属性
	 *	@param[in]	i_propHead		自定义属性段头
	 *	@param[in]	i_stkFileP		SVL文件指针
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		填写成功
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS					WriteMetaData(StkSegmentHead* i_propHead, Stk_File *i_stkFileP);

};

#endif