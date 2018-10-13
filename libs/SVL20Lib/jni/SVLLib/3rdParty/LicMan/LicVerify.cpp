// LicVerify.cpp : 定义 DLL 的初始化例程。
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
//TODO: 如果此 DLL 相对于 MFC DLL 是动态链接的，
//		则从此 DLL 导出的任何调入
//		MFC 的函数必须将 AFX_MANAGE_STATE 宏添加到
//		该函数的最前面。
//
//		例如:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。这意味着
//		它必须作为函数中的第一个语句
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//


// CLicVerifyApp

BEGIN_MESSAGE_MAP(CLicVerifyApp, CWinApp)
END_MESSAGE_MAP()


// CLicVerifyApp 构造

CLicVerifyApp::CLicVerifyApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CLicVerifyApp 对象

CLicVerifyApp theApp;
CReadLicData readLicObj;

// CLicVerifyApp 初始化

BOOL CLicVerifyApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

// 导出函数
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

// 注册验证
int __stdcall VerifyRegCode(char* pszRegCode, int AppFlag, char* pszVersion)
{
	string	strRegCode = "";
	string	strContent = "";
	
	// Lite版本判断
	if (strContent == LITEREGCODE)
	{
		return 0;
	}

	// 去除"-"
	while(*pszRegCode != '\0')
	{
		if (*pszRegCode != '-')
		{
			strContent.push_back(*pszRegCode);
		}
		pszRegCode++;
	}

	// 数据解密
	strRegCode = readLicObj.DecryptData(strContent);

	RegisterCode RegCode;
	ZeroMemory(&RegCode,sizeof(RegisterCode));

	// 注册码还原
	CDataProcess cData;
	bool bIsFlag = cData.RestoreCode(strRegCode,RegCode);
	if (!bIsFlag)
	{
		return -1; // 注册码数据解密出错
	}
#if 0
	// 获取高版本
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

	// 产品版本校验
	if (RegCode.iProFlag != AppFlag || RegCode.szProVer != strHighVer)
	{
		return -2; // 产品版本校验失败
	}
#endif
	// Mac校验
	CSignature sn;
	if (!sn.VerifyMac(RegCode.szMac1,RegCode.szMac2))
	{
		return -3; // Mac校验失败
	}

	return 0;
}

// 获取注册码
int __stdcall GetRegCode(char* pszRegCode, char* pszProduct, char* pszVersion)
{
	// 得到当前运行目录
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

	// 获取激活文件
	string strRegFile;
	//strRegFile = strCurrentDirTmp;
	strRegFile = "C:\\flexlm";
	strRegFile += "\\";
	strRegFile += RegFileName;

	// 查看文件是否存在
	bool bIsFlag = readLicObj.IsRegFileExist(strRegFile);
	if (bIsFlag)
	{
		// 获取注册码
		string strContent;
		int iRet = readLicObj.ReadRegFile(strRegFile,strContent);
		if (iRet == -1)
		{
			return -2; // 打开文件失败
		}
		else if (iRet == -2)
		{
			return -3; // 文件解密失败
		}
		else
		{
			strcpy_s(pszRegCode,REGCODE_LEN+1,strContent.c_str());
		}
	}
	else
	{
		// 文件不存在
		return -1;
	}

	return 0;
}

// 注册码保存
void __stdcall SetRegCode(char* pszRegCode, char* pszProduct, char* pszVersion)
{
	string strCurrentDir;
	string strProduct = pszProduct;
	string strVersion = pszVersion;
	wstring wstrProductTmp = L"";
	wstring wstrVersionTmp = L"";

	wstrProductTmp = StringToWString(strProduct);
	wstrVersionTmp = StringToWString(strVersion);
	// 数据加密
	//CString strEncrypt = readLicObj.EncryptFile(pszRegCode);

	// 得到当前运行目录
	CString strCurrentDirTmp = readLicObj.GetRunDirectory(wstrProductTmp.c_str(),wstrVersionTmp.c_str());
	
	// CString->string
	wstring wstrCurrentDir = strCurrentDirTmp.GetString();
	int nLength = WideCharToMultiByte(CP_ACP, 0, wstrCurrentDir.c_str(), -1, NULL, NULL,NULL,NULL);
	strCurrentDir.resize(nLength);
	LPSTR lpszStr = new char[nLength];
	WideCharToMultiByte(CP_ACP, 0, wstrCurrentDir.c_str(), -1, lpszStr, nLength,NULL,NULL);
	strCurrentDir = lpszStr;
	delete [] lpszStr;

	// 获取激活文件
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

// 获取版本类型
int __stdcall GetEdition(char* pszRegCode)
{
	string strRegCode;
	string strEndDate;
	string strContent(pszRegCode);

	// Lite版本判断
	if (strContent == LITEREGCODE)
	{
		return 0;
	}

	// 去除"-"
	strContent.clear();
	while(*pszRegCode != '\0')
	{
		if (*pszRegCode != '-')
		{
			strContent.push_back(*pszRegCode);
		}
		pszRegCode++;
	}

	// 数据解密
	strRegCode = readLicObj.DecryptData(strContent);

	RegisterCode RegCode;
	ZeroMemory(&RegCode,sizeof(RegisterCode));

	// 注册码还原
	CDataProcess cData;
	cData.RestoreCode(strRegCode,RegCode);

	return RegCode.iProEdition;
}

// 获取剩余期限
int __stdcall GetSpareDate(char* pszRegCode)
{
	string strRegCode;
	string strEndDate;
	string strContent;

	// 去除"-"
	while(*pszRegCode != '\0')
	{
		if (*pszRegCode != '-')
		{
			strContent.push_back(*pszRegCode);
		}
		pszRegCode++;
	}

	// 数据解密
	strRegCode = readLicObj.DecryptData(strContent);

	RegisterCode RegCode;
	ZeroMemory(&RegCode,sizeof(RegisterCode));

	// 注册码还原
	CDataProcess cData;
	cData.RestoreCode(strRegCode,RegCode);

	// 获取过期时间
	strEndDate = cData.GetProperTime(RegCode.szSpaceDate);

	int lSpan = cData.GetSpaceDate(strEndDate);

	return lSpan;
}

int __stdcall VerifySystemTime(char* pszProduct, char* pszVersion)
{
	// 查找记录中上一次软件运行时间
	string strProduct = pszProduct;
	string strVersion = pszVersion;
	
	int iPos = strVersion.find('.'); // 获取高版本号
	if (iPos != -1)
	{
		strVersion = strVersion.substr(0, iPos);
	}
	strProduct += strVersion;
	
	CString strSystemTime = CTime::GetCurrentTime().Format("%Y%m%d%H%M"); //获取当前系统时间

	HKEY hKey;
	DWORD dwAction;
	DWORD dwType = REG_SZ;
	DWORD dwCbData = MAX_PATH;
	CString strLastRunTime;
	if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_LOCAL_MACHINE,LICIT_REG_LASTTIMERECORD + CString(strProduct.c_str()),0,KEY_ALL_ACCESS,&hKey))
	{
		RegQueryValueEx(hKey,_T("LastRunTime"),NULL,&dwType,(LPBYTE)strLastRunTime.GetBuffer(MAX_PATH),&dwCbData); //获取注册表中记录的上一次运行时间
		strLastRunTime.ReleaseBuffer();
		//比较上一次软件运行时间和当前系统时间
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
			return -1; //上一次运行时间大于当前系统时间，说明修改过系统时间
		}
		//更新上一次运行时间
		RegSetValueEx(hKey,_T("LastRunTime"),NULL,dwType,(LPBYTE)strSystemTime.GetString(),strSystemTime.GetLength());
	}
	else
	{
		//第一次运行，创建记录
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