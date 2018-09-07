/*
 * InstanceAttribute.h
 *
 *  Created on: 2015-4-10
 *      Author: bing
 */

#ifndef INSTANCEATTRIBUTE_H_
#define INSTANCEATTRIBUTE_H_
#include "m3d/base/Color.h"
#include "m3d/base/Matrix4.h"
#include <string>

namespace M3D
{
	typedef struct {
		int id;
		string path;
		bool visible;
		bool hasColor;
		Color insColor;
		int materialId;
		Matrix4 placeMatrix;
	} InstanceAttribute;

}
#endif /* INSTANCEATTRIBUTE_H_ */
