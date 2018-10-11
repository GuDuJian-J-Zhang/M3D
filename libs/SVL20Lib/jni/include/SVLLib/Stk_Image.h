#ifndef _SVLLIB_IMAGE_H_
#define _SVLLIB_IMAGE_H_


// All Rights Reserved. Copyright (C) 2017 HOTEAMSOFT, Ltd
/****************************************************************************
*	@file	Stk_Image.h
*
*	@brief	图像类
*
*	@par	历史:
*		2017/06/10	创建。qinyp@HOTEAMSOFT
****************************************************************************/

#pragma once
#include "Stk_Object.h"
namespace HoteamSoft {
	namespace SVLLib {
		class STK_TOOLKIT_EXPORT Stk_Image : public Stk_Object
		{
			STK_DECLARE_CLASS(Stk_Image)
		public:
			enum {
				_typeID = _SVLLIB_TYPE_IMAGE
			};	//!< 类别
			Stk_Image();
			virtual ~Stk_Image();
		public:
			/****************************************************************************
			*	@brief		设置图片类型
			*	@param[in]	i_eType				图片类型
			*	@param[out]	无
			*	@retval		STK_SUCCESS			设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS				SetType(StkPictureTypeEnum i_eType);

			/****************************************************************************
			*	@brief		获取图片类型
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		图片类型
			*	@note		无
			*	@attention	无
			****************************************************************************/
			StkPictureTypeEnum		GetType();

			/****************************************************************************
			*	@brief		设置图片位置
			*	@param[in]	i_pntLocation		图片位置
			*	@param[out]	无
			*	@retval		STK_SUCCESS			设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS				SetLocation(STK_PNT32 i_pntLocation);

			/****************************************************************************
			*	@brief		获取图片位置
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		图片位置
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_PNT32				GetLocation();

			/****************************************************************************
			*	@brief		设置图片场景图显示的宽度
			*	@param[in]	i_fDisplayWidth		图片场景图显示的宽度
			*	@param[out]	无
			*	@retval		STK_SUCCESS			设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS				SetDisplayWidth(STK_FLOAT32 i_fDisplayWidth);

			/****************************************************************************
			*	@brief		获取图片场景图显示的宽度
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		图片场景图显示的宽度
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_FLOAT32				GetDisplayWidth();

			/****************************************************************************
			*	@brief		设置图片场景图显示的高度
			*	@param[in]	i_fDisplayHeight	图片场景图显示的高度
			*	@param[out]	无
			*	@retval		STK_SUCCESS			设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS				SetDisplayHeight(STK_FLOAT32 i_fDisplayHeight);

			/****************************************************************************
			*	@brief		获取图片场景图显示的高度
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		图片场景图显示的高度
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_FLOAT32				GetDisplayHeight();

			/****************************************************************************
			*	@brief		设置图片实际的宽度
			*	@param[in]	i_fActualWidth		图片实际的宽度
			*	@param[out]	无
			*	@retval		STK_SUCCESS			设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS				SetActualWidth(STK_FLOAT32 i_fActualWidth);

			/****************************************************************************
			*	@brief		获取图片实际的宽度
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		图片实际的宽度
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_FLOAT32				GetActualWidth();

			/****************************************************************************
			*	@brief		获取图片实际的高度
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		图片场景图显示的高度
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_FLOAT32				GetActualHeight();

			/****************************************************************************
			*	@brief		设置图片实际的高度
			*	@param[in]	i_fActualHeight		图片实际的高度
			*	@param[out]	无
			*	@retval		STK_SUCCESS			设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS				SetActualHeight(STK_FLOAT32 i_fActualHeight);

			/****************************************************************************
			*	@brief		设置图片保存方式
			*	@param[in]	i_eSaveStyle		图片保存方式：SVL外部/内部
			*	@param[out]	无
			*	@retval		STK_SUCCESS			设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS				SetPictureSaveStyle(StkPicSaveStyleEnum i_eSaveStyle);

			/****************************************************************************
			*	@brief		获取图片保存方式
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		图片保存方式：SVL外部/内部
			*	@note		无
			*	@attention	无
			****************************************************************************/
			StkPicSaveStyleEnum		GetPictureSaveStyle();

			/****************************************************************************
			*	@brief		设置图片的保存路径
			*	@param[in]	i_wstrSavePath		图片的保存路径
			*	@param[out]	无
			*	@retval		STK_SUCCESS			设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS				SetPictureSavePath(wstring i_wstrSavePath);

			/****************************************************************************
			*	@brief		获取图片的保存路径
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		图片的保存路径
			*	@note		无
			*	@attention	无
			****************************************************************************/
			wstring					GetPictureSavePath();

			/****************************************************************************
			*	@brief		设置图片位图NC
			*	@param[in]	i_nNC				位图NC
			*	@param[out]	无
			*	@retval		STK_SUCCESS			设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS				SetNC(STK_INT32 i_nNC);

			/****************************************************************************
			*	@brief		获取图片位图NC
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		位图NC
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_INT32				GetNC();

			/****************************************************************************
			*	@brief		设置图片的数据流
			*	@param[in]	i_nudataSize		图片的数据流长度
			*	@param[in]	i_chDataP			图片的数据流
			*	@param[out]	无
			*	@retval		STK_SUCCESS			设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS				SetData(STK_UINT32 i_nudataSize, STK_CHAR* i_chDataP);

			/****************************************************************************
			*	@brief		获取图片的数据流
			*	@param[in]	无
			*	@param[out]	o_nudataSize		图片的数据流长度
			*	@param[out]	o_chDataP			图片的数据流
			*	@retval		STK_SUCCESS			设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS				GetData(STK_UINT32& o_nudataSize, STK_CHAR*& o_chDataP);

			/****************************************************************************
			*	@brief		设置X方向缩放因子
			*	@param[in]	i_fScaleX			X方向缩放因子
			*	@param[out]	无
			*	@retval		STK_SUCCESS			设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS				SetScaleX(STK_FLOAT32 i_fScaleX);

			/****************************************************************************
			*	@brief		获取X方向缩放因子
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		X方向缩放因子
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_FLOAT32				GetScaleX();

			/****************************************************************************
			*	@brief		设置Y方向缩放因子
			*	@param[in]	i_fScaleY			Y方向缩放因子
			*	@param[out]	无
			*	@retval		STK_SUCCESS			设置成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS				SetScaleY(STK_FLOAT32 i_fScaleY);

			/****************************************************************************
			*	@brief		获取Y方向缩放因子
			*	@param[in]	无
			*	@param[out]	无
			*	@retval		Y方向缩放因子
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_FLOAT32				GetScaleY();

			/****************************************************************************
			*	@brief		写文件
			*	@param[in]	i_DataP		数据写入位置
			*	@param[in]	i_pDoc		文档
			*	@param[out]	无
			*	@retval		STK_SUCCESS	写入成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS					ToJson(void* i_DataP, void* pDoc);

			/****************************************************************************
			*	@brief		读文件
			*	@param[in]	i_DataP		数据读入位置
			*	@param[out]	无
			*	@retval		STK_SUCCESS	读入成功
			*	@note		无
			*	@attention	无
			****************************************************************************/
			STK_STATUS					FromJson(void* i_DataP);
		};
	}
}
#endif
