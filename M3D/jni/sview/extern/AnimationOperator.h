/**@file
 *@brief	特殊效果实现类
 *@author	XL
 *@date		2014-1-7
 *@version	1.0
 *
 */
#ifndef ANIMATION_OPERATOR_H_
#define ANIMATION_OPERATOR_H_

#include "m3d/M3D.h"
#include "m3d/base/Vector3.h"
#include "m3d/base/Color.h"
#include "m3d/base/Matrix4.h"

#include "m3d/base/CTimer.h"

namespace SVIEW
{
class View;
}

using namespace SVIEW;

namespace M3D
{
class Shape;
class SceneNode;
class GroupNode;

class BaseAnimation;

//并行执行的动画队列
typedef vector<BaseAnimation*> ParallelAnimationQueue;
//顺序执行的动画队列
typedef vector<BaseAnimation*> SequentialAnimationQueue;

/**
 * 处理零件移动后，双击复位动画，
 * 包括显示隐藏，设置颜色，设置位置等组合操作
 */
class M3D_API AnimationOperator
{
public:
	static AnimationOperator* GetInstance();
	static AnimationOperator* GetInstance(View* view);
	static const int TIMER_INTER; //定时器时间间隔,单位ms

	AnimationOperator();
	virtual ~AnimationOperator();

	bool Add(BaseAnimation*animation);

	bool Remove(BaseAnimation* animation);

	bool Add(ParallelAnimationQueue& animations);

	bool Remove(ParallelAnimationQueue& animations);

	bool Add(vector<SequentialAnimationQueue>& animations);

	void Stop();

	bool Blink(View* view, int style, vector<Shape*>& shapes,
			vector<Color>& colors, int effectSpeed = M_EFFECT_SPEED,
			int keepTime = M_EFFECT_KEEPTIME);

	bool HideShow(View* view, int style, vector<Shape*>& shapes,
			vector<Color>& colors, int effectSpeed = M_EFFECT_SPEED,
			int keepTime = M_EFFECT_KEEPTIME);

	bool Translate(View* view, int style, vector<Shape*>& shapes,
			vector<Color>& colors, int effectSpeed = M_EFFECT_SPEED,
			int keepTime = M_EFFECT_KEEPTIME);

private:
	void SetView(View* view);

	void Pause();
	void Continue();

	void Reset();
	bool IsFinish();

private:
	static AnimationOperator* instance;

	static const int M_EFFECT_SPEED;
	static const int M_EFFECT_KEEPTIME;

	static void* OperatorTask(void* data);

	vector<SequentialAnimationQueue> m_animations;

	View* m_pView;

	CTimer m_timer;

	mutable Mutex m_mutex;

	bool m_finish;
};
}

#endif //ANIMATION_OPERATOR_H_
