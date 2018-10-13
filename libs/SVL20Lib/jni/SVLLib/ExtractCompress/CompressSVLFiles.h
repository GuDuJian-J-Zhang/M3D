#ifdef __MOBILE__
#include "../AutoTchar.h"
#include "ExtractTypeDef.h"
#else
#include <atlstr.h>
#endif
#include "Stk_Object.h"

#pragma once
#include "Compress.h"
class CCompressSVLFiles
{
public:
	CCompressSVLFiles();
	~CCompressSVLFiles();

public:
	bool CompressHandleToSVLFiles(
		const std::string &i_strDestCurDirPrefix,		// (i) 压缩文件存放路径
		const std::vector<std::string> &i_strArcNameA,	// (i) 压缩文件名称
		const HANDLE i_handle[],						// (i) 压缩文件句柄数组
		const int i_iHandleNum							// (i) 压缩文件句柄数
	);
	bool CompressHandleToSVLFiles(
		const std::string &i_strDestCurDirPrefix,		// (i) 压缩文件存放路径
		const std::vector<std::string> &i_strArcNameA,	// (i) 压缩文件名称
		const HANDLE i_handle[],						// (i) 压缩文件句柄数组
		const int i_iHandleNum,							// (i) 压缩文件句柄数
		const std::vector<std::string> &i_strFolderNameA,	// (i) 压缩文件名称
		const std::vector<std::string> &i_FolderPathA,	// (i) 压缩目录路径数组
		const int i_iFolderNum							// (i) 压缩目录数
		);
	bool CompressFileToSVLFiles(
		HZIP hz,										/* (i) 文件句柄 */ 
		const std::string &i_strFileName,				/* (i) 压缩文件名称 */ 
		const std::string &i_strFilePath					/* (i) 压缩文件路径 */
	);
	bool CompressBufferToSVLFiles(
		const std::string &i_strDestCurDirPrefix,		// (i) 压缩文件存放路径
		const std::vector<std::string> &i_strArcNameA,	// (i) 压缩文件名称
		const char **i_bufP,							// (i) 压缩内存区域
		std::vector<unsigned int> &i_vcBufNum			// (i) 解压文件数据大小
		);
	bool CompressFileNameToSVLFiles(
		const std::string &i_strDestCurDirPrefix,		// (i) 压缩文件存放路径
		const std::vector<std::string> &i_strArcNameA,	// (i) 压缩文件名称
		const std::vector<std::string> &i_FilePathA,	// (i) 压缩文件路径数组
		const int i_iFileNum							// (i) 压缩文件句柄数
		);
};

