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
#include "m3d/graphics/Renderable.h"

namespace SVIEW
{
	class View;
}

namespace M3D
{
class Mesh;
class VertexSet;
class RenderAction;

class M3D_API TouchEvent
{
public:
	enum EventType {
		NONE = 0,
		PUSH = 1 << 0,
		RELEASE = 1 << 1,
		DOUBLECLICK = 1 << 2,
		DRAG = 1 << 3,
		MOVE = 1 << 4,
		KEYDOWN = 1 << 5,
		KEYUP = 1 << 6,
		FRAME = 1 << 7,
		RESIZE = 1 << 8,
		SCROLL = 1 << 9,
		PEN_PRESSURE = 1 << 10,
		PEN_ORIENTATION = 1 << 11,
		PEN_PROXIMITY_ENTER = 1 << 12,
		PEN_PROXIMITY_LEAVE = 1 << 13,
		CLOSE_WINDOW = 1 << 14,
		QUIT_APPLICATION = 1 << 15,
		USER = 1 << 16
	};

public:
	TouchEvent();

	TouchEvent(float* touchData, int touchPoints);


	float getX() const;
	float getY() const;

	void setHandled(bool handled)
	{
		_handled = handled;
	}

	bool getHandled()
	{
		return _handled;
	}
	/** Set whether this event has been handled by an event handler or not.*/
	void setHandled(bool handled) const { _handled = handled; }

	/** Get whether this event has been handled by an event handler or not.*/
	bool getHandled() const { return _handled; }

	/** set the event type. */
	void setEventType(EventType Type) { _eventType = Type; }

	/** get the event type. */
	virtual EventType getEventType() const { return _eventType; }

	SVIEW::View* GetView() const;
	void SetView(SVIEW::View* val);
protected:
	SVIEW::View* m_view;
private:
	EventType _eventType;
	int _touchPoints;
	float* touchPointsData;
	mutable bool _handled;
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
	virtual Color* GetDrawColor();

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
