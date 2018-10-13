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

#include "Stk_Feat_MagneticLine.h"

namespace HoteamSoft {
	namespace SVLLib {
		STK_IMPLEMENT_CLASS(Stk_Feat_MagneticLine, Stk_Object)
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
	}
}