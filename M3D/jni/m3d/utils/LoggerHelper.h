/**@file
 *@brief	日志系统
 *@author	XL
 *@date
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */
#ifndef M3D_LOGGER_HELPER_H
#define M3D_LOGGER_HELPER_H
#include "m3d/M3D.h"

namespace M3D
{
class LoggerHelper;

/**
 * @class TODO
 * @brief 日志辅助类。
 */
class M3D_API LoggerHelper
{
public:
	static const int TYPE_READER;
	static const int TYPE_MODEL;

	static const int Level_INFO;
	static const int Level_ERROR;

	static LoggerHelper* Instance();

public:
	LoggerHelper();
	virtual ~LoggerHelper();

	void Begin(int type);

	void Append(int type,int level,const string& info);

	void End(int type);

	string* Get(int type);
	string* GetLevel(int level);

private:
	static LoggerHelper* pLoggerHelper;

	static string defaultLog;

	static string readerLog;
	static string modelLog;

	static string BEGIN;
	static string END;

	static string INFO;
	static string LERROR;
};
}

#endif  /*M3D_LOGGER_HELPER_H*/
