﻿/**
 *@file
 *@brief	标准头文件，包括STL等常用库
 *@author	XL
 *@date		2014-1-7
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */
#ifndef M3D_H_
#define M3D_H_

#define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS 1

#include<m3d/utils/mem_leak_detector.h>

#include <cerrno>
#include <cstddef>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <string.h>
#include <map>
#include <vector>
#include <fstream>
#include <stack>
#include <list>
#include <queue>

//#include <memory>
using std::ifstream;
using std::ostream;
using std::ios;

using std::string;
using std::map;
using std::vector;
using std::pair;
using std::stack;
using std::list;
using std::queue;

//#include <math.h>
#include <assert.h>

#include "Utility.h"
#ifdef WIN32
 #define IDTYPE unsigned long long int
#else
 #define IDTYPE int
#endif

#define M3D_API
#ifdef _WIN32
#ifdef M3D_EXPORTS
#define M3D_API __declspec(dllexport)
#else
#define M3D_API __declspec(dllimport)
#endif
#endif

//#define HUAWEI
#include "m3d/base/Object.h"
/**
 *试衣软件中绘制剖切线用的宏，这个宏主要是用到了原始的剖切线数据
 */
#define CLOTH
#undef CLOTH

#endif /*M3D_H_*/

