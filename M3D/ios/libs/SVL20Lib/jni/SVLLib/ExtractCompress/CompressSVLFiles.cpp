
#include "CompressSVLFiles.h"
#include "Compress.h"
#include <stdlib.h>
#include <sys/stat.h>
#ifdef WIN32
#include <io.h>
#include <direct.h>
#else
#include <sys/types.h>
#include <dirent.h>
#include <iostream>
#include <unistd.h>
#endif

extern TCHAR *char2tchar(char *str);


bool get_src_files_name(std::string srcDirPath, std::vector<std::string>& fileNameList)
{
#ifdef _WIN32  
	_finddata_t file;
	long lf;
	std::string src = srcDirPath + "\\*.*";
	if ((lf = _findfirst(src.c_str(), &file)) == -1)
	{
		return false;
	}
	else {
		while (_findnext(lf, &file) == 0)
		{
			if (strcmp(file.name, ".") == 0 || strcmp(file.name, "..") == 0)
				continue;
			fileNameList.push_back(file.name);
		}
	}


	_findclose(lf);
#else  // Linux  
	DIR *dir;
	struct dirent *ptr;

	if ((dir = opendir(srcDirPath.c_str())) == NULL)
	{
		std::cout << srcDirPath << " not found" << std::endl;
		return false;
	}

	while ((ptr = readdir(dir)) != NULL)
	{
		if ((ptr->d_name == ".") || (ptr->d_name == ".."))  //current / parent  
			continue;
		else if (ptr->d_type == 8)  //file  
			fileNameList.push_back(ptr->d_name);
		else if (ptr->d_type == 10)  //link file  
			continue;
		else if (ptr->d_type == 4)  //dir  
			fileNameList.push_back(ptr->d_name);
	}
	closedir(dir);

#endif  

	return true;

}

CCompressSVLFiles::CCompressSVLFiles()
{
}


CCompressSVLFiles::~CCompressSVLFiles()
{
}

bool CCompressSVLFiles::CompressBufferToSVLFiles(
	const std::string &i_strDestCurDirPrefix,		// (i) 压缩文件存放路径
	const std::vector<std::string> &i_strArcNameA,	// (i) 压缩文件名称
	const char **i_bufP,							// (i) 压缩内存区域
	std::vector<unsigned int> &i_vcBufSize			// (i) 解压文件数据大小
	)
{
	HZIP hz;
	char *strP = (char *)i_strDestCurDirPrefix.c_str();
	TCHAR *strPathName;
	strPathName = char2tchar(strP);
	hz = CreateZip(strPathName, 0);

	TCHAR *strFileName;
	std::string strFile;
	for (int iIndex = 0; iIndex < (int)i_vcBufSize.size(); iIndex++)
	{
		strFile = i_strArcNameA.at(iIndex);
		strP = (char *)strFile.c_str();
		strFileName = char2tchar(strP);
		ZipAdd(hz, strFileName, (void*)i_bufP[iIndex], i_vcBufSize.at(iIndex));
		delete[]strFileName;
	}

	delete[]strPathName;
	CloseZip(hz);
	return true;
}

bool CCompressSVLFiles::CompressHandleToSVLFiles(
	const std::string &i_strDestCurDirPrefix,		// (i) 压缩文件存放路径
	const std::vector<std::string> &i_strArcNameA,	// (i) 压缩文件名称
	const HANDLE i_handle[],						// (i) 压缩文件句柄数组
	const int i_iHandleNum							// (i) 压缩文件句柄数
)
{
	HZIP hz;
	char *strP = (char *)i_strDestCurDirPrefix.c_str();
	TCHAR *strPathName;
	strPathName = char2tchar(strP);
	hz = CreateZip(strPathName, 0);

	TCHAR *strFileName;
	std::string strFile;
	for (int iIndex = 0; iIndex < i_iHandleNum; iIndex++)
	{
		strFile = i_strArcNameA.at(iIndex);
		strP = (char *)strFile.c_str();
		strFileName = char2tchar(strP);
		ZipAddHandle(hz, strFileName, i_handle[iIndex]);
		delete[]strFileName;
	}

	delete[]strPathName;
	CloseZip(hz);
	return true;
}

bool CCompressSVLFiles::CompressHandleToSVLFiles(
	const std::string &i_strDestCurDirPrefix,		// (i) 压缩文件存放路径
	const std::vector<std::string> &i_strArcNameA,	// (i) 压缩文件名称
	const HANDLE i_handle[],						// (i) 压缩文件句柄数组
	const int i_iHandleNum,							// (i) 压缩文件句柄数
	const std::vector<std::string> &i_strFolderNameA,	// (i) 压缩文件名称
	const std::vector<std::string> &i_FolderPathA,	// (i) 压缩目录路径数组
	const int i_iFolderNum							// (i) 压缩目录数
	)
{
	HZIP hz;
	char *strP = (char *)i_strDestCurDirPrefix.c_str();
	TCHAR *strPathName;
	strPathName = char2tchar(strP);
	hz = CreateZip(strPathName, 0);

	TCHAR *strFileName;
	std::string strFile;
	for (int iIndex = 0; iIndex < i_iHandleNum; iIndex ++)
	{
		strFile = i_strArcNameA.at(iIndex);
		strP = (char *)strFile.c_str();
		strFileName = char2tchar(strP);
		ZipAddHandle(hz, strFileName, i_handle[iIndex]);
		delete []strFileName;
	}

	for (int iIndex = 0; iIndex < i_iFolderNum; iIndex++)
	{
		const std::string strFolderName = i_strFolderNameA[iIndex];
		const std::string strFolderPath = i_FolderPathA[iIndex];
		CompressFileToSVLFiles(hz, strFolderName, strFolderPath);
		rmdir(strFolderPath.c_str());
	}

	delete []strPathName;
	CloseZip(hz);
	return true;
}

bool CCompressSVLFiles::CompressFileToSVLFiles(
	HZIP hz,										/* (i) 文件句柄 */
	const std::string &i_strFileName,				/* (i) 压缩文件名称 */
	const std::string &i_strFilePath				/* (i) 压缩文件路径 */
) 
{
	TCHAR *strFileName = 0;
	TCHAR *strFilePath = 0;
	char *strP = (char *)i_strFileName.c_str();
	strFileName = char2tchar(strP);
	strFilePath = char2tchar((char*)i_strFilePath.c_str());
	struct stat fileStat;
#ifdef _WIN32
	if ((stat(i_strFilePath.c_str(), &fileStat) == 0) && (fileStat.st_mode & _S_IFDIR))
#else
	if ((stat(i_strFilePath.c_str(), &fileStat) == 0) && (fileStat.st_mode & S_IFDIR))
#endif
	{
		ZipAddFolder(hz, strFileName);

		std::vector<std::string> fileNameList;
		get_src_files_name(i_strFilePath, fileNameList);
		for (int i = 0; i < fileNameList.size(); i++)
		{
#ifdef _WIN32  
			string strCurFilePath = i_strFilePath + "\\" + fileNameList[i];
			string strCurFileName = i_strFileName + "/" + fileNameList[i];
#else
            string strCurFilePath = i_strFilePath;
            string strCurFileName = i_strFileName;
            if(fileNameList[i] != "." && fileNameList[i] != ".."){
                strCurFilePath = i_strFilePath + "/" + fileNameList[i];
                strCurFileName = i_strFileName + "/" + fileNameList[i];
            }else{
                continue;
            }
#endif
			CompressFileToSVLFiles(hz, strCurFileName, strCurFilePath);
		}
		rmdir(i_strFilePath.c_str());
	}
	else
	{
        if (stat(i_strFilePath.c_str(), &fileStat) == 0){
                
#ifdef _WIN32
		HANDLE tempHandle = CreateFileA(i_strFilePath.c_str(), GENERIC_READ, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_TEMPORARY | FILE_FLAG_DELETE_ON_CLOSE, 0);
		ZipAddHandle(hz, strFileName, tempHandle);
		CloseHandle(tempHandle);
#else
		ZipAdd(hz, strFileName, strFilePath);
#endif
            remove(i_strFilePath.c_str());
        }
	}
	
	delete[]strFileName;
	delete[]strFilePath;
	return true;
}

bool CCompressSVLFiles::CompressFileNameToSVLFiles(
		const std::string &i_strDestCurDirPrefix,		// (i) 压缩文件存放路径
		const std::vector<std::string> &i_strArcNameA,	// (i) 压缩文件名称
		const std::vector<std::string> &i_FilePathA,	// (i) 压缩文件路径数组
		const int i_iFileNum							// (i) 压缩文件句柄数
		)
{
	HZIP hz;
	char *strP = (char *)i_strDestCurDirPrefix.c_str();
	TCHAR *strPathName;
	strPathName = char2tchar(strP);
	hz = CreateZip(strPathName, 0);

	TCHAR *strFileName = 0;
	TCHAR *strFilePath = 0;
	std::string strFile;
	for (int iIndex = 0; iIndex < i_iFileNum; iIndex ++)
	{
		//strFile = i_strArcNameA.at(iIndex);
		//strP = (char *)strFile.c_str();
		//strFileName = char2tchar(strP);
		//strFilePath = char2tchar((char*)i_FilePathA[iIndex].c_str());
		//ZipAdd(hz, strFileName, strFilePath);
		//remove(i_FilePathA[iIndex].c_str());
		//delete []strFileName;
		//delete []strFilePath;
		const std::string strFileName = i_strArcNameA[iIndex];
		const std::string strFilePath = i_FilePathA[iIndex];
		CompressFileToSVLFiles(hz, strFileName, strFilePath);
		remove(strFilePath.c_str());
	}

	delete []strPathName;
	CloseZip(hz);
	return true;
}
