// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_Feat.cpp
 *
 *	@brief	特征类的实现
 *
 *	@par	历史:
 *		2014/05/30	创建。WangY@HOTEAMSOFT
****************************************************************************/

#ifndef __MOBILE__
#pragma once
#endif

#include "Stk_Feat.h"

namespace HoteamSoft {
	namespace SVLLib {

		STK_IMPLEMENT_CLASS(Stk_Feat, Stk_Object)
		/************************************************************************//**
		 *	@brief		Stk_Feat构造函数
		 *	@param[in]	i_stkPrototypeP		所属零件
		 *	@retval		无
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/

		Stk_Feat::Stk_Feat()
		{
			m_iModelID = 0;
			m_eType = StkFeatTypeEnum::FEAT_TYPE_UNKNOWN;
			m_bActive = false;
			m_wstrName = L"";
			m_dataP = NULL;
		}


		 /************************************************************************//**
		  *	@brief		Stk_Feat析构函数
		  *	@param[in]	无
		  *	@retval		无
		  *	@note		无
		  *	@attention	无
		  ****************************************************************************/

		Stk_Feat::~Stk_Feat()
		{
			
		}

		/************************************************************************//**
		 *	@brief		设置特征类型
		 *	@param[in]	i_eType			特征类型
		 *	@retval		STK_SUCCESS		设置成功
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/

		STK_STATUS Stk_Feat::SetFeatType(StkFeatTypeEnum i_eType)
		{
			m_eType = i_eType;

			return STK_SUCCESS;
		}

		/************************************************************************//**
		 *	@brief		获取特征类型
		 *	@param[in]	无
		 *	@retval		m_eType			特征类型
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/

		StkFeatTypeEnum Stk_Feat::GetFeatType()
		{
			return m_eType;
		}

		/************************************************************************//**
		 *	@brief		是否活动状态
		 *	@param[in]	无
		 *	@retval		m_bActive		是否活动状态
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/

		STK_BOOL Stk_Feat::IsActiveed()
		{
			return m_bActive;
		}

		/************************************************************************//**
		 *	@brief		设置活动状态
		 *	@param[in]	i_bActived		活动状态
		 *	@retval		STK_SUCCESS		设置成功
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/

		STK_STATUS Stk_Feat::SetActiveed(STK_BOOL i_bActived)
		{
			m_bActive = i_bActived;

			return STK_SUCCESS;
		}

		/************************************************************************//**
		 *	@brief		设置特证名
		 *	@param[in]	i_wstrName		特证名
		 *	@retval		STK_SUCCESS		设置成功
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/

		STK_STATUS Stk_Feat::SetFeatName(wstring i_wstrName)
		{
			m_wstrName = i_wstrName;

			return STK_SUCCESS;
		}

		/************************************************************************//**
		 *	@brief		获取特证名
		 *	@param[in]	无
		 *	@retval		m_wstrName		特证名
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/

		wstring Stk_Feat::GetFeatName()
		{
			return m_wstrName;
		}

		/************************************************************************//**
		 *	@brief		设置线缆特征
		 *	@param[in]	i_FeatPipe		线缆特征对象指针
		 *	@retval		STK_SUCCESS		设置成功
		 *	@retval		其他			设置失败
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/

		STK_STATUS Stk_Feat::SetFeatPipe(Stk_Feat_PipePtr i_FeatPipe)
		{
			if (m_eType == FEAT_TYPE_PIPE)
			{
				m_dataP = i_FeatPipe;
				return STK_SUCCESS;
			}
			else
			{
				return STK_ERROR;
			}
		}

		/************************************************************************//**
		 *	@brief		获取线缆特征
		 *	@param[in]	无
		 *	@retval		m_dataP			线缆特征
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/

		Stk_Feat_PipePtr Stk_Feat::GetFeatPipe()
		{
			if (m_eType == FEAT_TYPE_PIPE)
			{
				return Stk_Feat_PipePtr::DynamicCast(m_dataP);
			}
			else
			{
				return NULL;
			}
		}


		STK_STATUS Stk_Feat::SetFeatHole(Stk_Feat_HolePtr i_FeatHole)
		{
			if (m_eType == FEAT_TYPE_HOLE)
			{
				m_dataP = i_FeatHole;
				return STK_SUCCESS;
			}
			else
			{
				return STK_ERROR;
			}
		}

		Stk_Feat_HolePtr Stk_Feat::GetFeatHole()
		{
			if (m_eType == FEAT_TYPE_HOLE)
			{
				return Stk_Feat_HolePtr::DynamicCast(m_dataP);
			}
			else
			{
				return NULL;
			}
		}

		/************************************************************************//**
		 *	@brief		设置磁力线特征
		 *	@param[in]	i_MagneticLineP	磁力线特征对象
		 *	@retval		STK_SUCCESS		设置成功
		 *	@retval		其他			设置失败
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_STATUS Stk_Feat::SetMagneticLine(Stk_Feat_MagneticLinePtr i_MagneticLineP)
		{
			if (m_eType == FEAT_TYPE_MAGNETIC_LINE)
			{
				m_dataP = i_MagneticLineP;
				return STK_SUCCESS;
			}
			else
			{
				return STK_ERROR;
			}
		}

		/************************************************************************//**
		 *	@brief		获取磁力线特征
		 *	@param[in]	无
		 *	@retval		磁力线特征
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		Stk_Feat_MagneticLinePtr Stk_Feat::GetMagneticLine()
		{
			if (m_eType == FEAT_TYPE_MAGNETIC_LINE)
			{
				return Stk_Feat_MagneticLinePtr::DynamicCast(m_dataP);
			}
			else
			{
				return NULL;
			}
		}
		
		/************************************************************************//**
		 *	@brief		设置标记特征
		 *	@param[in]	i_FeatMarkP		标记特征对象
		 *	@retval		STK_SUCCESS		设置成功
		 *	@retval		其他			设置失败
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_STATUS Stk_Feat::SetFeatMark(Stk_Feat_MarkPtr i_FeatMarkP)
		{
			if (m_eType == FEAT_TYPE_MARK)
			{
				m_dataP = i_FeatMarkP;
				return STK_SUCCESS;
			}
			else
			{
				return STK_ERROR;
			}
		}

		/************************************************************************//**
		 *	@brief		获取标记特征
		 *	@param[in]	无
		 *	@retval		标记特征
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		Stk_Feat_MarkPtr Stk_Feat::GetFeatMark()
		{
			if (m_eType == FEAT_TYPE_MARK)
			{
				return Stk_Feat_MarkPtr::DynamicCast(m_dataP);
			}
			else
			{
				return NULL;
			}
		}

	}
}