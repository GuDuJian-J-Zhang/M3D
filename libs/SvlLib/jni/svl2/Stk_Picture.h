// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_Picture.h
 *
 *	@brief	图片类的声明
 *
 *	@par	历史:
 *		2015/05/25	创建。WangY@HOTEAMSOFT
****************************************************************************/
#ifndef _Stk_Picture_H_
#define _Stk_Picture_H_

#ifndef __MOBILE__
#pragma once
#endif

#include "DataManager.h"
#include "Stk_Object.h"
#include "Stk_File.h"
#include <string>

using namespace std;

/**************************************************************************************
	 *	@class	Stk_Picture
	 *	@brief	图片类
	 *
	 *
***************************************************************************************/

class DATAMANAGER_API Stk_Picture :
	public Stk_Object
{
private:
	StkPictureTypeEnum	m_eType;			//!< 图片类型
	STK_PNT32			m_pntLocation;		//!< 位置
	STK_FLOAT32			m_fDisPlayWidth;	//!< 图片场景图显示宽
	STK_FLOAT32			m_fDisplayHeight;	//!< 图片场景图显示高
	STK_FLOAT32			m_fActualWidth;		//!< 图片实际宽
	STK_FLOAT32			m_fActualHeight;	//!< 图片实际高
	StkPicSaveStyleEnum	m_eSaveStyle;		//!< 图片存储位置标识
	STK_INT32			m_nNC;				//!< 图片位图
	wstring				m_wstrPath;			//!< 图片存储位置路径
	STK_UINT32			m_nudataSize;		//!< 图片数据流长度
	STK_CHAR*			m_dataP;			//!< 图片数据流
	STK_FLOAT32			m_fScaleX;			//!< X方向缩放因子
	STK_FLOAT32			m_fScaleY;			//!< Y方向缩放因子
public:
	/****************************************************************************
	 *	@brief		Stk_Picture构造函数
	 *	@param[in]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_Picture();

	/****************************************************************************
	 *	@brief		Stk_Picture析构函数
	 *	@param[in]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	~Stk_Picture();

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
	 *	@brief		设置图片实际的高度
	 *	@param[in]	i_fActualHeight		图片实际的高度
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS			设置成功
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS				SetActualHeight(STK_FLOAT32 i_fActualHeight);
	
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

	/*****************************************************************************
	 *	@brief		写图片数据
	 *	@param[in]	i_head			数据段头指针
	 *	@param[in]	i_stkFileP		文件对象指针
	 *	@param[out]	无
	 *	@retval		STK_SUCCESS		写入成功
	 *	@retval		其它			写入失败
	 *	@note		无
	 *	@attention	内部接口
	 ****************************************************************************/
	STK_STATUS				WriteFile(StkSegmentHead* i_head, Stk_File* i_stkFileP);
};

#endif