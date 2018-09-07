/**
 *@file
 *@brief
 *@author	XL
 *@date		2015-2-26
 *@version	1.0
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 */
#ifndef M3D_FRUSTUM_H_
#define M3D_FRUSTUM_H_

#include "m3d/base/BoundingBox.h"
#include "m3d/base/Plane.h"
#include "m3d/base/Rect.h"
#include "m3d/base/Sphere.h"
#include "m3d/base/Matrix3x4.h"

namespace M3D
{

/**
 * 视锥六个裁剪平面
 */
enum FrustumPlane
{
	PLANE_NEAR = 0, PLANE_LEFT, PLANE_RIGHT, PLANE_UP, PLANE_DOWN, PLANE_FAR,
};

static const unsigned NUM_FRUSTUM_PLANES = 6;
static const unsigned NUM_FRUSTUM_VERTICES = 8;

/** 
 * @brief  Convex constructed of 6 planes. 
 */
class M3D_API Frustum
{
public:
	/**
	 * @brief  Construct a degenerate frustum with all points at origin.
	 */
	Frustum();
	/**
	 * @brief  Copy-construct from another frustum.
	 */
	Frustum(const Frustum& frustum);

	/**
	 * @brief  Assign from another frustum.
	 */
	Frustum& operator =(const Frustum& rhs);

	/**
	 * @brief  Define with projection parameters and a transform matrix.
	 */
	void Define(float fov, float aspectRatio, float zoom, float nearZ,
			float farZ, const Matrix3x4& transform = Matrix3x4::IDENTITY);

	/**
	* @brief  Define with eight points righttop 0 rightbottom 1 leftbotto 2 lefttop3
	*/
	void Define(const Vector3& nearRightTop, const Vector3& nearRightBottom, 
		const Vector3& nearLeftBottom,
		const Vector3& nearLeftTop,
		const Vector3& farRightTop, const Vector3& farRightBottom,
		const Vector3& farLeftBottom,
		const Vector3& farLeftTop);

	/**
	 * @brief  Define with near and far dimension vectors and a transform matrix.
	 */
	void Define(const Vector3& near, const Vector3& far,
			const Matrix3x4& transform = Matrix3x4::IDENTITY);
	/**
	 * @brief  Define with a bounding box and a transform matrix.
	 */
	void Define(const BoundingBox& box, const Matrix3x4& transform =
			Matrix3x4::IDENTITY);
	/**
	 * @brief  Define with orthographic projection parameters and a transform matrix.
	 */
	void DefineOrtho(float orthoSize, float aspectRatio, float zoom,
			float nearZ, float farZ, const Matrix3x4& transform =
					Matrix3x4::IDENTITY);
	/**
	 * @brief  Transform by a 3x3 matrix.
	 */
	void Transform(const Matrix3& transform);
	/**
	 * @brief  Transform by a 3x4 matrix.
	 */
	void Transform(const Matrix3x4& transform);

	/**
	 * @brief  Test if a point is inside or outside.
	 */
	Intersection IsInside(const Vector3& point) const
	{
		for (unsigned i = 0; i < NUM_FRUSTUM_PLANES; ++i)
		{
			if (m_planes[i].Distance(point) < 0.0f)
				return OUTSIDE;
		}

		return INSIDE;
	}

	/**
	 * @brief  Test if a sphere is inside, outside or intersects.
	 */
	Intersection IsInside(const Sphere& sphere) const
	{
		bool allInside = true;
		for (unsigned i = 0; i < NUM_FRUSTUM_PLANES; ++i)
		{
			float dist = m_planes[i].Distance(sphere.GetCenter());
			if (dist < -sphere.GetRadius())
				return OUTSIDE;
			else if (dist < sphere.GetRadius())
				allInside = false;
		}

		return allInside ? INSIDE : INTERSECTS;
	}

	/**
	 * @brief  Test if a sphere if (partially) inside or outside.
	 */
	Intersection IsInsideFast(const Sphere& sphere) const
	{
		for (unsigned i = 0; i < NUM_FRUSTUM_PLANES; ++i)
		{
			if (m_planes[i].Distance(sphere.GetCenter()) < -sphere.GetRadius())
				return OUTSIDE;
		}

		return INSIDE;
	}

	/**
	 * @brief  Test if a bounding box is inside, outside or intersects.
	 */
	Intersection IsInside(const BoundingBox& box) const
	{
		Vector3 center = box.Center();
		Vector3 edge = center - box.m_min;
		bool allInside = true;

		for (unsigned i = 0; i < NUM_FRUSTUM_PLANES; ++i)
		{
			const Plane& plane = m_planes[i];
			float dist = plane.m_normal.DotProduct(center) + plane.m_d;
			float absDist = plane.m_absNormal.DotProduct(edge);

			if (dist < -absDist)
				return OUTSIDE;
			else if (dist < absDist)
				allInside = false;
		}

		return allInside ? INSIDE : INTERSECTS;
	}

	/**
	 * @brief  Test if a bounding box is (partially) inside or outside.
	 */
	Intersection IsInsideFast(const BoundingBox& box) const
	{
		Vector3 center = box.Center();
		Vector3 edge = center - box.m_min;

		for (unsigned i = 0; i < NUM_FRUSTUM_PLANES; ++i)
		{
			const Plane& plane = m_planes[i];
			float dist = plane.m_normal.DotProduct(center) + plane.m_d;
			float absDist = plane.m_absNormal.DotProduct(edge);

			if (dist < -absDist)
				return OUTSIDE;
		}

		return INSIDE;
	}

	/**
	 * @brief  Return distance of a point to the frustum, or 0 if inside.
	 */
	float Distance(const Vector3& point) const
	{
		float distance = 0.0f;
		for (unsigned i = 0; i < NUM_FRUSTUM_PLANES; ++i)
			distance = Max(-m_planes[i].Distance(point), distance);

		return distance;
	}

	/**
	 * @brief  Return transformed by a 3x3 matrix.
	 */
	Frustum Transformed(const Matrix3& transform) const;
	/**
	 * @brief  Return transformed by a 3x4 matrix.
	 */
	Frustum Transformed(const Matrix3x4& transform) const;
	/**
	 * @brief  Return projected by a 4x4 projection matrix.
	 */
	Rect Projected(const Matrix4& transform) const;

	/**
	 * @brief  Update the planes. Called internally.
	 */
	void UpdatePlanes();
	/**
	 * @brief 获取变量格式化之后的字符串，共调试使用
	 */
	string ToString() const;

	Plane m_planes[NUM_FRUSTUM_PLANES];//!< Frustum planes.

	Vector3 m_vertices[NUM_FRUSTUM_VERTICES];//!< Frustum vertices.
	//点坐标 nearplane righttop 0 rightbottom 1 leftbotto 2 lefttop3
	// farplane  righttop 4 rightbottom 5 leftbotto 6 lefttop7

};

}
#endif //M3D_FRUSTUM_H_
