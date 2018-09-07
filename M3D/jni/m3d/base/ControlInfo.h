/**@file
 *@brief	控制矩阵 M3D为简化设计而构造，后续可能回被优化掉 TODO
 *@author	XL
 *@date		2014-1-7
 *@version	1.0
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 */

#ifndef   M3D_MVMATRIX_H_
#define	  M3D_MVMATRIX_H_

#include "m3d/base/Quaternion.h"
#include "m3d/base/Matrix4.h"

namespace M3D
{
/**
 * @class
 * @brief M3D简化设计，后续优化掉
 * 控制矩阵 M3D为简化设计而构造，后续可能回被优化掉
 * TODO
 *
 */
class M3D_API ControlInfo
{
public:
	/** 
	 * @brief
	 */
	ControlInfo();
	/** 
	 * @brief
	 */
	ControlInfo(float Ax, float Ay, float Az, float angle);
	/**
	 * @brief
	 */
	void ReSet();
	/** 
	 * @brief
	 */
	void SetScale(float scale);

	/** 
	 * @brief
	 */
	float GetScale();
	/** 
	 * @brief
	 */
	float GetFitFactor();
	/** 
	 * @brief
	 */
	void SetFitFactor(float facetor);
	/** 
	 * @brief
	 */
	void SetAspective(float aspective);
	/**
	 * @brief
	 */
	float GetAspective();
	/** 
	 * @brief
	 */
	void SetMoveVec(const Vector3& movVec);
	/** 
	 * @brief
	 */
	void SetRotate(const Quaternion& rot);

	float scaleFactor;

	Vector3 moveVector;

	Quaternion rotation;

	Vector3 currPos;
	Vector3 scaleVector;
private:

	float fitFactor;
	float aspective;
};
}

#endif /*M3D_MVMATRIX_H_*/
