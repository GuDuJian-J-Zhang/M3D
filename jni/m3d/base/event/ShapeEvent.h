/**
 *@file
 *@brief    CamereEvent
 *@author	XL
 *@date		2014-1-7
 *@version	1.0
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 */

#ifndef  M3D_CAMERA_EVENT_H_
#define  M3D_CAMERA_EVENT_H_

#include "m3d/base/event/BaseEvent.h"

namespace M3D
{
/**
 * @class
 */
class M3D_API CamereEvent:public BaseEvent
{
public:
	CamereEvent();
	virtual ~CamereEvent();

};

}
#endif /*M3D_CAMERA_EVENT_H_*/

