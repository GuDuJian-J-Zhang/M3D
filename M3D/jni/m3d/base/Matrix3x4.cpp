
#include "m3d/base/Matrix3x4.h"

#include <cstdio>


namespace M3D
{

const Matrix3x4 Matrix3x4::ZERO(
	    0.0f, 0.0f, 0.0f, 0.0f,
	    0.0f, 0.0f, 0.0f, 0.0f,
	    0.0f, 0.0f, 0.0f, 0.0f);

const Matrix3x4 Matrix3x4::IDENTITY;

Matrix3x4::Matrix3x4(const Vector3& translation, const Quaternion& rotation, float scale)
{
	   SetRotation(rotation.RotationMatrix() * scale);
	    SetTranslation(translation);
}

Matrix3x4::Matrix3x4(const Vector3& translation, const Quaternion& rotation, const Vector3& scale)
{
    SetRotation(rotation.RotationMatrix().Scaled(scale));
    SetTranslation(translation);
}

void Matrix3x4::Decompose(Vector3& translation, Quaternion& rotation, Vector3& scale) const
{
    translation.m_x = m_m03;
    translation.m_y = m_m13;
    translation.m_z = m_m23;
    
    scale.m_x = sqrtf(m_m00 * m_m00 + m_m10 * m_m10 + m_m20 * m_m20);
    scale.m_y = sqrtf(m_m01 * m_m01 + m_m11 * m_m11 + m_m21 * m_m21);
    scale.m_z = sqrtf(m_m02 * m_m02 + m_m12 * m_m12 + m_m22 * m_m22);
    
    Vector3 invScale(1.0f / scale.m_x, 1.0f / scale.m_y, 1.0f / scale.m_z);
    rotation = Quaternion(ToMatrix3().Scaled(invScale));
}

Matrix3x4 Matrix3x4::Inverse() const
{
    float det = m_m00 * m_m11 * m_m22 +
        m_m10 * m_m21 * m_m02 +
        m_m20 * m_m01 * m_m12 -
        m_m20 * m_m11 * m_m02 -
        m_m10 * m_m01 * m_m22 -
        m_m00 * m_m21 * m_m12;
    
    float invDet = 1.0f / det;
    Matrix3x4 ret;
    
    ret.m_m00 = (m_m11 * m_m22 - m_m21 * m_m12) * invDet;
    ret.m_m01 = -(m_m01 * m_m22 - m_m21 * m_m02) * invDet;
    ret.m_m02 = (m_m01 * m_m12 - m_m11 * m_m02) * invDet;
    ret.m_m03 = -(m_m03 * ret.m_m00 + m_m13 * ret.m_m01 + m_m23 * ret.m_m02);
    ret.m_m10 = -(m_m10 * m_m22 - m_m20 * m_m12) * invDet;
    ret.m_m11 = (m_m00 * m_m22 - m_m20 * m_m02) * invDet;
    ret.m_m12 = -(m_m00 * m_m12 - m_m10 * m_m02) * invDet;
    ret.m_m13 = -(m_m03 * ret.m_m10 + m_m13 * ret.m_m11 + m_m23 * ret.m_m12);
    ret.m_m20 = (m_m10 * m_m21 - m_m20 * m_m11) * invDet;
    ret.m_m21 = -(m_m00 * m_m21 - m_m20 * m_m01) * invDet;
    ret.m_m22 = (m_m00 * m_m11 - m_m10 * m_m01) * invDet;
    ret.m_m23 = -(m_m03 * ret.m_m20 + m_m13 * ret.m_m21 + m_m23 * ret.m_m22);
    
    return ret;
}

void Matrix3x4::MultiTranslate(const Vector3& mov)
{
	Matrix3x4 temp;
	temp.SetTranslation(mov);
	*this = (*this)*temp ;
}

void Matrix3x4::MultiRotatiton(const Quaternion& rotation)
{
	Matrix3x4 temp;
	temp.SetRotation(rotation.RotationMatrix());
	*this = (*this)*temp ;
}

void Matrix3x4::MultiScale(float scale)
{
	Matrix3x4 temp;
	temp.SetScale(scale);
	*this = (*this)*temp ;
}

void Matrix3x4::MultiScale(const Vector3& scale)
{
	Matrix3x4 temp;
	temp.SetScale(scale);
	*this = (*this)*temp ;
}

string Matrix3x4::ToString() const
{
    char tempBuffer[MATRIX_CONVERSION_BUFFER_LENGTH];
    sprintf(tempBuffer, "%g %g %g %g %g %g %g %g %g %g %g %g", m_m00, m_m01, m_m02, m_m03, m_m10, m_m11, m_m12, m_m13, m_m20, m_m21, m_m22,
        m_m23);
    return string(tempBuffer);
}

}
