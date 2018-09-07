// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_Platform.h
 *
 *	@brief	公用接口声明
 *
 *	@par	历史:
 *		2014/4/16	创建。Xuli@HOTEAMSOFT
****************************************************************************/
#ifndef _SVLLIB_PLATFORM_H_
#define _SVLLIB_PLATFORM_H_

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>
#include "Stk_Object.h"

namespace HoteamSoft {
	namespace SVLLib {


#ifdef __ANDROID__
#define UTF_8  "utf8"
#define UTF_16 "utf16"
#define UTF_32 "utf32"

#define UTF_16BE "utf16_BigEndian"
#define UTF_32BE "utf32_BigEndian"

#define UTF_16LE "utf16_LittleEndian"
#define UTF_32LE "utf32_LittleEndian"
#endif

#ifdef __IOS__
#define UTF_8  "UTF-8"
#define UTF_16 "UTF-16"
#define UTF_32 "UTF-32"

#define UTF_16BE "UTF-16BE"
#define UTF_32BE "UTF-32BE"

#define UTF_16LE "UTF-16LE"
#define UTF_32LE "UTF-32LE"
#endif

#ifdef __MOBILE__

#ifndef _CLASS_SLOPPY_
#define _CLASS_SLOPPY_
template<class T>
class sloppy {};

// convert between T** and const T**
template<class T>
class sloppy<T**>
{
    T** t;
public:
    sloppy(T** mt) : t(mt) {}
    sloppy(const T** mt) : t(const_cast<T**>(mt)) {}
    
    operator T** () const { return t; }
    operator const T** () const { return const_cast<const T**>(t); }
};
#endif

#endif

class STK_TOOLKIT_EXPORT Stk_Platform
{
#ifdef __MOBILE__
private:
	static int wlen(const STK_WCHAR* strString);						//!< //取得ushort型（2字节）字符串的长度
	static short l2b(const short* pSrc);								//!< short 小头转大头
	static void makePWCHARBig(short* pSrcStr, int len);					//!< 将WCHAR字符串的制定长度转为大头
	static int UCNVConvert(const char* tarEncode,						//!< 调用系统icu库的编码转换函数
							const char* srcEncode,
							char* tarBuf,
							int32_t tarBuffLen,
							const char* srcBuf,
							int32_t srcBufLen,
							int32_t* pErrorCode
							);
    
#ifdef __IOS__
	static  int code_convert( const char *from_charset, const char *to_charset,
			const char *inbuf, size_t inlen, char *outbuf, size_t outlen );
#endif

public:
	/****************************************************************************
	*	@brief		将STK_WCHAR(2字节)字符串转换为Andriod/IOS的wstring
	 *	@param[in]	srcStr					STK_WCHAR字符串
	 *	@param[out]	无
	 *	@retval		wstrTmp					wstring类型字符串
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	static std::wstring STKWCHARStrToWString(const STK_WCHAR* srcStr);

	/****************************************************************************
	 *	@brief		将Andriod/IOS的wstring转换为STK_WCHAR字符串
	 *	@param[in]	srcStr					wstring类型字符串
	 *	@param[out]	无
	 *	@retval		wstrTmp					STK_WCHAR字符串
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	static STK_WCHAR* WStringToSTKWCHARStr(const std::wstring& srcStr);

	/****************************************************************************
	 *	@brief		将Andriod/IOS的string转换为wstring
	 *	@param[in]	srcStr					string类型字符串
	 *	@param[in]	srcEncode				编码类型
	 *	@param[out]	无
	 *	@retval								wstring类型字符串
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	static std::wstring StringToWString(const std::string& srcStr,const char* srcEncode="utf8");

	/****************************************************************************
	 *	@brief		将Andriod/IOS的wstring转换为string
	 *	@param[in]	srcStr					wstring类型字符串
	 *	@param[out]	无
	 *	@retval								string类型字符串
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	static std::string WStringToString(const std::wstring& srcStr);
#else

public:
	/****************************************************************************
	 *	@brief		将Windows的string转换为wstring
	 *	@param[in]	i_Str					string类型字符串
	 *	@param[out]	无
	 *	@retval								wstring类型字符串
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	static std::wstring	StringToWString(const std::string& i_Str);
	
	/****************************************************************************
	 *	@brief		将Windows的wstring转换为string
	 *	@param[in]	i_wstr					wstring类型字符串
	 *	@param[out]	无
	 *	@retval								string类型字符串
	 *	@note		无
	 *	@attention	无
	 ****************************************************************************/
	static std::string	WStringToString(const std::wstring& i_wstr);
#endif
};

	}//svllib
}//hoteamsoft


#endif
