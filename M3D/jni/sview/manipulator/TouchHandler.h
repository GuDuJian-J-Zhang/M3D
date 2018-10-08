/**@file
 *@brief
 *@author  XL
 *@date    2015-10-18
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 *
 *
 */
#ifndef M3D_ITOUCHHANDLER_H_
#define M3D_ITOUCHHANDLER_H_

#include "m3d/M3D.h"
#include "m3d/utils/Trackball.h"
#include "sview/extern/WorkNodes.h"
#include "m3d/base/CTimer.h"
#define EBUILDING
namespace M3D
{
class WorkNodes;
class SceneManager;
class Quaternion;
}

using namespace M3D;

namespace SVIEW
{


enum TouchHandlerType
{
	HANDLER_WALKTHROUGH = 0,
	HANDLER_COMMON = 1,
	HANDLER_ORBIT = 2,
	HANDLER_DRAGGER = 3
};

class View;

/**@class TouchHandler
 * @brief TouchHandler类
 *
 * TouchHandler类
 *
 */
class M3D_API TouchHandler
{
public:
	TouchHandler();
	virtual ~TouchHandler();

	/**
	 * 设置此TouchHandler所属的View
	 * @param pView
	 */
	void SetView(SVIEW::View* pView);

	/**
	 * 复位清除此Handler的所有操作
	 */
	void Reset();

	/**
	 * 处理外部触摸事件
	 * @param p 触摸数据坐标指针数组
	 * @param n 触摸点的个数
	 * @param TouchType 触摸的类型 DOWN=1，MOVE=2，UP=0
	 */
	void HandleTouchEvent(float* p, int n, int TouchType);

	/**
	 * 处理外部触摸事件
	 * @param p 触摸数据坐标指针数组
	 * @param n 触摸点的个数
	 * @param TouchType 触摸的类型 DOWN=1，MOVE=2，UP=0
	 * @para moveType =1一手指旋转，=2一手指平移，=3一手指缩放
	 */
	void HandleTouchEvent(float* p, int n, int TouchType, int moveType);

	/**
	 *结束当前持续旋转状态
	 */
	void CloseKeepState();

	//void RefreshModelViewCamera();

	void Open();

	void Close();

	virtual TouchHandlerType GetTouchHandlerType()
	{
		return m_touchHandlerType;
	}

	int GetDefaultView();
	void SetDefaultView(int viewType);

	virtual void OribitMode(bool value); //动态观察模式

	virtual void ConstraintMode(bool value); //约束模式

	virtual void FreeViewMode(bool value); //自由观察模式;

	virtual void OnUpDataTouchIntent();

	void StartRotateOnFixedPoint();
	void EndRotateOnFixedPoint();
	void SetBirdEyeParameter(int pointId, float speed);
	void SetBirdEyeSpeed(float speed);
	void SetFixedPointRotateParameter(int pointId, float speed);
	void SetFirstPersionParameter(float speed);
	void SetUpDirection(M3D::Vector3 & up, View * view);
    //TODO 德方要求
	/**
	 * 可以通过单指，绕垂直于屏幕方向旋转
	 * @param screenPos 手指点击的屏幕坐标
	 * @param angle 角度单位为弧度
	 */
	void VerticalScreenRotation(M3D::Vector2 & screenPos,float angle);

	/**
	 * 两指旋转
	 * @param p
	 * @param n
	 */
	void TwoFingersRotation(float* p, int n);

	//TODO 德方要求
	void StartModelRotateOnFixedPoint();
	void EndModelRotateOnFixedPoint();
	void ModelRotateOnFixedPoint();
    void CameraRotateOnFixedPoint();
	static void * GetModelRotateOnFixedPointFunc(void * data);
	float m_modelRotateAngle;
	CTimer m_ModelRotateOnFixedPointTimer;
	Matrix3x4 m_setUpPlcMat;//!<设置正方向时，设置的矩阵，临时存放起来，用作德方的那种旋转

	//下面的旋转为实现展示功能，与Web版上的展示功能一致
	void OnlyRotateOnAxisFunc(void * data);
	void GetRotateOnScreenMiddleLIneFunc(void * data);
	float m_showModeSpeed;
	float m_rotCircleCount;
	float m_showModeRotAngle;
	void SetShowModeSpeed(float speed);
	//--------------------------------------------
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

	M3D::Vector3 GetPickPointCoord(SceneManager * scene, M3D::Vector2 & screenPos);

protected:
	virtual void InitCamera();

	//virtual void InitModelViewCamera();
	/**
	 * 优化摄像机的位置，切换到漫游模式
	 */
	virtual void OptimizeCamera();

	virtual void OnTouchUp(float* p, int n);
	virtual void OnTouchMove(int moveType, float* p, int n);
	virtual void OnTouchDown(float* p, int n);

	void TouchUp(float* p, int n);
	void TouchMove(int moveType, float* p, int n);
	void TouchDown(float* p, int n);
	void UpDataTouchIntent();
	void UpDataTouchIntentWithSpeedControl(float speedControl);
	void EndKeepState();
	void StartKeepState();
	void KeepState();
	bool StateChanged();

	/**
	 * 更新场景的运动状态
	 */
	void UpdateRenderQuality(bool isMoving);
	static void* TimerTask(void* data);

public:
	static const int TOUCHUP = 0;
	static const int TOUCHDOWN = 1;
	static const int TOUCHMOVE = 2;

protected:
	IntVector2 m_PriPointTwo1;
	IntVector2 m_PriPointTwo2;
	WorkNodes* m_SelectedNodes;
	SceneManager* m_SceneManager;
	Trackball m_TrackBall;
	View* m_pView;
	int m_Width;
	int m_Height;
	CTimer m_KeepRotateTimer;
	float m_screenDepth;
	TouchHandlerType m_touchHandlerType;

	M3D::Vector3 m_upDirection;

	int m_defaultView;//默认试图朝向

#ifdef EBUILDING
	static bool m_freeViewMode;//自由观察模式

	static bool m_controlLockXY;	//约束模式

	static bool m_oribitMode;//锁定一个轴模式，动态观察
#else
	bool m_freeViewMode;//自由观察模式

	bool m_controlLockXY;	//约束模式

	bool m_oribitMode;//锁定一个轴模式，动态观察
#endif

	M3D::Vector3 m_oribitControlTarget;//oribit 观察点

	float m_oribitPhi;
	float m_oribitTheta;

	CTimer m_RotateOnFixedPointTimer;
	bool m_needInitDirection;

	float m_theta; //旋转是的旋转角度间隔
	float m_birdEyeSpeed; //鸟瞰速度
	M3D::Vector3 m_birdEyeFixedPnt; //鸟瞰选择的点

	bool m_changeDisOnRotate;

	float m_firstPersionSpeed; //漫游速度
	float m_standardSpeed;
	Quaternion m_modelRotation;
	M3D::Vector3 m_oldCameraPos;
	float m_rotateRadius;
};

}

#endif /*M3D_ITOUCHHANDLER_H_*/
