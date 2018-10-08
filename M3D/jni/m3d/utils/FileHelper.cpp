#include "m3d/M3D.h"
#include "m3d/utils/FileHelper.h"
#include <stdio.h>
#include <stdlib.h>


namespace M3D{
string FileHelper::GetUnionStylePath(const string filePath)
{
	string linuxstylePath = filePath;
	FileHelper::Replace_all(linuxstylePath,"\\","/");
	return linuxstylePath;
}

void FileHelper::Replace_all(string& src,const string& oldvalue, const string& newvalue)
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

string FileHelper::GetDirName(const string& filePath)
{
	string tFilePath = filePath;
	const char* path = tFilePath.c_str();
	char* dir;
	char* s;
#ifdef WIN32
	dir =  _strdup(path);
#endif

#ifdef __MOBILE__
	dir =  strdup(path);
#endif
	s = strrchr(dir, '/');
	//s = strrchr(dir, '\\');
	if (s)
		s[1] = '\0';
	else
		dir[0] = '\0';

	return string(dir);
}

string FileHelper::GetFileName(const string& filePath)
{
	int pos = filePath.find_last_not_of("/");

	string name = "";

	if(pos > 0 && pos < filePath.size()-1)
	{
		name = filePath.substr(pos+1,filePath.size());
	}

	return name;
}

string FileHelper::GetExt(const string& filePath)
{
	string ext;
	int dotIndex = filePath.find_last_of('.');
	if (dotIndex != -1)
	{
		ext = filePath.substr(dotIndex + 1);
	}
	return ext;
}

string FileHelper::GetEnCode(const string& filePath)
{
	string code = "ASCII";

	return code;
}

bool FileHelper::FileExist(const string& filePath)
{
	bool exist = false;
#ifdef WIN32
	if(_access(filePath.c_str(),0) != -1)
		{
			exist = true;
		}else{
			 LOGE("File not exist %d, access-- %d",errno,_access(filePath.c_str(),0));
		}
#endif

#ifdef __MOBILE__
	if(access(filePath.c_str(),0) != -1)
		{
			exist = true;
		}else{
			 LOGE("File not exist %d, access-- %d",errno,access(filePath.c_str(),F_OK));
		}
#endif


	return exist;
}
}
