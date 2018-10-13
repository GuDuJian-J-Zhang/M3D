// LicVerify.h : LicVerify DLL ����ͷ�ļ�
//

#pragma once

#ifndef _LICVERIFY_H
#define _LICVERIFY_H

#include "resource.h"		// ������
#include <string>
#include <afxwin.h>
using namespace std;

// CLicVerifyApp
// �йش���ʵ�ֵ���Ϣ������� LicVerify.cpp
//

class CLicVerifyApp : public CWinApp
{
public:
	CLicVerifyApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

// --------------------------------------------------------------------------
//
// ��������
// 
// --------------------------------------------------------------------------
// ע��У��
extern "C" int __stdcall VerifyRegCode(char* pszRegCode, int AppFlag, char* pszVersion);
// ��ȡע����
extern "C" int __stdcall GetRegCode(char* pszRegCode, char* pszProduct, char* pszVersion);
// дע����
extern "C" void __stdcall SetRegCode(char* pszRegCode, char* pszProduct, char* pszVersion);
// ��ȡ�汾����
extern "C" int __stdcall GetEdition(char* pszRegCode);
// ��ȡʣ������
extern "C" int __stdcall GetSpareDate(char* pszRegCode);
// ϵͳʱ��У��
extern "C" int __stdcall VerifySystemTime(char* pszProduct, char* pszVersion);

// wstring->string
std::string WStringToString(const std::wstring& i_wstr);

// string->wstring
std::wstring StringToWString(const std::string& i_Str);

#endif