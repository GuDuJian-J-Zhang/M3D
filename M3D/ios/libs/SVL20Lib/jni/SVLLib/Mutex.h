/**
 *@file
 *@brief
 *@author	XL
 *@date		2015-2-26
 *@version	1.0
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 */
#ifndef _SVLLIB_MUTEX_H_
#define _SVLLIB_MUTEX_H_


namespace HoteamSoft {
	namespace SVLLib {
/** 
 * @brief Operating system mutual exclusion primitive. 
 */
class Mutex
{
public:
	/**
	 * @brief Construct.
	 */
	Mutex();
	/** 
	 * @brief Destruct.
	 */
	~Mutex();
	Mutex(const Mutex& orig);

	Mutex& operator =(const Mutex& orig);
	/** 
	 * @brief Acquire the mutex. Block if already acquired.
	 */
	void Acquire();
	/** 
	 * @brief Release the mutex.
	 */
	void Release();

private:
	/** 
	 * @brief Mutex handle.
	 */
	void* m_handle;
};

/** 
 * @brief Lock that automatically acquires and releases a mutex. 
 */
class MutexLock
{
public:
	/** 
	 * @brief Construct and acquire the mutex.
	 */
	MutexLock(Mutex& mutex);
	/** 
	 * @brief Destruct. Release the mutex.
	 */
	~MutexLock();

private:
	/** 
	 * @brief Prevent copy construction.
	 */
	MutexLock(const MutexLock& rhs);
	/** 
	 * @brief Prevent assignment.
	 */
	MutexLock& operator =(const MutexLock& rhs);

	/** 
	 * @brief Mutex reference.
	 */
	Mutex& m_mutex;
};

}
}
#endif //_SVLLIB_MUTEX_H
