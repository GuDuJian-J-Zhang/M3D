// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_Platform.h
 *
 *	@brief	公用接口声明
 *
 *	@par	历史:
 *		2014/4/16	创建。Xuli@HOTEAMSOFT
 ****************************************************************************/

#include "StdAfx.h"
#include "Stk_Platform.h"

#ifdef __MOBILE__

#ifdef __ANDROID__
#include <jni.h>
#include "sys/system_properties.h"
#endif

#ifdef __IOS__
#include <iconv.h>
#endif

#include "dlfcn.h"
#include <sys/stat.h>
#include <sstream>
#include "Utility.h"

/****************************************************************************
 *	@brief		取锟斤拷ushort锟酵ｏ拷2锟街节ｏ拷锟街凤拷某锟斤拷锟�
 *	@param[in]	strString				原锟街凤拷
 *	@param[out]	锟斤拷
 *	@retval		tmpwstr					锟街凤拷某锟斤拷锟�
 *	@note		锟斤拷
 *	@attention	锟斤拷
 ****************************************************************************/
int Stk_Platform::wlen(const STK_WCHAR* strString) {
	int i = 0;
	while (1) {
		if (strString[i] == 0) {
			break;
		} else {
			i++;
		}
	}
	return i;
}

/****************************************************************************
 *	@brief		short 小头转锟斤拷头
 *	@param[in]	pSrc					原锟街凤拷
 *	@param[out]	锟斤拷
 *	@retval		
 *	@note		锟斤拷
 *	@attention	锟斤拷
 ****************************************************************************/

short Stk_Platform::l2b(const short* pSrc) {
	return (*pSrc >> 8) & 0xff | (*pSrc << 8) & 0xff00;
}
void* Stk_Platform::pDL = dlopen("/system/lib/libicuuc.so", RTLD_LAZY);
;
/****************************************************************************
 *	@brief		锟斤拷WCHAR锟街凤拷锟斤拷贫锟斤拷锟斤拷锟阶拷锟酵�
 *	@param[in]	pSrcStr					原锟街凤拷
 *	@param[in]	len						锟街凤拷锟斤拷
 *	@param[out]	pSrcStr					目锟斤拷锟街凤拷
 *	@retval		锟斤拷
 *	@note		锟斤拷
 *	@attention	锟斤拷
 ****************************************************************************/

void Stk_Platform::makePWCHARBig(short* pSrcStr, int len) {
	for (int i = 0; i < len; i++) {
		short* pTmp = pSrcStr + i;
		*pTmp = l2b(pTmp);
	}
}

// std::wstring Stk_Platform::CHARSToWString(const char* c,const char* srcEncode)
// {
// 	std::string str(c);
// 	return StringToWString(str,srcEncode);
// }

/****************************************************************************
 *	@brief		string->wstring转锟斤拷
 *	@param[in]	s						string锟斤拷锟斤拷锟街凤拷
 *	@param[in]	local					原锟斤拷锟斤拷
 *	@param[out]	锟斤拷
 *	@retval		tmpwstr					wstring锟斤拷锟斤拷锟街凤拷
 *	@note		锟斤拷
 *	@attention	锟斤拷
 ****************************************************************************/

std::wstring Stk_Platform::StringToWString(const std::string& s,
		const char* local) {
	const char* srcStr = (const char*) s.c_str();
	int srcLen = s.size() + 1;
	int tarBufLen = srcLen * 4;
	char tarBuf[tarBufLen];
	memset(tarBuf, '\0', tarBufLen);

	int errCode = 0;
	int ret = 0;
	ret = UCNVConvert(UTF_32LE, local, tarBuf, tarBufLen, srcStr, srcLen,
			&errCode);
	wstring tmpwstr = (wchar_t*) tarBuf;

	return tmpwstr;
}

/****************************************************************************
 *	@brief		wstring->string转锟斤拷
 *	@param[in]	ws						原锟街凤拷wstring
 *	@param[out]	锟斤拷
 *	@retval		tmpstr					string锟斤拷锟斤拷锟街凤拷
 *	@note		锟斤拷
 *	@attention	锟斤拷
 ****************************************************************************/

std::string Stk_Platform::WStringToString(const std::wstring& ws) {
	if (ws.length() == 0) {
		return "";
	}

	const char* srcStr = (const char*) ws.c_str();
	int srcLen = (ws.size()) * 4;
	int tarBufLen = srcLen;
	char tarBuf[tarBufLen];
	memset(tarBuf, '\0', tarBufLen);

	int errCode = 0;
	int ret = 0;
	ret = UCNVConvert(UTF_8, UTF_32LE, tarBuf, tarBufLen, srcStr, srcLen,
			&errCode);
	string tmpstr = tarBuf;

	return tmpstr;
}

/****************************************************************************
 *	@brief		//锟斤拷2锟街斤拷WCHAR*转锟斤拷为4锟街斤拷wchar_t
 //53 0 56 0 4c 0 46 0 49 0 4c 0 45 0 0 0 =>
 //53 0 0 0 56 0 0 0 4c 0 0 0 46 0 0 0 49 0 0 0 4c 0 0 0 45 0 0 0 0 0 0 0
 *	@param[in]	srcStr					原锟街凤拷
 *	@param[out]	tmpwstr					目锟斤拷锟街凤拷
 *	@retval		锟斤拷
 *	@note		锟斤拷
 *	@attention	锟斤拷
 ****************************************************************************/
wstring Stk_Platform::STKWCHARStrToWString(const STK_WCHAR * srcStr) {
	int srcLen = wlen(srcStr);
	int tarBufLen = (srcLen + 2) * 4;
	char tarBuf[tarBufLen];
	memset(tarBuf, 0x0, tarBufLen);

	int errCode = 0;
	int ret = 0;
	ret = UCNVConvert2Android(UTF_32LE, UTF_16LE, tarBuf, tarBufLen,
			(const char*) srcStr, srcLen * 2, &errCode);
	wstring tmpwstr = (wchar_t*) tarBuf;

	return tmpwstr;
}

/****************************************************************************
 *	@brief		锟斤拷32位wchar_t转为16位ushort(锟皆讹拷锟斤拷锟絎CHAR)
 *	@param[in]	ws						原锟街凤拷
 *	@param[out]	outWCHAR				目锟斤拷锟街凤拷
 *	@retval		锟斤拷
 *	@note		锟斤拷
 *	@attention	锟斤拷
 ****************************************************************************/
STK_WCHAR* Stk_Platform::WStringToSTKWCHARStr(const std::wstring& ws) {
	const char* srcStr = (const char*) ws.c_str();
	int srcLen = (ws.size()) * 4;
	int tarBufLen = srcLen + 4;
	char tarBuf[tarBufLen];

	memset(tarBuf, '\0', tarBufLen);

	int errCode = 0;
	int ret = 0;
	ret = UCNVConvert(UTF_16LE, UTF_32LE, tarBuf, tarBufLen, srcStr, srcLen,
			&errCode);

	STK_WCHAR* outWCHAR = (STK_WCHAR*) malloc(tarBufLen);
	memcpy(outWCHAR, tarBuf, tarBufLen);

	///TODO 杩欓噷浼氬鑷村唴瀛樻硠婕�璇锋兂鍔炴硶澶勭悊
	return outWCHAR;
}

#ifdef __IOS__
int Stk_Platform::code_convert( const char *from_charset, const char *to_charset, const char *inbuf, size_t inlen, char *outbuf, size_t outlen )
{
	iconv_t cd;
	const char *temp = inbuf;
	const char **pin = &temp;
	char **pout = &outbuf;
	memset(outbuf,0,outlen);
	cd = iconv_open(to_charset,from_charset);
	if(cd==0) return -1;
	if(iconv(cd,sloppy<char**>(pin),&inlen,pout,&outlen)==-1) return -1;
	iconv_close(cd);
	return 0;
}
#endif

/****************************************************************************
 *	@brief		锟斤拷锟斤拷系统icu锟斤拷谋锟斤拷锟阶拷锟斤拷锟斤拷锟�
 *	@param[in]	tarEncode				目锟斤拷锟斤拷锟�
 *	@param[in]	srcEncode				原锟斤拷锟斤拷
 *	@param[in]	tarBuf					目锟斤拷buf
 *	@param[in]	tarBufLen				目锟斤拷锟斤拷锟斤拷byte锟斤拷
 *	@param[in]	srcBuf					原buf
 *	@param[in]	srcBufLen				原锟斤拷锟斤拷byte锟斤拷
 *	@param[out]	pErrorCode				锟斤拷锟截的达拷锟斤拷锟斤拷
 *	@retval		ret						锟斤拷全转锟斤拷锟斤拷要锟斤拷锟街节筹拷锟饺ｏ拷byte锟斤拷
 *	@note		锟斤拷
 *	@attention	锟斤拷
 ****************************************************************************/
char* convertFunName = NULL;
int Stk_Platform::UCNVConvert2Android(const char* tarEncode,
		const char* srcEncode, char* tarBuf, int32_t tarBufLen,
		const char* srcBuf, int32_t srcBufLen, int32_t* pErrorCode) {
	int ret = -1;
#ifdef __IOS__
	ret = code_convert(srcEncode,tarEncode,srcBuf,srcBufLen,tarBuf,tarBufLen);
#endif

#ifdef __ANDROID__
	//锟斤拷锟斤拷锟斤拷指锟斤拷
	int (*ucnv_convert)(const char*, const char*, char*, int32_t, const char*,
			int32_t, int32_t*)=0;

	//锟斤拷锟截讹拷态锟斤拷
	char ver[50];
	__system_property_get("ro.build.version.sdk", ver);
	int verNo = atoi(ver);
	//	LOGE("sys ver:%s",ver);

	if (verNo >= 26) //android 8.0 N =26;
			{
		convertFunName = "ucnv_convert_58";
	} else if (verNo >= 24) //android 7.0 N =24;
			{
		convertFunName = "ucnv_convert_56";
	} else if (verNo >= 23) //android 6.0 M =23;
			{
		convertFunName = "ucnv_convert_55";
	} else if (verNo >= 21) //android 5.0 lollipop =21;android5.1 lollipop mr1 = 22
			{
		convertFunName = "ucnv_convert_53";
	} else if (verNo >= 19) //android 4.4 kitkat and 4.4w kitkat watch
			{
		convertFunName = "ucnv_convert_51";
	} else if (verNo >= 18) //jelly bean mr2 android 4.3
			{
		convertFunName = "ucnv_convert_50";
	} else if (verNo >= 16) //jelly bean android4.2
			{
		convertFunName = "ucnv_convert_48";
	} else if (verNo >= 14) //ice cream sandwich 4.0
			{
		convertFunName = "ucnv_convert_46";
	} else if (verNo >= 9) {
		convertFunName = "ucnv_convert_44";
	} else if (verNo >= 8) {
		convertFunName = "ucnv_convert_4_2";
	}

	//锟斤拷莅姹撅拷锟酵拷锟缴诧拷同锟斤拷缀锟斤拷ucnv_convert锟斤拷锟斤拷
	//android2.2为ucnv_convert_4_2
	//2.3=44
	//4.0=46
	//

	ucnv_convert = (int (*)(const char*, const char*, char*, int32_t,
			const char*, int32_t, int32_t*))dlsym(pDL,convertFunName);

	//使锟斤拷ucnv_convert锟斤拷锟斤拷锟斤拷锟阶拷锟斤拷址锟�
if(	ucnv_convert) {
		ret = ucnv_convert(tarEncode,srcEncode,tarBuf,tarBufLen,srcBuf,srcBufLen,pErrorCode);
	}
	else
	{
//		LOGE("ucnvConvert err: fun %s not found!",convertFunName);
	}
#endif
	return ret;
}
int Stk_Platform::UCNVConvert(const char* tarEncode, const char* srcEncode,
		char* tarBuf, int32_t tarBufLen, const char* srcBuf, int32_t srcBufLen,
		int32_t* pErrorCode) {
	int ret = -1;
#ifdef __IOS__
	ret = code_convert(srcEncode,tarEncode,srcBuf,srcBufLen,tarBuf,tarBufLen);
#endif

#ifdef __ANDROID__
	//锟斤拷锟斤拷锟斤拷指锟斤拷
	int (*ucnv_convert)(const char*, const char*, char*, int32_t, const char*,
			int32_t, int32_t*)=0;

	//锟斤拷锟截讹拷态锟斤拷
	void* pDL = dlopen("/system/lib/libicuuc.so", RTLD_LAZY);

	//锟斤拷莅姹撅拷锟酵拷锟缴诧拷同锟斤拷缀锟斤拷ucnv_convert锟斤拷锟斤拷
	//android2.2为ucnv_convert_4_2
	//2.3=44
	//4.0=46
	//
	char ver[50];
	__system_property_get("ro.build.version.sdk", ver);
	int verNo = atoi(ver);
	//	LOGE("sys ver:%s",ver);
	char* convertFunName;
	if (verNo >= 26) //android 8.0 N =26;
			{
		convertFunName = "ucnv_convert_58";
	} else if (verNo >= 24) //android 7.0 N =24;
			{
		convertFunName = "ucnv_convert_56";
	} else if (verNo >= 23) //android 6.0 M =23;
			{
		convertFunName = "ucnv_convert_55";
	} else if (verNo >= 21) //android 5.0 lollipop =21;android5.1 lollipop mr1 = 22
			{
		convertFunName = "ucnv_convert_53";
	} else if (verNo >= 19) //android 4.4 kitkat and 4.4w kitkat watch
			{
		convertFunName = "ucnv_convert_51";
	} else if (verNo >= 18) //jelly bean mr2 android 4.3
			{
		convertFunName = "ucnv_convert_50";
	} else if (verNo >= 16) //jelly bean android4.2
			{
		convertFunName = "ucnv_convert_48";
	} else if (verNo >= 14) //ice cream sandwich 4.0
			{
		convertFunName = "ucnv_convert_46";
	} else if (verNo >= 9) {
		convertFunName = "ucnv_convert_44";
	} else if (verNo >= 8) {
		convertFunName = "ucnv_convert_4_2";
	}

	ucnv_convert = (int (*)(const char*, const char*, char*, int32_t,
			const char*, int32_t, int32_t*))dlsym(pDL,convertFunName);

	//使锟斤拷ucnv_convert锟斤拷锟斤拷锟斤拷锟阶拷锟斤拷址锟�
if(	ucnv_convert) {
		ret = ucnv_convert(tarEncode,srcEncode,tarBuf,tarBufLen,srcBuf,srcBufLen,pErrorCode);
	}
	else
	{
//		LOGE("ucnvConvert err: fun %s not found!",convertFunName);
	}
#endif
	return ret;
}

#else
/****************************************************************************
 *	@brief		将Windows的string转换为wstring
 *	@param[in]	i_Str					string类型字符串
 *	@param[out]	无
 *	@retval								wstring类型字符串
 *	@note		无
 *	@attention	无
 ****************************************************************************/
std::wstring Stk_Platform::StringToWString(const std::string& i_Str)
{
	std::wstring wstrTmp;
	STK_INT32 niLength = MultiByteToWideChar(CP_ACP, 0, i_Str.c_str(), -1, NULL, NULL);
	wstrTmp.resize(niLength);
	LPWSTR lpwszStr = new wchar_t[niLength];
	MultiByteToWideChar(CP_ACP, 0, i_Str.c_str(), -1, lpwszStr, niLength);
	wstrTmp = lpwszStr;
	delete [] lpwszStr;
	return wstrTmp;
}

/****************************************************************************
 *	@brief		将Windows的wstring转换为string
 *	@param[in]	i_wstr					wstring类型字符串
 *	@param[out]	无
 *	@retval								string类型字符串
 *	@note		无
 *	@attention	无
 ****************************************************************************/
std::string Stk_Platform::WStringToString(const std::wstring& i_wstr)
{
	if (i_wstr.length() == 0)
	{
		return "";
	}

	std::string strTmp;
	int nLength = WideCharToMultiByte(CP_ACP, 0, i_wstr.c_str(), -1, NULL, NULL,NULL,NULL);
	strTmp.resize(nLength);
	LPSTR lpszStr = new char[nLength];
	WideCharToMultiByte(CP_ACP, 0, i_wstr.c_str(), -1, lpszStr, nLength,NULL,NULL);
	strTmp = lpszStr;
	delete [] lpszStr;
	return strTmp;
}

#endif
