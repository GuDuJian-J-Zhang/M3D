// All Rights Reserved. Copyright(C) 2012 HOTEAMSOFT
/****************************************************************************
*	@file	Stk_Texture.h
*
*	@brief	Stk_Texture纹理类
*
*	@par	历史:
*		2015/09/07	创建。WangY@HOTEAMSOFT
 ****************************************************************************/

#ifndef __SVLLIB_TEXTURE_H_
#define __SVLLIB_TEXTURE_H_

#ifndef __MOBILE__
#pragma once
#endif

#include "Stk_Object.h"
#include "Stk_Image.h"

/**************************************************************************************
	 *	@class	Stk_Texture
	 *	@brief	Stk_Texture 纹理类
	 *
	 *	继承自Stk_Object类。
***************************************************************************************/
namespace HoteamSoft {
	namespace SVLLib {
		class STK_TOOLKIT_EXPORT  Stk_Texture : public Stk_Object
		{
			STK_DECLARE_CLASS(Stk_Texture)
		protected:
			enum { _typeID = _SVLLIB_TYPE_TEXTURE };	//!< 类别
		private:
			StkTextureMappingTypeEnum	m_eMappingType;			//!< 纹理映射类型
			STK_FLOAT32					m_fRotateAngle;			//!< 旋转角度
			Stk_ImagePtr					m_stkPictureP;				//!< 图片
			STK_FLOAT32					m_fUOffset;				//!< U偏移距离
			STK_FLOAT32					m_fVOffset;				//!< V偏移距离
			STK_FLOAT32					m_fUScale;				//!< U缩放因子
			STK_FLOAT32					m_fVScale;				//!< V缩放因子
			STK_BOOL					m_bUFlipped;			//!< 
			STK_BOOL					m_bVFlipped;			//!< 
		public:
			/****************************************************************************
			 *	@brief		Stk_Texture构造函数
			 *	@param[in]	无
			 *	@retval		无
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			Stk_Texture();

			/****************************************************************************
			 *	@brief		Stk_Texture析构函数
			 *	@param[in]	无
			 *	@retval		无
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			~Stk_Texture();
		public:
			/****************************************************************************
			 *	@brief		设置纹理映射类型
			 *	@param[in]	i_eMappingType		纹理映射类型
			 *	@param[out]	无
			 *	@retval		STK_SUCCESS			设置成功
			 *	@retval		其它				设置失败
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			STK_STATUS					SetType(StkTextureMappingTypeEnum i_eMappingType);

			/****************************************************************************
			 *	@brief		获取纹理映射类型
			 *	@param[in]	无
			 *	@param[out]	无
			 *	@retval		纹理映射类型
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			StkTextureMappingTypeEnum	GetType();

			/****************************************************************************
			 *	@brief		设置旋转角度
			 *	@param[in]	i_fRotateAngle		旋转角度
			 *	@param[out]	无
			 *	@retval		STK_SUCCESS			设置成功
			 *	@retval		其它				设置失败
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			STK_STATUS					SetRotateAngle(STK_FLOAT32 i_fRotateAngle);

			/****************************************************************************
			 *	@brief		获取旋转角度
			 *	@param[in]	无
			 *	@param[out]	无
			 *	@retval		旋转角度
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			STK_FLOAT32					GetRotateAngle();

			/****************************************************************************
			 *	@brief		设置纹理图片
			 *	@param[in]	i_stkPictureP		纹理图片
			 *	@param[out]	无
			 *	@retval		STK_SUCCESS			设置成功
			 *	@retval		其它				设置失败
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			STK_STATUS					SetTextureImage(Stk_ImagePtr i_stkPictureP);

			/****************************************************************************
			 *	@brief		获取纹理图片
			 *	@param[in]	无
			 *	@param[out]	无
			 *	@retval		纹理图片
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			Stk_ImagePtr				GetTextureImage();

			/****************************************************************************
			 *	@brief		设置U方向偏置距离
			 *	@param[in]	i_fUOffset			U方向偏置距离
			 *	@param[out]	无
			 *	@retval		STK_SUCCESS			设置成功
			 *	@retval		其它				设置失败
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			STK_STATUS					SetUOffset(STK_FLOAT32 i_fUOffset);

			/****************************************************************************
			 *	@brief		获取U方向偏置距离
			 *	@param[in]	无
			 *	@param[out]	无
			 *	@retval		U方向偏置距离
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			STK_FLOAT32					GetUOffset();

			/****************************************************************************
			 *	@brief		设置V方向偏置距离
			 *	@param[in]	i_fUOffset			V方向偏置距离
			 *	@param[out]	无
			 *	@retval		STK_SUCCESS			设置成功
			 *	@retval		其它				设置失败
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			STK_STATUS					SetVOffset(STK_FLOAT32 i_fVOffset);

			/****************************************************************************
			 *	@brief		获取V方向偏置距离
			 *	@param[in]	无
			 *	@param[out]	无
			 *	@retval		V方向偏置距离
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			STK_FLOAT32					GetVOffset();

			/****************************************************************************
			 *	@brief		设置U方向缩放因子
			 *	@param[in]	i_fScale			U方向缩放因子
			 *	@param[out]	无
			 *	@retval		STK_SUCCESS			设置成功
			 *	@retval		其它				设置失败
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			STK_STATUS					SetUScale(STK_FLOAT32 i_fScale);

			/****************************************************************************
			 *	@brief		获取U方向缩放因子
			 *	@param[in]	无
			 *	@param[out]	无
			 *	@retval		U方向缩放因子
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			STK_FLOAT32					GetUScale();

			/****************************************************************************
			 *	@brief		设置V方向缩放因子
			 *	@param[in]	i_fScale			V方向缩放因子
			 *	@param[out]	无
			 *	@retval		STK_SUCCESS			设置成功
			 *	@retval		其它				设置失败
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			STK_STATUS					SetVScale(STK_FLOAT32 i_fScale);

			/****************************************************************************
			 *	@brief		获取V方向缩放因子
			 *	@param[in]	无
			 *	@param[out]	无
			 *	@retval		V方向缩放因子
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			STK_FLOAT32					GetVScale();

			/****************************************************************************
			 *	@brief		设置Flip标识
			 *	@param[in]	i_bUFlip			U方向Flip
			 *	@param[in]	i_bVFlip			V方向Flip
			 *	@param[out]	无
			 *	@retval		STK_SUCCESS			设置成功
			 *	@retval		其它				设置失败
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			STK_STATUS					SetFlipFlag(STK_BOOL i_bUFlip, STK_BOOL i_bVFlip);

			/****************************************************************************
			 *	@brief		U方向Flip标识
			 *	@param[in]	无
			 *	@param[out]	无
			 *	@retval		U方向Flip标识
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			STK_BOOL					IsUFlipped();

			/****************************************************************************
			 *	@brief		V方向Flip标识
			 *	@param[in]	无
			 *	@param[out]	无
			 *	@retval		V方向Flip标识
			 *	@note		无
			 *	@attention	无
			 ****************************************************************************/
			STK_BOOL					IsVFlipped();

		};
	}
}
#endif