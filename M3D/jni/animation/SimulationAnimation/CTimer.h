/**@file
 *@brief	Linux下定时器
 *@author	XL
 *@date		2014-1-7
 *@version	1.0
 *
 */
#ifndef M3D_CTIMER_H_
#define M3D_CTIMER_H_
#include <pthread.h>
#include <sys/time.h>

typedef void* (*TimerFun)(void*);
/**@class
 * @brief linux下定时器简易实现
 *
 */
class CTimer
{
public:
	CTimer();
	CTimer(long second, long microsecond);
	virtual ~CTimer();
	void SetTimer(long second, long microsecond);
	void SetTimer(TimerFun task, void* taskObject, long second,
			long microsecond);
	void StartTimer();
	void StopTimer();
	bool IsStart();
	virtual void OnTimer();

private:
	static void *OnTimer_stub(void *p);
	void thread_proc();

private:
	volatile bool m_switch_variable;
	TimerFun m_timeTask;
	void* m_taskObject;
	pthread_t m_thread_timer;
	long m_second;
	long m_microsecond;

};
#endif /* CTIMER_H_ */
