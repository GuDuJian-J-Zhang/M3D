#include "m3d/utils/LoggerHelper.h"

namespace M3D
{

const int LoggerHelper::TYPE_READER = 0;
const int LoggerHelper::TYPE_MODEL = 1;

const int LoggerHelper::Level_INFO = 0 ;
const int LoggerHelper::Level_ERROR =1 ;

string LoggerHelper::defaultLog = "";
string LoggerHelper::readerLog = "" ;
string LoggerHelper::modelLog = "" ;

string LoggerHelper::BEGIN = "--BEGIN:";
string LoggerHelper::END = "--END:";
string LoggerHelper::INFO = "--INFO:";
string LoggerHelper::LERROR = "--ERROR:";

LoggerHelper* LoggerHelper::pLoggerHelper = new LoggerHelper;

LoggerHelper* LoggerHelper::Instance()
{
	if(LoggerHelper::pLoggerHelper == NULL)
	{
		LoggerHelper::pLoggerHelper = new LoggerHelper;
	}
	return LoggerHelper::pLoggerHelper;
}

LoggerHelper::LoggerHelper()
{

}

LoggerHelper::~LoggerHelper()
{

}

void LoggerHelper::Begin(int type)
{
	string* logInfo = Get(type);
	*logInfo = LoggerHelper::BEGIN;
}

void LoggerHelper::Append(int type,int level,const string& info)
{
	string* logInfo = Get(type);
	string* logLevel = GetLevel(level);
	*logInfo += *logLevel;
	*logInfo += info;
}

void LoggerHelper::End(int type)
{
	string* logInfo = Get(type);
	*logInfo += LoggerHelper::END;
}

string* LoggerHelper::Get(int type)
{
	string* logInfo = &LoggerHelper::defaultLog;
	switch(type)
	{
	case TYPE_READER:
		logInfo = &LoggerHelper::readerLog;
		break;
	case TYPE_MODEL:
		logInfo = &LoggerHelper::modelLog;
		break;
	default:
		break;
	}
	return logInfo;
}

string* LoggerHelper::GetLevel(int level)
{
	string* logInfo = &LoggerHelper::INFO;
	switch(level)
	{
	case Level_INFO:
		logInfo = &LoggerHelper::INFO;
		break;
	case Level_ERROR:
		logInfo = &LoggerHelper::LERROR;
		break;
	default:
		break;
	}
	return logInfo;
}
}
