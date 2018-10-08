
#include "m3d/base/BoundingBox.h"
#include "m3d/base/Frustum.h"
#include "m3d/base/Plane.h"
#include "m3d/base/Ray.h"
#include "m3d/base/Sphere.h"
#include "m3d/base/Vector4.h"

namespace M3D
{

Vector3 Ray::ClosestPoint(const Ray& ray) const
{
    // Algorithm based on http://paulbourke.net/geometry/lineline3d/
    Vector3 p13 = m_origin - ray.m_origin;
    Vector3 p43 = ray.m_direction;
    Vector3 p21 = m_direction;
    
    float d1343 = p13.DotProduct(p43);
    float d4321 = p43.DotProduct(p21);
    float d1321 = p13.DotProduct(p21);
    float d4343 = p43.DotProduct(p43);
    float d2121 = p21.DotProduct(p21);
    
    float d = d2121 * d4343 - d4321 * d4321;
    if (Abs(d) < M_EPSILON)
        return m_origin;
    float n = d1343 * d4321 - d1321 * d4343;
    float a = n / d;
    
    return m_origin + a * m_direction;
}

float Ray::HitDistance(const Plane& plane) const
{
    float d = plane.m_normal.DotProduct(m_direction);
    if (Abs(d) >= M_EPSILON)
    {
        float t = -(plane.m_normal.DotProduct(m_origin) + plane.m_d) / d;
        if (t >= 0.0f)
            return t;
        else
            return M_INFINITY;
    }
    else
        return M_INFINITY;
}

float Ray::HitDistance(const BoundingBox& box) const
{
    // If undefined, no hit (infinite distance)
    if (!box.m_defined)
        return M_INFINITY;
    
    // Check for ray origin being inside the box
    if (box.IsInside(m_origin))
        return 0.0f;
    
    float dist = M_INFINITY;
    
    // Check for intersecting in the X-direction
    if (m_origin.m_x < box.m_min.m_x && m_direction.m_x > 0.0f)
    {
        float x = (box.m_min.m_x - m_origin.m_x) / m_direction.m_x;
        if (x < dist)
        {
            Vector3 point = m_origin + x * m_direction;
            if (point.m_y >= box.m_min.m_y && point.m_y <= box.m_max.m_y && point.m_z >= box.m_min.m_z && point.m_z <= box.m_max.m_z)
                dist = x;
        }
    }
    if (m_origin.m_x > box.m_max.m_x && m_direction.m_x < 0.0f)
    {
        float x = (box.m_max.m_x - m_origin.m_x) / m_direction.m_x;
        if (x < dist)
        {
            Vector3 point = m_origin + x * m_direction;
            if (point.m_y >= box.m_min.m_y && point.m_y <= box.m_max.m_y && point.m_z >= box.m_min.m_z && point.m_z <= box.m_max.m_z)
                dist = x;
        }
    }
    // Check for intersecting in the Y-direction
    if (m_origin.m_y < box.m_min.m_y && m_direction.m_y > 0.0f)
    {
        float x = (box.m_min.m_y - m_origin.m_y) / m_direction.m_y;
        if (x < dist)
        {
            Vector3 point = m_origin + x * m_direction;
            if (point.m_x >= box.m_min.m_x && point.m_x <= box.m_max.m_x && point.m_z >= box.m_min.m_z && point.m_z <= box.m_max.m_z)
                dist = x;
        }
    }
    if (m_origin.m_y > box.m_max.m_y && m_direction.m_y < 0.0f)
    {
        float x = (box.m_max.m_y - m_origin.m_y) / m_direction.m_y;
        if (x < dist)
        {
            Vector3 point = m_origin + x * m_direction;
            if (point.m_x >= box.m_min.m_x && point.m_x <= box.m_max.m_x && point.m_z >= box.m_min.m_z && point.m_z <= box.m_max.m_z)
                dist = x;
        }
    }
    // Check for intersecting in the Z-direction
    if (m_origin.m_z < box.m_min.m_z && m_direction.m_z > 0.0f)
    {
        float x = (box.m_min.m_z - m_origin.m_z) / m_direction.m_z;
        if (x < dist)
        {
            Vector3 point = m_origin + x * m_direction;
            if (point.m_x >= box.m_min.m_x && point.m_x <= box.m_max.m_x && point.m_y >= box.m_min.m_y && point.m_y <= box.m_max.m_y)
                dist = x;
        }
    }
    if (m_origin.m_z > box.m_max.m_z && m_direction.m_z < 0.0f)
    {
        float x = (box.m_max.m_z - m_origin.m_z) / m_direction.m_z;
        if (x < dist)
        {
            Vector3 point = m_origin + x * m_direction;
            if (point.m_x >= box.m_min.m_x && point.m_x <= box.m_max.m_x && point.m_y >= box.m_min.m_y && point.m_y <= box.m_max.m_y)
                dist = x;
        }
    }
    
    return dist;
}

float Ray::HitDistance(const Frustum& frustum, bool solidInside) const
{
    float maxOutside = 0.0f;
    float minInside = M_INFINITY;
    bool allInside = true;
    
    for (unsigned i = 0; i < NUM_FRUSTUM_PLANES; ++i)
    {
        const Plane& plane = frustum.m_planes[i];
        float distance = HitDistance(frustum.m_planes[i]);
        
        if (plane.Distance(m_origin) < 0.0f)
        {
            maxOutside = Max(maxOutside, distance);
            allInside = false;
        }
        else
            minInside = Min(minInside, distance);
    }
    
    if (allInside)
        return solidInside ? 0.0f : minInside;
    else if (maxOutside <= minInside)
        return maxOutside;
    else
        return M_INFINITY;
}

float Ray::HitDistance(const Sphere& sphere) const
{
    Vector3 centeredOrigin = m_origin - sphere.m_center;
    float squaredRadius = sphere.m_radius * sphere.m_radius;
    
    // Check if ray originates inside the sphere
    if (centeredOrigin.LengthSquared() <= squaredRadius)
        return 0.0f;
    
    // Calculate intersection by quadratic equation
    float a = m_direction.DotProduct(m_direction);
    float b = 2.0f * centeredOrigin.DotProduct(m_direction);
    float c = centeredOrigin.DotProduct(centeredOrigin) - squaredRadius;
    float d = b * b - 4.0f * a * c;
    
    // No solution
    if (d < 0.0f)
        return M_INFINITY;
    
    // Get the nearer solution
    float dSqrt = sqrtf(d);
    float dist = (-b - dSqrt) / (2.0f * a);
    if (dist >= 0.0f)
        return dist;
    else
        return (-b + dSqrt) / (2.0f * a);
}

float Ray::HitDistance(const Vector3& v0, const Vector3& v1, const Vector3& v2) const
{
    return HitDistance(v0, v1, v2, 0);
}

float Ray::HitDistance(const Vector3& v0, const Vector3& v1, const Vector3& v2, Vector3* outNormal) const
{
    // Based on Fast, Minimum Storage Ray/Triangle Intersection by M�ller & Trumbore
    // http://www.graphics.cornell.edu/pubs/1997/MT97.pdf
    // Calculate edge vectors
    Vector3 edge1(v1 - v0);
    Vector3 edge2(v2 - v0);
    
    // Calculate determinant & check backfacing
    Vector3 p(m_direction.CrossProduct(edge2));
    float det = edge1.DotProduct(p);
    if (det >= M_EPSILON)
    {
        // Calculate u & v parameters and test
        Vector3 t(m_origin - v0);
        float u = t.DotProduct(p);
        if (u >= 0.0f && u <= det)
        {
            Vector3 q(t.CrossProduct(edge1));
            float v = m_direction.DotProduct(q);
            if (v >= 0.0f && u + v <= det)
            {
                float distance = edge2.DotProduct(q) / det;
                // Discard hits behind the ray
                if (distance >= 0.0f)
                {
                    // There is an intersection, so calculate distance & optional normal
                    if (outNormal)
                        *outNormal = edge1.CrossProduct(edge2);
                    
                    return distance;
                }
            }
        }
    }
    
    return M_INFINITY;
}

float Ray::HitDistance(const void* vertexData, unsigned verteSize, unsigned vertexStart, unsigned vertexCount, Vector3* outNormal) const
{
    float nearest = M_INFINITY;
    const unsigned char* vertices = ((const unsigned char*)vertexData) + vertexStart * verteSize;
    unsigned index = 0;
    
    while (index + 2 < vertexCount)
    {
        const Vector3& v0 = *((const Vector3*)(&vertices[index * verteSize]));
        const Vector3& v1 = *((const Vector3*)(&vertices[(index + 1) * verteSize]));
        const Vector3& v2 = *((const Vector3*)(&vertices[(index + 2) * verteSize]));
        nearest = Min(nearest, HitDistance(v0, v1, v2, outNormal));
        index += 3;
    }
    
    return nearest;
}

float Ray::HitDistance(const void* vertexData, unsigned verteSize, const void* indexData, unsigned indeSize,
    unsigned indexStart, unsigned indexCount, Vector3* outNormal) const
{
    float nearest = M_INFINITY;
    const unsigned char* vertices = (const unsigned char*)vertexData;
    
    // 16-bit indices
    if (indeSize == sizeof(M3D_INDEX_TYPE))
    {
        const M3D_INDEX_TYPE* indices = ((const M3D_INDEX_TYPE*)indexData) + indexStart;
        const M3D_INDEX_TYPE* indicesEnd = indices + indexCount;
        
        while (indices < indicesEnd)
        {
            const Vector3& v0 = *((const Vector3*)(&vertices[indices[0] * verteSize]));
            const Vector3& v1 = *((const Vector3*)(&vertices[indices[1] * verteSize]));
            const Vector3& v2 = *((const Vector3*)(&vertices[indices[2] * verteSize]));
            nearest = Min(nearest, HitDistance(v0, v1, v2, outNormal));
            indices += 3;
        }
    }
    // 32-bit indices
    else
    {
        const unsigned* indices = ((const unsigned*)indexData) + indexStart;
        const unsigned* indicesEnd = indices + indexCount;
        
        while (indices < indicesEnd)
        {
            const Vector3& v0 = *((const Vector3*)(&vertices[indices[0] * verteSize]));
            const Vector3& v1 = *((const Vector3*)(&vertices[indices[1] * verteSize]));
            const Vector3& v2 = *((const Vector3*)(&vertices[indices[2] * verteSize]));
            nearest = Min(nearest, HitDistance(v0, v1, v2, outNormal));
            indices += 3;
        }
    }
    
    return nearest;
}

bool Ray::InsideGeometry(const void* vertexData, unsigned verteSize, unsigned vertexStart, unsigned vertexCount) const
{
    float currenFrontFace = M_INFINITY;
    float currentBackFace = M_INFINITY;
    const unsigned char* vertices = ((const unsigned char*)vertexData) + vertexStart * verteSize;
    unsigned index = 0;
    
    while (index + 2 < vertexCount)
    {
        const Vector3& v0 = *((const Vector3*)(&vertices[index * verteSize]));
        const Vector3& v1 = *((const Vector3*)(&vertices[(index + 1) * verteSize]));
        const Vector3& v2 = *((const Vector3*)(&vertices[(index + 2) * verteSize]));
        float frontFaceDistance = HitDistance(v0, v1, v2);
        float backFaceDistance = HitDistance(v2, v1, v0);
        currenFrontFace = Min(frontFaceDistance > 0.0f ? frontFaceDistance : M_INFINITY, currenFrontFace);
        // A backwards face is just a regular one, with the vertices in the opposite order. This essentially checks backfaces by
        // checking reversed frontfaces
        currentBackFace = Min(backFaceDistance > 0.0f ? backFaceDistance : M_INFINITY, currentBackFace);
        index += 3;
    }
    
    // If the closest face is a backface, that means that the ray originates from the inside of the geometry
    // NOTE: there may be cases where both are equal, as in, no collision to either. This is prevented in the most likely case
    // (ray doesnt hit either) by this conditional
    if (currenFrontFace != M_INFINITY || currentBackFace != M_INFINITY)
        return currentBackFace < currenFrontFace;
    
    // It is still possible for two triangles to be equally distant from the triangle, however, this is extremely unlikely.
    // As such, it is safe to assume they are not
    return false;
}

bool Ray::InsideGeometry(const void* vertexData, unsigned verteSize, const void* indexData, unsigned indeSize,
    unsigned indexStart, unsigned indexCount) const
{
    float currenFrontFace = M_INFINITY;
    float currentBackFace = M_INFINITY;
    const unsigned char* vertices = (const unsigned char*)vertexData;
    
    // 16-bit indices
    if (indeSize == sizeof(M3D_INDEX_TYPE))
    {
        const M3D_INDEX_TYPE* indices = ((const M3D_INDEX_TYPE*)indexData) + indexStart;
        const M3D_INDEX_TYPE* indicesEnd = indices + indexCount;
        
        while (indices < indicesEnd)
        {
            const Vector3& v0 = *((const Vector3*)(&vertices[indices[0] * verteSize]));
            const Vector3& v1 = *((const Vector3*)(&vertices[indices[1] * verteSize]));
            const Vector3& v2 = *((const Vector3*)(&vertices[indices[2] * verteSize]));
            float frontFaceDistance = HitDistance(v0, v1, v2);
            float backFaceDistance = HitDistance(v2, v1, v0);
            currenFrontFace = Min(frontFaceDistance > 0.0f ? frontFaceDistance : M_INFINITY, currenFrontFace);
            // A backwards face is just a regular one, with the vertices in the opposite order. This essentially checks backfaces by
            // checking reversed frontfaces
            currentBackFace = Min(backFaceDistance > 0.0f ? backFaceDistance : M_INFINITY, currentBackFace);
            indices += 3;
        }
    }
    // 32-bit indices
    else
    {
        const unsigned* indices = ((const unsigned*)indexData) + indexStart;
        const unsigned* indicesEnd = indices + indexCount;
        
        while (indices < indicesEnd)
        {
            const Vector3& v0 = *((const Vector3*)(&vertices[indices[0] * verteSize]));
            const Vector3& v1 = *((const Vector3*)(&vertices[indices[1] * verteSize]));
            const Vector3& v2 = *((const Vector3*)(&vertices[indices[2] * verteSize]));
            float frontFaceDistance = HitDistance(v0, v1, v2);
            float backFaceDistance = HitDistance(v2, v1, v0);
            currenFrontFace = Min(frontFaceDistance > 0.0f ? frontFaceDistance : M_INFINITY, currenFrontFace);
            // A backwards face is just a regular one, with the vertices in the opposite order. This essentially checks backfaces by
            // checking reversed frontfaces
            currentBackFace = Min(backFaceDistance > 0.0f ? backFaceDistance : M_INFINITY, currentBackFace);
            indices += 3; 
        }
    }
    
    // If the closest face is a backface, that means that the ray originates from the inside of the geometry
    // NOTE: there may be cases where both are equal, as in, no collision to either. This is prevented in the most likely case
    // (ray doesnt hit either) by this conditional
    if (currenFrontFace != M_INFINITY || currentBackFace != M_INFINITY)
        return currentBackFace < currenFrontFace;
    
    // It is still possible for two triangles to be equally distant from the triangle, however, this is extremely unlikely.
    // As such, it is safe to assume they are not
    return false;
}

Ray Ray::Transformed(const Matrix3x4& transform) const
{
    Ray ret;
    ret.m_origin = transform * m_origin;
    ret.m_direction = transform * Vector4(m_direction, 0.0f);
    return ret;
}

Vector3& Ray::GetDirection()const
{
	return this->m_direction;
}

Vector3& Ray::GetOrigin()const
{
	return this->m_origin;
}
}
