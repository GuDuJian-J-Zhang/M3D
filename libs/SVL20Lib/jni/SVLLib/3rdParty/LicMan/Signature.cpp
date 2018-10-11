// Signature.cpp : 实现文件
//
//#include "..\..\StdAfx.h"
#include "MD5.h"
#include "Util.h"
#include "Signature.h"
#include "LicVerify.h"

// 构造函数
CSignature::CSignature()
{
}

// 析构函数
CSignature::~CSignature()
{
}

// Mac校验
bool CSignature::VerifyMac(string strMac1, string strMac2)
{
	CStringArray saMac;
	CUtil::GetAllMac(saMac);

	for (int i=0; i<saMac.GetCount(); ++i)
	{
		CString strMac = saMac[i];
		strMac.Remove('-');
		// CString->string
		wstring wstrMacTmp = strMac.GetString();
		string strMacTmp = "";
		strMacTmp = WStringToString(wstrMacTmp);

		// 比较Mac是否相等
		if (CompareMac(strMac1,strMacTmp) || CompareMac(strMac2,strMacTmp))
		{
			return true;
		}
	}

	return false;
}

// 比较Mac相等
bool CSignature::CompareMac(string strMac, string strUtilMac)
{
	// 数据校验
	if (strMac.length() != 2) return false;
	if (strUtilMac.length() != 12) return false;

	// MD5加密并获取后4位
	strUtilMac = CMD5::GetMD5(const_cast<char*>(strUtilMac.c_str()));
	if (strUtilMac.length() > 4)
	{
		strUtilMac = strUtilMac.substr(strUtilMac.length() - 4, 4);
		basic_string <char>::iterator itor = ( strUtilMac.begin() + 2);
		strUtilMac.insert(itor, ':');
	}
	else
	{
		return false;
	}

	// Mac加密
	char szUtilMac[3]= {};
	MacEncrypt((char*)strUtilMac.c_str(),szUtilMac);
	//若为空，则用‘0’代替
	for (int i=0; i<2; ++i)
	{
		if (szUtilMac[i] == NULL)
		{
			szUtilMac[i] = '0';
		}
	}

	// 值判断
	if (strMac == szUtilMac)
	{
		return true;
	}

	return false;
}

// MAC压缩
int CSignature::MacEncrypt( char *str, char *mac)
{
	int i;
	char *s, *e;

	if ((mac == NULL) || (str == NULL))
	{
		return -1;
	}

	s = (char *) str;
	for (i = 0; i < 3; ++i)
	{
		mac[i] = s ? strtoul (s, &e, 16) : 0;
		if (s)
			s = (*e) ? e + 1 : e;
	}

	return 0;
}