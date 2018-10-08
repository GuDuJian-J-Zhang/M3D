/**@file
 *@brief	几何属性信息空间变换
 *@author
 *@date		2015-8-17
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */

#ifndef M3D_GEMETRYHELPER_H_
#define M3D_GEMETRYHELPER_H_
#include "m3d/M3D.h"
namespace M3D
{
class Vector3;
class Matrix3x4;
class PlaneFaceAttribute;
class RevolutionFaceAttribute;
class CylinderFaceAttribute;
class ConicalFaceAttribute;
class SphereFaceAttribute;
class ToroidalFaceAttribute;
class LineAttribute;
class EllipseAttribute;

/**
 * @brief 集合帮助类
 */
class M3D_API GeometryHelper
{
public:
	GeometryHelper();
	virtual ~GeometryHelper();

	/**
	 * 坐标点变换
	 * @param coordinate 要变化的坐标
	 * @param worldMatrix 变换矩阵
	 * @return 成功返回true 否则 false
	 */
	static bool Transform(Vector3& coordinate,const Matrix3x4& matrix);

	/**
	 * 平面几何属性变换
	 * @param attribute 待变换的几何属性
	 * @param matrix 变换矩阵
	 * @return true 成功 false 失败
	 */
	static bool Transform(PlaneFaceAttribute& attribute,const Matrix3x4& matrix);

	/**
	 * 旋转面几何属性变换
	 * @param attribute 待变换的几何属性
	 * @param matrix  变换矩阵
	 * @return true 成功 false 失败
	 */
	static bool Transform(RevolutionFaceAttribute& attribute,const Matrix3x4& matrix);

	/**
	 * 圆柱面几何属性变换
	 * @param attribute 待变换的几何属性
	 * @param matrix 变换矩阵
	 * @return true 成功 false 失败
	 */
	static bool Transform(CylinderFaceAttribute& attribute,const Matrix3x4& matrix);

	/**
	 * 圆锥面几何属性变换
	 * @param attribute 待变换的几何属性
	 * @param matrix 变换矩阵
	 * @return true 成功 false 失败
	 */
	static bool Transform(ConicalFaceAttribute& attribute,const Matrix3x4& matrix);

	/**
	 * 球面几何属性变换
	 * @param attribute 待变换的几何属性
	 * @param matrix 变换矩阵
	 * @return true 成功 false 失败
	 */
	static bool Transform(SphereFaceAttribute& attribute,const Matrix3x4& matrix);

	/**
	 * 轮胎面几何属性变换
	 * @param attribute 待变换的几何属性
	 * @param matrix 变换矩阵
	 * @return true 成功 false 失败
	 */
	static bool Transform(ToroidalFaceAttribute& attribute,const Matrix3x4& matrix);

	/**
	 * Line几何属性变换
	 * @param attribute 待变换的几何属性
	 * @param matrix 变换矩阵
	 * @return true 成功 false 失败
	 */
	static bool Transform(LineAttribute& attribute,const Matrix3x4& matrix);

	/**
	 * Ellipse几何属性变换
	 * @param attribute 待变换的几何属性
	 * @param matrix 变换矩阵
	 * @return true 成功 false 失败
	 */
	static bool Transform(EllipseAttribute& attribute,const Matrix3x4& matrix);
};

}

#endif /*M3D_GEMETRYHELPER_H_*/

