/***********************************************************************************
*   Created on 2013/07/29 by zyp
*   All Rights Reserved. Copyright (C) 2013 HoteamSoft, Ltd.  
*
*   Function:
*      日志
*
***********************************************************************************/

#ifndef _LOGFILE_H
#define _LOGFILE_H

#ifdef _WINDOWS_

#include <assert.h>
#include <time.h>
#include <stdio.h>
#include <windows.h>
#include <TCHAR.H>
#include "DataManager.h"

#define LOGBUFFER_LEN 51200 //1024*40
class DATAMANAGER_API CLogFile
{
public:
	CLogFile(LPCTSTR szFileName = _T("SVLLog.log"));
	virtual ~CLogFile(void);
protected:
	CRITICAL_SECTION _csLock;
	TCHAR _szFileName[MAX_PATH + 20];
	HANDLE _hFile;
	//bool     _bFlash;

	bool OpenFile();//打开文件， 指针到文件尾
	DWORD Write(LPCVOID lpBuffer, DWORD dwLength);

	virtual void WriteLog( LPCVOID lpBuffer, DWORD dwLength);//写日志, 可以扩展修改
	void Lock();
	void Unlock();

public:

	LPCTSTR GetFileName()
	{
		return _szFileName;
	}
	void SetFileName(LPCTSTR szName);//修改文件名， 同时关闭上一个日志文件
	bool IsOpen()
	{
		return _hFile != INVALID_HANDLE_VALUE;
	}

	virtual void Close();

	void Log(LPCVOID lpBuffer, DWORD dwLength);//追加日志内容
	void Log(LPCTSTR szText);
	void Flush();

private://屏蔽函数
	CLogFile(const CLogFile&);
	CLogFile&operator = (const CLogFile&);
};


class DATAMANAGER_API Stk_LogFileEx : public CLogFile
{
protected:

	TCHAR *_szPath;
	TCHAR _szLastDate[9];
	TCHAR _szLogFileName[MAX_PATH];
	int _iType;
	int _PrintLogLevel;
	int _PrintLogType;

	void SetPath(LPCTSTR szPath);
	void Log(LPCVOID lpBuffer, DWORD dwLength, int inCurLogLevel);
public:

	enum LOG_TYPE{YEAR = 0, MONTH = 1, DAY = 2};

	Stk_LogFileEx(LPCTSTR szPath = _T(""), LOG_TYPE iType = MONTH);
	~Stk_LogFileEx();
	void SetLogFileName(LPCTSTR szLogFileName);

	LPCTSTR GetPath()
	{
		return _szPath;
	}

	void Log0(LPCTSTR szText);
	void Log(LPCTSTR szText, int inLogLevel=0);

	void PrintLogType(int inType=0);

	void PrintLogLevel(int inLevel=0);
	

private://屏蔽函数

	Stk_LogFileEx(const Stk_LogFileEx&);
	Stk_LogFileEx& operator = (const Stk_LogFileEx&);

};
#endif

#endif