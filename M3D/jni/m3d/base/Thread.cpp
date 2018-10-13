
#include "m3d/base/Thread.h"

#ifdef WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

namespace M3D
{

#ifdef WIN32
DWORD WINAPI ThreadFunctionStatic(void* data)
{
    Thread* thread = static_cast<Thread*>(data);

    thread->ThreadFunction();
    return 0;
}
#else
void* ThreadFunctionStatic(void* data)
{
    Thread* thread = static_cast<Thread*>(data);
    thread->ThreadFunction();
    pthread_exit((void*)0);
    return 0;
}
#endif

ThreadID Thread::mainThreadID;

Thread::Thread() :
    m_handle(0),
    m_shouldRun(false),
	m_threadID(0)
{
}

Thread::~Thread()
{
    Stop();
}

void Thread::ThreadFunction() 
{	
	m_threadID = GetCurrentThreadID();

	this->OnThreadFunction();

#ifdef WIN32
	CloseHandle((HANDLE)m_handle);
#else
	pthread_t* thread = (pthread_t*)m_handle;
	if (thread)
	delete thread;
#endif
	m_handle = 0;
	m_shouldRun = false;
}

bool Thread::Run()
{
    // Check if already running
    if (m_handle)
        return false;
    
    m_shouldRun = true;
    #ifdef WIN32
    m_handle = CreateThread(0, 0, ThreadFunctionStatic, this, 0, 0);
    #else
    m_handle = new pthread_t;
    pthread_attr_t type;
    pthread_attr_init(&type);
    pthread_attr_setdetachstate(&type, PTHREAD_CREATE_JOINABLE);
    pthread_create((pthread_t*)m_handle, &type, ThreadFunctionStatic, this);
    #endif
	
    return m_handle != 0;
}

void Thread::Stop()
{
    // Check if already stopped
    if (!m_handle)
        return;

	if(m_threadID != GetCurrentThreadID())
	{
#ifdef WIN32
		if(WaitForSingleObject((HANDLE)m_handle, INFINITE)!= WAIT_ABANDONED)
		{
			CloseHandle((HANDLE)m_handle);
		}
#else
		pthread_t* thread = (pthread_t*)m_handle;
		if (thread)
			pthread_join(*thread, 0);
		delete thread;
#endif
	} 
	m_shouldRun = false;
	m_threadID = 0;
    m_handle = 0;
}

void Thread::SetPriority(int priority)
{
    #ifdef WIN32
    if (m_handle)
        SetThreadPriority((HANDLE)m_handle, priority);
    #endif
    #if defined(__linux__) && !defined(ANDROID)
    pthread_t* thread = (pthread_t*)m_handle;
    if (thread)
        pthread_setschedprio(*thread, priority);
    #endif
}

void Thread::SetMainThread()
{
    mainThreadID = GetCurrentThreadID();
}

ThreadID Thread::GetCurrentThreadID()
{
    #ifdef WIN32
    return GetCurrentThreadId();
    #else
    return pthread_self();
    #endif
}

bool Thread::IsMainThread()
{
    return GetCurrentThreadID() == mainThreadID;
}

void Thread::Sleep(unsigned microsecond)
{
#ifdef WIN32
	::Sleep(microsecond);
#else
	usleep(microsecond * 1000);
#endif
}

}
