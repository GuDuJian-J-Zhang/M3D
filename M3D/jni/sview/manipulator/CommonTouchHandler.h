/**@file
 *@brief	CommonTouchHandler类头文件
 *@author
 *@date
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 *提供TouchHandler类的声明
 *
 */
#ifndef M3D_TOUCHEVENTHANDLER_H_
#define M3D_TOUCHEVENTHANDLER_H_

#include "sview/manipulator/TouchHandler.h"
#include "m3d/base/Quaternion.h"

namespace SVIEW
{

class View;

/**@class CommonTouchHandler
 * @brief CommonTouchHandler类
 *
 * CommonTouchHandler类,用来管理平行投影模式下的一系列触屏操作
 *
 */
class M3D_API CommonTouchHandler: public TouchHandler
{
public:
	CommonTouchHandler();
	virtual ~CommonTouchHandler();
	/**
	 * 观察模式
	 * @param value
	 */
	virtual void OribitMode(bool value); //观察模式

	/**
	 * 方向锁定
	 * @param value
	 */
	virtual void ConstraintMode(bool value);

	/**
	 * 自由模式
	 * @param value
	 */
	virtual void FreeViewMode(bool value); //自由观察模式

	/**
	 * @brief 绕坐标轴旋转
	 */
	void RotateAroundAxis();

	/**
	 * @brief 自由旋转
	 */
	void FreeViewRotate();
	/**
	 * @brief 更新触摸类型
	 */
	virtual void OnUpDataTouchIntent();

	virtual void ResetViewCamera();
 
	/**
	 * @brief
	 */
	virtual void OrbitControl();
	/**
	 * @brief 初始化相机
	 */
	virtual void InitCamera(bool useAni = true);
	/**
	 * @brief 优化相机
	 */
	virtual void OptimizeCamera();
	virtual void OnTouchUp(float* p, int n);
	virtual void OnTouchMove(int moveType, float* p, int n);
	virtual void OnTouchDown(float* p, int n);

	Quaternion m_quat;
};

}

#endif /*M3D_TOUCHEVENTHANDLER_H_*/
