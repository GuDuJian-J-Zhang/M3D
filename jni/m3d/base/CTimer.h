/**@file
 *@brief	定时器封装，采用线程实现
 *@author	XL
 *@date		2014-1-7
 *@version	1.0
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 */
#ifndef M3D_CTIMER_H_
#define M3D_CTIMER_H_

#include "m3d/base/Thread.h"

#ifdef __MOBILE__
#include <sys/time.h>
#elif _WIN32

#endif

namespace M3D
{
#ifdef _WIN32
class M3D_API timeval {
public:
	long tv_sec;
	long tv_usec;
};
#endif

# define MILLISECONDS 1000

/**
 * 定时器回调函数
 * @param
 */
typedef void* (*TimerFun)(void*);
/**
 * @class
 * @brief 定时器简易实现
 *
 */
class M3D_API TimerThread: public Thread
{
public:
	/** 
	 * @brief  Construct. Does not start the thread yet.
	 */
	TimerThread();
	/** 
	 * @brief  Destruct. If running, stop and wait for thread to finish.
	 */
	virtual ~TimerThread();
	/** 
	 * @brief  The function to run in the thread.
	 */
	virtual void OnThreadFunction();
	/**
	 * @brief 设置执行间隔时间间隔   秒 + 毫秒 来设置
	 */
	void SetTimer(long second, long microsecond);
	/** 
	 * @brief 设置执行时间间隔 定时器回调函数 和 回调所需数据
	 */
	void SetTimer(TimerFun task, void* taskObject, long second,
			long microsecond);
	/**
	 * @brief 开始状态
	 */
	bool IsStart();
	/**
	 * @brief 执行tick 回调
	 */
	void OnTimer();
	/**
	 * @brief 设置持续执行标识
	 */
	void Keep(bool isKeep);
private:
	volatile bool m_keep; //!<续执行标识
	TimerFun m_timeTask;//!<定时器回调
	void* m_taskObject;//!<定时器数据对象

	long m_second;//!< 秒
	long m_microsecond;//!<毫秒
	long m_interval;//!<执行间隔 根据秒和毫秒计算
};

class M3D_API CTimer
{
public:
	/** 
	 * @brief 构造函数
	 */
	CTimer();
	/** 
	 * @brief 根据秒和毫秒进行构造
	 */
	CTimer(long second, long microsecond);
	/** 
	 * @brief 析构函数
	 */
	virtual ~CTimer();
	/** 
	 * @brief 设置定时器时间间隔
	 */
	void SetTimer(long second, long microsecond);
	/** 
	 * @brief 设置定时器时间间隔和任务回调函数 和回调数据对象
	 */
	void SetTimer(TimerFun task, void* taskObject, long second,
			long microsecond);
	/** 
	 * @brief 开始定时器
	 */
	void StartTimer();
	/** 
	 * @brief 关闭定时器
	 */
	void StopTimer();
	/** 
	 * @brief 定时器执行标识
	 */
	bool IsStart();
	/** 
	 * @brief 工具函数 获取系统当前执行毫秒数
	 */
	static unsigned Tick();
	/** 
	 * @brief TODO 工具函数 获取系统当前执行毫秒数 高精度
	 */
	static long long HiresTick();

	static int GetTimeofDay(struct timeval * tp, struct timezone * tzp);

private:
	TimerThread* m_timerThread;//!< 定时器线程函数
};
}
#endif /* M3D_CTIMER_H_ */
