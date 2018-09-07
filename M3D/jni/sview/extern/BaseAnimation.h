/**@file
 *@brief	特殊效果实现类
 *@author	XL
 *@date		2014-1-7
 *@version	1.0
 *
 */
#ifndef ANIMATIONBASE_H_
#define ANIMATIONBASE_H_

#define NO_EFFECT -1
#define BLINK_EFFECT 0 //闪烁效果
#define HIDESHOW_EFFECT 1 //显隐效果
#include "m3d/M3D.h"
#include "m3d/base/Vector3.h"
#include "m3d/base/Color.h"

namespace SVIEW
{
class View;
}

using namespace SVIEW;

namespace M3D
{
class Shape;
class Model;
class GroupNode;
class AnimationOperator;

class M3D_API AnimtaionListener
{
public:
	AnimtaionListener();
	virtual ~AnimtaionListener();

	virtual void OnFrame();

	virtual void OnStop();
};
/**
 * 动画播放基类，
 * ISUE
 * 只能在动画执行前设置其 播放速度 执行的帧动作。动画播放开始后再调用可能会由于线程资源竞争问题出错。
 */
class M3D_API BaseAnimation
{
public:
	BaseAnimation();
	virtual ~BaseAnimation();

	void SetAnimationOperator(AnimationOperator* aniOperator);

	/**
	 *
	 * 设置播放速度和持续时间，默认播放速度为每秒3帧，持续时间为1秒
	 */
	void SetSpeedAndKeepTime(int speed = M_EFFECT_SPEED, int keepTime =
			M_EFFECT_KEEPTIME);

	void Start();

	void Stop();

	void Frame();

	bool AddListener(AnimtaionListener* listener);
	bool RemoveListener(AnimtaionListener* listener);

	bool IsFinish();

protected:
	virtual void OnFrame();

	virtual void OnStop();

private:

	void Init();
	void Reset();

	void RemoveFromOperator();

protected:
	int m_allFrameCount; //总帧数
	int m_currentFrameIndex; //当前执行到的帧数

private:
	static const int M_EFFECT_SPEED;
	static const int M_EFFECT_KEEPTIME;

	bool m_finish;

	vector<AnimtaionListener*> m_listeners;

	int m_keepTime; //持续时间单位秒

	int m_speed; //播放速度  每秒执行帧 最大值为100

	bool m_dirt; //数据脏标示

	AnimationOperator* m_animationOperator;

	int m_tickTime; //外部动画控制器，调用Frme的次数

};
}

#endif //ANIMATIONBASE_H_
