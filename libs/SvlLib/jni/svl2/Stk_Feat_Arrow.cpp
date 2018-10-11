// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_Feat_Arrow.cpp
 *
 *	@brief	箭头特征类的实现
 *
 *	@par	历史:
 *		2015/08/07	创建。WangY@HOTEAMSOFT
****************************************************************************/

#ifndef __MOBILE__
#pragma once
#endif

#include "StdAfx.h"
#include "Stk_Feat_Arrow.h"
#include "Stk_DocumentManager.h"
#include "Stk_Platform.h"
#ifdef _WINDOWS_
#include "Stk_LogFile.h"
#endif

/************************************************************************//**
 *	@brief		箭头特征构造函数
 *	@param[in]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_Feat_Arrow::Stk_Feat_Arrow()
{
	m_wstrName = L"";
	m_fAlpha = 1;
	m_bIsOnTop = true;
	m_eActorPosition = ACTOR_POS_BEGIN;
	m_fActorForm = 0;
	m_fActorLength = 1;
	m_fActorWidth = 1;
	m_fWidth = 1;
	m_fGradient = 0;
	m_bIsGradualChange = false;
	m_bIsDisplayBorder = true;
	m_fBorderWidth = 0.5;
	m_eLineType = CONTINUOUS_LINE;
}

/************************************************************************//**
 *	@brief		箭头特征析构函数
 *	@param[in]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_Feat_Arrow::~Stk_Feat_Arrow()
{

}

/************************************************************************//**
 *	@brief		设置箭头名称
 *	@param[in]	i_wstrName		箭头名称
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Feat_Arrow::SetName(wstring i_wstrName)
{
	m_wstrName = i_wstrName; 
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取箭头名称
 *	@param[in]	无
 *	@retval		箭头名称
 *	@note		无
 *	@attention	无
 ****************************************************************************/
wstring Stk_Feat_Arrow::GetName()
{
	return m_wstrName;
}

/************************************************************************//**
 *	@brief		设置箭头类型
 *	@param[in]	i_eType			箭头类型
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Feat_Arrow::SetType(StkArrowTypeEnum i_eType)
{
	m_eType = i_eType;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取箭头类型
 *	@param[in]	无
 *	@retval		箭头类型
 *	@note		无
 *	@attention	无
 ****************************************************************************/
StkArrowTypeEnum Stk_Feat_Arrow::GetType()
{
	return m_eType;
}

/************************************************************************//**
 *	@brief		设置箭头的不透明性
 *	@param[in]	i_fAlpha		不透明性
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Feat_Arrow::SetOpacity(STK_FLOAT32 i_fAlpha)
{
	m_fAlpha = i_fAlpha;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取箭头的不透明性
 *	@param[in]	i_fAlpha		不透明性
 *	@retval		不透明性
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_FLOAT32 Stk_Feat_Arrow::GetOpacity()
{
	return m_fAlpha;
}

/************************************************************************//**
 *	@brief		设置箭头是否总在最前端
 *	@param[in]	i_bIsOnTop		是否总在最前端
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Feat_Arrow::SetOnTop(STK_BOOL i_bIsOnTop)
{
	m_bIsOnTop = i_bIsOnTop;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		箭头是否总在最前端
 *	@param[in]	无
 *	@retval		是否总在最前端
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_BOOL Stk_Feat_Arrow::IsOnTop()
{
	return m_bIsOnTop;
}

/************************************************************************//**
 *	@brief		设置灯头位置
 *	@param[in]	i_eActorPosition	灯头位置
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Feat_Arrow::SetActorPosition(StkActorPositionEnum i_eActorPosition)
{
	m_eActorPosition = i_eActorPosition;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取灯头位置
 *	@param[in]	无
 *	@retval		灯头位置
 *	@note		无
 *	@attention	无
 ****************************************************************************/
StkActorPositionEnum Stk_Feat_Arrow::GetActorPosition()
{
	return m_eActorPosition;
}

/************************************************************************//**
 *	@brief		设置灯头形状
 *	@param[in]	i_fActorForm		灯头形状
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Feat_Arrow::SetActorForm(STK_FLOAT32 i_fActorForm)
{
	m_fActorForm = i_fActorForm;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取灯头形状
 *	@param[in]	无
 *	@retval		灯头形状
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_FLOAT32 Stk_Feat_Arrow::GetActorForm()
{
	return m_fActorForm;
}

/************************************************************************//**
 *	@brief		设置灯头长度
 *	@param[in]	i_fActorLength		灯头长度
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Feat_Arrow::SetActorLength(STK_FLOAT32 i_fActorLength)
{
	m_fActorLength = i_fActorLength;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取灯头长度
 *	@param[in]	无
 *	@retval		灯头长度
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_FLOAT32 Stk_Feat_Arrow::GetActorLength()
{
	return m_fActorLength;
}

/************************************************************************//**
 *	@brief		设置灯头宽度
 *	@param[in]	i_fActorWidth		灯头宽度
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Feat_Arrow::SetActorWidth(STK_FLOAT32 i_fActorWidth)
{
	m_fActorWidth = i_fActorWidth;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取灯头宽度
 *	@param[in]	无
 *	@retval		灯头宽度
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_FLOAT32 Stk_Feat_Arrow::GetActorWidth()
{
	return m_fActorWidth;
}

/************************************************************************//**
 *	@brief		设置箭头宽度
 *	@param[in]	i_fArrowWidth		箭头宽度
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Feat_Arrow::SetArrowWidth(STK_FLOAT32 i_fArrowWidth)
{
	m_fWidth = i_fArrowWidth;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取箭头宽度
 *	@param[in]	无
 *	@retval		箭头宽度
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_FLOAT32 Stk_Feat_Arrow::GetArrowWidth()
{
	return m_fWidth;
}

/************************************************************************//**
 *	@brief		设置箭头倾斜度
 *	@param[in]	i_fArrowGradient	箭头倾斜度
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Feat_Arrow::SetArrowGradient(STK_FLOAT32 i_fArrowGradient)
{
	m_fGradient = i_fArrowGradient;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取箭头倾斜度
 *	@param[in]	无
 *	@retval		箭头倾斜度
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_FLOAT32 Stk_Feat_Arrow::GetArrowGradient()
{
	return m_fGradient;
}

/************************************************************************//**
 *	@brief		设置箭头背景色
 *	@param[in]	i_rgbaBackground	箭头背景色
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Feat_Arrow::SetBackgroundColor(STK_RGBA32 i_rgbaBackground)
{
	m_rgbaBackground = i_rgbaBackground;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取箭头背景色
 *	@param[in]	无
 *	@retval		箭头背景色
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_RGBA32 Stk_Feat_Arrow::GetBackgroundColor()
{
	return m_rgbaBackground;
}

/************************************************************************//**
 *	@brief		设置箭头背景色是否渐变
 *	@param[in]	i_bBackgroundColorChange	箭头背景色是否渐变
 *	@retval		STK_SUCCESS					设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Feat_Arrow::SetBackgroundColorChange(STK_BOOL i_bBackgroundColorChange)
{
	m_bIsGradualChange = i_bBackgroundColorChange;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		箭头背景色是否渐变
 *	@param[in]	无
 *	@retval		箭头背景色是否渐变
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_BOOL Stk_Feat_Arrow::IsBackgroundColorChange()
{
	return m_bIsGradualChange;
}

/************************************************************************//**
 *	@brief		设置边框是否显示
 *	@param[in]	i_bIsBorderDisplay	边框是否显示
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Feat_Arrow::SetBorderDisplay(STK_BOOL i_bIsBorderDisplay)
{
	m_bIsDisplayBorder = i_bIsBorderDisplay;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		边框是否显示
 *	@param[in]	无
 *	@retval		边框是否显示
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_BOOL Stk_Feat_Arrow::IsBorderDisplay()
{
	return m_bIsDisplayBorder;
}

/************************************************************************//**
 *	@brief		设置边框宽度
 *	@param[in]	i_fBorderWidth		边框宽度
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Feat_Arrow::SetBorderWidth(STK_FLOAT32 i_fBorderWidth)
{
	m_fBorderWidth = i_fBorderWidth;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取边框宽度
 *	@param[in]	无
 *	@retval		边框宽度
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_FLOAT32 Stk_Feat_Arrow::GetBorderWidth()
{
	return m_fBorderWidth;
}

/************************************************************************//**
 *	@brief		设置边框颜色
 *	@param[in]	i_rgbaBorder		边框颜色
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Feat_Arrow::SetBorderColor(STK_RGBA32 i_rgbaBorder)
{
	m_rgbaBorder = i_rgbaBorder;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取边框颜色
 *	@param[in]	无
 *	@retval		边框颜色
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_RGBA32 Stk_Feat_Arrow::GetBorderColor()
{
	return m_rgbaBorder;
}

/************************************************************************//**
 *	@brief		设置边框线类型
 *	@param[in]	i_eBorderType		边框线类型
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Feat_Arrow::SetBorderType(StkCurveDispTypeEnum i_eBorderType)
{
	m_eLineType = i_eBorderType;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取边框线类型
 *	@param[in]	无
 *	@retval		边框线类型
 *	@note		无
 *	@attention	无
 ****************************************************************************/
StkCurveDispTypeEnum Stk_Feat_Arrow::GetBorderType()
{
	return m_eLineType;
}

/************************************************************************//**
 *	@brief		获取定义平面
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		定义平面
 *	@note		无
 *	@attention	无
 ****************************************************************************/
const STK_MTX32& Stk_Feat_Arrow::GetDefPlane()
{
	return m_mtxDefPlane;
}

/************************************************************************//**
 *	@brief		设置定义平面
 *	@param[in]	i_mtxDefPlane		定义平面
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Feat_Arrow::SetDefPlane(const STK_MTX32& i_mtxDefPlane)
{
	m_mtxDefPlane = i_mtxDefPlane;
	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		写箭头特征数据
 *	@param[in]	i_stkFileP			文件对象指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS			写入成功
 *	@retval		其它				写入失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_Feat_Arrow::WriteFile(Stk_File* i_stkFileP, StkSegmentHead* i_HeadP)
{
	STK_STATUS			eState = STK_SUCCESS;
	StkFeatArrowRec2	FeatArrowRec;
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

	FeatArrowRec.Control.type = REC_TYPE_ARROW;
	FeatArrowRec.Control.length = sizeof(StkFeatArrowRec2) - sizeof(StkControl);
	FeatArrowRec.Control.version = _VER_FEAT_ARROW_RECORD;

#ifdef __MOBILE__
	memcpy(FeatArrowRec.name, Stk_Platform::WStringToSTKWCHARStr(m_wstrName),(m_wstrName.size()+1)*sizeof(STK_WCHAR));
#else
	wcscpy_s(FeatArrowRec.name, STK_MAXNAME,(WCHAR*)m_wstrName.c_str());
#endif
	FeatArrowRec.nuType = m_eType;
	FeatArrowRec.fAlpha = m_fAlpha;
	FeatArrowRec.IsItem = 0;
	if (!m_bIsOnTop)
	{
		FeatArrowRec.IsItem |= STK_ARROW_IS_NO_ONTOP;
	}
	if (m_bIsGradualChange)
	{
		FeatArrowRec.IsItem |= STK_BACKGROUND_IS_GRADUALCHANGED;
	}
	if (!m_bIsDisplayBorder)
	{
		FeatArrowRec.IsItem |= STK_BORDER_IS_NODISPLAY;
	}
	FeatArrowRec.nuActorPosition = m_eActorPosition;
	FeatArrowRec.fActorForm = m_fActorForm;
	FeatArrowRec.fActorLength = m_fActorLength;
	FeatArrowRec.fActorWidth = m_fActorWidth;
	FeatArrowRec.fArrowWidth = m_fWidth;
	FeatArrowRec.fArrowGradient = m_fGradient;
	FeatArrowRec.rgbaBackground = m_rgbaBackground;
	FeatArrowRec.fBorderWidth = m_fBorderWidth;
	FeatArrowRec.rgbaBorder = m_rgbaBorder;
	FeatArrowRec.nuBorderType = m_eLineType;
	FeatArrowRec.DefPlane = m_mtxDefPlane;

	eState = i_stkFileP->WriteSegmentRecordData(i_HeadP, (StkControl *)&FeatArrowRec);
	if(eState < STK_SUCCESS)
	{
		return STK_ERROR;
	}

	return eState;
}

/************************************************************************//**
 *	@brief		直线箭头特征构造函数
 *	@param[in]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_Feat_LineArrow::Stk_Feat_LineArrow()
{
	m_dirX1.X = 1;
	m_dirX1.Y = 0;
	m_dirX1.Z = 0;
	m_dirY1.X = 0;
	m_dirY1.Y = 1;
	m_dirY1.Z = 0;

	m_dirX2.X = 1;
	m_dirX2.Y = 0;
	m_dirX2.Z = 0;
	m_dirY2.X = 0;
	m_dirY2.Y = 1;
	m_dirY2.Z = 0;
}

/************************************************************************//**
 *	@brief		直线箭头特征析构函数
 *	@param[in]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_Feat_LineArrow::~Stk_Feat_LineArrow()
{

}

/************************************************************************//**
 *	@brief		设置直线箭头的第一个锚点
 *	@param[in]	i_pntAnchor1		锚点1
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Feat_LineArrow::SetFirstAnchorPoint(STK_PNT32 i_pntAnchor1)
{
	m_pntAnchor1 = i_pntAnchor1;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取直线箭头的第一个锚点
 *	@param[in]	无
 *	@retval		锚点1
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_PNT32 Stk_Feat_LineArrow::GetFirstAnchorPoint()
{
	return m_pntAnchor1;
}

/************************************************************************//**
 *	@brief		设置直线箭头的第二个锚点
 *	@param[in]	i_pntAnchor2		锚点2
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Feat_LineArrow::SetSecondAnchorPoint(STK_PNT32 i_pntAnchor2)
{
	m_pntAnchor2 = i_pntAnchor2;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取直线箭头的第二个锚点
 *	@param[in]	无
 *	@retval		锚点2
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_PNT32 Stk_Feat_LineArrow::GetSecondAnchorPoint()
{
	return m_pntAnchor2;
}

/************************************************************************//**
 *	@brief		设置锚点1的X方向向量
 *	@param[in]	i_dirX				X方向向量
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Feat_LineArrow::SetFirstAnchorPointDirX(STK_DIR32 i_dirX)
{
	m_dirX1 = i_dirX;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取锚点1的X方向向量
 *	@param[in]	无
 *	@retval		锚点1的X方向向量
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_DIR32 Stk_Feat_LineArrow::GetFirstAnchorPointDirX()
{
	return m_dirX1;
}

/************************************************************************//**
 *	@brief		设置锚点1的Y方向向量
 *	@param[in]	i_dirY				Y方向向量
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Feat_LineArrow::SetFirstAnchorPointDirY(STK_DIR32 i_dirY)
{
	m_dirY1 = i_dirY;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取锚点1的Y方向向量
 *	@param[in]	无
 *	@retval		锚点1的Y方向向量
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_DIR32 Stk_Feat_LineArrow::GetFirstAnchorPointDirY()
{
	return m_dirY1;
}

/************************************************************************//**
 *	@brief		设置锚点2的X方向向量
 *	@param[in]	i_dirX				X方向向量
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Feat_LineArrow::SetSecondAnchorPointDirX(STK_DIR32 i_dirX)
{
	m_dirX2 = i_dirX;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取锚点2的X方向向量
 *	@param[in]	无
 *	@retval		锚点2的X方向向量
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_DIR32 Stk_Feat_LineArrow::GetSecondAnchorPointDirX()
{
	return m_dirX2;
}

/************************************************************************//**
 *	@brief		设置锚点2的Y方向向量
 *	@param[in]	i_dirY				Y方向向量
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Feat_LineArrow::SetSecondAnchorPointDirY(STK_DIR32 i_dirY)
{
	m_dirY2 = i_dirY;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取锚点2的Y方向向量
 *	@param[in]	无
 *	@retval		锚点1的Y方向向量
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_DIR32 Stk_Feat_LineArrow::GetSecondAnchorPointDirY()
{
	return m_dirY2;
}

/************************************************************************//**
 *	@brief		读取文件中的直线箭头特征数据
 *	@param[in]	i_FileP				文件对象指针
 *	@param[in]	i_LineArrowHeadP	直线箭头记录指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS			读取成功
 *	@retval		STK_ERROR_MEMORY	内存申请失败
 *	@retval		其他				读取失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_Feat_LineArrow::ReadFile(Stk_File *i_FileP, StkSegmentHead *i_LineArrowHeadP)
{
	STK_STATUS				eState = STK_SUCCESS;
	StkControl**			RecordPP = NULL;
	StkSegmentHead			head ={0};
	StkFeatArrowRec*		FeatArrowRecP = NULL;
	StkFeatArrowRec2*		FeatArrowRec2P = NULL;
	StkFeatLineArrowRec*	FeatLineArrowRecP = NULL;
	
	/*
	 *	读取直线箭头特征段
	 */
	RecordPP = (StkControl **)calloc(i_LineArrowHeadP->recnum, sizeof(StkControl*));
	if(i_LineArrowHeadP->recnum !=0 && RecordPP == NULL)
	{
		return STK_ERROR_MEMORY;
	}
	eState = i_FileP->ReadSegmentAllRecordData(i_LineArrowHeadP, (StkControl **)RecordPP);
	if (eState!=STK_SUCCESS)
	{
		return eState;
	}
	/*
	 *	读取直线箭头特征段的各记录
	 */
	for (int ii = 0; ii < i_LineArrowHeadP->recnum; ii++)
	{
		switch(RecordPP[ii]->type)
		{
		case REC_TYPE_ARROW:
			if (RecordPP[ii]->version == 1)
			{
				FeatArrowRecP = (StkFeatArrowRec*)RecordPP[ii];
#ifdef __MOBILE__
				SetName(Stk_Platform::STKWCHARStrToWString(FeatArrowRecP->name));
#else
				SetName(FeatArrowRecP->name);
#endif
				SetType((StkArrowTypeEnum)FeatArrowRecP->nuType);
				if (FeatArrowRecP->IsItem & STK_ARROW_IS_NO_ONTOP)
				{
					SetOnTop(false);
				}
				if (FeatArrowRecP->IsItem & STK_BACKGROUND_IS_GRADUALCHANGED)
				{
					SetBackgroundColorChange(true);
				}
				if (FeatArrowRecP->IsItem & STK_BORDER_IS_NODISPLAY)
				{
					SetBorderDisplay(false);
				}
				SetActorPosition((StkActorPositionEnum)FeatArrowRecP->nuActorPosition);
				SetActorForm(FeatArrowRecP->fActorForm);
				SetActorLength(FeatArrowRecP->fActorLength);
				SetActorWidth(FeatArrowRecP->fActorWidth);
				SetArrowWidth(FeatArrowRecP->fArrowWidth);
				SetArrowGradient(FeatArrowRecP->fArrowGradient);
				SetBackgroundColor(FeatArrowRecP->rgbaBackground);
				SetBorderWidth(FeatArrowRecP->fBorderWidth);
				SetBorderColor(FeatArrowRecP->rgbaBorder);
				SetBorderType((StkCurveDispTypeEnum)FeatArrowRecP->nuBorderType);
			}
			else if (RecordPP[ii]->version == 2)
			{
				FeatArrowRec2P = (StkFeatArrowRec2*)RecordPP[ii];
#ifdef __MOBILE__
				SetName(Stk_Platform::STKWCHARStrToWString(FeatArrowRec2P->name));
#else
				SetName(FeatArrowRec2P->name);
#endif
				SetType((StkArrowTypeEnum)FeatArrowRec2P->nuType);
				if (FeatArrowRec2P->IsItem & STK_ARROW_IS_NO_ONTOP)
				{
					SetOnTop(false);
				}
				if (FeatArrowRec2P->IsItem & STK_BACKGROUND_IS_GRADUALCHANGED)
				{
					SetBackgroundColorChange(true);
				}
				if (FeatArrowRec2P->IsItem & STK_BORDER_IS_NODISPLAY)
				{
					SetBorderDisplay(false);
				}
				SetActorPosition((StkActorPositionEnum)FeatArrowRec2P->nuActorPosition);
				SetActorForm(FeatArrowRec2P->fActorForm);
				SetActorLength(FeatArrowRec2P->fActorLength);
				SetActorWidth(FeatArrowRec2P->fActorWidth);
				SetArrowWidth(FeatArrowRec2P->fArrowWidth);
				SetArrowGradient(FeatArrowRec2P->fArrowGradient);
				SetBackgroundColor(FeatArrowRec2P->rgbaBackground);
				SetBorderWidth(FeatArrowRec2P->fBorderWidth);
				SetBorderColor(FeatArrowRec2P->rgbaBorder);
				SetBorderType((StkCurveDispTypeEnum)FeatArrowRec2P->nuBorderType);
				SetDefPlane(FeatArrowRec2P->DefPlane);
			}
			break;
		case REC_TYPE_LINE_ARROW:
			if (RecordPP[ii]->version == 1)
			{
				FeatLineArrowRecP = (StkFeatLineArrowRec*)RecordPP[ii];
				m_pntAnchor1 = FeatLineArrowRecP->pntFirstAnchor;
				m_pntAnchor2 = FeatLineArrowRecP->pntSecondAnchor;
				m_dirX1 = FeatLineArrowRecP->dirFirstX;
				m_dirY1 = FeatLineArrowRecP->dirFirstY;
				m_dirX2 = FeatLineArrowRecP->dirSecondX;
				m_dirY2 = FeatLineArrowRecP->dirSecondY;
			}
			break;
		}
	}

	return eState;
}

/*************************************************************************//**
 *	@brief		写直线箭头特征数据
 *	@param[in]	i_stkFileP			文件对象指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS			写入成功
 *	@retval		其它				写入失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_Feat_LineArrow::WriteFile(Stk_File *i_stkFileP)
{
	STK_STATUS				eState = STK_SUCCESS;
	StkSegmentHead			head = {0};
	StkFeatLineArrowRec		FeatLineArrowRec = {0};
#ifdef _WINDOWS_
	WCHAR					strConvertLog[MAX_PATH + 512];
	Stk_SVLLog				stkSVLLog;
	Stk_LogFileEx*			stkSVLLogFile;
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
	 *	【第一步】 输出直线箭头段
	 */
	head.id = 0;
	head.kind = SEG_TYPE_FEAT_LINEARROW;
	eState = i_stkFileP->WriteSegmentHeadData(&head);
	if (eState < STK_SUCCESS)
	{
		return STK_ERROR;
	}

	// [1.1] 输出箭头记录
	Stk_Feat_Arrow::WriteFile(i_stkFileP, &head);

	// [1.2] 输出直线箭头记录
	FeatLineArrowRec.Control.type = REC_TYPE_LINE_ARROW;
	FeatLineArrowRec.Control.length = sizeof(StkFeatLineArrowRec) - sizeof(StkControl);
	FeatLineArrowRec.Control.version = _VER_FEAT_LINE_ARROW_RECORD;

	FeatLineArrowRec.pntFirstAnchor = m_pntAnchor1;
	FeatLineArrowRec.pntSecondAnchor = m_pntAnchor2;
	FeatLineArrowRec.dirFirstX = m_dirX1;
	FeatLineArrowRec.dirFirstY = m_dirY1;
	FeatLineArrowRec.dirSecondX = m_dirX2;
	FeatLineArrowRec.dirSecondY = m_dirY2;

	eState = i_stkFileP->WriteSegmentRecordData(&head, (StkControl *)&FeatLineArrowRec);
	if (eState != STK_SUCCESS)
	{
		return STK_ERROR;
	}

	// [1.3] 完成直线箭头段输出
	eState = i_stkFileP->WriteSegmentFinishData(&head);
	if (eState != STK_SUCCESS)
	{
		return STK_ERROR;
	}
	
	return eState;
}

/************************************************************************//**
 *	@brief		圆形箭头特征构造函数
 *	@param[in]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_Feat_CircularArrow::Stk_Feat_CircularArrow()
{
	m_bIsDisplayAxis = false;
	m_bIsPlaneRender = false;
	m_dirX.X = 1;
	m_dirX.Y = 0;
	m_dirX.Z = 0;
	m_dirY.X = 0;
	m_dirY.Y = 1;
	m_dirY.Z = 0;
}

/************************************************************************//**
 *	@brief		圆形箭头特征析构函数
 *	@param[in]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_Feat_CircularArrow::~Stk_Feat_CircularArrow()
{

}

/************************************************************************//**
 *	@brief		设置是否显示圆形箭头的轴
 *	@param[in]	i_bIsDisplayAxis	是否显示轴
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Feat_CircularArrow::SetDisplayAxis(STK_BOOL i_bIsDisplayAxis)
{
	m_bIsDisplayAxis = i_bIsDisplayAxis;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		是否显示圆形箭头的轴
 *	@param[in]	无
 *	@retval		是否显示轴
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_BOOL Stk_Feat_CircularArrow::IsDisplayAxis()
{
	return m_bIsDisplayAxis;
}

/************************************************************************//**
 *	@brief		设置是否显示平面渲染
 *	@param[in]	i_bIsPlaneRender	是否显示平面渲染
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Feat_CircularArrow::SetPlaneRender(STK_BOOL i_bIsPlaneRender)
{
	m_bIsPlaneRender = i_bIsPlaneRender;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		是否显示平面渲染
 *	@param[in]	无
 *	@retval		是否显示平面渲染
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_BOOL Stk_Feat_CircularArrow::IsPlaneRender()
{
	return m_bIsPlaneRender;
}

/************************************************************************//**
 *	@brief		设置半径
 *	@param[in]	i_fRadius			半径
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Feat_CircularArrow::SetRadius(STK_FLOAT32 i_fRadius)
{
	m_fRadius = i_fRadius;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取半径
 *	@param[in]	无
 *	@retval		半径
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_FLOAT32 Stk_Feat_CircularArrow::GetRadius()
{
	return m_fRadius;
}

/************************************************************************//**
 *	@brief		设置圆心
 *	@param[in]	i_pntOrigin			圆心
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Feat_CircularArrow::SetOrigin(STK_PNT32 i_pntOrigin)
{
	m_pntOrigin = i_pntOrigin;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取圆心
 *	@param[in]	无
 *	@retval		圆心
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_PNT32 Stk_Feat_CircularArrow::GetOrigin()
{
	return m_pntOrigin;
}

/************************************************************************//**
 *	@brief		设置圆心的X方向向量
 *	@param[in]	i_dirX				圆心的X方向向量
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Feat_CircularArrow::SetDirX(STK_DIR32 i_dirX)
{
	m_dirX = i_dirX;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取圆心的X方向向量
 *	@param[in]	无
 *	@retval		圆心的X方向向量
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_DIR32 Stk_Feat_CircularArrow::GetDirX()
{
	return m_dirX;
}

/************************************************************************//**
 *	@brief		设置圆心的Y方向向量
 *	@param[in]	i_dirY				圆心的Y方向向量
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Feat_CircularArrow::SetDirY(STK_DIR32 i_dirY)
{
	m_dirY = i_dirY;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取圆心的Y方向向量
 *	@param[in]	无
 *	@retval		圆心的Y方向向量
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_DIR32 Stk_Feat_CircularArrow::GetDirY()
{
	return m_dirY;
}

/************************************************************************//**
 *	@brief		设置进程
 *	@param[in]	i_nuStep			进程
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Feat_CircularArrow::SetStep(STK_UINT32 i_nuStep)
{
	m_nuStep = i_nuStep;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取进程
 *	@param[in]	无
 *	@retval		进程
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_UINT32 Stk_Feat_CircularArrow::GetStep()
{
	return m_nuStep;
}

/************************************************************************//**
 *	@brief		设置角度
 *	@param[in]	i_fAngle			角度
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Feat_CircularArrow::SetAngle(STK_FLOAT32 i_fAngle)
{
	m_fAngle = i_fAngle;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取角度
 *	@param[in]	无
 *	@retval		角度
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_FLOAT32 Stk_Feat_CircularArrow::GetAngle()
{
	return m_fAngle;
}

/************************************************************************//**
 *	@brief		读取文件中的圆形箭头特征数据
 *	@param[in]	i_FileP				文件对象指针
 *	@param[in]	i_CircularArrowHeadP直线箭头记录指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS			读取成功
 *	@retval		STK_ERROR_MEMORY	内存申请失败
 *	@retval		其他				读取失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_Feat_CircularArrow::ReadFile(Stk_File *i_FileP, StkSegmentHead *i_CircularArrowHeadP)
{
	STK_STATUS					eState = STK_SUCCESS;
	StkControl**				RecordPP = NULL;
	StkSegmentHead				head ={0};
	StkFeatArrowRec*			FeatArrowRecP = NULL;
	StkFeatArrowRec2*			FeatArrowRec2P = NULL;
	StkFeatCircularArrowRec*	FeatCircularArrowRecP = NULL;
	
	/*
	 *	读取圆形箭头特征段
	 */
	RecordPP = (StkControl **)calloc(i_CircularArrowHeadP->recnum, sizeof(StkControl*));
	if(i_CircularArrowHeadP->recnum !=0 && RecordPP == NULL)
	{
		return STK_ERROR_MEMORY;
	}
	eState = i_FileP->ReadSegmentAllRecordData(i_CircularArrowHeadP, (StkControl **)RecordPP);
	if (eState!=STK_SUCCESS)
	{
		return eState;
	}
	/*
	 *	读取圆形箭头特征段的各记录
	 */
	for (int ii = 0; ii < i_CircularArrowHeadP->recnum; ii++)
	{
		switch(RecordPP[ii]->type)
		{
		case REC_TYPE_ARROW:
			if (RecordPP[ii]->version == 1)
			{
				FeatArrowRecP = (StkFeatArrowRec*)RecordPP[ii];
#ifdef __MOBILE__
				SetName(Stk_Platform::STKWCHARStrToWString(FeatArrowRecP->name));
#else
				SetName(FeatArrowRecP->name);
#endif
				SetType((StkArrowTypeEnum)FeatArrowRecP->nuType);
				if (FeatArrowRecP->IsItem & STK_ARROW_IS_NO_ONTOP)
				{
					SetOnTop(false);
				}
				if (FeatArrowRecP->IsItem & STK_BACKGROUND_IS_GRADUALCHANGED)
				{
					SetBackgroundColorChange(true);
				}
				if (FeatArrowRecP->IsItem & STK_BORDER_IS_NODISPLAY)
				{
					SetBorderDisplay(false);
				}
				SetActorPosition((StkActorPositionEnum)FeatArrowRecP->nuActorPosition);
				SetActorForm(FeatArrowRecP->fActorForm);
				SetActorLength(FeatArrowRecP->fActorLength);
				SetActorWidth(FeatArrowRecP->fActorWidth);
				SetArrowWidth(FeatArrowRecP->fArrowWidth);
				SetArrowGradient(FeatArrowRecP->fArrowGradient);
				SetBackgroundColor(FeatArrowRecP->rgbaBackground);
				SetBorderWidth(FeatArrowRecP->fBorderWidth);
				SetBorderColor(FeatArrowRecP->rgbaBorder);
				SetBorderType((StkCurveDispTypeEnum)FeatArrowRecP->nuBorderType);
			}
			else if (RecordPP[ii]->version == 2)
			{
				FeatArrowRec2P = (StkFeatArrowRec2*)RecordPP[ii];
#ifdef __MOBILE__
				SetName(Stk_Platform::STKWCHARStrToWString(FeatArrowRec2P->name));
#else
				SetName(FeatArrowRec2P->name);
#endif
				SetType((StkArrowTypeEnum)FeatArrowRec2P->nuType);
				if (FeatArrowRec2P->IsItem & STK_ARROW_IS_NO_ONTOP)
				{
					SetOnTop(false);
				}
				if (FeatArrowRec2P->IsItem & STK_BACKGROUND_IS_GRADUALCHANGED)
				{
					SetBackgroundColorChange(true);
				}
				if (FeatArrowRec2P->IsItem & STK_BORDER_IS_NODISPLAY)
				{
					SetBorderDisplay(false);
				}
				SetActorPosition((StkActorPositionEnum)FeatArrowRec2P->nuActorPosition);
				SetActorForm(FeatArrowRec2P->fActorForm);
				SetActorLength(FeatArrowRec2P->fActorLength);
				SetActorWidth(FeatArrowRec2P->fActorWidth);
				SetArrowWidth(FeatArrowRec2P->fArrowWidth);
				SetArrowGradient(FeatArrowRec2P->fArrowGradient);
				SetBackgroundColor(FeatArrowRec2P->rgbaBackground);
				SetBorderWidth(FeatArrowRec2P->fBorderWidth);
				SetBorderColor(FeatArrowRec2P->rgbaBorder);
				SetBorderType((StkCurveDispTypeEnum)FeatArrowRec2P->nuBorderType);
				SetDefPlane(FeatArrowRec2P->DefPlane);
			}
			break;
		case REC_TYPE_CIRCULAR_ARROW:
			if (RecordPP[ii]->version == 1)
			{
				FeatCircularArrowRecP = (StkFeatCircularArrowRec*)RecordPP[ii];
				if (FeatCircularArrowRecP->IsItem & STK_AXIS_IS_DISPLAY)
				{
					m_bIsDisplayAxis = true;
				}
				if (FeatCircularArrowRecP->IsItem & STK_PLANE_IS_RENDER)
				{
					m_bIsPlaneRender = true;
				}
				m_fRadius = FeatCircularArrowRecP->fRadius;
				m_pntOrigin = FeatCircularArrowRecP->pntOrigin;
				m_dirX = FeatCircularArrowRecP->dirX;
				m_dirY = FeatCircularArrowRecP->dirY;
				m_nuStep = FeatCircularArrowRecP->nuStep;
				m_fAngle = FeatCircularArrowRecP->fAngle;
			}
			break;
		}
	}

	return eState;
}

/*************************************************************************//**
 *	@brief		写圆形箭头特征数据
 *	@param[in]	i_stkFileP			文件对象指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS			写入成功
 *	@retval		其它				写入失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_Feat_CircularArrow::WriteFile(Stk_File *i_stkFileP)
{
	STK_STATUS				eState = STK_SUCCESS;
	StkSegmentHead			head = {0};
	StkFeatCircularArrowRec	FeatCircularArrowRec = {0};
#ifdef _WINDOWS_
	WCHAR					strConvertLog[MAX_PATH + 512];
	Stk_SVLLog				stkSVLLog;
	Stk_LogFileEx*			stkSVLLogFile;
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
	 *	【第一步】输出圆形箭头特征段
	 */
	head.id = 0;
	head.kind = SEG_TYPE_FEAT_CIRCULARARROW;

	eState = i_stkFileP->WriteSegmentHeadData(&head);
	if (eState < STK_SUCCESS)
	{
		return STK_ERROR;
	}

	// 【1.1】输出箭头段头记录
	Stk_Feat_Arrow::WriteFile(i_stkFileP, &head);

	// 【1.2】输出圆形箭头段头记录
	FeatCircularArrowRec.Control.type = REC_TYPE_CIRCULAR_ARROW;
	FeatCircularArrowRec.Control.length = sizeof(StkFeatCircularArrowRec) - sizeof(StkControl);
	FeatCircularArrowRec.Control.version = _VER_FEAT_MAGNETICLINE_RECORD;

	if (m_bIsDisplayAxis)
	{
		FeatCircularArrowRec.IsItem |= STK_AXIS_IS_DISPLAY;
	}
	if (m_bIsPlaneRender)
	{
		FeatCircularArrowRec.IsItem |= STK_PLANE_IS_RENDER;
	}

	FeatCircularArrowRec.fRadius = m_fRadius;
	FeatCircularArrowRec.pntOrigin = m_pntOrigin;
	FeatCircularArrowRec.dirX = m_dirX;
	FeatCircularArrowRec.dirY = m_dirY;
	FeatCircularArrowRec.fAngle = m_fAngle;
	FeatCircularArrowRec.nuStep = m_nuStep;

	eState = i_stkFileP->WriteSegmentRecordData(&head, (StkControl *)&FeatCircularArrowRec);
	if(eState < STK_SUCCESS)
	{
		return STK_ERROR;
	}

	// 【1.3】 完成圆形箭头特征段的输出
	eState = i_stkFileP->WriteSegmentFinishData(&head);
	if(eState < STK_SUCCESS)
	{
		return STK_ERROR;
	}

	return eState;
}

/************************************************************************//**
 *	@brief		直角箭头特征构造函数
 *	@param[in]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_Feat_RightAngleArrow::Stk_Feat_RightAngleArrow()
{
	m_dirX.X = 1;
	m_dirX.Y = 0;
	m_dirX.Z = 0;
	m_dirY.X = 0;
	m_dirY.Y = 1;
	m_dirY.Z = 0;
}

/************************************************************************//**
 *	@brief		直角箭头特征析构函数
 *	@param[in]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_Feat_RightAngleArrow::~Stk_Feat_RightAngleArrow()
{

}

/************************************************************************//**
 *	@brief		设置直角箭头的长度1
 *	@param[in]	i_fFirstLength		直角箭头长度1
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Feat_RightAngleArrow::SetFirstLength(STK_FLOAT32 i_fFirstLength)
{
	m_fFirstLength = i_fFirstLength;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取直角箭头的长度1
 *	@param[in]	i_fFirstLength		直角箭头长度1
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_FLOAT32 Stk_Feat_RightAngleArrow::GetFirstLength()
{
	return m_fFirstLength;
}

/************************************************************************//**
 *	@brief		设置直角箭头的长度2
 *	@param[in]	i_fSecondLength		直角箭头长度2
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Feat_RightAngleArrow::SetSecondLength(STK_FLOAT32 i_fSecondLength)
{
	m_fSecondLength = i_fSecondLength;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取直角箭头的长度2
 *	@param[in]	i_fSecondLength		直角箭头长度2
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_FLOAT32 Stk_Feat_RightAngleArrow::GetSecondLength()
{
	return m_fSecondLength;
}

/************************************************************************//**
 *	@brief		设置直角箭头的原点
 *	@param[in]	i_pntOrigin			直角箭头原点
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Feat_RightAngleArrow::SetOrigin(STK_PNT32 i_pntOrigin)
{
	m_pntOrigin = i_pntOrigin;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取直角箭头的原点
 *	@param[in]	i_fSecondLength		直角箭头原点
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_PNT32 Stk_Feat_RightAngleArrow::GetOrigin()
{
	return m_pntOrigin;
}

/************************************************************************//**
 *	@brief		设置直角的X方向向量
 *	@param[in]	i_dirX				圆心的X方向向量
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Feat_RightAngleArrow::SetDirX(STK_DIR32 i_dirX)
{
	m_dirX = i_dirX;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取直角的X方向向量
 *	@param[in]	无
 *	@retval		圆心的X方向向量
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_DIR32 Stk_Feat_RightAngleArrow::GetDirX()
{
	return m_dirX;
}

/************************************************************************//**
 *	@brief		设置直角的Y方向向量
 *	@param[in]	i_dirY				圆心的Y方向向量
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Feat_RightAngleArrow::SetDirY(STK_DIR32 i_dirY)
{
	m_dirY = i_dirY;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取直角的Y方向向量
 *	@param[in]	无
 *	@retval		圆心的Y方向向量
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_DIR32 Stk_Feat_RightAngleArrow::GetDirY()
{
	return m_dirY;
}

/************************************************************************//**
 *	@brief		读取文件中的直角箭头特征数据
 *	@param[in]	i_FileP					文件对象指针
 *	@param[in]	i_RightAngleArrowHeadP	直角箭头记录指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS			读取成功
 *	@retval		STK_ERROR_MEMORY	内存申请失败
 *	@retval		其他				读取失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_Feat_RightAngleArrow::ReadFile(Stk_File *i_FileP, StkSegmentHead *i_RightAngleArrowHeadP)
{
	STK_STATUS					eState = STK_SUCCESS;
	StkControl**				RecordPP = NULL;
	StkSegmentHead				head ={0};
	StkFeatArrowRec*			FeatArrowRecP = NULL;
	StkFeatArrowRec2*			FeatArrowRec2P = NULL;
	StkFeatRightAngleArrowRec*	FeatRightAngleArrowRecP = NULL;
	
	/*
	 *	读取直角箭头特征段
	 */
	RecordPP = (StkControl **)calloc(i_RightAngleArrowHeadP->recnum, sizeof(StkControl*));
	if(i_RightAngleArrowHeadP->recnum !=0 && RecordPP == NULL)
	{
		return STK_ERROR_MEMORY;
	}
	eState = i_FileP->ReadSegmentAllRecordData(i_RightAngleArrowHeadP, (StkControl **)RecordPP);
	if (eState!=STK_SUCCESS)
	{
		return eState;
	}
	/*
	 *	读取直角箭头特征段的各记录
	 */
	for (int ii = 0; ii < i_RightAngleArrowHeadP->recnum; ii++)
	{
		switch(RecordPP[ii]->type)
		{
		case REC_TYPE_ARROW:
			if (RecordPP[ii]->version == 1)
			{
				FeatArrowRecP = (StkFeatArrowRec*)RecordPP[ii];
#ifdef __MOBILE__
				SetName(Stk_Platform::STKWCHARStrToWString(FeatArrowRecP->name));
#else
				SetName(FeatArrowRecP->name);
#endif
				SetType((StkArrowTypeEnum)FeatArrowRecP->nuType);
				if (FeatArrowRecP->IsItem & STK_ARROW_IS_NO_ONTOP)
				{
					SetOnTop(false);
				}
				if (FeatArrowRecP->IsItem & STK_BACKGROUND_IS_GRADUALCHANGED)
				{
					SetBackgroundColorChange(true);
				}
				if (FeatArrowRecP->IsItem & STK_BORDER_IS_NODISPLAY)
				{
					SetBorderDisplay(false);
				}
				SetActorPosition((StkActorPositionEnum)FeatArrowRecP->nuActorPosition);
				SetActorForm(FeatArrowRecP->fActorForm);
				SetActorLength(FeatArrowRecP->fActorLength);
				SetActorWidth(FeatArrowRecP->fActorWidth);
				SetArrowWidth(FeatArrowRecP->fArrowWidth);
				SetArrowGradient(FeatArrowRecP->fArrowGradient);
				SetBackgroundColor(FeatArrowRecP->rgbaBackground);
				SetBorderWidth(FeatArrowRecP->fBorderWidth);
				SetBorderColor(FeatArrowRecP->rgbaBorder);
				SetBorderType((StkCurveDispTypeEnum)FeatArrowRecP->nuBorderType);
			}
			else if (RecordPP[ii]->version == 2)
			{
				FeatArrowRec2P = (StkFeatArrowRec2*)RecordPP[ii];
#ifdef __MOBILE__
				SetName(Stk_Platform::STKWCHARStrToWString(FeatArrowRec2P->name));
#else
				SetName(FeatArrowRec2P->name);
#endif
				SetType((StkArrowTypeEnum)FeatArrowRec2P->nuType);
				if (FeatArrowRec2P->IsItem & STK_ARROW_IS_NO_ONTOP)
				{
					SetOnTop(false);
				}
				if (FeatArrowRec2P->IsItem & STK_BACKGROUND_IS_GRADUALCHANGED)
				{
					SetBackgroundColorChange(true);
				}
				if (FeatArrowRec2P->IsItem & STK_BORDER_IS_NODISPLAY)
				{
					SetBorderDisplay(false);
				}
				SetActorPosition((StkActorPositionEnum)FeatArrowRec2P->nuActorPosition);
				SetActorForm(FeatArrowRec2P->fActorForm);
				SetActorLength(FeatArrowRec2P->fActorLength);
				SetActorWidth(FeatArrowRec2P->fActorWidth);
				SetArrowWidth(FeatArrowRec2P->fArrowWidth);
				SetArrowGradient(FeatArrowRec2P->fArrowGradient);
				SetBackgroundColor(FeatArrowRec2P->rgbaBackground);
				SetBorderWidth(FeatArrowRec2P->fBorderWidth);
				SetBorderColor(FeatArrowRec2P->rgbaBorder);
				SetBorderType((StkCurveDispTypeEnum)FeatArrowRec2P->nuBorderType);
				SetDefPlane(FeatArrowRec2P->DefPlane);
			}
			break;
		case REC_TYPE_RIGHTANGLE_ARROW:
			if (RecordPP[ii]->version == 1)
			{
				FeatRightAngleArrowRecP = (StkFeatRightAngleArrowRec*)RecordPP[ii];
				m_fFirstLength = FeatRightAngleArrowRecP->fFirstLength;
				m_fSecondLength = FeatRightAngleArrowRecP->fSecondLength;
				m_pntOrigin = FeatRightAngleArrowRecP->pntOrigin;
				m_dirX = FeatRightAngleArrowRecP->dirX;
				m_dirY = FeatRightAngleArrowRecP->dirY;
			}
			break;
		}
	}

	return eState;
}

/*************************************************************************//**
 *	@brief		写直角箭头特征数据
 *	@param[in]	i_stkFileP			文件对象指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS			写入成功
 *	@retval		其它				写入失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_Feat_RightAngleArrow::WriteFile(Stk_File *i_stkFileP)
{
	STK_STATUS					eState = STK_SUCCESS;
	StkSegmentHead				head = {0};
	StkFeatRightAngleArrowRec	FeatRightAngleRec = {0};
#ifdef _WINDOWS_
	WCHAR					strConvertLog[MAX_PATH + 512];
	Stk_SVLLog				stkSVLLog;
	Stk_LogFileEx*			stkSVLLogFile;
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
	 *	【第一步】 输出直角箭头段
	 */
	head.id = 0;
	head.kind = SEG_TYPE_FEAT_RIGHTANGLEARROW;

	eState = i_stkFileP->WriteSegmentHeadData(&head);
	if (eState < STK_SUCCESS)
	{
		return STK_ERROR;
	}
	// [1.1] 输出箭头记录
	Stk_Feat_Arrow::WriteFile(i_stkFileP, &head);

	// [1.2] 输出直角箭头记录
	FeatRightAngleRec.Control.type = REC_TYPE_RIGHTANGLE_ARROW;
	FeatRightAngleRec.Control.length = sizeof(StkFeatRightAngleArrowRec) - sizeof(StkControl);
	FeatRightAngleRec.Control.version = _VER_FEAT_RIGHTANGLE_ARROW_RECORD;

	FeatRightAngleRec.fFirstLength = m_fFirstLength;
	FeatRightAngleRec.fSecondLength = m_fSecondLength;
	FeatRightAngleRec.pntOrigin = m_pntOrigin;
	FeatRightAngleRec.dirX = m_dirX;
	FeatRightAngleRec.dirY = m_dirY;

	eState = i_stkFileP->WriteSegmentRecordData(&head, (StkControl*)&FeatRightAngleRec);
	if (eState != STK_SUCCESS)
	{
		return STK_ERROR;
	}
	
	// [1.3] 完成直角箭头段的输出
	eState = i_stkFileP->WriteSegmentFinishData(&head);
	if (eState != STK_SUCCESS)
	{
		return STK_ERROR;
	}
	
	return eState;
}