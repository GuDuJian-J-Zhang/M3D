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
	// 查看注册码文件是否存在
	bool IsRegFileExist(string strRegFile);
	// 读取文件
	int ReadRegFile(string strRegFile, string& strContent);
	// 获取运行目录
	CString GetRunDirectory(CString strProduct, CString strVersion);
	// 解密数据
	string DecryptData(string& strContent);
	// 加密文件
	CString EncryptFile(LPTSTR pszData);
};


#endif