// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_Picture.cpp
 *
 *	@brief	图片类成员函数的实现
 *
 *	@par	历史:
 *		2015/05/26	创建。WangY@HOTEAMSOFT
****************************************************************************/

#ifndef __MOBILE__
#pragma once
#endif

#include "StdAfx.h"
#include "Stk_Define.h"
#include "Stk_Picture.h"
#include "Stk_Platform.h"
#include "Stk_Enum.h"

/************************************************************************//**
 *	@brief		Stk_Picture构造函数
 *	@param[in]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_Picture::Stk_Picture()
{
	m_eType = PICTURE_TYPE_UNKNOWN;
	m_pntLocation.X = 0;
	m_pntLocation.Y = 0;
	m_pntLocation.Z = 0;
	m_fDisPlayWidth = 0;
	m_fDisplayHeight = 0;
	m_fActualWidth = 0;
	m_fActualHeight = 0;
	m_eSaveStyle = PICTRE_SAVE_STYLE_OUTER;
	m_nNC = 0;
	m_wstrPath = L"";
	m_nudataSize = 0;
	m_dataP = NULL;
}

/************************************************************************//**
 *	@brief		Stk_Picture析构函数
 *	@param[in]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_Picture::~Stk_Picture()
{
	if (m_dataP != NULL)
	{
		delete m_dataP;
		m_dataP = NULL;
	}
}

/************************************************************************//**
 *	@brief		设置图片类型
 *	@param[in]	i_eType				图片类型
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Picture::SetType(StkPictureTypeEnum i_eType)
{
	m_eType = i_eType;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取图片类型
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		图片类型
 *	@note		无
 *	@attention	无
 ****************************************************************************/
StkPictureTypeEnum Stk_Picture::GetType()
{
	return m_eType;
}

/************************************************************************//**
 *	@brief		设置图片位置
 *	@param[in]	i_pntLocation		图片位置
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Picture::SetLocation(STK_PNT32 i_pntLocation)
{
	m_pntLocation = i_pntLocation;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取图片位置
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		图片位置
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_PNT32 Stk_Picture::GetLocation()
{
	return m_pntLocation;
}

/************************************************************************//**
 *	@brief		设置图片场景图显示的宽度
 *	@param[in]	i_fDisplayWidth		图片场景图显示的宽度
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Picture::SetDisplayWidth(STK_FLOAT32 i_fDisplayWidth)
{
	m_fDisPlayWidth = i_fDisplayWidth;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取图片场景图显示的宽度
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		图片场景图显示的宽度
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_FLOAT32 Stk_Picture::GetDisplayWidth()
{
	return m_fDisPlayWidth;
}

/************************************************************************//**
 *	@brief		设置图片场景图显示的高度
 *	@param[in]	i_fDisplayHeight	图片场景图显示的高度
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Picture::SetDisplayHeight(STK_FLOAT32 i_fDisplayHeight)
{
	m_fDisplayHeight = i_fDisplayHeight;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取图片场景图显示的高度
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		图片场景图显示的高度
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_FLOAT32 Stk_Picture::GetDisplayHeight()
{
	return m_fDisplayHeight;
}

/************************************************************************//**
 *	@brief		设置图片保存方式
 *	@param[in]	i_eSaveStyle		图片保存方式：SVL外部/内部
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Picture::SetPictureSaveStyle(StkPicSaveStyleEnum i_eSaveStyle)
{
	m_eSaveStyle = i_eSaveStyle;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取图片保存方式
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		图片保存方式：SVL外部/内部
 *	@note		无
 *	@attention	无
 ****************************************************************************/
StkPicSaveStyleEnum Stk_Picture::GetPictureSaveStyle()
{
	return m_eSaveStyle;
}

/************************************************************************//**
 *	@brief		设置图片的保存路径
 *	@param[in]	i_wstrSavePath		图片的保存路径
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Picture::SetPictureSavePath(wstring i_wstrSavePath)
{
	m_wstrPath = i_wstrSavePath;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取图片的保存路径
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		图片的保存路径
 *	@note		无
 *	@attention	无
 ****************************************************************************/
wstring Stk_Picture::GetPictureSavePath()
{
	return m_wstrPath;
}

/************************************************************************//**
 *	@brief		设置图片的数据流
 *	@param[in]	i_nudataSize		图片的数据流长度
 *	@param[in]	i_chDataP			图片的数据流
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Picture::SetData(STK_UINT32 i_nudataSize, STK_CHAR* i_chDataP)
{
	if ((i_nudataSize == 0 && i_chDataP != NULL) ||
		(i_nudataSize != 0) && i_chDataP == NULL)
	{
		return STK_ERROR;
	}
	m_nudataSize = i_nudataSize;

	if (i_nudataSize > 0)
	{
		m_dataP = (STK_CHAR *)malloc(i_nudataSize);
		if (m_dataP == NULL)
		{
			return STK_ERROR;
		}
		memcpy(m_dataP, i_chDataP, i_nudataSize);
	}

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取图片的数据流
 *	@param[in]	无
 *	@param[out]	o_nudataSize		图片的数据流长度
 *	@param[out]	o_chDataP			图片的数据流
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Picture::GetData(STK_UINT32& o_nudataSize, STK_CHAR*& o_chDataP)
{
	o_nudataSize = m_nudataSize;

	if (m_nudataSize > 0)
	{
		o_chDataP = (STK_CHAR *)malloc(m_nudataSize);
		if (m_dataP == NULL)
		{
			return STK_ERROR;
		}
		memcpy(o_chDataP, m_dataP, m_nudataSize);
	}

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		设置X方向缩放因子
 *	@param[in]	i_fScaleX			X方向缩放因子
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Picture::SetScaleX(STK_FLOAT32 i_fScaleX)
{
	m_fScaleX = i_fScaleX;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取X方向缩放因子
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		X方向缩放因子
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_FLOAT32 Stk_Picture::GetScaleX()
{
	return m_fScaleX;
}

/************************************************************************//**
 *	@brief		设置Y方向缩放因子
 *	@param[in]	i_fScaleY			Y方向缩放因子
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Picture::SetScaleY(STK_FLOAT32 i_fScaleY)
{
	m_fScaleY = i_fScaleY;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取Y方向缩放因子
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		Y方向缩放因子
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_FLOAT32 Stk_Picture::GetScaleY()
{
	return m_fScaleY;
}

/*************************************************************************//**
 *	@brief		写图片数据
 *	@param[in]	i_head			数据段头指针
 *	@param[in]	i_stkFileP		文件对象指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS		写入成功
 *	@retval		其它			写入失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_Picture::WriteFile(StkSegmentHead* i_head, Stk_File* i_stkFileP)
{
	STK_STATUS		eState = STK_SUCCESS;
	StkPictureRec3*	stkPictureRecP = NULL;

	if(i_stkFileP == NULL || i_head == NULL)
	{
		return STK_ERROR;
	}
	
	stkPictureRecP = (StkPictureRec3 *)calloc(1, sizeof(StkPictureRec3) - sizeof(STK_CHAR) + m_nudataSize * sizeof(STK_CHAR));
	if(stkPictureRecP == NULL)
	{
		return STK_ERROR;
	}
	/*
	 *	属性填充
	 */
	stkPictureRecP->Control.type = REC_TYPE_PICTURE;
	stkPictureRecP->Control.length = sizeof(StkPictureRec3) - sizeof(STK_CHAR) + m_nudataSize * sizeof(STK_CHAR) - sizeof(StkControl);
	stkPictureRecP->Control.version = _VER_PICTURE_RECORD;

	stkPictureRecP->nuType = m_eType;
	stkPictureRecP->pntLocation = m_pntLocation;
	stkPictureRecP->fDispalyWidth = m_fDisPlayWidth;
	stkPictureRecP->fDispalyHeight = m_fDisplayHeight;
	stkPictureRecP->fActualWidth = m_fActualWidth;
	stkPictureRecP->fActualHeight = m_fActualHeight;
	stkPictureRecP->nuSaveStyle = m_eSaveStyle;
	stkPictureRecP->nNC = m_nNC;
	stkPictureRecP->fScaleX = m_fScaleX;
	stkPictureRecP->fScaleY = m_fScaleY;
#ifdef __MOBILE__
	memcpy(stkPictureRecP->wchPicturePath,Stk_Platform::WStringToSTKWCHARStr(m_wstrPath),(m_wstrPath.size()+1)*sizeof(STK_WCHAR));
#else
	wcscpy_s(stkPictureRecP->wchPicturePath, STK_MAXPATH, (WCHAR*)m_wstrPath.c_str());
#endif
	stkPictureRecP->nudataSize = m_nudataSize;
	memcpy(stkPictureRecP->atr, m_dataP, m_nudataSize);

	eState = i_stkFileP->WriteSegmentRecordData(i_head, (StkControl *)stkPictureRecP);
	if(eState < STK_SUCCESS)
	{
		return STK_ERROR;
	}

	if(stkPictureRecP != NULL)
	{
		free(stkPictureRecP);
		stkPictureRecP = NULL;
	}
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		设置图片实际的宽度
 *	@param[in]	i_fActualWidth		图片实际的宽度
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Picture::SetActualWidth(STK_FLOAT32 i_fActualWidth)
{
	m_fActualWidth = i_fActualWidth;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取图片实际的宽度
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		图片实际的宽度
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_FLOAT32 Stk_Picture::GetActualWidth()
{
	return m_fActualWidth;
}

/************************************************************************//**
 *	@brief		设置图片实际的高度
 *	@param[in]	i_fActualHeight		图片实际的高度
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Picture::SetActualHeight(STK_FLOAT32 i_fActualHeight)
{
	m_fActualHeight = i_fActualHeight;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取图片实际的高度
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		图片场景图显示的高度
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_FLOAT32 Stk_Picture::GetActualHeight()
{
	return m_fActualHeight;
}

/************************************************************************//**
 *	@brief		设置图片位图NC
 *	@param[in]	i_nNC				位图NC
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Picture::SetNC(STK_INT32 i_nNC)
{
	m_nNC = i_nNC;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取图片位图NC
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		位图NC
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_INT32 Stk_Picture::GetNC()
{
	return m_nNC;
}