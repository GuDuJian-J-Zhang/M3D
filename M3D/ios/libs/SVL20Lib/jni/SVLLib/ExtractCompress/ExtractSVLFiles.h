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
		const unsigned int i_niBufferSize,		// (i) ��ѹѹ���ļ���С
		const char *i_bufP,						// (i) ��ѹѹ���ļ��ڴ�
		const std::wstring &i_strArcNameA,		// (i) ��ѹ�ļ�����
		char ***o_bufP,							// (i) ��ѹ�ļ�����
		std::vector<int> &o_vcBufNum			// (i) ��ѹ�ļ����ݴ�С
		);
	static bool ExtractBufferToSVLFiles(
		const unsigned int i_niBufferSize,		// (i) ��ѹѹ���ļ���С
		const char *i_bufP,						// (i) ��ѹѹ���ļ��ڴ�
		const std::string &i_strArcNameA,		// (i) ��ѹ�ļ�����
		char ***o_bufP,							// (i) ��ѹ�ļ�����
		std::vector<int> &o_vcBufNum			// (i) ��ѹ�ļ����ݴ�С
	);

	static bool ExtractFileToSVLFiles(
		const std::wstring &i_strDestCurDirPrefix,	// (i) ��ѹ�ļ�·��
		const std::wstring &i_strArcNameA,			// (i) ��ѹ�ļ�����
		char ***o_bufP,								// (o) ��ѹ�ļ�����
		std::vector<int> &o_vcBufNum				// (o) ��ѹ�ļ����ݴ�С
		);
	static bool ExtractFileToSVLFiles(
		const std::string &i_strDestCurDirPrefix,	// (i) ��ѹ�ļ�·��
		const std::string &i_strArcNameA,			// (i) ��ѹ�ļ�����
		char ***o_bufP,								// (o) ��ѹ�ļ�����
		std::vector<int> &o_vcBufNum				// (o) ��ѹ�ļ����ݴ�С
	);
	
	static bool ExtractFileToAllSVLFiles(
		const std::wstring &i_strDestCurDirPrefix,			// (i) ��ѹ�ļ�·��
		std::vector<std::wstring> &o_strArcNameA,			// (o) ��ѹ�ļ�����
		char ****o_bufP,									// (o) ��ѹ�ļ�����
		std::vector<std::vector<int>> &o_vcBufNum			// (o) ��ѹ�ļ����ݴ�С
		);

	static bool ExtractFileToAllSVLFiles(
		const std::string &i_strDestCurDirPrefix,			// (i) ��ѹ�ļ�·��
		std::vector<std::string> &o_strArcNameA,			// (o) ��ѹ�ļ�����
		char ****o_bufP,									// (o) ��ѹ�ļ�����
		std::vector<std::vector<int>> &o_vcBufNum			// (o) ��ѹ�ļ����ݴ�С
	);

	static bool ExtractBufferToAllSVLFiles(
		const unsigned int i_niBufferSize,					// (i) ��ѹѹ���ļ���С
		const char *i_bufP,									// (i) ��ѹѹ���ļ��ڴ�
		std::vector<std::wstring> &o_strArcNameA,			// (o) ��ѹ�ļ�����
		char ****o_bufP,									// (o) ��ѹ�ļ�����
		std::vector<std::vector<int>> &o_vcBufNum			// (o) ��ѹ�ļ����ݴ�С
		);

	static bool ExtractBufferToAllSVLFiles(
		const unsigned int i_niBufferSize,					// (i) ��ѹѹ���ļ���С
		const char *i_bufP,									// (i) ��ѹѹ���ļ��ڴ�
		std::vector<std::string> &o_strArcNameA,			// (o) ��ѹ�ļ�����
		char ****o_bufP,									// (o) ��ѹ�ļ�����
		std::vector<std::vector<int>> &o_vcBufNum			// (o) ��ѹ�ļ����ݴ�С
	);

	bool GetFolderFileList(const unsigned int i_niBufferSize,		// (i) ��ѹѹ���ļ���С
		const char *i_bufP,						// (i) ��ѹѹ���ļ��ڴ�
		const std::wstring &i_strFolderName, 
		std::vector<std::wstring> &o_strFileNameA, /* (o) �ļ����б� */ 
		std::vector<std::wstring> &o_strFolderNameA /* (o) �ļ����б� */);

	bool GetFolderFileList(const unsigned int i_niBufferSize,		// (i) ��ѹѹ���ļ���С
		const char *i_bufP,						// (i) ��ѹѹ���ļ��ڴ�
		const std::string &i_strFolderName,
		std::vector<std::string> &o_strFileNameA, /* (o) �ļ����б� */
		std::vector<std::string> &o_strFolderNameA /* (o) �ļ����б� */);
};

