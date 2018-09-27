#ifdef WIN32
#include <atlstr.h>
#else
#include "../AutoTchar.h"
#include "Stk_Define.h"
#endif
#include "Stk_Object.h"

#pragma once
class STK_TOOLKIT_EXPORT CGetCompressContents
{
public:
	CGetCompressContents();
	~CGetCompressContents();

public:
	bool GetCompressBufferContents(
		const unsigned int i_niBufferSize,					// (i) 解压压缩文件大小
		const char *i_bufP,									// (i) 解压压缩文件内存
		std::vector<std::string> &o_strFileContentsA		// (o) 压缩文件目录
		);
	bool GetCompressFilesContents(
		const std::string &i_strDestCurDirPrefix,			// (i) 压缩文件存放路径
		std::vector<std::string> &o_strFileContentsA		// (o) 压缩文件目录
		);
};

