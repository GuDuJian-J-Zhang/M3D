/**@file
 *@brief	TODO TranslateHandler类头文件
 *@author	xuli
 *@date		2015-5-14
 *@version	1.0
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 *TranslateHandler类头文件
 *
 */

#ifndef  TRANSLATE_HANDLER_H
#define TRANSLATE_HANDLER_H
#include "m3d/Handler/Handler.h"
#include "m3d/base/Ray.h"

namespace M3D
{

class M3D_API Translate1DHandler: public Handler
{
public:
	Translate1DHandler();
	virtual ~Translate1DHandler();

	/**
	 * @see Handler
	 */
	virtual void ComputeBox();
	/**
	 *@see Handler
	 * @param action
	 */
	virtual void RayPick(RayPickAction* action);
	/**
	 * @see Handler
	 * @param renderAction
	 */
	virtual void FindVisiableObject(RenderAction* renderAction);

protected:
	/**
	 * @see Handler
	 * @param event
	 */
	virtual void OnTouchBegin(const TouchEvent& event);
	/**
	 * @see Handler
	 * @param event
	 */
	virtual void OnTouchMove(const TouchEvent& event);

	/**
	 * @see Handler
	 * @param event
	 */
	virtual void OnTouchEnd(const TouchEvent& event);

private:
	void Init();

private:
	Ray m_Ray; //!<

};
}
#endif /*TRANSLATE_HANDLER_H*/
