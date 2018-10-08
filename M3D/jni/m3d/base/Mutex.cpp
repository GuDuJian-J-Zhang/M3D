#include "m3d/base/Mutex.h"

#ifdef WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

namespace M3D
{
#ifdef WIN32
Mutex::Mutex() :
m_handle(new CRITICAL_SECTION)
{
	InitializeCriticalSection((CRITICAL_SECTION*)m_handle);
}

Mutex::~Mutex()
{
	CRITICAL_SECTION* cs = (CRITICAL_SECTION*)m_handle;
	DeleteCriticalSection(cs);
	delete cs;
	m_handle = 0;
}

void Mutex::Acquire()
{
	   EnterCriticalSection((CRITICAL_SECTION*)m_handle);
}

void Mutex::Release()
{
	LeaveCriticalSection((CRITICAL_SECTION*)m_handle);
}
#else
Mutex::Mutex() :
		m_handle(new pthread_mutex_t)
{
	pthread_mutex_t* mutex = (pthread_mutex_t*) m_handle;
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(mutex, &attr);
}

Mutex::~Mutex()
{
	pthread_mutex_t* mutex = (pthread_mutex_t*) m_handle;
	pthread_mutex_destroy(mutex);
	delete mutex;
	m_handle = 0;
}

Mutex::Mutex(const Mutex& orig)
{
	//锁对象不支持拷贝
}

Mutex& Mutex::operator =(const Mutex& orig)
{
	if (this != &orig)
	{

	}
	return *this;
}


void Mutex::Acquire()
{
	pthread_mutex_lock((pthread_mutex_t*) m_handle);
}

void Mutex::Release()
{
	pthread_mutex_unlock((pthread_mutex_t*) m_handle);
}
#endif

MutexLock::MutexLock(Mutex& mutex) :
		m_mutex(mutex)
{
	m_mutex.Acquire();
}

MutexLock::~MutexLock()
{
	m_mutex.Release();
}

}
