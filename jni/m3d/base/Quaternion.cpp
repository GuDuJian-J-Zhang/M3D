
#include "m3d/base/Quaternion.h"
#include "m3d/graphics/CameraNode.h"

#include <cstdio>

namespace M3D
{

const Quaternion Quaternion::IDENTITY;


bool Quaternion::IsZero() const
{
	return (m_x*m_x + m_y*m_y + m_z*m_z) < 0.00000001f;
}

void Quaternion::FromAngleAxis(float angle, const Vector3& axis)
{
    Vector3 normAxis = axis.Normalized();
    angle *= M_DEGTORAD_2;
    float sinAngle = sinf(angle);
    float cosAngle = cosf(angle);
    
    m_w = cosAngle;
    m_x = normAxis.m_x * sinAngle;
    m_y = normAxis.m_y * sinAngle;
    m_z = normAxis.m_z * sinAngle;
}

void Quaternion::FromEulerAngles(float x, float y, float z)
{
    // Order of rotations: Z first, then X, then Y (mimics typical FPS camera with gimbal lock at top/bottom)
    x *= M_DEGTORAD_2;
    y *= M_DEGTORAD_2;
    z *= M_DEGTORAD_2;
    float sinX = sinf(x);
    float cosX = cosf(x);
    float sinY = sinf(y);
    float cosY = cosf(y);
    float sinZ = sinf(z);
    float cosZ = cosf(z);
    
    m_w = cosY * cosX * cosZ + sinY * sinX * sinZ;
    m_x = cosY * sinX * cosZ + sinY * cosX * sinZ;
    m_y = sinY * cosX * cosZ - cosY * sinX * sinZ;
    m_z = cosY * cosX * sinZ - sinY * sinX * cosZ;
}

void Quaternion::FromRotationTo(const Vector3& start, const Vector3& end)
{
    Vector3 normStart = start.Normalized();
    Vector3 normEnd = end.Normalized();
    float d = normStart.DotProduct(normEnd);
    
    if (d > -1.0f + M_EPSILON)
    {
        Vector3 c = normStart.CrossProduct(normEnd);
        float s = sqrtf((1.0f + d) * 2.0f);
        float invS = 1.0f / s;
        
        m_x = c.m_x * invS;
        m_y = c.m_y * invS;
        m_z = c.m_z * invS;
        m_w = 0.5f * s;
    }
    else
    {
        Vector3 axis = Vector3::RIGHT.CrossProduct(normStart);
        if (axis.Length() < M_EPSILON)
            axis = Vector3::UP.CrossProduct(normStart);
        
        FromAngleAxis(180.f, axis);
    }
}

void Quaternion::FromAxes(const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis)
{
    Matrix3 matrix(
        xAxis.m_x, yAxis.m_x, zAxis.m_x,
        xAxis.m_y, yAxis.m_y, zAxis.m_y,
        xAxis.m_z, yAxis.m_z, zAxis.m_z
    );
    
    FromRotationMatrix(matrix);
}

void Quaternion::FromRotationMatrix(const Matrix3& matrix)
{
	 float t = matrix.m_m00 + matrix.m_m11 + matrix.m_m22;

	    if (t > 0.0f)
	    {
	        float invS = 0.5f / sqrtf(1.0f + t);

	        m_x = (matrix.m_m21 - matrix.m_m12) * invS;
	        m_y = (matrix.m_m02 - matrix.m_m20) * invS;
	        m_z = (matrix.m_m10 - matrix.m_m01) * invS;
	        m_w = 0.25f / invS;
	    }
	    else
	    {
	        if (matrix.m_m00 > matrix.m_m11 && matrix.m_m00 > matrix.m_m22)
	        {
	            float invS = 0.5f / sqrtf(1.0f + matrix.m_m00 - matrix.m_m11 - matrix.m_m22);

	            m_x = 0.25f / invS;
	            m_y = (matrix.m_m01 + matrix.m_m10) * invS;
	            m_z = (matrix.m_m20 + matrix.m_m02) * invS;
	            m_w = (matrix.m_m21 - matrix.m_m12) * invS;
	        }
	        else if (matrix.m_m11 > matrix.m_m22)
	        {
	            float invS = 0.5f / sqrtf(1.0f + matrix.m_m11 - matrix.m_m00 - matrix.m_m22);

	            m_x = (matrix.m_m01 + matrix.m_m10) * invS;
	            m_y = 0.25f / invS;
	            m_z = (matrix.m_m12 + matrix.m_m21) * invS;
	            m_w = (matrix.m_m02 - matrix.m_m20) * invS;
	        }
	        else
	        {
	            float invS = 0.5f / sqrtf(1.0f + matrix.m_m22 - matrix.m_m00 - matrix.m_m11);

	            m_x = (matrix.m_m02 + matrix.m_m20) * invS;
	            m_y = (matrix.m_m12 + matrix.m_m21) * invS;
	            m_z = 0.25f / invS;
	            m_w = (matrix.m_m10 - matrix.m_m01) * invS;
	        }
	    }
}

bool Quaternion::FromLookRotation(const Vector3& direction, const Vector3& upDirection)
{
	Vector3 forward =  direction.Normalized();
	Vector3 up = upDirection.Normalized();
	Vector3 side = forward.CrossProduct(up).Normalized();

	/* Recompute up as: up = side x forward */
	up = side.CrossProduct(forward).Normalized();

    Quaternion ret;
    ret.FromAxes(side, up, -forward);

    if (!ret.IsNaN())
    {
        (*this) = ret;
        return true;
    }
    else
        return false;
}

Vector3 Quaternion::EulerAngles() const
{
    // Derivation from http://www.geometrictools.com/Documentation/EulerAngles.pdf
    // Order of rotations: Z first, then X, then Y
    float check = 2.0f * (-m_y * m_z + m_w * m_x);
    
    if (check < -0.995f)
    {
        return Vector3(
            -90.0f,
            0.0f,
            -atan2f(2.0f * (m_x * m_z - m_w * m_y), 1.0f - 2.0f * (m_y * m_y + m_z * m_z)) * M_RADTODEG
        );
    }
    else if (check > 0.995f)
    {
        return Vector3(
            90.0f,
            0.0f,
            atan2f(2.0f * (m_x * m_z - m_w * m_y), 1.0f - 2.0f * (m_y * m_y + m_z * m_z)) * M_RADTODEG
        );
    }
    else
    {
        return Vector3(
            asinf(check) * M_RADTODEG,
            atan2f(2.0f * (m_x * m_z + m_w * m_y), 1.0f - 2.0f * (m_x * m_x + m_y * m_y)) * M_RADTODEG,
            atan2f(2.0f * (m_x * m_y + m_w * m_z), 1.0f - 2.0f * (m_x * m_x + m_z * m_z)) * M_RADTODEG
        );
    }
}

float Quaternion::YawAngle() const
{
    return EulerAngles().m_y;
}

float Quaternion::PitchAngle() const
{
    return EulerAngles().m_x;
}

float Quaternion::RollAngle() const
{
    return EulerAngles().m_z;
}


Matrix3 Quaternion::RotationMatrix() const
{
	//left hand
//    return Matrix3(
//        1.0f - 2.0f * m_y * m_y - 2.0f * m_z * m_z,
//        2.0f * m_x * m_y + 2.0f * m_w * m_z,
//        2.0f * m_x * m_z - 2.0f * m_w * m_y,
//        2.0f * m_x * m_y - 2.0f * m_w * m_z,
//        1.0f - 2.0f * m_x * m_x - 2.0f * m_z * m_z,
//        2.0f * m_y * m_z + 2.0f * m_w * m_x,
//        2.0f * m_x * m_z + 2.0f * m_w * m_y,
//        2.0f * m_y * m_z - 2.0f * m_w * m_x,
//        1.0f - 2.0f * m_x * m_x - 2.0f * m_y * m_y
//    );
	//right hand
    return Matrix3(
        1.0f - 2.0f * m_y * m_y - 2.0f * m_z * m_z,
        2.0f * m_x * m_y - 2.0f * m_w * m_z,
        2.0f * m_x * m_z + 2.0f * m_w * m_y,
        2.0f * m_x * m_y + 2.0f * m_w * m_z,
        1.0f - 2.0f * m_x * m_x - 2.0f * m_z * m_z,
        2.0f * m_y * m_z - 2.0f * m_w * m_x,
        2.0f * m_x * m_z - 2.0f * m_w * m_y,
        2.0f * m_y * m_z + 2.0f * m_w * m_x,
        1.0f - 2.0f * m_x * m_x - 2.0f * m_y * m_y
    );
}

Quaternion Quaternion::Slerp(Quaternion rhs, float t) const
{
    float cosAngle = DotProduct(rhs);
    // Enable shortest path rotation
    if (cosAngle < 0.0f)
    {
        cosAngle = -cosAngle;
        rhs = -rhs;
    }
    
    float angle = acosf(cosAngle);
    float sinAngle = sinf(angle);
    float t1, t2;
    
    if (sinAngle > 0.001f)
    {
        float invSinAngle = 1.0f / sinAngle;
        t1 = sinf((1.0f - t) * angle) * invSinAngle;
        t2 = sinf(t * angle) * invSinAngle;
    }
    else
    {
        t1 = 1.0f - t;
        t2 = t;
    }
    
    return *this * t1 + rhs * t2;
}

Quaternion Quaternion::Nlerp(Quaternion rhs, float t, bool shortestPath) const
{
    Quaternion result;
    float fCos = DotProduct(rhs);
    if (fCos < 0.0f && shortestPath)
        result = (*this) + (((-rhs) - (*this)) * t);
    else
        result = (*this) + ((rhs - (*this)) * t);
    result.Normalize();
    return result;
}

string Quaternion::Tostring() const
{
    char tempBuffer[CONVERSION_BUFFER_LENGTH];
    sprintf(tempBuffer, "%g %g %g %g", m_w, m_x, m_y, m_z);
    return string(tempBuffer);
}

void
Quaternion::GetValue(Vector3& axis, float & fAngle) const
{
	if ((m_w >= -1.0f) && (m_w <= 1.0f)) {
		float radians = float(acos(m_w)) * 2.0f;
		float scale = static_cast<float>(sin(radians / 2.0f));
		fAngle = radians*M_RADTODEG;
		if (scale != 0.0f) {
			axis.m_x = m_x / scale;
			axis.m_y = m_y / scale;
			axis.m_z = m_z / scale;
			// FIXME: why not just flip the sign on each component according
			// to "scale" and normalize the axis instead? 20010111 mortene.
			return;
		}
	}

	// Quaternion can't be converted to axis and rotation angle, so we just
	// set up values to indicate this.
	axis.m_x = 0.0f;
	axis.m_y = 0.0f;
	axis.m_z = 1.0f;
	fAngle = 0.0f;
}

Vector3 Quaternion::GetAixsValue() const
{
	Vector3 axis;
	if ((m_w >= -1.0f) && (m_w <= 1.0f)) {
		float radians = float(acos(m_w)) * 2.0f;
		float scale = static_cast<float>(sin(radians / 2.0f));
		if (scale != 0.0f) {
			axis.m_x = m_x / scale;
			axis.m_y = m_y / scale;
			axis.m_z = m_z / scale;
			// FIXME: why not just flip the sign on each component according
			// to "scale" and normalize the axis instead? 20010111 mortene.
			return axis;
		}
	}

	// Quaternion can't be converted to axis and rotation angle, so we just
	// set up values to indicate this.
	axis.m_x = 0.0f;
	axis.m_y = 0.0f;
	axis.m_z = 1.0f;

	return axis;
}


float Quaternion::GetAngleValue() const
{
	float fAngle;
	if ((m_w >= -1.0f) && (m_w <= 1.0f)) {
		float radians = float(acos(m_w)) * 2.0f;
		float scale = static_cast<float>(sin(radians / 2.0f));
		fAngle = radians*M_RADTODEG;
		if (scale != 0.0f) {		
			// FIXME: why not just flip the sign on each component according
			// to "scale" and normalize the axis instead? 20010111 mortene.
			return fAngle;
		}
	}
	// Quaternion can't be converted to axis and rotation angle, so we just
	// set up values to indicate this.
	fAngle = 0.0f;

	return fAngle;
}


}
