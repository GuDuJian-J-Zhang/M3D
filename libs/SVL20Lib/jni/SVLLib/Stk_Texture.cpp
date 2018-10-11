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

#include "Stk_Texture.h"
#include "Stk_Platform.h"
#include "Stk_ObjectImpl.h"

namespace HoteamSoft {
	namespace SVLLib {
		STK_IMPLEMENT_CLASS(Stk_Texture, Stk_Object)
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
			m_stkPictureP = nullptr;
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
			DELETE_STK_CLASS_IMPL
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
		STK_STATUS Stk_Texture::SetTextureImage(Stk_ImagePtr i_stkPictureP)
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
		Stk_ImagePtr Stk_Texture::GetTextureImage()
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

	}
}