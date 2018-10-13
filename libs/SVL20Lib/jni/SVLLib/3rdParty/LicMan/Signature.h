#ifndef SIGNATURE_HEAD_FILE
#define	SIGNATURE_HEAD_FILE

#pragma once

class CSignature
{
public:
	CSignature();
	~CSignature();

public:
	// Mac校验
	bool VerifyMac(string strMac1, string strMac2);
	// 比较Mac相等
	bool CompareMac(string strMac,string strUtilMac);
	// Mac压缩
	int MacEncrypt( char *str, char *mac);
};

#endif