/**
 *@file
 *@brief
 *@author	XL
 *@date		2015-2-26
 *@version	1.0
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 */
#ifndef M3D_RECT_H_
#define M3D_RECT_H_

#include "m3d/base/Vector4.h"

namespace M3D
{

/** 
 * @brief Two-dimensional bounding rectangle. 
 */
class M3D_API Rect
{
public:
	/**
	 * @brief Construct an undefined rect.
	 */
	Rect() :
			m_min(Vector2::ZERO), m_max(Vector2::ZERO), m_defined(false)
	{
	}

	/**
	 * @brief Construct from minimum and maximum vectors.
	 */
	Rect(const Vector2& min, const Vector2& max) :
			m_min(min), m_max(max), m_defined(true)
	{
	}

	/**
	 * @brief Construct from coordinates.
	 */
	Rect(float left, float top, float right, float bottom) :
			m_min(left, top), m_max(right, bottom), m_defined(true)
	{
	}

	/**
	 * @brief Construct from a Vector4.
	 */
	Rect(const Vector4& vector) :
			m_min(vector.m_x, vector.m_y), m_max(vector.m_z, vector.m_w), m_defined(
					true)
	{
	}

	/**
	 * @brief Construct from a float array.
	 */
	Rect(const float* data) :
			m_min(data[0], data[1]), m_max(data[2], data[3]), m_defined(true)
	{
	}

	/**
	 * @brief Copy-construct from another rect.
	 */
	Rect(const Rect& rect) :
			m_min(rect.m_min), m_max(rect.m_max), m_defined(rect.m_defined)
	{
	}

	/**
	 * @brief Assign from another rect.
	 */
	Rect& operator =(const Rect& rhs)
	{
		m_min = rhs.m_min;
		m_max = rhs.m_max;
		m_defined = rhs.m_defined;
		return *this;
	}

	/**
	 * @brief Test for equality with another rect.
	 */
	bool operator ==(const Rect& rhs) const
	{
		return m_min == rhs.m_min && m_max == rhs.m_max;
	}
	/**
	 * @brief Test for inequality with another rect.
	 */
	bool operator !=(const Rect& rhs) const
	{
		return m_min != rhs.m_min || m_max != rhs.m_max;
	}

	/**
	 * @brief Define from another rect.
	 */
	void Define(const Rect& rect)
	{
		m_min = rect.m_min;
		m_max = rect.m_max;
		m_defined = true;
	}

	/**
	 * @brief Define from minimum and maximum vectors.
	 */
	void Define(const Vector2& min, const Vector2& max)
	{
		m_min = min;
		m_max = max;
		m_defined = true;
	}

	/**
	 * @brief Define from a point.
	 */
	void Define(const Vector2& point)
	{
		m_min = m_max = point;
		m_defined = true;
	}

	/**
	 * @brief Merge a point.
	 */
	void Merge(const Vector2& point)
	{
		if (!m_defined)
		{
			m_min = m_max = point;
			m_defined = true;
		}

		if (point.m_x < m_min.m_x)
			m_min.m_x = point.m_x;
		if (point.m_x > m_max.m_x)
			m_max.m_x = point.m_x;
		if (point.m_y < m_min.m_y)
			m_min.m_y = point.m_y;
		if (point.m_y > m_max.m_y)
			m_max.m_y = point.m_y;
	}

	/**
	 * @brief Merge a rect.
	 */
	void Merge(const Rect& rect)
	{
		if (!m_defined)
		{
			m_min = rect.m_min;
			m_max = rect.m_max;
			m_defined = true;
		}

		if (rect.m_min.m_x < m_min.m_x)
			m_min.m_x = rect.m_min.m_x;
		if (rect.m_min.m_y < m_min.m_y)
			m_min.m_y = rect.m_min.m_y;
		if (rect.m_max.m_x > m_max.m_x)
			m_max.m_x = rect.m_max.m_x;
		if (rect.m_max.m_y > m_max.m_y)
			m_max.m_y = rect.m_max.m_y;
	}

	/**
	 * @brief Clear to undefined state.
	 */
	void Clear()
	{
		m_min = Vector2::ZERO;
		m_max = Vector2::ZERO;
		m_defined = false;
	}

	/**
	 * @brief Clip with another rect.
	 */
	void Clip(const Rect& rect);

	/**
	 * @brief Return center.
	 */
	Vector2 Center() const
	{
		return (m_max + m_min) * 0.5f;
	}
	/**
	 * @brief Return size.
	 */
	Vector2 Size() const
	{
		return m_max - m_min;
	}
	/**
	 * @brief Return half-size.
	 */
	Vector2 HalSize() const
	{
		return (m_max - m_min) * 0.5f;
	}
	/**
	 * @brief Test for equality with another rect with epsilon.
	 */
	bool Equals(const Rect& rhs) const
	{
		return m_min.Equals(rhs.m_min) && m_max.Equals(rhs.m_max);
	}

	/**
	 * @brief Test whether a point is inside.
	 */
	Intersection IsInside(const Vector2& point) const
	{
		if (point.m_x < m_min.m_x || point.m_y < m_min.m_y
				|| point.m_x > m_max.m_x || point.m_y > m_max.m_y)
			return OUTSIDE;
		else
			return INSIDE;
	}

	/**
	 * @brief Return float data.
	 */
	const void* Data() const
	{
		return &m_min.m_x;
	}
	/**
	 * @brief Return as a vector.
	 */
	Vector4 ToVector4() const
	{
		return Vector4(m_min.m_x, m_min.m_y, m_max.m_x, m_max.m_y);
	}
	/**
	 * @brief Return as string.
	 */
	string Tostring() const;

	/**
	 * @brief Minimum vector.
	 */
	Vector2 m_min;
	/**
	 * @brief Maximum vector.
	 */
	Vector2 m_max;
	/**
	 * @brief Defined flag.
	 */
	bool m_defined;

	/**
	 * @brief Rect in the range (-1, -1) - (1, 1)
	 */
	static const Rect FULL;
	/**
	 * @brief Rect in the range (0, 0) - (1, 1)
	 */
	static const Rect POSITIVE;
	/**
	 * @brief Zero-sized rect.
	 */
	static const Rect ZERO;
};

/** 
 * @brief Two-dimensional bounding rectangle with integer values. 
 */
class M3D_API IntRect
{
public:
	/**
	 * @brief Construct a zero rect.
	 */
	IntRect() :
			m_left(0), m_top(0), m_right(0), m_bottom(0)
	{
	}

	/**
	 * @brief Construct from coordinates.
	 */
	IntRect(int left, int top, int right, int bottom) :
			m_left(left), m_top(top), m_right(right), m_bottom(bottom)
	{
	}

	/**
	 * @brief Construct from an int array.
	 */
	IntRect(const int* data) :
			m_left(data[0]), m_top(data[1]), m_right(data[2]), m_bottom(data[3])
	{
	}

	/**
	 * @brief Test for equality with another rect.
	 */
	bool operator ==(const IntRect& rhs) const
	{
		return m_left == rhs.m_left && m_top == rhs.m_top
				&& m_right == rhs.m_right && m_bottom == rhs.m_bottom;
	}
	/**
	 * @brief Test for inequality with another rect.
	 */
	bool operator !=(const IntRect& rhs) const
	{
		return m_left != rhs.m_left || m_top != rhs.m_top
				|| m_right != rhs.m_right || m_bottom != rhs.m_bottom;
	}

	/**
	 * @brief Return size.
	 */
	IntVector2 Size() const
	{
		return IntVector2(Width(), Height());
	}

	IntVector2 Center() const;

	/**
	 * @brief Return width.
	 */
	int Width() const
	{
		return m_right - m_left;
	}
	/**
	 * @brief Return height.
	 */
	int Height() const
	{
		return m_bottom - m_top;
	}

	/**
	 * @brief Test whether a point is inside.
	 */
	Intersection IsInside(const IntVector2& point) const
	{
		if (point.m_x < m_left || point.m_y < m_top || point.m_x >= m_right
				|| point.m_y >= m_bottom)
			return OUTSIDE;
		else
			return INSIDE;
	}

	/**
	 * @brief Return integer data.
	 */
	const int* Data() const
	{
		return &m_left;
	}
	/**
	 * @brief Return as string.
	 */
	string Tostring() const;

	/**
	 * @brief Left coordinate.
	 */
	int m_left;
	/**
	 * @brief Top coordinate.
	 */
	int m_top;
	/**
	 * @brief Right coordinate.
	 */
	int m_right;
	/**
	 * @brief Bottom coordinate.
	 */
	int m_bottom;

	/**
	 * @brief Zero-sized rect.
	 */
	static const IntRect ZERO;
};

}
#endif //RECT_H
