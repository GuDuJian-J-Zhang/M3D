//
//  IOSExternDef.h
//  SViewIOS
//
//  Created by hoteam on 14/12/10.
//
//

#ifndef __IOSExternDef__
#define __IOSExternDef__

#ifdef WIN32
//#include <windows.h>
#else
#include <pthread.h>
#endif
#include <stdio.h>

#define CLOCK_REALTIME 0



#define SIGRTMIN 32
#ifdef WIN32

#else
//IOS
struct itimespec{
    struct timespec it_interval;  /*next value*/
    struct timespec it_value;    /* current value */
};
#endif

#endif /* defined(__IOSExternDef__) */
