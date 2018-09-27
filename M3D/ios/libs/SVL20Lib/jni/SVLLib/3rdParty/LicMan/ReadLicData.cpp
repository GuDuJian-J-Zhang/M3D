// ReadLicData.cpp : ʵ���ļ�
//

//#include "..\..\StdAfx.h"
#include "DesUtil.h"
#include "ReadLicData.h"
#include <Shlwapi.h>
#include <io.h>

#pragma comment(lib,"Shlwapi.lib")

// ���캯��
CReadLicData::CReadLicData()
{
}

// ��������
CReadLicData::~CReadLicData()
{
}

// ��ȡ��ǰ����Ŀ¼
CString CReadLicData::GetRunDirectory(CString strProduct,CString strVersion)
{
	HKEY hkey;
	DWORD type = REG_SZ;
	DWORD cbData = MAX_PATH;
	CString strRecordFile;
	if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_LOCAL_MACHINE,_T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders"),0,KEY_READ,&hkey))
	{
		RegQueryValueEx(hkey,_T("Common AppData"),NULL,&type,(LPBYTE)strRecordFile.GetBuffer(MAX_PATH),&cbData);
		strRecordFile.ReleaseBuffer();
		strRecordFile.Append(_T("\\HoteamSoft"));
		CreateDirectory(strRecordFile,NULL);
	}
	else
	{
		GetModuleFileName(NULL, strRecordFile.GetBuffer(MAX_PATH), MAX_PATH);
		strRecordFile.ReleaseBuffer();
		int iPos = strRecordFile.ReverseFind(_T('\\'));
		strRecordFile = strRecordFile.Left(iPos);
	}
	RegCloseKey(hkey);
	
	if (!PathFileExists(strRecordFile))
	{
		CreateDirectory(strRecordFile,NULL);
	}	

	// ��ȡ�߰汾
	CString strHighVer = strVersion;
	int iPos = strHighVer.Find('.');
	if (iPos != -1)
	{
		strHighVer = strHighVer.Left(iPos);
	}
	strProduct.Append(strHighVer);
	strRecordFile.Append(_T("\\"));
	strRecordFile.Append(strProduct);
	if (!PathFileExists(strRecordFile))
	{
		CreateDirectory(strRecordFile,NULL);
	}

	return strRecordFile;
}

// ע�����ļ��Ƿ����
bool CReadLicData::IsRegFileExist(string strRegFile)
{
	// �鿴�ļ��Ƿ����
	if (access(strRegFile.c_str(), 0) == -1)
	{
		return false;
	}

	return true;
}

// ��ȡ�ļ�
int CReadLicData::ReadRegFile(string strRegFile, string& strContent)
{
	// ���ļ�
	try	
	{
		CString strRegFileTmp(strRegFile.c_str());
		CString strContentTmp;
		wstring wstrContent;
		CStdioFile cFile;
		if (0 == cFile.Open((LPCTSTR)strRegFileTmp, CStdioFile::modeRead))
		{
			return -1; // ���ļ�ʧ��
		}
		cFile.ReadString(strContentTmp);
		// CString->string
		wstrContent = strContentTmp.GetString();
		std::string strTmp;
		int nLength = WideCharToMultiByte(CP_ACP, 0, wstrContent.c_str(), -1, NULL, NULL,NULL,NULL);
		strContent.resize(nLength);
		LPSTR lpszStr = new char[nLength];
		WideCharToMultiByte(CP_ACP, 0, wstrContent.c_str(), -1, lpszStr, nLength,NULL,NULL);
		strContent = lpszStr;
		delete [] lpszStr;

		cFile.Close();
	}
	catch (CException* e)
	{
		e->Delete();
	}

	return 0;
}

// ��������
string CReadLicData::DecryptData(string& strContent)
{
	CDesUtil desObj;
	string strRegCode;

	const char key[9] = {'4','5','1','D','6','B','E','7','\0'};

	// ����
	desObj.DesDecryptString(strContent,strRegCode,key);

	return strRegCode;
}

#if 0
// �ļ�����
CString CReadLicData::EncryptFile(LPTSTR pszData)
{
	CString strEncrypt;

	// ������
	const char key[9] = {'5','3','F','D','A','B','E','7','\0'};

	CDesUtil desObj;

	int inlength,outlength;
	inlength = (int)strlen(pszData);
	if ((inlength%8) !=  0)
	{
		outlength = (inlength/8 +1) * 8;
	}
	else
	{ 
		outlength = inlength;
	}

	TCHAR* pszOutput = new TCHAR[outlength+1];
	ZeroMemory(pszOutput,outlength+1);

	// Des����
	desObj.DesBytesEncrypt(pszOutput,outlength,pszData,inlength,key);

	strEncrypt = pszOutput;

	// �ڴ��ͷ�
	if (pszOutput != NULL)
	{
		delete[] pszOutput;
		pszOutput = NULL;
	}

	return strEncrypt;
}
#endif