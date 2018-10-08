#ifndef __CTDRIVER_H__
#define __CTDRIVER_H__
#include "TimerManager.h"
#include "CTimer.h"

/*! 
    This class delivers timer ticks which are intended to be used by a platform/GUI independent timer service.
*/

//SA_NAMESPACE_BEGIN
class SA_API CTickTimer
{
// Construction
public:
	//! custom constructor sets 10ms tick
	CTickTimer( int milliseconds = 10 );  
	virtual ~CTickTimer();

	static void StartTimer();  //!< starts timer
    static void KillTimer();   //!< kills timer

    static int Init();  //!< sets up window

    static bool m_isFirstGetTime;

    void OnTimerTick();

protected:

    static CTimer m_timer;
	static int     m_milliseconds;  //!< amount of time to tick

    static void timerTimeout(union sigval sig);
    
    static void* timerTask(void* arg);

};


//SA_NAMESPACE_END

#endif
