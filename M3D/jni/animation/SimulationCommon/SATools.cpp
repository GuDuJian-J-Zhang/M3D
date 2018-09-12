// All Rights Reserved. Copyright (C) 2014 HOTEAMSOFT, Ltd
/****************************************************************************
*	@file		SATools.cpp
*
*	@brief	定义动画字符串处理函数等工具
*
*
*	@par	历史:
*
****************************************************************************/

#include "stdafx.h"
#include "SATools.h"

#define  D_TOL		0.00001
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//char* 转换成 wstring
std::wstring SATools::c2w( const char* pc )
{
	std::wstring val = L"";  
	if(NULL == pc)  
	{  
		return val;  
	}  
	//size_t size_of_ch = strlen(pc)*sizeof(char);  
	//size_t size_of_wc = get_wchar_size(pc);  
	size_t size_of_wc;  
	size_t destlen = mbstowcs(0,pc,0);  
	if (destlen ==(size_t)(-1))  
	{  
		return val;  
	}  
	size_of_wc = destlen+1;  
	wchar_t * pw  = new wchar_t[size_of_wc];  
	mbstowcs(pw,pc,size_of_wc);  
	val = pw;  
	delete pw;  
	return val;
}
//wchar 转换成 char*
std::string SATools::w2c( const wchar_t* pw )
{
	std::string val = "";  
	if(!pw)  
	{  
		return val;  
	}  
	size_t size= wcslen(pw)*sizeof(wchar_t);  
	char *pc = NULL;  
	if(!(pc = (char*)malloc(size)))  
	{  
		return val;  
	}  
	size_t destlen = wcstombs(pc,pw,size);  
	/*转换不为空时，返回值为-1。如果为空，返回值0*/  
	if (destlen ==(size_t)(0))  
	{  
		return val;  
	}  
	val = pc;  
	delete pc;  
	return val; 
}

int SATools::VersionCompare( const char* version1,const char* version2 )
{
	int iRet = 0;
	if(strcmp(version1,version2) == 0)
	{
		iRet = 0;
	}
	else
	{
		float fVersion1 = atof(version1);
		float fVersion2 = atof(version2);
		if((fVersion1 - fVersion2) > D_TOL)
		{
			iRet = 1;
		}
		else if((fVersion1 - fVersion2) < -D_TOL)
		{
			iRet = -1;
		}
	}
	return iRet;
}

void SATools::PlcPath16To10(const char *oldPlcPath, char *newPlcPath)
{
	char *cPlcIdP = NULL;
	char srcPath[SA_BUFFER_SIZE_SMALL];
	char targetPath[SA_BUFFER_SIZE_SMALL];
	int wiPlcId = 0;
	char *endptr;
	strcpy(newPlcPath,""); 
	strcpy(srcPath,oldPlcPath);
	cPlcIdP = strtok(srcPath,"\\");
	while(cPlcIdP !=NULL)
	{
		wiPlcId = strtoul(cPlcIdP, &endptr, 16);
		wiPlcId &= 0x00FFFFFF;
		if(strcmp(newPlcPath,"") == 0)
		{
			sprintf(targetPath,"%d",wiPlcId);
		}
		else
		{
			sprintf(targetPath,"%s\\%d",newPlcPath,wiPlcId);
		}
		strcpy(newPlcPath,targetPath);
		cPlcIdP = strtok(NULL,"\\");
	}
}

// 将strRes中的t替换为s，替换成功返回1，否则返回0。
int SATools::StrReplace(char strRes[],char from[], char to[]) {
	int i,flag = 0;
	char *p,*q,*ts;
	for(i = 0; strRes[i]; ++i) {
		if(strRes[i] == from[0]) {
			p = strRes + i;
			q = from;
			while(*q && (*p++ == *q++));
			if(*q == '\0') {
				ts = (char *)malloc(strlen(strRes) + 1);
				strcpy(ts,p);
				strRes[i] = '\0';
				strcat(strRes,to);
				strcat(strRes,ts);
				free(ts);
				flag = 1;
			}
		}
	}
	return flag;
}

void SATools::FindFileNameExtension(const char * filename_in, char * ext_out, int size)
{
	ext_out[0] = '\0';

	int strLen = strlen(filename_in);

	int i = 0;
	int iLastDotIndex = 0;
	for (i = strLen - 1; i>= 0; i-- )
	{
		if (filename_in[i] == '.')
		{
			iLastDotIndex = i;
			break;
		}
	}

	for (i = iLastDotIndex; i < strLen; i++)
		ext_out[i] = (char)tolower(filename_in[i]);

	ext_out[i] = '\0';
}

