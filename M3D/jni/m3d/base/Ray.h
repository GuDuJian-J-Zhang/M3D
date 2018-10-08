/**
 *@file
 *@brief
 *@author	XL
 *@date		2015-2-26
 *@version	1.0
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 */

#ifndef M3D_RAY_H_
#define M3D_RAY_H_

#include "m3d/base/Vector3.h"
#include "m3d/base/Matrix4.h"

namespace M3D
{

class BoundingBox;
class Frustum;
class Plane;
class Sphere;

/** 
 * @brief Infinite straight line in three-dimensional space. 
 */
class M3D_API Ray
{
public:
	/**
	 * @brief Construct a degenerate ray with zero origin and direction.
	 */
	Ray()
	{
	}

	/**
	 * @brief  Construct from origin and direction. The direction will be normalized.
	 */
	Ray(const Vector3& origin, const Vector3& direction)
	{
		Define(origin, direction);
	}

	/**
	 * @brief  Copy-construct from another ray.
	 */
	Ray(const Ray& ray) :
			m_origin(ray.m_origin), m_direction(ray.m_direction)
	{
	}

	/**
	 * @brief  Assign from another ray.
	 */
	Ray& operator =(const Ray& rhs)
	{
		m_origin = rhs.m_origin;
		m_direction = rhs.m_direction;
		return *this;
	}

	/**
	 * @brief  Check for equality with another ray.
	 */
	bool operator ==(const Ray& rhs) const
	{
		return m_origin == rhs.m_origin && m_direction == rhs.m_direction;
	}
	/**
	 * @brief  Check for inequality with another ray.
	 */
	bool operator !=(const Ray& rhs) const
	{
		return m_origin != rhs.m_origin || m_direction != rhs.m_direction;
	}

	/**
	 * @brief  Define from origin and direction. The direction will be normalized.
	 */
	void Define(const Vector3& origin, const Vector3& direction)
	{
		m_origin = origin;
		m_direction = direction.Normalized();
	}

	/**
	 * @brief  Project a point on the ray.
	 */
	Vector3 Project(const Vector3& point) const
	{
		Vector3 offset = point - m_origin;
		return m_origin + offset.DotProduct(m_direction) * m_direction;
	}

	/**
	 * @brief  Return distance of a point from the ray.
	 */
	float Distance(const Vector3& point) const
	{
		Vector3 projected = Project(point);
		return (point - projected).Length();
	}

	/**
	 * @brief  Return closest point to another ray.
	 */
	Vector3 ClosestPoint(const Ray& ray) const;
	/**
	 * @brief  Return hit distance to a plane, or infinity if no hit.
	 */
	float HitDistance(const Plane& plane) const;
	/**
	 * @brief  Return hit distance to a bounding box, or infinity if no hit.
	 */
	float HitDistance(const BoundingBox& box) const;
	/**
	 * @brief  Return hit distance to a frustum, or infinity if no hit. If solidInside parameter is true (default) rays originating from inside return zero distance, otherwise the distance to the closest plane.
	 */
	float HitDistance(const Frustum& frustum, bool solidInside = true) const;
	/**
	 * @brief  Return hit distance to a sphere, or infinity if no hit.
	 */
	float HitDistance(const Sphere& sphere) const;
	/**
	 * @brief  Return hit distance to a triangle, or infinity if no hit.
	 */
	float HitDistance(const Vector3& v0, const Vector3& v1,
			const Vector3& v2) const;
	/**
	 * @brief  Return hit distance to a triangle and out normal, or infinity if no hit.
	 */
	float HitDistance(const Vector3& v0, const Vector3& v1, const Vector3& v2,
			Vector3* outNormal) const;
	/**
	 * @brief  Return hit distance to non-indexed geometry data, or infinity if no hit. Optionally return normal.
	 */
	float HitDistance(const void* vertexData, unsigned verteSize,
			unsigned vertexStart, unsigned vertexCount,
			Vector3* outNormal = 0) const;
	/**
	 * @brief  Return hit distance to indexed geometry data, or infinity if no hit.
	 */
	float HitDistance(const void* vertexData, unsigned verteSize,
			const void* indexData, unsigned indeSize, unsigned indexStart,
			unsigned indexCount, Vector3* outNormal = 0) const;
	/**
	 * @brief  Return whether ray is inside non-indexed geometry.
	 */
	bool InsideGeometry(const void* vertexData, unsigned verteSize,
			unsigned vertexStart, unsigned vertexCount) const;
	/**
	 * @brief  Return whether ray is inside indexed geometry.
	 */
	bool InsideGeometry(const void* vertexData, unsigned verteSize,
			const void* indexData, unsigned indeSize, unsigned indexStart,
			unsigned indexCount) const;
	/**
	 * @brief  Return transformed by a 3x4 matrix. This may result in a non-normalized direction.
	 */
	Ray Transformed(const Matrix3x4& transform) const;

	/**
	 * @brief
	 */
	Vector3& GetDirection() const;

	/**
	 * @brief
	 */
	Vector3& GetOrigin() const;

	/**
	 * @brief  Ray origin.
	 */
	mutable Vector3 m_origin;
	/**
	 * @brief  Ray direction.
	 */
	mutable Vector3 m_direction;
};

}
#endif //RAY_H
