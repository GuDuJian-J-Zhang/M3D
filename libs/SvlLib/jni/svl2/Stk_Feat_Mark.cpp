// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_Feat_Mark.cpp
 *
 *	@brief	标记特征类的实现
 *
 *	@par	历史:
 *		2016/02/15	创建。WangY@HOTEAMSOFT
****************************************************************************/

#ifndef __MOBILE__
#pragma once
#endif

#include "StdAfx.h"
#include "Stk_Feat_Mark.h"
#include "Stk_DocumentManager.h"
#ifdef _WINDOWS_
#include "Stk_LogFile.h"
#endif

/************************************************************************//**
 *	@brief		标记特征构造函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_Feat_Mark::Stk_Feat_Mark()
{
	m_rgbaMark.Red = 1.0;
	m_rgbaMark.Green = 0.0;
	m_rgbaMark.Blue = 0.0;
	m_rgbaMark.Alpha = 0.0;
	m_eType = MARK_TYPE_RECTANGLE;
	m_fCurveWidth = 0.5;
	m_eCurveType = CONTINUOUS_LINE;
	for (int ii = 0; ii < 4; ii++)
	{
		for (int jj = 0; jj < 4; jj++)
		{
			if (ii == jj)
			{
				m_mtxDefPlane.PlcMatrix[ii][jj] = 1.0;
			}
			else
			{
				m_mtxDefPlane.PlcMatrix[ii][jj] = 0.0;
			}
		}
	}
}

/************************************************************************//**
 *	@brief		标记特征析构函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_Feat_Mark::~Stk_Feat_Mark()
{

}

/************************************************************************//**
 *	@brief		设置标记颜色
 *	@param[in]	i_rgbaMark		标记颜色
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Feat_Mark::SetColor(STK_RGBA32 i_rgbaMark)
{
	m_rgbaMark = i_rgbaMark;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取标记颜色
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		标记颜色
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_RGBA32 Stk_Feat_Mark::GetColor()
{
	return m_rgbaMark;
}

/************************************************************************//**
 *	@brief		设置标记类型
 *	@param[in]	i_eType			标记类型
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Feat_Mark::SetType(StkMarkTypeEnum i_eType)
{
	m_eType = i_eType;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取标记类型
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		标记类型
 *	@note		无
 *	@attention	无
 ****************************************************************************/
StkMarkTypeEnum Stk_Feat_Mark::GetType()
{
	return m_eType;
}

/************************************************************************//**
 *	@brief		设置定义平面
 *	@param[in]	i_mtxDefPlane		定义平面
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Feat_Mark::SetDefPlane(const STK_MTX32& i_mtxDefPlane)
{
	m_mtxDefPlane = i_mtxDefPlane;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取定义平面
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		定义平面
 *	@note		无
 *	@attention	无
 ****************************************************************************/
const STK_MTX32& Stk_Feat_Mark::GetDefPlane()
{
	return m_mtxDefPlane;
}

/************************************************************************//**
 *	@brief		设置线的宽度
 *	@param[in]	i_fCurveWidth	线宽
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Feat_Mark::SetCurveWidth(STK_FLOAT32 i_fCurveWidth)
{
	m_fCurveWidth = i_fCurveWidth;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取线的宽度
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		线宽
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_FLOAT32 Stk_Feat_Mark::GetCurveWidth()
{
	return m_fCurveWidth;
}

/************************************************************************//**
 *	@brief		设置线的类型
 *	@param[in]	i_eCurveType	线型
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Feat_Mark::SetCurveType(StkCurveDispTypeEnum i_eCurveType)
{
	m_eCurveType = i_eCurveType;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取线的类型
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		线型
 *	@note		无
 *	@attention	无
 ****************************************************************************/
StkCurveDispTypeEnum Stk_Feat_Mark::GetCurveType()
{
	return m_eCurveType;
}

/*************************************************************************//**
 *	@brief		写标记特征数据
 *	@param[in]	i_stkFileP			文件对象指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS			写入成功
 *	@retval		其它				写入失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_Feat_Mark::WriteFile(Stk_File* i_stkFileP, StkSegmentHead* i_HeadP)
{
	STK_STATUS			eState = STK_SUCCESS;
	StkFeatMarkRec		FeatMarkRec;
#ifdef _WINDOWS_
	WCHAR				strConvertLog[MAX_PATH + 512];
	Stk_SVLLog			stkSVLLog;
	Stk_LogFileEx*		stkSVLLogFile;
#endif

	if (i_stkFileP == NULL || i_HeadP == NULL)
	{
		return STK_ERROR;
	}
#ifdef _WINDOWS_
	stkSVLLog = i_stkFileP->GetDocumentManager()->GetSVLLogIni();
	stkSVLLogFile = i_stkFileP->GetDocumentManager()->GetSVLLogFile();
#endif

	FeatMarkRec.Control.type = REC_TYPE_FEAT_MARK;
	FeatMarkRec.Control.length = sizeof(StkFeatMarkRec) - sizeof(StkControl);
	FeatMarkRec.Control.version = _VER_FEAT_MARK_RECORD;

	FeatMarkRec.nuType = m_eType;
	FeatMarkRec.rgbaMark = m_rgbaMark;
	FeatMarkRec.mtxDefPlane = m_mtxDefPlane;
	FeatMarkRec.fCurveWidth = m_fCurveWidth;
	FeatMarkRec.nuCurveType = m_eCurveType;

	eState = i_stkFileP->WriteSegmentRecordData(i_HeadP, (StkControl *)&FeatMarkRec);
	if(eState < STK_SUCCESS)
	{
		return STK_ERROR;
	}

	return eState;
}

/************************************************************************//**
 *	@brief		矩形标记特征构造函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_Mark_RectAngle::Stk_Mark_RectAngle()
{
	m_eFillStyle = MARK_FILLSTYLE_TRANSPARENT;
}

/************************************************************************//**
 *	@brief		矩形标记特征析构函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_Mark_RectAngle::~Stk_Mark_RectAngle()
{

}

/************************************************************************//**
 *	@brief		设置填充方式
 *	@param[in]	i_eFillStyle	填充方式
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Mark_RectAngle::SetFillStyle(StkMarkFillStyleEnum i_eFillStyle)
{
	m_eFillStyle = i_eFillStyle;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取填充方式
 *	@param[in]	无
 *	@retval		填充方式
 *	@note		无
 *	@attention	无
 ****************************************************************************/
StkMarkFillStyleEnum Stk_Mark_RectAngle::GetFillStyle()
{
	return m_eFillStyle;
}

/************************************************************************//**
 *	@brief		设置矩形标记锚点
 *	@param[in]	i_pntLeftTop		左上角锚点
 *	@param[in]	i_pntRightBottom	右下角锚点
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Mark_RectAngle::SetAnchorPoint(STK_PNT32& i_pntLeftTop, STK_PNT32& i_pntRightBottom)
{
	m_pntLeftTop = i_pntLeftTop;
	m_pntRightBottom = i_pntRightBottom;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取矩形标记锚点
 *	@param[in]	无
 *	@param[out]	o_pntLeftTop		左上角锚点
 *	@param[out]	o_pntRightBottom	右下角锚点
 *	@retval		STK_SUCCESS			获取成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Mark_RectAngle::GetAnchorPoint(STK_PNT32& o_pntLeftTop, STK_PNT32& o_pntRightBottom)
{
	o_pntLeftTop = m_pntLeftTop;
	o_pntRightBottom = m_pntRightBottom;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		读取文件中的矩形标记特征数据
 *	@param[in]	i_FileP				文件对象指针
 *	@param[in]	i_RectAngleHeadP	矩形标记记录指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS			读取成功
 *	@retval		STK_ERROR_MEMORY	内存申请失败
 *	@retval		其他				读取失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_Mark_RectAngle::ReadFile(Stk_File *i_FileP, StkSegmentHead *i_RectAngleHeadP)
{
	STK_STATUS				eState = STK_SUCCESS;
	StkControl**			RecordPP = NULL;
	StkSegmentHead			head ={0};
	StkFeatMarkRec*			FeatMarkRecP = NULL;
	StkMarkRectAngleRec*	MarkRectAngleRecP = NULL;

	/*
	 *	读取矩形特征标记段
	 */
	RecordPP = (StkControl **)calloc(i_RectAngleHeadP->recnum, sizeof(StkControl*));
	if(i_RectAngleHeadP->recnum !=0 && RecordPP == NULL)
	{
		return STK_ERROR_MEMORY;
	}
	eState = i_FileP->ReadSegmentAllRecordData(i_RectAngleHeadP, (StkControl **)RecordPP);
	if (eState!=STK_SUCCESS)
	{
		return eState;
	}
	
	/*
	 *	读取矩形特征标记段的各记录
	 */
	for (int ii = 0; ii < i_RectAngleHeadP->recnum; ii++)
	{
		switch(RecordPP[ii]->type)
		{
		case REC_TYPE_FEAT_MARK:
			if (RecordPP[ii]->version == 1)
			{
				FeatMarkRecP = (StkFeatMarkRec*)RecordPP[ii];
				SetType((StkMarkTypeEnum)FeatMarkRecP->nuType);
				SetColor(FeatMarkRecP->rgbaMark);
				SetDefPlane(FeatMarkRecP->mtxDefPlane);
				SetCurveType((StkCurveDispTypeEnum)FeatMarkRecP->nuCurveType);
				SetCurveWidth(FeatMarkRecP->fCurveWidth);
			}
			break;
		case REC_TYPE_MARK_RECTANGLE:
			if (RecordPP[ii]->version == 1)
			{
				MarkRectAngleRecP = (StkMarkRectAngleRec*)RecordPP[ii];
				m_eFillStyle = (StkMarkFillStyleEnum)MarkRectAngleRecP->nuFillStyle;
				m_pntLeftTop = MarkRectAngleRecP->pntLeftTop;
				m_pntRightBottom = MarkRectAngleRecP->pntRightBottom;
			}
			break;
		}
	}

	// 释放内存
	eState = i_FileP->ReadSegmentFinishData(i_RectAngleHeadP, (StkControl **)RecordPP);
	if (eState!=STK_SUCCESS)
	{
		return STK_ERROR_HEADRD;
	}
	if (RecordPP != NULL)
	{
		free(RecordPP);
		RecordPP = NULL;
	}

	return eState;
}

/*************************************************************************//**
 *	@brief		写矩形标记特征数据
 *	@param[in]	i_stkFileP			文件对象指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS			写入成功
 *	@retval		其它				写入失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_Mark_RectAngle::WriteFile(Stk_File *i_stkFileP)
{
	STK_STATUS			eState = STK_SUCCESS;
	StkSegmentHead		head = {0};
	StkMarkRectAngleRec	MarkRectAngleRec = {0};
	
	if (i_stkFileP == NULL)
	{
		return STK_ERROR;
	}
	/*
	 *	【第一步】 输出矩形标记段
	 */
	head.id = 0;
	head.kind = SEG_TYPE_MARK_RECTANGLE;
	eState = i_stkFileP->WriteSegmentHeadData(&head);
	if (eState < STK_SUCCESS)
	{
		return STK_ERROR;
	}
	// [1.1] 输出标记记录
	eState = Stk_Feat_Mark::WriteFile(i_stkFileP, &head);
	if (eState < STK_SUCCESS)
	{
		return STK_ERROR;
	}

	// [1.2] 输出矩形标记记录
	MarkRectAngleRec.Control.type = REC_TYPE_MARK_RECTANGLE;
	MarkRectAngleRec.Control.length = sizeof(StkMarkRectAngleRec) - sizeof(StkControl);
	MarkRectAngleRec.Control.version = _VER_FEAT_MARK_RECTANGLE_RECORD;

	MarkRectAngleRec.nuFillStyle = m_eFillStyle;
	MarkRectAngleRec.pntLeftTop = m_pntLeftTop;
	MarkRectAngleRec.pntRightBottom = m_pntRightBottom;

	eState = i_stkFileP->WriteSegmentRecordData(&head, (StkControl *)&MarkRectAngleRec);
	if (eState != STK_SUCCESS)
	{
		return STK_ERROR;
	}

	// [1.3] 完成矩形标记段输出
	eState = i_stkFileP->WriteSegmentFinishData(&head);
	if (eState != STK_SUCCESS)
	{
		return STK_ERROR;
	}

	return eState;
}

/************************************************************************//**
 *	@brief		椭圆标记特征构造函数
 *	@param[in]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_Mark_Ellipse::Stk_Mark_Ellipse()
{
	m_eFillStyle = MARK_FILLSTYLE_TRANSPARENT;
	m_fMajorRadius = 0;
	m_fMinorRadius = 0;
	m_dirNormal.X = 1.0;
	m_dirNormal.Y = 0.0;
	m_dirNormal.Z = 0.0;
	m_dirOrigin.X = 1.0;
	m_dirOrigin.Y = 0.0;
	m_dirOrigin.Z = 0.0;
}

/************************************************************************//**
 *	@brief		椭圆标记特征析构函数
 *	@param[in]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_Mark_Ellipse::~Stk_Mark_Ellipse()
{

}

/************************************************************************//**
 *	@brief		设置填充方式
 *	@param[in]	i_eFillStyle	填充方式
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Mark_Ellipse::SetFillStyle(StkMarkFillStyleEnum i_eFillStyle)
{
	m_eFillStyle = i_eFillStyle;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取填充方式
 *	@param[in]	无
 *	@retval		填充方式
 *	@note		无
 *	@attention	无
 ****************************************************************************/
StkMarkFillStyleEnum Stk_Mark_Ellipse::GetFillStyle()
{
	return m_eFillStyle;
}

/************************************************************************//**
 *	@brief		设置椭圆中心点
 *	@param[in]	i_pntCenter		椭圆中心点
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Mark_Ellipse::SetCenter(STK_PNT32 i_pntCenter)
{
	m_pntCenter = i_pntCenter;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取椭圆中心点
 *	@param[in]	无
 *	@retval		椭圆中心点
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_PNT32 Stk_Mark_Ellipse::GetCenter()
{
	return m_pntCenter;
}

/************************************************************************//**
 *	@brief		设置椭圆长半径
 *	@param[in]	i_fMajorRadius	椭圆长半径
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Mark_Ellipse::SetMajorRadius(STK_FLOAT32 i_fMajorRadius)
{
	m_fMajorRadius = i_fMajorRadius;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取椭圆长半径
 *	@param[in]	无
 *	@retval		椭圆长半径
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_FLOAT32 Stk_Mark_Ellipse::GetMajorRadius()
{
	return m_fMajorRadius;
}

/************************************************************************//**
 *	@brief		设置椭圆短半径
 *	@param[in]	i_fMajorRadius	椭圆短半径
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Mark_Ellipse::SetMinorRadius(STK_FLOAT32 i_fMinorRadius)
{
	m_fMinorRadius = i_fMinorRadius;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取椭圆短半径
 *	@param[in]	无
 *	@retval		椭圆短半径
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_FLOAT32 Stk_Mark_Ellipse::GetMinorRadius()
{
	return m_fMinorRadius;
}

/************************************************************************//**
 *	@brief		设置椭圆法线方向
 *	@param[in]	i_dirNormal		椭圆法线方向
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Mark_Ellipse::SetNormal(STK_DIR32 i_dirNormal)
{
	m_dirNormal = i_dirNormal;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取椭圆法线方向
 *	@param[in]	无
 *	@retval		椭圆法线方向
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_DIR32 Stk_Mark_Ellipse::GetNormal()
{
	return m_dirNormal;
}

/************************************************************************//**
 *	@brief		设置椭圆原点方向
 *	@param[in]	i_dirOrigin		椭圆原点方向
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Mark_Ellipse::SetOriginDir(STK_DIR32 i_dirOrigin)
{
	m_dirOrigin = i_dirOrigin;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取椭圆原点方向
 *	@param[in]	无
 *	@retval		椭圆原点方向
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_DIR32 Stk_Mark_Ellipse::GetOriginDir()
{
	return m_dirOrigin;
}

/************************************************************************//**
 *	@brief		读取文件中的椭圆标记特征数据
 *	@param[in]	i_FileP				文件对象指针
 *	@param[in]	i_MarkEllipseHeadP	椭圆标记记录指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS			读取成功
 *	@retval		STK_ERROR_MEMORY	内存申请失败
 *	@retval		其他				读取失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_Mark_Ellipse::ReadFile(Stk_File *i_FileP, StkSegmentHead *i_MarkEllipseHeadP)
{
	STK_STATUS				eState = STK_SUCCESS;
	StkControl**			RecordPP = NULL;
	StkSegmentHead			head ={0};
	StkFeatMarkRec*			FeatMarkRecP = NULL;
	StkMarkEllipseRec*		MarkEllipseRecP = NULL;

	/*
	 *	读取椭圆特征标记段
	 */
	RecordPP = (StkControl **)calloc(i_MarkEllipseHeadP->recnum, sizeof(StkControl*));
	if(i_MarkEllipseHeadP->recnum !=0 && RecordPP == NULL)
	{
		return STK_ERROR_MEMORY;
	}
	eState = i_FileP->ReadSegmentAllRecordData(i_MarkEllipseHeadP, (StkControl **)RecordPP);
	if (eState!=STK_SUCCESS)
	{
		return eState;
	}
	
	/*
	 *	读取椭圆特征标记段的各记录
	 */
	for (int ii = 0; ii < i_MarkEllipseHeadP->recnum; ii++)
	{
		switch(RecordPP[ii]->type)
		{
		case REC_TYPE_FEAT_MARK:
			if (RecordPP[ii]->version == 1)
			{
				FeatMarkRecP = (StkFeatMarkRec*)RecordPP[ii];
				SetType((StkMarkTypeEnum)FeatMarkRecP->nuType);
				SetColor(FeatMarkRecP->rgbaMark);
				SetDefPlane(FeatMarkRecP->mtxDefPlane);
				SetCurveType((StkCurveDispTypeEnum)FeatMarkRecP->nuCurveType);
				SetCurveWidth(FeatMarkRecP->fCurveWidth);
			}
			break;
		case REC_TYPE_MARK_ELLIPSE:
			if (RecordPP[ii]->version == 1)
			{
				MarkEllipseRecP = (StkMarkEllipseRec*)RecordPP[ii];
				m_eFillStyle = (StkMarkFillStyleEnum)MarkEllipseRecP->nuFillStyle;
				m_pntCenter = MarkEllipseRecP->pntCenter;
				m_fMajorRadius = MarkEllipseRecP->fMajorRadius;
				m_fMinorRadius = MarkEllipseRecP->fMinorRadius;
				m_dirNormal = MarkEllipseRecP->dirNormal;
				m_dirOrigin = MarkEllipseRecP->dirOrigin;
			}
			break;
		}
	}

	// 释放内存
	eState = i_FileP->ReadSegmentFinishData(i_MarkEllipseHeadP, (StkControl **)RecordPP);
	if (eState!=STK_SUCCESS)
	{
		return STK_ERROR_HEADRD;
	}
	if (RecordPP != NULL)
	{
		free(RecordPP);
		RecordPP = NULL;
	}

	return eState;
}

/*************************************************************************//**
 *	@brief		写椭圆标记特征数据
 *	@param[in]	i_stkFileP			文件对象指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS			写入成功
 *	@retval		其它				写入失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_Mark_Ellipse::WriteFile(Stk_File *i_stkFileP)
{
	STK_STATUS			eState = STK_SUCCESS;
	StkSegmentHead		head = {0};
	StkMarkEllipseRec	MarkEllipseRec = {0};

	if (i_stkFileP == NULL)
	{
		return STK_ERROR;
	}
	/*
	 *	【第一步】 输出椭圆标记段
	 */
	head.id = 0;
	head.kind = SEG_TYPE_MARK_ELLIPSE;
	eState = i_stkFileP->WriteSegmentHeadData(&head);
	if (eState < STK_SUCCESS)
	{
		return STK_ERROR;
	}
	// [1.1] 输出标记记录
	eState = Stk_Feat_Mark::WriteFile(i_stkFileP, &head);
	if (eState < STK_SUCCESS)
	{
		return STK_ERROR;
	}

	// [1.2] 输出椭圆标记记录
	MarkEllipseRec.Control.type = REC_TYPE_MARK_ELLIPSE;
	MarkEllipseRec.Control.length = sizeof(StkMarkEllipseRec) - sizeof(StkControl);
	MarkEllipseRec.Control.version = _VER_FEAT_MARK_ELLIPSE_RECORD;

	MarkEllipseRec.nuFillStyle = m_eFillStyle;
	MarkEllipseRec.pntCenter = m_pntCenter;
	MarkEllipseRec.fMajorRadius = m_fMajorRadius;
	MarkEllipseRec.fMinorRadius = m_fMinorRadius;
	MarkEllipseRec.dirNormal = m_dirNormal;
	MarkEllipseRec.dirOrigin = m_dirOrigin;

	eState = i_stkFileP->WriteSegmentRecordData(&head, (StkControl*)&MarkEllipseRec);
	if (eState != STK_SUCCESS)
	{
		return STK_ERROR;
	}

	// [1.3] 完成矩形标记段输出
	eState = i_stkFileP->WriteSegmentFinishData(&head);
	if (eState != STK_SUCCESS)
	{
		return STK_ERROR;
	}

	return eState;
}

/************************************************************************//**
 *	@brief		徒手画标记特征构造函数
 *	@param[in]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_Mark_FreeHand::Stk_Mark_FreeHand()
{
	m_fUMin = 0.0;
	m_fUMax = 0.0;
}

/************************************************************************//**
 *	@brief		徒手画标记特征析构函数
 *	@param[in]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_Mark_FreeHand::~Stk_Mark_FreeHand()
{

}

/************************************************************************//**
 *	@brief		设置徒手画的起始终止范围
 *	@param[in]	i_fUMin				起始范围
 *	@param[in]	i_fUMax				终止范围
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@retval		STK_ERROR			设置失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Mark_FreeHand::SetDomain(STK_FLOAT32 i_fUMin, STK_FLOAT32 i_fUMax)
{
	m_fUMin = i_fUMin;
	m_fUMax = i_fUMax;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取徒手画的起始终止范围
 *	@param[in]	无
 *	@param[out]	o_fUMin				起始范围
 *	@param[out]	o_fUMax				终止范围
 *	@retval		STK_SUCCESS			设置成功
 *	@retval		STK_ERROR			设置失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Mark_FreeHand::GetDomain(STK_FLOAT32& o_fUMin, STK_FLOAT32& o_fUMax)
{
	o_fUMin = m_fUMin;
	o_fUMax = m_fUMax;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		添加徒手画的点列数据
 *	@param[in]	i_pntFreeHand		点列数据
 *	@param[out]	无
 *	@retval		STK_SUCCESS			添加成功
 *	@retval		其它				添加失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Mark_FreeHand::AddPoint(STK_PNT32 i_pntFreeHand)
{
	m_vcPoints.push_back(i_pntFreeHand);
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取徒手画的点列数量
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		徒手画的点列数量
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_UINT32 Stk_Mark_FreeHand::GetPointsNum()
{
	return m_vcPoints.size();
}

/************************************************************************//**
 *	@brief		获取指定索引的点列数据
 *	@param[in]	i_nuIndex			索引
 *	@param[out]	无
 *	@retval		指定索引的点列数据
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_PNT32 Stk_Mark_FreeHand::GetPointByIndex(STK_UINT32 i_nuIndex)
{
	return m_vcPoints.at(i_nuIndex);
}

/************************************************************************//**
 *	@brief		读取文件中的徒手画标记特征数据
 *	@param[in]	i_FileP				文件对象指针
 *	@param[in]	i_MarkFreeHandHeadP	徒手画标记记录指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS			读取成功
 *	@retval		STK_ERROR_MEMORY	内存申请失败
 *	@retval		其他				读取失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_Mark_FreeHand::ReadFile(Stk_File *i_FileP, StkSegmentHead *i_MarkFreeHandHeadP)
{
	STK_STATUS				eState = STK_SUCCESS;
	StkControl**			RecordPP = NULL;
	StkSegmentHead			head ={0};
	StkFeatMarkRec*			FeatMarkRecP = NULL;
	StkMarkFreeHandRec*		MarkFreeHandRecP = NULL;

	/*
	 *	读取徒手画特征标记段
	 */
	RecordPP = (StkControl **)calloc(i_MarkFreeHandHeadP->recnum, sizeof(StkControl*));
	if(i_MarkFreeHandHeadP->recnum !=0 && RecordPP == NULL)
	{
		return STK_ERROR_MEMORY;
	}
	eState = i_FileP->ReadSegmentAllRecordData(i_MarkFreeHandHeadP, (StkControl **)RecordPP);
	if (eState!=STK_SUCCESS)
	{
		return eState;
	}
	
	/*
	 *	读取徒手画特征标记段的各记录
	 */
	for (int ii = 0; ii < i_MarkFreeHandHeadP->recnum; ii++)
	{
		switch(RecordPP[ii]->type)
		{
		case REC_TYPE_FEAT_MARK:
			if (RecordPP[ii]->version == 1)
			{
				FeatMarkRecP = (StkFeatMarkRec*)RecordPP[ii];
				SetType((StkMarkTypeEnum)FeatMarkRecP->nuType);
				SetColor(FeatMarkRecP->rgbaMark);
				SetDefPlane(FeatMarkRecP->mtxDefPlane);
				SetCurveType((StkCurveDispTypeEnum)FeatMarkRecP->nuCurveType);
				SetCurveWidth(FeatMarkRecP->fCurveWidth);
			}
			break;
		case REC_TYPE_MARK_FREEHAND:
			if (RecordPP[ii]->version == 1)
			{
				MarkFreeHandRecP = (StkMarkFreeHandRec*)RecordPP[ii];
				m_fUMax = MarkFreeHandRecP->fUMax;
				m_fUMin = MarkFreeHandRecP->fUMin;
				for (int ii = 0; ii < MarkFreeHandRecP->nuPointNum; ii++)
				{
					STK_PNT32 pntPoint;
					pntPoint.X = MarkFreeHandRecP->atr[ii*3 + 0];
					pntPoint.Y = MarkFreeHandRecP->atr[ii*3 + 1];
					pntPoint.Z = MarkFreeHandRecP->atr[ii*3 + 2];
					m_vcPoints.push_back(pntPoint);
				}
			}
			break;
		}
	}

	// 释放内存
	eState = i_FileP->ReadSegmentFinishData(i_MarkFreeHandHeadP, (StkControl **)RecordPP);
	if (eState!=STK_SUCCESS)
	{
		return STK_ERROR_HEADRD;
	}
	if (RecordPP != NULL)
	{
		free(RecordPP);
		RecordPP = NULL;
	}

	return eState;
}

/*************************************************************************//**
 *	@brief		写徒手画标记特征数据
 *	@param[in]	i_stkFileP			文件对象指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS			写入成功
 *	@retval		其它				写入失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_Mark_FreeHand::WriteFile(Stk_File *i_stkFileP)
{
	STK_STATUS			eState = STK_SUCCESS;
	StkSegmentHead		head = {0};
	StkMarkFreeHandRec*	MarkFreeHandRecP = NULL;

	if (i_stkFileP == NULL)
	{
		return STK_ERROR;
	}

	/*
	 *	【第一步】 输出徒手画标记段
	 */
	head.id = 0;
	head.kind = SEG_TYPE_MARK_FREEHAND;
	eState = i_stkFileP->WriteSegmentHeadData(&head);
	if (eState < STK_SUCCESS)
	{
		return STK_ERROR;
	}
	// [1.1] 输出标记记录
	eState = Stk_Feat_Mark::WriteFile(i_stkFileP, &head);
	if (eState < STK_SUCCESS)
	{
		return STK_ERROR;
	}

	// [1.2] 输出徒手画标记记录
	MarkFreeHandRecP = (StkMarkFreeHandRec*)calloc(1, sizeof(StkMarkFreeHandRec) - sizeof(STK_CHAR) + m_vcPoints.size()*sizeof(STK_PNT32));
	if (MarkFreeHandRecP == NULL)
	{
		return STK_ERROR_MEMORY;
	}
	MarkFreeHandRecP->Control.type = REC_TYPE_MARK_FREEHAND;
	MarkFreeHandRecP->Control.length = sizeof(StkMarkFreeHandRec) - sizeof(STK_CHAR) + m_vcPoints.size()*sizeof(STK_PNT32) - sizeof(StkControl);
	MarkFreeHandRecP->Control.version = _VER_FEAT_MARK_FREEHAND_RECORD;

	MarkFreeHandRecP->fUMax = m_fUMax;
	MarkFreeHandRecP->fUMin = m_fUMin;
	MarkFreeHandRecP->nuPointNum = m_vcPoints.size();
	for (int ii = 0; ii < m_vcPoints.size(); ii++)
	{
		memcpy(MarkFreeHandRecP->atr + ii*3, &m_vcPoints[ii], sizeof(STK_PNT32));
	}

	eState = i_stkFileP->WriteSegmentRecordData(&head, (StkControl *)MarkFreeHandRecP);
	if (eState != STK_SUCCESS)
	{
		return STK_ERROR;
	}

	// [1.3] 完成徒手画标记段输出
	eState = i_stkFileP->WriteSegmentFinishData(&head);
	if (eState != STK_SUCCESS)
	{
		return STK_ERROR;
	}

	return eState;
}