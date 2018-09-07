#ifndef READLICDATA_HEAD_FILE
#define	READLICDATA_HEAD_FILE

#pragma once
#include <string>
using namespace std;

#define RegFileName "ProRegFile.dat"
#define LITEREGCODE "0000-0000-0000-0000"
#define	REGCODE_LEN	19

class CReadLicData
{
public:
	CReadLicData();
	~CReadLicData();

public:
	// �鿴ע�����ļ��Ƿ����
	bool IsRegFileExist(string strRegFile);
	// ��ȡ�ļ�
	int ReadRegFile(string strRegFile, string& strContent);
	// ��ȡ����Ŀ¼
	CString GetRunDirectory(CString strProduct, CString strVersion);
	// ��������
	string DecryptData(string& strContent);
	// �����ļ�
	CString EncryptFile(LPTSTR pszData);
};


#endif