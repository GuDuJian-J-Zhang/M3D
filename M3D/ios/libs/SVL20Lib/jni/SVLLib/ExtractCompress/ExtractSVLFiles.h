#ifdef WIN32
#include <atlstr.h>
#else
#include "AutoTchar.h"
#include "ExtractTypeDef.h"
#endif
#include "Stk_Object.h"
#include "7zTypes.h"

#pragma once
class STK_TOOLKIT_EXPORT CExtractSVLFiles
{
public:
	CExtractSVLFiles();
	~CExtractSVLFiles();

public:
	static bool ExtractBufferToSVLFiles(
		const unsigned int i_niBufferSize,		// (i) 解压压缩文件大小
		const char *i_bufP,						// (i) 解压压缩文件内存
		const std::wstring &i_strArcNameA,		// (i) 解压文件名称
		char ***o_bufP,							// (i) 解压文件数据
		std::vector<int> &o_vcBufNum			// (i) 解压文件数据大小
		);
	static bool ExtractBufferToSVLFiles(
		const unsigned int i_niBufferSize,		// (i) 解压压缩文件大小
		const char *i_bufP,						// (i) 解压压缩文件内存
		const std::string &i_strArcNameA,		// (i) 解压文件名称
		char ***o_bufP,							// (i) 解压文件数据
		std::vector<int> &o_vcBufNum			// (i) 解压文件数据大小
	);

	static bool ExtractFileToSVLFiles(
		const std::wstring &i_strDestCurDirPrefix,	// (i) 解压文件路径
		const std::wstring &i_strArcNameA,			// (i) 解压文件名称
		char ***o_bufP,								// (o) 解压文件数据
		std::vector<int> &o_vcBufNum				// (o) 解压文件数据大小
		);
	static bool ExtractFileToSVLFiles(
		const std::string &i_strDestCurDirPrefix,	// (i) 解压文件路径
		const std::string &i_strArcNameA,			// (i) 解压文件名称
		char ***o_bufP,								// (o) 解压文件数据
		std::vector<int> &o_vcBufNum				// (o) 解压文件数据大小
	);
	
	static bool ExtractFileToAllSVLFiles(
		const std::wstring &i_strDestCurDirPrefix,			// (i) 解压文件路径
		std::vector<std::wstring> &o_strArcNameA,			// (o) 解压文件名称
		char ****o_bufP,									// (o) 解压文件数据
		std::vector<std::vector<int>> &o_vcBufNum			// (o) 解压文件数据大小
		);

	static bool ExtractFileToAllSVLFiles(
		const std::string &i_strDestCurDirPrefix,			// (i) 解压文件路径
		std::vector<std::string> &o_strArcNameA,			// (o) 解压文件名称
		char ****o_bufP,									// (o) 解压文件数据
		std::vector<std::vector<int>> &o_vcBufNum			// (o) 解压文件数据大小
	);

	static bool ExtractBufferToAllSVLFiles(
		const unsigned int i_niBufferSize,					// (i) 解压压缩文件大小
		const char *i_bufP,									// (i) 解压压缩文件内存
		std::vector<std::wstring> &o_strArcNameA,			// (o) 解压文件名称
		char ****o_bufP,									// (o) 解压文件数据
		std::vector<std::vector<int>> &o_vcBufNum			// (o) 解压文件数据大小
		);

	static bool ExtractBufferToAllSVLFiles(
		const unsigned int i_niBufferSize,					// (i) 解压压缩文件大小
		const char *i_bufP,									// (i) 解压压缩文件内存
		std::vector<std::string> &o_strArcNameA,			// (o) 解压文件名称
		char ****o_bufP,									// (o) 解压文件数据
		std::vector<std::vector<int>> &o_vcBufNum			// (o) 解压文件数据大小
	);

	bool GetFolderFileList(const unsigned int i_niBufferSize,		// (i) 解压压缩文件大小
		const char *i_bufP,						// (i) 解压压缩文件内存
		const std::wstring &i_strFolderName, 
		std::vector<std::wstring> &o_strFileNameA, /* (o) 文件名列表 */ 
		std::vector<std::wstring> &o_strFolderNameA /* (o) 文件夹列表 */);

	bool GetFolderFileList(const unsigned int i_niBufferSize,		// (i) 解压压缩文件大小
		const char *i_bufP,						// (i) 解压压缩文件内存
		const std::string &i_strFolderName,
		std::vector<std::string> &o_strFileNameA, /* (o) 文件名列表 */
		std::vector<std::string> &o_strFolderNameA /* (o) 文件夹列表 */);
};

