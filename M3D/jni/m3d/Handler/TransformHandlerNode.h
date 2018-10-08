/**@file
 *@brief    TODO TransformHandlerNode类，实现类似把手的相关功能
 *@author	XL
 *@date		2014-1-7
 *@version	1.0
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */
#ifndef TRANSFORM_HANDLER_NODE_H
#define  TRANSFORM_HANDLER_NODE_H

#include "m3d/M3D.h"
#include "m3d/Handler/HandlerNode.h"
namespace M3D
{
class Translate1DHandler;
class Rotate1DHandler;
class Scale1DHandler;

class M3D_API TransformHandlerNode: public HandlerNode
{
public:
	TransformHandlerNode();
	virtual ~TransformHandlerNode();

private:
	//初始化，内部创建平移旋转和缩放节点，统一加入到HandlerNode中
	void Init();

private:
	Translate1DHandler* m_xTran;  //!<x轴的移动Handler
	Translate1DHandler* m_yTran;  //!<y轴的移动Handler
	Translate1DHandler* m_zTran;  //!<z轴的移动Handler

	Rotate1DHandler* m_xRotate;  //!<沿x轴的旋转Handler
	Rotate1DHandler* m_yRotate;  //!<沿y轴的旋转Handler
	Rotate1DHandler* m_zRotate;  //!<沿z轴的旋转Handler

	Scale1DHandler* m_xScale;  //!<x轴的缩放Handler
	Scale1DHandler* m_yScale;  //!<x轴的缩放Handler
	Scale1DHandler* m_zScale;  //!<x轴的缩放Handler

};
}

#endif /*TRANSFORM_HANDLER_NODE_H*/


