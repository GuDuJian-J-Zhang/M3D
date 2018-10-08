/*
 * CTimer.cpp
 *
 * Created on: 2009-7-13
 *      Author: DEAN
 */
#include "CTimer.h"
#include <iostream>
#include <sys/select.h>
#include <time.h>
#include <pthread.h>
using namespace std;

CTimer::CTimer() :
		m_second(0), m_microsecond(0), m_switch_variable(false)
{
}
CTimer::CTimer(long second, long microsecond) :
		m_second(second), m_microsecond(microsecond), m_switch_variable(false)
{
}
CTimer::~CTimer()
{

}
void CTimer::SetTimer(long second, long microsecond)
{
	m_second = second;
	m_microsecond = microsecond;
}
void CTimer::SetTimer(TimerFun task, void* taskObject, long second,
		long microsecond)
{
	this->m_timeTask = task;
	this->m_taskObject = taskObject;
	m_second = second;
	m_microsecond = microsecond;
}

void CTimer::StartTimer()
{
	this->m_switch_variable = true;
	pthread_create(&m_thread_timer, NULL, OnTimer_stub, this);
}
void CTimer::StopTimer()
{
	//pthread_cancel(thread_timer);
	this->m_switch_variable = false;
	pthread_join(m_thread_timer, NULL); //wait the thread stopped
}

void CTimer::thread_proc()
{
	while (m_switch_variable)
	{
		OnTimer();
		//pthread_testcancel();
		struct timeval tempval;
		tempval.tv_sec = m_second;
		tempval.tv_usec = m_microsecond;
		select(0, NULL, NULL, NULL, &tempval);
	}
}
void CTimer::OnTimer()
{
	this->m_timeTask(this->m_taskObject);
}

void * CTimer::OnTimer_stub(void *p)
{
	(static_cast<CTimer*>(p))->thread_proc();
	return NULL;
}

bool CTimer::IsStart()
{
	return this->m_switch_variable;
}

