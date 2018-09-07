/**
 *@file
 *@brief BoundingBox
 *@author	XL
 *@date		2015-2-26
 *@version	1.0
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 */

#ifndef M3D_BOUNDINGBOX_H_
#define M3D_BOUNDINGBOX_H_

#include "m3d/base/MathDefs.h"

#include "m3d/base/Rect.h"
#include "m3d/base/Vector3.h"
#include "m3d/base/Matrix3x4.h"

namespace M3D
{

class Polyhedron;
class Frustum;
class Matrix3;
class Matrix4;
class Sphere;
class Plane;

/** 
 * @brief Three-dimensional axis-aligned bounding box.
 *
 */
class M3D_API BoundingBox
{
public:
	/**
	 * @brief Construct with zero size.
	 *
	 */
	BoundingBox() :
			m_min(Vector3::ZERO), m_max(Vector3::ZERO), m_defined(false)
	{
	}

	/**
	 * @brief Copy-construct from another bounding box.
	 *
	 * @param box
	 */
	BoundingBox(const BoundingBox& box) :
			m_min(box.m_min), m_max(box.m_max), m_defined(box.m_defined)
	{
	}

	/**
	 * @brief Construct from a rect, with the Z dimension left zero.
	 *
	 * @param rect
	 */
	BoundingBox(const Rect& rect) :
			m_min(Vector3(rect.m_min, 0.0f)), m_max(Vector3(rect.m_max, 0.0f)), m_defined(
					true)
	{
	}

	/**
	 * @brief Construct from minimum and maximum vectors.
	 *
	 * @param min
	 * @param max
	 */
	BoundingBox(const Vector3& min, const Vector3& max) :
			m_min(min), m_max(max), m_defined(true)
	{
	}

	/**
	 * @brief Construct from minimum and maximum floats (all dimensions same.)
	 *
	 * @param min
	 * @param max
	 */
	BoundingBox(float min, float max) :
			m_min(Vector3(min, min, min)), m_max(Vector3(max, max, max)), m_defined(
					true)
	{
	}

	/**
	 * @brief Construct from an array of vertices.
	 *
	 * @param vertices
	 * @param count
	 */
	BoundingBox(const Vector3* vertices, unsigned count) :
			m_defined(false)
	{
		Define(vertices, count);
	}

	/**
	 * @brief Construct from a frustum.
	 *
	 * @param frustum
	 */
	BoundingBox(const Frustum& frustum) :
			m_defined(false)
	{
		Define(frustum);
	}

	/**
	 * @brief Construct from a polyhedron.
	 *
	 * @param poly
	 */
	BoundingBox(const Polyhedron& poly) :
			m_defined(false)
	{
		Define(poly);
	}

	/**
	 * @brief Construct from a sphere.
	 *
	 * @param sphere
	 */
	BoundingBox(const Sphere& sphere) :
			m_defined(false)
	{
		Define(sphere);
	}

	/**
	 * @brief Assign from another bounding box.
	 *
	 * @param rhs
	 * @return
	 */
	BoundingBox& operator =(const BoundingBox& rhs)
	{
		m_min = rhs.m_min;
		m_max = rhs.m_max;
		m_defined = rhs.m_defined;
		return *this;
	}

	/**
	 * @brief Assign from a Rect, with the Z dimension left zero.
	 *
	 * @param rhs
	 * @return
	 */
	BoundingBox& operator =(const Rect& rhs)
	{
		m_min = Vector3(rhs.m_min, 0.0f);
		m_max = Vector3(rhs.m_max, 0.0f);
		m_defined = true;
		return *this;
	}

	/**
	 * @brief Test for equality with another bounding box.
	 *
	 * @param rhs
	 * @return
	 */
	bool operator ==(const BoundingBox& rhs) const
	{
		return (m_min == rhs.m_min && m_max == rhs.m_max);
	}

	/**
	 * @brief Test for inequality with another bounding box.
	 *
	 * @param rhs
	 * @return
	 */
	bool operator !=(const BoundingBox& rhs) const
	{
		return (m_min != rhs.m_min || m_max != rhs.m_max);
	}

	/**
	 * @brief Define from another bounding box.
	 *
	 * @param box
	 */
	void Define(const BoundingBox& box)
	{
		Define(box.m_min, box.m_max);
	}

	/**
	 * @brief Define from a Rect.
	 *
	 * @param rect
	 */
	void Define(const Rect& rect)
	{
		Define(Vector3(rect.m_min, 0.0f), Vector3(rect.m_max, 0.0f));
	}

	/**
	 * @brief Define from minimum and maximum vectors.
	 *
	 * @param min
	 * @param max
	 */
	void Define(const Vector3& min, const Vector3& max)
	{
		m_min = min;
		m_max = max;
		m_defined = true;
	}

	/**
	 * @brief Define from minimum and maximum floats (all dimensions same.)
	 *
	 * @param min
	 * @param max
	 */
	void Define(float min, float max)
	{
		m_min = Vector3(min, min, min);
		m_max = Vector3(max, max, max);
		m_defined = true;
	}

	/**
	 * @brief Define from a point.
	 *
	 * @param point
	 */
	void Define(const Vector3& point)
	{
		m_min = m_max = point;
		m_defined = true;
	}

	/**
	 * @brief Merge a point.
	 *
	 * @param point
	 */
	void Merge(const Vector3& point)
	{
		if (!m_defined)
		{
			m_min = m_max = point;
			m_defined = true;
			return;
		}

		if (point.m_x < m_min.m_x)
			m_min.m_x = point.m_x;
		if (point.m_y < m_min.m_y)
			m_min.m_y = point.m_y;
		if (point.m_z < m_min.m_z)
			m_min.m_z = point.m_z;
		if (point.m_x > m_max.m_x)
			m_max.m_x = point.m_x;
		if (point.m_y > m_max.m_y)
			m_max.m_y = point.m_y;
		if (point.m_z > m_max.m_z)
			m_max.m_z = point.m_z;
	}

	/**
	 * @brief Merge another bounding box.
	 *
	 * @param box
	 */
	void Merge(const BoundingBox& box)
	{
		if (!m_defined)
		{
			m_min = box.m_min;
			m_max = box.m_max;
			m_defined = true;
			return;
		}

		if (box.m_min.m_x < m_min.m_x)
			m_min.m_x = box.m_min.m_x;
		if (box.m_min.m_y < m_min.m_y)
			m_min.m_y = box.m_min.m_y;
		if (box.m_min.m_z < m_min.m_z)
			m_min.m_z = box.m_min.m_z;
		if (box.m_max.m_x > m_max.m_x)
			m_max.m_x = box.m_max.m_x;
		if (box.m_max.m_y > m_max.m_y)
			m_max.m_y = box.m_max.m_y;
		if (box.m_max.m_z > m_max.m_z)
			m_max.m_z = box.m_max.m_z;
	}

	/**
	 * @brief Define from an array of vertices.
	 *
	 * @param vertices
	 * @param count
	 */
	void Define(const Vector3* vertices, unsigned count);
	/**
	 * @brief Define from a frustum.
	 *
	 * @param frustum
	 */
	void Define(const Frustum& frustum);
	/**
	 * @brief Define from a polyhedron.
	 *
	 * @param poly
	 */
	void Define(const Polyhedron& poly);
	/**
	 * @brief Define from a sphere.
	 *
	 * @param sphere
	 */
	void Define(const Sphere& sphere);
	/**
	 * @brief Merge an array of vertices.
	 *
	 * @param vertices
	 * @param count
	 */
	void Merge(const Vector3* vertices, unsigned count);
	/**
	 * @brief Merge a frustum.
	 *
	 * @param frustum
	 */
	void Merge(const Frustum& frustum);
	/**
	 * @brief Merge a polyhedron.
	 *
	 */
//    void Merge(const Polyhedron& poly);
	/**
	 * @brief Merge a sphere.
	 *
	 * @param sphere
	 */
	void Merge(const Sphere& sphere);
	/**
	 * @brief Clip with another bounding box.
	 *
	 * @param box
	 */
	void Clip(const BoundingBox& box);
	/**
	 * @brief Transform with a 3x3 matrix.
	 *
	 * @param transform
	 */
	void Transform(const Matrix3& transform);
	/**
	 * @brief Transform with a 3x4 matrix.
	 *
	 * @param transform
	 */
	void Transform(const Matrix3x4& transform);

	/**
	 * @brief Clear to undefined state.
	 *
	 */
	void Clear()
	{
		m_min = Vector3::ZERO;
		m_max = Vector3::ZERO;
		m_defined = false;
		m_points.clear();
		m_pointsArray.clear();
	}

	/**
	 * @brief Return center.
	 *
	 * @return
	 */
	Vector3 Center() const
	{
		return (m_max + m_min) * 0.5f;
	}
	/**
	 * @brief Return size.
	 *
	 * @return
	 */
	Vector3 Size() const
	{
		return m_max - m_min;
	}
	/**
	 * @brief Return half-size.
	 *
	 * @return
	 */
	Vector3 HalfSize() const
	{
		return (m_max - m_min) * 0.5f;
	}
	/**
	 * @brief
	 *
	 * @return
	 */
	float Length() const
	{
		return (m_max - m_min).Length();
	}
	/**
	 * @brief
	 *
	 * @return
	 */
	float GetXLength()
	{
		float l = m_max.m_x - m_min.m_x;
		return l >= 0 ? l : -l;
	}
	/**
	 * @brief
	 *
	 * @return
	 */
	float GetYLength() const
	{
		float l = m_max.m_y - m_min.m_y;
		return l >= 0 ? l : -l;
	}
	/**
	 * @brief
	 *
	 * @return
	 */
	float GetZLength() const
	{
		float l = m_max.m_z - m_min.m_z;
		return l >= 0 ? l : -l;
	}

	/**
	 * @brief Return transformed by a 3x3 matrix.
	 *
	 * @param transform
	 * @return
	 */
	BoundingBox Transformed(const Matrix3& transform) const;
	/**
	 * @brief Return transformed by a 3x4 matrix.
	 *
	 * @param transform
	 * @return
	 */
	BoundingBox Transformed(const Matrix3x4& transform) const;

	/**
	 * @brief Return projected by a 4x4 projection matrix.
	 *
	 * @param projection
	 * @return
	 */
	Rect Projected(const Matrix4& projection) const;

	BoundingBox Projected(const Plane& plane) const;

	/**
	 * @brief Test if a point is inside.
	 *
	 * @param point
	 * @return
	 */
	Intersection IsInside(const Vector3& point) const
	{
		if (point.m_x < m_min.m_x || point.m_x > m_max.m_x || point.m_y < m_min.m_y
				|| point.m_y > m_max.m_y || point.m_z < m_min.m_z
				|| point.m_z > m_max.m_z)
			return OUTSIDE;
		else
			return INSIDE;
	}


	/*
		判断是否与线段相交
	*/
	Intersection IsInside(const Vector3& start , const Vector3& end)const;

	/**
	 * @brief Test if another bounding box is inside, outside or intersects.
	 *
	 * @param box
	 * @return
	 */
	Intersection IsInside(const BoundingBox& box) const
	{
		if (box.m_max.m_x < m_min.m_x || box.m_min.m_x > m_max.m_x
				|| box.m_max.m_y < m_min.m_y || box.m_min.m_y > m_max.m_y
				|| box.m_max.m_z < m_min.m_z || box.m_min.m_z > m_max.m_z)
			return OUTSIDE;
		else if (box.m_min.m_x < m_min.m_x || box.m_max.m_x > m_max.m_x
				|| box.m_min.m_y < m_min.m_y || box.m_max.m_y > m_max.m_y
				|| box.m_min.m_z < m_min.m_z || box.m_max.m_z > m_max.m_z)
			return INTERSECTS;
		else
			return INSIDE;
	}

	/**
	 * @brief Test if another bounding box is (partially) inside or outside.
	 *
	 * @param box
	 * @return
	 */
	Intersection IsInsideFast(const BoundingBox& box) const
	{
		if (box.m_max.m_x < m_min.m_x || box.m_min.m_x > m_max.m_x
				|| box.m_max.m_y < m_min.m_y || box.m_min.m_y > m_max.m_y
				|| box.m_max.m_z < m_min.m_z || box.m_min.m_z > m_max.m_z)
			return OUTSIDE;
		else
			return INSIDE;
	}

	/**
	 * @brief Test if a sphere is inside, outside or intersects.
	 *
	 * @param sphere
	 * @return
	 */
	Intersection IsInside(const Sphere& sphere) const;
	/**
	 * @brief Test if a sphere is (partially) inside or outside.
	 *
	 * @param sphere
	 * @return
	 */
	Intersection IsInsideFast(const Sphere& sphere) const;
	/**
	 * @brief
	 *
	 * @return
	 */
	bool GetTriangleArray(vector<float>& outPoints);
		/**
		 * @brief
		 *
		 * @return
		 */
	bool GetVertexs(vector<float>& outPoints);
	/**
	 * @brief
	 *
	 * @return
	 */
	bool Defined() const
	{
		return m_defined;
	}

	/**
	 * @brief Return as string.
	 *
	 * @return
	 */
	string Tostring() const;
	/**
	 * @brief
	 *
	 * @param scale
	 */
	void ScaleBox(float scale);

	M3D_ADDRESSTYPE Address()
	{
		//获取地址的真实方式
		return TAddress(*this);
	}

	void FillByAddress(M3D_ADDRESSTYPE memoryAddress)
	{
		*this = *TFromAddress<BoundingBox>(memoryAddress);
	}
public:
	//static BoundingBox ZeroBox; //!<
	//绘制索引
	static M3D_INDEX_TYPE boxIndexs[24]; //!<
	static int triIndex[12][3]; //!<

	static const  BoundingBox MAX_BOX; //!<
	static BoundingBox NO_BOX; //!<

	static vector<float> m_points; //!<
	static vector<float> m_pointsArray; //!<
	Vector3 m_min; //!<
	Vector3 m_max; //!<
	bool m_defined; //!<
};

}
#endif //M3D_BOUNDINGBOX_H_
