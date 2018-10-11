// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_Feat_Hole.cpp
 *
 *	@brief	孔特征类的实现
 *
 *	@par	历史:
 *		2014/11/19	创建。WangY@HOTEAMSOFT
****************************************************************************/

#ifndef __MOBILE__
#pragma once
#endif

#include "StdAfx.h"
#include "Stk_Feat_Hole.h"
#include "Stk_DocumentManager.h"
#ifdef _WINDOWS_
#include "Stk_LogFile.h"
#endif

/************************************************************************//**
 *	@brief		构造函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_Feat_Axis::Stk_Feat_Axis()
{
	m_dirAxis.X = 0;
	m_dirAxis.Y = 0;
	m_dirAxis.Z = 0;
	memset(&m_pntOrigin, 0, sizeof(STK_PNT32));
}

/************************************************************************//**
 *	@brief		析构函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_Feat_Axis::~Stk_Feat_Axis()
{

}

/************************************************************************//**
 *	@brief		设置轴方向
 *	@param[in]	i_dirAxis		轴方向向量
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Feat_Axis::SetDirection(STK_DIR32 i_dirAxis)
{
	m_dirAxis = i_dirAxis;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取轴方向
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		轴方向向量
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_DIR32 Stk_Feat_Axis::GetDirection()
{
	return m_dirAxis;
}

/************************************************************************//**
 *	@brief		设置原点
 *	@param[in]	i_pntOrigin		原点
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Feat_Axis::SetOrigin(STK_PNT32 i_pntOrigin)
{
	m_pntOrigin = i_pntOrigin;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取原点
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		原点
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_PNT32 Stk_Feat_Axis::GetOrigin()
{
	return m_pntOrigin;
}

/************************************************************************//**
 *	@brief		填写轴特征
 *	@param[in]	i_stkFileP			文件指针
 *	@param[in]	i_AxisHeadP			轴记录头
 *	@retval		STK_SUCCESS			写入成功
 *	@retval		其他				写入失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_Feat_Axis::WriteFeatAxis(Stk_File *i_stkFileP, StkSegmentHead* i_AxisHeadP)
{
	STK_STATUS			eState = STK_SUCCESS;
	StkFeatAxisRec		FeatAxisRec;
#ifdef _WINDOWS_
	WCHAR				strConvertLog[MAX_PATH + 512];
	Stk_SVLLog			stkSVLLog;
	Stk_LogFileEx*		stkSVLLogFile;
#endif

	if (i_stkFileP == NULL)
	{
		return STK_ERROR;
	}
#ifdef _WINDOWS_
	stkSVLLog = i_stkFileP->GetDocumentManager()->GetSVLLogIni();
	stkSVLLogFile = i_stkFileP->GetDocumentManager()->GetSVLLogFile();
#endif

	FeatAxisRec.Control.type = REC_TYPE_FEATAXIS;
	FeatAxisRec.Control.length = sizeof(StkFeatAxisRec) - sizeof(StkControl);
	FeatAxisRec.Control.version = _VER_FEATAXIS_RECORD;

	FeatAxisRec.dirAxis = m_dirAxis;
	FeatAxisRec.pntOrigin = m_pntOrigin;

	eState = i_stkFileP->WriteSegmentRecordData(i_AxisHeadP, (StkControl *)&FeatAxisRec);
	if(eState < STK_SUCCESS)
	{
#ifdef _WINDOWS_
		if (1 == stkSVLLog.bLogOutput && 1 == stkSVLLog.bBody)
		{
			_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：轴特征记录写入失败！"));
			stkSVLLogFile->Log(strConvertLog);
		}
#endif
		return STK_ERROR;
	}

	return eState;
}

/************************************************************************//**
 *	@brief		构造函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_FeatLimit::Stk_FeatLimit()
{
	m_eLimitType = TYPE_LIMIT_DEFAULT;
	m_dValue = 0;
	m_stkNodeConnectorP = NULL;
}

/************************************************************************//**
 *	@brief		构造函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_FeatLimit::~Stk_FeatLimit()
{
	if (m_stkNodeConnectorP != NULL)
	{
		delete m_stkNodeConnectorP;
		m_stkNodeConnectorP = NULL;
	}
}

/************************************************************************//**
 *	@brief		设置Limit类型
 *	@param[in]	i_eLimitType		Limit类型
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_FeatLimit::SetType(StkLimitTypeEnum i_eLimitType)
{
	m_eLimitType = i_eLimitType;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取Limit类型
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		Limit类型
 *	@note		无
 *	@attention	无
 ****************************************************************************/
StkLimitTypeEnum Stk_FeatLimit::GetType()
{
	return m_eLimitType;
}

/************************************************************************//**
 *	@brief		设置Limit值
 *	@param[in]	i_dValue			Limit值
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_FeatLimit::SetValue(STK_DOUBLE64 i_dValue)
{
	m_dValue = i_dValue;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取Limit值
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		Limit值
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_DOUBLE64 Stk_FeatLimit::GetValue()
{
	return m_dValue;
}

/************************************************************************//**
 *	@brief		设置关联对象
 *	@param[in]	i_NodeConnectorP	关联
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_FeatLimit::SetReferenceObject(Stk_NodeConnector* i_NodeConnectorP)
{
	m_stkNodeConnectorP = i_NodeConnectorP;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取关联对象
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		关联
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_NodeConnector* Stk_FeatLimit::GetReferenceObject()
{
	return m_stkNodeConnectorP;
}

/************************************************************************//**
 *	@brief		填写螺纹特征
 *	@param[in]	i_stkFileP			文件指针
 *	@retval		STK_SUCCESS			写入成功
 *	@retval		其他				写入失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_FeatLimit::WriteFile(Stk_File *i_stkFileP)
{
	STK_STATUS			eState = STK_SUCCESS;
	StkSegmentHead		head = {0};
	StkSegmentHead		endHead = {0};
	StkFeatLimitRec		FeatLimitRec = {0};
#ifdef _WINDOWS_
	WCHAR				strConvertLog[MAX_PATH + 512];
	Stk_SVLLog			stkSVLLog;
	Stk_LogFileEx*		stkSVLLogFile;
#endif

	if (i_stkFileP == NULL)
	{
		return STK_ERROR;
	}
#ifdef _WINDOWS_
	stkSVLLog = i_stkFileP->GetDocumentManager()->GetSVLLogIni();
	stkSVLLogFile = i_stkFileP->GetDocumentManager()->GetSVLLogFile();
#endif

	/*
	 *	【第一步】输出Limit段
	 */
	// 【1.1】输出Limit段头记录
	head.id = 0;
	head.kind = SEG_TYPE_FEATLIMIT;

	eState = i_stkFileP->WriteSegmentHeadData(&head);
	if (eState < STK_SUCCESS)
	{
		return eState;
	}
	
	// 【1.2】输出Limit记录
	FeatLimitRec.nuType = m_eLimitType;
	FeatLimitRec.dValue = m_dValue;

	eState = i_stkFileP->WriteSegmentRecordData(&head, (StkControl*)&FeatLimitRec);
	if (eState < STK_SUCCESS)
	{
#ifdef _WINDOWS_
		if (1 == stkSVLLog.bLogOutput && 1 == stkSVLLog.bBody)
		{
			_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：Limit特征记录写入失败！"));
			stkSVLLogFile->Log(strConvertLog);
		}
#endif
		return eState;
	}

	// 【1.3】完成Limit段输出
	eState = i_stkFileP->WriteSegmentFinishData(&head);
	if (eState < STK_SUCCESS)
	{
		return eState;
	}

	/*
	 *	【第二步】输出NodeConnector段
	 */
	if (m_stkNodeConnectorP != NULL)
	{
		m_stkNodeConnectorP->WriteFile(i_stkFileP);
	}

	/*
	 *	【第三步】输出Limit-END段
	 */
	endHead.id = 0;
	endHead.kind = SEG_TYPE_FEATLIMIT_END;
	eState = i_stkFileP->WriteSegmentFinishData(&endHead);
	if (eState < STK_SUCCESS)
	{
#ifdef _WINDOWS_
		if (1 == stkSVLLog.bLogOutput && 1 == stkSVLLog.bBody)
		{
			_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：Limit特征-END段写入失败！"));
			stkSVLLogFile->Log(strConvertLog);
		}
#endif
		return eState;
	}

	return eState;
}

/************************************************************************//**
 *	@brief		读取螺纹特征中除NodeConnector的数据
 *	@param[in]	i_stkFileP			文件指针
 *	@param[in]	i_LimitHeadP		Limit段头
 *	@retval		STK_SUCCESS			写入成功
 *	@retval		其他				写入失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_FeatLimit::ReadFile(Stk_File* i_stkFileP, StkSegmentHead* i_LimitHeadP)
{
	STK_STATUS			eState = STK_SUCCESS;
	StkSegmentHead		head = {0};
	StkControl**		RecordPP = NULL;
	StkFeatLimitRec*	FeatLimitRecP = NULL;
	Stk_NodeConnector*	stkNodeConnectorP = NULL;

	/*
	 *	读取Limit特征段
	 */
	RecordPP = (StkControl**)calloc(i_LimitHeadP->recnum, sizeof(StkControl*));
	if (RecordPP == NULL)
	{
		return STK_ERROR_MEMORY;
	}
	eState = i_stkFileP->ReadSegmentAllRecordData(i_LimitHeadP, (StkControl**)RecordPP);
	if (eState != STK_SUCCESS)
	{
		return eState;
	}

	/*
	 *	读取Limit段的各记录
	 */
	for (int ii = 0; ii < i_LimitHeadP->recnum; ii++)
	{
		switch(RecordPP[ii]->type)
		{
		case REC_TYPE_FEATLIMIT:
			if (RecordPP[ii]->version == 1)
			{
				FeatLimitRecP = (StkFeatLimitRec*)RecordPP[ii];
				m_eLimitType = (StkLimitTypeEnum)FeatLimitRecP->nuType;
				m_dValue = FeatLimitRecP->dValue;
			}
			break;
		default:
			break;
		}
	}

	// 释放内存
	i_stkFileP->ReadSegmentFinishData(i_LimitHeadP, (StkControl **)RecordPP);
	if (eState != STK_SUCCESS)
	{
		return STK_ERROR_HEADRD;
	}
	if (RecordPP != NULL)
	{
		free(RecordPP);
		RecordPP = NULL;
	}

	/*
	 *	读取下一段
	 */
	while(1)
	{
		eState = i_stkFileP->ReadSegmentHeadData(&head);
		if (eState != STK_SUCCESS)
		{
			return eState;
		}
		switch(head.kind)
		{
		case SEG_TYPE_CONNECTOR:
			stkNodeConnectorP = new Stk_NodeConnector(NULL);
			eState = stkNodeConnectorP->ReadFile(i_stkFileP, &head);
			if (eState != STK_SUCCESS)
			{
				delete stkNodeConnectorP;
				return eState;
			}
			m_stkNodeConnectorP = stkNodeConnectorP;
			break;
		case SEG_TYPE_FEATLIMIT_END:
			goto rtn;
			break;
		default:
			StkControl **TmpRecordPP = NULL;
			TmpRecordPP = (StkControl **)calloc(head.recnum, sizeof(StkControl*));
			if (head.recnum !=0 && TmpRecordPP == NULL)
			{
				return STK_ERROR_MEMORY;
			}
			eState = i_stkFileP->ReadSegmentAllRecordData(&head, (StkControl **)TmpRecordPP);
			if (eState!=STK_SUCCESS)
			{
				return STK_ERROR_HEADRD;
			}
			eState = i_stkFileP->ReadSegmentFinishData(&head, (StkControl **)TmpRecordPP);// 释放内存
			if (eState!=STK_SUCCESS)
			{
				return STK_ERROR_HEADRD;
			}
			if (TmpRecordPP != NULL)
			{
				free(TmpRecordPP);
				TmpRecordPP = NULL;
			}
			break;
		}
	}
rtn:
	return eState;
}

/************************************************************************//**
 *	@brief		构造函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_FeatInnerThread::Stk_FeatInnerThread()
{
	m_eThreadLimitType = TYPE_THREAD_DIMENSION;
	m_eThreadType = TYPE_THREAD_UNKNOWN;
	m_dDepth = 0;
	m_bHasDepthTol = false;
	m_dDepthTol = 0;
	m_dDiameter = 0;
	m_bHasDiameterTol = false;
	m_dDiameterTol = 0;
	m_dPitch = 0;
	m_bHasPitchTol = false;
	m_dPitchTol = 0;
	m_bIsRightThread = true;
	m_NodeConnectorP = NULL;
}

/************************************************************************//**
 *	@brief		析构函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_FeatInnerThread::~Stk_FeatInnerThread()
{
	if (m_NodeConnectorP != NULL)
	{
		delete m_NodeConnectorP;
		m_NodeConnectorP = NULL;
	}
}

/************************************************************************//**
 *	@brief		设置螺纹底部类型
 *	@param[in]	i_eThreadLimitType	螺纹底部类型
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_FeatInnerThread::SetThreadLimitType(StkThreadLimitTypeEnum i_eThreadLimitType)
{
	m_eThreadLimitType = i_eThreadLimitType;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取螺纹底部类型
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		螺纹底部类型
 *	@note		无
 *	@attention	无
 ****************************************************************************/
StkThreadLimitTypeEnum Stk_FeatInnerThread::GetThreadLimitType()
{
	return m_eThreadLimitType;
}

/************************************************************************//**
 *	@brief		设置螺纹类型
 *	@param[in]	i_eThreadType	螺纹类型
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_FeatInnerThread::SetThreadType(StkThreadTypeEnum i_eThreadType)
{
	m_eThreadType = i_eThreadType;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取螺纹类型
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		螺纹类型
 *	@note		无
 *	@attention	无
 ****************************************************************************/
StkThreadTypeEnum Stk_FeatInnerThread::GetThreadType()
{
	return m_eThreadType;
}

/************************************************************************//**
 *	@brief		设置螺纹深度
 *	@param[in]	i_dDepth		螺纹深度
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_FeatInnerThread::SetDepth(STK_DOUBLE64 i_dDepth)
{
	m_dDepth = i_dDepth;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取螺纹深度
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		螺纹深度
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_DOUBLE64 Stk_FeatInnerThread::GetDepth()
{
	return m_dDepth;
}

/************************************************************************//**
 *	@brief		设置是否有螺纹深度公差
 *	@param[in]	i_bHasTol		公差标识
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_FeatInnerThread::SetDepthHasTol(STK_BOOL i_bHasTol)
{
	m_bHasDepthTol = i_bHasTol;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		是否有螺纹深度公差
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		公差标识
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_BOOL Stk_FeatInnerThread::IsDepthHasTol()
{
	return m_bHasDepthTol;
}

/************************************************************************//**
 *	@brief		设置螺纹深度公差
 *	@param[in]	i_dDepthTol		深度公差
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_FeatInnerThread::SetDepthTol(STK_DOUBLE64 i_dDepthTol)
{
	m_dDepthTol = i_dDepthTol;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取螺纹深度公差
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		深度公差
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_DOUBLE64 Stk_FeatInnerThread::GetDepthTol()
{
	return m_dDepthTol;
}

/************************************************************************//**
 *	@brief		设置螺纹直径
 *	@param[in]	i_dDiameter		螺纹直径
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_FeatInnerThread::SetDiameter(STK_DOUBLE64 i_dDiameter)
{
	m_dDiameter = i_dDiameter;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取螺纹直径
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		螺纹直径
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_DOUBLE64 Stk_FeatInnerThread::GetDiameter()
{
	return m_dDiameter;
}

/************************************************************************//**
 *	@brief		设置是否有螺纹直径公差
 *	@param[in]	i_bDiameterHasTol	螺纹直径公差标识
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_FeatInnerThread::SetDiameterHasTol(STK_BOOL i_bDiameterHasTol)
{
	m_bHasDiameterTol = i_bDiameterHasTol;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		是否有螺纹直径公差
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		螺纹直径公差标识
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_BOOL Stk_FeatInnerThread::IsHasDiameterTol()
{
	return m_bHasDiameterTol;
}

/************************************************************************//**
 *	@brief		设置螺距
 *	@param[in]	i_dPitch		螺距
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_FeatInnerThread::SetPitch(STK_DOUBLE64 i_dPitch)
{
	m_dPitch = i_dPitch;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取螺距
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		螺距
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_DOUBLE64 Stk_FeatInnerThread::GetPitch()
{
	return m_dPitch;
}

/************************************************************************//**
 *	@brief		设置是否有螺距公差标识
 *	@param[in]	i_bPitchHasTol	螺距公差标识
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_FeatInnerThread::SetPitchHasTol(STK_BOOL i_bPitchHasTol)
{
	m_bHasPitchTol = i_bPitchHasTol;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		是否有螺距公差标识
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		螺距公差标识
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_BOOL Stk_FeatInnerThread::IsHasPitchTol()
{
	return m_bHasPitchTol;
}

/************************************************************************//**
 *	@brief		设置螺距公差
 *	@param[in]	i_dPitchHasTol	螺距公差
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_FeatInnerThread::SetPitchTol(STK_DOUBLE64 i_dPitchHasTol)
{
	m_dPitchTol = i_dPitchHasTol;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取螺距公差
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		螺距公差
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_DOUBLE64 Stk_FeatInnerThread::GetPitchTol()
{
	return m_dPitchTol;
}

/************************************************************************//**
 *	@brief		设置是否是右旋螺纹
 *	@param[in]	i_bRightThread	右旋螺纹标识
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_FeatInnerThread::SetRightThreaded(STK_BOOL i_bRightThread)
{
	m_bIsRightThread = i_bRightThread;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		是否是右旋螺纹
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		右旋螺纹标识
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_BOOL Stk_FeatInnerThread::IsRightThreaded()
{
	return m_bIsRightThread;
}

/************************************************************************//**
 *	@brief		设置螺纹参考面
 *	@param[in]	i_NodeConnectorP	关联
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		只有当m_eThreadLimitType类型为TYPE_THREAD_UP_TO_PLANE时，才会有该属性
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_FeatInnerThread::SetThreadLimitReference(Stk_NodeConnector* i_NodeConnectorP)
{
	if (m_eThreadLimitType == TYPE_THREAD_UP_TO_PLANE)
	{
		m_NodeConnectorP = i_NodeConnectorP;
	}
	else
	{
		m_NodeConnectorP = NULL;
	}
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取螺纹参考面
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		关联
 *	@note		只有当m_eThreadLimitType类型为TYPE_THREAD_UP_TO_PLANE时，才会有该属性
 *	@attention	无
 ****************************************************************************/
Stk_NodeConnector* Stk_FeatInnerThread::GetThreadLimitReference()
{
	return m_NodeConnectorP;
}

/************************************************************************//**
 *	@brief		填写螺纹特征
 *	@param[in]	i_stkFileP			文件指针
 *	@retval		STK_SUCCESS			写入成功
 *	@retval		其他				写入失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_FeatInnerThread::WriteFile(Stk_File *i_stkFileP)
{
	STK_STATUS				eState = STK_SUCCESS;
	StkSegmentHead			head = {0};
	StkSegmentHead			endHead = {0};
	StkFeatInnerThreadRec	FeatInnerThreadRec = {0};
#ifdef _WINDOWS_
	WCHAR				strConvertLog[MAX_PATH + 512];
	Stk_SVLLog			stkSVLLog;
	Stk_LogFileEx*		stkSVLLogFile;
#endif

	if (i_stkFileP == NULL)
	{
		return STK_ERROR;
	}
#ifdef _WINDOWS_
	stkSVLLog = i_stkFileP->GetDocumentManager()->GetSVLLogIni();
	stkSVLLogFile = i_stkFileP->GetDocumentManager()->GetSVLLogFile();
#endif

	/*
	 *	【第一步】输出孔螺纹特征段
	 */
	// 【1.1】输出螺纹特征的段头记录
	head.id = 0;
	head.kind = SEG_TYPE_INNERTHREAD;

	eState = i_stkFileP->WriteSegmentHeadData(&head);
	if (eState < STK_SUCCESS)
	{
		return STK_ERROR;
	}

	// 【1.2】 输出螺纹特征记录
	FeatInnerThreadRec.Control.type = REC_TYPE_FEATINNERTHREAD;
	FeatInnerThreadRec.Control.length = sizeof(StkFeatInnerThreadRec) - sizeof(StkControl);
	FeatInnerThreadRec.Control.version = _VER_FEATINNERTHREAD_RECORD;

	FeatInnerThreadRec.dDepth = m_dDepth;
	FeatInnerThreadRec.dDiameter = m_dDiameter;
	FeatInnerThreadRec.dPitch = m_dPitch;
	
	if (m_bHasDepthTol)
	{
		FeatInnerThreadRec.IsItem |= STK_HAS_DEPTHTOL;
	}
	if (m_bHasDiameterTol)
	{
		FeatInnerThreadRec.IsItem |= STK_HAS_DIAMETERTOL;
	}
	if (m_bHasPitchTol)
	{
		FeatInnerThreadRec.IsItem |= STK_HAS_PITCHTOL;
	}
	if (m_bIsRightThread)
	{
		FeatInnerThreadRec.IsItem |= STK_IS_RIGHT_THREAD;
	}

	FeatInnerThreadRec.dDepthTol = m_dDepthTol;
	FeatInnerThreadRec.dDiameterTol = m_dDiameterTol;
	FeatInnerThreadRec.dPitchTol = m_dPitchTol;
	
	eState = i_stkFileP->WriteSegmentRecordData(&head, (StkControl *)&FeatInnerThreadRec);
	if(eState < STK_SUCCESS)
	{
#ifdef _WINDOWS_
		if (1 == stkSVLLog.bLogOutput && 1 == stkSVLLog.bBody)
		{
			_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：螺纹特征记录写入失败！"));
			stkSVLLogFile->Log(strConvertLog);
		}
#endif
		return STK_ERROR;
	}

	// 【1.3】 完成孔螺纹特征段的输出
	eState = i_stkFileP->WriteSegmentFinishData(&head);
	if(eState < STK_SUCCESS)
	{
		return STK_ERROR;
	}

	/*
	 *	【第二步】输出要素关联记录段
	 */
	if (m_NodeConnectorP != NULL)
	{
		m_NodeConnectorP->WriteFile(i_stkFileP);
	}

	/*
	 *	【第三步】输出螺纹特征-END段
	 */
	endHead.id = 0;
	endHead.kind = SEG_TYPE_INNERTHREAD_END;
	eState = i_stkFileP->WriteSegmentHeadData(&endHead);
	if(eState < STK_SUCCESS)
	{
#ifdef _WINDOWS_
		if (1 == stkSVLLog.bLogOutput && 1 == stkSVLLog.bBody)
		{
			_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：孔特征-END段写入失败！"));
			stkSVLLogFile->Log(strConvertLog);
		}
#endif
		return STK_ERROR;
	}

	return eState;
}

/************************************************************************//**
 *	@brief		读取螺纹特征中除NodeConnector的数据
 *	@param[in]	i_stkFileP			文件指针
 *	@param[in]	i_InnerThreadHeadP	螺纹段头
 *	@retval		STK_SUCCESS			写入成功
 *	@retval		其他				写入失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_FeatInnerThread::ReadFile(Stk_File* i_stkFileP, StkSegmentHead* i_InnerThreadHeadP)
{
	STK_STATUS				eState = STK_SUCCESS;
	StkSegmentHead			head={0};
	StkControl**			RecordPP = NULL;
	StkFeatInnerThreadRec*	FeatInnerThreadRecP = NULL;
	Stk_NodeConnector*		stkNodeConnectorP = NULL;
	
	/*
	 *	读取孔螺纹特征段
	 */
	RecordPP = (StkControl **)calloc(i_InnerThreadHeadP->recnum, sizeof(StkControl*));
	if(i_InnerThreadHeadP->recnum !=0 && RecordPP == NULL)
	{
		return STK_ERROR_MEMORY;
	}
	eState = i_stkFileP->ReadSegmentAllRecordData(i_InnerThreadHeadP, (StkControl **)RecordPP);
	if (eState != STK_SUCCESS)
	{
		return eState;
	}

	/*
	 * 读取孔螺纹特征段的各记录
	 */
	for (int ii = 0; ii < i_InnerThreadHeadP->recnum; ii++)
	{
		switch(RecordPP[ii]->type)
		{
		case REC_TYPE_FEATINNERTHREAD:
			if (RecordPP[ii]->version == 1)
			{
				FeatInnerThreadRecP = (StkFeatInnerThreadRec*)RecordPP[ii];
				m_eThreadLimitType = (StkThreadLimitTypeEnum)FeatInnerThreadRecP->nuThreadLimitType;
				m_eThreadType = (StkThreadTypeEnum)FeatInnerThreadRecP->nuThreadType;
				m_dDepth = FeatInnerThreadRecP->dDepth;
				m_dDiameter = FeatInnerThreadRecP->dDiameter;
				m_dPitch = FeatInnerThreadRecP->dPitch;
				if (FeatInnerThreadRecP->IsItem & STK_HAS_DEPTHTOL)
				{
					m_bHasDepthTol = true;
					m_dDepthTol = FeatInnerThreadRecP->dDepthTol;
				}
				if (FeatInnerThreadRecP->IsItem & STK_HAS_DIAMETERTOL)
				{
					m_bHasDiameterTol = true;
					m_dDiameterTol = FeatInnerThreadRecP->dDiameterTol;
				}
				if (FeatInnerThreadRecP->IsItem & STK_HAS_PITCHTOL)
				{
					m_bHasPitchTol = true;
					m_dPitchTol = FeatInnerThreadRecP->dPitchTol;
				}
			}
			break;
		default:
			break;
		}
	}

	// 释放内存
	i_stkFileP->ReadSegmentFinishData(i_InnerThreadHeadP, (StkControl **)RecordPP);
	if (eState != STK_SUCCESS)
	{
		return STK_ERROR_HEADRD;
	}
	if (RecordPP != NULL)
	{
		free(RecordPP);
		RecordPP = NULL;
	}

	/*
	 *	读取下一段的数据
	 */
	while(1)
	{
		eState = i_stkFileP->ReadSegmentHeadData(&head);
		if (eState!=STK_SUCCESS)
		{
			return STK_ERROR_HEADRD;
		}
		switch(head.kind)
		{
		case SEG_TYPE_CONNECTOR:
			stkNodeConnectorP = new Stk_NodeConnector(NULL);
			eState = stkNodeConnectorP->ReadFile(i_stkFileP, &head);
			if (eState != STK_SUCCESS)
			{
				delete stkNodeConnectorP;
				return eState;
			}

			m_NodeConnectorP = stkNodeConnectorP;
			break;
		case SEG_TYPE_INNERTHREAD_END:
			goto rtn;
			break;
		default:
			StkControl **TmpRecordPP = NULL;
			TmpRecordPP = (StkControl **)calloc(head.recnum, sizeof(StkControl*));
			if (head.recnum !=0 && TmpRecordPP == NULL)
			{
				return STK_ERROR_MEMORY;
			}
			eState = i_stkFileP->ReadSegmentAllRecordData(&head, (StkControl **)TmpRecordPP);
			if (eState!=STK_SUCCESS)
			{
				return STK_ERROR_HEADRD;
			}
			eState = i_stkFileP->ReadSegmentFinishData(&head, (StkControl **)TmpRecordPP);// 释放内存
			if (eState!=STK_SUCCESS)
			{
				return STK_ERROR_HEADRD;
			}
			if (TmpRecordPP != NULL)
			{
				free(TmpRecordPP);
				TmpRecordPP = NULL;
			}
			break;
		}
	}
rtn:
	return eState;
}

/************************************************************************//**
 *	@brief		构造函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_Feat_Hole::Stk_Feat_Hole()
{
	m_eHoleAnchorType = TYPE_NO_ANCHOR;
	m_eHoleType = TYPE_HOLE_SIMPLE;
	m_eHoleBottomType = TYPE_BOTTOM_FLAT;
	m_FeatAxisP = NULL;
	m_FeatInnerThreadP = NULL;
	m_dDepth = 0;
	m_bHasDepthTol = false;
	m_dDepthTol = 0;
	m_dDiameter = 0;
	m_bHasDiameterTol = false;
	m_dDiamterTol = 0;
	m_dBottomAngle = 0;
	m_bHasBottomAngleTol = false;
	m_dBottomAngleTol = 0;
	m_dCounterAngle = 0;
	m_bHasCounterAngleTol = false;
	m_dCounterAngleTol = 0;
	m_dCounterDepth = 0;
	m_bHasCounterDepthTol = false;
	m_dCounterDepthTol = 0;
	m_dCounterDiamter = 0;
	m_bHasCounterDiamterTol = false;
	m_dCounterDiamterTol = 0;
}

/************************************************************************//**
 *	@brief		析构函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_Feat_Hole::~Stk_Feat_Hole()
{
	if (m_FeatAxisP != NULL)
	{
		delete m_FeatAxisP;
		m_FeatAxisP = NULL;
	}
	if (m_FeatInnerThreadP != NULL)
	{
		delete m_FeatInnerThreadP;
		m_FeatInnerThreadP = NULL;
	}
}

/************************************************************************//**
 *	@brief		设置孔的固定类型
 *	@param[in]	i_eAnchorType		固定类型
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Feat_Hole::SetAnchorType(StkHoleAnchorTypeEnum i_eAnchorType)
{
	m_eHoleAnchorType = i_eAnchorType;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取孔的固定类型
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		固定类型
 *	@note		无
 *	@attention	无
 ****************************************************************************/
StkHoleAnchorTypeEnum Stk_Feat_Hole::GetAnchorType()
{
	return m_eHoleAnchorType;
}

/************************************************************************//**
 *	@brief		设置孔的类型
 *	@param[in]	i_eType				孔的类型
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Feat_Hole::SetType(StkHoleTypeEnum i_eType)
{
	m_eHoleType = i_eType;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取孔的类型
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		孔的类型
 *	@note		无
 *	@attention	无
 ****************************************************************************/
StkHoleTypeEnum Stk_Feat_Hole::GetType()
{
	return m_eHoleType;
}

/************************************************************************//**
 *	@brief		设置孔的底部类型
 *	@param[in]	i_eBottomType		底部类型
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Feat_Hole::SetBottomType(StkHoleBottomTypeEnum i_eBottomType)
{
	m_eHoleBottomType = i_eBottomType;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取孔的底部类型
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		孔的底部类型
 *	@note		无
 *	@attention	无
 ****************************************************************************/
StkHoleBottomTypeEnum Stk_Feat_Hole::GetBottomType()
{
	return m_eHoleBottomType;
}

/************************************************************************//**
 *	@brief		设置轴
 *	@param[in]	i_AxisP				轴
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Feat_Hole::SetFeatAxis(Stk_Feat_Axis* i_AxisP)
{
	m_FeatAxisP = i_AxisP;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取轴
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		轴
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_Feat_Axis* Stk_Feat_Hole::GetFeatAxis()
{
	return m_FeatAxisP;
}

/************************************************************************//**
 *	@brief		设置螺纹
 *	@param[in]	i_InnerThreadP		螺纹
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Feat_Hole::SetInnerThread(Stk_FeatInnerThread* i_InnerThreadP)
{
	m_FeatInnerThreadP = i_InnerThreadP;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取螺纹
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		螺纹
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_FeatInnerThread* Stk_Feat_Hole::GetInnerThread()
{
	return m_FeatInnerThreadP;
}

/************************************************************************//**
 *	@brief		设置Limit特征
 *	@param[in]	i_FeatLimitP		Limit特征
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Feat_Hole::SetFeatLimit(Stk_FeatLimit* i_FeatLimitP)
{
	m_FeatLimitP = i_FeatLimitP;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取Limit特征
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		Limit特征
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_FeatLimit* Stk_Feat_Hole::GetFeatLimit()
{
	return m_FeatLimitP;
}

/************************************************************************//**
 *	@brief		设置孔深度
 *	@param[in]	i_dDepth			孔深度
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Feat_Hole::SetDepth(STK_DOUBLE64 i_dDepth)
{
	m_dDepth = i_dDepth;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取孔深度
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		孔深度
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_DOUBLE64 Stk_Feat_Hole::GetDepth()
{
	return m_dDepth;
}

/************************************************************************//**
 *	@brief		设置是否有孔深度公差
 *	@param[in]	i_bDepthTol			孔深度公差标识
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Feat_Hole::SetDepthHasTol(STK_BOOL i_bDepthTol)
{
	m_bHasDepthTol = i_bDepthTol;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		是否有孔深度公差
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		孔深度公差标识
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_BOOL Stk_Feat_Hole::IsHasDepthTol()
{
	return m_bHasDepthTol;
}

/************************************************************************//**
 *	@brief		设置孔深度公差
 *	@param[in]	i_dDepthTol			孔深度公差
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Feat_Hole::SetDepthWithTol(STK_DOUBLE64 i_dDepthTol)
{
	m_dDepthTol = i_dDepthTol;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取孔深度公差
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		孔深度公差
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_DOUBLE64 Stk_Feat_Hole::GetDepthWithTol()
{
	return m_dDepthTol;
}

/************************************************************************//**
 *	@brief		设置孔直径
 *	@param[in]	i_dDiameter			孔直径
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Feat_Hole::SetDiameter(STK_DOUBLE64 i_dDiameter)
{
	m_dDiameter = i_dDiameter;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取孔直径
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		孔直径
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_DOUBLE64 Stk_Feat_Hole::GetDiameter()
{
	return m_dDiameter;
}

/************************************************************************//**
 *	@brief		设置孔直径公差标识
 *	@param[in]	i_bDiameterTol		孔直径公差标识
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Feat_Hole::SetDiameterHasTol(STK_BOOL i_bDiameterTol)
{
	m_bHasDiameterTol = i_bDiameterTol;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取孔直径公差标识
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		孔直径公差标识
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_BOOL Stk_Feat_Hole::IsHasDiameterTol()
{
	return m_bHasDiameterTol;
}

/************************************************************************//**
 *	@brief		设置孔直径公差
 *	@param[in]	i_dDiameterTol		孔直径公差
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Feat_Hole::SetDiameterWithTol(STK_DOUBLE64 i_dDiameterTol)
{
	m_dDiamterTol = i_dDiameterTol;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取孔直径公差
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		孔直径公差
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_DOUBLE64 Stk_Feat_Hole::GetDiameterWithTol()
{
	return m_dDiamterTol;
}

/************************************************************************//**
 *	@brief		设置孔底部角度
 *	@param[in]	i_dBottomAngle		孔底部角度
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Feat_Hole::SetBottomAngle(STK_DOUBLE64 i_dBottomAngle)
{
	m_dBottomAngle = i_dBottomAngle;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取孔底部角度
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		孔底部角度
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_DOUBLE64 Stk_Feat_Hole::GetBottomAngle()
{
	return m_dBottomAngle;
}

/************************************************************************//**
 *	@brief		设置孔底部角度公差标识
 *	@param[in]	i_bBottomAngleTol	孔底部角度公差标识
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Feat_Hole::SetBottomAngleHasTol(STK_BOOL i_bBottomAngleTol)
{
	m_bHasBottomAngleTol = i_bBottomAngleTol;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取孔底部角度公差标识
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		孔底部角度标识公差
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_BOOL Stk_Feat_Hole::IsHasBottomAngleTol()
{
	return m_bHasBottomAngleTol;
}

/************************************************************************//**
 *	@brief		设置孔底部角度公差
 *	@param[in]	i_dBottomAngleTol	孔底部角度公差
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Feat_Hole::SetBottomAngleTol(STK_DOUBLE64 i_dBottomAngleTol)
{
	m_dBottomAngleTol = i_dBottomAngleTol;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取孔底部角度公差
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		孔底部角度公差
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_DOUBLE64 Stk_Feat_Hole::GetBottomAngleTol()
{
	return m_dBottomAngleTol;
}

/************************************************************************//**
 *	@brief		设置孔的反向角度
 *	@param[in]	i_dCounterAngle		孔的反向角度
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Feat_Hole::SetCounterAngle(STK_DOUBLE64 i_dCounterAngle)
{
	m_dCounterAngle = i_dCounterAngle;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取孔的反向角度
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		孔反向角度
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_DOUBLE64 Stk_Feat_Hole::GetCounterAngle()
{
	return m_dCounterAngle;
}

/************************************************************************//**
 *	@brief		设置孔的反向角度公差标识
 *	@param[in]	i_bCounterAngleTol	孔的反向角度公差标识
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Feat_Hole::SetCounterAngleHasTol(STK_BOOL i_bCounterAngleTol)
{
	m_bHasCounterAngleTol = i_bCounterAngleTol;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		孔的反向角度公差标识
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		孔反向角度公差标识
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_BOOL Stk_Feat_Hole::IsHasCounterAngleTol()
{
	return m_bHasCounterAngleTol;
}

/************************************************************************//**
 *	@brief		设置孔的反向角度公差
 *	@param[in]	i_dCounterAngleTol	孔的反向角度公差
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Feat_Hole::SetCounterAngleTol(STK_DOUBLE64 i_dCounterAngleTol)
{
	m_dCounterAngleTol = i_dCounterAngleTol;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		孔的反向角度公差
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		孔反向角度公差
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_DOUBLE64 Stk_Feat_Hole::GetCounterAngleTol()
{
	return m_dCounterAngleTol;
}

/************************************************************************//**
 *	@brief		设置孔的反向深度
 *	@param[in]	i_dCounterDepth		孔的反向角度深度
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Feat_Hole::SetCounterDepth(STK_DOUBLE64 i_dCounterDepth)
{
	m_dCounterDepth = i_dCounterDepth;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		孔的反向深度
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		孔反向深度
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_DOUBLE64 Stk_Feat_Hole::GetCounterDepth()
{
	return m_dCounterDepth;
}

/************************************************************************//**
 *	@brief		设置孔的反向深度公差标识
 *	@param[in]	i_bCounterDepthTol	孔的反向深度公差标识
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Feat_Hole::SetCounterDepthHasTol(STK_BOOL i_bCounterDepthTol)
{
	m_bHasCounterDepthTol = i_bCounterDepthTol;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		孔的反向深度公差标识
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		孔反向深度公差标识
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_BOOL Stk_Feat_Hole::IsHasCounterDepthTol()
{
	return m_bHasCounterDepthTol;
}

/************************************************************************//**
 *	@brief		设置孔的反向深度公差
 *	@param[in]	i_dCounterDepthTol	孔的反向深度公差
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Feat_Hole::SetCounterDepthTol(STK_DOUBLE64 i_dCounterDepthTol)
{
	m_dCounterDepthTol = i_dCounterDepthTol;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		孔的反向深度公差
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		孔反向深度公差
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_DOUBLE64 Stk_Feat_Hole::GetCounterDepthTol()
{
	return m_dCounterDepthTol;
}

/************************************************************************//**
 *	@brief		设置孔的反向直径
 *	@param[in]	i_dCounterDiameter	孔的反向直径
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Feat_Hole::SetCounterDiameter(STK_DOUBLE64 i_dCounterDiameter)
{
	m_dCounterDiamter = i_dCounterDiameter;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		孔的反向直径
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		孔反向直径
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_DOUBLE64 Stk_Feat_Hole::GetCounterDiameter()
{
	return m_dCounterDiamter;
}

/************************************************************************//**
 *	@brief		设置孔的反向直径公差标识
 *	@param[in]	i_bCounterDiameterTol	孔的反向直径公差标识
 *	@param[out]	无
 *	@retval		STK_SUCCESS				设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Feat_Hole::SetCounterDiameterHasTol(STK_BOOL i_bCounterDiameterTol)
{
	m_bHasCounterDiamterTol = i_bCounterDiameterTol;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		孔的反向直径公差标识
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		孔反向直径公差标识
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_BOOL Stk_Feat_Hole::IsHasCounterDiameterTol()
{
	return m_bHasCounterDiamterTol;
}

/************************************************************************//**
 *	@brief		设置孔的反向直径公差
 *	@param[in]	i_dCounterDiameterTol	孔的反向直径公差
 *	@param[out]	无
 *	@retval		STK_SUCCESS				设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Feat_Hole::SetCounterDiameterTol(STK_DOUBLE64 i_dCounterDiameterTol)
{
	m_dCounterDiamterTol = i_dCounterDiameterTol;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取孔的反向直径公差
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		孔反向直径公差
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_DOUBLE64 Stk_Feat_Hole::GetCounterDiameterTol()
{
	return m_dCounterDiamterTol;
}

/************************************************************************//**
 *	@brief		填写孔特征
 *	@param[in]	i_stkFileP			文件指针
 *	@retval		STK_SUCCESS			写入成功
 *	@retval		其他				写入失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_Feat_Hole::WriteFile(Stk_File *i_stkFileP)
{
	STK_STATUS		eState = STK_SUCCESS;
	StkSegmentHead	head = {0};
	StkSegmentHead	endHead = {0};
	StkFeatHoleRec	FeatHoleRec = {0};
#ifdef _WINDOWS_
	WCHAR				strConvertLog[MAX_PATH + 512];
	Stk_SVLLog			stkSVLLog;
	Stk_LogFileEx*		stkSVLLogFile;
#endif

	if (i_stkFileP == NULL)
	{
		return STK_ERROR;
	}
#ifdef _WINDOWS_
	stkSVLLog = i_stkFileP->GetDocumentManager()->GetSVLLogIni();
	stkSVLLogFile = i_stkFileP->GetDocumentManager()->GetSVLLogFile();
#endif

	/*
	 *	【第一步】输出孔特征段
	 */
	// 【1.1】输出孔特征段头记录
	head.id = 0;
	head.kind = SEG_TYPE_FEATHOLE;

	eState = i_stkFileP->WriteSegmentHeadData(&head);
	if (eState < STK_SUCCESS)
	{
		return STK_ERROR;
	}

	// 【1.2】输出孔特征段记录
	FeatHoleRec.Control.type = REC_TYPE_FEATHOLE;
	FeatHoleRec.Control.length = sizeof(StkFeatHoleRec) - sizeof(StkControl);
	FeatHoleRec.Control.version = _VER_FEATHOLE_RECORD;

	FeatHoleRec.nuAnchorType = m_eHoleAnchorType;
	FeatHoleRec.nuType = m_eHoleType;
	FeatHoleRec.nuBottomType = m_eHoleBottomType;
	FeatHoleRec.dDepth = m_dDepth;
	FeatHoleRec.dDiameter = m_dDiameter;
	FeatHoleRec.dBottomAngle = m_dBottomAngle;
	FeatHoleRec.dCounterAngle = m_dCounterAngle;
	FeatHoleRec.dCounterDepth = m_dCounterDepth;
	FeatHoleRec.dCounterDiameter = m_dCounterDiamter;
	if (m_bHasDepthTol)
	{
		FeatHoleRec.IsItem |= STK_HAS_DEPTHTOL;
	}
	if (m_bHasDiameterTol)
	{
		FeatHoleRec.IsItem |= STK_HAS_DIAMETERTOL;
	}
	if (m_bHasBottomAngleTol)
	{
		FeatHoleRec.IsItem |= STK_HAS_BOTTOMANGLETOL;
	}
	if (m_bHasCounterDepthTol)
	{
		FeatHoleRec.IsItem |= STK_HAS_COUNTER_DEPTHTOL;
	}
	if (m_bHasCounterDiamterTol)
	{
		FeatHoleRec.IsItem |= STK_HAS_COUNTER_DIAMETERTOL;
	}
	if (m_bHasCounterAngleTol)
	{
		FeatHoleRec.IsItem |= STK_HAS_COUNTER_ANGLETOL;
	}
	FeatHoleRec.dDepthTol = m_dDepthTol;
	FeatHoleRec.dDiameterTol = m_dDiamterTol;
	FeatHoleRec.dBottomAngleTol = m_dBottomAngleTol;
	FeatHoleRec.dCounterAngleTol = m_dCounterAngleTol;
	FeatHoleRec.dCounterDepthTol = m_dCounterDepthTol;
	FeatHoleRec.dCounterDiameterTol = m_dCounterDiamterTol;

	eState = i_stkFileP->WriteSegmentRecordData(&head, (StkControl *)&FeatHoleRec);
	if(eState < STK_SUCCESS)
	{
#ifdef _WINDOWS_
		if (1 == stkSVLLog.bLogOutput && 1 == stkSVLLog.bBody)
		{
			_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：孔特征记录写入失败！"));
			stkSVLLogFile->Log(strConvertLog);
		}
#endif
		return STK_ERROR;
	}

	// 【1.3】 输出孔的轴特征记录
	if (m_FeatAxisP != NULL)
	{
		m_FeatAxisP->WriteFeatAxis(i_stkFileP, &head);
	}

	// 【1.4】 完成孔特征段的输出
	eState = i_stkFileP->WriteSegmentFinishData(&head);
	if(eState < STK_SUCCESS)
	{
		return STK_ERROR;
	}

	/*
	 *	【第二步】输出孔螺纹特征段
	 */
	if (m_FeatInnerThreadP != NULL)
	{
		m_FeatInnerThreadP->WriteFile(i_stkFileP);
	}

	/*
	 *	【第三步】输出Limit特征段
	 */
	if (m_FeatLimitP != NULL)
	{
		m_FeatLimitP->WriteFile(i_stkFileP);
	}

	/*
	 *	【第四步】 输出孔特征段END记录
	 */
	endHead.id = 0;
	endHead.kind = SEG_TYPE_FEATHOLE_END;
	eState = i_stkFileP->WriteSegmentHeadData(&endHead);
	if(eState < STK_SUCCESS)
	{
#ifdef _WINDOWS_
		if (1 == stkSVLLog.bLogOutput && 1 == stkSVLLog.bBody)
		{
			_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：孔特征-END段写入失败！"));
			stkSVLLogFile->Log(strConvertLog);
		}
#endif
		return STK_ERROR;
	}

	return eState;
}

/************************************************************************//**
 *	@brief		读取孔特征
 *	@param[in]	i_stkFileP			文件指针
 *	@param[in]	i_HoleHeadP			孔段头指针
 *	@retval		STK_SUCCESS			写入成功
 *	@retval		其他				写入失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_Feat_Hole::ReadFile(Stk_File *i_stkFileP, StkSegmentHead* i_HoleHeadP)
{
	STK_STATUS				eState = STK_SUCCESS;
	StkControl**			RecordPP = NULL;
	StkSegmentHead			head ={0};
	StkFeatHoleRec*			FeatHoleRecP = NULL;
	StkFeatAxisRec*			FeatAxisRecP = NULL;

	Stk_Feat_Axis*			stkFeatAxisP = NULL;
	Stk_FeatInnerThread*	stkFeatInnerThreadP = NULL;
	Stk_FeatLimit*			stkFeatLimitP = NULL;

	/*
	 *	读取孔特征段
	 */
	RecordPP = (StkControl **)calloc(i_HoleHeadP->recnum, sizeof(StkControl*));
	if(i_HoleHeadP->recnum !=0 && RecordPP == NULL)
	{
		return STK_ERROR_MEMORY;
	}
	eState = i_stkFileP->ReadSegmentAllRecordData(i_HoleHeadP, (StkControl **)RecordPP);
	if (eState!=STK_SUCCESS)
	{
		return eState;
	}
	/*
	 *	读取孔特征段的各记录
	 */
	for (int ii = 0; ii < i_HoleHeadP->recnum; ii++)
	{
		switch(RecordPP[ii]->type)
		{
		case REC_TYPE_FEATHOLE:
			if (RecordPP[ii]->version == 1)
			{
				FeatHoleRecP = (StkFeatHoleRec*)RecordPP[ii];
				m_eHoleAnchorType = (StkHoleAnchorTypeEnum)FeatHoleRecP->nuAnchorType;
				m_eHoleType = (StkHoleTypeEnum)FeatHoleRecP->nuType;
				m_eHoleBottomType = (StkHoleBottomTypeEnum)FeatHoleRecP->nuBottomType;
				m_dDepth = FeatHoleRecP->dDepth;
				m_dDiameter = FeatHoleRecP->dDiameter;
				m_dBottomAngle = FeatHoleRecP->dBottomAngle;
				m_dCounterAngle = FeatHoleRecP->dCounterAngle;
				m_dCounterDepth = FeatHoleRecP->dCounterDepth;
				m_dCounterDiamter = FeatHoleRecP->dCounterDiameter;
				if (FeatHoleRecP->IsItem & STK_HAS_DEPTHTOL)
				{
					m_bHasDepthTol = true;
					m_dDepthTol = FeatHoleRecP->dDepthTol;
				}
				if (FeatHoleRecP->IsItem & STK_HAS_DIAMETERTOL)
				{
					m_bHasDiameterTol = true;
					m_dDiamterTol = FeatHoleRecP->dDiameterTol;
				}
				if (FeatHoleRecP->IsItem & STK_HAS_BOTTOMANGLETOL)
				{
					m_bHasBottomAngleTol = true;
					m_dBottomAngleTol = FeatHoleRecP->dBottomAngleTol;
				}
				if (FeatHoleRecP->IsItem & STK_HAS_COUNTER_ANGLETOL)
				{
					m_bHasCounterAngleTol = true;
					m_dCounterAngleTol = FeatHoleRecP->dCounterAngleTol;
				}
				if (FeatHoleRecP->IsItem & STK_HAS_COUNTER_DEPTHTOL)
				{

					m_bHasCounterDepthTol = true;
					m_dCounterDepthTol = FeatHoleRecP->dCounterDepthTol;
				}
				if (FeatHoleRecP->IsItem & STK_HAS_COUNTER_DIAMETERTOL)
				{
					m_bHasCounterDiamterTol = true;
					m_dCounterDiamterTol = FeatHoleRecP->dCounterDiameterTol;
				}
			}
			break;
		case REC_TYPE_FEATAXIS:
			if (RecordPP[ii]->version == 1)
			{
				FeatAxisRecP = (StkFeatAxisRec*)RecordPP[ii];
				stkFeatAxisP = new Stk_Feat_Axis();
				stkFeatAxisP->SetDirection(FeatAxisRecP->dirAxis);
				stkFeatAxisP->SetOrigin(FeatAxisRecP->pntOrigin);
				m_FeatAxisP = stkFeatAxisP;
			}
			break;
		default:
			StkControl **TmpRecordPP = NULL;
			TmpRecordPP = (StkControl **)calloc(head.recnum, sizeof(StkControl*));
			if (i_HoleHeadP->recnum !=0 && TmpRecordPP == NULL)
			{
				return STK_ERROR_MEMORY;
			}
			eState = i_stkFileP->ReadSegmentAllRecordData(i_HoleHeadP, (StkControl **)TmpRecordPP);
			if (eState!=STK_SUCCESS)
			{
				return eState;
			}
			eState = i_stkFileP->ReadSegmentFinishData(i_HoleHeadP, (StkControl **)TmpRecordPP);// 释放内存
			if (eState!=STK_SUCCESS)
			{
				return eState;
			}
			if (TmpRecordPP != NULL)
			{
				free(TmpRecordPP);
				TmpRecordPP = NULL;
			}
			break;
		}
	}

	// 释放内存
	eState = i_stkFileP->ReadSegmentFinishData(i_HoleHeadP, (StkControl **)RecordPP);
	if(eState != STK_SUCCESS)
	{
		return STK_ERROR_HEADRD;
	}
	if(RecordPP != NULL)
	{
		free(RecordPP);
		RecordPP = NULL;
	}

	/*
	 *	读取下一个段
	 */
	while(1)
	{
		eState = i_stkFileP->ReadSegmentHeadData(&head);
		if (eState!=STK_SUCCESS)
		{
			return eState;
		}
		switch(head.kind)
		{
		case SEG_TYPE_INNERTHREAD:
			stkFeatInnerThreadP = new Stk_FeatInnerThread();
			eState = stkFeatInnerThreadP->ReadFile(i_stkFileP, &head);
			if (eState != STK_SUCCESS)
			{
				return eState;
			}
			m_FeatInnerThreadP = stkFeatInnerThreadP;
			break;
		case SEG_TYPE_FEATLIMIT:
			stkFeatLimitP = new Stk_FeatLimit();
			eState = stkFeatLimitP->ReadFile(i_stkFileP, &head);
			if (eState != STK_SUCCESS)
			{
				return eState;
			}
			m_FeatLimitP = stkFeatLimitP;
			break;
		case SEG_TYPE_FEATHOLE_END:
			goto rtn;
			break;
		default:
			StkControl **TmpRecordPP = NULL;
			TmpRecordPP = (StkControl **)calloc(head.recnum, sizeof(StkControl*));
			if (head.recnum !=0 && TmpRecordPP == NULL)
			{
				return STK_ERROR_MEMORY;
			}
			eState = i_stkFileP->ReadSegmentAllRecordData(&head, (StkControl **)TmpRecordPP);
			if (eState!=STK_SUCCESS)
			{
				return STK_ERROR_HEADRD;
			}
			eState = i_stkFileP->ReadSegmentFinishData(&head, (StkControl **)TmpRecordPP);// 释放内存
			if (eState!=STK_SUCCESS)
			{
				return STK_ERROR_HEADRD;
			}
			if (TmpRecordPP != NULL)
			{
				free(TmpRecordPP);
				TmpRecordPP = NULL;
			}
			break;
		}
	}
rtn:
	return eState;
}