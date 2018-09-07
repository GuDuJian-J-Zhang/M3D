/**
 *@file
 *@brief
 *@author	XL
 *@date		2015-2-26
 *@version	1.0
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 */
#ifndef M3D_RANDOM_H_
#define M3D_RANDOM_H_

#include "m3d/M3D.h"

namespace M3D
{

/** 
 * @brief Set the random seed. The default seed is 1. 
 */
M3D_API void SetRandomSeed(unsigned seed);
/** 
 * @brief Return the current random seed. 
 */
M3D_API unsigned GetRandomSeed();
/** 
 * @brief Return a random number between 0-32767. Should operate similarly to MSVC rand(). 
 */
M3D_API int Rand();
/** 
 * @brief Return a standard normal distributed number. 
 */
M3D_API float RandStandardNormal();

}
#endif //RANDOM_H
