/**@file
 *@brief
 *@author	XL
 *@date		2014-1-7
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */

#ifndef M3D_STRING_HELPER_H_
#define  M3D_STRING_HELPER_H_
#include "m3d/M3DExport.h"
namespace M3D
{
/**
 * @class
 * @brief 字符串辅助类。提供操作字符串的工具函数
 */
class M3D_API StringHelper
{
public:
	/**
	 * 得到统一风格的文件路径  ".../.../..."
	 * @param filePath
	 * @return
	 */
	static string GetUnionStylePath(const string filePath);
	/**
	 * 统一替换oldvalue标识的字符串，成newvalue字符串
	 * @param src
	 * @param oldvalue
	 * @param newvalue
	 */
	static void Replace_all(string& src, const string& oldvalue,
			const string& newvalue);

	static bool HasSuffix(const std::string &str, const std::string &suffix)
	{
		return str.size() >= suffix.size() &&
			str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
	}
	/**
	 * 是否是空格子字符
	 * @param c
	 * @return
	 */
	static bool IsSpace(const char c)
	{
		return (c == ' ') || (c == '\t');
	}

	/**
	 * 字符串转换成大写
	 * @param src
	 * @return
	 */
	static string ToUpper(const string& src);

	/**
	 * 字符串转换成小写
	 * @param src
	 * @return
	 */
	static string ToLower(const string& src);

	/**
	 * 整数转换成字符串
	 * @param iZ
	 * @return
	 */
	static string IntToString(int iZ)
	{
		char tempstr[64];
		memset(tempstr, '\0', 64);
		sprintf(tempstr, "%d", iZ);
		return string(tempstr);
	}

	/**
	 * 字符串转换成string
	 * @param fZ
	 * @param alen
	 * @return
	 */
	static string floatToString(float fZ, const int alen = 3)
	{
		char tempstr[256];
		memset(tempstr, '\0', 256);
		sprintf(tempstr, "%.*lf", alen, fZ);
		return string(tempstr);
	}

	/**
	 *字符串转换成int
	 * @param src
	 * @return
	 */
	static int StringToInt(const string& src)
	{
		int n = 0;
		n = atoi(src.c_str());
		return n;
	}

	/**
	 * 字符串转成成float
	 * @param src
	 * @return
	 */
	static float StringToFloat(const string& src)
	{
		float f = 0.0f;
		f = atof(src.c_str());
		return f;
	}

	/**
	 * if split "a,b,,d" by ",", result is "a", "b", "", "d".
	 * @param strTarget
	 * @param strDelimiter
	 * @return
	 */
	static vector<string> Split(const string &strTarget,
			const string &strDelimiter);

	static vector<string> SplitIngnoreEmpty(const string &strTarget,
		const string &strDelimiter);

	/**
	 * if split "a,b,,d" by ",", result is "a"
	 * @param strTarget
	 * @param strKey
	 * @return
	 */
	static string GetBefore(const string &strTarget, const string &strKey);

	/**
	 * if split "a,b,,d" by ",", result is "b,,d"
	 * @param strTarget
	 * @param strKey
	 * @return
	 */
	static string GetAfter(const string &strTarget, const string &strKey);

	/**
	 * 裁剪掉两段的空白
	 * @param strTarget
	 * @return
	 */
	static string TrimWhiteSpace(const string &strTarget);

	/**
	 * 得到以strStartKey和strEndKey之间的字符串
	 * @param strTarget
	 * @param strStartKey
	 * @param strEndKey
	 * @return
	 */
	static string GetBetween(const string &strTarget, const string &strStartKey,
			const string &strEndKey);

	/**
	 * 得到大写字符串
	 * @param strTarget
	 * @return
	 */
	static string getUpperCase(const string &strTarget);

	/**
	 * 得到小写字符串
	 * @param strTarget
	 * @return
	 */
	static string getLowerCase(const string &strTarget);

	/**
	 * 查找strKey在strTarget中出现的所有位置
	 * @param strTarget
	 * @param strKey
	 * @return
	 */
	static vector<std::string::size_type> GetIndexList(const string &strTarget,
			const string &strKey);

	/**
	 * if iCount <= 0, return string is empty
	 * @param strTarget
	 * @param iCount
	 * @return
	 */
	static string Repeat(const string &strTarget, const int &iCount);

	/**
	 * 将src进行分割
	 * @param src
	 * @param begin
	 * @param end
	 * @return
	 */
	static string subString(const string& src, int begin, int end);

	static bool Startswith(const std::string& str, const std::string& start);

	static bool Endswith(const std::string& str, const std::string& end);

	static string Trim(const std::string& str);
};
}
#endif /*M3D_STRING_HELPER_H_*/
