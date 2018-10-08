/**@file
 *@brief	ScaleHandler类头文件
 *@author	TODO xuli
 *@date		2015-5-14
 *@version	1.0
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 *ScaleHandler类头文件
 *
 */

#ifndef  SCALE_HANDLER_H
#define SCALE_HANDLER_H
#include "m3d/Handler/Handler.h"

namespace M3D
{

class M3D_API Scale1DHandler: public Handler
{
public:
	Scale1DHandler();
	virtual ~Scale1DHandler();
	/**
	 * @see Handler
	 */
	virtual void ComputeBox();
	/**
	 * @see Handler
	 * @param action
	 */
	virtual void RayPick(RayPickAction* action);
	/**
	 * @see Handler
	 * @param renderAction
	 */
	virtual void FindVisiableObject(RenderAction* renderAction);
	/**
	 * @see Shape
	 * @return
	 */
	virtual const  Color& GetRenderColor();
protected:
	virtual void OnTouchBegin(const TouchEvent& event);
	virtual void OnTouchMove(const TouchEvent& event);
	virtual void OnTouchEnd(const TouchEvent& event);

};
}
#endif /*SCALE_HANDLER_H*/
