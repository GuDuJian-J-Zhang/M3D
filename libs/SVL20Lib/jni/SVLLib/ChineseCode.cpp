// ChineseCode.cpp: implementation of the ChineseCode class.
//
//////////////////////////////////////////////////////////////////////
#include "ChineseCode.h"
#include <stdio.h>
#include <stdlib.h>
#ifdef __MOBILE__

#else
#include <windows.h>
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
void ChineseCode::UTF_8ToUnicode(wchar_t* pOut,char *pText)
{
    char* uchar = (char *)pOut;

    uchar[1] = ((pText[0] & 0x0F) << 4) + ((pText[1] >> 2) & 0x0F);
    uchar[0] = ((pText[1] & 0x03) << 6) + (pText[2] & 0x3F);

    return;
}


void ChineseCode::UnicodeToUTF_8(char* pOut,wchar_t* pText)
{
    char* pchar = (char *)pText;

    pOut[0] = (0xE0 | ((pchar[1] & 0xF0) >> 4));
    pOut[1] = (0x80 | ((pchar[1] & 0x0F) << 2)) + ((pchar[0] & 0xC0) >> 6);
    pOut[2] = (0x80 | (pchar[0] & 0x3F));

    return;
}


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
#include <stdlib.h>


int ChineseCode::wlen(const wchar_t* strString)
{
	int i = 0;
	while (1)
	{
		if (strString[i] == 0)
		{
			break;
		}
		else
		{
			i++;
		}
	}
	return i;
}

short ChineseCode::l2b(const short* pSrc)
{
	return (*pSrc >> 8) & 0xff | (*pSrc << 8) & 0xff00;
}

void ChineseCode::makePWCHARBig(short* pSrcStr, int len)
{
	for (int i = 0; i < len; i++)
	{
		short* pTmp = pSrcStr + i;
		*pTmp = l2b(pTmp);
	}
}

std::wstring ChineseCode::StringToWString(const std::string& s,const char* srcEncode/*="utf8"*/)
{
	const char* srcStr = (const char*)s.c_str();
	int srcLen = s.size()+1;
	int tarBufLen = srcLen*4;
	char tarBuf[tarBufLen];
	memset(tarBuf, '\0', tarBufLen);

	int errCode = 0;
	int ret = 0;
	ret = UCNVConvert(UTF_32LE,srcEncode,tarBuf,
		tarBufLen,srcStr, srcLen, &errCode);
	wstring tmpwstr = (wchar_t*)tarBuf;

	return tmpwstr;
}

std::string ChineseCode::WStringToString(const std::wstring& ws,const char* srcEncode/*="utf8"*/)
{
	const char* srcStr = (const char*)ws.c_str();
	int srcLen = (ws.size())*4;
	int tarBufLen = srcLen;
	char tarBuf[tarBufLen];
	memset(tarBuf,'\0', tarBufLen);

	int errCode = 0;
	int ret = 0;
	ret = UCNVConvert(srcEncode, UTF_32LE, tarBuf,
		tarBufLen,srcStr, srcLen, &errCode);
	string tmpstr = tarBuf;

	return tmpstr;
}

wstring	ChineseCode::UTF_8ToUnicode(const char *pText)
{
	return StringToWString(pText);
}
void ChineseCode::UTF_8ToUnicode(wstring &wstrOut, const string &strUTFText)
{
	wstrOut = StringToWString(strUTFText);
}


void	ChineseCode::UnicodeToUTF_8(const wchar_t* pUnicode, string &oUFT8)
{
	oUFT8 = WStringToString((wstring)pUnicode);
}


wstring ChineseCode::STKWCHARStrToWString(const wchar_t * srcStr)
{
	int srcLen = wlen(srcStr);
	int tarBufLen = (srcLen + 2) * 4;
	char tarBuf[tarBufLen];
	memset(tarBuf, 0x0, tarBufLen);

	int errCode = 0;
	int ret = 0;
	ret = UCNVConvert(UTF_32LE, UTF_16LE, tarBuf,
		tarBufLen, (const char*) srcStr, srcLen * 2, &errCode);
	wstring tmpwstr = (wchar_t*) tarBuf;

	return tmpwstr;
}


wchar_t* ChineseCode::WStringToSTKWCHARStr(const std::wstring& ws)
{
	const char* srcStr = (const char*)ws.c_str();
	int srcLen = (ws.size())*4;
	int tarBufLen = srcLen + 4;
	char tarBuf[tarBufLen];

	memset(tarBuf,'\0', tarBufLen);

	int errCode = 0;
	int ret = 0;
	ret = UCNVConvert(UTF_16LE, UTF_32LE, tarBuf,
		tarBufLen,srcStr, srcLen, &errCode);

	wchar_t* outWCHAR = (wchar_t*)malloc(tarBufLen);
	memcpy(outWCHAR,tarBuf,tarBufLen);

	return outWCHAR;
}

#ifdef __IOS__
int ChineseCode::code_convert( const char *from_charset, const char *to_charset, const char *inbuf, size_t inlen, char *outbuf, size_t outlen )
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

int ChineseCode::UCNVConvert(
			const char* tarEncode,
			const char* srcEncode,
			char* tarBuf,
			int32_t tarBufLen,
			const char* srcBuf,
			int32_t srcBufLen,
			int32_t* pErrorCode
)
{
	int ret = -1;
#ifdef __IOS__
	ret = code_convert(srcEncode,tarEncode,srcBuf,srcBufLen,tarBuf,tarBufLen);
#endif

#ifdef __ANDROID__

	int (*ucnv_convert)(const char*, const char*, char*, int32_t, const char*,
		int32_t, int32_t*)=0;


	void* pDL = dlopen("/system/lib/libicuuc.so", RTLD_LAZY);


	char ver[50];
	__system_property_get("ro.build.version.sdk",ver);
	int verNo = atoi(ver);
	//	LOGE("sys ver:%s",ver);
	char convertFunName[128] ={0} ;

	if(verNo>=23) //android 6.0 M =23;
	{
		strcpy(convertFunName,"ucnv_convert_55");
	}else if(verNo>=21) //android 5.0 lollipop =21;android5.1 lollipop mr1 = 22
	{
		strcpy(convertFunName,"ucnv_convert_53");
	}else if(verNo>=19)//android 4.4 kitkat and 4.4w kitkat watch
	{
		strcpy(convertFunName,"ucnv_convert_51");
	}
	else if(verNo>=18)//jelly bean mr2 android 4.3
	{
		strcpy(convertFunName,"ucnv_convert_50");
	}
	else if(verNo>=16)//jelly bean android4.2
	{
		strcpy(convertFunName,"ucnv_convert_48");
	}
	else if(verNo>=14)//ice cream sandwich 4.0
	{
		strcpy(convertFunName,"ucnv_convert_46");
	}
	else if(verNo >=9)
	{
		strcpy(convertFunName,"ucnv_convert_44");
	}
	else if(verNo>=8)
	{
		strcpy(convertFunName,"ucnv_convert_4_2");
	}

	ucnv_convert = (int (*)(const char*, const char*, char*, int32_t,
		const char*, int32_t, int32_t*))dlsym(pDL,convertFunName);

	if(	ucnv_convert) {
		ret = ucnv_convert(tarEncode,srcEncode,tarBuf,tarBufLen,srcBuf,srcBufLen,pErrorCode);
	}
	else
	{
		//LOGE("ucnvConvert err: fun %s not found!",convertFunName);
	}
#endif
	return ret;
}

#else
void ChineseCode::UTF_8ToUnicode(wstring &wstrOut, char *pText)
{
	if(!pText || 0 == pText[0])
		return;

	wchar_t* pUnicodeText;
	int iTextbyte = MultiByteToWideChar(CP_UTF8, 0, pText, -1, NULL, 0);
	pUnicodeText = new wchar_t[iTextbyte];


	memset(pUnicodeText, 0, iTextbyte);
	::MultiByteToWideChar(CP_UTF8, 0, pText, -1, pUnicodeText, iTextbyte);

	wstrOut = pUnicodeText;
	delete []pUnicodeText;
	return;
}

void ChineseCode::UTF_8ToUnicode(wstring &wstrOut, const string &strUTFText)
{
	if(0 == strUTFText.size())
		return;

	wchar_t* pUnicodeText;
	int iTextbyte = MultiByteToWideChar(CP_UTF8, 0, strUTFText.c_str(), -1, NULL, 0);
	pUnicodeText = new wchar_t[iTextbyte];


	memset(pUnicodeText, 0, iTextbyte);
	::MultiByteToWideChar(CP_UTF8, 0, strUTFText.c_str(), -1, pUnicodeText, iTextbyte);

	wstrOut = pUnicodeText;
	delete []pUnicodeText;
}

wstring ChineseCode::UTF_8ToUnicode( const char* strUTFText )
{
	std::wstring wstrRet = L"";
	if(NULL == strUTFText)
		return wstrRet;

	wchar_t* pUnicodeText;
	int iTextbyte = MultiByteToWideChar(CP_UTF8, 0, strUTFText, -1, NULL, 0);
	pUnicodeText = new wchar_t[iTextbyte];


	memset(pUnicodeText, 0, iTextbyte);
	::MultiByteToWideChar(CP_UTF8, 0, strUTFText, -1, pUnicodeText, iTextbyte);

	wstrRet = pUnicodeText;
	delete []pUnicodeText;
	return wstrRet;
}

//Unicode转UTF-8
void ChineseCode::UnicodeToUTF_8(const wchar_t* pUnicode, string &oUFT8)
{
	if(!pUnicode || 0 == pUnicode[0])
		return;

	char	*pUTF8Text;
	int iTextbyte = WideCharToMultiByte(CP_UTF8, 0, pUnicode, -1, NULL, 0, NULL, NULL);
	pUTF8Text = new char[iTextbyte];

	memset(pUTF8Text, 0, iTextbyte);
	::WideCharToMultiByte(CP_UTF8, 0, pUnicode, -1, pUTF8Text, iTextbyte, NULL, NULL);
	oUFT8 = pUTF8Text;
	delete [] pUTF8Text;
}

void ChineseCode::UnicodeToGB2312(char* pOut,wchar_t uData)
{
    WideCharToMultiByte(CP_ACP,NULL,&uData,1,pOut,sizeof(wchar_t),NULL,NULL);
    return;
}     

void ChineseCode::Gb2312ToUnicode(wchar_t* pOut,char *gbBuffer)
{
    ::MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,gbBuffer,2,pOut,1);
    return ;
}

void ChineseCode::GB2312ToUTF_8(string& pOut,char *pText, int pLen)
{
    char buf[4];
    int nLength = (pLen+1)* 3;
    char* rst = new char[nLength];
    
    memset(buf,0,4);
    memset(rst,0,nLength);
    
    int i = 0;
    int j = 0;      
    while(i < pLen)
    {
            //如果是英文直接复制就可以
            if( *(pText + i) >= 0)
            {
                    rst[j++] = pText[i++];
            }
            else
            {
                    wchar_t pbuffer;
                    Gb2312ToUnicode(&pbuffer,pText+i);
                    
                    UnicodeToUTF_8(buf,&pbuffer);
                    
                    unsigned short int tmp = 0;
                    tmp = rst[j] = buf[0];
                    tmp = rst[j+1] = buf[1];
                    tmp = rst[j+2] = buf[2];    
                    
                    j += 3;
                    i += 2;
            }
    }
    rst[j] = '\0';

    //返回结果
    pOut = rst;             
    delete []rst;   
    
    return;
}

void ChineseCode::UTF_8ToGB2312(string &pOut, char *pText, int pLen)
{
    char * newBuf = new char[pLen+1];
    char Ctemp[4];
    memset(Ctemp,0,4);

    int i =0;
    int j = 0;
    
    while(i < pLen)
    {
        if(pText[i] > 0)
        {
                newBuf[j++] = pText[i++];                       
        }
        else                 
        {
                wchar_t Wtemp;
                UTF_8ToUnicode(&Wtemp,pText + i);
        
                UnicodeToGB2312(Ctemp,Wtemp);
            
                newBuf[j] = Ctemp[0];
                newBuf[j + 1] = Ctemp[1];

                i += 3;    
                j += 2;   
        }
    }
    newBuf[j] = '\0';
    pOut = newBuf;
    delete []newBuf;
    
    return; 
}

wstring ChineseCode::StringToWString( const std::string& s )
{
	std::wstring wszStr;
	int nLength = MultiByteToWideChar(CP_ACP, 0, s.c_str(), -1, NULL, NULL);
	wszStr.resize(nLength);
	LPWSTR lpwszStr = new wchar_t[nLength];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), -1, lpwszStr, nLength);
	wszStr = lpwszStr;
	delete [] lpwszStr;
	return wszStr;
}

string ChineseCode::WStringToString( const std::wstring& ws )
{
	std::string szStr;
	int nLength = WideCharToMultiByte(CP_ACP, 0, ws.c_str(), -1, NULL, NULL,NULL,NULL);
	szStr.resize(nLength);
	LPSTR lpszStr = new char[nLength];
	WideCharToMultiByte(CP_ACP, 0, ws.c_str(), -1, lpszStr, nLength,NULL,NULL);
	szStr = lpszStr;
	delete [] lpszStr;
	return szStr;
}
#endif
