/**@file
 *@brief	TODO handlerRotateCenter类头文件
 *@author	xuli
 *@date		2013-7-10
 *@version	1.0
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 *handlerRotateCenter类头文件
 *
 */
#ifndef  HANDLER_ROTATECENTER_H
#define HANDLER_ROTATECENTER_H
#include "m3d/Handler/Handler.h"
#include "m3d/model/Shape.h"

namespace M3D
{
/**@class HandlerRotateCenter
 * @brief HandlerRotateCenter类
 *
 * 提供handlerRotateCenter类的相关方法
 */
class M3D_API HandlerRotateCenter: public Shape
{
public:
	HandlerRotateCenter();
	virtual
	~HandlerRotateCenter();

	/**
	 *
	 * @return
	 */
	Point* GetCenter();

	/**
	 *
	 * @param center
	 * @param size
	 */
	void SetCenter(const Vector3& center,float size);
private:
	Point* point; //!<
	Vector3 m_center;  //!<旋转中心点
};
}
#endif /*HANDLER_H*/
