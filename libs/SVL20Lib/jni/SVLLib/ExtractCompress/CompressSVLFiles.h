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
		const std::string &i_strDestCurDirPrefix,		// (i) ѹ���ļ����·��
		const std::vector<std::string> &i_strArcNameA,	// (i) ѹ���ļ�����
		const HANDLE i_handle[],						// (i) ѹ���ļ��������
		const int i_iHandleNum							// (i) ѹ���ļ������
	);
	bool CompressHandleToSVLFiles(
		const std::string &i_strDestCurDirPrefix,		// (i) ѹ���ļ����·��
		const std::vector<std::string> &i_strArcNameA,	// (i) ѹ���ļ�����
		const HANDLE i_handle[],						// (i) ѹ���ļ��������
		const int i_iHandleNum,							// (i) ѹ���ļ������
		const std::vector<std::string> &i_strFolderNameA,	// (i) ѹ���ļ�����
		const std::vector<std::string> &i_FolderPathA,	// (i) ѹ��Ŀ¼·������
		const int i_iFolderNum							// (i) ѹ��Ŀ¼��
		);
	bool CompressFileToSVLFiles(
		HZIP hz,										/* (i) �ļ���� */ 
		const std::string &i_strFileName,				/* (i) ѹ���ļ����� */ 
		const std::string &i_strFilePath					/* (i) ѹ���ļ�·�� */
	);
	bool CompressBufferToSVLFiles(
		const std::string &i_strDestCurDirPrefix,		// (i) ѹ���ļ����·��
		const std::vector<std::string> &i_strArcNameA,	// (i) ѹ���ļ�����
		const char **i_bufP,							// (i) ѹ���ڴ�����
		std::vector<unsigned int> &i_vcBufNum			// (i) ��ѹ�ļ����ݴ�С
		);
	bool CompressFileNameToSVLFiles(
		const std::string &i_strDestCurDirPrefix,		// (i) ѹ���ļ����·��
		const std::vector<std::string> &i_strArcNameA,	// (i) ѹ���ļ�����
		const std::vector<std::string> &i_FilePathA,	// (i) ѹ���ļ�·������
		const int i_iFileNum							// (i) ѹ���ļ������
		);
};

