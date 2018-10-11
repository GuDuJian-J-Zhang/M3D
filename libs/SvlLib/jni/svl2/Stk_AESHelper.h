// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_AesHelper.h
 *
 *	@brief	文件加密
 *
 *	@par	历史:
 *		2015/07/20	创建。WangY@HOTEAMSOFT
****************************************************************************/
#ifndef STK_AESHELPER_H
#define STK_AESHELPER_H

#ifndef __MOBILE__
#pragma once
#endif

#include "DataManager.h"
#include "Stk_Object.h"
#include "AES.h"

class DATAMANAGER_API Stk_AesHelper :
	public Stk_Object
{
private:
	AES*	m_stkAesP;			//!< 文件加密对象指针
public:
	/****************************************************************************
	 *	@brief		文件加密构造函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	Stk_AesHelper();

	/****************************************************************************
	 *	@brief		文件加密析构函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	~Stk_AesHelper();
	
	/****************************************************************************
	 *	@brief		文件加密
	 *	@param[in]	i_wstrMingwen		明文
	 *	@param[out]	o_wstrMiwenHex		密文
	 *	@retval		STK_SUCCESS			加密成功
	 *	@retval		其他				加密失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS aesEncrypt(wstring i_wstrMingwen, wstring& o_wstrMiwenHex);
	
	/****************************************************************************
	 *	@brief		文件解密
	 *	@param[in]	i_wstrMiwenHex		密文
	 *	@param[out]	o_wstrMingwen		明文
	 *	@retval		STK_SUCCESS			解密成功
	 *	@retval		其他				解密失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	STK_STATUS aesUncrypt(wstring i_wstrMiwenHex, wstring& o_wstrMingwen);
};

#endif // AESHELPER_H