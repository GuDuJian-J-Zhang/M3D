// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_MetaDataEntity.cpp
 *
 *	@brief	自定义属性类的实现
 *
 *	@par	历史:
 *		2014/01/08	创建。WangY@HOTEAMSOFT
****************************************************************************/

#ifndef __MOBILE__
#pragma once
#endif

#include "StdAfx.h"
#include "Stk_MetaDataEntity.h"
#include "Stk_Platform.h"
#include "Stk_DocumentManager.h"
#ifdef _WINDOWS_
#include "Stk_LogFile.h"
#endif

/************************************************************************//**
 *	@brief		Stk_MetaData构造函数
 *	@param[in]	i_stkProtoP			组件ProtoType对象指针
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_MetaData::Stk_MetaData(Stk_ProtoType* i_stkProtoP)
{
	m_stkProtoP = i_stkProtoP;
	m_wstrTitle = L"";
	m_wstrValue = L"";
	m_wstrUnits = L"";
	m_eValueType = TypeValueUnknown;
	m_NodeP = NULL;
}

/************************************************************************//**
 *	@brief		Stk_MetaData析构函数
 *	@param[in]	无
 *	@retval		无
 *	@note		如果已关联到Node，会自动析构Node并解除Node父子关联
 *	@attention	无
 ****************************************************************************/

Stk_MetaData::~Stk_MetaData()
{
	// 删除所从属的Node节点
	if (m_NodeP != NULL)
	{
		m_NodeP->SetMetaData(NULL); //防止析构Node时再次析构Meta造成死循环
		delete m_NodeP;
		m_NodeP = NULL;
	}
}

/************************************************************************//**
 *	@brief		设置自定义属性名称
 *	@param[in]	i_wstrTitle			自定义属性名称
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_MetaData::SetTitle(wstring i_wstrTitle)
{
	m_wstrTitle = i_wstrTitle;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取自定义属性名称
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_wstrTitle		自定义属性名称
 *	@note		无
 *	@attention	无
 ****************************************************************************/

wstring Stk_MetaData::GetTitle()
{
	return m_wstrTitle;
}

/************************************************************************//**
 *	@brief		设置自定义属性值的类型
 *	@param[in]	i_eValueType	自定义属性值的类型
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_MetaData::SetType(StkMetaDataValueTypeEnum i_eValueType)
{
	m_eValueType = i_eValueType;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取自定义属性值的类型
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_eValueType		自定义属性值的类型
 *	@note		无
 *	@attention	无
 ****************************************************************************/

StkMetaDataValueTypeEnum Stk_MetaData::GetType()
{
	return m_eValueType;
}

/************************************************************************//**
 *	@brief		设置自定义属性值
 *	@param[in]	i_wstrValue		自定义属性值
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_MetaData::SetValue(wstring i_wstrValue)
{
	m_wstrValue = i_wstrValue;

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取自定义属性值
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_wstrValue		自定义属性值
 *	@note		无
 *	@attention	无
 ****************************************************************************/

wstring Stk_MetaData::GetValue()
{
	return m_wstrValue;
}

/************************************************************************//**
 *	@brief		设置自定义属性单位
 *	@param[in]	i_wstrUnits		自定义属性单位
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_MetaData::SetUnits(wstring i_wstrUnits)
{
	m_wstrUnits = i_wstrUnits;

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取自定义属性单位
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_wstrUnits		自定义属性单位
 *	@note		无
 *	@attention	无
 ****************************************************************************/

wstring Stk_MetaData::GetUnits()
{
	return m_wstrUnits;
}

/*************************************************************************//**
 *	@brief		设置所对应的Node
 *	@param[in]	i_NodeP				所对应的Node
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_MetaData::SetNode(Stk_Node* i_NodeP)
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
Stk_Node* Stk_MetaData::GetNode()
{
	return m_NodeP;
}

/************************************************************************//**
 *	@brief		从SVL中读取自定义属性
 *	@param[in]	i_FileP				SVL文件指针
 *	@param[in]	i_PropHeadP			自定义属性段头
 *	@param[out]	无
 *	@retval		STK_SUCCESS			读取成功
 *	@retval		STK_ERROR_MEMORY	内存申请失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_MetaData::ReadMetaData(Stk_File *i_FileP, StkSegmentHead *i_PropHeadP)
{
	StkMetaDataRec*		MetaDataRecP = NULL;
	StkControl**		RecordPP = NULL;
	STK_UINT32			nuNumTitle = 0;
	STK_UINT32			nuNumValue = 0;
	STK_UINT32			nuNumUnits = 0;
	STK_UINT32			ii, jj;
	STK_STATUS			eState;

	/*
	 *	读取自定义属性段的数据
	 */
	RecordPP = (StkControl**)calloc(1, sizeof(StkControl *) * i_PropHeadP->recnum);
	if (i_PropHeadP->recnum != 0 && RecordPP == NULL)
	{
		return STK_ERROR_MEMORY;
	}
	eState = i_FileP->ReadSegmentAllRecordData(i_PropHeadP, (StkControl **)RecordPP);
	if (eState!=STK_SUCCESS)
	{
		return eState;
	}
	/*
	 *	读取自定义属性的各记录
	 */
	for (ii = 0; ii < i_PropHeadP->recnum; ii++)
	{
		if (RecordPP[ii]->type == REC_TYPE_METADATA)
		{
			// 版本1
			if (RecordPP[ii]->version == 1)
			{
				MetaDataRecP = (StkMetaDataRec*)RecordPP[ii];
				//m_eValueType = (StkMetaDataValueTypeEnum)MetaDataRecP->nuValueType;
				// 属性名称
				nuNumTitle = MetaDataRecP->nuNumTitle;
				STK_WCHAR* wchTitleP = new STK_WCHAR[nuNumTitle];
				if (nuNumTitle > 0 && wchTitleP == NULL)
				{
					return STK_ERROR_MEMORY;
				}
				for (jj = 0; jj < nuNumTitle; jj++)
				{
					wchTitleP[jj] = MetaDataRecP->atr[jj];
				}
#ifdef __MOBILE__
				m_wstrTitle = Stk_Platform::STKWCHARStrToWString((STK_WCHAR*)wchTitleP);
#else
				m_wstrTitle = wchTitleP;
#endif
				//属性类型
				m_eValueType = (StkMetaDataValueTypeEnum)MetaDataRecP->nuValueType;
				// 属性值
				nuNumValue = MetaDataRecP->nuNumValue;
				STK_WCHAR* wchValueP = new STK_WCHAR[nuNumValue];
				if (nuNumValue > 0 && wchValueP == NULL)
				{
					return STK_ERROR_MEMORY;
				}
				for (jj = 0; jj < nuNumValue; jj++)
				{
					wchValueP[jj] = MetaDataRecP->atr[nuNumTitle + jj];
				}

#ifdef __MOBILE__
				m_wstrValue = Stk_Platform::STKWCHARStrToWString(wchValueP);
#else
				m_wstrValue = wchValueP;
#endif
				// 属性单位
				nuNumUnits = MetaDataRecP->nuNumUnits;
				STK_WCHAR* wchUnitsP = new STK_WCHAR[nuNumUnits];
				if (nuNumUnits > 0 && wchUnitsP == NULL)
				{
					return STK_ERROR_MEMORY;
				}
				for (jj = 0; jj < nuNumUnits; jj++)
				{
					wchUnitsP[jj] = MetaDataRecP->atr[nuNumTitle + nuNumValue + jj];
				}
#ifdef __MOBILE__
				m_wstrUnits = Stk_Platform::STKWCHARStrToWString(wchUnitsP);
#else
				m_wstrUnits = wchUnitsP;
#endif

				//删除临时分配的内存
				if (wchTitleP != NULL)
				{
					delete [] wchTitleP;
					wchTitleP = NULL;
				}
				if (wchValueP != NULL)
				{
					delete [] wchValueP;
					wchValueP = NULL;
				}
				if (wchUnitsP != NULL)
				{
					delete [] wchUnitsP;
					wchUnitsP = NULL;
				}
			}
		}
	}

	// 释放内存
	i_FileP->ReadSegmentFinishData(i_PropHeadP, (StkControl **)RecordPP);
	if (RecordPP != NULL)
	{
		free(RecordPP);
		RecordPP = NULL;
	}

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		往SVL文件读取自定义属性
 *	@param[in]	i_propHead		自定义属性段头
 *	@param[in]	i_stkFileP		SVL文件指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS		填写成功
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_MetaData::WriteMetaData(StkSegmentHead* i_propHead, Stk_File *i_stkFileP)
{
	StkMetaDataRec*		metaDataRecP = NULL;
	STK_STATUS			ret = STK_SUCCESS;
	STK_UINT32			nuNumTitle = 0;
	STK_UINT32			nuNumValue = 0;
	STK_UINT32			nuNumUnits = 0;
	STK_CHAR*			wchItorP = NULL;	// 游标

#ifdef _WINDOWS_
	WCHAR			strConvertLog[MAX_PATH + 512];
	Stk_SVLLog		stkSVLLog;
	Stk_LogFileEx*	stkSVLLogFile;
#endif

	if (i_propHead == NULL || i_stkFileP == NULL)
	{
		return STK_ERROR;
	}
#ifdef _WINDOWS_
	stkSVLLog = i_stkFileP->GetDocumentManager()->GetSVLLogIni();
	stkSVLLogFile = i_stkFileP->GetDocumentManager()->GetSVLLogFile();
	if (1 == stkSVLLog.bLogOutput && 1 == stkSVLLog.bMetaData)
	{
		_stprintf_s(strConvertLog,  _countof(strConvertLog), 
			_T("【MetaData段】属性名：%s 类型：%d 值：%s 单位：%s"), m_wstrTitle.c_str(), m_eValueType, m_wstrValue.c_str(), m_wstrUnits.c_str());
		stkSVLLogFile->Log(strConvertLog);
	}
#endif

	nuNumTitle = m_wstrTitle.size() + 1;
	nuNumValue = m_wstrValue.size() + 1;
	nuNumUnits = m_wstrUnits.size() + 1;
	metaDataRecP = (StkMetaDataRec *)calloc(1, sizeof(StkMetaDataRec) - sizeof(STK_WCHAR)
		+ nuNumTitle * sizeof(STK_WCHAR) + nuNumValue * sizeof(STK_WCHAR) + nuNumUnits * sizeof(STK_WCHAR));
	if (metaDataRecP == NULL)
	{
		return STK_ERROR;
	}
	/*
	 *	属性填充
	 */
	metaDataRecP->Control.type = REC_TYPE_METADATA;
	metaDataRecP->Control.length = sizeof(StkMetaDataRec) - sizeof(STK_WCHAR) 
		+ nuNumTitle * sizeof(STK_WCHAR) + nuNumValue * sizeof(STK_WCHAR) + nuNumUnits * sizeof(STK_WCHAR) - sizeof(StkControl);
	metaDataRecP->Control.version = _VER_METADATA_RECORD;

	metaDataRecP->nuNumTitle = nuNumTitle;
	metaDataRecP->nuNumValue = nuNumValue;
	metaDataRecP->nuNumUnits = nuNumUnits;

	metaDataRecP->nuValueType = m_eValueType;
	
#ifdef __MOBILE__
	wchItorP = (STK_CHAR*)metaDataRecP->atr;
	memcpy(wchItorP,Stk_Platform::WStringToSTKWCHARStr(m_wstrTitle),nuNumTitle*sizeof(STK_WCHAR));
	wchItorP += nuNumTitle*sizeof(STK_WCHAR);
	memcpy(wchItorP,Stk_Platform::WStringToSTKWCHARStr(m_wstrValue),nuNumValue*sizeof(STK_WCHAR));
	wchItorP += nuNumValue*sizeof(STK_WCHAR);
	memcpy(wchItorP,Stk_Platform::WStringToSTKWCHARStr(m_wstrUnits),nuNumUnits*sizeof(STK_WCHAR));
#else
	for (int ii = 0; ii < nuNumTitle - 1; ii++)
	{
		metaDataRecP->atr[ii] = m_wstrTitle.at(ii);
	}
	metaDataRecP->atr[nuNumTitle - 1] = L'\0';

	for (int ii = 0; ii < nuNumValue - 1; ii++)
	{
		metaDataRecP->atr[nuNumTitle + ii] = m_wstrValue.at(ii);
	}
	metaDataRecP->atr[nuNumTitle + nuNumValue - 1] = L'\0';

	for (int ii = 0; ii < nuNumUnits - 1; ii++)
	{
		metaDataRecP->atr[nuNumTitle + nuNumValue + ii] = m_wstrUnits.at(ii);
	}
	metaDataRecP->atr[nuNumTitle + nuNumValue + nuNumUnits - 1] = L'\0';
#endif

	ret = i_stkFileP->WriteSegmentRecordData(i_propHead, (StkControl *)metaDataRecP);
	if(ret < STK_SUCCESS)
	{
#ifdef _WINDOWS_
		if (1 == stkSVLLog.bLogOutput && 1 == stkSVLLog.bMetaData)
		{
			_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：MetaData Base记录写入失败！"));
			stkSVLLogFile->Log(strConvertLog);
		}
#endif
		return STK_ERROR;
	}

	if(metaDataRecP != NULL)
	{
		free(metaDataRecP);
		metaDataRecP = NULL;
	}

	return STK_SUCCESS;
}
