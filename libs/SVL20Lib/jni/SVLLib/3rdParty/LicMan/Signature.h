#ifndef SIGNATURE_HEAD_FILE
#define	SIGNATURE_HEAD_FILE

#pragma once

class CSignature
{
public:
	CSignature();
	~CSignature();

public:
	// MacУ��
	bool VerifyMac(string strMac1, string strMac2);
	// �Ƚ�Mac���
	bool CompareMac(string strMac,string strUtilMac);
	// Macѹ��
	int MacEncrypt( char *str, char *mac);
};

#endif