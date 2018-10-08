//
//  IOSExternDef.h
//  SViewIOS
//
//  Created by hoteam on 14/12/10.
//
//

#ifndef __IOSExternDef__
#define __IOSExternDef__

#include <pthread.h>
#include <stdio.h>

#define CLOCK_REALTIME 0



#define SIGRTMIN 32

//IOS
struct itimespec{
    struct timespec it_interval;  /*next value*/
    struct timespec it_value;    /* current value */
};


#endif /* defined(__IOSExternDef__) */
