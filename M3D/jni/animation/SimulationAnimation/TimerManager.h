// All Rights Reserved.Copyright(C) 2014 HOTEAMSOFT, Ltd
/****************************************************************************
*	@file		TickManager.h
*
*	@brief	动画时钟信号处理定义文件
*
*	@par	历史:
*
****************************************************************************/


#ifndef _TIMERMANAGER_H
#define _TIMERMANAGER_H
#ifdef WIN32
//#include <windows.h>
//#include <mmsystem.h>
//#include <stdint.h> // portable: uint64_t   MSVC: __int64 
#else
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#endif
#ifdef USE_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#define	LOCK_MUTEX(_m_)	EnterCriticalSection ((CRITICAL_SECTION *)_m_)
#define	UNLOCK_MUTEX(_m_) LeaveCriticalSection ((CRITICAL_SECTION *)_m_)



#include "../SimulationCommon/SATools.h"
#include "SADefine.h"


//SA_NAMESPACE_BEGIN
class CTimerClient;
class CTickTimer;
namespace NS_SimulationAnimation {
	class Mutex;
}
/****************************************************************************
*	动画时钟信息处理类
****************************************************************************/
class SA_API CTimerManager
{
public:
	/*! Constructs an CTimerManager object. 
	\param output_hz The number of buckets into which the timer manager splits a single second. 
	*/
    CTimerManager( int output_hz = 100 );
    virtual ~CTimerManager();

    /*! The mechanism for receiving timer events from the OS is platform dependent, so here we only provide 
		a function call for the external "timer driver" to deliver timer events.  It is recommended that 
		such events be delivered at the highest available frequency. */
    virtual void Tick( float time );
    /*! This method informs the timer manager of a client that would like to start receiving timer events.
	\param c A pointer to the CTimerClient that would like to receiver timer events.
	*/
    void RegisterClient( CTimerClient *c );
    /*!This method stops the timer service for the given client.
	\param c A pointer the CTimerClient that no longer wants to receive timer events.
	*/
    void UnRegisterClient( CTimerClient *c );
    /*! \return A pointer to the timer manager. */
    static CTimerManager *GetCurrentTimerManager();

protected:
    double m_interval;               //!< 1.0 / m_output_hz
    double m_request_time;           //!< The time that the current bucket's events had requested.
    double m_actual_time;            //!< The actual time which may be different from m_request_time if control is lost for a while.
    int m_output_hz;                //!< The granularity of the timer, i.e. the number of buckets into which the timer manager splits a single second.  Timer events scheduled in the same bucket may be delivered sorted only by priority.
    struct vlist_s **m_buckets;     //!< The array of buckets of size m_output_hz. This array holds precisely 1 sec of events.
    int m_current_bucket;           //!< The bucket of events currently being dispatched.
    struct vlist_s *m_spillover;    //!< An array of things that need to be scheduled for more than 1s away.
    struct vhash_s *m_active_clients; //!< A hash table of all the currently registered clients.  Only currently registered clients can receive ticks, or even accessed in any way to make sure we don't access any memory after free. */
    struct vlist_s *m_recently_deleted_expirations; //!< A vlist parallel to m_recently_deleted_clients to indicate how long things need to stay on the list.
    struct vlist_s *m_recently_deleted_clients; //!< A list of recently deleted clients.  Only if an item is unregistered and then re-registered very quickly. 

    /*! This method must be called before any dispatching can be done.  Note it is automatically performed on the first call to 
		RegisterClient.  It allocates all required memory for member data. 
		\param start_time The time. 
		*/
    void Init( float start_time );
    /*! This method finds the appropriate list to which the given client should be added so that an event is scheduled at 
	the right time. 
	\param c The client that is requesting a timer event.  
	\param time Pass the ideal time at which this timer event should be delivered.
	*/
    void ScheduleNextTick( CTimerClient *c, float time );

	//add
private:
#ifdef WIN32
	static NS_SimulationAnimation::Mutex *m_Mutex;
#else
	static pthread_mutex_t * m_Mutex;
#endif
	static CTimerManager * m_pCTimerManager;
public:
	/*! \returns A pointer to the current CTimerManager object (the timer manager).  This method creates one if none exists. */
	static CTimerManager * GetTimerManager();

	/*! This method replaces the timer manager.
	\param manager  A pointer to an CTimerManager object.
	\return A pointer to the replaced CTimerManager.  
	*/
	static CTimerManager * SetTimerManager(CTimerManager * manager);
	void Lock();
	void UnLock();
	
};

/*! \enum TimerClientType
	Indicates how timer events will be delivered
*/
enum TimerClientType {
    TimerClientType_Invalid,       //!< An invalid default state for the client type.  The client type must be set before anything useful will happen.
    TimerClientType_Once,          //!< A single event will be delivered at the designated time, after which the timer client will automatically be unregistered.
    TimerClientType_Periodic,      //!< Events will be delivered at the interval specified with the timer client's "mt_interval" member variable.  If for any reason the timer manager falls behind, a series of "make-up" events will be delivered, with identical values for "actual_time", but different "request_time"'s
    TimerClientType_PeriodicSkip   //!< Similar to TimerClientType_Periodic but this option has no make-up events.
};

/*! var typedef bool(*CTimerClientTickFunction)(float request_time, float actual_time, void * user_data) */
typedef bool(*CTimerClientTickFunction)(float request_time, float actual_time, void * user_data);

/****************************************************************************
*	动画时钟信息处理类
****************************************************************************/
class SA_API CTimerClient
{
public:
	/*! Constructs an CTimerClient object.
		\param interval Pass the requested time between timer events.
		\param type This parameter defaults to TimerClientType_Invalid but before the client can be registered, it must be set
		to one of the #TimerClientType enumerations.  
		\param tick_function The callback function pointer which is generally a static function associated
				with the class.
		\param user_data A pointer passed back to the tick_function callback.  Generally set to "this"
				so that the tick callback can find its way to the class. */
    CTimerClient(   float interval = 0.1f, 
				TimerClientType type = TimerClientType_Invalid, 
				CTimerClientTickFunction tick_function = 0, 
				void* user_data = 0 ) {
        mt_interval = interval;
        mt_type = type;
        mt_next_request = 0;
        mt_priority = 0;
        mt_tick_function = tick_function;
        mt_user_data = user_data;
    }
    virtual ~CTimerClient();

    /*! Timer events are delivered here first.  If this function is not overridden, the events will be delivered
		to whatever was set with the #mt_tick_function argument of the constructor or later on with SetTickFunction().
		\param request_time The time for which the timer event was originally scheduled.
		\param actual_time The time at which the tick was actually delivered which is greater than or equal to <i>request_time</i>.
		\return True if no errors were encountered. */
	virtual bool Tick( float request_time, float actual_time ){ 
        if( mt_tick_function ) 
            return mt_tick_function(request_time, actual_time, mt_user_data);
        else
            return true;
    };

    /*! This method sets the interval in which the CTimerClient would like to receive timer events.
	\param interval Pass a float that defines that period of time in between receiving timer events.*/
    void SetInterval( float interval ) { mt_interval = interval; };
    
	/*! \return The timer interval in which the CTimerClient would like to receive timer events. */
    float GetInterval() const { return mt_interval; };
    
	/*!This method sets how the client would like to receive tick calls.
	\param type Pass one the defined types: #TimerClientType_Invalid, #TimerClientType_Once, #TimerClientType_Periodic or #TimerClientType_PeriodicSkip. Note 
	that you must select a type other than #TimerClientType_Invalid to receive timer events.
	*/
    void SetTimerType( TimerClientType type ) { mt_type = type; };
    
	/*! \return The timer type which describes how the client would like to receive timer events.*/
    TimerClientType GetTimerType() const { return mt_type; };
    
	/*!  This method sets the next timer request.  Generally this is only called from the timer manager after a timer 
	event has been delivered to a Periodic-type client.
	\param nr The time at which we would like to receive the next timer event.  */
    void SetNextRequest( float nr ) { mt_next_request = nr; };
    
	/*! \return The time of the next scheduled event. */
    float GetNextRequest() const { return mt_next_request; };
    
	/*! \return The priority level for this client to receive timer event. Within a single time slice, the CTimerManager 
	delivers timer events according to priority values of the given CTimerClients.  Note that clients with higher priorities 
	will receive timer events before those with lower priority levels.*/
    int  GetPriority() const { return mt_priority; };
    
	/*! This method sets the priority to the lowest supported value.  This client will subsequently be placed at the list 
		tail of any bucket it is scheduled into, as opposed to the head, as usual.  It has the effect of postponing 
		timer events very slightly. */
    void SetPriorityLowest() { mt_priority = -1; };
    
	/*! Use this method to pass data via the timer callback.  When the timer manager calls the #Tick() function, 
	it will pass the user data.
	\param user_data User defined data that may be specifically used for this client's Tick function. */
    void SetUserData( void* user_data ) { mt_user_data = user_data; };
    
	/*! \return The user data that is specific to this client. */
    void* GetUserData() const { return mt_user_data; };
    
	/*! Use this method to set the callback function that will handle the timer events.
	\param tick_function A pointer to the function that will handle this client's timer events. 
	*/
    void SetTickFunction( CTimerClientTickFunction tick_function) { mt_tick_function = tick_function; };
    
	/*! \return A pointer to the Tick function callback. */
    CTimerClientTickFunction GetTickFunction() const { return mt_tick_function; };

protected:
	/*! The client type which describes how this object would like to receive timer events.*/
    TimerClientType mt_type;                      

	/*! The interval, in seconds, at which tick events occur.  The first event will occur mt_interval 
	seconds after the call to CTimerManager::RegisterClient().  For #TimerClientType_Once clients, mt_interval specifies 
	the delay until the one and only timer event is received.*/
    float mt_interval;        

	/*! The time at which the next event is scheduled.*/
    float mt_next_request;                  

	/*! The priority determines the order in which to sort timer events within a single CTimerManager bucket.  
	In other words, it is a tie breaker for two timer events that are supposed to occur at the same time.
	*/
    int mt_priority;                        

	/*! A pointer passed back to the Tick function callback, if one is set.*/
    void* mt_user_data;                     
    
	/*! A function to call whenever timer events are received.  
	This is called automatically if and only if Tick() is not overridden.*/
	CTimerClientTickFunction mt_tick_function;  
};

//SA_NAMESPACE_END

#ifdef USE_PACK_8
#pragma pack(pop)
#endif

#endif



