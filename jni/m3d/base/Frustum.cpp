
#include "m3d/base/Frustum.h"

namespace M3D
{

	inline Vector3 ClipEdgeZ(const Vector3& v0, const Vector3& v1, float clipZ)
	{
		return Vector3(
			v1.m_x + (v0.m_x - v1.m_x) * ((clipZ - v1.m_z) / (v0.m_z - v1.m_z)),
			v1.m_y + (v0.m_y - v1.m_y) * ((clipZ - v1.m_z) / (v0.m_z - v1.m_z)),
			clipZ
		);
	}

	void ProjectAndMergeEdge(Vector3 v0, Vector3 v1, Rect& rect, const Matrix4& projection)
	{
		// Check if both vertices behind near plane
		if (v0.m_z < M_MIN_NEARCLIP && v1.m_z < M_MIN_NEARCLIP)
			return;

		// Check if need to clip one of the vertices
		if (v1.m_z < M_MIN_NEARCLIP)
			v1 = ClipEdgeZ(v1, v0, M_MIN_NEARCLIP);
		else if (v0.m_z < M_MIN_NEARCLIP)
			v0 = ClipEdgeZ(v0, v1, M_MIN_NEARCLIP);

		// Project, perspective divide and merge
		Vector3 tV0(projection * v0);
		Vector3 tV1(projection * v1);
		rect.Merge(Vector2(tV0.m_x, tV0.m_y));
		rect.Merge(Vector2(tV1.m_x, tV1.m_y));
	}

	Frustum::Frustum()
	{
		UpdatePlanes();
	}

	Frustum::Frustum(const Frustum& frustum)
	{
		*this = frustum;
	}

	Frustum& Frustum::operator = (const Frustum& rhs)
	{
		for (unsigned i = 0; i < NUM_FRUSTUM_PLANES; ++i)
			m_planes[i] = rhs.m_planes[i];
		for (unsigned i = 0; i < NUM_FRUSTUM_VERTICES; ++i)
			m_vertices[i] = rhs.m_vertices[i];

		return *this;
	}

	void Frustum::Define(float fov, float aspectRatio, float zoom, float nearZ, float farZ, const Matrix3x4& transform)
	{
		nearZ = Max(nearZ, 0.0f);
		farZ = Max(farZ, nearZ);
		float halfViewSize = tanf(fov * M_DEGTORAD_2) / zoom;
		Vector3 _near;
		Vector3 _far;

		//左手坐标系
		//near.m_z = nearZ;
		//右手坐标系
		_near.m_z = -nearZ;
		_near.m_y = _near.m_z * halfViewSize;
		_near.m_x = _near.m_y * aspectRatio;

		//左手坐标系
		//far.m_z = farZ;
		//右手坐标系
		_far.m_z = -farZ;
		_far.m_y = _far.m_z * halfViewSize;
		_far.m_x = _far.m_y * aspectRatio;

		Define(_near, _far, transform);
	}

void Frustum::Define(const Vector3& nearRightTop, const Vector3& nearRightBottom,
		const Vector3& nearLeftBottom,
		const Vector3& nearLeftTop,
		const Vector3& farRightTop, const Vector3& farRightBottom,
		const Vector3& farLeftBottom,
		const Vector3& farLeftTop)
	{
		m_vertices[4] =  nearRightTop;
		m_vertices[5] = nearRightBottom;
		m_vertices[6] = nearLeftBottom;
		m_vertices[7] = nearLeftTop;
		m_vertices[0] = farRightTop;
		m_vertices[1] = farRightBottom;
		m_vertices[2] = farLeftBottom;
		m_vertices[3] = farLeftTop;

		UpdatePlanes();
	}

void Frustum::Define(const Vector3& _near, const Vector3& _far, const Matrix3x4& transform)
{
    m_vertices[0] = transform * _near;
    m_vertices[1] = transform * Vector3(_near.m_x, -_near.m_y, _near.m_z);
    m_vertices[2] = transform * Vector3(-_near.m_x, -_near.m_y, _near.m_z);
    m_vertices[3] = transform * Vector3(-_near.m_x, _near.m_y, _near.m_z);
    m_vertices[4] = transform * _far;
    m_vertices[5] = transform * Vector3(_far.m_x, -_far.m_y, _far.m_z);
    m_vertices[6] = transform * Vector3(-_far.m_x, -_far.m_y, _far.m_z);
    m_vertices[7] = transform * Vector3(-_far.m_x, _far.m_y, _far.m_z);
    
    UpdatePlanes();
}

void Frustum::Define(const BoundingBox& box, const Matrix3x4& transform)
{
    m_vertices[0] = transform * Vector3(box.m_max.m_x, box.m_max.m_y, box.m_min.m_z);
    m_vertices[1] = transform * Vector3(box.m_max.m_x, box.m_min.m_y, box.m_min.m_z);
    m_vertices[2] = transform * Vector3(box.m_min.m_x, box.m_min.m_y, box.m_min.m_z);
    m_vertices[3] = transform * Vector3(box.m_min.m_x, box.m_max.m_y, box.m_min.m_z);
    m_vertices[4] = transform * Vector3(box.m_max.m_x, box.m_max.m_y, box.m_max.m_z);
    m_vertices[5] = transform * Vector3(box.m_max.m_x, box.m_min.m_y, box.m_max.m_z);
    m_vertices[6] = transform * Vector3(box.m_min.m_x, box.m_min.m_y, box.m_max.m_z);
    m_vertices[7] = transform * Vector3(box.m_min.m_x, box.m_max.m_y, box.m_max.m_z);
    
    UpdatePlanes();
}

void Frustum::DefineOrtho(float orthoSize, float aspectRatio, float zoom, float nearZ, float farZ, const Matrix3x4& transform)
{
    nearZ = Max(nearZ, 0.0f);
    farZ = Max(farZ, nearZ);
    float halfViewSize = orthoSize * 0.5f / zoom;
    Vector3 _near, _far;
    
    //left hand
    _near.m_z = -nearZ;
    _far.m_z = -farZ;

    //right hand
//  near.m_z = -nearZ;
//  far.m_z = -farZ;

    _far.m_y = _near.m_y = halfViewSize;
    _far.m_x = _near.m_x = _near.m_y * aspectRatio;
    
    Define(_near, _far, transform);
}

void Frustum::Transform(const Matrix3& transform)
{
    for (unsigned i = 0; i < NUM_FRUSTUM_VERTICES; ++i)
        m_vertices[i] = transform * m_vertices[i];
    
    UpdatePlanes();
}

void Frustum::Transform(const Matrix3x4& transform)
{
    for (unsigned i = 0; i < NUM_FRUSTUM_VERTICES; ++i)
        m_vertices[i] = transform * m_vertices[i];
    
    UpdatePlanes();
}

Frustum Frustum::Transformed(const Matrix3& transform) const
{
    Frustum transformed;
    for (unsigned i = 0; i < NUM_FRUSTUM_VERTICES; ++i)
        transformed.m_vertices[i] = transform * m_vertices[i];
    
    transformed.UpdatePlanes();
    return transformed;
}

Frustum Frustum::Transformed(const Matrix3x4& transform) const
{
    Frustum transformed;
    for (unsigned i = 0; i < NUM_FRUSTUM_VERTICES; ++i)
        transformed.m_vertices[i] = transform * m_vertices[i];
    
    transformed.UpdatePlanes();
    return transformed;
}

Rect Frustum::Projected(const Matrix4& projection) const
{
    Rect rect;
    
    ProjectAndMergeEdge(m_vertices[0], m_vertices[4], rect, projection);
    ProjectAndMergeEdge(m_vertices[1], m_vertices[5], rect, projection);
    ProjectAndMergeEdge(m_vertices[2], m_vertices[6], rect, projection);
    ProjectAndMergeEdge(m_vertices[3], m_vertices[7], rect, projection);
    ProjectAndMergeEdge(m_vertices[4], m_vertices[5], rect, projection);
    ProjectAndMergeEdge(m_vertices[5], m_vertices[6], rect, projection);
    ProjectAndMergeEdge(m_vertices[6], m_vertices[7], rect, projection);
    ProjectAndMergeEdge(m_vertices[7], m_vertices[4], rect, projection);
    
    return rect;
}

void Frustum::UpdatePlanes()
{
    m_planes[PLANE_NEAR].Define(m_vertices[2], m_vertices[1], m_vertices[0]);
    m_planes[PLANE_LEFT].Define(m_vertices[3], m_vertices[7], m_vertices[6]);
    m_planes[PLANE_RIGHT].Define(m_vertices[1], m_vertices[5], m_vertices[4]);
    m_planes[PLANE_UP].Define(m_vertices[0], m_vertices[4], m_vertices[7]);
    m_planes[PLANE_DOWN].Define(m_vertices[6], m_vertices[5], m_vertices[1]);
    m_planes[PLANE_FAR].Define(m_vertices[5], m_vertices[6], m_vertices[7]);

    // Check if we ended up with inverted planes (reflected transform) and flip in that case
    if (m_planes[PLANE_NEAR].Distance(m_vertices[5]) < 0.0f)
    {
        for (unsigned i = 0; i < NUM_FRUSTUM_PLANES; ++i)
        {
            m_planes[i].m_normal = -m_planes[i].m_normal;
            m_planes[i].m_d = -m_planes[i].m_d;
        }
    }

}

string Frustum::ToString() const
{
	string frustumInfo = "";
	for(int i=0;i<NUM_FRUSTUM_VERTICES ;i++ )
	{
		frustumInfo += "---";
		frustumInfo += m_vertices[i].Tostring();
	}
    return frustumInfo;
}

}
