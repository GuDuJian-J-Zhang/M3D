// Signature.cpp : ʵ���ļ�
//
//#include "..\..\StdAfx.h"
#include "MD5.h"
#include "Util.h"
#include "Signature.h"
#include "LicVerify.h"

// ���캯��
CSignature::CSignature()
{
}

// ��������
CSignature::~CSignature()
{
}

// MacУ��
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

		// �Ƚ�Mac�Ƿ����
		if (CompareMac(strMac1,strMacTmp) || CompareMac(strMac2,strMacTmp))
		{
			return true;
		}
	}

	return false;
}

// �Ƚ�Mac���
bool CSignature::CompareMac(string strMac, string strUtilMac)
{
	// ����У��
	if (strMac.length() != 2) return false;
	if (strUtilMac.length() != 12) return false;

	// MD5���ܲ���ȡ��4λ
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

	// Mac����
	char szUtilMac[3]= {};
	MacEncrypt((char*)strUtilMac.c_str(),szUtilMac);
	//��Ϊ�գ����á�0������
	for (int i=0; i<2; ++i)
	{
		if (szUtilMac[i] == NULL)
		{
			szUtilMac[i] = '0';
		}
	}

	// ֵ�ж�
	if (strMac == szUtilMac)
	{
		return true;
	}

	return false;
}

// MACѹ��
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