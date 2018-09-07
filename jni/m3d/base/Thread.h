/**
 *@file
 *@brief
 *@author	XL
 *@date		2015-2-26
 *@version	1.0
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 */

#ifndef M3D_BASE_THREAD_H_
#define M3D_BASE_THREAD_H_

#include "m3d/M3D.h"

#ifndef WIN32
#include <pthread.h>
typedef pthread_t ThreadID;
#else
typedef unsigned ThreadID;
#endif

namespace M3D
{

/** 
 * @brief Operating system thread. 
 */
class M3D_API Thread
{
public:
	/**
	 * @brief Construct. Does not start the thread yet.
	 */
	Thread();
	/**
	 * @brief Destruct. If running, stop and wait for thread to finish.
	 */
	virtual ~Thread();

	/**
	 * @brief The function to run in the thread.
	 */
	virtual void ThreadFunction();

	virtual void OnThreadFunction() = 0;

	/**
	 * @brief Start running the thread. Return true if successful, or false if already running or if can not create the thread.
	 */
	bool Run();
	/**
	 * @brief Set the running flag to false and wait for the thread to finish.
	 */
	void Stop();
	/**
	 * @brief Set thread priority. The thread must have been started first.
	 */
	void SetPriority(int priority);

	/**
	 * @brief Return whether thread exists.
	 */
	bool IsStarted() const
	{
		return m_handle != 0;
	}

	/**
	 * @brief Set the current thread as the main thread.
	 */
	static void SetMainThread();
	/**
	 * @brief Return the current thread's ID.
	 */
	static ThreadID GetCurrentThreadID();
	/**
	 * @brief Return whether is executing in the main thread.
	 */
	static bool IsMainThread();

	static void Sleep(unsigned microsecond);

protected:
	/**
	 * @brief Thread handle.
	 */
	void* m_handle;
	/**
	 * @brief Running flag.
	 */
	volatile bool m_shouldRun;

	ThreadID m_threadID;
	/**
	 * @brief Main thread's thread ID.
	 */
	static ThreadID mainThreadID;
};

}

#endif
