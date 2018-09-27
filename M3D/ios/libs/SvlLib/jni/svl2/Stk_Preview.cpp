// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_Preview.cpp
 *
 *	@brief	预览图
 *
 *	@par	历史:
 *		2013/10/06	创建。Gaoqq@HOTEAMSOFT
****************************************************************************/

#include "StdAfx.h"
#include "Stk_Preview.h"
#include "Stk_DocumentManager.h"
#ifdef _WINDOWS_
#include "Stk_LogFile.h"
#endif

/************************************************************************//**
 *	@brief		Stk_Preview构造函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_Preview::Stk_Preview(void)
{
	//m_HasPreview = FALSE;
	m_eFiletype = PREVIEW_TYPE_UNKNOWN;
	m_nuSize = 0;
	m_chStreamP = NULL;
} 

/************************************************************************//**
 *	@brief		Stk_Preview析构函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_Preview::~Stk_Preview(void)
{
	if( m_chStreamP!= NULL){
		free(m_chStreamP);
		m_chStreamP = NULL;
	}
}

/************************************************************************//**
 *	@brief		设置预览图类型
 *	@param[in]	i_eType				预览图类型
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

void Stk_Preview::SetType(StkPreviewTypeEnum i_eType)
{
	m_eFiletype = i_eType;
}

/************************************************************************//**
 *	@brief		获取预览图类型
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_eFiletype			预览图类型
 *	@note		无
 *	@attention	无
 ****************************************************************************/

StkPreviewTypeEnum Stk_Preview::GetType(void)
{
	return m_eFiletype;
}

/************************************************************************//**
 *	@brief		设置预览图流数据大小
 *	@param[in]	i_nuSize			流数据大小
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

void Stk_Preview::SetStreamSize(STK_UINT32 i_nuSize)
{
	m_nuSize = i_nuSize;
}

/************************************************************************//**
 *	@brief		获取预览图流数据大小
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_nuSize			流数据大小
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_UINT32 Stk_Preview::GetStreamSize()
{
	return m_nuSize;
}

/************************************************************************//**
 *	@brief		设置预览图流数据
 *	@param[in]	i_chStreamP			预览图流数据
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

void Stk_Preview::SetStream(STK_CHAR* i_chStreamP)
{
	m_chStreamP = i_chStreamP;
}

/************************************************************************//**
 *	@brief		获取预览图流数据
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		m_chStreamP			预览图流数据
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_CHAR* Stk_Preview::GetStream()
{
	return m_chStreamP;
}

/************************************************************************//**
 *	@brief		读取文件中的预览图数据
 *	@param[in]	i_FileP				文件对象指针
 *	@param[in]	i_PrevHeadP			预览图记录头指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS			读取成功
 *	@retval		STK_ERROR_MEMORY	内存申请失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_Preview::ReadFile(Stk_File *i_FileP, StkSegmentHead *i_PrevHeadP)
{
	StkControl			**RecordPP = NULL;
	STK_UINT32			ii;
	StkThumbNailRec		*ThumbRecP = NULL;
	STK_DOUBLE64		dFileVer = 0.0;
	STK_DOUBLE64		dVer = 0.0;
	STK_STATUS			eState;
	
	RecordPP = (StkControl **)calloc(i_PrevHeadP->recnum, sizeof(StkControl*));
	if (i_PrevHeadP->recnum != 0 && RecordPP == NULL)
	{
		return STK_ERROR_MEMORY;
	}
	eState = i_FileP->ReadSegmentAllRecordData(i_PrevHeadP, (StkControl **)RecordPP);
	if (eState!=STK_SUCCESS)
	{
		return eState;
	}
	
	/*
	 *	读取File段的各记录
	 */
	for (ii = 0; ii < i_PrevHeadP->recnum; ii++)
	{
		if (RecordPP[ii]->type == REC_TYPE_THUMBNAIL)
		{
			if (RecordPP[ii]->version == 1)
			{
				/*
				 *	填充File基础记录
				 */
				ThumbRecP = (StkThumbNailRec *)RecordPP[ii];
				m_eFiletype = (StkPreviewTypeEnum)ThumbRecP->filetype;
				m_nuSize = ThumbRecP->buffsize;
				m_chStreamP = (STK_CHAR*)calloc(m_nuSize, sizeof(STK_CHAR)); // 在析构时释放
				if (m_chStreamP == NULL)
				{
					//错误
				}
				memcpy( m_chStreamP,ThumbRecP->buff, sizeof(CHAR)*m_nuSize);
				//m_HasPreview = TRUE;
			}
		}
	}

	// 释放内存
	i_FileP->ReadSegmentFinishData(i_PrevHeadP, (StkControl **)RecordPP);
	if (RecordPP != NULL)
	{
		free(RecordPP);
		RecordPP = NULL;
	}


	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		写入预览图数据
 *	@param[in]	i_stkFileP			文件对象指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS			写入成功
 *	@retval		STK_ERROR			写入失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_Preview::WriteFile(Stk_File *i_stkFileP)
{
	
	STK_STATUS ret = STK_SUCCESS;
	StkSegmentHead head = {0};
	StkThumbNailRec *thumbRecP = NULL;
#ifdef _WINDOWS_
	WCHAR			strConvertLog[MAX_PATH + 512];
	Stk_SVLLog		stkSVLLog;
	Stk_LogFileEx*	stkSVLLogFile;
#endif

	if(i_stkFileP == NULL)
	{
		return STK_ERROR;
	}
#ifdef _WINDOWS_
	stkSVLLog = i_stkFileP->GetDocumentManager()->GetSVLLogIni();
	stkSVLLogFile = i_stkFileP->GetDocumentManager()->GetSVLLogFile();
#endif

	/*
	 *	【第一步】 输出Preview段
	 */
#ifdef _WINDOWS_
	if (1 == stkSVLLog.bLogOutput)
	{
		_stprintf_s(strConvertLog,  _countof(strConvertLog), 
			_T("【Preview段】图片类型：%d"), m_eFiletype);
		stkSVLLogFile->Log(strConvertLog);
	}
#endif

	// 【第1.1步】输出Preview HEAD Record
	head.id = 0;
	head.kind = SEG_TYPE_PREVIEW;
	ret = i_stkFileP->WriteSegmentHead(&head);
	if(ret < STK_SUCCESS)
	{
#ifdef _WINDOWS_
		if (1 == stkSVLLog.bLogOutput)
		{
			_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：Preview段-头记录写入失败！"));
			stkSVLLogFile->Log(strConvertLog);
		}
#endif
		return STK_ERROR;
	}

	thumbRecP = (StkThumbNailRec*)calloc(1, sizeof(StkThumbNailRec) - sizeof(CHAR) + m_nuSize * sizeof(CHAR));

	// 【第1.2步】输出Thumbnail Record
	thumbRecP->Control.type = REC_TYPE_THUMBNAIL;
	thumbRecP->Control.version = _VER_THUMBNAIL_RECORD /*1*/;
	thumbRecP->Control.length = sizeof(StkThumbNailRec) - sizeof(CHAR) + m_nuSize * sizeof(CHAR) - sizeof(StkControl);

	thumbRecP->filetype = m_eFiletype;
	thumbRecP->buffsize = m_nuSize;
	memcpy( thumbRecP->buff, m_chStreamP, sizeof(CHAR)*m_nuSize);

	ret = i_stkFileP->WriteSegmentRecord(&head, (StkControl *)thumbRecP);
	if(ret < STK_SUCCESS)
	{
#ifdef _WINDOWS_
		if (1 == stkSVLLog.bLogOutput)
		{
			_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：Preview段Base记录写入失败！"));
			stkSVLLogFile->Log(strConvertLog);
		}
#endif
		return STK_ERROR;
	}
	/*
	 *	【1.3 完成Title段的输出】
	 */
	ret = i_stkFileP->WriteSegmentFinish(&head);
	if(ret < STK_SUCCESS)
	{
#ifdef _WINDOWS_
		if (1 == stkSVLLog.bLogOutput)
		{
			_stprintf_s(strConvertLog, _countof(strConvertLog), _T("■ERROR：Preview段Finish时失败！"));
			stkSVLLogFile->Log(strConvertLog);
		}
#endif
		return STK_ERROR;
	}

	if(thumbRecP != NULL){
		free(thumbRecP);
		thumbRecP = NULL;
	}
	return STK_SUCCESS;
}