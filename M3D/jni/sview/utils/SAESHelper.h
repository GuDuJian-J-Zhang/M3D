// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_AesHelper.h
 *
 *	@brief	文件加密
 *
 *	@par	履历:
 *		2015/07/20	创建。WangY@HOTEAMSOFT
****************************************************************************/
#ifndef STK_AESHELPER_H
#define STK_AESHELPER_H

#include <string>
using std::string;
#include "SAES.h"

#include "m3d/M3D.h"
#define AES_MIN_PASSWORD	6
#define AES_MAX_PASSWORD	1000

namespace SVIEW
{
class  M3D_API SAESHelper
{
public:
	/****************************************************************************
	 *	@brief		文件加密构造函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	SAESHelper();

	/****************************************************************************
	 *	@brief		文件加密析构函数
	 *	@param[in]	无
	 *	@param[out]	无
	 *	@retval		无
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	~SAESHelper();
	
	/****************************************************************************
	 *	@brief		文件加密
	 *	@param[in]	i_wstrMingwen		明文
	 *	@param[out]	o_wstrMiwenHex		密文
	 *	@retval		STK_SUCCESS			加密成功
	 *	@retval		其他				加密失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	int aesEncrypt(string i_strMingwen, string& o_strMiwenHex);
	
	/****************************************************************************
	 *	@brief		文件解密
	 *	@param[in]	i_wstrMiwenHex		密文
	 *	@param[out]	o_wstrMingwen		明文
	 *	@retval		STK_SUCCESS			解密成功
	 *	@retval		其他				解密失败
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	int aesUncrypt(string i_strMiwenHex, string& o_strMingwen);

private:
	//SAES *aes;
};

class M3D_API ZBase64
{
public:
    /*编码
    DataByte
        [in]输入的数据长度,以字节为单位
    */
    static string Encode(const unsigned char* Data,int DataByte);
    /*解码
    DataByte
        [in]输入的数据长度,以字节为单位
    OutByte
        [out]输出的数据长度,以字节为单位,请不要通过返回值计算
        输出数据的长度
    */
	static int Decode(const char* Data, int DataByte, char* outData);
};

}

#endif // AESHELPER_H
