// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_Texture.cpp
 *
 *	@brief	纹理类成员函数的实现
 *
 *	@par	历史:
 *		2015/09/08	创建。WangY@HOTEAMSOFT
****************************************************************************/

#ifndef __MOBILE__
#pragma once
#endif

#include "StdAfx.h"
#include "Stk_Texture.h"
#include "Stk_Platform.h"

/************************************************************************//**
 *	@brief		Stk_Texture构造函数
 *	@param[in]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_Texture::Stk_Texture()
{
	m_eMappingType = MAPPINT_TYPE_STORED_UV;
	m_fRotateAngle = 0;
	m_stkPictureP = NULL;
	m_fUOffset = 0;
	m_fVOffset = 0;
	m_fUScale = 0;
	m_fVScale = 0;
	m_bUFlipped = false;
	m_bVFlipped = false;
}

/************************************************************************//**
 *	@brief		Stk_Texture析构函数
 *	@param[in]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_Texture::~Stk_Texture()
{
	if (m_stkPictureP != NULL)
	{
		delete m_stkPictureP;
		m_stkPictureP = NULL;
	}
}

/************************************************************************//**
 *	@brief		设置纹理映射类型
 *	@param[in]	i_eMappingType		纹理映射类型
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@retval		其它				设置失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Texture::SetType(StkTextureMappingTypeEnum i_eMappingType)
{
	m_eMappingType = i_eMappingType;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取纹理映射类型
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		纹理映射类型
 *	@note		无
 *	@attention	无
 ****************************************************************************/
StkTextureMappingTypeEnum Stk_Texture::GetType()
{
	return m_eMappingType;
}

/************************************************************************//**
 *	@brief		设置旋转角度
 *	@param[in]	i_fRotateAngle		旋转角度
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@retval		其它				设置失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Texture::SetRotateAngle(STK_FLOAT32 i_fRotateAngle)
{
	m_fRotateAngle = i_fRotateAngle;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取旋转角度
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		旋转角度
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_FLOAT32 Stk_Texture::GetRotateAngle()
{
	return m_fRotateAngle;
}

/************************************************************************//**
 *	@brief		设置纹理图片
 *	@param[in]	i_stkPictureP		纹理图片
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@retval		其它				设置失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Texture::SetTextureImage(Stk_Picture* i_stkPictureP)
{
	m_stkPictureP = i_stkPictureP;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取纹理图片
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		纹理图片
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_Picture* Stk_Texture::GetTextureImage()
{
	return m_stkPictureP;
}

/************************************************************************//**
 *	@brief		设置U方向偏置距离
 *	@param[in]	i_fUOffset			U方向偏置距离
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@retval		其它				设置失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Texture::SetUOffset(STK_FLOAT32 i_fUOffset)
{
	m_fUOffset = i_fUOffset;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取U方向偏置距离
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		U方向偏置距离
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_FLOAT32 Stk_Texture::GetUOffset()
{
	return m_fUOffset;
}

/************************************************************************//**
 *	@brief		设置V方向偏置距离
 *	@param[in]	i_fUOffset			V方向偏置距离
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@retval		其它				设置失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Texture::SetVOffset(STK_FLOAT32 i_fVOffset)
{
	m_fVOffset = i_fVOffset;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取V方向偏置距离
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		V方向偏置距离
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_FLOAT32 Stk_Texture::GetVOffset()
{
	return m_fVOffset;
}

/************************************************************************//**
 *	@brief		设置U方向缩放因子
 *	@param[in]	i_fScale			U方向缩放因子
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@retval		其它				设置失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Texture::SetUScale(STK_FLOAT32 i_fScale)
{
	m_fUScale = i_fScale;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取U方向缩放因子
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		U方向缩放因子
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_FLOAT32 Stk_Texture::GetUScale()
{
	return m_fUScale;
}

/************************************************************************//**
 *	@brief		设置V方向缩放因子
 *	@param[in]	i_fScale			V方向缩放因子
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@retval		其它				设置失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Texture::SetVScale(STK_FLOAT32 i_fScale)
{
	m_fVScale = i_fScale;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取V方向缩放因子
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		V方向缩放因子
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_FLOAT32 Stk_Texture::GetVScale()
{
	return m_fVScale;
}

/************************************************************************//**
 *	@brief		设置Flip标识
 *	@param[in]	i_bUFlip			U方向Flip
 *	@param[in]	i_bVFlip			V方向Flip
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@retval		其它				设置失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Texture::SetFlipFlag(STK_BOOL i_bUFlip, STK_BOOL i_bVFlip)
{
	m_bUFlipped = i_bUFlip;
	m_bVFlipped = i_bVFlip;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		U方向Flip标识
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		U方向Flip标识
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_BOOL Stk_Texture::IsUFlipped()
{
	return m_bUFlipped;
}

/************************************************************************//**
 *	@brief		V方向Flip标识
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		V方向Flip标识
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_BOOL Stk_Texture::IsVFlipped()
{
	return m_bVFlipped;
}

/************************************************************************//**
 *	@brief		读取纹理文件
 *	@param[in]	i_stkFileP			文件指针
 *	@param[in]	i_TextureHeadSP		纹理段头
 *	@param[out]	无
 *	@retval		STK_SUCCESS			读取成功
 *	@retval		STK_SUCCESS_EXIST	已读取跳出
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_Texture::ReadFile(Stk_File *i_stkFileP, StkSegmentHead *i_TextureHeadSP)
{
	STK_STATUS		eState = STK_SUCCESS;
	StkControl**	RecordPP = NULL;
	StkTextureRec*	stkTextureRecP = NULL;
	StkPictureRec3*	stkPictureRec3P = NULL;
	Stk_Picture*	stkPictureP = NULL;

	/*
	 *	读取纹理段的数据
	 */
	RecordPP = (StkControl **)calloc(i_TextureHeadSP->recnum, sizeof(StkControl*));
	if (i_TextureHeadSP->recnum != 0 && RecordPP == NULL)
	{
		return STK_ERROR_MEMORY;
	}
	eState = i_stkFileP->ReadSegmentAllRecordData(i_TextureHeadSP, (StkControl **)RecordPP);
	if (eState != STK_SUCCESS)
	{
		return eState;
	}

	/*
	 *	读取纹理段的各记录
	 */
	for (int ii = 0; ii < i_TextureHeadSP->recnum; ii++)
	{
		switch(RecordPP[ii]->type)
		{
		case REC_TYPE_TEXTURE:
			if (RecordPP[ii]->version == 1)
			{
				stkTextureRecP = (StkTextureRec*)RecordPP[ii];
				m_eMappingType = (StkTextureMappingTypeEnum)stkTextureRecP->nuMappingType;
				m_fRotateAngle = stkTextureRecP->fRotateAngle;
				m_fUOffset = stkTextureRecP->fUOffset;
				m_fVOffset = stkTextureRecP->fVOffset;
				m_fUScale = stkTextureRecP->fUScale;
				m_fVScale = stkTextureRecP->fVScale;
				if (stkTextureRecP->IsItem & STK_TEXTURE_IS_UFLIPPED)
				{
					m_bUFlipped = true;
				}
				if (stkTextureRecP->IsItem & STK_TEXTURE_IS_VFLIPPED)
				{
					m_bVFlipped = true;
				}
			}
			break;
		case REC_TYPE_PICTURE:
			if (RecordPP[ii]->version == 3)
			{
				stkPictureRec3P = (StkPictureRec3*)RecordPP[ii];
				stkPictureP = new Stk_Picture();
				stkPictureP->SetType((StkPictureTypeEnum)stkPictureRec3P->nuType);
				stkPictureP->SetLocation(stkPictureRec3P->pntLocation);
				stkPictureP->SetDisplayWidth(stkPictureRec3P->fDispalyWidth);
				stkPictureP->SetDisplayHeight(stkPictureRec3P->fDispalyHeight);
				stkPictureP->SetActualWidth(stkPictureRec3P->fActualWidth);
				stkPictureP->SetActualHeight(stkPictureRec3P->fActualHeight);
				stkPictureP->SetPictureSaveStyle((StkPicSaveStyleEnum)stkPictureRec3P->nuSaveStyle);
				stkPictureP->SetNC(stkPictureRec3P->nNC);
				stkPictureP->SetScaleX(stkPictureRec3P->fScaleX);
				stkPictureP->SetScaleY(stkPictureRec3P->fScaleY);
#ifdef __MOBILE__
				stkPictureP->SetPictureSavePath(Stk_Platform::STKWCHARStrToWString(stkPictureRec3P->wchPicturePath));
#else
				stkPictureP->SetPictureSavePath(stkPictureRec3P->wchPicturePath);
#endif
				stkPictureP->SetData(stkPictureRec3P->nudataSize, stkPictureRec3P->atr);
				m_stkPictureP = stkPictureP;
			}

			break;
		}
	}

	// 释放内存
	eState = i_stkFileP->ReadSegmentFinishData(i_TextureHeadSP, (StkControl **)RecordPP);
	if (RecordPP != NULL)
	{
		free(RecordPP);
		RecordPP = NULL;
	}
	if (eState!=STK_SUCCESS)
	{
		return eState;
	}

	return eState;
}

/************************************************************************//**
 *	@brief		写纹理文件
 *	@param[in]	i_stkFileP		文件指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS		写入成功
 *	@retval		其他			写入失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_Texture::WriteFile(Stk_File *i_stkFileP)
{
	STK_STATUS			eState = STK_SUCCESS;
	StkSegmentHead		TextureHead = {0};
	StkTextureRec		stkTextureRec = {0};
	
	if(i_stkFileP == NULL)
	{
		return STK_ERROR;
	}

	/*
	 * 【第1步】 输出纹理段
	 */

	// 【1.1 输出段头记录】
	TextureHead.id = 0;
	TextureHead.kind = SEG_TYPE_TEXTURE;
	eState = i_stkFileP->WriteSegmentHeadData(&TextureHead);
	if(eState < STK_SUCCESS)
	{
		return STK_ERROR;
	}

	//	【1.2 输出纹理记录】
	stkTextureRec.Control.type = REC_TYPE_TEXTURE;
	stkTextureRec.Control.version = _VER_TEXTURE_RECORD; /*1*/
	stkTextureRec.Control.length = sizeof(StkTextureRec) - sizeof(StkControl);

	stkTextureRec.nuMappingType = m_eMappingType;
	stkTextureRec.fRotateAngle = m_fRotateAngle;
	stkTextureRec.fUOffset = m_fUOffset;
	stkTextureRec.fVOffset = m_fVOffset;
	stkTextureRec.fUScale = m_fUScale;
	stkTextureRec.fVScale = m_fVScale;
	if (m_bUFlipped)
	{
		stkTextureRec.IsItem |= STK_TEXTURE_IS_UFLIPPED;
	}
	if (m_bVFlipped)
	{
		stkTextureRec.IsItem |= STK_TEXTURE_IS_VFLIPPED;
	}

	eState = i_stkFileP->WriteSegmentRecordData(&TextureHead, (StkControl*)&stkTextureRec);
	if(eState < STK_SUCCESS)
	{
		return STK_ERROR;
	}

	//	【1.3 输出图片记录】
	if (m_stkPictureP != NULL)
	{
		eState = m_stkPictureP->WriteFile(&TextureHead, i_stkFileP);
		if(eState < STK_SUCCESS)
		{
			return STK_ERROR;
		}
	}

	// 【1.4 完成纹理段的输出】
	eState = i_stkFileP->WriteSegmentFinishData(&TextureHead);
	if( eState < STK_SUCCESS)
	{
		return STK_ERROR;
	}

	return eState;
}