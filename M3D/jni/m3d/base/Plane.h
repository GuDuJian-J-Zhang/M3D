/**
 *@file
 *@brief
 *@author	XL
 *@date		2015-2-26
 *@version	1.0
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 */
#ifndef M3D_MPLANE_H_
#define M3D_MPLANE_H_

#include "m3d/base/Matrix3x4.h"
#include "m3d/base/Matrix4.h"

namespace M3D
{
class Ray;
/** 
 * @brief Surface in three-dimensional space. 
 */
class M3D_API Plane
{
public:
	/**
	 * @brief Construct a degenerate plane with zero normal and parameter.
	 */
	Plane() :
			m_d(0.0f)
	{
	}

	/**
	 * @brief Copy-construct from another plane.
	 */
	Plane(const Plane& plane) :
			m_normal(plane.m_normal), m_absNormal(plane.m_absNormal), m_d(plane.m_d)
	{
	}

	/**
	 * @brief Construct from 3 vertices.
	 */
	Plane(const Vector3& v0, const Vector3& v1, const Vector3& v2)
	{
		Define(v0, v1, v2);
	}

	/**
	 * @brief Construct from a normal vector and a point on the plane.
	 */
	Plane(const Vector3& normal, const Vector3& point)
	{
		Define(normal, point);
	}

	Plane(const Vector3& normal, const float D)
	{
		m_normal = normal;
		m_absNormal = m_normal.Abs();
		m_d = D;
	}

	/**
	 * @brief Construct from a 4-dimensional vector, where the w coordinate is the plane parameter.
	 */
	Plane(const Vector4& plane)
	{
		Define(plane);
	}

	/**
	 * @brief Define from 3 vertices.
	 */
	void Define(const Vector3& v0, const Vector3& v1, const Vector3& v2)
	{
		Vector3 dist1 = v1 - v0;
		Vector3 dist2 = v2 - v0;

		Define(dist1.CrossProduct(dist2), v0);
	}

	/**
	 * @brief Define from a normal vector and a point on the plane.
	 */
	void Define(const Vector3& normal, const Vector3& point)
	{
		m_normal = normal.Normalized();
		m_absNormal = m_normal.Abs();
		m_d = -m_normal.DotProduct(point);
	}

	/**
	 * @brief Define from a 4-dimensional vector, where the w coordinate is the plane parameter.
	 */
	void Define(const Vector4& plane)
	{
		m_normal = Vector3(plane.m_x, plane.m_y, plane.m_z);
		m_absNormal = m_normal.Abs();
		m_d = plane.m_w;
	}

	/**
	 * @brief Transform with a 3x3 matrix.
	 */
	void Transform(const Matrix3& transform);
	/**
	 * @brief Transform with a 3x4 matrix.
	 */
	void Transform(const Matrix3x4& transform);
	/**
	 * @brief Transform with a 4x4 matrix.
	 */
	void Transform(const Matrix4& transform);

	/**
	 * @brief Project a point on the plane.
	 */
	Vector3 Project(const Vector3& point) const
	{
		return point - m_normal * (m_normal.DotProduct(point) + m_d);
	}
	/**
	 * @brief Return signed distance to a point.
	 */
	float Distance(const Vector3& point) const
	{
		return m_normal.DotProduct(point) + m_d;
	}
	/**
	 * @brief Reflect a normalized direction vector.
	 */
	Vector3 Reflect(const Vector3& direction) const
	{
		return direction - (2.0f * m_normal.DotProduct(direction) * m_normal);
	}
	/**
	 * @brief Return a reflection matrix.
	 */
	Matrix3x4 ReflectionMatrix() const;
	/**
	 * @brief Return transformed by a 3x3 matrix.
	 */
	Plane Transformed(const Matrix3& transform) const;
	/**
	 * @brief Return transformed by a 3x4 matrix.
	 */
	Plane Transformed(const Matrix3x4& transform) const;
	/**
	 * @brief Return transformed by a 4x4 matrix.
	 */
	Plane Transformed(const Matrix4& transform) const;
	/**
	 * @brief Return as a vector.
	 */
	Vector4 ToVector4() const
	{
		return Vector4(m_normal, m_d);
	}
	M3D_ADDRESSTYPE Address()
	{
		//获取地址的真实方式
		return TAddress(*this);
	}


	void FillByAddress(M3D_ADDRESSTYPE memoryAddress)
	{
		*this = *TFromAddress<Plane>(memoryAddress);
	}

	/**
	 * @brief 计算射线与面的交点
	 */
	bool GetInsectPnt(const Ray& ray, Vector3& intersectPnt);

	/**
	 * @brief Plane normal.
	 */
	Vector3 m_normal;
	/**
	 * @brief Plane absolute normal.
	 */
	Vector3 m_absNormal;
	/**
	 * @brief Plane constant.
	 */
	float m_d;

	/**
	 * @brief Plane at origin with normal pointing up.
	 */
	static const Plane UP;
};

}
#endif //PLANE_H
