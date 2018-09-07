/**@file
 *@brief	追踪球类，把屏幕上的操作通过追踪球算法映射到三维空间中，提升交互性
 *@author	XL
 *@date		2014-1-7
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */
#ifndef M3D_TRACKBALL_H
#define M3D_TRACKBALL_H

#include "m3d/base/Vector3.h"
#include "m3d/base/Quaternion.h"
#include "m3d/base/ControlInfo.h"

namespace M3D
{
class SceneNode;
class SceneManager;
class CameraNode;
/**
 * @class
 * @brief 追踪求算法类
 * 将屏幕上的二维点操作，转换成空间中的点操作
 */
class M3D_API Trackball
{
public:
	static int KEEPINGSTATETIMES;
	static float CURRENTMODELSIZE;
	static bool ISMOVING;
	static float DrawLimit;

	static bool ISROTATING;
	static int MOVESTATE;
	static int TIMES;

	static Trackball* Instance();

private:
	static Trackball* instance;
public:
	Trackball(void);
	virtual ~Trackball(void);

	void Reset();
	/**
	 * 处理一点抬起操作
	 * @param pos
	 * @param n
	 */
	void OnePointUp(float*pos, int n);
	/**
	 * 处理两点抬起操作
	 * @param pos
	 * @param n
	 */
	void TwoPointsUp(float*pos, int n);
	/**
	 * 处理一点按下操作
	 * @param
	 * @param n
	 */
	void OnePointStart(float*, int n = 1);

	/**
	 *处理两点按下操作
	 * @param
	 * @param n
	 */
	void TwoPointsStart(float*, int n = 2);

	/**
	 * 处理一点旋转移动操作
	 * @param
	 * @param n
	 */
	void OnePointRotate(float*, int n = 1);
	/**
	 * 处理两点平移操作
	 * @param
	 * @param n
	 */
	void OnePointsMove(float*, int n = 1);
	/**
	 * 处理一点缩放操作
	 * @param
	 * @param n
	 */
	void OnePointsScale(float*, int n = 1);

	/**
	 * 处理两点移动操作
	 * @param
	 * @param n
	 */
	void TwoPointsMove(float*, int n = 2);

	/**
	 * 设置需要转换当前视口窗口的宽和高
	 * @param iWidth
	 * @param iHeight
	 */
	void SetTrackWindow(int iWidth, int iHeight);
	void SetTrackWindow(int iWidth, int iHeight, int iCenterX, int iCenterY);

	/**
	 * @brief 设置该trackball 所属的场景管理指针
	 * @param pSceneManager
	 */
	void SetSceneManager(SceneManager* pSceneManager);

	/**
	 * 设置旋转操作速度
	 * @param rotateSpeed
	 */
	void SetRotateSpeed(float rotateSpeed);

	/**
	 * 得到旋转操作速度
	 * @return
	 */
	float GetRotateSpeed();

private:

	void Tracking(int iDx, int iDy, Vector3 *axis, float *fAngle);

	/**
	 * 将屏幕上点转换成三维空间点
	 * @param iMouseX
	 * @param iMouseY
	 * @return
	 */
	Vector3 ScreenToVector(int iMouseX, int iMouseY);

	/**
	 * 两点之间的距离
	 * @param
	 * @param n
	 * @return
	 */
	float TwoPointsDis(float*, int n = 2);
	float TwoPointsTrackingAngle(float*, int n = 2);

	///获取空间中的点，用来在缩放或者平移时，确定模型的移动位置
	Vector3 ScreenToDepthVector(const IntVector2& scrVector);

	///获取空间中的点，用来在缩放或者平移时，确定模型的移动位置
	void AdjustScaleToMoveVector(const IntVector2& scrVector);

public:

	Vector3 m_axis; //!< 纪录旋转轴
	float m_angle;//!<纪录旋转角度

	SceneManager* pSceneManager;

	CameraNode* GetCamera() { return pCamera; }
	void SetCamera(CameraNode* val) { pCamera = val; }

	ControlInfo mvMatrix;//!< 追踪球，转换得到的三维操作信息
private:
	// previous points
	IntVector2 m_PriPointOne;//!<一点操作，前一次点
	IntVector2 m_PriPointOneScale;//!<一点操作，前一次缩放值
	IntVector2 m_PriPointOneScaleCenter;//!<一点缩放，前一次缩放中心
	IntVector2 m_PriPointOneMove;//!<一点平移，前一次平移位置
	IntVector2 m_PriPointTwo1;//!<两点操作，前一次点
	IntVector2 m_PriPointTwo2;//!<两地操作，前一次点

	float m_PirDistance;//!<前一次缩放距离
	float m_CurrDistance;//!<当前缩放距离

	Vector3 m_priRotationCenter; //!<前一次旋转中心
	Vector3 m_rotationAxis; //!<两指旋转 旋转轴
	float m_priAngle; //!<两指旋转前一次旋转角度
	float m_currAngle; //!<两指旋转当前旋转角度

	Vector3 m_cachePriPoint;//!<一点平移旋转缩放时前一次点
	Vector3 m_cacheCurPointNear;//!<

	int m_iWidth;//!<屏幕宽和高
	int m_iHeight;
	int m_iCenterX;
	int m_iCenterY;
	int m_iRadius; //!< 追踪求半径

	Vector3 m_Start;//!<
	Vector3 m_Stop;

	float m_screenDepth; //!<默认投影深度

	float m_rotateSpeed;//!<旋转速度

	int m_priDownTick;

	CameraNode* pCamera;
};
}

#endif /*M3D_TRACKBALL_H*/
