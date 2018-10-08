/**
 *@file
 *@brief
 *@author	XL
 *@date		2015-2-26
 *@version	1.0
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 */
#ifndef M3D_SPHERE_H_
#define M3D_SPHERE_H_

#include "m3d/base/Vector3.h"

namespace M3D
{

class BoundingBox;
class Polyhedron;
class Frustum;

/** 
 * @brief %Sphere in three-dimensional space. 
 */
class M3D_API Sphere
{
public:
	/**
	 * @brief Construct undefined.
	 */
	Sphere() :
			m_center(Vector3::ZERO), m_radius(0.0f), m_defined(false)
	{
	}

	/**
	 * @brief Copy-construct from another sphere.
	 */
	Sphere(const Sphere& sphere) :
			m_center(sphere.m_center), m_radius(sphere.m_radius), m_defined(
					sphere.m_defined)
	{
	}

	/**
	 * @brief Construct from center and radius.
	 */
	Sphere(const Vector3& center, float radius) :
			m_center(center), m_radius(radius), m_defined(true)
	{
	}

	/**
	 * @brief Construct from an array of vertices.
	 */
	Sphere(const Vector3* vertices, unsigned count) :
			m_defined(false)
	{
		Define(vertices, count);
	}

	/**
	 * @brief Construct from a bounding box.
	 */
	Sphere(const BoundingBox& box) :
			m_defined(false)
	{
		Define(box);
	}

	/**
	 * @brief Construct from a frustum.
	 */
	Sphere(const Frustum& frustum) :
			m_defined(false)
	{
		Define(frustum);
	}

	/**
	 * @brief Construct from a polyhedron.
	 */
//    Sphere(const Polyhedron& poly) :
//        defined_(false)
//    {
//        Define(poly);
//    }
	/**
	 * @brief Assign from another sphere.
	 */
	Sphere& operator =(const Sphere& rhs)
	{
		m_center = rhs.m_center;
		m_radius = rhs.m_radius;
		m_defined = rhs.m_defined;
		return *this;
	}

	/**
	 * @brief Test for equality with another sphere.
	 */
	bool operator ==(const Sphere& rhs) const
	{
		return m_center == rhs.m_center && m_radius == rhs.m_radius;
	}
	/**
	 * @brief Test for inequality with another sphere.
	 */
	bool operator !=(const Sphere& rhs) const
	{
		return m_center != rhs.m_center || m_radius != rhs.m_radius;
	}

	/**
	 * @brief Define from another sphere.
	 */
	void Define(const Sphere& sphere)
	{
		Define(sphere.m_center, sphere.m_radius);
	}

	/**
	 * @brief Define from center and radius.
	 */
	void Define(const Vector3& center, float radius)
	{
		m_center = center;
		m_radius = radius;
		m_defined = true;
	}

	/**
	 * @brief Define from an array of vertices.
	 */
	void Define(const Vector3* vertices, unsigned count);
	/**
	 * @brief Define from a bounding box.
	 */
	void Define(const BoundingBox& box);
	/**
	 * @brief Define from a frustum.
	 */
	void Define(const Frustum& frustum);
	/**
	 * @brief Define from a polyhedron.
	 */
//    void Define(const Polyhedron& poly);
	/**
	 * @brief Merge a point.
	 */
	void Merge(const Vector3& point)
	{
		if (!m_defined)
		{
			m_center = point;
			m_radius = 0.0f;
			m_defined = true;
			return;
		}

		Vector3 offset = point - m_center;
		float dist = offset.Length();

		if (dist > m_radius)
		{
			float half = (dist - m_radius) * 0.5f;
			m_radius += half;
			m_center += (half / dist) * offset;
		}
	}

	/**
	 * @brief Merge an array of vertices.
	 */
	void Merge(const Vector3* vertices, unsigned count);
	/**
	 * @brief Merge a bounding box.
	 */
	void Merge(const BoundingBox& box);
	/**
	 * @brief Merge a frustum.
	 */
	void Merge(const Frustum& frustum);
	/**
	 * @brief Merge a polyhedron.
	 */
	void Merge(const Polyhedron& poly);
	/**
	 * @brief Merge a sphere.
	 */
	void Merge(const Sphere& sphere);

	/**
	 * @brief Clear to undefined state.
	 */
	void Clear()
	{
		m_center = Vector3::ZERO;
		m_radius = 0.0f;
		m_defined = false;
	}

	/**
	 * @brief Test if a point is inside.
	 */
	Intersection IsInside(const Vector3& point) const
	{
		float distSquared = (point - m_center).LengthSquared();
		if (distSquared < m_radius * m_radius)
			return INSIDE;
		else
			return OUTSIDE;
	}

	/**
	 * @brief Test if another sphere is inside, outside or intersects.
	 */
	Intersection IsInside(const Sphere& sphere) const
	{
		float dist = (sphere.m_center - m_center).Length();
		if (dist >= sphere.m_radius + m_radius)
			return OUTSIDE;
		else if (dist + sphere.m_radius < m_radius)
			return INSIDE;
		else
			return INTERSECTS;
	}

	/**
	 * @brief Test if another sphere is (partially) inside or outside.
	 */
	Intersection IsInsideFast(const Sphere& sphere) const
	{
		float distSquared = (sphere.m_center - m_center).LengthSquared();
		float combined = sphere.m_radius + m_radius;

		if (distSquared >= combined * combined)
			return OUTSIDE;
		else
			return INSIDE;
	}

	/**
	 * @brief Test if a bounding box is inside, outside or intersects.
	 */
	Intersection IsInside(const BoundingBox& box) const;
	/**
	 * @brief Test if a bounding box is (partially) inside or outside.
	 */
	Intersection IsInsideFast(const BoundingBox& box) const;

	/**
	 * @brief Return distance of a point to the surface, or 0 if inside.
	 */
	float Distance(const Vector3& point) const
	{
		return Max((point - m_center).Length() - m_radius, 0.0f);
	}

	/**
	 * @brief Sphere center.
	 */
	Vector3 m_center;
	/**
	 * @brief Sphere radius.
	 */
	float m_radius;
	/**
	 * @brief Defined flag.
	 */
	bool m_defined;
};

}
#endif //SPHERE_H
