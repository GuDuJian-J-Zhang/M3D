// All Rights Reserved. Copyright (C) 2014 HOTEAMSOFT, Ltd
/****************************************************************************
*	@file		SATools.h
*
*	@brief	定义动画字符串处理函数等工具
*
*
*	@par	历史:
*
****************************************************************************/

#ifndef _SATOOLS_H
#define _SATOOLS_H

#include "utf_utils.h"
#include <string>
using namespace std;

#ifdef USE_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif


#ifndef SA_POINTER_SIZED_INT
#ifdef POINTER_SIZED_INT
#   define SA_POINTER_SIZED_INT POINTER_SIZED_INT
#   define SA_POINTER_SIZED_UINT unsigned POINTER_SIZED_INT 
#else
#if defined(WIN64) || defined(_M_X64) || defined(_WIN64) || defined(_M_AMD64)
#   define SA_POINTER_SIZED_INT __int64
#   define SA_POINTER_SIZED_UINT unsigned __int64
#else
#   define SA_POINTER_SIZED_INT long
#   define SA_POINTER_SIZED_UINT unsigned long
#endif
#endif
#endif

#define SA_V2I(x) ((SA_POINTER_SIZED_INT)(x))
#define SA_I2V(x) ((void *)(SA_POINTER_SIZED_INT)(x))


#ifdef _MSC_VER
#pragma warning(disable: 4127 4310 4244 4706 4206 4514 4100 4996)
#ifndef WINDOWS_SYSTEM
#define WINDOWS_SYSTEM
#endif

#ifdef SA_EXPORTS
#    define SA_API  __declspec (dllexport)
#else
#    define SA_API  __declspec (dllimport)
#endif

#else
#define SA_API
#endif

#ifdef WINDOWS_SYSTEM
#	ifdef _WIN32_WCE
#	define stricmp strcasecmp
#	define wcsicmp wcscasecmp
#	endif
#	define NO_CASE_CMP(x,y) stricmp(x,y)
#	define WNO_CASE_CMP(x,y) wcsicmp(x,y)
#   define _tcseq(a,b)		(!_tcscmp(a,b))
#   define _tcsieq(a,b)		(!_tcsicmp(a,b))
#else
#	define NO_CASE_CMP(x,y) strcasecmp(x,y)
#	define WNO_CASE_CMP(x,y) wcscasecmp(x,y)
#endif

#define streq(a,b)      (!strcmp(a,b))
#define wcseq(a,b)		(!wcscmp(a,b))
#define strieq(a,b)     (!NO_CASE_CMP(a,b))
#define wcsieq(a,b)		(!WNO_CASE_CMP(a,b))

#define feq(a, b, eps)	((fabs((a)-(b)) < (eps)))

/* define MIN and MAX here rather than earlier in the file because
 * some system headers define their own (compatible) macros */
#ifndef MIN
# define MIN(a, b)		((a) < (b) ? (a) : (b))
#endif
#ifndef MAX
# define MAX(a, b)		((a) > (b) ? (a) : (b))
#endif

#include "SAGlobals.h"

// SACLOCALE does whatever operation it is given in the standard "C" locale,
// rather than whatever had been defined previously.  This prevents things like
// sprintf("%f", 3.14f) from coming out as "3,14" when set to european locales (or
// those of most of the rest of the world, for that matter.

#ifdef _WIN32_WCE
#define SACLOCALE(x) x
#else
#include <locale.h>
#define SACLOCALE(x) do { \
    char *origL; \
    origL = setlocale(LC_NUMERIC, "C"); \
    x; \
    setlocale(LC_NUMERIC, origL); \
} while(0)
#endif

//#define H_EXTRA_POINTER_FORMAT SAUtility::extra_pointer_format() 

#ifndef INVALID_KEY
#define INVALID_KEY (-1L)
#endif

#ifndef _WIN32_WCE
#include <assert.h>
#else  
#define assert(exp)    ((void)0)
#endif

#ifdef USE_PACK_8
#pragma pack(pop)
#endif

class SATools
{
public:
	static std::wstring c2w(const char* pc);	//char* 转换成 wchar
	static std::string	w2c(const wchar_t* pw);	//wchar 转换成 char*
	static int VersionCompare(const char* version1,const char* version2);
	static void PlcPath16To10(const char *oldPlcPath, char *newPlcPath);
	static int StrReplace(char strRes[],char from[], char to[]);

	static void		FindFileNameExtension(const char * filename_in, char * ext_out, int size = -1);

};

#endif


