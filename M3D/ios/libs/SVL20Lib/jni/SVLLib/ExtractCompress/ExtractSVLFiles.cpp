#include "ExtractSVLFiles.h"
#include "Extract.h"
#include "Alloc.h"
#include <stdlib.h>
#include "../ChineseCode.h"
#include <sys/stat.h>

TCHAR *char2tchar(char *str) ;
std::string TCHAR2STRING(TCHAR *STR);

CExtractSVLFiles::CExtractSVLFiles()
{
}


CExtractSVLFiles::~CExtractSVLFiles()
{
}

bool CExtractSVLFiles::ExtractBufferToSVLFiles(
	const unsigned int i_niBufferSize,		// (i) 解压压缩文件大小
	const char *i_bufP,						// (i) 解压压缩文件内存
	const std::wstring &i_strArcNameA,		// (i) 解压文件名称
	char ***o_bufP,							// (i) 解压文件数据
	std::vector<int> &o_vcBufNum			// (i) 解压文件数据大小
	)
{
	bool bRet = false;
	HZIP hz;
	hz = OpenZip((void*)i_bufP, i_niBufferSize, 0);
	ZIPENTRY ze;
	GetZipItem(hz, -1, &ze);
	int numitems = ze.index;

	TCHAR *strFileName = (wchar_t *)i_strArcNameA.c_str();

	for (int zi = 0; zi < numitems; zi++)
	{
		GetZipItem(hz, zi, &ze);
#ifdef __MOBILE__
		if (0 == wcscasecmp(strFileName,ze.name))
#else
		if (0 == wcsicmp(strFileName,ze.name))
#endif
		{
			if(UnzipItem(hz, zi, o_bufP, o_vcBufNum) == ZR_OK)
			{
				bRet = true;
				break;
			}
		}
	}

	CloseZip(hz);
	return bRet;
}

bool CExtractSVLFiles::ExtractBufferToSVLFiles(const unsigned int i_niBufferSize, /* (i) 解压压缩文件大小 */ 
	const char *i_bufP,							/* (i) 解压压缩文件内存 */ 
	const std::string &i_strArcNameA,			/* (i) 解压文件名称 */ 
	char ***o_bufP,								/* (i) 解压文件数据 */ 
	std::vector<int> &o_vcBufNum				/* (i) 解压文件数据大小 */
)
{
	wstring wstrArcName = ChineseCode::StringToWString(i_strArcNameA);
	return ExtractBufferToSVLFiles(i_niBufferSize, i_bufP, wstrArcName, o_bufP, o_vcBufNum);
}

bool CExtractSVLFiles::ExtractFileToSVLFiles(
	const std::wstring &i_strDestCurDirPrefix,	// (i) 解压文件路径
	const std::wstring &i_strArcNameA,			// (i) 解压文件名称
	char  ***o_bufP,							// (o) 解压文件数据(使用完需要释放内存)
	std::vector<int> &o_vcBufNum				// (o) 解压文件数据大小
	)
{
	HZIP hz;

	TCHAR *strPathName = (TCHAR *)i_strDestCurDirPrefix.c_str();
	hz = OpenZip(strPathName, 0);
	ZIPENTRY ze;
	GetZipItem(hz, -1, &ze);
	int numitems = ze.index;

	TCHAR *strFileName = (TCHAR *)i_strArcNameA.c_str();


	TCHAR strCompFileName[256];
	for (int zi = 0; zi < numitems; zi++)
	{
		GetZipItem(hz, zi, &ze);

		wcscpy(strCompFileName,ze.name);
#ifdef __MOBILE__
		if (0 == wcscasecmp(strFileName,strCompFileName))
#else
		if (0 == wcsicmp(strFileName,strCompFileName))
#endif
		{
			UnzipItem(hz, zi, o_bufP, o_vcBufNum);
		}
	}

	CloseZip(hz);
	return true;
}

bool CExtractSVLFiles::ExtractFileToSVLFiles(
	const std::string &i_strDestCurDirPrefix,			/* (i) 解压文件路径 */ 
	const std::string &i_strArcNameA,					/* (i) 解压文件名称 */ 
	char ***o_bufP,										/* (o) 解压文件数据 */ 
	std::vector<int> &o_vcBufNum						/* (o) 解压文件数据大小 */
)
{
	wstring strDestCurDirPrefix = ChineseCode::StringToWString(i_strDestCurDirPrefix);
	wstring wstrArcName = ChineseCode::StringToWString(i_strArcNameA);
	return ExtractFileToSVLFiles(strDestCurDirPrefix, wstrArcName, o_bufP, o_vcBufNum);
}

bool CExtractSVLFiles::ExtractFileToAllSVLFiles(const std::wstring &i_strDestCurDirPrefix, /* (i) 解压文件路径 */
	std::vector<std::wstring> &o_strArcNameA, /* (o) 解压文件名称 */
	char ****o_bufP, /* (o) 解压文件数据 */ 
	std::vector<std::vector<int>> &o_vcBufNum /* (o) 解压文件数据大小 */)
{
	TCHAR *strPathName = (TCHAR *)i_strDestCurDirPrefix.c_str();

	HZIP hz;
	hz = OpenZip(strPathName, 0);
	ZIPENTRY ze;
	GetZipItem(hz, -1, &ze);
	int numitems = ze.index;

	char ***bufP; // 解压文件数据
	bufP = (char ***)MidAlloc(numitems * sizeof(char***));
	std::vector<int> vcBufNum;

	for (int zi = 0; zi < numitems; zi++)
	{
		GetZipItem(hz, zi, &ze);
		UnzipItem(hz, zi, &bufP[zi], vcBufNum);
		o_vcBufNum.push_back(vcBufNum);
		o_strArcNameA.push_back(ze.name);
	}

	*o_bufP = bufP;
	CloseZip(hz);

	return true;
}

bool CExtractSVLFiles::ExtractFileToAllSVLFiles(
	const std::string &i_strDestCurDirPrefix,			/* (i) 解压文件路径 */ 
	std::vector<std::string> &o_strArcNameA,			/* (o) 解压文件名称 */ 
	char ****o_bufP,									/* (o) 解压文件数据 */ 
	std::vector<std::vector<int>> &o_vcBufNum			/* (o) 解压文件数据大小 */
)
{
	bool bRet = false;
	wstring strDestCurDirPrefix = ChineseCode::StringToWString(i_strDestCurDirPrefix);
	std::vector<std::wstring> wstrArcNameA;
	bRet = ExtractFileToAllSVLFiles(strDestCurDirPrefix, wstrArcNameA, o_bufP, o_vcBufNum);
	for (int i = 0; i < wstrArcNameA.size(); i++)
	{
		o_strArcNameA.push_back(ChineseCode::WStringToString(wstrArcNameA[i]));
	}
	return bRet;
}

bool CExtractSVLFiles::ExtractBufferToAllSVLFiles(const unsigned int i_niBufferSize, /* (i) 解压压缩文件大小 */
	const char *i_bufP, /* (i) 解压压缩文件内存 */
	std::vector<std::wstring> &o_strArcNameA, /* (o) 解压文件名称 */ 
	char ****o_bufP, /* (o) 解压文件数据 */ 
	std::vector<std::vector<int>> &o_vcBufNum /* (o) 解压文件数据大小 */)
{
	HZIP hz;

	hz = OpenZip((void*)i_bufP, i_niBufferSize, 0);
	ZIPENTRY ze;
	GetZipItem(hz, -1, &ze);
	int numitems = ze.index;

	char ***bufP; // 解压文件数据
	bufP = (char ***)MidAlloc(numitems * sizeof(char***));
	std::vector<int> vcBufNum;

	for (int zi = 0; zi < numitems; zi++)
	{
		GetZipItem(hz, zi, &ze);
		UnzipItem(hz, zi, &bufP[zi], vcBufNum);
		o_vcBufNum.push_back(vcBufNum);
		o_strArcNameA.push_back(ze.name);
	}

	*o_bufP = bufP;
	CloseZip(hz);

	return true;
}

bool CExtractSVLFiles::ExtractBufferToAllSVLFiles(
	const unsigned int i_niBufferSize,				/* (i) 解压压缩文件大小 */ 
	const char *i_bufP,								/* (i) 解压压缩文件内存 */ 
	std::vector<std::string> &o_strArcNameA,		/* (o) 解压文件名称 */ 
	char ****o_bufP,								/* (o) 解压文件数据 */ 
	std::vector<std::vector<int>> &o_vcBufNum		/* (o) 解压文件数据大小 */
)
{
	bool bRet = false;
	std::vector<std::wstring> wstrArcNameA;
	bRet = ExtractBufferToAllSVLFiles(i_niBufferSize, i_bufP, wstrArcNameA, o_bufP, o_vcBufNum);
	for (int i = 0; i < wstrArcNameA.size(); i++)
	{
		o_strArcNameA.push_back(ChineseCode::WStringToString(wstrArcNameA[i]));
	}
	return bRet;
}

//bool ExtractFileToAllSVLFiles(
//	const std::string &i_strDestCurDirPrefix,			// (i) 解压文件路径
//	std::vector<std::string> &o_strArcNameA,			// (o) 解压文件名称
//	char ****o_bufP,									// (o) 解压文件数据
//	std::vector<std::vector<int>> &o_vcBufNum			// (o) 解压文件数据大小
//	)
//{
//	
//}
//
//bool ExtractBufferToAllSVLFiles(
//	const unsigned int i_niBufferSize,					// (i) 解压压缩文件大小
//	const CHAR *i_bufP,									// (i) 解压压缩文件内存
//	std::vector<std::string> &o_strArcNameA,			// (o) 解压文件名称
//	char ****o_bufP,									// (o) 解压文件数据
//	std::vector<std::vector<int>> &o_vcBufNum			// (o) 解压文件数据大小
//	)
//{
//	
//}

bool CExtractSVLFiles::GetFolderFileList(const unsigned int i_niBufferSize,		// (i) 解压压缩文件大小
	const char *i_bufP,						// (i) 解压压缩文件内存
	const std::wstring &i_strFolderName,
	std::vector<std::wstring> &o_strFileNameA, /* (o) 文件名列表 */
	std::vector<std::wstring> &o_strFolderNameA /* (o) 文件夹列表 */)
{
	HZIP hz;
	hz = OpenZip((void*)i_bufP, i_niBufferSize, 0);
	ZIPENTRY ze;
	GetZipItem(hz, -1, &ze);
	int numitems = ze.index;
	std::wstring strFolderName = i_strFolderName;
	

	std::wstring strFileName;
	std::wstring strFileNamePre;
	for (int zi = 0; zi < numitems; zi++)
	{
		GetZipItem(hz, zi, &ze);
		strFileName = ze.name;
		strFileNamePre = strFileName.substr(0, strFolderName.length());
		
#ifdef __MOBILE__
		if (0 == wcscasecmp(strFolderName.c_str(),strFileNamePre.c_str()))
		{
			if ((ze.attr&S_IFDIR) != 0)
#else
		if (0 == wcsicmp(strFolderName.c_str(),strFileNamePre.c_str()))
		{
			if ((ze.attr&FILE_ATTRIBUTE_DIRECTORY) != 0)
#endif

			{
				if (strFileName != strFolderName &&
					strFileName.substr(0, strFileName.length()-1) != strFolderName)
				{
					o_strFolderNameA.push_back(strFileName);
				}
			}
			else
			{
				o_strFileNameA.push_back(strFileName);
			}
		}
	}
	CloseZip(hz);

	return true;
}

bool CExtractSVLFiles::GetFolderFileList(const unsigned int i_niBufferSize,		// (i) 解压压缩文件大小
	const char *i_bufP,						// (i) 解压压缩文件内存
	const std::string &i_strFolderName, 
	std::vector<std::string> &o_strFileNameA, /* (o) 文件名列表 */ 
	std::vector<std::string> &o_strFolderNameA /* (o) 文件夹列表 */)
{
	bool bRet = false;
	std::wstring strDestCurDirPrefix;
	std::wstring strFolderName;
	strFolderName = char2tchar((char*)i_strFolderName.c_str());

	std::vector<std::wstring> strFileNameA;
	std::vector<std::wstring> strFolderNameA;
	bRet = GetFolderFileList(i_niBufferSize, i_bufP,strFolderName, strFileNameA, strFolderNameA);
	for (int i = 0; i < strFileNameA.size(); i++)
	{
		o_strFileNameA.push_back(TCHAR2STRING((TCHAR*)strFileNameA[i].c_str()));
	}
	for (int i = 0; i < strFolderNameA.size(); i++)
	{
		o_strFolderNameA.push_back(TCHAR2STRING((TCHAR*)strFolderNameA[i].c_str()));
	}
	return bRet;
}
