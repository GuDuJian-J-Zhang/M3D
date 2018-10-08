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
#include <string>
#include <iostream>

using std::string;
using std::iostream;
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

};
}
#endif/*M3D_FILE_HELPER_H*/
