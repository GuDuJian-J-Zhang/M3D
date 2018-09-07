#include "m3d/base/BoundingBox.h"

#include "m3d/base/Frustum.h"
#include "m3d/base/Matrix4.h"
#include "m3d/base/Plane.h"

namespace M3D
{
const BoundingBox BoundingBox::MAX_BOX (Vector3::MINIMUM,Vector3::MAXMUN);
BoundingBox BoundingBox::NO_BOX;
vector<float> BoundingBox::m_points; //!<
vector<float> BoundingBox::m_pointsArray; //!<

M3D_INDEX_TYPE BoundingBox::boxIndexs[24] =
{ 0, 1, 1, 2, 2, 3, 3, 0, 3, 4, 4, 5, 5, 0, 5, 6, 6, 7, 7, 4, 6, 1, 7, 2 };
//
// 顶点索引（不要格式化�?
//	    ^z
//	    |
//	   2|________3
//    1/______0 /|
//	  |        | |
//	  |7       | |4____> y
//	  |________|/
//   / 6 	5
// |/x
//
// 0=max 7 = min
//

int BoundingBox::triIndex[12][3] =
{
{ 1, 0, 2 },
{ 0, 3, 2 },
{ 0, 4, 3 },
{ 0, 5, 4 },
{ 7, 4, 5 },
{ 5, 6, 7 },
{ 6, 1, 7 },
{ 1, 2, 7 },
{ 6, 0, 1 },
{ 6, 5, 0 },
{ 3, 7, 2 },
{ 3, 4, 7 } };

void BoundingBox::Define(const Vector3* vertices, unsigned count)
{
    if (!count)
        return;
    
    m_defined = false;
    Merge(vertices, count);
}

void BoundingBox::Define(const Frustum& frustum)
{
    Define(frustum.m_vertices, NUM_FRUSTUM_VERTICES);
}

void BoundingBox::Define(const Polyhedron& poly)
{
    m_defined = false;
    Merge(poly);
}

void BoundingBox::Define(const Sphere& sphere)
{
    const Vector3& center = sphere.GetCenter();
    float radius = sphere.GetRadius();
    
    m_min = center + Vector3(-radius, -radius, -radius);
    m_max = center + Vector3(radius, radius, radius);
    m_defined = true;
}

void BoundingBox::Merge(const Vector3* vertices, unsigned count)
{
    while (count--)
        Merge(*vertices++);
}

void BoundingBox::Merge(const Frustum& frustum)
{
    Merge(frustum.m_vertices, NUM_FRUSTUM_VERTICES);
}

void BoundingBox::Merge(const Sphere& sphere)
{
    const Vector3& center = sphere.GetCenter();
    float radius = sphere.GetRadius();
    
    Merge(center + Vector3(radius, radius, radius));
    Merge(center + Vector3(-radius, -radius, -radius));
}

void BoundingBox::Clip(const BoundingBox& box)
{
    if (box.m_min.m_x > m_min.m_x)
        m_min.m_x = box.m_min.m_x;
    if (box.m_max.m_x < m_max.m_x)
        m_max.m_x = box.m_max.m_x;
    if (box.m_min.m_y > m_min.m_y)
        m_min.m_y = box.m_min.m_y;
    if (box.m_max.m_y < m_max.m_y)
        m_max.m_y = box.m_max.m_y;
    if (box.m_min.m_z > m_min.m_z)
        m_min.m_z = box.m_min.m_z;
    if (box.m_max.m_z < m_max.m_z)
        m_max.m_z = box.m_max.m_z;
    
    if (m_min.m_x > m_max.m_x)
        swap(m_min.m_x, m_max.m_x);
    if (m_min.m_y > m_max.m_y)
    	swap(m_min.m_y, m_max.m_y);
    if (m_min.m_z > m_max.m_z)
    	swap(m_min.m_z, m_max.m_z);
}

void BoundingBox::Transform(const Matrix3& transform)
{
    *this = Transformed(Matrix3x4(transform));
}

void BoundingBox::Transform(const Matrix3x4& transform)
{
    *this = Transformed(transform);
}

BoundingBox BoundingBox::Transformed(const Matrix3& transform) const
{
    return Transformed(Matrix3x4(transform));
}

BoundingBox BoundingBox::Transformed(const Matrix3x4& transform) const
{
    Vector3 newCenter = transform * Center();
    Vector3 oldEdge = Size() * 0.5f;
    Vector3 newEdge = Vector3(
        Abs(transform.m_m00) * oldEdge.m_x + Abs(transform.m_m01) * oldEdge.m_y + Abs(transform.m_m02) * oldEdge.m_z,
        Abs(transform.m_m10) * oldEdge.m_x + Abs(transform.m_m11) * oldEdge.m_y + Abs(transform.m_m12) * oldEdge.m_z,
        Abs(transform.m_m20) * oldEdge.m_x + Abs(transform.m_m21) * oldEdge.m_y + Abs(transform.m_m22) * oldEdge.m_z
    );
    
    return BoundingBox(newCenter - newEdge, newCenter + newEdge);
}

Rect BoundingBox::Projected(const Matrix4& projection) const
{
    Vector3 projMin = m_min;
    Vector3 projMax = m_max;
    if (projMin.m_z < M_MIN_NEARCLIP)
        projMin.m_z = M_MIN_NEARCLIP;
    if (projMax.m_z < M_MIN_NEARCLIP)
        projMax.m_z = M_MIN_NEARCLIP;
    
    Vector3 vertices[8];
    vertices[0] = projMin;
    vertices[1] = Vector3(projMax.m_x, projMin.m_y, projMin.m_z);
    vertices[2] = Vector3(projMin.m_x, projMax.m_y, projMin.m_z);
    vertices[3] = Vector3(projMax.m_x, projMax.m_y, projMin.m_z);
    vertices[4] = Vector3(projMin.m_x, projMin.m_y, projMax.m_z);
    vertices[5] = Vector3(projMax.m_x, projMin.m_y, projMax.m_z);
    vertices[6] = Vector3(projMin.m_x, projMax.m_y, projMax.m_z);
    vertices[7] = projMax;
    
    Rect rect;
    for (unsigned i = 0; i < 8; ++i)
    {
        Vector3 projected = projection * vertices[i];
        rect.Merge(Vector2(projected.m_x, projected.m_y));
    }
    
    return rect;
}

BoundingBox BoundingBox::Projected(const Plane& plane) const
{
	BoundingBox box;

    Vector3 projMin = m_min;
    Vector3 projMax = m_max;
    Vector3 vertices[8];
    vertices[0] = projMin;
    vertices[1] = Vector3(projMax.m_x, projMin.m_y, projMin.m_z);
    vertices[2] = Vector3(projMin.m_x, projMax.m_y, projMin.m_z);
    vertices[3] = Vector3(projMax.m_x, projMax.m_y, projMin.m_z);
    vertices[4] = Vector3(projMin.m_x, projMin.m_y, projMax.m_z);
    vertices[5] = Vector3(projMax.m_x, projMin.m_y, projMax.m_z);
    vertices[6] = Vector3(projMin.m_x, projMax.m_y, projMax.m_z);
    vertices[7] = projMax;

    for (unsigned i = 0; i < 8; ++i)
    {
        Vector3 projected = plane.Project(vertices[i]);
        box.Merge(Vector3(projected.m_x, projected.m_y,projected.m_z));
    }

    return box;
}

Intersection BoundingBox::IsInside(const Sphere& sphere) const
{
    float distSquared = 0;
    float temp;
    const Vector3& center = sphere.GetCenter();
    
    if (center.m_x < m_min.m_x)
    {
        temp = center.m_x - m_min.m_x;
        distSquared += temp * temp;
    }
    else if (center.m_x > m_max.m_x)
    {
        temp = center.m_x - m_max.m_x;
        distSquared += temp * temp;
    }
    if (center.m_y < m_min.m_y)
    {
        temp = center.m_y - m_min.m_y;
        distSquared += temp * temp;
    }
    else if (center.m_y > m_max.m_y)
    {
        temp = center.m_y - m_max.m_y;
        distSquared += temp * temp;
    }
    if (center.m_z < m_min.m_z)
    {
        temp = center.m_z - m_min.m_z;
        distSquared += temp * temp;
    }
    else if (center.m_z > m_max.m_z)
    {
        temp = center.m_z - m_max.m_z;
        distSquared += temp * temp;
    }
    
    float radius = sphere.GetRadius();
    if (distSquared >= radius * radius)
        return OUTSIDE;
    else if (center.m_x - radius < m_min.m_x || center.m_x + radius > m_max.m_x || center.m_y - radius < m_min.m_y ||
        center.m_y + radius > m_max.m_y || center.m_z - radius < m_min.m_z || center.m_z + radius > m_max.m_z)
        return INTERSECTS;
    else
        return INSIDE;
}

Intersection BoundingBox::IsInsideFast(const Sphere& sphere) const
{
    float distSquared = 0;
    float temp;
    const Vector3& center = sphere.GetCenter();
    
    if (center.m_x < m_min.m_x)
    {
        temp = center.m_x - m_min.m_x;
        distSquared += temp * temp;
    }
    else if (center.m_x > m_max.m_x)
    {
        temp = center.m_x - m_max.m_x;
        distSquared += temp * temp;
    }
    if (center.m_y < m_min.m_y)
    {
        temp = center.m_y - m_min.m_y;
        distSquared += temp * temp;
    }
    else if (center.m_y > m_max.m_y)
    {
        temp = center.m_y - m_max.m_y;
        distSquared += temp * temp;
    }
    if (center.m_z < m_min.m_z)
    {
        temp = center.m_z - m_min.m_z;
        distSquared += temp * temp;
    }
    else if (center.m_z > m_max.m_z)
    {
        temp = center.m_z - m_max.m_z;
        distSquared += temp * temp;
    }
    
    float radius = sphere.GetRadius();
    if (distSquared >= radius * radius)
        return OUTSIDE;
    else
        return INSIDE;
}

Intersection BoundingBox::IsInside(const Vector3 & start, const Vector3 & end) const
{
	Vector3 min, max;
	min = max = start;
	if (end.m_x < min.m_x)
		min.m_x = end.m_x;
	if (end.m_y < min.m_y)
		min.m_y = end.m_y;
	if (end.m_z < min.m_z)
		min.m_z = end.m_z;
	if (end.m_x > max.m_x)
		max.m_x = end.m_x;
	if (end.m_y > max.m_y)
		max.m_y = end.m_y;
	if (end.m_z > max.m_z)
		max.m_z = end.m_z;

	if (max.m_x < m_min.m_x || min.m_x > m_max.m_x
		|| max.m_y < m_min.m_y || min.m_y > m_max.m_y
		|| max.m_z < m_min.m_z || min.m_z > m_max.m_z)
		return OUTSIDE;
	else
		return INSIDE;
}

bool BoundingBox::GetTriangleArray(vector<float>& outPoints)
{
	outPoints.resize(12 * 9);
	vector<float> points;
	this->GetVertexs(points);

	float tmpP[4];
	for (int i = 0; i < 12; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			int index = BoundingBox::triIndex[i][j] * 3;
			tmpP[0] = points[index];
			tmpP[1] = points[index + 1];
			tmpP[2] = points[index + 2];
			tmpP[3] = 1;
			outPoints[i * 9 + j * 3 + 0] = tmpP[0] / tmpP[3];
			outPoints[i * 9 + j * 3 + 1] = tmpP[1] / tmpP[3];
			outPoints[i * 9 + j * 3 + 2] = tmpP[2] / tmpP[3];
		}
	}
	return m_pointsArray.size()>0;
}

bool BoundingBox::GetVertexs(vector<float>& outPoints)
{
	outPoints.resize(24);
	outPoints[0] = m_max.m_x;
	outPoints[1] = m_max.m_y;
	outPoints[2] = m_max.m_z;

	outPoints[3] = m_min.m_x;
	outPoints[4] = m_max.m_y;
	outPoints[5] = m_max.m_z;

	outPoints[6] = m_min.m_x;
	outPoints[7] = m_min.m_y;
	outPoints[8] = m_max.m_z;

	outPoints[9] = m_max.m_x;
	outPoints[10] = m_min.m_y;
	outPoints[11] = m_max.m_z;

	outPoints[12] = m_max.m_x;
	outPoints[13] = m_min.m_y;
	outPoints[14] = m_min.m_z;

	outPoints[15] = m_max.m_x;
	outPoints[16] = m_max.m_y;
	outPoints[17] = m_min.m_z;

	outPoints[18] = m_min.m_x;
	outPoints[19] = m_max.m_y;
	outPoints[20] = m_min.m_z;

	outPoints[21] = m_min.m_x;
	outPoints[22] = m_min.m_y;
	outPoints[23] = m_min.m_z;

	return outPoints.size()>0;
}

string BoundingBox::Tostring() const
{
    return m_min.Tostring() + " - " + m_max.Tostring();
}

void BoundingBox::ScaleBox(float scale)
{
	//TODO 不对 废弃
  Vector3 center = Center();

  Vector3 maxDir = (m_max-center).Normalized();
  Vector3 minDir = (m_min - center).Normalized();

  Vector3 newMax = center + maxDir * (m_max-center).Length()*scale;
  Vector3 newMin = center + minDir * (m_min-center).Length()*scale;

  m_max = newMax;
  m_min = newMin;

}

}
