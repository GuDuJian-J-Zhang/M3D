/**@file
 *@brief    TODO 旋转中心Handler
 *@author	XL
 *@date		2014-1-7
 *@version	1.0
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */
#ifndef HANDLEROTATECENTER_H
#define  HANDLEROTATECENTER_H
#include "m3d/Handler/HandlerNode.h"
namespace M3D
{
class SolidPlane;
class Image;
class Texture;

/**@class HandlerNodeRotateCenter
 * @brief HandlerNodeRotateCenter类
 */
class M3D_API HandlerNodeRotateCenter: public HandlerNode
{
public:
	HandlerNodeRotateCenter();
	virtual ~HandlerNodeRotateCenter();
	virtual int GetType(void);

public:
	static float axisPntX[]; //!<
	static float axisPntY[]; //!<
	static float axisPntZ[]; //!<
	static float axisPntO[]; //!<

	int m_iPntXNum; //!<
	int m_iPntYNum; //!<
	int m_iPntZNum; //!<
	int m_iPntONum; //!<

	Color m_X; //!<
	Color m_Y; //!<
	Color m_Z; //!<
	Color m_O; //!<

	SolidPlane* m_pSolidPlan; //!<
private:

	Texture* m_pImageRotate; //!<
	Texture* m_ImageTran; //!<
	Texture* m_ImageScale; //!<

};

}

#endif /*HANDLEROTATECENTER_H*/
