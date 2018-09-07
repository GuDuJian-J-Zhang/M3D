// ALL RIGHTS RESERVED COPYRIGHT(C) 2016 HOTEAMSOFT, Ltd.
/*****************************************************************************
* @file FileCacheManager
* @brief: 文件缓存 内存映射管理
* @par 履历：Create by zhouyunpeng
*
******************************************************************************/

#include "FileCacheManager.h"
#include <assert.h>
#include <stdlib.h>

#ifdef WIN32

#endif

#ifdef __MOBILE__
#define INVALID_HANDLE_VALUE (void*)-1

#include <fcntl.h>
#include <sys/mman.h>
 #include <unistd.h>
#define _MAX_PATH 256
#define LEN_FILE_NAME 256

#define TEMP_DIR "/mnt/sdcard/Android/data/ht.sview/cache/"
#endif

namespace M3D
{
#define SEGMENT_MAXBUF (524288) //512Kb
#define BUFSIZE 512

class CFileCacheManagerP
{
public:
	CFileCacheManagerP(CFileCacheManager* pOwn);
	~CFileCacheManagerP();
	CFileCacheManager* publ;

	HANDLE	hFile;
	HANDLE	hFileMapping;
	WCHAR	strCacheFile[_MAX_PATH];
	BYTE*	byteSegBuf;//512Kb的缓存区
	UINT	uiMaxSegBuf;
	UINT	uiLenSegBuf;
	UINT	uiLenSegment;
	DWORD	dwBytesInBlock;

	BOOL	isOpenCache();
	DWORD   writeCache(LPCVOID lpBuffer, DWORD dwLength, DWORD& outSegmentAdd, DWORD& outOffsetAdd);
	int		endWriteCache();
	void	endCache();

	void endMapping();
};

CFileCacheManagerP::CFileCacheManagerP(CFileCacheManager* pOwn){
	publ = pOwn;
	hFile = INVALID_HANDLE_VALUE;
	hFileMapping = INVALID_HANDLE_VALUE;
	strCacheFile[0] = 0;
	byteSegBuf = 0;
	uiLenSegment = 0;
	uiLenSegment = 0;
	dwBytesInBlock = 0;
	uiMaxSegBuf = SEGMENT_MAXBUF;
}

CFileCacheManagerP::~CFileCacheManagerP(){
	if (byteSegBuf) {
		delete[] byteSegBuf;
		byteSegBuf = NULL;
	}
}

BOOL CFileCacheManagerP::isOpenCache() {
	return hFile != INVALID_HANDLE_VALUE;
}

DWORD CFileCacheManagerP::writeCache(LPCVOID lpBuffer, DWORD dwLength, DWORD& outSegmentAdd, DWORD& outOffsetAdd)
{
	DWORD dwWriteLength = 0;
	if (hFile == INVALID_HANDLE_VALUE || !lpBuffer || 0 == dwLength)
		return dwWriteLength;

#ifdef WIN32
	if ((dwLength + uiLenSegBuf) > SEGMENT_MAXBUF && uiLenSegBuf > 0)
	{
		WriteFile(hFile, byteSegBuf, uiLenSegBuf, &dwWriteLength, NULL);
		assert(0 != dwWriteLength);
		if (0 == dwWriteLength)
			return -1;

		unsigned int preSegment = uiLenSegment;
		uiLenSegment += (uiLenSegBuf + SEGMENT_MAXBUF - 1) / SEGMENT_MAXBUF;//上取整当前数据占用了几个段

		memset(byteSegBuf, 0, uiLenSegBuf);

		//填充空余的控件以便64K数据的对齐
		WriteFile(hFile, byteSegBuf,
			(uiLenSegment - preSegment) * SEGMENT_MAXBUF - uiLenSegBuf,
			&dwWriteLength, NULL);

		uiLenSegBuf = 0;
	}

	outSegmentAdd = uiLenSegment * SEGMENT_MAXBUF;
	outOffsetAdd = uiLenSegBuf;


	if (dwLength > uiMaxSegBuf) {
		uiMaxSegBuf = dwLength;
		delete[] byteSegBuf;
		byteSegBuf = new BYTE[uiMaxSegBuf];
	}


	memcpy(byteSegBuf + uiLenSegBuf, lpBuffer, dwLength);
	uiLenSegBuf += dwLength;
	dwWriteLength = dwLength > SEGMENT_MAXBUF ? dwLength : SEGMENT_MAXBUF;
#endif
	return dwWriteLength;
}

int CFileCacheManagerP::endWriteCache()
{
	DWORD dwWriteLength = 0;
	if (hFile == INVALID_HANDLE_VALUE || 0 == uiLenSegBuf)
		return -1;
#ifdef WIN32
	WriteFile(hFile, byteSegBuf, uiLenSegBuf, &dwWriteLength, NULL);

	unsigned int preSegment = uiLenSegment;
	uiLenSegment += (uiLenSegBuf + SEGMENT_MAXBUF - 1) / SEGMENT_MAXBUF;//上取整当前数据占用了几个段

	memset(byteSegBuf, 0, uiLenSegBuf);

	//填充空余的控件以便64K数据的对齐
	WriteFile(hFile, byteSegBuf,
		(uiLenSegment - preSegment) * SEGMENT_MAXBUF - uiLenSegBuf,
		&dwWriteLength, NULL);

	//uiLenSegment = 0;
	uiLenSegBuf = 0;

	FlushFileBuffers(hFile);
	
	//delete[] byteSegBuf;
	//byteSegBuf = NULL;
#endif
	return 0 != dwWriteLength ? 0 : -1;
}

void CFileCacheManagerP::endCache()
{
#ifdef WIN32
	if (hFileMapping != INVALID_HANDLE_VALUE) {
		CloseHandle(hFileMapping);
		hFileMapping = INVALID_HANDLE_VALUE;
	}

	if (hFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFile);
		hFile = INVALID_HANDLE_VALUE;
		DeleteFileW(strCacheFile);
		strCacheFile[0] = 0;
	}
#endif
}

void CFileCacheManagerP::endMapping()
{
#ifdef WIN32
	if (hFileMapping != INVALID_HANDLE_VALUE) {
		CloseHandle(hFileMapping);
		hFileMapping = INVALID_HANDLE_VALUE;
	}
#endif
}


CFileCacheManager*	gFileCacheMgrP = NULL;
CFileCacheManager::CFileCacheManager()
{
	m_pImpl = new CFileCacheManagerP(this);
}

CFileCacheManager::~CFileCacheManager()
{
	delete m_pImpl;
}

CFileCacheManager* CFileCacheManager::getInstance()
{
	if (!gFileCacheMgrP)
	{
		gFileCacheMgrP = new CFileCacheManager();
	}
	return gFileCacheMgrP;
}

int CFileCacheManager::bgeinCache(LPCWSTR strCacheFile)
{
	if (!strCacheFile || m_pImpl->isOpenCache()) {
		return -1;
	}
#ifdef WIN32
	wcscpy_s(m_pImpl->strCacheFile, strCacheFile);

	m_pImpl->hFile = CreateFileW(
		m_pImpl->strCacheFile,
		GENERIC_WRITE | GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL
		);

	if (m_pImpl->byteSegBuf) {
		delete[] m_pImpl->byteSegBuf;
	}

	m_pImpl->byteSegBuf = new BYTE[m_pImpl->uiMaxSegBuf];//创建512K的缓冲区
	m_pImpl->uiLenSegment = 0;
	m_pImpl->uiLenSegBuf = 0;
#endif
	return m_pImpl->isOpenCache() ? 0 : -1;
}

int CFileCacheManager::bgeinCache()
{
	if(!this->isOpenCache())
	{
		DWORD dwRetVal;
		DWORD dwBufSize = BUFSIZE;
		UINT uRetVal;
		wchar_t szTempName[BUFSIZE];
		wchar_t lpPathBuffer[BUFSIZE];
#ifdef WIN32
		// Get the temp path.
		dwRetVal = GetTempPathW(dwBufSize,     // length of the buffer
			lpPathBuffer); // buffer for path 
		if (dwRetVal > dwBufSize || (dwRetVal == 0))
		{
			return (2);
		}

		// Create a temporary file. 
		uRetVal = GetTempFileNameW(lpPathBuffer, // directory for tmp files
			L"SView-net",        // temp file name prefix 
			0,            // create unique name 
			szTempName);  // buffer for name 
		if (uRetVal == 0)
		{
			return (3);
		}
		return bgeinCache(szTempName);
#endif
	}

	return 0;
}

DWORD CFileCacheManager::writeCache(LPCVOID lpBuffer, DWORD dwLength, DWORD& outSegmentAdd, DWORD& outOffsetAdd)
{
	return m_pImpl->writeCache(lpBuffer, dwLength, outSegmentAdd, outOffsetAdd);
}

int CFileCacheManager::endWriteCache()
{
	return m_pImpl->endWriteCache();
}

void CFileCacheManager::endCache()
{
	m_pImpl->endCache();
}

void CFileCacheManager::endMapping()
{
	m_pImpl->endMapping();
}

BOOL CFileCacheManager::isOpenCache()
{
	return m_pImpl->isOpenCache();
}

HANDLE CFileCacheManager::getFileMapping()
{
	if (!m_pImpl->isOpenCache())
		return INVALID_HANDLE_VALUE;
#ifdef WIN32
	if (m_pImpl->hFileMapping == INVALID_HANDLE_VALUE) {
		m_pImpl->hFileMapping = CreateFileMapping(m_pImpl->hFile, NULL, PAGE_READONLY, 0, 0, NULL);
	}
#endif
	return m_pImpl->hFileMapping;
}

LPVOID CFileCacheManager::getMapView(DWORD dwFileOffsetHigh, DWORD dwFileOffsetLow, DWORD dwNumberOfBytesToMap)
{
	if (!m_pImpl->isOpenCache() || INVALID_HANDLE_VALUE == m_pImpl->hFileMapping)
		return NULL;
#ifdef WIN32
	return MapViewOfFile(m_pImpl->hFileMapping, FILE_MAP_READ, dwFileOffsetHigh, dwFileOffsetLow, dwNumberOfBytesToMap);
#endif

#ifdef __MOBILE__
	return NULL;
#endif
}

BOOL CFileCacheManager::unMapVew(LPVOID pMapAddr)
{
#ifdef WIN32
	return UnmapViewOfFile(pMapAddr);
#endif

#ifdef __MOBILE__
	return true;
#endif
}
}
