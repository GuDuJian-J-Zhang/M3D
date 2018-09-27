// ChineseCode.h: interface for the CChineseCode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHINESECODE_H__E9206178_CA96_4B8A_AB2A_AD94C9CD12B1__INCLUDED_)
#define AFX_CHINESECODE_H__E9206178_CA96_4B8A_AB2A_AD94C9CD12B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <string>
using namespace std;


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


class ChineseCode
{
   public:
       static void		UTF_8ToUnicode(wchar_t* pOut,char *pText);  // ∞—UTF-8◊™ªª≥…Unicode
       static void		UnicodeToUTF_8(char* pOut,wchar_t* pText);  //Unicode ◊™ªª≥…UTF-8
#ifdef __MOBILE__
   private:
   	static int wlen(const wchar_t* strString);						//!< //»°µ√ushort–Õ£®2◊÷Ω⁄£©◊÷∑˚¥Æµƒ≥§∂»
   	static short l2b(const short* pSrc);								//!< short –°Õ∑◊™¥ÛÕ∑
   	static void makePWCHARBig(short* pSrcStr, int len);					//!< Ω´WCHAR◊÷∑˚¥Æµƒ÷∆∂®≥§∂»◊™Œ™¥ÛÕ∑
   	static int UCNVConvert(const char* tarEncode,						//!< µ˜”√œµÕ≥icuø‚µƒ±‡¬Î◊™ªª∫Ø ˝
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
   	*	@brief		Ω´wchar_t(2◊÷Ω⁄)◊÷∑˚¥Æ◊™ªªŒ™Andriod/IOSµƒwstring
   	 *	@param[in]	srcStr					wchar_t◊÷∑˚¥Æ
   	 *	@param[out]	Œﬁ
   	 *	@retval		wstrTmp					wstring¿‡–Õ◊÷∑˚¥Æ
   	 *	@note		Œﬁ
   	 *	@attention	Œﬁ
   	 ****************************************************************************/
   	static std::wstring STKWCHARStrToWString(const wchar_t* srcStr);

   	/****************************************************************************
   	 *	@brief		Ω´Andriod/IOSµƒwstring◊™ªªŒ™wchar_t◊÷∑˚¥Æ
   	 *	@param[in]	srcStr					wstring¿‡–Õ◊÷∑˚¥Æ
   	 *	@param[out]	Œﬁ
   	 *	@retval		wstrTmp					wchar_t◊÷∑˚¥Æ
   	 *	@note		Œﬁ
   	 *	@attention	Œﬁ
   	 ****************************************************************************/
   	static wchar_t* WStringToSTKWCHARStr(const std::wstring& srcStr);

   	/****************************************************************************
   	 *	@brief		Ω´Andriod/IOSµƒstring◊™ªªŒ™wstring
   	 *	@param[in]	srcStr					string¿‡–Õ◊÷∑˚¥Æ
   	 *	@param[in]	srcEncode				±‡¬Î¿‡–Õ
   	 *	@param[out]	Œﬁ
   	 *	@retval								wstring¿‡–Õ◊÷∑˚¥Æ
   	 *	@note		Œﬁ
   	 *	@attention	Œﬁ
   	 ****************************************************************************/
   	static std::wstring StringToWString(const std::string& srcStr,const char* srcEncode="utf8");

   	/****************************************************************************
   	 *	@brief		Ω´Andriod/IOSµƒwstring◊™ªªŒ™string
   	 *	@param[in]	srcStr					wstring¿‡–Õ◊÷∑˚¥Æ
   	 *	@param[out]	Œﬁ
   	 *	@retval								string¿‡–Õ◊÷∑˚¥Æ
   	 *	@note		Œﬁ
   	 *	@attention	Œﬁ
   	 ****************************************************************************/
   	static std::string WStringToString(const std::wstring& srcStr,const char* srcEncode="utf8");

	/****************************************************************************
   	 *	@brief		Ω´Andriod/IOSµƒstring utf8◊™ªªŒ™wstring
   	 *	@param[out]	wstrOut				wstring¿‡–Õ◊÷∑˚¥Æ
   	 *	@param[in]	pText				 ‰»Î◊÷∑˚¥Æ
   	 *	@retval		Œﬁ
   	 *	@note		Œﬁ
   	 *	@attention	Œﬁ
   	 ****************************************************************************/
   	static void		UTF_8ToUnicode(wstring &wstrOut, const string &strUTFText);
   	static wstring		UTF_8ToUnicode(const char *pText);

   	/****************************************************************************
   	   	 *	@brief		Ω´Andriod/IOSµƒwstring◊™ªªŒ™utf8
   	   	 *	@param[in]	pUnicode				wstring¿‡–Õ◊÷∑˚¥Æ
   	   	 *	@param[out]	pText					UTF8 ‰»Î◊÷∑˚¥Æ
   	   	 *	@retval		Œﬁ
   	   	 *	@note		Œﬁ
   	   	 *	@attention	Œﬁ
   	   	 ****************************************************************************/
   	static void		UnicodeToUTF_8(const wchar_t* pUnicode, string &oUFT8);
#else
       static void		UTF_8ToUnicode(wstring &wstrOut, char *pText);
	   static void		UnicodeToUTF_8(const wchar_t* pUnicode, string &oUFT8);
	   static void		UTF_8ToUnicode(wstring &wstrOut, const string &strUTFText);
       static wstring	UTF_8ToUnicode(const char* strUTFText);
       static void		UnicodeToGB2312(char* pOut,wchar_t uData);  // ∞—Unicode ◊™ªª≥… GB2312
       static void		Gb2312ToUnicode(wchar_t* pOut,char *gbBuffer);// GB2312 ◊™ªª≥…°°Unicode
       static void		GB2312ToUTF_8(string& pOut,char *pText, int pLen);//GB2312 ◊™Œ™ UTF-8
       static void		UTF_8ToGB2312(string &pOut, char *pText, int pLen);//UTF-8 ◊™Œ™ GB2312
	   static wstring StringToWString(const std::string& s);	//string->wstring◊™ªª
	   static string WStringToString(const std::wstring& ws); //wstring->string◊™ªª
#endif
};

#endif // !defined(AFX_CHINESECODE_H__E9206178_CA96_4B8A_AB2A_AD94C9CD12B1__INCLUDED_)
