// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_VirtualBomItemInfo.cpp
 *
 *	@brief	工艺BOM属性信息栏类的实现
 *
 *	@par	历史:
 *		2016/02/24	创建。WangY@HOTEAMSOFT
****************************************************************************/

#ifndef __MOBILE__
#pragma once
#endif

#include "StdAfx.h"
#include "Stk_VirtualBomItemInfo.h"
#include "Stk_DocumentManager.h"
#include "Stk_Platform.h"
#ifdef _WINDOWS_
#include "Stk_LogFile.h"
#endif

/************************************************************************//**
 *	@brief		工艺BOM属性信息栏构造函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_VirtualBomItemInfo::Stk_VirtualBomItemInfo()
{
	m_wstrID = L"";
	m_wstrSymbol = L"";
	m_wstrName = L"";
	m_wstrMaterial = L"";
	m_nuNumber = 0;
}

/************************************************************************//**
 *	@brief		工艺BOM属性信息栏析构函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_VirtualBomItemInfo::~Stk_VirtualBomItemInfo()
{

}

/************************************************************************//**
 *	@brief		设置工艺BOM标签信息的序号
 *	@param[in]	i_wstrID		工艺BOM标签信息的序号
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_VirtualBomItemInfo::SetID(wstring i_wstrID)
{
	m_wstrID = i_wstrID;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取工艺BOM标签信息的序号
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		工艺BOM标签信息的序号
 *	@note		无
 *	@attention	无
 ****************************************************************************/
wstring Stk_VirtualBomItemInfo::GetID()
{
	return m_wstrID;
}

/************************************************************************//**
 *	@brief		设置工艺BOM标签信息的代号
 *	@param[in]	i_wstrSymbol	代号
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_VirtualBomItemInfo::SetSymbol(wstring i_wstrSymbol)
{
	m_wstrSymbol = i_wstrSymbol;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取工艺BOM标签信息的代号
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		工艺BOM标签信息的代号
 *	@note		无
 *	@attention	无
 ****************************************************************************/
wstring Stk_VirtualBomItemInfo::GetSymbol()
{
	return m_wstrSymbol;
}

/************************************************************************//**
 *	@brief		设置名称
 *	@param[in]	i_wstrName			名称
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_VirtualBomItemInfo::SetName(wstring i_wstrName)
{
	m_wstrName = i_wstrName;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取名称
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		名称
 *	@note		无
 *	@attention	无
 ****************************************************************************/
wstring Stk_VirtualBomItemInfo::GetName()
{
	return m_wstrName;
}

/************************************************************************//**
 *	@brief		设置材料
 *	@param[in]	i_wstrMaterial	材料
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_VirtualBomItemInfo::SetMaterial(wstring i_wstrMaterial)
{
	m_wstrMaterial = i_wstrMaterial;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取材料
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		材料
 *	@note		无
 *	@attention	无
 ****************************************************************************/
wstring Stk_VirtualBomItemInfo::GetMaterial()
{
	return m_wstrMaterial;
}

/************************************************************************//**
 *	@brief		设置共有当前序号的实例数量
 *	@param[in]	i_nuNumber		共有当前序号的实例数量
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_VirtualBomItemInfo::SetNumber(STK_UINT32 i_nuNumber)
{
	m_nuNumber = i_nuNumber;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取共有当前序号的实例数量
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		共有当前序号的实例数量
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_UINT32 Stk_VirtualBomItemInfo::GetNumber()
{
	return m_nuNumber;
}

/************************************************************************//**
 *	@brief		写工艺BOM文件
 *	@param[in]	i_stkFileP			文件指针
 *	@param[in]	i_VirtualBomHeadSP	工艺BOM段头
 *	@param[out]	无
 *	@retval		STK_SUCCESS			写入成功
 *	@retval		其他				写入失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_VirtualBomItemInfo::WriteFile(Stk_File *i_stkFileP, StkSegmentHead *i_VirtualBomHeadSP)
{
	STK_STATUS			eState = STK_SUCCESS;
	StkVirtualBomRec	virtualBomRec = {0};
	
	if(i_stkFileP == NULL || i_VirtualBomHeadSP == NULL)
	{
		return STK_ERROR;
	}

	/*
	 *	属性填充
	 */
	virtualBomRec.Control.type = REC_TYPE_VIRTUALBOM_ITEM;
	virtualBomRec.Control.length = sizeof(StkVirtualBomRec) - sizeof(StkControl);
	virtualBomRec.Control.version = _VER_VIRTUALBOM_ITEM_RECORD;

#ifdef __MOBILE__
	memcpy(virtualBomRec.chID,Stk_Platform::WStringToSTKWCHARStr(m_wstrID),(m_wstrID.size()+1)*sizeof(STK_WCHAR));
	memcpy(virtualBomRec.chSymbol,Stk_Platform::WStringToSTKWCHARStr(m_wstrSymbol),(m_wstrSymbol.size()+1)*sizeof(STK_WCHAR));
	memcpy(virtualBomRec.chName,Stk_Platform::WStringToSTKWCHARStr(m_wstrName),(m_wstrName.size()+1)*sizeof(STK_WCHAR));
	memcpy(virtualBomRec.chMaterial,Stk_Platform::WStringToSTKWCHARStr(m_wstrMaterial),(m_wstrMaterial.size()+1)*sizeof(STK_WCHAR));
#else
	wcscpy_s(virtualBomRec.chID, m_wstrID.size() + 1, (WCHAR*)m_wstrID.c_str());
	wcscpy_s(virtualBomRec.chSymbol, m_wstrSymbol.size() + 1, (WCHAR*)m_wstrSymbol.c_str());
	wcscpy_s(virtualBomRec.chName, m_wstrName.size() + 1, (WCHAR*)m_wstrName.c_str());
	wcscpy_s(virtualBomRec.chMaterial, m_wstrMaterial.size() + 1, (WCHAR*)m_wstrMaterial.c_str());
#endif
	virtualBomRec.nuNumber = m_nuNumber;

	/*
	 *	【1.2】输出工艺BOM表格栏的记录
	 */
	eState = i_stkFileP->WriteSegmentRecordData(i_VirtualBomHeadSP, (StkControl *)&virtualBomRec);
	if(eState != STK_SUCCESS)
	{
		return STK_ERROR;
	}

	return eState;
}