/**
 *@file
 *@brief
 *@author	XL
 *@date		2015-2-26
 *@version	1.0
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 */
#ifndef M3D_MATRIX3_H_
#define M3D_MATRIX3_H_

#include "m3d/base/Vector3.h"

namespace M3D
{

/** 
 * @brief 3x3 matrix for rotation and scaling. 
 */
class M3D_API Matrix3
{
public:
	/**
	 * @brief Construct an identity matrix.
	 */
	Matrix3();

	/**
	 * @brief Copy-construct from another matrix.
	 */
	Matrix3(const Matrix3& matrix);

	/**
	 * @brief Construct from values.
	 */
	Matrix3(float v00, float v01, float v02, float v10, float v11, float v12,
			float v20, float v21, float v22);

	/**
	 * @brief Construct from a float array.
	 */
	Matrix3(const float* data);

	/**
	 * @brief Assign from another matrix.
	 */
	Matrix3& operator =(const Matrix3& rhs);

	/**
	 * @brief Test for equality with another matrix without epsilon.
	 */
	bool operator ==(const Matrix3& rhs) const;

	/**
	 * @brief Test for inequality with another matrix without epsilon.
	 */
	bool operator !=(const Matrix3& rhs) const
	{
		return !(*this == rhs);
	}

	/**
	 * @brief Multiply a Vector3.
	 */
	Vector3 operator *(const Vector3& rhs) const;
	Vector3 Multiply(const Vector3& rhs) const;

	/**
	 * @brief Add a matrix.
	 */
	Matrix3 operator +(const Matrix3& rhs) const;
	Matrix3 Add(const Matrix3& rhs) const;

	/**
	 * @brief Subtract a matrix.
	 */
	Matrix3 operator -(const Matrix3& rhs) const;
	Matrix3 Sub(const Matrix3& rhs) const;

	/**
	 * @brief Multiply with a scalar.
	 */
	Matrix3 operator *(float rhs) const;
	Matrix3 Multiply(float rhs) const;

	/**
	 * @brief Multiply a matrix.
	 */
	Matrix3 operator *(const Matrix3& rhs) const;
	Matrix3 Multiply(const Matrix3& rhs) const;

	/**
	 * @brief Set scaling elements.
	 */
	void SetScale(const Vector3& scale);

	/**
	 * @brief Set uniform scaling elements.
	 */
	void SetScale(float scale);

	/**
	 * @brief Return the scaling part.
	 */
	Vector3 Scale() const;

	/**
	 * @brief Return transpose.
	 */
	Matrix3 Transpose() const;

	/**
	 * @brief Return scaled by a vector.
	 */
	Matrix3 Scaled(const Vector3& scale) const;

	/**
	 * @brief Test for equality with another matrix with epsilon.
	 */
	bool Equals(const Matrix3& rhs) const;

	/**
	 * @brief Return inverse.
	 */
	Matrix3 Inverse() const;

	/**
	 * @brief Return float data.
	 */
	const float* Data() const;
	/**
	 * @brief Return as string.
	 */
	string Tostring() const;

	/**
	 * @brief Bulk transpose matrices.
	 */
	static void BulkTranspose(float* dest, const float* src, unsigned count);
	M3D_ADDRESSTYPE Address()
	{
		//获取地址的真实方式
		return TAddress(*this);
	}	

	void FillByAddress(M3D_ADDRESSTYPE memoryAddress)
	{
		*this = *TFromAddress<Matrix3>(memoryAddress);
	}
	float m_m00; //!< 数据
	float m_m01;
	float m_m02;
	float m_m10;
	float m_m11;
	float m_m12;
	float m_m20;
	float m_m21;
	float m_m22;

	/**
	 * @brief Zero matrix.
	 */
	static const Matrix3 ZERO;
	/**
	 * @brief Identity matrix.
	 */
	static const Matrix3 IDENTITY;
};

/** 
 * @brief Multiply a 3x3 matrix with a scalar. 
 */
inline Matrix3 operator *(float lhs, const Matrix3& rhs)
{
	return rhs * lhs;
}

}

#endif //M3D_MATRIX3_H_
