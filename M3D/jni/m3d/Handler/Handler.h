/**@file
 *@brief	Handler类头文件
 *@author	xuli
 *@date		2013-7-10
 *@version	1.0
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 *Handler类头文件
 *
 */

#ifndef  HANDLER_H
#define HANDLER_H
#include "m3d/M3DMacros.h"
#include "m3d/model/Shape.h"
#include "m3d/action/RenderAction.h"
#include "m3d/graphics/Renderable.h"
namespace M3D
{
class Mesh;
class VertexSet;

//触摸操作封装
class M3D_API TouchEvent
{

};


/**@class Handler
 * @brief Handler类
 *
 * 提供Handler类的相关方法
 */
class M3D_API Handler: public Shape, public Renderable
{
public:
	Handler();
	virtual ~Handler();

	/**
	 * 相应触摸操作事件
	 * @param event
	 * @return false 表示不再继续向下传递 touch事件
	 */
	bool OnTouch(const TouchEvent& event);

	/**
	 * shape virtual functions
	 */
	virtual void ComputeBox();
	/**
	 * @see Shape
	 * @param action
	 */
	virtual void RayPick(RayPickAction* action);

	/**
	 * @see Shape
	 * @return
	 */
	virtual Color& GetDrawColor();

	/**
	 * @see Shape
	 * @return
	 */
	virtual const  Color& GetRenderColor();

	/**
	 * @see Shape
	 * @param renderAction
	 */
	virtual void FindVisiableObject(RenderAction* renderAction);

	/**
	 * 将当前的touch操作，解析为世界坐标系的变化量
	 * @return
	 */
	Matrix3x4 GetWorldTransform();

	/**
	 * @see Shape
	 * @param mesh
	 */
	void SetMesh(Mesh* mesh);

	/**
	 * @see Shape
	 * @return
	 */
	Mesh* GetMesh();

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

	/**
	 * 计算局部坐标系的变化量
	 */
	virtual Matrix3x4 ComputeTransform();

private:
	Mesh* m_Mesh;
};
}
#endif /*HANDLER_H*/
