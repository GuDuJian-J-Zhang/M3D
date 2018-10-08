/**@file
 *@brief	特殊效果实现类
 *@author	XL
 *@date		2014-1-7
 *@version	1.0
 *
 */
#ifndef ANIMATION_NORMALUSE_H_
#define ANIMATION_NORMALUSE_H_

#include "m3d/M3D.h"
#include "m3d/base/Vector3.h"
#include "m3d/base/Color.h"

#include "sview/extern/BaseAnimation.h"

namespace M3D
{
class Shape;
class SceneNode;

/**
 * 显示隐藏动画 TODO
 */
class M3D_API HideShowAnimation: public BaseAnimation
{
public:
	/**
	 *
	 * @param shape
	 * @param from
	 * @param to
	 */
	HideShowAnimation(Shape* shape, bool from, bool to);
	/**
	 *
	 */
	virtual ~HideShowAnimation();

private:
	/**
	 *
	 */
	virtual void OnFrame();

	/**
	 *
	 */
	virtual void OnStop();

	/**
	 *
	 * @param shape
	 * @param from
	 * @param to
	 */
	void SetState(Shape* shape, bool from, bool to);

	/**
	 *
	 * @return
	 */
	bool GetState();

	/**
	 *
	 */
	void Reset();
private:
	Shape* m_shape;

	bool m_fromState;

	bool m_toState;
};

class M3D_API ChangeColorAnimation: public BaseAnimation
{
public:
	/**
	 *
	 * @param shape
	 * @param distColor
	 */
	ChangeColorAnimation(Shape* shape, const Color& distColor);

	/**
	 *
	 */
	virtual ~ChangeColorAnimation();

private:
	/**
	 *
	 */
	virtual void OnFrame();

	/**
	 *
	 */
	virtual void OnStop();

	/**
	 *
	 * @param shape
	 * @param frameStates
	 */
	void SetValue(Shape* shape, const vector<Color>& frameStates);

	/**
	 *
	 * @param shape
	 * @param distColor
	 */
	void SetValue(Shape* shape, const Color& distColor);

	/**
	 *
	 * @return
	 */
	Color GetState();

	/**
	 *
	 */
	void Reset();
private:
	vector<Color> frameStates;

	Shape* m_shape;

	int m_stateIndex;

	bool m_useOrignColor;
};

class M3D_API ChangePosAnimation: public BaseAnimation
{
public:

	/**
	 *
	 * @param shapeNode
	 * @param fromPos
	 * @param disPos
	 */
	ChangePosAnimation(SceneNode* shapeNode, const Vector3& fromPos,
			const Vector3& toPos);

	/**
	 *
	 */
	virtual ~ChangePosAnimation();

private:

	/**
	 *
	 */
	virtual void OnFrame();

	/**
	 *
	 */
	virtual void OnStop();

	/**
	 *
	 * @param shapeNode
	 * @param fromPos
	 * @param disPos
	 */
	void SetValue(SceneNode* shapeNode, const Vector3& fromPos,
			const Vector3& toPos);

	/**
	 *
	 * @return
	 */
	Vector3 GetState();

	/**
	 *
	 */
	void Reset();
private:

	Vector3 m_fromPos;

	Vector3 m_distance;

	SceneNode* shapeNode;
};

}

#endif //ANIMATIONBASE_H_
