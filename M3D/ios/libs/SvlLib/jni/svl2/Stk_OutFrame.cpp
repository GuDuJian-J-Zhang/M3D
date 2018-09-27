// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_OutFrame.cpp
 *
 *	@brief	外框类
 *
 *	@par	历史:
 *		2013/08/19	创建。WangY@HOTEAMSOFT
****************************************************************************/

#ifndef __MOBILE__
#pragma once
#endif

#include "StdAfx.h"
#include "Stk_OutFrame.h"
#include "Stk_DocumentManager.h"
#ifdef _WINDOWS_
#include "Stk_LogFile.h"
#endif
/************************************************************************//**
 *	@brief		Stk_OutFrame构造函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_OutFrame::Stk_OutFrame(void)
{
	m_eFrameType = OUTFRAME_USAGE_UNKNOWN;
	m_eDefType = STK_FRAME_DEF_PMI;
}

/************************************************************************//**
 *	@brief		Stk_OutFrame析构函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_OutFrame::~Stk_OutFrame(void)
{
	for (int ii = 0; ii < m_vcCurves.size(); ii++)
	{
		if (m_vcCurves[ii] != NULL)
		{
			delete m_vcCurves[ii];
			m_vcCurves[ii] = NULL;
		}
	}
}

/************************************************************************//**
 *	@brief		获取外框的应用类型
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_eFrameType		外框类型
 *	@note		无
 *	@attention	无
 ****************************************************************************/

StkOutFrameUsageEnum Stk_OutFrame::GetFrameType()
{
	return m_eFrameType;
}

/************************************************************************//**
 *	@brief		设置外框的应用类型
 *	@param[in]	i_eType				外框类型
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_OutFrame::SetFrameType(StkOutFrameUsageEnum i_eType)
{
	m_eFrameType = i_eType;
	return STK_SUCCESS;
}

/****************************************************************************
 *	@brief		获取外框的定义类型
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_eFrameType		外框类型
 *	@note		无
 *	@attention	无
 ****************************************************************************/

StkOutFrameDefEnum Stk_OutFrame::GetDefineType()
{
	return m_eDefType;
}

/****************************************************************************
 *	@brief		设置外框的定义类型
 *	@param[in]	i_eType				外框类型
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_OutFrame::SetDefineType(StkOutFrameDefEnum i_eType)
{
	m_eDefType = i_eType;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取所有的外框线数据
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_vcCurves			所有的外框线数据
 *	@note		无
 *	@attention	无
 ****************************************************************************/

const vector<Stk_CurveEntity*>& Stk_OutFrame::GetFrameData()
{
	return m_vcCurves;
}

/************************************************************************//**
 *	@brief		设置外框线数据
 *	@param[in]	i_CurveP			外框线数据
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_OutFrame::SetFrameData(Stk_CurveEntity* i_CurveP)
{
	m_vcCurves.push_back(i_CurveP);
	return STK_SUCCESS;
}
/*************************************************************************//**
 *	@brief		写入外框数据
 *	@param[in]	i_head			数据段头指针
 *	@param[in]	i_stkFileP		文件对象指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS		写入成功
 *	@retval		其它			写入失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_OutFrame::WriteFrame(StkSegmentHead *i_head, Stk_File *i_stkFileP)
{
	STK_STATUS		ret = STK_SUCCESS;
#ifdef _WINDOWS_
	WCHAR			strConvertLog[MAX_PATH + 512];
	Stk_SVLLog		stkSVLLog;
	Stk_LogFileEx*	stkSVLLogFile;
#endif

	if(i_head == NULL || i_stkFileP == NULL)
	{
		return STK_ERROR;
	}
#ifdef _WINDOWS_
	stkSVLLog = i_stkFileP->GetDocumentManager()->GetSVLLogIni();
	stkSVLLogFile = i_stkFileP->GetDocumentManager()->GetSVLLogFile();
#endif

	StkFrameRec2 *OutFrameRec2 = NULL;
	OutFrameRec2 = (StkFrameRec2*)calloc(1, sizeof(StkFrameRec2));
	if(OutFrameRec2 == NULL){
		return STK_ERROR;
	}
	/*
	 *	属性填充
	 */
	OutFrameRec2->Control.type = REC_TYPE_OUTFRAME;
	OutFrameRec2->Control.length = sizeof(StkFrameRec2) - sizeof(StkControl);
	OutFrameRec2->Control.version = _VER_OUTFRAME_RECORD /*2*/;

	OutFrameRec2->FrameType = m_eFrameType;
	OutFrameRec2->defType = m_eDefType; //定义类型，例如定义于PMI、定义Text
	OutFrameRec2->NumCurve = m_vcCurves.size();

	ret = i_stkFileP->WriteSegmentRecordData(i_head, (StkControl *)OutFrameRec2);
	if(ret < STK_SUCCESS)
	{
#ifdef _WINDOWS_
		if (1 == stkSVLLog.bLogOutput && 1 == stkSVLLog.bPMIAndView)
		{
			_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：OutFrame记录写入失败！"));
			stkSVLLogFile->Log(strConvertLog);
		}
#endif
		return STK_ERROR;
	}

	if(OutFrameRec2 != NULL){
		free(OutFrameRec2);
		OutFrameRec2 = NULL;
	}

	return STK_SUCCESS;
}

/****************************************************************************
 *	@brief		获取外框线的数量
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		外框线的数量
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_UINT32 Stk_OutFrame::GetCurvesNum()
{
	return m_vcCurves.size();
}


/****************************************************************************
 *	@brief		根据索引获取外框线的数量
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		线指针
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_CurveEntity* Stk_OutFrame::GetCurveByIndex(STK_UINT32 i_nuIndex)
{
	Stk_CurveEntity* stkCurveP = NULL;

	if(i_nuIndex >= 0 && i_nuIndex < m_vcCurves.size())
	{
		stkCurveP = m_vcCurves[i_nuIndex];
	}

	return stkCurveP;
}