/***********************************************************************************
*   Created on 2013/07/29 by zyp
*   All Rights Reserved. Copyright (C) 2013 HoteamSoft, Ltd.  
*
*   Function:
*      日志
*
***********************************************************************************/

#include "StdAfx.h"
#include "Stk_LogFile.h"
#include <stdlib.h>
#include <tchar.h>

#ifdef _WINDOWS_

CLogFile::CLogFile(LPCTSTR szFileName/* = "Log.log"*/)
{
	//_szFileName = NULL;
	memset(_szFileName, 0, sizeof(_szFileName));
	_hFile = INVALID_HANDLE_VALUE;
	::InitializeCriticalSection(&_csLock);

	SetFileName(szFileName);
}

CLogFile::~CLogFile(void)
{
	::DeleteCriticalSection(&_csLock);
	Close();
	/*if(_szFileName)
		delete []_szFileName;*/
}

bool CLogFile::OpenFile()
{
	if(IsOpen())
		return true;

	if(!_tcslen(_szFileName))
		return false;

	_hFile =  CreateFile(
		_szFileName, 
		GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL 
		);

	if(!IsOpen() && GetLastError() == 2)//打开不成功， 且因为文件不存在， 创建文件
		_hFile =  CreateFile(
		_szFileName, 
		GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL 
		);  

	if(IsOpen())
	{
		#ifdef _UNICODE
		CHAR UniHead[3] = {(CHAR)0xFF, (CHAR)0xFE, (CHAR)0};
		Write(UniHead, 2);
		#endif
		SetFilePointer(_hFile, 0, NULL, FILE_END);
	}

	return IsOpen();
}

DWORD CLogFile::Write( LPCVOID lpBuffer, DWORD dwLength )
{
	DWORD dwWriteLength = 0;

	if(IsOpen())
		WriteFile(_hFile, lpBuffer, dwLength, &dwWriteLength, NULL);

	return dwWriteLength;
}

void CLogFile::WriteLog( LPCVOID lpBuffer, DWORD dwLength )
{
	struct tm newtime;
	time_t now;
	TCHAR temp[21];
	DWORD dwWriteLength;
	
	if(IsOpen())
	{
		time(&now);
		localtime_s(&newtime, &now);
		//_tcsftime(temp, 20, _T("%Y-%m-%d %H:%M:%S"), &newtime);
		_tcsftime(temp, 20, _T("%H:%M:%S"), &newtime);
#ifdef _UNICODE
		//WriteFile(_hFile, L"#-", 4, &dwWriteLength, NULL);
		//WriteFile(_hFile, temp, (_tcslen(temp))*sizeof(TCHAR), &dwWriteLength, NULL);
		//WriteFile(_hFile, L"-# ", 6, &dwWriteLength, NULL);
		WriteFile(_hFile, lpBuffer, dwLength*2, &dwWriteLength, NULL);
		WriteFile(_hFile, _T("\xd\xa"), 4, &dwWriteLength, NULL);
#else
		//WriteFile(_hFile, "#-", 2, &dwWriteLength, NULL);
		//WriteFile(_hFile, temp, 19, &dwWriteLength, NULL);
		//WriteFile(_hFile, "-# ", 3, &dwWriteLength, NULL);
		WriteFile(_hFile, lpBuffer, dwLength, &dwWriteLength, NULL);
		WriteFile(_hFile, _T("\xd\xa"), 2, &dwWriteLength, NULL);
#endif
		
		//FlushFileBuffers(_hFile);

	}
}

void CLogFile::Lock()
{
	  ::EnterCriticalSection(&_csLock); 
}

void CLogFile::Unlock()
{
	::LeaveCriticalSection(&_csLock);
}

void CLogFile::SetFileName(LPCTSTR szName )
{
	assert(szName);

	if(_tcslen(_szFileName))
		memset(_szFileName, 0, sizeof(_szFileName));

	Close();
	/*size_t nameSize = _tcslen(szName) + 1;
	_szFileName = new TCHAR[nameSize];*/
	assert(_szFileName);
	_tcscpy_s(_szFileName, _countof(_szFileName), szName);
}

void CLogFile::Close()
{
	if(IsOpen())
	{
		Flush();
		CloseHandle(_hFile);
		_hFile = INVALID_HANDLE_VALUE;
	}
}

void CLogFile::Log( LPCVOID lpBuffer, DWORD dwLength )
{
	assert(lpBuffer);
	__try
	{
		Lock();

		if(!OpenFile())
			return;

		WriteLog(lpBuffer, dwLength);
	}
	__finally
	{
		Unlock();
	} 
}

void CLogFile::Log(LPCTSTR szText )
{
	Log(szText, _tcslen(szText));
}

void CLogFile::Flush()
{
	if(IsOpen())
	{
		FlushFileBuffers(_hFile);
	}
}

Stk_LogFileEx::Stk_LogFileEx(LPCTSTR szPath /*= "."*/, LOG_TYPE iType /*= MONTH*/ )
{
	_szPath = NULL;
	SetPath(szPath);
	_iType = iType;
	memset(_szLastDate, 0, sizeof(_szLastDate));
	_szLogFileName[0] = 0;
	_PrintLogLevel = 0;
	_PrintLogType = 0;
}

Stk_LogFileEx::~Stk_LogFileEx()
{
	if(_szPath)
		delete []_szPath;
}


void Stk_LogFileEx::SetPath(LPCTSTR szPath )
{
	assert(szPath);

	WIN32_FIND_DATA wfd;
	TCHAR temp[MAX_PATH + 50] = {0};
	TCHAR tempPath[MAX_PATH + 50] = {0};

	_tcscpy_s(tempPath, _countof(tempPath), szPath);
	if (_tcscmp(tempPath, L"") == 0)
	{
		return;
	}

	if(FindFirstFile(szPath, &wfd) == INVALID_HANDLE_VALUE && CreateDirectory(szPath, NULL) == 0)
	{
		_tcscpy_s(temp, _countof(temp), szPath);
		_tcscat_s(temp, _countof(temp), _T(" Create Fail. Exit Now! Error ID :"));
		_ltot_s(GetLastError(), temp + _tcslen(temp), sizeof(temp), 10);
		MessageBox(NULL, temp, _T("Class LogFileEx"), MB_OK);
		//exit(1);
	}
	else
	{
		GetFullPathName(szPath, MAX_PATH, temp, NULL);
		size_t pathSize = _tcslen(temp) + 1;
		_szPath = new TCHAR[pathSize];
		assert(_szPath);
		_tcscpy_s(_szPath, pathSize, temp);
	}
}

void Stk_LogFileEx::Log( LPCVOID lpBuffer, DWORD dwLength, int inCurLogLevel)
{
	assert(lpBuffer);

	TCHAR temp[10];
	static const TCHAR format[3][10] = {_T("%Y"), _T("%Y-%m"), _T("%Y%m%d")};

	if(!_PrintLogType || inCurLogLevel > _PrintLogLevel)//_PrintLogLevel == 0 不输出日志
		return;

	__try
	{
		Lock();
		struct tm newtime;
		time_t now = time(NULL);
		localtime_s(&newtime, &now);
		_tcsftime(temp, 9, format[_iType], &newtime);

		if(0 != _szLogFileName[0] && 0 != _tcscmp(_szFileName, _szLogFileName))
		{
			_tcscpy_s(_szFileName, _countof(_szFileName), _szLogFileName);
			Close();
		}
		else if(_tcscmp(_szLastDate, temp) != 0)//更换文件名
		{
			if(NULL == _szLogFileName[0])
			{
				_tcscpy_s(_szFileName, _countof(_szFileName), _szPath);
				_tcscat_s(_szFileName, _countof(_szFileName), _T("\\"));
				_tcscat_s(_szFileName, _countof(_szFileName), temp);
				_tcscat_s(_szFileName, _countof(_szFileName),  _T(".log"));
			}
			else
			{
				_tcscpy_s(_szFileName, _countof(_szFileName), _szLogFileName);
			}
			_tcscpy_s(_szLastDate, _countof(_szLastDate), temp);
			Close();
		}

		if(!OpenFile())
			return;

		WriteLog(lpBuffer, dwLength);
		if(2 == _PrintLogType)//_PrintLogLevel==2实时输出日志
			Flush();
	}
	__finally
	{
		Unlock();
	}
}

void Stk_LogFileEx::Log(LPCTSTR szText, int inCurLogLevel)
{
	Log(szText, _tcslen(szText), inCurLogLevel);
}

void Stk_LogFileEx::SetLogFileName( LPCTSTR szLogFileName )
{
	assert(szLogFileName);
	Close();
	/*size_t nameSize = _tcslen(szName) + 1;
	_szFileName = new TCHAR[nameSize];*/
	if(NULL != szLogFileName)
		_tcscpy_s(_szLogFileName, MAX_PATH, szLogFileName);
	else
		_szLogFileName[0] = 0;
}

void Stk_LogFileEx::PrintLogType(int inType/*=0*/)
{
	_PrintLogType = inType;
	if(_PrintLogType<0)
		_PrintLogType = 0;
}

void Stk_LogFileEx::PrintLogLevel( int inLevel/*=0*/ )
{
	_PrintLogLevel = inLevel;
	if(_PrintLogLevel<0)
		_PrintLogLevel = 0;
}

void Stk_LogFileEx::Log0( LPCTSTR szText )
{
	int tPrintLogType = _PrintLogType;
	_PrintLogType = 2;
	Log(szText, 0);
	_PrintLogType = tPrintLogType;
}

#endif