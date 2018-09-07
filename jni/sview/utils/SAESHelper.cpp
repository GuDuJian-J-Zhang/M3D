// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_AesHelper.cpp
 *
 *	@brief	文件加密
 *
 *	@par	履历:
 *		2015/07/20	创建。WangY@HOTEAMSOFT
 ****************************************************************************/
#include "SAESHelper.h"

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "SAES.h"
#include <string.h>
#define Byte unsigned char

namespace SVIEW
{
/************************************************************************//**
 *	@brief		文件加密构造函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
SAESHelper::SAESHelper()
{
	//unsigned char key[] = "HoteamSoft123";
	//aes = new SAES(key);
}

/************************************************************************//**
 *	@brief		文件加密析构函数
 *	@param[in]	无
 *	@param[out]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/
SAESHelper::~SAESHelper()
{
	//delete aes;
	//aes = 0;
}


/************************************************************************//**
 *	@brief		文件加密
 *	@param[in]	i_strMingwen		明文
 *	@param[out]	o_strMiwenHex		密文
 *	@retval		0			加密成功
 *	@retval		其他				加密失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
int SAESHelper::aesEncrypt(string i_strMingwen, string& o_strMiwenHex)
{
	char miwen_hex[1024] = { 0 };
	//char miwen_hex[] = "8FEEEFE524F8B68DC1FCA2899AC1A6B82E636F6D";

	string result = string("");
	char ch_mingwen[1024] = { 0 };
	char ch_miwen[1024] = { 0 };
#ifdef WIN32
 	strcpy_s(ch_mingwen, i_strMingwen.size() + 1, i_strMingwen.c_str());
#else
	strcpy(ch_mingwen, i_strMingwen.c_str());
#endif
	//aes->Cipher(ch_mingwen, ch_miwen);
	int key[] = { 'H','o','t','e','a' ,'m' ,'s' ,'o' ,'f' ,'t' };//加密字符
																 //aes->Cipher(ch_mingwen, ch_miwen);
	Makecode(ch_mingwen, key);//加密
							  //o_strMiwenHex = string(ch_mingwen);
	o_strMiwenHex = ZBase64::Encode((unsigned char*)ch_mingwen, (i_strMingwen.size() + 1) * sizeof(char));

	return o_strMiwenHex.length() > 0;
}

/************************************************************************//**
 *	@brief		文件解密
 *	@param[in]	i_wstrMiwenHex		密文
 *	@param[out]	o_wstrMingwen		明文
 *	@retval		0			解密成功
 *	@retval		其他				解密失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
int SAESHelper::aesUncrypt(string i_strMiwenHex, string& o_strMingwen)
{
	char miwen_hex[1024] = { 0 };
	int OutByte = ZBase64::Decode((char*)(i_strMiwenHex.c_str()), (i_strMiwenHex.size() + 1) * sizeof(char), miwen_hex);

	//char miwen_hex[] = "8FEEEFE524F8B68DC1FCA2899AC1A6B82E636F6D";
	string result = string("");
	//char ch_mingwen[1024] = { 0 };
	//char ch_miwen[1024] = { 0 };
	//strcpy_s(ch_miwen, i_strMiwenHex.size() + 1,i_strMiwenHex.c_str());
	//aes->InvCipher(ch_miwen, ch_mingwen);
	int key[] = { 'H','o','t','e','a' ,'m' ,'s' ,'o' ,'f' ,'t' };//加密字符
	Cutecode(miwen_hex, key, OutByte);//解密

	o_strMingwen = string(miwen_hex);

	return o_strMingwen.size() > 0;
}

string ZBase64::Encode(const unsigned char* Data, int DataByte)
{
	//编码表
	const char EncodeTable[] =
			"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	//返回值
	string strEncode;
	unsigned char Tmp[4] =
	{ 0 };
	int LineLength = 0;
	for (int i = 0; i < (int) (DataByte / 3); i++)
	{
		Tmp[1] = *Data++;
		Tmp[2] = *Data++;
		Tmp[3] = *Data++;
		strEncode += EncodeTable[Tmp[1] >> 2];
		strEncode += EncodeTable[((Tmp[1] << 4) | (Tmp[2] >> 4)) & 0x3F];
		strEncode += EncodeTable[((Tmp[2] << 2) | (Tmp[3] >> 6)) & 0x3F];
		strEncode += EncodeTable[Tmp[3] & 0x3F];
		if (LineLength += 4, LineLength == 76)
		{
			strEncode += "\r\n";
			LineLength = 0;
		}
	}
	//对剩余数据进行编码
	int Mod = DataByte % 3;
	if (Mod == 1)
	{
		Tmp[1] = *Data++;
		strEncode += EncodeTable[(Tmp[1] & 0xFC) >> 2];
		strEncode += EncodeTable[((Tmp[1] & 0x03) << 4)];
		strEncode += "==";
	}
	else if (Mod == 2)
	{
		Tmp[1] = *Data++;
		Tmp[2] = *Data++;
		strEncode += EncodeTable[(Tmp[1] & 0xFC) >> 2];
		strEncode +=
				EncodeTable[((Tmp[1] & 0x03) << 4) | ((Tmp[2] & 0xF0) >> 4)];
		strEncode += EncodeTable[((Tmp[2] & 0x0F) << 2)];
		strEncode += "=";
	}

	return strEncode;
}

int ZBase64::Decode(const char* Data, int DataByte, char* strDecode)
{
	int OutByte = 0;
    //解码表
    const char DecodeTable[] =
    {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        62, // '+'
        0, 0, 0,
        63, // '/'
        52, 53, 54, 55, 56, 57, 58, 59, 60, 61, // '0'-'9'
        0, 0, 0, 0, 0, 0, 0,
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
        13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, // 'A'-'Z'
        0, 0, 0, 0, 0, 0,
        26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
        39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, // 'a'-'z'
    };
    //返回值
    //string strDecode;
    int nValue;
    int i= 0;
    while (i < DataByte)
    {
        if (*Data != '\r' && *Data!='\n')
        {
            nValue = DecodeTable[*Data++] << 18;
            nValue += DecodeTable[*Data++] << 12;
            strDecode[OutByte]=(nValue & 0x00FF0000) >> 16;
            OutByte++;
            if (*Data != '=')
            {
                nValue += DecodeTable[*Data++] << 6;
				strDecode[OutByte] =(nValue & 0x0000FF00) >> 8;
                OutByte++;
                if (*Data != '=')
                {
                    nValue += DecodeTable[*Data++];
					strDecode[OutByte] =nValue & 0x000000FF;
                    OutByte++;
                }
            }
            i += 4;
        }
        else// 回车换行,跳过
        {
            Data++;
            i++;
        }
     }
    return OutByte;
}
}
