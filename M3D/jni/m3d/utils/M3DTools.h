/**@file
 *@brief
 *@author	XL
 *@date		2014-1-7
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */
#ifndef	 M3D_TOOLS_H
#define  M3D_TOOLS_H
#include "m3d/M3D.h"
#include "m3d/base/Matrix4.h"
#include "m3d/base/Quaternion.h"
#include "m3d/base/Vector3.h"

namespace M3D
{
/**@class
 * @brief M3D工具类。提供一些常用的工具函数
 *
 *
 *
 */
class M3D_API M3DTOOLS
{
public:
	/**
	 * 浮点数转换成字符串
	 * @param fZ
	 * @param alen
	 * @return
	 */
	static string floatToString(float fZ, const int alen = 3);
	/**
	 * 整数转换成字符串
	 * @param iZ
	 * @return
	 */
	static string IntToString(int iZ);

	/**
	 * float插值
	 * @param from
	 * @param to
	 * @param count
	 * @param midScale
	 * @return
	 */
	static bool FloatSlerp(float from, float to, int count, float* midScale);
	/**
	 * Vector插值
	 * @param from
	 * @param to
	 * @param count
	 * @param midVec
	 * @return
	 */
	static bool VecSlerp(const Vector3& from, const Vector3& to, int count,
			Vector3* midVec);
	/**
	 * 四元数插值
	 * @param from
	 * @param to
	 * @param count
	 * @param midRotation
	 * @return
	 */
	static bool RotationSlerp(const Quaternion& from, const Quaternion& to,
			int count, Quaternion* midRotation);
	/**
	 * 浮点数插值
	 * @param from
	 * @param to
	 * @param midScale
	 * @param interPos
	 * @return
	 */
	static bool FloatSlerp(float from, float to, float* midScale,
			float interPos);
	/**
	 * 向量插值
	 * @param from
	 * @param to
	 * @param midVec
	 * @param interPos
	 * @return
	 */
	static bool VecSlerp(const Vector3& from, const Vector3& to,
			Vector3* midVec, float interPos);

	/**
	 * 向量插值
	 * @param from
	 * @param to
	 * @param interPos
	 * @return
	 */
	static Vector3 VecSlerp(const Vector3& from, const Vector3& to,
			float interPos);
};
}
#endif /*M3D_TOOLS_H*/

