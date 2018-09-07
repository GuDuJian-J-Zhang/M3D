// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_Feat_MangeticLine.cpp
 *
 *	@brief	磁力线特征类的实现
 *
 *	@par	历史:
 *		2015/04/08	创建。WangY@HOTEAMSOFT
****************************************************************************/

#ifndef __MOBILE__
#pragma once
#endif

#include "StdAfx.h"
#include "Stk_Feat_MagneticLine.h"

/************************************************************************//**
 *	@brief		磁力线特征构造函数
 *	@param[in]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_Feat_MagneticLine::Stk_Feat_MagneticLine()
{
	m_pntStart.X = 0;
	m_pntStart.Y = 0;
	m_pntStart.Z = 0;
	m_pntEnd.X = 0;
	m_pntEnd.Y = 0;
	m_pntEnd.Z = 0;
	m_bEffective = true;
	m_eArrangement = TYPE_ARRANGEMENT_EQUAL;
}

/************************************************************************//**
 *	@brief		磁力线特征析构函数
 *	@param[in]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_Feat_MagneticLine::~Stk_Feat_MagneticLine()
{

}

/************************************************************************//**
 *	@brief		设置磁力线起始点
 *	@param[in]	i_pntStart	起始点
 *	@param[out]	无
 *	@retval		STK_SUCCESS	设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Feat_MagneticLine::SetStartPoint(STK_PNT32& i_pntStart)
{
	m_pntStart = i_pntStart;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取磁力线起始点
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		起始点
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_PNT32 Stk_Feat_MagneticLine::GetStartPoint()
{
	return m_pntStart;
}

/************************************************************************//**
 *	@brief		设置磁力线终止点
 *	@param[in]	i_pntEnd	终止点
 *	@param[out]	无
 *	@retval		STK_SUCCESS	设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Feat_MagneticLine::SetEndPoint(STK_PNT32& i_pntEnd)
{
	m_pntEnd = i_pntEnd;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取磁力线终止点
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		终止点
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_PNT32 Stk_Feat_MagneticLine::GetEndPoint()
{
	return m_pntEnd;
}

/************************************************************************//**
 *	@brief		设置磁体是否有效
 *	@param[in]	i_bEffective	是否有效
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Feat_MagneticLine::SetEffective(STK_BOOL i_bEffective)
{
	m_bEffective = i_bEffective;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		磁体是否有效
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		是否有效
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_BOOL Stk_Feat_MagneticLine::IsEffective()
{
	return m_bEffective;
}

/************************************************************************//**
 *	@brief		设置磁力线关联的要素的排列方式
 *	@param[in]	i_eArrangement	排列方式
 *	@param[out]	无
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Feat_MagneticLine::SetArrangementType(StkMagneticArrangementEnum i_eArrangement)
{
	m_eArrangement = i_eArrangement;
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取磁力线关联的要素的排列方式
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		排列方式
 *	@note		无
 *	@attention	无
 ****************************************************************************/
StkMagneticArrangementEnum Stk_Feat_MagneticLine::GetArrangementType()
{
	return m_eArrangement;
}

/************************************************************************//**
 *	@brief		读取文件中的磁力线特征数据
 *	@param[in]	i_FileP				文件对象指针
 *	@param[in]	i_MagneticLineHeadP	PMI记录头指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS			读取成功
 *	@retval		STK_ERROR_MEMORY	内存申请失败
 *	@retval		其他				读取失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_Feat_MagneticLine::ReadFile(Stk_File *i_FileP, StkSegmentHead *i_MagneticLineHeadP)
{
	StkControl**			RecordPP = NULL;
	STK_STATUS				eState = STK_SUCCESS;
	StkFeatMagneticLineRec*	FeatMagneticLineRecP = NULL;
	StkFeatMagneticLineRec2* FeatMagneticLineRec2P = NULL;

	if (i_FileP == NULL || i_MagneticLineHeadP == NULL)
	{
		return STK_ERROR;
	}

	/*
	 *	读取磁力线特征的数据
	 */
	RecordPP = (StkControl **)calloc(i_MagneticLineHeadP->recnum, sizeof(StkControl *));
	if (i_MagneticLineHeadP->recnum != 0 && RecordPP == NULL)
	{
		return STK_ERROR_MEMORY;
	}

	eState = i_FileP->ReadSegmentAllRecordData(i_MagneticLineHeadP, (StkControl **)RecordPP);
	if (eState!=STK_SUCCESS)
	{
		return eState;
	}

	/*
	 *	读取线缆特征段的各记录
	 */
	for (int ii = 0; ii < i_MagneticLineHeadP->recnum; ii++)
	{
		if(RecordPP[ii]->type == REC_TYPE_FEAT_MAGNETICLINE)
		{
			if (RecordPP[ii]->version == 1)
			{
				FeatMagneticLineRecP = (StkFeatMagneticLineRec*)RecordPP[ii];
				m_pntStart = FeatMagneticLineRecP->pntStatrt;
				m_pntEnd = FeatMagneticLineRecP->pntEnd;
				m_eArrangement = (StkMagneticArrangementEnum)FeatMagneticLineRecP->Arrangement;
				if (FeatMagneticLineRecP->IsItem & STK_IS_UNEFFECTIVE)
				{
					m_bEffective = false;
				}
			}
			else if (RecordPP[ii]->version == 2)
			{
				FeatMagneticLineRec2P = (StkFeatMagneticLineRec2*)RecordPP[ii];
				m_pntStart = FeatMagneticLineRec2P->pntStatrt;
				m_pntEnd = FeatMagneticLineRec2P->pntEnd;
				m_eArrangement = (StkMagneticArrangementEnum)FeatMagneticLineRec2P->Arrangement;
				if (FeatMagneticLineRec2P->IsItem & STK_IS_UNEFFECTIVE)
				{
					m_bEffective = false;
				}
				m_rgbaMagneticLine = FeatMagneticLineRec2P->rgbaMagneticLine;
			}
		}
	}

	eState = i_FileP->ReadSegmentFinishData(i_MagneticLineHeadP,(StkControl **)RecordPP);// 释放内存
	if (eState != STK_SUCCESS)
	{
		return eState;
	}
	if (RecordPP != NULL)
	{
		free(RecordPP);
		RecordPP = NULL;
	}

	return STK_SUCCESS;
}

/*************************************************************************//**
 *	@brief		写磁力线特征数据
 *	@param[in]	i_stkFileP			文件对象指针
 *	@param[out]	无
 *	@retval		STK_SUCCESS			写入成功
 *	@retval		其它				写入失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/
STK_STATUS Stk_Feat_MagneticLine::WriteFile(Stk_File *i_stkFileP)
{
	StkSegmentHead			FeatMagneticLineHead ={0};
	StkFeatMagneticLineRec2	FeatMagneticLineRec;
	STK_STATUS				eState = STK_SUCCESS;

	if (i_stkFileP == NULL)
	{
		return STK_ERROR;
	}

	/*
	 *	【第1步】 输出磁力线特征段头记录
	 */
	FeatMagneticLineHead.id = 0;
	FeatMagneticLineHead.kind = SEG_TYPE_FEAT_MAGNETICLINE;
	eState = i_stkFileP->WriteSegmentHeadData(&FeatMagneticLineHead);
	if (eState != STK_SUCCESS)
	{
		return eState;
	}

	/*
	 *	【第2步】 输出磁力线特征记录
	 */
	FeatMagneticLineRec.Control.type = REC_TYPE_FEAT_MAGNETICLINE;
	FeatMagneticLineRec.Control.length = sizeof(StkFeatMagneticLineRec2) - sizeof(StkControl);
	FeatMagneticLineRec.Control.version = _VER_FEAT_MAGNETICLINE_RECORD;
	FeatMagneticLineRec.pntStatrt = m_pntStart;
	FeatMagneticLineRec.pntEnd = m_pntEnd;
	FeatMagneticLineRec.Arrangement = m_eArrangement;
	FeatMagneticLineRec.IsItem = 0;
	if (!m_bEffective)
	{
		FeatMagneticLineRec.IsItem |= STK_IS_UNEFFECTIVE;
	}
	FeatMagneticLineRec.rgbaMagneticLine = m_rgbaMagneticLine;

	eState = i_stkFileP->WriteSegmentRecordData(&FeatMagneticLineHead, (StkControl *)&FeatMagneticLineRec);
	if(eState < STK_SUCCESS)
	{
		return STK_ERROR;
	}

	/*
	 *	【第3步】 完成磁力线特征段的输出
	 */
	eState = i_stkFileP->WriteSegmentFinishData(&FeatMagneticLineHead);
	if(eState < STK_SUCCESS)
	{
		return STK_ERROR;
	}

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		设置磁力线颜色
 *	@param[in]	i_rgbaMagneticLine	磁力线颜色
 *	@param[out]	无
 *	@retval		STK_SUCCESS			设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Feat_MagneticLine::SetColor(STK_RGBA32 i_rgbaMagneticLine)
{
	m_rgbaMagneticLine = i_rgbaMagneticLine;
	
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取磁力线颜色
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		磁力线颜色
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_RGBA32 Stk_Feat_MagneticLine::GetColor()
{
	return m_rgbaMagneticLine;
}