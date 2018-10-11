// All Rights Reserved. Copyright (C) 2014 HOTEAMSOFT, Ltd
/*****************************************************************************
 *	@file	Stk_VirtualBomItemInfo.h
 *
 *	@brief	工艺BOM属性信息栏类
 *
 *	@par	历史:
 *		2016/02/24	创建。WangY@HOTEAMSOFT
****************************************************************************/

#ifndef _STK_VIRTUAL_BOMITEMINFO_H_
#define _STK_VIRTUAL_BOMITEMINFO_H_

#ifndef __MOBILE__
#pragma once
#endif

#include "Stk_Entity.h"
#include "Stk_Enum.h"
#include "Stk_File.h"

/****************************************************************************
	 *	@class	Stk_VirtualBomItemInfo
	 *	@brief	工艺BOM属性信息栏类
	 *
	 *	创建用于存储BOM属性的类库。<br>
	 *	继承自Stk_Entity 类。
 ****************************************************************************/

class DATAMANAGER_API Stk_VirtualBomItemInfo :
	public Stk_Entity
{
private:
	wstring					m_wstrID;			//!< 序号
	wstring					m_wstrSymbol;		//!< 代号
	wstring					m_wstrName;			//!< 名称
	wstring					m_wstrMaterial;		//!< 材料
	STK_UINT32				m_nuNumber;			//!< 共有当前序号的数量
protected:
public:
	/****************************************************************************
	 *	@brief		工艺BOM属性信息栏构造函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_VirtualBomItemInfo();
	
	/****************************************************************************
	 *	@brief		工艺BOM属性信息栏析构函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	~Stk_VirtualBomItemInfo();
public:
	/****************************************************************************
	 *	@brief		设置工艺BOM标签信息的序号
	 *	@param[in]	i_wstrID		工艺BOM标签信息的序号
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetID(wstring i_wstrID);
	
	/****************************************************************************
	 *	@brief		获取工艺BOM标签信息的序号
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		工艺BOM标签信息的序号
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	wstring					GetID();

	/****************************************************************************
	 *	@brief		设置工艺BOM标签信息的代号
	 *	@param[in]	i_wstrSymbol	代号
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetSymbol(wstring i_wstrSymbol);
	
	/****************************************************************************
	 *	@brief		获取工艺BOM标签信息的代号
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		工艺BOM标签信息的代号
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	wstring					GetSymbol();

	/****************************************************************************
	 *	@brief		设置名称
	 *	@param[in]	i_wstrName			名称
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetName(wstring i_wstrName);

	/****************************************************************************
	 *	@brief		获取名称
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		名称
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	wstring					GetName();

	/****************************************************************************
	 *	@brief		设置材料
	 *	@param[in]	i_wstrMaterial	材料
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetMaterial(wstring i_wstrMaterial);
	
	/****************************************************************************
	 *	@brief		获取材料
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		材料
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	wstring					GetMaterial();

	/****************************************************************************
	 *	@brief		设置共有当前序号的实例数量
	 *	@param[in]	i_nuNumber		共有当前序号的实例数量
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetNumber(STK_UINT32 i_nuNumber);

	/****************************************************************************
	 *	@brief		获取共有当前序号的实例数量
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		共有当前序号的实例数量
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_UINT32				GetNumber();
	
	/****************************************************************************
	 *	@brief		写工艺BOM文件
	 *	@param[in]	i_stkFileP			文件指针
	 *	@param[in]	i_VirtualBomHeadSP	工艺BOM段头
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			写入成功
	 *	@retval		其他				写入失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS			WriteFile(Stk_File *i_stkFileP, StkSegmentHead *i_VirtualBomHeadSP);
};

#endif