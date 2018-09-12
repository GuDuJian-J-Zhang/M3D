// All Rights Reserved.Copyright(C) 2014 HOTEAMSOFT, Ltd
/****************************************************************************
*	@file		TickManager.cpp
*
*	@brief	动画时钟信号处理定义文件
*
*	@par	历史:
*
****************************************************************************/

#include "stdafx.h"
#include <stdio.h>
#include "TimerManager.h"
#include "../SimulationCommon/vlist.h"
#include "../SimulationCommon/vhash.h"
#include "Mutex.h"


//SA_NAMESPACE_BEGIN
#ifdef WIN32
NS_SimulationAnimation::Mutex* CTimerManager::m_Mutex = 0;
#else 
pthread_mutex_t* CTimerManager::m_Mutex = 0;
#endif
CTimerManager* CTimerManager::m_pCTimerManager = 0;
 
CTimerManager::CTimerManager ( int output_hz )
{
    m_output_hz = output_hz;
    m_interval = 1.0f / output_hz;
    m_buckets = 0;
    m_spillover = 0;
    m_active_clients = 0;
    m_current_bucket = 0;
    m_request_time = m_actual_time = 0;
    m_recently_deleted_clients = 0;
    m_recently_deleted_expirations = 0;
#ifdef WIN32
	m_Mutex = new NS_SimulationAnimation::Mutex();	
#else
	m_Mutex = new pthread_mutex_t();
	pthread_mutexattr_t attr;
	pthread_mutex_init(m_Mutex, &attr);
#endif
}


CTimerManager::~CTimerManager ()
{
    int i;

    if( m_active_clients != 0 ) {
        delete_vhash( m_active_clients );
    }
    if( m_recently_deleted_clients != 0 ) {
        delete_vlist( m_recently_deleted_clients );
    }
    if( m_recently_deleted_expirations != 0 ) {
        delete_vlist( m_recently_deleted_expirations );
    }
    if( m_buckets != 0 ) {
        for( i = 0 ; i < m_output_hz ; i++ ) {
            delete_vlist( m_buckets[i] );
        }
        delete [] m_buckets;
        m_buckets = 0;
    }
    if( m_spillover != 0 )
        delete_vlist( m_spillover );
	// Release resources used by the critical section object.
#ifdef WIN32	
	delete m_Mutex;
#else
    pthread_mutex_destroy(m_Mutex);
	delete m_Mutex;
#endif
	m_pCTimerManager = NULL;
}


void CTimerManager::Init( float start_time )
{
    int i;

    if( m_buckets == 0 ) {
        m_buckets = new struct vlist_s *[m_output_hz];
        for( i = 0 ; i < m_output_hz ; i++ ) {
            m_buckets[i] = new_vlist( malloc, free );
        }
    }
    if( m_spillover == 0 )
        m_spillover = new_vlist( malloc, free );
	/*if( m_active_clients == 0 )
        m_active_clients = new_vhash( 100, malloc, free );*/
	
	if( m_active_clients == 0 )
        m_active_clients = new_vhash( 100, malloc, free );
    if( m_recently_deleted_clients == 0 )
        m_recently_deleted_clients = new_vlist( malloc, free );
    if( m_recently_deleted_expirations == 0 )
        m_recently_deleted_expirations = new_vlist( malloc, free );

    m_request_time = m_actual_time = start_time;
}

//static int timerTime =0;
void CTimerManager::Tick( float time )
{
    
    CTimerClient *client;
    float t, dt;
    bool keep_going = true;
	int expiration;

    if( m_buckets == 0 )
        Init( time );
    m_actual_time = time;
    while( m_request_time + m_interval < m_actual_time ) {
        while( (client = (CTimerClient *)vlist_remove_first( m_buckets[m_current_bucket] )) != 0 ) {
			if( vhash_lookup_item( m_active_clients, client, 0 ) ) {
				dt = client->GetInterval();
				switch( client->GetTimerType() ) {
					case TimerClientType_Periodic: {
						//synthesize ticks to make up for the ones we missed
						for( t = client->GetNextRequest(); t < m_actual_time && keep_going ; t += dt ) {
							if( !vhash_lookup_item( m_active_clients, client, 0 ) )
								break;
							keep_going = client->Tick( t, (float)m_actual_time );
						}
						if( keep_going )
							ScheduleNextTick( client, t );
                        
					} break;
					case TimerClientType_PeriodicSkip: {
						//schedule next event, but no make up calls
						keep_going = client->Tick( (float)m_request_time, (float)m_actual_time );
						if( keep_going )
							ScheduleNextTick( client, (float)m_actual_time + dt );
					} break;
					case TimerClientType_Once: {
						//do nothing except call the one tick.
						client->Tick( (float)m_request_time, (float)m_actual_time );
					} break;
					default:
						//if you get here, the most likely causes are either that m_type was never set
						//(defaults to TimerClientType_Invalid at construction), or the client was deleted without
						//first unregistering itself.
						assert( 0 );
				}
			}
        }
        m_current_bucket++;
        m_request_time += m_interval;
        if( m_current_bucket == m_output_hz ) {
            //process spillover
            while((client = (CTimerClient *)vlist_peek_first( m_spillover )) != 0 ) {
                t = client->GetNextRequest();
                if( t - m_request_time >= 1 )
                    break;
                vlist_remove_first( m_spillover );
                ScheduleNextTick( client, t );
            } 
            m_current_bucket = 0;
            //take stuff off the front of the recent deletions list
            for(;;) {
				expiration = SA_V2I(vlist_peek_first( m_recently_deleted_expirations ));
				if( expiration != 0 &&
					expiration < (int)m_actual_time ) {
					vlist_remove_first( m_recently_deleted_clients );
					vlist_remove_first( m_recently_deleted_expirations );
				}
				else
					break;
			}
        }
    }
} // end function CTimerManager::Tick


void CTimerManager::ScheduleNextTick( CTimerClient *c, float time )
{
	Lock();
	CTimerClient *temp;
    int i, offset;

    if( time < m_request_time ) {
        time = (float)(m_request_time + m_interval);
    }
    c->SetNextRequest( time );
    if( time - m_request_time >= 1 ) {
        temp = (CTimerClient *) vlist_peek_first( m_spillover );
        if( ( temp == 0 ) || 
            ( time < temp->GetNextRequest())) {
            vlist_add_first( m_spillover, c );
        }
        else {
            /* insert into m_spillover, but maintain m_spillover's sorting (effectively
             * a bubble sort, but O(N^2) should still be relatively small in this case)*/
            vlist_reset_cursor( m_spillover );
            for(;;) {
                temp = (CTimerClient *) vlist_peek_cursor_next( m_spillover );
                if( ( temp == 0 ) || 
                    ( time < temp->GetNextRequest())) {
                    vlist_add_after_cursor( m_spillover, c );
                    break;
                }
                vlist_advance_cursor( m_spillover );
            }
        }
 		UnLock();
        return;
    }
    offset = (int)((time - m_request_time) * m_output_hz);
    if( offset <= 0 )
        offset = 1;
    i = (m_current_bucket + offset) % m_output_hz;
    /* if the client prefers to be the last item within a tick 
     * (e.g. drivers checking their dirty flags), add to the end */
    if( c->GetPriority() == 0)
        vlist_add_first( m_buckets[i], c );
    else
        vlist_add_last( m_buckets[i], c );
	UnLock();
 }



void CTimerManager::RegisterClient( CTimerClient *c )
{
	Lock();
 	int i;

    if( m_buckets == 0 )
        Init( c->GetNextRequest() );
	//ensure uniqueness of the active clients hash
	if( vhash_lookup_item( m_active_clients, c, 0 ) == VHASH_STATUS_SUCCESS ) {
		UnLock();
		return;
	}
 	if( vlist_item_exists( m_recently_deleted_clients, c )) {
		//remove any references to the new guy in the schedule
		for( i = 0 ; i < m_output_hz ; i++ ) {
			vlist_remove( m_buckets[i], c );
		}
		vlist_remove( m_spillover, c );
	}
	vhash_insert_item( m_active_clients, c, 0 );
	UnLock();
	ScheduleNextTick( c, c->GetNextRequest() );
 }


void CTimerManager::UnRegisterClient( CTimerClient *c )
{
	Lock();
 	int expiration;

	if( !m_active_clients ) {
		UnLock();
 		return;
	}
	vhash_remove_item( m_active_clients, c, 0 );
	//remove any references to the new guy in the schedule
	vlist_remove( m_spillover, c );
	expiration = (int)m_actual_time + 2;
	vlist_add_last( m_recently_deleted_clients, c );
	vlist_add_last( m_recently_deleted_expirations, SA_I2V(expiration) );
	UnLock();
 }


CTimerManager *CTimerManager::GetCurrentTimerManager()
{
    return GetTimerManager();
}



CTimerClient::~CTimerClient() 
{
	CTimerManager::GetTimerManager()->UnRegisterClient( this );
}

//add
CTimerManager * CTimerManager::GetTimerManager()
{
	if(!m_pCTimerManager) {
		m_pCTimerManager = new CTimerManager(100);
	}

	return m_pCTimerManager;
}

CTimerManager * CTimerManager::SetTimerManager(CTimerManager * manager)
{
	CTimerManager * old_ht_manager = m_pCTimerManager;
	m_pCTimerManager = manager;
	return old_ht_manager;
}

void CTimerManager::Lock()
{
#ifdef WIN32
	m_Mutex->Acquire();
#else
	pthread_mutex_lock(m_Mutex);
#endif // WIN32

}
void CTimerManager::UnLock()
{	
#ifdef WIN32
	m_Mutex->Release();
#else
	pthread_mutex_unlock(m_Mutex);
#endif // WIN32
}

//SA_NAMESPACE_END
