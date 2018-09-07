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

#ifndef M3D_MEASURE_PROPERTY_H_
#define M3D_MEASURE_PROPERTY_H_

#include "m3d/extras/measure/Measure.h"

namespace M3D
{

/**
 * @brief 测量属性类
 */
class M3D_API MeasureProperty: public Measure
{
public:

//	const static int MEASURE_TYPE_LINE_LENGTH = 31; ///边线的长度
//	const static int MEASURE_TYPE_LINE_LENGTH = 32; ///圆或圆弧的弧长、直径（半径）、圆心
//
//	const static int MEASURE_TYPE_ = 33; ///圆或圆弧的弧长、直径（半径）、圆心
//	const static int MEASURE_TYPE_LINE_LENGTH = 34; ///孔的圆心、直径（半径）、深度
//	const static int MEASURE_TYPE_LINE_LENGTH = 35; ///面的面积

public:
	MeasureProperty();
	virtual ~MeasureProperty();

};

}

#endif /*M3D_MEASURE_PROPERTY_H_*/

