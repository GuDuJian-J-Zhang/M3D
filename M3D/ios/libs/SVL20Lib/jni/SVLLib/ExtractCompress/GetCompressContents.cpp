
#include "GetCompressContents.h"
#include "Extract.h"


std::string TCHAR2STRING(TCHAR *STR);
extern TCHAR *char2tchar(char *str);
extern char *tchar2char(TCHAR *wcstr);

CGetCompressContents::CGetCompressContents()
{
}


CGetCompressContents::~CGetCompressContents()
{
}

bool CGetCompressContents::GetCompressFilesContents(
	const std::string &i_strDestCurDirPrefix,			// (i) —πÀıŒƒº˛¥Ê∑≈¬∑æ∂
	std::vector<std::string> &o_strFileContentsA		// (o) —πÀıŒƒº˛ƒø¬º
	)
{
	char *strP = (char *)i_strDestCurDirPrefix.c_str();
	TCHAR *strPathName;
	strPathName = char2tchar(strP);

	HZIP hz;
	hz = OpenZip(strPathName, 0);
	ZIPENTRY ze;
	GetZipItem(hz, -1, &ze);
	int numitems = ze.index;
	std::string strFileName;

	for (int zi = 0; zi < numitems; zi++)
	{
		GetZipItem(hz, zi, &ze);
		//strFileName = TCHAR2STRING(ze.name);
		char *name = 0;
#ifdef UNICODE
		name = tchar2char(ze.name);
#else
		name = new char[strlen(ze.name) + 1];
		strcpy(name, ze.name);
#endif
		strFileName = name;
		delete name;
		o_strFileContentsA.push_back(strFileName);
	}

	CloseZip(hz);

	return true;
}

bool CGetCompressContents::GetCompressBufferContents(
	const unsigned int i_niBufferSize,					// (i) Ω‚—π—πÀıŒƒº˛¥Û–°
	const char *i_bufP,									// (i) Ω‚—π—πÀıŒƒº˛ƒ⁄¥Ê
	std::vector<std::string> &o_strFileContentsA		// (o) —πÀıŒƒº˛ƒø¬º
	)
{
	HZIP hz;
	hz = OpenZip((void*)i_bufP, i_niBufferSize, 0);
	ZIPENTRY ze;
	GetZipItem(hz, -1, &ze);
	int numitems = ze.index;

	std::string strFileName;
	for (int zi = 0; zi < numitems; zi++)
	{
        
        GetZipItem(hz, zi, &ze);
		//strFileName = TCHAR2STRING(ze.name);
		char *name = 0;
#ifdef UNICODE
		name = tchar2char(ze.name);
#else
		name = new char[strlen(ze.name) + 1];
		strcpy(name, ze.name);
#endif
		strFileName = name;
		delete name;
		o_strFileContentsA.push_back(strFileName);
	}

	CloseZip(hz);

	return true;
}

std::string TCHAR2STRING(TCHAR *STR)
{
#ifdef __MOBILE__
	std::string str("");
	char* pChar = tchar2char(STR);
	str = pChar;
	delete pChar;
	return str;
#else
	int iLen = WideCharToMultiByte(CP_ACP, 0, STR, -1, NULL, 0, NULL, NULL);
	char* chRtn = new char[iLen*sizeof(char)];
	WideCharToMultiByte(CP_ACP, 0, STR, -1, chRtn, iLen, NULL, NULL);
	std::string str(chRtn);
	return str;
#endif

}
