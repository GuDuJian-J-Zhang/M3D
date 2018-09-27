// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_UserDataEntity.cpp
 *
 *	@brief	用户数据类的实现
 *
 *	@par	历史:
 *		2014/08/27	创建。Gaoqq@HOTEAMSOFT
****************************************************************************/

#ifndef __MOBILE__
#pragma once
#endif

#include "StdAfx.h"
#include "Stk_UserData.h"
#include "Stk_Platform.h"
#include "Stk_DocumentManager.h"
#ifdef _WINDOWS_
#include "Stk_LogFile.h"
#endif

/****************************************************************************
 *	@brief		Stk_UserData构造函数
 *	@param[in]	i_stkProtoP			所属零件指针
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_UserData::Stk_UserData(Stk_ProtoType* i_stkProtoP)
{
	m_stkProtoP = i_stkProtoP;
	m_wstrDataName = L"";
	m_wstrFileName = L"";
	m_id = 0;
	m_dataP = NULL;
	m_nudataSize = 0;
}

/****************************************************************************
 *	@brief		Stk_UserData析构函数
 *	@param[in]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_UserData::~Stk_UserData()
{
	if (m_dataP != NULL)
	{
		free(m_dataP);
		m_dataP = NULL;
	}
}

#if 0
/****************************************************************************
 *	@brief		设置用户数据文件的路径
 *	@param[in]	i_wstrFilePath	文件路径
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

void Stk_UserData::SetFilePath(wstring i_wstrFilePath)
{
	STK_INT32	nuPos = 0;

	// 设置FilePath
	m_wstrFilePath = i_wstrFilePath;

	// 设置FileName
	nuPos = i_wstrFilePath.find_last_of(L"\\");
	if (nuPos != -1)
	{
		m_wstrFileName = i_wstrFilePath.substr(nuPos + 1, i_wstrFilePath.length() - nuPos - 1);
	}
	else{
		nuPos = i_wstrFilePath.find_last_of(L"/");
		m_wstrFileName = i_wstrFilePath.substr(nuPos + 1, i_wstrFilePath.length() - nuPos - 1);
	}

}

/****************************************************************************
 *	@brief		获取用户数据文件的路径
 *	@param[in]	无
 *	@retval		文件路径
 *	@note		无
 *	@attention	无
 ****************************************************************************/

wstring Stk_UserData::GetFilePath()
{
	return m_wstrFilePath;
}
#endif

/****************************************************************************
 *	@brief		设置用户数据
 *	@param[in]	i_wstrDataName	数据名称
 *	@param[in]	i_id			数据ID
 *	@param[in]	i_dataP			数据流
 *	@param[in]	i_dataSize		数据流长度
 *	@param[out]	i_wstrFileName	物理文件名(建议包括后缀)
 *	@param[out]	无
 *	@retval		STK_SUCCESS		成功
 *	@retval		其它			失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS	Stk_UserData::SetUserData( wstring i_wstrDataName, STK_ID i_id, STK_CHAR *i_dataP, STK_SIZE_T i_dataSize, wstring i_wstrFileName)
{
	m_wstrDataName = i_wstrDataName;
	m_nudataSize = i_dataSize;
	m_wstrFileName = i_wstrFileName;
	m_id = i_id;

	if (i_dataSize>0)
	{
		m_dataP = (STK_CHAR *)malloc(i_dataSize);
		if (m_dataP == NULL)
		{
			return STK_ERROR;
		}
		memcpy(m_dataP, i_dataP, i_dataSize);
	}

	return STK_SUCCESS;
}

/****************************************************************************
 *	@brief		获取用户数据
 *	@param[in]	无
 *	@param[out]	o_wstrDataName	数据名称
 *	@param[out]	o_id			数据ID
 *	@param[out]	o_dataP			数据流
 *	@param[out]	o_dataSize		数据流长度
 *	@param[out]	o_wstrFileName	物理文件名
 *	@retval		STK_SUCCESS		成功
 *	@retval		其它			失败
 *	@note		参数o_dataP是为用户分配的内存副本，使用完请用free释放内存
 *	@attention	
 ****************************************************************************/
STK_STATUS	Stk_UserData::GetUserData( wstring &o_wstrDataName, STK_ID &o_id, STK_CHAR* &o_dataP, STK_SIZE_T &o_dataSize, wstring &o_wstrFileName)
{
	o_wstrDataName = m_wstrDataName;
	o_id = m_id;
	o_dataSize = m_nudataSize;
	o_wstrFileName = m_wstrFileName;

	if (m_nudataSize>0)
	{
		o_dataP = (STK_CHAR *)malloc(m_nudataSize);
		if (m_dataP == NULL)
		{
			return STK_ERROR;
		}
		memcpy(o_dataP, m_dataP, m_nudataSize);
	}

	return STK_SUCCESS;
}


/****************************************************************************
 *	@brief		往SVL文件读取用户数据
 *	@param[in]	i_stkFileP		SVL文件指针
 *	@param[in]	i_propHead		用户数据段头
 *	@param[out]	无
 *	@retval		STK_SUCCESS		填写成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_UserData::WriteUserData(Stk_File *i_stkFileP, StkSegmentHead* i_userHead)
{
	StkUserDataRec2*	userDataRecP = NULL;
	STK_STATUS			ret = STK_SUCCESS;
	STK_UINT32			nuNumFilePath = 0;
	STK_UINT32			nuNumValue = 0;
	STK_UINT32			nuNumUnits = 0;
#ifdef _WINDOWS_
	WCHAR			strConvertLog[MAX_PATH + 512];
	Stk_SVLLog		stkSVLLog;
	Stk_LogFileEx*	stkSVLLogFile;
#endif

	if (i_userHead == NULL || i_stkFileP == NULL)
	{
		return STK_ERROR;
	}

#ifdef _WINDOWS_
	stkSVLLog = i_stkFileP->GetDocumentManager()->GetSVLLogIni();
	stkSVLLogFile = i_stkFileP->GetDocumentManager()->GetSVLLogFile();
#endif

	userDataRecP = (StkUserDataRec2*)calloc(1, sizeof(StkUserDataRec2) - sizeof(STK_CHAR) + m_nudataSize * sizeof(STK_CHAR));

	// 【第1.2步】输出Thumbnail Record
	userDataRecP->Control.type = REC_TYPE_USERDEFDATA;
	userDataRecP->Control.version = _VER_USERDEFDATA_RECORD /*2*/;
	userDataRecP->Control.length = sizeof(StkUserDataRec2) - sizeof(STK_CHAR) + m_nudataSize * sizeof(STK_CHAR) - sizeof(StkControl);

#ifdef __MOBILE__ 
	memcpy(userDataRecP->dataName,Stk_Platform::WStringToSTKWCHARStr(m_wstrDataName),(m_wstrDataName.size()+1)*sizeof(STK_WCHAR));
	memcpy(userDataRecP->FileName,Stk_Platform::WStringToSTKWCHARStr(m_wstrFileName),(m_wstrFileName.size()+1)*sizeof(STK_WCHAR));
#else
	wcscpy_s(userDataRecP->dataName, STK_ULTRANAME, (WCHAR*)m_wstrDataName.c_str());
	wcscpy_s(userDataRecP->FileName, STK_ULTRANAME, (WCHAR*)m_wstrFileName.c_str());
#endif
	userDataRecP->dataID = m_id;
	userDataRecP->nuCharNum = m_nudataSize;
	memcpy( userDataRecP->atr, m_dataP, m_nudataSize);

	ret = i_stkFileP->WriteSegmentRecordData(i_userHead, (StkControl *)userDataRecP);
	if(ret < STK_SUCCESS)
	{
#ifdef _WINDOWS_
		if (1 == stkSVLLog.bLogOutput)
		{
			_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：UserData记录写入失败！"));
			stkSVLLogFile->Log(strConvertLog);
		}
#endif
		return STK_ERROR;
	}

	if(userDataRecP != NULL){
		free(userDataRecP);
		userDataRecP = NULL;
	}

	return STK_SUCCESS;
}
