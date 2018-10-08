/**
 *@file
 *@brief
 *@author	XL
 *@date		2015-2-26
 *@version	1.0
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 */
#ifndef M3D_VECTOR3_H_
#define M3D_VECTOR3_H_

#include "m3d/base/Vector2.h"

namespace M3D
{

/** 
 * @brief Three-dimensional vector. 
 */
class M3D_API Vector3
{
public:
	/**
	 * @brief Construct a zero vector.
	 */
	Vector3();

	/**
	 * @brief Copy-construct from another vector.
	 */
	Vector3(const Vector3& vector) :
			m_x(vector.m_x), m_y(vector.m_y), m_z(vector.m_z)
	{
	}

	/**
	 * @brief Construct from a two-dimensional vector and the Z coordinate.
	 */
	Vector3(const Vector2& vector, float z) :
			m_x(vector.m_x), m_y(vector.m_y), m_z(z)
	{
	}

	/**
	 * @brief Construct from a two-dimensional vector (for Urho2D).
	 */
	Vector3(const Vector2& vector) :
			m_x(vector.m_x), m_y(vector.m_y), m_z(0.0f)
	{
	}

	/**
	 * @brief Construct from coordinates.
	 */
	Vector3(float x, float y, float z) :
			m_x(x), m_y(y), m_z(z)
	{
	}

	/**
	 * @brief Construct from two-dimensional coordinates (for Urho2D).
	 */
	Vector3(float x, float y) :
			m_x(x), m_y(y), m_z(0.0f)
	{
	}

	/**
	 * @brief Construct from a float array.
	 */
	Vector3(const float* data) :
			m_x(data[0]), m_y(data[1]), m_z(data[2])
	{
	}

	/**
	 * @brief Assign from another vector.
	 */
	Vector3& operator =(const Vector3& rhs)
	{
		m_x = rhs.m_x;
		m_y = rhs.m_y;
		m_z = rhs.m_z;
		return *this;
	}

	/**
	 * @brief Test for equality with another vector without epsilon.
	 */
	bool operator ==(const Vector3& rhs) const
	{
		return m_x == rhs.m_x && m_y == rhs.m_y && m_z == rhs.m_z;
	}
	/**
	 * @brief Test for inequality with another vector without epsilon.
	 */
	bool operator !=(const Vector3& rhs) const
	{
		return m_x != rhs.m_x || m_y != rhs.m_y || m_z != rhs.m_z;
	}
	/**
	 * @brief Add a vector.
	 */
	Vector3 operator +(const Vector3& rhs) const
	{
		return Vector3(m_x + rhs.m_x, m_y + rhs.m_y, m_z + rhs.m_z);
	}
	/**
	 * @brief Return negation.
	 */
	Vector3 operator -() const
	{
		return Vector3(-m_x, -m_y, -m_z);
	}
	/**
	 * @brief Subtract a vector.
	 */
	Vector3 operator -(const Vector3& rhs) const
	{
		return Vector3(m_x - rhs.m_x, m_y - rhs.m_y, m_z - rhs.m_z);
	}
	/**
	 * @brief Multiply with a scalar.
	 */
	Vector3 operator *(float rhs) const
	{
		return Vector3(m_x * rhs, m_y * rhs, m_z * rhs);
	}
	/**
	 * @brief Multiply with a vector.
	 */
	Vector3 operator *(const Vector3& rhs) const
	{
		return Vector3(m_x * rhs.m_x, m_y * rhs.m_y, m_z * rhs.m_z);
	}
	/**
	 * @brief Divide by a scalar.
	 */
	Vector3 operator /(float rhs) const
	{
		return Vector3(m_x / rhs, m_y / rhs, m_z / rhs);
	}
	/**
	 * @brief Divide by a vector.
	 */
	Vector3 operator /(const Vector3& rhs) const
	{
		return Vector3(m_x / rhs.m_x, m_y / rhs.m_y, m_z / rhs.m_z);
	}

	/**
	 * @brief Add-assign a vector.
	 */
	Vector3& operator +=(const Vector3& rhs)
	{
		m_x += rhs.m_x;
		m_y += rhs.m_y;
		m_z += rhs.m_z;
		return *this;
	}

	/**
	 * @brief Subtract-assign a vector.
	 */
	Vector3& operator -=(const Vector3& rhs)
	{
		m_x -= rhs.m_x;
		m_y -= rhs.m_y;
		m_z -= rhs.m_z;
		return *this;
	}

	/**
	 * @brief Multiply-assign a scalar.
	 */
	Vector3& operator *=(float rhs)
	{
		m_x *= rhs;
		m_y *= rhs;
		m_z *= rhs;
		return *this;
	}

	/**
	 * @brief Multiply-assign a vector.
	 */
	Vector3& operator *=(const Vector3& rhs)
	{
		m_x *= rhs.m_x;
		m_y *= rhs.m_y;
		m_z *= rhs.m_z;
		return *this;
	}

	/**
	 * @brief Divide-assign a scalar.
	 */
	Vector3& operator /=(float rhs)
	{
		float invRhs = 1.0f / rhs;
		m_x *= invRhs;
		m_y *= invRhs;
		m_z *= invRhs;
		return *this;
	}

	/**
	 * @brief Divide-assign a vector.
	 */
	Vector3& operator /=(const Vector3& rhs)
	{
		m_x /= rhs.m_x;
		m_y /= rhs.m_y;
		m_z /= rhs.m_z;
		return *this;
	}

	/**
	 * @brief Normalize to unit length.
	 */
	void Normalize()
	{
		float lenSquared = LengthSquared();
		if (!M3D::Equals(lenSquared, 1.0f) && lenSquared > 0.0f)
		{
			float invLen = 1.0f / sqrtf(lenSquared);
			m_x *= invLen;
			m_y *= invLen;
			m_z *= invLen;
		}
	}

	/**
	 * @brief Return length.
	 */
	float Length() const
	{
		return sqrtf(m_x * m_x + m_y * m_y + m_z * m_z);
	}
	/**
	 * @brief Return squared length.
	 */
	float LengthSquared() const
	{
		return m_x * m_x + m_y * m_y + m_z * m_z;
	}
	/**
	 * @brief Calculate dot product.
	 */
	float DotProduct(const Vector3& rhs) const
	{
		return m_x * rhs.m_x + m_y * rhs.m_y + m_z * rhs.m_z;
	}
	/**
	 * @brief Calculate absolute dot product.
	 */
	float AbsDotProduct(const Vector3& rhs) const
	{
		return M3D::Abs(m_x * rhs.m_x) + M3D::Abs(m_y * rhs.m_y)
				+ M3D::Abs(m_z * rhs.m_z);
	}

	/**
	 * @brief Calculate cross product.
	 */
	Vector3 CrossProduct(const Vector3& rhs) const
	{
		return Vector3(m_y * rhs.m_z - m_z * rhs.m_y,
				m_z * rhs.m_x - m_x * rhs.m_z, m_x * rhs.m_y - m_y * rhs.m_x);
	}

	/**
	 * @brief Return absolute vector.
	 */
	Vector3 Abs() const
	{
		return Vector3(M3D::Abs(m_x), M3D::Abs(m_y), M3D::Abs(m_z));
	}
	/**
	 * @brief Linear interpolation with another vector.
	 */
	Vector3 Lerp(const Vector3& rhs, float t) const
	{
		return *this * (1.0f - t) + rhs * t;
	}
	/**
	 * @brief Test for equality with another vector with epsilon.
	 */
	bool Equals(const Vector3& rhs) const
	{
		return M3D::Equals(m_x, rhs.m_x) && M3D::Equals(m_y, rhs.m_y)
				&& M3D::Equals(m_z, rhs.m_z);
	}
	/**
	 * @brief Returns the angle between this vector and another vector in degrees.
	 */
	float Angle(const Vector3& rhs) const
	{
		return M3D::Acos(DotProduct(rhs) / (Length() * rhs.Length()));
	}
	/**
	 * @brief Return whether is NaN.
	 */
	bool IsNaN() const
	{
		return M3D::IsNaN(m_x) || M3D::IsNaN(m_y) || M3D::IsNaN(m_z);
	}
	bool IsZero() const
	{
		return (this->LengthSquared() < 0.00000001);
	}

	Vector3 Nagative() const
	{
		return *this * -1;
	}

	void ToZero()
	{
		*this = Vector3::ZERO;
	}

	void ToOne()
	{
		*this = Vector3::ONE;
	}
	/**
	 * @brief Return normalized to unit length.
	 */
	Vector3 Normalized() const
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
	 * @brief Return float data.
	 */
	const float* Data() const
	{
		return &m_x;
	}
	/**
	 * @brief Return as string.
	 */
	string Tostring() const;

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
	 * @brief Zero vector.
	 */
	static const Vector3 ZERO;
	/**
	 * @brief (-1,0,0) vector.
	 */
	static const Vector3 LEFT;
	/**
	 * @brief (1,0,0) vector.
	 */
	static const Vector3 RIGHT;
	/**
	 * @brief (0,1,0) vector.
	 */
	static const Vector3 UP;
	/**
	 * @brief (0,-1,0) vector.
	 */
	static const Vector3 DOWN;
	/**
	 * @brief (0,0,1) vector.
	 */
	static const Vector3 FORWARD;
	/**
	 * @brief (0,0,-1) vector.
	 */
	static const Vector3 BACK;
	/**
	 * @brief (1,1,1) vector.
	 */
	static const Vector3 ONE;

	static const Vector3 MINIMUM;

	static const Vector3 MAXMUN;
};

/** 
 * @brief Multiply Vector3 with a scalar. 
 */
inline Vector3 operator *(float lhs, const Vector3& rhs)
{
	return rhs * lhs;
}

}

#endif //VECTOR3_H
