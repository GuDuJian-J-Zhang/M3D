/**@file
 *@brief	TODO  HandlerPoint类头文件
 *@author	xuli
 *@date		2013-7-10
 *@version	1.0
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 *HandlerPoint类头文件
 *
 */
#ifndef  HANDLERPOINT_H
#define HANDLERPOINT_H
#include "m3d/Handler/Handler.h"
#include "m3d/graphics/Billboard.h"

namespace M3D
{
class CameraNode;
class Point;

/**@class HandlerPoint
 * @brief HandlerPoint类
 *
 * 提供HandlerPoint类的相关方法
 */
class M3D_API HandlerPoint: public Handler
{
public:
	HandlerPoint();
/**
 *
 * @param vec
 * @param size
 */
	HandlerPoint(const Vector3& vec,float size = 1.0f);

	virtual ~HandlerPoint();
	/**
	 * @see Shape
	 * @param action
	 */
	virtual void RayPick(RayPickAction* action);

	/**
	 * @see Shape
	 * @param renderAction
	 */
	virtual void FindVisiableObject(RenderAction* renderAction);

	/**
	 *
	 * @param vec
	 * @param size
	 */
	void Init(const Vector3& vec,float size);
    
	/**
	 *
	 * @param tyep
	 */
    void SetDrawType(int tyep);

    /**
     *
     * @return
     */
	const Vector3& GetPosition();

	/**
	 *
	 * @return
	 */
	Point* GetPoint();

	/**
	 *  @see Shape
	 */
	virtual void ComputeBox();

	virtual const  Color& GetRenderColor();

private:
	Point* m_point; //!<
    int m_drawType; //!<
};
}
#endif /*HANDLERPOINT_H*/
