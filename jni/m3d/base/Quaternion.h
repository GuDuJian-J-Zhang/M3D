/**
 *@file
 *@brief
 *@author	XL
 *@date		2015-2-26
 *@version	1.0
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 */
#ifndef M3D_QUATERNION_H_
#define M3D_QUATERNION_H_

#include "m3d/base/Matrix3.h"

namespace M3D
{

/** 
 * @brief Rotation represented as a four-dimensional normalized vector. 
 */
class M3D_API Quaternion
{
public:
	/**
	 * @brief Construct an identity quaternion.
	 */
	Quaternion() :
			m_w(1.0f), m_x(0.0f), m_y(0.0f), m_z(0.0f)
	{
	}

	/**
	 * @brief Copy-construct from another quaternion.
	 */
	Quaternion(const Quaternion& quat) :
			m_w(quat.m_w), m_x(quat.m_x), m_y(quat.m_y), m_z(quat.m_z)
	{
	}

	/**
	 * @brief Construct from values.
	 */
	Quaternion(float w, float x, float y, float z) :
			m_w(w), m_x(x), m_y(y), m_z(z)
	{
	}

	/**
	 * @brief Construct from a float array.
	 */
	Quaternion(const float* data) :
			m_w(data[0]), m_x(data[1]), m_y(data[2]), m_z(data[3])
	{
	}

	/**
	 * @brief Construct from an angle (in degrees) and axis.
	 */
	Quaternion(float angle, const Vector3& axis)
	{
		FromAngleAxis(angle, axis);
	}

	/**
	 * @brief Construct from an angle (in degrees, for M3D).
	 */
	Quaternion(float angle)
	{
		FromAngleAxis(angle, Vector3::FORWARD);
	}

	/**
	 * @brief Construct from Euler angles (in degrees.)
	 */
	Quaternion(float x, float y, float z)
	{
		FromEulerAngles(x, y, z);
	}

	/**
	 * @brief Construct from the rotation difference between two direction vectors.
	 */
	Quaternion(const Vector3& start, const Vector3& end)
	{
		FromRotationTo(start, end);
	}

	/**
	 * @brief Construct from orthonormal axes.
	 */
	Quaternion(const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis)
	{
		FromAxes(xAxis, yAxis, zAxis);
	}

	/**
	 * @brief Construct from a rotation matrix.
	 */
	Quaternion(const Matrix3& matrix)
	{
		FromRotationMatrix(matrix);
	}

	/**
	 * @brief Assign from another quaternion.
	 */
	Quaternion& operator =(const Quaternion& rhs)
	{
		m_w = rhs.m_w;
		m_x = rhs.m_x;
		m_y = rhs.m_y;
		m_z = rhs.m_z;
		return *this;
	}

	/**
	 * @brief Add-assign a quaternion.
	 */
	Quaternion& operator +=(const Quaternion& rhs)
	{
		m_w += rhs.m_w;
		m_x += rhs.m_x;
		m_y += rhs.m_y;
		m_z += rhs.m_z;
		return *this;
	}

	/**
	 * @brief Multiply-assign a scalar.
	 */
	Quaternion& operator *=(float rhs)
	{
		m_w *= rhs;
		m_x *= rhs;
		m_y *= rhs;
		m_z *= rhs;
		return *this;
	}

	/**
	 * @brief Test for equality with another quaternion without epsilon.
	 */
	bool operator ==(const Quaternion& rhs) const
	{
		return m_w == rhs.m_w && m_x == rhs.m_x && m_y == rhs.m_y
				&& m_z == rhs.m_z;
	}
	/**
	 * @brief Test for inequality with another quaternion without epsilon.
	 */
	bool operator !=(const Quaternion& rhs) const
	{
		return m_w != rhs.m_w || m_x != rhs.m_x || m_y != rhs.m_y
				|| m_z != rhs.m_z;
	}
	/**
	 * @brief Multiply with a scalar.
	 */
	Quaternion operator *(float rhs) const
	{
		return Quaternion(m_w * rhs, m_x * rhs, m_y * rhs, m_z * rhs);
	}
	/**
	 * @brief Return negation.
	 */
	Quaternion operator -() const
	{
		return Quaternion(-m_w, -m_x, -m_y, -m_z);
	}
	/**
	 * @brief Add a quaternion.
	 */
	Quaternion operator +(const Quaternion& rhs) const
	{
		return Quaternion(m_w + rhs.m_w, m_x + rhs.m_x, m_y + rhs.m_y,
				m_z + rhs.m_z);
	}
	/**
	 * @brief Subtract a quaternion.
	 */
	Quaternion operator -(const Quaternion& rhs) const
	{
		return Quaternion(m_w - rhs.m_w, m_x - rhs.m_x, m_y - rhs.m_y,
				m_z - rhs.m_z);
	}

	/**
	 * @brief Multiply a quaternion.
	 */
	Quaternion operator *(const Quaternion& rhs) const
	{
		return Quaternion(
				m_w * rhs.m_w - m_x * rhs.m_x - m_y * rhs.m_y - m_z * rhs.m_z,
				m_w * rhs.m_x + m_x * rhs.m_w + m_y * rhs.m_z - m_z * rhs.m_y,
				m_w * rhs.m_y + m_y * rhs.m_w + m_z * rhs.m_x - m_x * rhs.m_z,
				m_w * rhs.m_z + m_z * rhs.m_w + m_x * rhs.m_y - m_y * rhs.m_x);
	}

	/**
	 * @brief Multiply a Vector3.
	 */
	Vector3 operator *(const Vector3& rhs) const
	{
		Vector3 qVec(m_x, m_y, m_z);
		Vector3 cross1(qVec.CrossProduct(rhs));
		Vector3 cross2(qVec.CrossProduct(cross1));

		return rhs + 2.0f * (cross1 * m_w + cross2);
	}

	bool IsZero() const;
	M3D_ADDRESSTYPE Address()
	{
		//获取地址的真实方式
		return TAddress(*this);
	}

	void FillByAddress(M3D_ADDRESSTYPE memoryAddress)
	{
		*this = *TFromAddress<Quaternion>(memoryAddress);
	}
	/**
	 * @brief Define from an angle (in degrees) and axis.
	 */
	void FromAngleAxis(float angle, const Vector3& axis);
	/**
	 * @brief Define from Euler angles (in degrees.)
	 */
	void FromEulerAngles(float x, float y, float z);
	/**
	 * @brief Define from the rotation difference between two direction vectors.
	 */
	void FromRotationTo(const Vector3& start, const Vector3& end);
	/**
	 * @brief Define from orthonormal axes.
	 */
	void FromAxes(const Vector3& xAxis, const Vector3& yAxis,
			const Vector3& zAxis);
	/**
	 * @brief Define from a rotation matrix.
	 */
	void FromRotationMatrix(const Matrix3& matrix);
	/**
	 * @brief Define from a direction to look in and an up direction. Return true if successful, or false if would result in a NaN, in which case the current value remains.
	 */
	bool FromLookRotation(const Vector3& direction, const Vector3& up =
			Vector3::UP);

	/**
	 * @brief Normalize to unit length.
	 */
	void Normalize()
	{
		float lenSquared = LengthSquared();
		if (!M3D::Equals(lenSquared, 1.0f) && lenSquared > 0.0f)
		{
			float invLen = 1.0f / sqrtf(lenSquared);
			m_w *= invLen;
			m_x *= invLen;
			m_y *= invLen;
			m_z *= invLen;
		}
	}
	void ToZero()
	{
		*this = Quaternion::IDENTITY;
	}
	/**
	 * @brief Return normalized to unit length.
	 */
	Quaternion Normalized() const
	{
		float lenSquared = LengthSquared();
		if (!M3D::Equals(lenSquared, 1.0f) && lenSquared > 0.0f)
		{
			float invLen = 1.0f / sqrtf(lenSquared);
			return *this * invLen;
		}
		else
			return *this;
	}

	/**
	 * @brief Return inverse.
	 */
	Quaternion Inverse() const
	{
		float lenSquared = LengthSquared();
		if (lenSquared == 1.0f)
			return Conjugate();
		else if (lenSquared >= M_EPSILON)
			return Conjugate() * (1.0f / lenSquared);
		else
			return IDENTITY;
	}

	/**
	 * @brief Return squared length.
	 */
	float LengthSquared() const
	{
		return m_w * m_w + m_x * m_x + m_y * m_y + m_z * m_z;
	}
	/**
	 * @brief Calculate dot product.
	 */
	float DotProduct(const Quaternion& rhs) const
	{
		return m_w * rhs.m_w + m_x * rhs.m_x + m_y * rhs.m_y + m_z * rhs.m_z;
	}
	/**
	 * @brief Test for equality with another quaternion with epsilon.
	 */
	bool Equals(const Quaternion& rhs) const
	{
		return M3D::Equals(m_w, rhs.m_w) && M3D::Equals(m_x, rhs.m_x)
				&& M3D::Equals(m_y, rhs.m_y) && M3D::Equals(m_z, rhs.m_z);
	}
	/**
	 * @brief Return whether is NaN.
	 */
	bool IsNaN() const
	{
		return M3D::IsNaN(m_w) || M3D::IsNaN(m_x) || M3D::IsNaN(m_y)
				|| M3D::IsNaN(m_z);
	}
	/**
	 * @brief Return conjugate.
	 */
	Quaternion Conjugate() const
	{
		return Quaternion(m_w, -m_x, -m_y, -m_z);
	}

	/**
	 * @brief Return Euler angles in degrees.
	 */
	Vector3 EulerAngles() const;
	/**
	 * @brief Return yaw angle in degrees.
	 */
	float YawAngle() const;
	/**
	 * @brief Return pitch angle in degrees.
	 */
	float PitchAngle() const;
	/**
	 * @brief Return roll angle in degrees.
	 */
	float RollAngle() const;
	/**
	 * @brief Return the rotation matrix that corresponds to this quaternion.
	 */
	Matrix3 RotationMatrix() const;
	/**
	 * @brief Spherical interpolation with another quaternion.
	 */
	Quaternion Slerp(Quaternion rhs, float t) const;
	/**
	 * @brief Normalized linear interpolation with another quaternion.
	 */
	Quaternion Nlerp(Quaternion rhs, float t, bool shortestPath = false) const;
	/**
	 * @brief Return float data.
	 */
	const float* Data() const
	{
		return &m_w;
	}

	/**
	 * @brief Return as string.
	 */
	string Tostring() const;

	/**
	* @brief Return as axis and angle.
	*/
	void GetValue(Vector3& axis, float & fAngle) const;
	Vector3 GetAixsValue() const;
	float GetAngleValue() const;
	/**
	 * @brief W coordinate.
	 */
	float m_w;
	/**
	 * @brief X coordinate.
	 */
	float m_x;
	/**
	 * @brief Y coordinate.
	 */
	float m_y;
	/**
	 * @brief Z coordinate.
	 */
	float m_z;

	/**
	 * @brief Identity quaternion.
	 */
	static const Quaternion IDENTITY;
};

}
#endif //QUATERNION_H
