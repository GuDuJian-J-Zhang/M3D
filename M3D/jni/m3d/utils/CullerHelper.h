/**@file
 *@brief    剔除算法工具类
 *@brief    包括视景体剔除和小件剔除
 *@author	XL
 *@date		2014-1-7
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */
#ifndef		M3D_CULLERHELPER_H
#define     M3D_CULLERHELPER_H

#include "m3d/base/Vector3.h"
#include "m3d/base/Matrix4.h"
#include "m3d/base/BoundingBox.h"

namespace M3D
{

class CameraNode;
/**@class
 * @brief 视景体剔除,微小模型剔除，LOD选择 算法实现类
 *
 */
class M3D_API CullerHelper
{
public:
	CullerHelper();
	virtual ~CullerHelper();

	/**
	 * 判断摄像机能够看到box
	 * @param box
	 * @param camera
	 * @return @see Intersection
	 */
	Intersection InViewPort(BoundingBox& box,CameraNode* camera);

	/**
	 * 判断box 是否为小模型
	 * @param box
	 * @param camera
	 * @return true小模型 false 大模型
	 */
	int IsLittleModel(const BoundingBox& box,CameraNode* camera);
	int IsLittleBody(const BoundingBox& box, CameraNode* camera);
	/**
	 * 设置比较模型对角线的长度
	 * @param modelLength
	 */
	void SetModelLength(float modelLength);

	/**
	 * 设置屏幕对角线长度
	 * @param srcLength
	 */
	void SetScreenLength(float srcLength);

	/**
	 * 通过摄像机状态跟新 剔除所需要的数据
	 * @param camera
	 */
	void UpDate(CameraNode* camera);

	void Reset();

	/**
	 * 设置是否允许剔除，通过摄像机更新剔除所需的数据
	 * @param camera
	 * @param allowCuller
	 */
	void AllowCuller(CameraNode* camera, bool allowCuller);

	void AddCullerRatio(float fps);
	void ReduceCullerRatio(float fps);

	float GetCullerPercent();

private:
	/**
	 *
	 * @param camera
	 * @return
	 */
	bool isCameraSacle(CameraNode* camera);

private:
	float m_drawLLLimit;//!<小于屏幕指定像素点 在世界坐标系的阈值
	float m_drawLimit;	//!<剔除比较阈值
	float m_modelLength;	//!<模型长度
	int m_cullerStyle;	//!<剔除方式
	float m_screenLength;//!< 屏幕对角线长度
	float m_priCameraScale;	//!<摄像机前一次的缩放值
	bool m_allowCuller;	//!<是否需要进行剔除

	float m_cullerbase;
	float m_cullerPercent;//!<剔除所占百分比
};

}
#endif /*CULLERHELPER_H*/

