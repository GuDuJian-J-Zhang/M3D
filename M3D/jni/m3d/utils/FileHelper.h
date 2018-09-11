/**@file
 *@brief
 *@author	XL
 *@date		2014-1-7
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */

#ifndef M3D_FILE_HELPER_H
#define  M3D_FILE_HELPER_H
#include "M3D/M3DExport.h"
namespace M3D
{

/**
 * @class
 * @brief 文件辅助类。用来提供一些文件操作辅助函数
 */
class M3D_API FileHelper
{
public:
	static const int NUMBER_OF_SAMPLES = 2048;

public:
	/**
	 * 得到统一风格的文件路径  ".../.../..."
	 * @param filePath
	 * @return
	 */
	static string GetUnionStylePath(const string filePath);

	static void Replace_all(string& src,const string& oldvalue, const string& newvalue);

	/**
	 * 根据路径得到去除文件名的路径
	 * @param filePath
	 * @return
	 */
	static string GetDirName(const string& filePath);

	/**
	 * 根据路径得到文件名称
	 * @param filePath
	 * @return
	 */
	static string GetFileName(const string& filePath);
	static string GetFileNameWithoutExt(const string& filePath);
	/**
	 * 得到后缀名
	 * @param filePath
	 * @return
	 */
	static string GetExt(const string& filePath);

	/**
	 * 获取文本文件编码方式
	 * @param filePath
	 * @return
	 */
	static string GetEnCode(const string& filePath);

	/**
	 * 判断文件存在
	 * @param filePath
	 * @return
	 */
	static bool FileExist(const string& filePath);
	static int is_dir(const char* path);
	static int endwith(const char* s, char c);
	static string str_contact(const char* str1, const char* str2);
	static void copy_file(const char* source_path, const char *destination_path);
	static void copy_folder(const char* source_path, const char *destination_path);

	static bool make_dir(const std::string& pathName);
	static bool delete_dir(std::string strDir);
	static bool get_src_files_name(const string&srcDirPath, std::vector<std::string>& fileNameList);
	static void do_copy(const string&srcDirPath, const string&desDirPath, const std::vector<std::string> &fileNameList);

	/************************************************************************/
	/* 读取文本文件                                                                     */
	/************************************************************************/

	static string ReadAsciiFile(const string& filePath);
	static string  ReadTxt(string file);

	//以二进制的方式读取buffer，需要在外部释放此反馈的buffer
	static bool ReadBinFile(const string& filePath,char* &readbuffer,int& bufferLength);

	static bool RemoveFile(const string& filePath);

	static int SaveFileAssAscii(const string& fileContent,const string& destFilePath);

	static int SaveFileAsBin(const char* buffer, int bufferSize,const string& destFilePath);
};
}
#endif/*M3D_FILE_HELPER_H*/
