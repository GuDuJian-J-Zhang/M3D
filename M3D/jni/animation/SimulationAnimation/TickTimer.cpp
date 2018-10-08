// All Rights Reserved.Copyright(C) 2014 HOTEAMSOFT, Ltd
/****************************************************************************
*	@file		TickTimer.cpp
*
*	@brief	动画播放定时器，实现播放服务
*
*	@par	历史:
*
****************************************************************************/
#include "Utility.h"
#include "stdafx.h"
#include "TickTimer.h"
#include "TimerManager.h"

#include "IOSExternDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//SA_NAMESPACE_BEGIN

int CTickTimer::m_milliseconds = 50000;
CTimer CTickTimer::m_timer;
bool CTickTimer::m_isFirstGetTime = true;

CTickTimer::CTickTimer( int milliseconds )
{
	m_milliseconds = milliseconds;
	//Init();
	m_isFirstGetTime = true;

}

CTickTimer::~CTickTimer()
{
	KillTimer();
	CTimerManager * pTimerManager = CTimerManager::GetTimerManager();
	if (NULL != pTimerManager)
		delete pTimerManager;
}

int CTickTimer::Init()
{
	// This is my invented formula to receive windows messages (esp. the timer messages)
	// in a non-window class (this one) I just register some class and create a window of 0 dimensions
	// This works cool - Rajesh B
	m_timer.SetTimer(timerTask, NULL, 0, m_milliseconds);
#ifndef __ANDROID__
	StartTimer();
#endif
	return 1;
}

void CTickTimer::StartTimer( )
{
	m_timer.StartTimer();

//	struct sigevent sev;
//	struct itimespec its;
//	long long freq_nanosecs;
//	struct sigaction sa;
//	sigset_t mask;
//
//	int nTimer = 1;
//	do {
//		sev.sigev_notify = SIGEV_THREAD;
//		sev.sigev_signo = SIGRTMIN;
//		sev.sigev_value.sival_ptr = &m_timer;
//		sev.sigev_notify_function = timerTimeout;
//		sev.sigev_notify_attributes = NULL;
//
//		/* create timer */
//		timer_create (CLOCK_REALTIME, &sev, &m_timer);
//		nTimer++;
//	} while( m_timer <= 0 && nTimer < 50 );
//
//	/* Start the timer */
//	its.it_value.tv_sec = 0 ;
//	its.it_value.tv_nsec = m_milliseconds*1000000;
//	//DEBUG_LOG ("set timer, tv_sec:%ld, tv_nsec:%ld",
//	//       its.it_value.tv_sec, its.it_value.tv_nsec);
//
//	its.it_interval.tv_sec = 0;
//	its.it_interval.tv_nsec = m_milliseconds*1000000;
//	 /*
//	  * it_value����ָ����ǰ�Ķ�ʱ����ʱ�䡣����ʱ���ڣ�it_value��ֵ��
//	  * �����³�it_interval ��ֵ�����it_interval��ֵΪ0����ʱ����һ��ʱ
//	  * ����ʱ��һ��it_value���ھͻ�ص�δ��״̬��
//	  */
//
//	if (timer_settime (m_timer, 0, &its, NULL) == -1)
//	{
//	    //DEBUG_ERR ("timer_settime error");
//	    return;
//	}

	return;
}

void CTickTimer::KillTimer()
{
//	// kill the timer if it was set
	if (m_timer.IsStart())
	{
		m_timer.StopTimer();
	}
}

void CTickTimer::OnTimerTick()
{
	timerTask(NULL);
}

void CTickTimer::timerTimeout(union sigval sig)
{
	/*struct itimerspec its;
	 its.it_value.tv_sec = m_milliseconds ;
	 its.it_value.tv_nsec = 0;

	 its.it_interval.tv_sec = m_milliseconds;
	 its.it_interval.tv_nsec = 0;

	 if (timer_settime (m_timer, 0, &its, NULL) == -1)
	 {
	 return;
	 }*/
	//StartTimer();
}

void* CTickTimer::timerTask(void* arg)
{
	float time;
	APP_Show_Time(&time);

    CTimerManager::GetTimerManager()->Tick( time );
	return NULL;
}

struct timeval gFirstGetTime;

void APP_Show_Time(float* time)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	if (CTickTimer::m_isFirstGetTime)
	{
		CTickTimer::m_isFirstGetTime = false;
		gFirstGetTime = tv;
	}

	*time = (tv.tv_sec - gFirstGetTime.tv_sec)
			+ ((double) (tv.tv_usec - gFirstGetTime.tv_usec)) / CLOCKS_PER_SEC;

	if (*time < 0)
	{
		LOGE(" time error %f", *time);
	}
}
//SA_NAMESPACE_END
