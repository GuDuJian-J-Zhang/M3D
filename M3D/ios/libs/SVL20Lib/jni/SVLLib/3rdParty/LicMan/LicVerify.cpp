// LicVerify.cpp : ���� DLL �ĳ�ʼ�����̡�
//

//#include "..\..\StdAfx.h"
#include "LicVerify.h"
#include "Signature.h"
#include "DataProcess.h"
#include "ReadLicData.h"

#define LICIT_REG_LASTTIMERECORD						_T("Software\\HoteamSoft\\LicMan_l\\")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
//		��Ӵ� DLL �������κε���
//		MFC �ĺ������뽫 AFX_MANAGE_STATE ����ӵ�
//		�ú�������ǰ�档
//
//		����:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ������ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//


// CLicVerifyApp

BEGIN_MESSAGE_MAP(CLicVerifyApp, CWinApp)
END_MESSAGE_MAP()


// CLicVerifyApp ����

CLicVerifyApp::CLicVerifyApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CLicVerifyApp ����

CLicVerifyApp theApp;
CReadLicData readLicObj;

// CLicVerifyApp ��ʼ��

BOOL CLicVerifyApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

// ��������
void * __stdcall ExportFunc(int FuncNum)
{
	void *pFunc = NULL;
	switch (FuncNum)
	{
	case 1:  pFunc = VerifyRegCode; break;
	case 2:  pFunc = GetRegCode; break;
	case 3:  pFunc = SetRegCode; break;
	case 4:  pFunc = GetEdition; break;
	case 5:  pFunc = GetSpareDate; break;
	case 6:  pFunc = VerifySystemTime; break;
	default: pFunc = NULL;
	}

	return pFunc;
}

// ע����֤
int __stdcall VerifyRegCode(char* pszRegCode, int AppFlag, char* pszVersion)
{
	string	strRegCode = "";
	string	strContent = "";
	
	// Lite�汾�ж�
	if (strContent == LITEREGCODE)
	{
		return 0;
	}

	// ȥ��"-"
	while(*pszRegCode != '\0')
	{
		if (*pszRegCode != '-')
		{
			strContent.push_back(*pszRegCode);
		}
		pszRegCode++;
	}

	// ���ݽ���
	strRegCode = readLicObj.DecryptData(strContent);

	RegisterCode RegCode;
	ZeroMemory(&RegCode,sizeof(RegisterCode));

	// ע���뻹ԭ
	CDataProcess cData;
	bool bIsFlag = cData.RestoreCode(strRegCode,RegCode);
	if (!bIsFlag)
	{
		return -1; // ע�������ݽ��ܳ���
	}
#if 0
	// ��ȡ�߰汾
	string	strHighVer = pszVersion;
	int iPos = strHighVer.find('.');
	if (iPos != -1)
	{
		strHighVer = strHighVer.substr(0, iPos);
		if (strHighVer.length() == 1)
		{
			string strTemp = strHighVer;
			strHighVer = "0";
			strHighVer += strTemp;
		}
	}

	// ��Ʒ�汾У��
	if (RegCode.iProFlag != AppFlag || RegCode.szProVer != strHighVer)
	{
		return -2; // ��Ʒ�汾У��ʧ��
	}
#endif
	// MacУ��
	CSignature sn;
	if (!sn.VerifyMac(RegCode.szMac1,RegCode.szMac2))
	{
		return -3; // MacУ��ʧ��
	}

	return 0;
}

// ��ȡע����
int __stdcall GetRegCode(char* pszRegCode, char* pszProduct, char* pszVersion)
{
	// �õ���ǰ����Ŀ¼
	CString strCurrentDir;
	string strProduct = pszProduct;
	string strVersion = pszVersion;
	wstring wstrProductTmp = L"";
	wstring wstrVersionTmp = L"";
#if 0
	// string->wstring
	wstrProductTmp = StringToWString(strProduct);
	wstrVersionTmp = StringToWString(strVersion);

	strCurrentDir = readLicObj.GetRunDirectory(wstrProductTmp.c_str(),wstrVersionTmp.c_str());
	// CString->string
	wstring wstrCurrentDir = strCurrentDir.GetString();
	string strCurrentDirTmp = WStringToString(wstrCurrentDir);
#endif

	// ��ȡ�����ļ�
	string strRegFile;
	//strRegFile = strCurrentDirTmp;
	strRegFile = "C:\\flexlm";
	strRegFile += "\\";
	strRegFile += RegFileName;

	// �鿴�ļ��Ƿ����
	bool bIsFlag = readLicObj.IsRegFileExist(strRegFile);
	if (bIsFlag)
	{
		// ��ȡע����
		string strContent;
		int iRet = readLicObj.ReadRegFile(strRegFile,strContent);
		if (iRet == -1)
		{
			return -2; // ���ļ�ʧ��
		}
		else if (iRet == -2)
		{
			return -3; // �ļ�����ʧ��
		}
		else
		{
			strcpy_s(pszRegCode,REGCODE_LEN+1,strContent.c_str());
		}
	}
	else
	{
		// �ļ�������
		return -1;
	}

	return 0;
}

// ע���뱣��
void __stdcall SetRegCode(char* pszRegCode, char* pszProduct, char* pszVersion)
{
	string strCurrentDir;
	string strProduct = pszProduct;
	string strVersion = pszVersion;
	wstring wstrProductTmp = L"";
	wstring wstrVersionTmp = L"";

	wstrProductTmp = StringToWString(strProduct);
	wstrVersionTmp = StringToWString(strVersion);
	// ���ݼ���
	//CString strEncrypt = readLicObj.EncryptFile(pszRegCode);

	// �õ���ǰ����Ŀ¼
	CString strCurrentDirTmp = readLicObj.GetRunDirectory(wstrProductTmp.c_str(),wstrVersionTmp.c_str());
	
	// CString->string
	wstring wstrCurrentDir = strCurrentDirTmp.GetString();
	int nLength = WideCharToMultiByte(CP_ACP, 0, wstrCurrentDir.c_str(), -1, NULL, NULL,NULL,NULL);
	strCurrentDir.resize(nLength);
	LPSTR lpszStr = new char[nLength];
	WideCharToMultiByte(CP_ACP, 0, wstrCurrentDir.c_str(), -1, lpszStr, nLength,NULL,NULL);
	strCurrentDir = lpszStr;
	delete [] lpszStr;

	// ��ȡ�����ļ�
	string strRegFile;
	strRegFile = strCurrentDir;
	strRegFile += "\\";
	strRegFile += RegFileName;

	try
	{
		CString strRegFileTmp(strRegFile.c_str());
		CStdioFile file(strRegFileTmp,CFile::modeCreate|CFile::modeReadWrite);
		file.WriteString((LPCTSTR)(LPTSTR)pszRegCode);
		file.Close();
	}
	catch (CFileException* ex)
	{
		ex->Delete();
	}
}

// ��ȡ�汾����
int __stdcall GetEdition(char* pszRegCode)
{
	string strRegCode;
	string strEndDate;
	string strContent(pszRegCode);

	// Lite�汾�ж�
	if (strContent == LITEREGCODE)
	{
		return 0;
	}

	// ȥ��"-"
	strContent.clear();
	while(*pszRegCode != '\0')
	{
		if (*pszRegCode != '-')
		{
			strContent.push_back(*pszRegCode);
		}
		pszRegCode++;
	}

	// ���ݽ���
	strRegCode = readLicObj.DecryptData(strContent);

	RegisterCode RegCode;
	ZeroMemory(&RegCode,sizeof(RegisterCode));

	// ע���뻹ԭ
	CDataProcess cData;
	cData.RestoreCode(strRegCode,RegCode);

	return RegCode.iProEdition;
}

// ��ȡʣ������
int __stdcall GetSpareDate(char* pszRegCode)
{
	string strRegCode;
	string strEndDate;
	string strContent;

	// ȥ��"-"
	while(*pszRegCode != '\0')
	{
		if (*pszRegCode != '-')
		{
			strContent.push_back(*pszRegCode);
		}
		pszRegCode++;
	}

	// ���ݽ���
	strRegCode = readLicObj.DecryptData(strContent);

	RegisterCode RegCode;
	ZeroMemory(&RegCode,sizeof(RegisterCode));

	// ע���뻹ԭ
	CDataProcess cData;
	cData.RestoreCode(strRegCode,RegCode);

	// ��ȡ����ʱ��
	strEndDate = cData.GetProperTime(RegCode.szSpaceDate);

	int lSpan = cData.GetSpaceDate(strEndDate);

	return lSpan;
}

int __stdcall VerifySystemTime(char* pszProduct, char* pszVersion)
{
	// ���Ҽ�¼����һ���������ʱ��
	string strProduct = pszProduct;
	string strVersion = pszVersion;
	
	int iPos = strVersion.find('.'); // ��ȡ�߰汾��
	if (iPos != -1)
	{
		strVersion = strVersion.substr(0, iPos);
	}
	strProduct += strVersion;
	
	CString strSystemTime = CTime::GetCurrentTime().Format("%Y%m%d%H%M"); //��ȡ��ǰϵͳʱ��

	HKEY hKey;
	DWORD dwAction;
	DWORD dwType = REG_SZ;
	DWORD dwCbData = MAX_PATH;
	CString strLastRunTime;
	if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_LOCAL_MACHINE,LICIT_REG_LASTTIMERECORD + CString(strProduct.c_str()),0,KEY_ALL_ACCESS,&hKey))
	{
		RegQueryValueEx(hKey,_T("LastRunTime"),NULL,&dwType,(LPBYTE)strLastRunTime.GetBuffer(MAX_PATH),&dwCbData); //��ȡע����м�¼����һ������ʱ��
		strLastRunTime.ReleaseBuffer();
		//�Ƚ���һ���������ʱ��͵�ǰϵͳʱ��
		// CString->string
		wstring wstrLastRunTimeTmp = strLastRunTime.GetString();
		wstring wstrSystemTimeTmp = strSystemTime.GetString();
		string strLastRunTimeTmp;
		string strSystemTimeTmp;
		strLastRunTimeTmp = WStringToString(wstrLastRunTimeTmp);
		strSystemTimeTmp = WStringToString(wstrSystemTimeTmp);

		double dLastRunTime = atof(strLastRunTimeTmp.c_str());
		double dSystemTime = atof(strSystemTimeTmp.c_str());
		if (dLastRunTime > dSystemTime)
		{
			return -1; //��һ������ʱ����ڵ�ǰϵͳʱ�䣬˵���޸Ĺ�ϵͳʱ��
		}
		//������һ������ʱ��
		RegSetValueEx(hKey,_T("LastRunTime"),NULL,dwType,(LPBYTE)strSystemTime.GetString(),strSystemTime.GetLength());
	}
	else
	{
		//��һ�����У�������¼
		if(ERROR_SUCCESS == RegCreateKeyEx(HKEY_LOCAL_MACHINE,LICIT_REG_LASTTIMERECORD + CString(strProduct.c_str()),0,_T("REG_NONE"),REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&hKey,&dwAction))
		{
			RegSetValueEx(hKey,_T("LastRunTime"),NULL,REG_SZ,(LPBYTE)strSystemTime.GetString(),dwCbData);
		}		
	}
	RegCloseKey(hKey);

	return 0;
}

// wstring->string
std::string WStringToString(const std::wstring& i_wstr)
{
	std::string strTmp;
	int nLength = WideCharToMultiByte(CP_ACP, 0, i_wstr.c_str(), -1, NULL, NULL,NULL,NULL);
	strTmp.resize(nLength);
	LPSTR lpszStr = new char[nLength];
	WideCharToMultiByte(CP_ACP, 0, i_wstr.c_str(), -1, lpszStr, nLength,NULL,NULL);
	strTmp = lpszStr;
	delete [] lpszStr;
	return strTmp;
}

// string->wstring
std::wstring StringToWString(const std::string& i_Str)
{
	std::wstring wstrTmp;
	int niLength = MultiByteToWideChar(CP_ACP, 0, i_Str.c_str(), -1, NULL, NULL);
	wstrTmp.resize(niLength);
	LPWSTR lpwszStr = new wchar_t[niLength];
	MultiByteToWideChar(CP_ACP, 0, i_Str.c_str(), -1, lpwszStr, niLength);
	wstrTmp = lpwszStr;
	delete [] lpwszStr;
	return wstrTmp;
}