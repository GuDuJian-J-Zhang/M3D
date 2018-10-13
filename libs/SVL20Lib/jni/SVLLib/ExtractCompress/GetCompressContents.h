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
		const unsigned int i_niBufferSize,					// (i) ��ѹѹ���ļ���С
		const char *i_bufP,									// (i) ��ѹѹ���ļ��ڴ�
		std::vector<std::string> &o_strFileContentsA		// (o) ѹ���ļ�Ŀ¼
		);
	bool GetCompressFilesContents(
		const std::string &i_strDestCurDirPrefix,			// (i) ѹ���ļ����·��
		std::vector<std::string> &o_strFileContentsA		// (o) ѹ���ļ�Ŀ¼
		);
};

