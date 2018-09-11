/**@file
 *@brief
 *@author	XL
 *@date		2014-1-7
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */
#ifndef _MEM_LEAK_DETECTOR_H
#define _MEM_LEAK_DETECTOR_H

/**
 * windows下内存泄漏检测函数
 */
#ifdef _DEBUG
	/**/
	#define _CRTDBG_MAP_ALLOC

	#include <stdlib.h>
	#include "crtdbg.h"
	#define DEBUG_NEW   new( _NORMAL_BLOCK, __FILE__, __LINE__)
#else
	#define DEBUG_NEW
#endif

#ifdef _DEBUG
	#define new DEBUG_NEW
#endif  

void memoryLeakDetector();

void setBreakPoint(int alloc_num);

void dumpMemoryLeaks();

#endif
