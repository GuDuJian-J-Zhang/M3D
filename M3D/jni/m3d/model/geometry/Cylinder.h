/**@file
 *@brief	Cylinder类头文件
 *@author	xuli
 *@date		2013-7-10
 *@version	1.0
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 *Handler类头文件
 *
 */

#ifndef  M3D_CYLINDER_H
#define M3D_CYLINDER_H
#include "m3d/model/Shape.h"
#include "m3d/M3DMacros.h"

namespace M3D
{
/**@class Cylinder
 * @brief Cylinder类
 *
 * 提供Cylinder类的相关方法
 */
class M3D_API Cylinder: public Shape
{
public:
	Cylinder();
	virtual ~Cylinder();

};
}
#endif /*CYLINDER_H*/
