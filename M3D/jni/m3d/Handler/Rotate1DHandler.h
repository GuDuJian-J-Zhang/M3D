/**@file
 *@brief	TODO RotateHandler类头文件
 *@author	xuli
 *@date		2015-5-14
 *@version	1.0
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 *RotateHandler类头文件
 *
 */

#ifndef ROTATE_HANDLER_H
#define ROTATE_HANDLER_H
#include "m3d/Handler/Handler.h"

namespace M3D
{

class M3D_API Rotate1DHandler: public Handler
{
public:
	Rotate1DHandler();
	virtual ~Rotate1DHandler();

	/**
	 *
	 */
	virtual void ComputeBox();
	/**
	 *
	 * @param action
	 */
	virtual void RayPick(RayPickAction* action);
	/**
	 *
	 * @param renderAction
	 */
	virtual void FindVisiableObject(RenderAction* renderAction);

	virtual const  Color& GetRenderColor();

protected:
	virtual void OnTouchBegin(const TouchEvent& event);
	virtual void OnTouchMove(const TouchEvent& event);
	virtual void OnTouchEnd(const TouchEvent& event);

};
}
#endif /*ROTATE_HANDLER_H*/
