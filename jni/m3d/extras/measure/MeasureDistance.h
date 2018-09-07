/**@file
 *@brief
 *@author   xuli
 *@date		2015-7-7
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 *Face类头文件
 *
 */

#ifndef M3D_MEASURE_DISTANCE_H_
#define M3D_MEASURE_DISTANCE_H_

#include "m3d/extras/measure/Measure.h"

namespace M3D
{

/**
 * @brief 距离测量对象类
 */
class M3D_API MeasureDistance: public Measure
{
public:

public:
	MeasureDistance();
	virtual ~MeasureDistance();

private:
	void Init();
};

}

#endif /*M3D_MEASURE_DISTANCE_H_*/

