#include "m3d/M3D.h"
#include "m3d/utils/StringHelper.h"

#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <cctype>
#include <iostream>

using namespace std;


namespace M3D{
string StringHelper::GetUnionStylePath(const string filePath)
{
	string linuxstylePath = filePath;
	StringHelper::Replace_all(linuxstylePath,"\\","/");
	return linuxstylePath;
}
   
void StringHelper::Replace_all(string& src,const string& oldvalue, const string& newvalue)
{
	while (true)
	{
		string::size_type pos(0);
		if((pos = src.find(oldvalue) ) !=string::npos )
		{
			src.replace(pos,oldvalue.length(),newvalue);
		}
		else
		{
			break;
		}
	}
}

string StringHelper::ToUpper(const string& src)
{
	string ret = src;
	transform(ret.begin(),ret.end(),ret.begin(),::toupper);
	return ret;
}

string StringHelper::ToLower(const string& src)
{
	string ret  = src ;
	transform(ret.begin(),ret.end(),ret.begin(),::tolower);
	return ret;
}

vector<string> StringHelper::Split(const string &strTarget, const string &strDelimiter)
{
	if (("" == strTarget) || ("" == strDelimiter)) return vector<string>();
	string str = strTarget;
	std::string::size_type pos;
	std::vector<std::string> result;
	str += strDelimiter;//扩展字符串以方便操作
	int size = str.size();

	for (int i = 0; i < size; i++)
	{
		pos = str.find(strDelimiter, i);
		if (pos < size)
		{
			std::string s = str.substr(i, pos - i);
			result.push_back(s);
			i = pos + strDelimiter.size() - 1;
		}
	}
	return result;
}

vector<string> StringHelper::SplitIngnoreEmpty(const string &strTarget,
	const string &strDelimiter) {
	if (("" == strTarget) || ("" == strDelimiter)) return vector<string>();
	string str = strTarget;
	std::string::size_type pos;
	std::vector<std::string> result;
	str += strDelimiter;//扩展字符串以方便操作
	int size = str.size();

	for (int i = 0; i < size; i++)
	{
		pos = str.find(strDelimiter, i);
		if (pos < size)
		{
			std::string s = str.substr(i, pos - i);
			if (s.size()>0)
			{
				result.push_back(s);
			}
			i = pos + strDelimiter.size() - 1;
		}
	}
	return result;
}

string StringHelper::GetBefore(const string &strTarget, const string &strKey)
{
	if (("" == strTarget) || ("" == strKey)) return string();
	std::string::size_type siIndex = strTarget.find(strKey);
	if (std::string::npos == siIndex)
		return std::string();
	return strTarget.substr(0, siIndex);
}

string StringHelper::GetAfter(const string &strTarget, const string &strKey)
{
	if (("" == strTarget) || ("" == strKey)) return string();
	std::string::size_type siIndex = strTarget.find(strKey);
	if (std::string::npos == siIndex)
		return std::string();
	return strTarget.substr(siIndex + strKey.size());
}

string StringHelper::TrimWhiteSpace(const string &strTarget)
{
	if ("" == strTarget) return string();
	std::string::size_type siStart;
	for (std::string::size_type i = 0;i < strTarget.size();++i)
	{
		if (' ' != strTarget.at(i))
		{
			siStart = i;
			break;
		}
	}
	std::string::size_type siEnd;
	for (std::string::size_type i = strTarget.size() - 1;0 <= i;--i)
	{
		if (' ' != strTarget.at(i))
		{
			siEnd = i;
			break;
		}
	}
	return strTarget.substr(siStart, siEnd - siStart + 1);
}

string StringHelper::GetBetween(const string &strTarget, const string &strStartKey, const string &strEndKey)
{
	return GetBefore(GetAfter(strTarget, strStartKey), strEndKey);
}

vector<string::size_type> StringHelper::GetIndexList(const string &strTarget, const string &strKey)
{
	if (("" == strTarget) || ("" == strKey)) return vector<string::size_type>();
	std::string::size_type siCurrent = 0;
	std::string::size_type siFound   = 0;
	vector<string::size_type> vecIndex;
	while (std::string::npos != (siFound = strTarget.find(strKey, siCurrent)))
	{
		vecIndex.push_back(siFound);
		siCurrent = siFound + strKey.size();
	}
	return vecIndex;
}

string StringHelper::Repeat(const std::string &strTarget, const int &iCount)
{
	if (iCount <= 0) return string();
	string strTmp;
	for (int i = 0;i < iCount;++i)
		strTmp += strTarget;
	return strTmp;
}

string StringHelper::subString(const string& src,int begin,int end)
{
	return src.substr(begin,end);
}

bool StringHelper::Startswith(const std::string& str, const std::string& start)
{
	int srclen = str.size();
	int startlen = start.size();
	if (srclen >= startlen)
	{
		string temp = str.substr(0, startlen);
		if (temp == start)
			return true;
	}

	return false;
}

bool StringHelper::Endswith(const std::string& str, const std::string& end)
{
	int srclen = str.size();
	int endlen = end.size();
	if (srclen >= endlen)
	{
		string temp = str.substr(srclen - endlen, endlen);
		if (temp == end)
			return true;
	}

	return false;
}

string StringHelper::Trim(const std::string& str)
{
	string ret;
	//find the first position of not start with space or '\t'
	string::size_type pos_begin = str.find_first_not_of(" \t");
	if (pos_begin == string::npos)
		return str;

	//find the last position of end with space or '\t'
	string::size_type pos_end = str.find_last_not_of(" \t");
	if (pos_end == string::npos)
		return str;

	ret = str.substr(pos_begin, pos_end - pos_begin);

	return ret;
}

}
