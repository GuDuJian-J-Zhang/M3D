// All Rights Reserved. Copyright (C) 2016 Hoteamsoft. 
/**************************************************************************
 *	@file		Stk_Log.cpp
 *  @brief		»’÷æ¿‡
 *	@par ¿˙ ∑º«¬º
 *				2016/11/8 ¥¥Ω®°£MengYaohui(Hoteamsoft)
**************************************************************************/
#include "Stk_Log.h"
#include <ctime>
#include <iostream>
#ifdef _WIN32
#include <direct.h>
#else
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#endif
namespace HoteamSoft {
	namespace SVLLib {

        
        STK_IMPLEMENT_CLASS(Stk_Log, Stk_Object)

 Stk_Log::Stk_Log(void):m_level(STK_LOG_LEVEL_FATAL),
 m_type(STK_LOG_TO_CONSOLE),
 m_pFile(NULL)
 {
	 Initialize();
 }
 
 Stk_Log::~Stk_Log(void)
 {
 	if (NULL == m_pFile)
 		return;
 	fclose(m_pFile);
 }
/**********************************************************************//**
 *	@brief		≥ı ºªØ
 *	@param[in]	Œﬁ
 *	@param[out]	Œﬁ
 *	@retval		Œﬁ
 *	@note		Œﬁ
 *	@attention	Œﬁ
**************************************************************************/
void Stk_Log::Initialize()
{
	m_strPath = "stkLog.log";
	m_pFile   = fopen(m_strPath.c_str(), "w");
}
/**********************************************************************//**
 *	@brief		…Ë÷√¥Ú”°»’÷æµƒ¬∑æ∂£¨ƒ¨»œ¬∑æ∂ «µ±«∞ƒø¬ºµƒ"stkLog.log"
 *	@param[in]	std::string strPath		»’÷æ¬∑æ∂
 *	@param[out]	Œﬁ
 *	@retval		Œﬁ
 *	@note		Œﬁ
 *	@attention	Œﬁ
**************************************************************************/
void Stk_Log::SetLogPath(Stk_string strPath)
{
	m_strPath = strPath;
	Stk_string strFile = strPath;
	Stk_string::size_type pos = strFile.rfind("\\");
	if (Stk_string::npos != pos)
	{
		strFile = strFile.substr(0, pos);
#ifdef __MOBILE__
		mkdir(strFile.c_str(),777);
#else
		mkdir(strFile.c_str());
#endif
	}
	if (NULL != m_pFile)
	{
		fclose(m_pFile);
	}
	m_pFile = fopen(m_strPath.c_str(), "w");
}
/**********************************************************************//**
 *	@brief		
 *	@param[in]	
 *	@param[out]	
 *	@retval		
 *	@note		Œﬁ
 *	@attention	Œﬁ
**************************************************************************/
Stk_string Stk_Log::GetAddtionInfo(int type)
{
	Stk_string strRet;
	if (!type)
	{
		return strRet;
	}

	//!<  ±º‰–≈œ¢
	time_t timer;
	time(&timer);
	struct tm *local = localtime(&timer);
	char cTimer[64] = {0};
	strftime(cTimer, 64,"[%Y-%m-%d %H:%M:%S]", local);
	Stk_string strTime(cTimer);

	switch(type)
	{
	case 1:
		strRet += strTime;
		break;
	default:
		break;
	}

	return strRet;
}
/**********************************************************************//**
 *	@brief		¥Ú”°»’÷æ
 *	@param[in]	std::string strMsg		»’÷æ–≈œ¢
 *	@param[in]	Stk_Log_Level level			»’÷æº∂±
 *	@param[in]	type	ªÒ»°∏Ωº”–≈œ¢µƒº∂±£¨”√Stk_Log_Addition_info√∂æŸ
 *						Ω¯––"|"◊È∫œ
 *	@param[out]	Œﬁ
 *	@retval		≥…π¶:STK_SUCCESS		 ß∞‹:STK_ERROR
 *	@note		Œﬁ
 *	@attention	Œﬁ
**************************************************************************/
STK_STATUS Stk_Log::PrintLog(Stk_string strMsg, Stk_Log_Level level /* = STK_LOG_LEVEL_HIGHEST */, int type /* = 0 */)
{
	if ( strMsg.empty() || m_level == STK_LOG_LEVEL_NONE || m_level > level)
	{
		return STK_ERROR;
	}

	Stk_string strLevel;
	switch(level)
	{
	case STK_LOG_LEVEL_TRACE:
		strLevel = "[Trace]:";
		break;
	case STK_LOG_LEVEL_DEBUG:
		strLevel = "[Debug]:";
		break;
	case STK_LOG_LEVEL_INFON:
		strLevel = "[Info]:";
		break;
	case STK_LOG_LEVEL_NOTICE:
		strLevel = "[Notice]:";
		break;
	case STK_LOG_LEVEL_WARNING:
		strLevel = "[Warning]:";
		break;
	case STK_LOG_LEVEL_ERROR:
		strLevel = "[Error]:";
		break;
	case STK_LOG_LEVEL_CRITICAL:
		strLevel = "[Critical]:";
		break;
	case STK_LOG_LEVEL_FATAL:
		strLevel = "[Fatal]:";
		break;
	case STK_LOG_LEVEL_HIGHEST:
		break;
	default:break;
	}

	Stk_string strInfo = GetAddtionInfo(type);
	Stk_string strFullMsg = strInfo + strLevel + strMsg;

	//!< –¥»’÷æ
	if (STK_LOG_TO_CONSOLE == m_type)
	{
		std::cout<<strFullMsg<<std::endl;
	} 
	else if (STK_LOG_TO_FILE == m_type && NULL != m_pFile)
	{
		fprintf(m_pFile, "%s\n", strFullMsg.c_str() );
		fflush(m_pFile);
	}
	else 
		return STK_ERROR;

	return STK_SUCCESS;
}

	}//svllib
}//hoteamsoft
