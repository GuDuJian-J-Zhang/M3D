/**
 *@file
 *@brief
 *@author	XL
 *@date		2015-2-26
 *@version	1.0
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 */
#ifndef M3D_MATRIX3X4_H_
#define M3D_MATRIX3X4_H_

#include "m3d/base/Matrix4.h"

namespace M3D
{

/** 
 * @brief 3x4 matrix for scene node transform calculations. 
 */
class M3D_API Matrix3x4
{
public:
	/**
	 * @brief Construct an identity matrix.
	 */
	Matrix3x4() :
			m_m00(1.0f), m_m01(0.0f), m_m02(0.0f), m_m03(0.0f), m_m10(0.0f), m_m11(
					1.0f), m_m12(0.0f), m_m13(0.0f), m_m20(0.0f), m_m21(0.0f), m_m22(
					1.0f), m_m23(0.0f)
	{
	}

	/**
	 * @brief Copy-construct from another matrix.
	 */
	Matrix3x4(const Matrix3x4& matrix) :
			m_m00(matrix.m_m00), m_m01(matrix.m_m01), m_m02(matrix.m_m02), m_m03(
					matrix.m_m03), m_m10(matrix.m_m10), m_m11(matrix.m_m11), m_m12(
					matrix.m_m12), m_m13(matrix.m_m13), m_m20(matrix.m_m20), m_m21(
					matrix.m_m21), m_m22(matrix.m_m22), m_m23(matrix.m_m23)
	{
	}

	/**
	 * @brief Copy-construct from a 3x3 matrix and set the extra elements to identity.
	 */
	Matrix3x4(const Matrix3& matrix) :
			m_m00(matrix.m_m00), m_m01(matrix.m_m01), m_m02(matrix.m_m02), m_m03(0.0f), m_m10(
					matrix.m_m10), m_m11(matrix.m_m11), m_m12(matrix.m_m12), m_m13(
					0.0f), m_m20(matrix.m_m20), m_m21(matrix.m_m21), m_m22(
					matrix.m_m22), m_m23(0.0f)
	{
	}

	/**
	 * @brief Copy-construct from a 4x4 matrix which is assumed to contain no projection.
	 */
	Matrix3x4(const Matrix4& matrix) :
			m_m00(matrix.m_m00), m_m01(matrix.m_m01), m_m02(matrix.m_m02), m_m03(
					matrix.m_m03), m_m10(matrix.m_m10), m_m11(matrix.m_m11), m_m12(
					matrix.m_m12), m_m13(matrix.m_m13), m_m20(matrix.m_m20), m_m21(
					matrix.m_m21), m_m22(matrix.m_m22), m_m23(matrix.m_m23)
	{
	}

	/** 
	 * @brief  Construct from values.
	 */
	Matrix3x4(float v00, float v01, float v02, float v03, float v10, float v11,
			float v12, float v13, float v20, float v21, float v22, float v23) :
			m_m00(v00), m_m01(v01), m_m02(v02), m_m03(v03), m_m10(v10), m_m11(v11), m_m12(
					v12), m_m13(v13), m_m20(v20), m_m21(v21), m_m22(v22), m_m23(v23)
	{
	}

	/**
	 * @brief Construct from a float array.
	 */
	Matrix3x4(const float* data) :
			m_m00(data[0]), m_m01(data[1]), m_m02(data[2]), m_m03(data[3]), m_m10(
					data[4]), m_m11(data[5]), m_m12(data[6]), m_m13(data[7]), m_m20(
					data[8]), m_m21(data[9]), m_m22(data[10]), m_m23(data[11])
	{
	}

	/**
	 * @brief Construct from translation, rotation and uniform scale.
	 */
	Matrix3x4(const Vector3& translation, const Quaternion& rotation,
			float scale);
	/**
	 * @brief Construct from translation, rotation and nonuniform scale.
	 */
	Matrix3x4(const Vector3& translation, const Quaternion& rotation,
			const Vector3& scale);

	/**
	 * @brief Assign from another matrix.
	 */
	Matrix3x4& operator =(const Matrix3x4& rhs)
	{
		m_m00 = rhs.m_m00;
		m_m01 = rhs.m_m01;
		m_m02 = rhs.m_m02;
		m_m03 = rhs.m_m03;
		m_m10 = rhs.m_m10;
		m_m11 = rhs.m_m11;
		m_m12 = rhs.m_m12;
		m_m13 = rhs.m_m13;
		m_m20 = rhs.m_m20;
		m_m21 = rhs.m_m21;
		m_m22 = rhs.m_m22;
		m_m23 = rhs.m_m23;
		return *this;
	}

	/**
	 * @brief Assign from a 3x3 matrix and set the extra elements to identity.
	 */
	Matrix3x4& operator =(const Matrix3& rhs)
	{
		m_m00 = rhs.m_m00;
		m_m01 = rhs.m_m01;
		m_m02 = rhs.m_m02;
		m_m03 = 0.0;
		m_m10 = rhs.m_m10;
		m_m11 = rhs.m_m11;
		m_m12 = rhs.m_m12;
		m_m13 = 0.0;
		m_m20 = rhs.m_m20;
		m_m21 = rhs.m_m21;
		m_m22 = rhs.m_m22;
		m_m23 = 0.0;
		return *this;
	}

	/**
	 * @brief Assign from a 4x4 matrix which is assumed to contain no projection.
	 */
	Matrix3x4& operator =(const Matrix4& rhs)
	{
		m_m00 = rhs.m_m00;
		m_m01 = rhs.m_m01;
		m_m02 = rhs.m_m02;
		m_m03 = rhs.m_m03;
		m_m10 = rhs.m_m10;
		m_m11 = rhs.m_m11;
		m_m12 = rhs.m_m12;
		m_m13 = rhs.m_m13;
		m_m20 = rhs.m_m20;
		m_m21 = rhs.m_m21;
		m_m22 = rhs.m_m22;
		m_m23 = rhs.m_m23;
		return *this;
	}

	/**
	 * @brief Test for equality with another matrix without epsilon.
	 */
	bool operator ==(const Matrix3x4& rhs) const
	{
		const float* leftData = Data();
		const float* rightData = rhs.Data();

		for (unsigned i = 0; i < 12; ++i)
		{
			if (leftData[i] != rightData[i])
				return false;
		}

		return true;
	}

	/**
	 * @brief Test for inequality with another matrix without epsilon.
	 */
	bool operator !=(const Matrix3x4& rhs) const
	{
		return !(*this == rhs);
	}

	/**
	 * @brief Multiply a Vector3 which is assumed to represent position.
	 */
	Vector3 operator *(const Vector3& rhs) const
	{
		return Vector3(
				(m_m00 * rhs.m_x + m_m01 * rhs.m_y + m_m02 * rhs.m_z + m_m03),
				(m_m10 * rhs.m_x + m_m11 * rhs.m_y + m_m12 * rhs.m_z + m_m13),
				(m_m20 * rhs.m_x + m_m21 * rhs.m_y + m_m22 * rhs.m_z + m_m23));
	}

	/**
	 * @brief Multiply a Vector4.
	 */
	Vector3 operator *(const Vector4& rhs) const
	{
		return Vector3(
				(m_m00 * rhs.m_x + m_m01 * rhs.m_y + m_m02 * rhs.m_z
						+ m_m03 * rhs.m_w),
				(m_m10 * rhs.m_x + m_m11 * rhs.m_y + m_m12 * rhs.m_z
						+ m_m13 * rhs.m_w),
				(m_m20 * rhs.m_x + m_m21 * rhs.m_y + m_m22 * rhs.m_z
						+ m_m23 * rhs.m_w));
	}

	/**
	 * @brief Add a matrix.
	 */
	Matrix3x4 operator +(const Matrix3x4& rhs) const
	{
		return Matrix3x4(m_m00 + rhs.m_m00, m_m01 + rhs.m_m01, m_m02 + rhs.m_m02,
				m_m03 + rhs.m_m03, m_m10 + rhs.m_m10, m_m11 + rhs.m_m11,
				m_m12 + rhs.m_m12, m_m13 + rhs.m_m13, m_m20 + rhs.m_m20,
				m_m21 + rhs.m_m21, m_m22 + rhs.m_m22, m_m23 + rhs.m_m23);
	}

	/**
	 * @brief Subtract a matrix.
	 */
	Matrix3x4 operator -(const Matrix3x4& rhs) const
	{
		return Matrix3x4(m_m00 - rhs.m_m00, m_m01 - rhs.m_m01, m_m02 - rhs.m_m02,
				m_m03 - rhs.m_m03, m_m10 - rhs.m_m10, m_m11 - rhs.m_m11,
				m_m12 - rhs.m_m12, m_m13 - rhs.m_m13, m_m20 - rhs.m_m20,
				m_m21 - rhs.m_m21, m_m22 - rhs.m_m22, m_m23 - rhs.m_m23);
	}

	/**
	 * @brief Multiply with a scalar.
	 */
	Matrix3x4 operator *(float rhs) const
	{
		return Matrix3x4(m_m00 * rhs, m_m01 * rhs, m_m02 * rhs, m_m03 * rhs,
				m_m10 * rhs, m_m11 * rhs, m_m12 * rhs, m_m13 * rhs, m_m20 * rhs,
				m_m21 * rhs, m_m22 * rhs, m_m23 * rhs);
	}

	/**
	 * @brief Multiply a matrix.
	 */
	Matrix3x4 operator *(const Matrix3x4& rhs) const
	{
		return Matrix3x4(m_m00 * rhs.m_m00 + m_m01 * rhs.m_m10 + m_m02 * rhs.m_m20,
				m_m00 * rhs.m_m01 + m_m01 * rhs.m_m11 + m_m02 * rhs.m_m21,
				m_m00 * rhs.m_m02 + m_m01 * rhs.m_m12 + m_m02 * rhs.m_m22,
				m_m00 * rhs.m_m03 + m_m01 * rhs.m_m13 + m_m02 * rhs.m_m23 + m_m03,
				m_m10 * rhs.m_m00 + m_m11 * rhs.m_m10 + m_m12 * rhs.m_m20,
				m_m10 * rhs.m_m01 + m_m11 * rhs.m_m11 + m_m12 * rhs.m_m21,
				m_m10 * rhs.m_m02 + m_m11 * rhs.m_m12 + m_m12 * rhs.m_m22,
				m_m10 * rhs.m_m03 + m_m11 * rhs.m_m13 + m_m12 * rhs.m_m23 + m_m13,
				m_m20 * rhs.m_m00 + m_m21 * rhs.m_m10 + m_m22 * rhs.m_m20,
				m_m20 * rhs.m_m01 + m_m21 * rhs.m_m11 + m_m22 * rhs.m_m21,
				m_m20 * rhs.m_m02 + m_m21 * rhs.m_m12 + m_m22 * rhs.m_m22,
				m_m20 * rhs.m_m03 + m_m21 * rhs.m_m13 + m_m22 * rhs.m_m23 + m_m23);
	}

	/**
	 * @brief Multiply a 4x4 matrix.
	 */
	Matrix4 operator *(const Matrix4& rhs) const
	{
		return Matrix4(
				m_m00 * rhs.m_m00 + m_m01 * rhs.m_m10 + m_m02 * rhs.m_m20
						+ m_m03 * rhs.m_m30,
				m_m00 * rhs.m_m01 + m_m01 * rhs.m_m11 + m_m02 * rhs.m_m21
						+ m_m03 * rhs.m_m31,
				m_m00 * rhs.m_m02 + m_m01 * rhs.m_m12 + m_m02 * rhs.m_m22
						+ m_m03 * rhs.m_m32,
				m_m00 * rhs.m_m03 + m_m01 * rhs.m_m13 + m_m02 * rhs.m_m23
						+ m_m03 * rhs.m_m33,
				m_m10 * rhs.m_m00 + m_m11 * rhs.m_m10 + m_m12 * rhs.m_m20
						+ m_m13 * rhs.m_m30,
				m_m10 * rhs.m_m01 + m_m11 * rhs.m_m11 + m_m12 * rhs.m_m21
						+ m_m13 * rhs.m_m31,
				m_m10 * rhs.m_m02 + m_m11 * rhs.m_m12 + m_m12 * rhs.m_m22
						+ m_m13 * rhs.m_m32,
				m_m10 * rhs.m_m03 + m_m11 * rhs.m_m13 + m_m12 * rhs.m_m23
						+ m_m13 * rhs.m_m33,
				m_m20 * rhs.m_m00 + m_m21 * rhs.m_m10 + m_m22 * rhs.m_m20
						+ m_m23 * rhs.m_m30,
				m_m20 * rhs.m_m01 + m_m21 * rhs.m_m11 + m_m22 * rhs.m_m21
						+ m_m23 * rhs.m_m31,
				m_m20 * rhs.m_m02 + m_m21 * rhs.m_m12 + m_m22 * rhs.m_m22
						+ m_m23 * rhs.m_m32,
				m_m20 * rhs.m_m03 + m_m21 * rhs.m_m13 + m_m22 * rhs.m_m23
						+ m_m23 * rhs.m_m33, rhs.m_m30, rhs.m_m31, rhs.m_m32,
				rhs.m_m33);
	}

	/**
	 * @brief Set translation elements.
	 */
	void SetTranslation(const Vector3& translation)
	{
		m_m03 = translation.m_x;
		m_m13 = translation.m_y;
		m_m23 = translation.m_z;
	}

	/**
	 * @brief Set rotation elements from a 3x3 matrix.
	 */
	void SetRotation(const Matrix3& rotation)
	{
		m_m00 = rotation.m_m00;
		m_m01 = rotation.m_m01;
		m_m02 = rotation.m_m02;
		m_m10 = rotation.m_m10;
		m_m11 = rotation.m_m11;
		m_m12 = rotation.m_m12;
		m_m20 = rotation.m_m20;
		m_m21 = rotation.m_m21;
		m_m22 = rotation.m_m22;
	}

	/**
	 * @brief Set scaling elements.
	 */
	void SetScale(const Vector3& scale)
	{
		m_m00 = scale.m_x;
		m_m11 = scale.m_y;
		m_m22 = scale.m_z;
	}

	/**
	 * @brief Set uniform scaling elements.
	 */
	void SetScale(float scale)
	{
		m_m00 = scale;
		m_m11 = scale;
		m_m22 = scale;
	}

	/**
	 * @brief Return the combined rotation and scaling matrix.
	 */
	Matrix3 ToMatrix3() const
	{
		return Matrix3(m_m00, m_m01, m_m02, m_m10, m_m11, m_m12, m_m20, m_m21, m_m22);
	}

	/**
	 * @brief Convert to a 4x4 matrix by filling in an identity last row.
	 */
	Matrix4 ToMatrix4() const
	{
		return Matrix4(m_m00, m_m01, m_m02, m_m03, m_m10, m_m11, m_m12, m_m13, m_m20,
				m_m21, m_m22, m_m23, 0.0f, 0.0f, 0.0f, 1.0f);
	}

	/**
	 * @brief Return the rotation matrix with scaling removed.
	 */
	Matrix3 RotationMatrix() const
	{
		Vector3 invScale(1.0f / sqrtf(m_m00 * m_m00 + m_m10 * m_m10 + m_m20 * m_m20),
				1.0f / sqrtf(m_m01 * m_m01 + m_m11 * m_m11 + m_m21 * m_m21),
				1.0f / sqrtf(m_m02 * m_m02 + m_m12 * m_m12 + m_m22 * m_m22));

		return ToMatrix3().Scaled(invScale);
	}

	/**
	 * @brief Return the translation part.
	 */
	Vector3 Translation() const
	{
		return Vector3(m_m03, m_m13, m_m23);
	}

	/**
	 * @brief Return the rotation part.
	 */
	Quaternion Rotation() const
	{
		return Quaternion(RotationMatrix());
	}

	/**
	 * @brief Return the scaling part.
	 */
	Vector3 Scale() const
	{
		return Vector3(sqrtf(m_m00 * m_m00 + m_m10 * m_m10 + m_m20 * m_m20),
				sqrtf(m_m01 * m_m01 + m_m11 * m_m11 + m_m21 * m_m21),
				sqrtf(m_m02 * m_m02 + m_m12 * m_m12 + m_m22 * m_m22));
	}

	void MultiTranslate(const Vector3& mov);

	void MultiRotatiton(const Quaternion& rotation);

	void MultiScale(float scale);

	void MultiScale(const Vector3& scale);

	/**
	 * @brief Test for equality with another matrix with epsilon.
	 */
	bool Equals(const Matrix3x4& rhs) const
	{
		const float* leftData = Data();
		const float* rightData = rhs.Data();

		for (unsigned i = 0; i < 12; ++i)
		{
			if (!M3D::Equals(leftData[i], rightData[i]))
				return false;
		}

		return true;
	}

	/**
	 * @brief Return decomposition to translation, rotation and scale.
	 */
	void Decompose(Vector3& translation, Quaternion& rotation,
			Vector3& scale) const;
	/**
	 * @brief Return inverse.
	 */
	Matrix3x4 Inverse() const;

	/**
	 * @brief Return float data.
	 */
	const float* Data() const
	{
		return &m_m00;
	}
	/**
	 * @brief Return as string.
	 */
	string ToString() const;

	float m_m00; //!< 数据
	float m_m01;
	float m_m02;
	float m_m03;
	float m_m10;
	float m_m11;
	float m_m12;
	float m_m13;
	float m_m20;
	float m_m21;
	float m_m22;
	float m_m23;

	/**
	 * @brief Zero matrix.
	 */
	static const Matrix3x4 ZERO;
	/**
	 * @brief Identity matrix.
	 */
	static const Matrix3x4 IDENTITY;
};

/** 
 * @brief Multiply a 3x4 matrix with a scalar. 
 */
inline Matrix3x4 operator *(float lhs, const Matrix3x4& rhs)
{
	return rhs * lhs;
}

}
#endif //MATRIX4X3_H
