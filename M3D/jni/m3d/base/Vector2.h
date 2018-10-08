/**
 *@file
 *@brief
 *@author	XL
 *@date		2015-2-26
 *@version	1.0
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 */
#ifndef M3D_VECTOR2_H_
#define M3D_VECTOR2_H_

#include "m3d/base/MathDefs.h"
#include "m3d/M3D.h"

namespace M3D
{

/** 
 * @brief Two-dimensional vector. 
 */
class M3D_API Vector2
{
public:
	/**
	 * @brief Construct a zero vector.
	 */
	Vector2();

	/**
	 * @brief Copy-construct from another vector.
	 */
	Vector2(const Vector2& vector) :
			m_x(vector.m_x), m_y(vector.m_y)
	{
	}

	/**
	 * @brief Construct from coordinates.
	 */
	Vector2(float x, float y) :
			m_x(x), m_y(y)
	{
	}

	/**
	 * @brief Construct from a float array.
	 */
	Vector2(const float* data) :
			m_x(data[0]), m_y(data[1])
	{
	}

	/**
	 * @brief Assign from another vector.
	 */
	Vector2& operator =(const Vector2& rhs)
	{
		m_x = rhs.m_x;
		m_y = rhs.m_y;
		return *this;
	}

	/**
	 * @brief Test for equality with another vector without epsilon.
	 */
	bool operator ==(const Vector2& rhs) const
	{
		return m_x == rhs.m_x && m_y == rhs.m_y;
	}
	/**
	 * @brief Test for inequality with another vector without epsilon.
	 */
	bool operator !=(const Vector2& rhs) const
	{
		return m_x != rhs.m_x || m_y != rhs.m_y;
	}
	/**
	 * @brief Add a vector.
	 */
	Vector2 operator +(const Vector2& rhs) const
	{
		return Vector2(m_x + rhs.m_x, m_y + rhs.m_y);
	}
	/**
	 * @brief Return negation.
	 */
	Vector2 operator -() const
	{
		return Vector2(-m_x, -m_y);
	}
	/**
	 * @brief Subtract a vector.
	 */
	Vector2 operator -(const Vector2& rhs) const
	{
		return Vector2(m_x - rhs.m_x, m_y - rhs.m_y);
	}
	/**
	 * @brief Multiply with a scalar.
	 */
	Vector2 operator *(float rhs) const
	{
		return Vector2(m_x * rhs, m_y * rhs);
	}
	/**
	 * @brief Multiply with a vector.
	 */
	Vector2 operator *(const Vector2& rhs) const
	{
		return Vector2(m_x * rhs.m_x, m_y * rhs.m_y);
	}
	/**
	 * @brief Divide by a scalar.
	 */
	Vector2 operator /(float rhs) const
	{
		return Vector2(m_x / rhs, m_y / rhs);
	}
	/**
	 * @brief Divide by a vector.
	 */
	Vector2 operator /(const Vector2& rhs) const
	{
		return Vector2(m_x / rhs.m_x, m_y / rhs.m_y);
	}

	/**
	 * @brief Add-assign a vector.
	 */
	Vector2& operator +=(const Vector2& rhs)
	{
		m_x += rhs.m_x;
		m_y += rhs.m_y;
		return *this;
	}

	/**
	 * @brief Subtract-assign a vector.
	 */
	Vector2& operator -=(const Vector2& rhs)
	{
		m_x -= rhs.m_x;
		m_y -= rhs.m_y;
		return *this;
	}

	/**
	 * @brief Multiply-assign a scalar.
	 */
	Vector2& operator *=(float rhs)
	{
		m_x *= rhs;
		m_y *= rhs;
		return *this;
	}

	/**
	 * @brief Multiply-assign a vector.
	 */
	Vector2& operator *=(const Vector2& rhs)
	{
		m_x *= rhs.m_x;
		m_y *= rhs.m_y;
		return *this;
	}

	/**
	 * @brief Divide-assign a scalar.
	 */
	Vector2& operator /=(float rhs)
	{
		float invRhs = 1.0f / rhs;
		m_x *= invRhs;
		m_y *= invRhs;
		return *this;
	}

	/**
	 * @brief Divide-assign a vector.
	 */
	Vector2& operator /=(const Vector2& rhs)
	{
		m_x /= rhs.m_x;
		m_y /= rhs.m_y;
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
		}
	}

	/**
	 * @brief Return length.
	 */
	float Length() const
	{
		return sqrtf(m_x * m_x + m_y * m_y);
	}
	/**
	 * @brief Return squared length.
	 */
	float LengthSquared() const
	{
		return m_x * m_x + m_y * m_y;
	}
	/**
	 * @brief Calculate dot product.
	 */
	float DotProduct(const Vector2& rhs) const
	{
		return m_x * rhs.m_x + m_y * rhs.m_y;
	}
	/**
	 * @brief Calculate absolute dot product.
	 */
	float AbsDotProduct(const Vector2& rhs) const
	{
		return M3D::Abs(m_x * rhs.m_x) + M3D::Abs(m_y * rhs.m_y);
	}
	/**
	 * @brief Return absolute vector.
	 */
	Vector2 Abs() const
	{
		return Vector2(M3D::Abs(m_x), M3D::Abs(m_y));
	}
	/**
	 * @brief Linear interpolation with another vector.
	 */
	Vector2 Lerp(const Vector2& rhs, float t) const
	{
		return *this * (1.0f - t) + rhs * t;
	}
	/**
	 * @brief Test for equality with another vector with epsilon.
	 */
	bool Equals(const Vector2& rhs) const
	{
		return M3D::Equals(m_x, rhs.m_x) && M3D::Equals(m_y, rhs.m_y);
	}
	/**
	 * @brief Return whether is NaN.
	 */
	bool IsNaN() const
	{
		return M3D::IsNaN(m_x) || M3D::IsNaN(m_y);
	}

	/**
	 * @brief Return normalized to unit length.
	 */
	Vector2 Normalized() const
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
	 * @brief Zero vector.
	 */
	static const Vector2 ZERO;
	/**
	 * @brief (-1,0) vector.
	 */
	static const Vector2 LEFT;
	/**
	 * @brief (1,0) vector.
	 */
	static const Vector2 RIGHT;
	/**
	 * @brief (0,1) vector.
	 */
	static const Vector2 UP;
	/**
	 * @brief (0,-1) vector.
	 */
	static const Vector2 DOWN;
	/**
	 * @brief (1,1) vector.
	 */
	static const Vector2 ONE;
};

/** 
 * @brief Multiply Vector2 with a scalar 
 */
inline Vector2 operator *(float lhs, const Vector2& rhs)
{
	return rhs * lhs;
}

/** 
 * @brief Two-dimensional vector with integer values. 
 */
class M3D_API IntVector2
{
public:
	/**
	 * @brief Construct a zero vector.
	 */
	IntVector2() :
			m_x(0), m_y(0)
	{
	}

	/**
	 * @brief Construct from coordinates.
	 */
	IntVector2(int x, int y) :
			m_x(x), m_y(y)
	{
	}

	/**
	 * @brief Construct from an int array.
	 */
	IntVector2(const int* data) :
			m_x(data[0]), m_y(data[1])
	{
	}

	/**
	 * @brief Copy-construct from another vector.
	 */
	IntVector2(const IntVector2& rhs) :
			m_x(rhs.m_x), m_y(rhs.m_y)
	{
	}

	/**
	 * @brief Test for equality with another vector.
	 */
	bool operator ==(const IntVector2& rhs) const
	{
		return m_x == rhs.m_x && m_y == rhs.m_y;
	}
	/**
	 * @brief Test for inequality with another vector.
	 */
	bool operator !=(const IntVector2& rhs) const
	{
		return m_x != rhs.m_x || m_y != rhs.m_y;
	}
	/**
	 * @brief Add a vector.
	 */
	IntVector2 operator +(const IntVector2& rhs) const
	{
		return IntVector2(m_x + rhs.m_x, m_y + rhs.m_y);
	}
	/**
	 * @brief Return negation.
	 */
	IntVector2 operator -() const
	{
		return IntVector2(-m_x, -m_y);
	}
	/**
	 * @brief Subtract a vector.
	 */
	IntVector2 operator -(const IntVector2& rhs) const
	{
		return IntVector2(m_x - rhs.m_x, m_y - rhs.m_y);
	}
	/**
	 * @brief Multiply with a scalar.
	 */
	IntVector2 operator *(int rhs) const
	{
		return IntVector2(m_x * rhs, m_y * rhs);
	}
	/**
	 * @brief Divide by a scalar.
	 */
	IntVector2 operator /(int rhs) const
	{
		return IntVector2(m_x / rhs, m_y / rhs);
	}

	/**
	 * @brief Add-assign a vector.
	 */
	IntVector2& operator +=(const IntVector2& rhs)
	{
		m_x += rhs.m_x;
		m_y += rhs.m_y;
		return *this;
	}

	/**
	 * @brief Subtract-assign a vector.
	 */
	IntVector2& operator -=(const IntVector2& rhs)
	{
		m_x -= rhs.m_x;
		m_y -= rhs.m_y;
		return *this;
	}

	/**
	 * @brief Multiply-assign a scalar.
	 */
	IntVector2& operator *=(int rhs)
	{
		m_x *= rhs;
		m_y *= rhs;
		return *this;
	}

	/**
	 * @brief Divide-assign a scalar.
	 */
	IntVector2& operator /=(int rhs)
	{
		m_x /= rhs;
		m_y /= rhs;
		return *this;
	}

	/**
	 * @brief Return integer data.
	 */
	const int* Data() const
	{
		return &m_x;
	}
	/**
	 * @brief Return as string.
	 */
	string Tostring() const;

	/**
	 * @brief Return length.
	 */
	float Length() const
	{
		return sqrtf((float) m_x * m_x + m_y * m_y);
	}
	/**
	 * @brief Return squared length.
	 */
	float LengthSquared() const
	{
		return (float) m_x * m_x + m_y * m_y;
	}

	/**
	 * @brief X coordinate.
	 */
	int m_x;
	/**
	 * @brief Y coordinate.
	 */
	int m_y;

	/**
	 * @brief Zero vector.
	 */
	static const IntVector2 ZERO;
};

/** 
 * @brief Multiply IntVector2 with a scalar. 
 */
inline IntVector2 operator *(int lhs, const IntVector2& rhs)
{
	return rhs * lhs;
}

}
#endif //VECTOR2_H
