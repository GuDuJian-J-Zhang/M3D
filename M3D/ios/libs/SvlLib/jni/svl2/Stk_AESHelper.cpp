// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_AesHelper.cpp
 *
 *	@brief	文件加密
 *
 *	@par	历史:
 *		2015/07/20	创建。WangY@HOTEAMSOFT
****************************************************************************/
#include "StdAfx.h"
#include "Stk_AESHelper.h"
#include "Stk_Platform.h"

/************************************************************************//**
 *	@brief		文件加密构造函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_AesHelper::Stk_AesHelper()
{
	unsigned char chKey[] = "SVL_LP2W3E4R";
	m_stkAesP = new AES(chKey);
}

/************************************************************************//**
 *	@brief		文件加密析构函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_AesHelper::~Stk_AesHelper()
{
	if (m_stkAesP != NULL)
	{
		delete m_stkAesP;
		m_stkAesP = NULL;
	}
}

/************************************************************************//**
 *	@brief		文件加密
 *	@param[in]	i_wstrMingwen		明文
 *	@param[out]	o_wstrMiwenHex		密文
 *	@retval		STK_SUCCESS			加密成功
 *	@retval		其他				加密失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_AesHelper::aesEncrypt(wstring i_wstrMingwen, wstring& o_wstrMiwenHex)
{
	STK_STATUS		eState = STK_SUCCESS;
	string			strMingwen;
	string			strMiwen_hex;
	STK_CHAR*		chMingwen = NULL;
	STK_CHAR*		chMiwenHex = NULL;
	STK_UINT32		nuMingwenSize = i_wstrMingwen.size();
	// 限制密码长度最长为16字节，最短为6字节
	if (nuMingwenSize < MIN_PASSWORD || nuMingwenSize > MAX_PASSWORD)
	{
		return STK_ERROR_PASSWORD;
	}

	strMingwen = Stk_Platform::WStringToString(i_wstrMingwen);
	chMingwen = new STK_CHAR[nuMingwenSize + 1];
	chMiwenHex = new STK_CHAR[STK_MAXPATH*STK_MAXPATH];

	memcpy(chMingwen, strMingwen.c_str(), sizeof(STK_CHAR)*(nuMingwenSize + 1));
	m_stkAesP->Cipher(chMingwen, chMiwenHex);
	strMiwen_hex = chMiwenHex;
	o_wstrMiwenHex = Stk_Platform::StringToWString(strMiwen_hex);

	if (chMingwen != NULL)
	{
		delete chMingwen;
		chMingwen = NULL;
	}
	if (chMiwenHex != NULL)
	{
		delete chMiwenHex;
		chMiwenHex = NULL;
	}

	return eState;
}

/************************************************************************//**
 *	@brief		文件解密
 *	@param[in]	i_wstrMiwenHex		密文
 *	@param[out]	o_wstrMingwen		明文
 *	@retval		STK_SUCCESS			解密成功
 *	@retval		其他				解密失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_AesHelper::aesUncrypt(wstring i_wstrMiwenHex, wstring& o_wstrMingwen)
{
	STK_STATUS		eState = STK_SUCCESS;
	string			strMingwen;
	string			strMiwen_hex;
	STK_CHAR*		chMingwen = NULL;
	STK_CHAR*		chMiwenHex = NULL;
	STK_UINT32		nuMiwenSize = i_wstrMiwenHex.size();

	strMiwen_hex = Stk_Platform::WStringToString(i_wstrMiwenHex);
	chMiwenHex = new STK_CHAR[nuMiwenSize + 1];
	chMingwen = new STK_CHAR[STK_MAXNAME];

	memcpy(chMiwenHex, strMiwen_hex.c_str(), sizeof(STK_CHAR)*(nuMiwenSize + 1));
	m_stkAesP->InvCipher(chMiwenHex, chMingwen);
	strMingwen = chMingwen;
	o_wstrMingwen = Stk_Platform::StringToWString(strMingwen);

	if (chMingwen != NULL)
	{
		delete chMingwen;
		chMingwen = NULL;
	}
	if (chMiwenHex != NULL)
	{
		delete chMiwenHex;
		chMiwenHex = NULL;
	}

	return eState;
}