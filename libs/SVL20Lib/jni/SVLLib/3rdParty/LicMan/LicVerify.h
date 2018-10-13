// LicVerify.h : LicVerify DLL 的主头文件
//

#pragma once

#ifndef _LICVERIFY_H
#define _LICVERIFY_H

#include "resource.h"		// 主符号
#include <string>
#include <afxwin.h>
using namespace std;

// CLicVerifyApp
// 有关此类实现的信息，请参阅 LicVerify.cpp
//

class CLicVerifyApp : public CWinApp
{
public:
	CLicVerifyApp();

// 重写
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

// --------------------------------------------------------------------------
//
// 函数声明
// 
// --------------------------------------------------------------------------
// 注册校验
extern "C" int __stdcall VerifyRegCode(char* pszRegCode, int AppFlag, char* pszVersion);
// 获取注册码
extern "C" int __stdcall GetRegCode(char* pszRegCode, char* pszProduct, char* pszVersion);
// 写注册码
extern "C" void __stdcall SetRegCode(char* pszRegCode, char* pszProduct, char* pszVersion);
// 获取版本类型
extern "C" int __stdcall GetEdition(char* pszRegCode);
// 获取剩余期限
extern "C" int __stdcall GetSpareDate(char* pszRegCode);
// 系统时间校验
extern "C" int __stdcall VerifySystemTime(char* pszProduct, char* pszVersion);

// wstring->string
std::string WStringToString(const std::wstring& i_wstr);

// string->wstring
std::wstring StringToWString(const std::string& i_Str);

#endif