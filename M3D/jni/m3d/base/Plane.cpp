
#include "m3d/base/Plane.h"
#include "m3d/base/Ray.h"

namespace M3D
{

// Static initialization order can not be relied on, so do not use Vector3 constants
const Plane Plane::UP(Vector3(0.0f, 1.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f));

void Plane::Transform(const Matrix3& transform)
{
    Define(Matrix4(transform).Inverse().Transpose() * ToVector4());
}
void Plane::Transform(const Matrix3x4& transform)
{
    Define(transform.ToMatrix4().Inverse().Transpose() * ToVector4());
}

void Plane::Transform(const Matrix4& transform)
{
    Define(transform.Inverse().Transpose() * ToVector4());
}

Matrix3x4 Plane::ReflectionMatrix() const
{
    return Matrix3x4(
        -2.0f * m_normal.m_x * m_normal.m_x + 1.0f,
        -2.0f * m_normal.m_x * m_normal.m_y,
        -2.0f * m_normal.m_x * m_normal.m_z,
        -2.0f * m_normal.m_x * m_d,
        -2.0f * m_normal.m_y * m_normal.m_x ,
        -2.0f * m_normal.m_y * m_normal.m_y + 1.0f,
        -2.0f * m_normal.m_y * m_normal.m_z,
        -2.0f * m_normal.m_y * m_d,
        -2.0f * m_normal.m_z * m_normal.m_x,
        -2.0f * m_normal.m_z * m_normal.m_y,
        -2.0f * m_normal.m_z * m_normal.m_z + 1.0f,
        -2.0f * m_normal.m_z * m_d
    );
}

Plane Plane::Transformed(const Matrix3& transform) const
{
    return Plane(Matrix4(transform).Inverse().Transpose() * ToVector4());
}

Plane Plane::Transformed(const Matrix3x4& transform) const
{
    return Plane(transform.ToMatrix4().Inverse().Transpose() * ToVector4());
}

Plane Plane::Transformed(const Matrix4& transform) const
{
    return Plane(transform.Inverse().Transpose() * ToVector4());
}

bool Plane::GetInsectPnt(const Ray& ray,Vector3& intersectPnt)
{

	float vpt = m_normal.DotProduct(ray.GetDirection());
	if(fabs(vpt) <1e-8)
	{
		return false;
	}

	//TODO
	Vector3& origin = ray.GetOrigin();

	float t = (-this->m_d - this->m_normal.DotProduct(origin))/vpt;
	if (t>=0.0)
	{
		intersectPnt = origin + ray.GetDirection()*t;
	}
	else
	{
		return false;
	}
	return true;
}

}
