/*
* CTimer.cpp
*
* Created on: 2009-7-13
*      Author: DEAN
*/
#include "CTimer.h"
#include <iostream>

#ifdef WIN32
#include <windows.h>
#include <mmsystem.h>
#include <stdint.h> // portable: uint64_t   MSVC: __int64 

#elif __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#else
#include <sys/time.h>
#include <unistd.h>
#endif

using namespace std;
namespace NS_SimulationAnimation
{
	/// Construct. Does not start the thread yet.
	TimerThread::TimerThread() :Thread(),
		m_second(0), m_microsecond(0), m_keep(false)
	{
		m_interval = 0;
		this->m_timeTask = NULL;
		this->m_taskObject = NULL;
	}

	/// Destruct. If running, stop and wait for thread to finish.
	TimerThread::~TimerThread()
	{
	}

	/// The function to run in the thread.
	void TimerThread::OnThreadFunction()
	{
		unsigned long tickNow = CTimer::Tick();
		unsigned long tickLastTime = tickNow;

		while (m_keep)
		{
			tickNow = CTimer::Tick();
			if (tickNow - tickLastTime > m_interval)
			{
				OnTimer();

				tickLastTime = CTimer::Tick();
			}

			Thread::Sleep(1);
		}

		this->Stop();
	}

	bool TimerThread::IsStart()
	{
		return this->m_keep;
	}

	void TimerThread::OnTimer()
	{
		if (this->m_timeTask && this->m_taskObject)
		{
			this->m_timeTask(this->m_taskObject);
		}
	}

	void TimerThread::Keep(bool isKeep)
	{
		this->m_keep = isKeep;
	}

	void TimerThread::SetTimer(long second, long microsecond)
	{
		m_second = second;
		m_microsecond = microsecond;

		m_interval = 1000 * m_second + m_microsecond;
	}
	void TimerThread::SetTimer(TimerFun task, void* taskObject, long second,
		long microsecond)
	{
		this->m_timeTask = task;
		this->m_taskObject = taskObject;
		m_second = second;
		m_microsecond = microsecond;

		m_interval = 1000 * m_second + m_microsecond;
	}

	CTimer::CTimer()
	{
		m_timerThread = new TimerThread();
	}
	CTimer::CTimer(long second, long microsecond)

	{
		m_timerThread = new TimerThread();
	}
	CTimer::~CTimer()
	{
		if (m_timerThread)
		{
			m_timerThread->Stop();
		}
	}

	void CTimer::SetTimer(long second, long microsecond)
	{
		if (m_timerThread)
		{
			m_timerThread->SetTimer(second, microsecond);
		}
	}
	void CTimer::SetTimer(TimerFun task, void* taskObject, long second,
		long microsecond)
	{
		if (m_timerThread)
		{
			m_timerThread->SetTimer(task, taskObject, second, microsecond);
		}
	}

	void CTimer::StartTimer()
	{
		if (m_timerThread && !m_timerThread->IsStart())
		{
			m_timerThread->Keep(true);
			m_timerThread->Run();
		}
	}

	void CTimer::StopTimer()
	{
		if (m_timerThread && m_timerThread->IsStart())
		{
			m_timerThread->Keep(false);
			m_timerThread->Stop();
		}
	}

	unsigned CTimer::Tick()
	{
#ifdef WIN32
		return (unsigned)timeGetTime();
#elif __EMSCRIPTEN__
		return (unsigned)emscripten_get_now();
#else
		struct timeval time;
		gettimeofday(&time, NULL);
		return (unsigned)(time.tv_sec * 1000 + time.tv_usec / 1000);
#endif
	}

	long long CTimer::HiresTick()
	{
		return 0;
	}

	bool CTimer::IsStart()
	{
		if (m_timerThread)
		{
			return m_timerThread->IsStart();
		}
		return false;
	}

	int CTimer::GetTimeofDay(struct timeval * tp, struct timezone * tzp)
	{
#ifdef WIN32
		// Note: some broken versions only have 8 trailing zero's, the correct epoch has 9 trailing zero's
		static const uint64_t EPOCH = ((uint64_t)116444736000000000ULL);

		SYSTEMTIME  system_time;
		FILETIME    file_time;
		uint64_t    time;

		GetSystemTime(&system_time);
		SystemTimeToFileTime(&system_time, &file_time);
		time = ((uint64_t)file_time.dwLowDateTime);
		time += ((uint64_t)file_time.dwHighDateTime) << 32;

		tp->tv_sec = (long)((time - EPOCH) / 10000000L);
		tp->tv_usec = (long)(system_time.wMilliseconds * 1000);
		return 0;
#elif __MOBILE__
		return gettimeofday(tp, tzp);
#endif
	}
}


