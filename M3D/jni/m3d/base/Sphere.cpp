
#include "m3d/base/Frustum.h"
//#include "m3d/base/Polyhedron.h"

namespace M3D
{

void Sphere::Define(const Vector3* vertices, unsigned count)
{
    if (!count)
        return;
    
    m_defined = false;
    Merge(vertices, count);
}

void Sphere::Define(const BoundingBox& box)
{
    const Vector3& min = box.m_min;
    const Vector3& max = box.m_max;
    
    m_defined = false;
    Merge(min);
    Merge(Vector3(max.m_x, min.m_y, min.m_z));
    Merge(Vector3(min.m_x, max.m_y, min.m_z));
    Merge(Vector3(max.m_x, max.m_y, min.m_z));
    Merge(Vector3(min.m_x, min.m_y, max.m_z));
    Merge(Vector3(max.m_x, min.m_y, max.m_z));
    Merge(Vector3(min.m_x, max.m_y, max.m_z));
    Merge(max);
}

void Sphere::Define(const Frustum& frustum)
{
    Define(frustum.m_vertices, NUM_FRUSTUM_VERTICES);
}

//void Sphere::Define(const Polyhedron& poly)
//{
//    defined_ = false;
//    Merge(poly);
//}

void Sphere::Merge(const Vector3* vertices, unsigned count)
{
    while (count--)
        Merge(*vertices++);
}

void Sphere::Merge(const BoundingBox& box)
{
    const Vector3& min = box.m_min;
    const Vector3& max = box.m_max;
    
    Merge(min);
    Merge(Vector3(max.m_x, min.m_y, min.m_z));
    Merge(Vector3(min.m_x, max.m_y, min.m_z));
    Merge(Vector3(max.m_x, max.m_y, min.m_z));
    Merge(Vector3(min.m_x, min.m_y, max.m_z));
    Merge(Vector3(max.m_x, min.m_y, max.m_z));
    Merge(Vector3(min.m_x, max.m_y, max.m_z));
    Merge(max);
}

void Sphere::Merge(const Frustum& frustum)
{
    const Vector3* vertices = frustum.m_vertices;
    Merge(vertices, NUM_FRUSTUM_VERTICES);
}

//void Sphere::Merge(const Polyhedron& poly)
//{
//    for (unsigned i = 0; i < poly.faces_.size(); ++i)
//    {
//        const vector<Vector3>& face = poly.faces_[i];
//        if (!face.empty())
//            Merge(&face[0], face.size());
//    }
//}

void Sphere::Merge(const Sphere& sphere)
{
    if (!m_defined)
    {
        m_center = sphere.m_center;
        m_radius = sphere.m_radius;
        m_defined = true;
        return;
    }
    
    Vector3 offset = sphere.m_center - m_center;
    float dist = offset.Length();
    
    // If sphere fits inside, do nothing
    if (dist + sphere.m_radius < m_radius)
        return;
    
    // If we fit inside the other sphere, become it
    if (dist + m_radius < sphere.m_radius)
    {
        m_center = sphere.m_center;
        m_radius = sphere.m_radius;
    }
    else
    {
        Vector3 NormalizedOffset = offset / dist;
        
        Vector3 min = m_center - m_radius * NormalizedOffset;
        Vector3 max = sphere.m_center + sphere.m_radius * NormalizedOffset;
        m_center = (min + max) * 0.5f;
        m_radius = (max - m_center).Length();
    }
}

Intersection Sphere::IsInside(const BoundingBox& box) const
{
    float radiusSquared = m_radius * m_radius;
    float distSquared = 0;
    float temp;
    Vector3 min = box.m_min;
    Vector3 max = box.m_max;
    
    if (m_center.m_x < min.m_x)
    {
        temp = m_center.m_x - min.m_x;
        distSquared += temp * temp;
    }
    else if (m_center.m_x > max.m_x)
    {
        temp = m_center.m_x - max.m_x;
        distSquared += temp * temp;
    }
    if (m_center.m_y < min.m_y)
    {
        temp = m_center.m_y - min.m_y;
        distSquared += temp * temp;
    }
    else if (m_center.m_y > max.m_y)
    {
        temp = m_center.m_y - max.m_y;
        distSquared += temp * temp;
    }
    if (m_center.m_z < min.m_z)
    {
        temp = m_center.m_z - min.m_z;
        distSquared += temp * temp;
    }
    else if (m_center.m_z > max.m_z)
    {
        temp = m_center.m_z - max.m_z;
        distSquared += temp * temp;
    }
    
    if (distSquared >= radiusSquared)
        return OUTSIDE;
    
    min -= m_center;
    max -= m_center;
    
    Vector3 tempVec = min; // - - -
    if (tempVec.LengthSquared() >= radiusSquared)
        return INTERSECTS;
    tempVec.m_x = max.m_x; // + - -
    if (tempVec.LengthSquared() >= radiusSquared)
        return INTERSECTS;
    tempVec.m_y = max.m_y; // + + -
    if (tempVec.LengthSquared() >= radiusSquared)
        return INTERSECTS;
    tempVec.m_x = min.m_x; // - + -
    if (tempVec.LengthSquared() >= radiusSquared)
        return INTERSECTS;
    tempVec.m_z = max.m_z; // - + +
    if (tempVec.LengthSquared() >= radiusSquared)
        return INTERSECTS;
    tempVec.m_y = min.m_y; // - - +
    if (tempVec.LengthSquared() >= radiusSquared)
        return INTERSECTS;
    tempVec.m_x = max.m_x; // + - +
    if (tempVec.LengthSquared() >= radiusSquared)
        return INTERSECTS;
    tempVec.m_y = max.m_y; // + + +
    if (tempVec.LengthSquared() >= radiusSquared)
        return INTERSECTS;
    
    return INSIDE;
}

Intersection Sphere::IsInsideFast(const BoundingBox& box) const
{
    float radiusSquared = m_radius * m_radius;
    float distSquared = 0;
    float temp;
    Vector3 min = box.m_min;
    Vector3 max = box.m_max;
    
    if (m_center.m_x < min.m_x)
    {
        temp = m_center.m_x - min.m_x;
        distSquared += temp * temp;
    }
    else if (m_center.m_x > max.m_x)
    {
        temp = m_center.m_x - max.m_x;
        distSquared += temp * temp;
    }
    if (m_center.m_y < min.m_y)
    {
        temp = m_center.m_y - min.m_y;
        distSquared += temp * temp;
    }
    else if (m_center.m_y > max.m_y)
    {
        temp = m_center.m_y - max.m_y;
        distSquared += temp * temp;
    }
    if (m_center.m_z < min.m_z)
    {
        temp = m_center.m_z - min.m_z;
        distSquared += temp * temp;
    }
    else if (m_center.m_z > max.m_z)
    {
        temp = m_center.m_z - max.m_z;
        distSquared += temp * temp;
    }
    
    if (distSquared >= radiusSquared)
        return OUTSIDE;
    else
        return INSIDE;
}

}
