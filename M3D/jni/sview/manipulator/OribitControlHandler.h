/**@file
 *@brief	TouchHandler类头文件
 *@author
 *@date
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 *提供TouchHandler类的声明
 *
 */
#ifndef M3D_ORIBITCONTROL_HANDLER_H_
#define M3D_ORIBITCONTROL_HANDLER_H_

#include "sview/manipulator/TouchHandler.h"

namespace SVIEW
{

class View;

/**@class TouchHandler
 * @brief TouchHandler类
 *
 * TouchHandler类
 *
 */
class M3D_API OribitControlHandler: public TouchHandler
{
public:
	OribitControlHandler();
	virtual ~OribitControlHandler();
	void StartRotateOnFixedPoint();
	void EndRotateOnFixedPoint();

private:
	virtual void OnTouchUp(float* p, int n);
	virtual void OnTouchMove(int moveType, float* p, int n);
	virtual void OnTouchDown(float* p, int n);

private:
	virtual void InitCamera(bool useAni = true);
	virtual void OptimizeCamera();
	void TwoPointsUp(float*pos, int n);
	void TwoPointsMove(float*, int n = 2);
	void TwoPointsStart(float*, int n = 2);
	float TwoPointsDis(float*, int n = 2);
private:
	/**
	 * 定时器回调函数
	 * @param data
	 */
	static void * GetRotateOnFixedPointFunc(void * data);
	/**
	 * 绕固定点旋转
	 */
	void RotateOnFixedPoint();
private:
	IntVector2 m_PriPointTwo1;
	IntVector2 m_PriPointTwo2;
	float m_PirDistance;
	float m_CurrDistance;

	Vector3 cachePriPoint;
	Vector3 cacheCurPointNear;
	Vector3 cacheCurPointFar;

	///摄像机距离包围盒的距离
	float m_cameraToCenterDis;
	///移动速度
	float m_movSpeed;

	CTimer m_RotateOnFixedPointTimer;

	Vector3 m_oldDirectionVector;

	Vector3 m_oldCameraPos;

	float m_theta;//旋转是的旋转角度间隔
	float m_rotateRadius;

	bool m_needInitDirection;
};

}

#endif //WALKTHROUGHHANDLER_H_
