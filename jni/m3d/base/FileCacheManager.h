/**@file    FileCacheManager
 *@brief	磁盘缓存管理工具类，实现将指定的数据段写入文件，提供返回指定数据段内存映射地址
 *@author	Create by zhouyunpeng
 *@date		2014-1-7
 *@version	1.0
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 */

#ifndef FILE_CACHE_H
#define FILE_CACHE_H
#include "m3d/M3D.h"
#ifdef WIN32
#include <windows.h>
#endif

#ifdef __MOBILE__
#include <stdlib.h>
typedef unsigned long DWORD;
typedef int BOOL;
typedef unsigned char BYTE;
typedef void * LPVOID;
typedef void *HANDLE;
typedef const char* LPCWSTR;
typedef void* LPCVOID;

typedef char WCHAR;
typedef unsigned int UINT;
#endif

namespace M3D
{
class CFileCacheManagerP;
class M3D_API CFileCacheManager
{
public:
	/** 
	 * @brief 构造函数
	 */
	CFileCacheManager();
	/** 
	 * @brief 析构函数
	 */
	~CFileCacheManager();
	/** 
	 * @brief  单例模式
	 */
	static CFileCacheManager* getInstance();

private:
	CFileCacheManagerP* m_pImpl; //!< 实现模式
	friend class CFileCacheManagerP;//!< 实现模式

public:
	/** 
	 * @brief 开始缓存接口 传入文件路径
	 */
	int bgeinCache(LPCWSTR strCacheFile);
	/** 
	 * @brief 开始缓存接口
	 */
	int bgeinCache();
	/** 
	 * @brief 开始写缓存 输入输入地址 和需要缓存的长度，返回文件段内的地址和段内的偏移量
	 */
	DWORD writeCache(LPCVOID lpBuffer, DWORD dwLength, DWORD& outSegmentAdd,
			DWORD& outOffsetAdd);
	/** 
	 * @brief 结束写缓存
	 */
	int endWriteCache();
	/** 
	 * @brief 结束文件缓存，清理临时文件资源等操作
	 */
	void endCache();
	/** 
	 * @brief 是否开始缓存
	 */
	BOOL isOpenCache();
	/** 
	 * @brief 开启文件映射
	 */
	HANDLE getFileMapping();
	/** 
	 * @brief 根据数据总长度，段地址和段内偏移量，返回映射后的内存地址，可以直接操作内存对象
	 */
	LPVOID getMapView(DWORD dwFileOffsetHigh, DWORD dwFileOffsetLow,
			DWORD dwNumberOfBytesToMap);
	/** 
	 * @brief 关闭上次getMapView映射的内存
	 */
	BOOL unMapVew(LPVOID pMapAddr);
	/** 
	 * @brief 关闭文件映射
	 */
	void endMapping();
};
}
#endif
