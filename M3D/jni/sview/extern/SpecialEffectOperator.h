/**@file
 *@brief	特殊效果实现类
 *@author	XL
 *@date		2014-1-7
 *@version	1.0
 *
 */
#ifndef SPECIALEFFECTOPERA_H_
#define SPECIALEFFECTOPERA_H_

#define NO_EFFECT -1
#define BLINK_EFFECT 0 //闪烁效果
#define HIDESHOW_EFFECT 1 //显隐效果
#include "m3d/M3D.h"
#include "m3d/base/Vector3.h"
#include "m3d/base/Color.h"
#include "m3d/base/Matrix4.h"

#include "m3d/base/CTimer.h"

namespace SVIEW
{
class View;
}

typedef void (*EffectFinishCallBack)(void*);

using namespace SVIEW;

namespace M3D
{
class Shape;
class SceneNode;
class GroupNode;

/**
 * 特殊效果操作类，
 * 通过NormalAnimation等动画组合来实现各种效果
 * TODO
 */
class M3D_API SpecialEffectOperator
{
public:
	static SpecialEffectOperator* GetInstance();

	SpecialEffectOperator();
	virtual ~SpecialEffectOperator();

	bool Add();

	bool Remove();

	bool ApplyBlinkEffect(View* view, int style, vector<Shape*>& shapes,
			vector<Color>& colors, int effectSpeed = M_EFFECT_SPEED,
			int keepTime = M_EFFECT_KEEPTIME);

	bool CloseEffect(View* view);

private:
	void Reset();

	void StartEffect();
	void StopEffect();

	void StopBlinkEffect();

private:
	static SpecialEffectOperator* instance;

	static void* BlinkEffectCallback(void* data);
	static const int M_EFFECT_SPEED;
	static const int M_EFFECT_KEEPTIME;

	vector<Color> m_blinkColors; //闪烁颜色
	vector<Color> m_defaultBlinkColors; //默认闪烁颜色

	vector<Shape*> m_effectShapes;
	vector<SceneNode*> m_effectNodes;

	int m_effectStyle;

	int m_currentColorIndex;

	int m_effectSpeed;
	int m_effectKeepTime;

	int m_timerTickTimees;

	View* m_pView;
	CTimer m_timer;
	EffectFinishCallBack m_effectFinishCallback;
	void* m_effectFinishCallbackData;

};
}

#endif //SPECIALEFFECTOPERA_H_
