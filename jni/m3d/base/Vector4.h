/**
 *@file
 *@brief
 *@author	XL
 *@date		2015-2-26
 *@version	1.0
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 */
#ifndef M3D_VECTOR4_H_
#define M3D_VECTOR4_H_

#include "m3d/base/Vector3.h"

namespace M3D
{

/** 
 * @brief Four-dimensional vector. 
 */
class M3D_API Vector4
{
public:
	/**
	 * @brief Construct a zero vector.
	 */
	Vector4();

	/**
	 * @brief Copy-construct from another vector.
	 */
	Vector4(const Vector4& vector) :
			m_x(vector.m_x), m_y(vector.m_y), m_z(vector.m_z), m_w(vector.m_w)
	{
	}

	/**
	 * @brief Construct from a 3-dimensional vector and the W coordinate.
	 */
	Vector4(const Vector3& vector, float w) :
			m_x(vector.m_x), m_y(vector.m_y), m_z(vector.m_z), m_w(w)
	{
	}

	/**
	 * @brief Construct from coordinates.
	 */
	Vector4(float x, float y, float z, float w) :
			m_x(x), m_y(y), m_z(z), m_w(w)
	{
	}

	/**
	 * @brief Construct from a float array.
	 */
	Vector4(const float* data) :
			m_x(data[0]), m_y(data[1]), m_z(data[2]), m_w(data[3])
	{
	}

	Vector3 GetVector3()
	{
		return Vector3(m_x, m_y, m_z);
	}

	/**
	 * @brief Assign from another vector.
	 */
	Vector4& operator =(const Vector4& rhs)
	{
		m_x = rhs.m_x;
		m_y = rhs.m_y;
		m_z = rhs.m_z;
		m_w = rhs.m_w;
		return *this;
	}

	/**
	 * @brief Test for equality with another vector without epsilon.
	 */
	bool operator ==(const Vector4& rhs) const
	{
		return m_x == rhs.m_x && m_y == rhs.m_y && m_z == rhs.m_z
				&& m_w == rhs.m_w;
	}
	/**
	 * @brief Test for inequality with another vector without epsilon.
	 */
	bool operator !=(const Vector4& rhs) const
	{
		return m_x != rhs.m_x || m_y != rhs.m_y || m_z != rhs.m_z
				|| m_w != rhs.m_w;
	}
	/**
	 * @brief Add a vector.
	 */
	Vector4 operator +(const Vector4& rhs) const
	{
		return Vector4(m_x + rhs.m_x, m_y + rhs.m_y, m_z + rhs.m_z,
				m_w + rhs.m_w);
	}	
	Vector4 Add(const Vector4& rhs) const
	{
		return Vector4(m_x + rhs.m_x, m_y + rhs.m_y, m_z + rhs.m_z,
			m_w + rhs.m_w);
	}
	/**
	 * @brief Return negation.
	 */
	Vector4 operator -() const
	{
		return Vector4(-m_x, -m_y, -m_z, -m_w);
	}
	Vector4 Negative() const
	{
		return Vector4(-m_x, -m_y, -m_z, -m_w);
	}
	/**
	 * @brief Subtract a vector.
	 */
	Vector4 operator -(const Vector4& rhs) const
	{
		return Vector4(m_x - rhs.m_x, m_y - rhs.m_y, m_z - rhs.m_z,
				m_w - rhs.m_w);
	}
	Vector4 Sub(const Vector4& rhs) const
	{
		return Vector4(m_x - rhs.m_x, m_y - rhs.m_y, m_z - rhs.m_z,
			m_w - rhs.m_w);
	}
	/**
	 * @brief Multiply with a scalar.
	 */
	Vector4 operator *(float rhs) const
	{
		return Vector4(m_x * rhs, m_y * rhs, m_z * rhs, m_w * rhs);
	}
	Vector4 Multiply(float rhs) const
	{
		return Vector4(m_x * rhs, m_y * rhs, m_z * rhs, m_w * rhs);
	}
	/**
	 * @brief Multiply with a vector.
	 */
	Vector4 operator *(const Vector4& rhs) const
	{
		return Vector4(m_x * rhs.m_x, m_y * rhs.m_y, m_z * rhs.m_z,
				m_w * rhs.m_w);
	}
	Vector4 Multiply(const Vector4& rhs) const
	{
		return Vector4(m_x * rhs.m_x, m_y * rhs.m_y, m_z * rhs.m_z,
			m_w * rhs.m_w);
	}
	/**
	 * @brief Divide by a scalar.
	 */
	Vector4 operator /(float rhs) const
	{
		return Vector4(m_x / rhs, m_y / rhs, m_z / rhs, m_w / rhs);
	}
	Vector4 Divede(float rhs) const
	{
		return Vector4(m_x / rhs, m_y / rhs, m_z / rhs, m_w / rhs);
	}
	/**
	 * @brief Divide by a vector.
	 */
	Vector4 operator /(const Vector4& rhs) const
	{
		return Vector4(m_x / rhs.m_x, m_y / rhs.m_y, m_z / rhs.m_z,
				m_w / rhs.m_w);
	}
	Vector4 Divede(const Vector4& rhs) const
	{
		return Vector4(m_x / rhs.m_x, m_y / rhs.m_y, m_z / rhs.m_z,
			m_w / rhs.m_w);
	}
	/**
	 * @brief Add-assign a vector.
	 */
	Vector4& operator +=(const Vector4& rhs)
	{
		m_x += rhs.m_x;
		m_y += rhs.m_y;
		m_z += rhs.m_z;
		m_w += rhs.m_w;
		return *this;
	}
	Vector4& Added(const Vector4& rhs)
	{
		m_x += rhs.m_x;
		m_y += rhs.m_y;
		m_z += rhs.m_z;
		m_w += rhs.m_w;
		return *this;
	}
	/**
	 * @brief Subtract-assign a vector.
	 */
	Vector4& operator -=(const Vector4& rhs)
	{
		m_x -= rhs.m_x;
		m_y -= rhs.m_y;
		m_z -= rhs.m_z;
		m_w -= rhs.m_w;
		return *this;
	}
	Vector4& Subed(const Vector4& rhs)
	{
		m_x -= rhs.m_x;
		m_y -= rhs.m_y;
		m_z -= rhs.m_z;
		m_w -= rhs.m_w;
		return *this;
	}
	/**
	 * @brief Multiply-assign a scalar.
	 */
	Vector4& operator *=(float rhs)
	{
		m_x *= rhs;
		m_y *= rhs;
		m_z *= rhs;
		m_w *= rhs;
		return *this;
	}
	Vector4& Multiplyed(float rhs)
	{
		m_x *= rhs;
		m_y *= rhs;
		m_z *= rhs;
		m_w *= rhs;
		return *this;
	}
	/**
	 * @brief Multiply-assign a vector.
	 */
	Vector4& operator *=(const Vector4& rhs)
	{
		m_x *= rhs.m_x;
		m_y *= rhs.m_y;
		m_z *= rhs.m_z;
		m_w *= rhs.m_w;
		return *this;
	}
	Vector4& Multiplyed(const Vector4& rhs)
	{
		m_x *= rhs.m_x;
		m_y *= rhs.m_y;
		m_z *= rhs.m_z;
		m_w *= rhs.m_w;
		return *this;
	}
	/**
	 * @brief Divide-assign a scalar.
	 */
	Vector4& operator /=(float rhs)
	{
		float invRhs = 1.0f / rhs;
		m_x *= invRhs;
		m_y *= invRhs;
		m_z *= invRhs;
		m_w *= invRhs;
		return *this;
	}	
	Vector4& Divided(float rhs)
	{
		float invRhs = 1.0f / rhs;
		m_x *= invRhs;
		m_y *= invRhs;
		m_z *= invRhs;
		m_w *= invRhs;
		return *this;
	}

	/**
	 * @brief Divide-assign a vector.
	 */
	Vector4& operator /=(const Vector4& rhs)
	{
		m_x /= rhs.m_x;
		m_y /= rhs.m_y;
		m_z /= rhs.m_z;
		m_w /= rhs.m_w;
		return *this;
	}
	Vector4& Divided(const Vector4& rhs)
	{
		m_x /= rhs.m_x;
		m_y /= rhs.m_y;
		m_z /= rhs.m_z;
		m_w /= rhs.m_w;
		return *this;
	}
	/**
	 * @brief Calculate dot product.
	 */
	float DotProduct(const Vector4& rhs) const
	{
		return m_x * rhs.m_x + m_y * rhs.m_y + m_z * rhs.m_z + m_w * rhs.m_w;
	}
	/**
	 * @brief Calculate absolute dot product.
	 */
	float AbsDotProduct(const Vector4& rhs) const
	{
		return M3D::Abs(m_x * rhs.m_x) + M3D::Abs(m_y * rhs.m_y)
				+ M3D::Abs(m_z * rhs.m_z) + M3D::Abs(m_w * rhs.m_w);
	}
	/**
	 * @brief Return absolute vector.
	 */
	Vector4 Abs() const
	{
		return Vector4(M3D::Abs(m_x), M3D::Abs(m_y), M3D::Abs(m_z),
				M3D::Abs(m_w));
	}
	/**
	 * @brief Linear interpolation with another vector.
	 */
	Vector4 Lerp(const Vector4& rhs, float t) const
	{
		return *this * (1.0f - t) + rhs * t;
	}
	/**
	 * @brief Test for equality with another vector with epsilon.
	 */
	bool Equals(const Vector4& rhs) const
	{
		return M3D::Equals(m_x, rhs.m_x) && M3D::Equals(m_y, rhs.m_y)
				&& M3D::Equals(m_z, rhs.m_z) && M3D::Equals(m_w, rhs.m_w);
	}
	/**
	 * @brief Return whether is NaN.
	 */
	bool IsNaN() const
	{
		return M3D::IsNaN(m_x) || M3D::IsNaN(m_y) || M3D::IsNaN(m_z)
				|| M3D::IsNaN(m_w);
	}

	/**
	 * @brief Return float data.
	 */
	const float* Data() const
	{
		return &m_x;
	}
	M3D_ADDRESSTYPE Address()
	{
		//获取地址的真实方式
		return TAddress(*this);
	}

	void FillByAddress(M3D_ADDRESSTYPE memoryAddress)
	{
		*this = *TFromAddress<Vector4>(memoryAddress);
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
	 * @brief W coordinate.
	 */
	float m_w;

	/**
	 * @brief Zero vector.
	 */
	static const Vector4 ZERO;
	/**
	 * @brief (1,1,1) vector.
	 */
	static const Vector4 ONE;
};

/** 
 * @brief Multiply Vector4 with a scalar. 
 */
inline Vector4 operator *(float lhs, const Vector4& rhs)
{
	return rhs * lhs;
}

}

#endif //M3D_VECTOR4_H_
