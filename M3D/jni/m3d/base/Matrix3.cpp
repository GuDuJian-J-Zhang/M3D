
#include "m3d/base/MathDefs.h"
#include "m3d/base/Matrix3.h"


namespace M3D
{

const Matrix3 Matrix3::ZERO(
    0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f);

const Matrix3 Matrix3::IDENTITY;

Matrix3 Matrix3::Inverse() const
{
    float det = m_m00 * m_m11 * m_m22 +
        m_m10 * m_m21 * m_m02 +
        m_m20 * m_m01 * m_m12 -
        m_m20 * m_m11 * m_m02 -
        m_m10 * m_m01 * m_m22 -
        m_m00 * m_m21 * m_m12;
    
    float invDet = 1.0f / det;
    
    return Matrix3(
        (m_m11 * m_m22 - m_m21 * m_m12) * invDet,
        -(m_m01 * m_m22 - m_m21 * m_m02) * invDet,
        (m_m01 * m_m12 - m_m11 * m_m02) * invDet,
        -(m_m10 * m_m22 - m_m20 * m_m12) * invDet,
        (m_m00 * m_m22 - m_m20 * m_m02) * invDet,
        -(m_m00 * m_m12 - m_m10 * m_m02) * invDet,
        (m_m10 * m_m21 - m_m20 * m_m11) * invDet,
        -(m_m00 * m_m21 - m_m20 * m_m01) * invDet,
        (m_m00 * m_m11 - m_m10 * m_m01) * invDet
    );
}

string Matrix3::Tostring() const
{
    char tempBuffer[MATRIX_CONVERSION_BUFFER_LENGTH];
    sprintf(tempBuffer, "%g %g %g %g %g %g %g %g %g", m_m00, m_m01, m_m02, m_m10, m_m11, m_m12, m_m20, m_m21, m_m22);
    return string(tempBuffer);
}

Matrix3::Matrix3() :
	m_m00(1.0f),
	m_m01(0.0f),
	m_m02(0.0f),
	m_m10(0.0f),
	m_m11(1.0f),
	m_m12(0.0f),
	m_m20(0.0f),
	m_m21(0.0f),
	m_m22(1.0f)
{

}

Matrix3::Matrix3( const Matrix3& matrix ) :
	m_m00(matrix.m_m00),
	m_m01(matrix.m_m01),
	m_m02(matrix.m_m02),
	m_m10(matrix.m_m10),
	m_m11(matrix.m_m11),
	m_m12(matrix.m_m12),
	m_m20(matrix.m_m20),
	m_m21(matrix.m_m21),
	m_m22(matrix.m_m22)
{

}

Matrix3::Matrix3( float v00, float v01, float v02, float v10, float v11, float v12, float v20, float v21, float v22 ) :
	m_m00(v00),
	m_m01(v01),
	m_m02(v02),
	m_m10(v10),
	m_m11(v11),
	m_m12(v12),
	m_m20(v20),
	m_m21(v21),
	m_m22(v22)
{

}

Matrix3::Matrix3( const float* data ) :
	m_m00(data[0]),
	m_m01(data[1]),
	m_m02(data[2]),
	m_m10(data[3]),
	m_m11(data[4]),
	m_m12(data[5]),
	m_m20(data[6]),
	m_m21(data[7]),
	m_m22(data[8])
{

}

Matrix3& Matrix3::operator=( const Matrix3& rhs )
{
	m_m00 = rhs.m_m00;
	m_m01 = rhs.m_m01;
	m_m02 = rhs.m_m02;
	m_m10 = rhs.m_m10;
	m_m11 = rhs.m_m11;
	m_m12 = rhs.m_m12;
	m_m20 = rhs.m_m20;
	m_m21 = rhs.m_m21;
	m_m22 = rhs.m_m22;
	return *this;
}

bool Matrix3::operator==( const Matrix3& rhs ) const
{
	const float* leftData = Data();
	const float* rightData = rhs.Data();

	for (unsigned i = 0; i < 9; ++i)
	{
		if (leftData[i] != rightData[i])
			return false;
	}

	return true;
}

void Matrix3::SetScale( const Vector3& scale )
{
    m_m00 = scale.m_x;
    m_m11 = scale.m_y;
    m_m22 = scale.m_z;
}

void Matrix3::SetScale( float scale )
{
	  m_m00 = scale;
	        m_m11 = scale;
	        m_m22 = scale;
}

M3D::Vector3 Matrix3::Scale() const
{
    return Vector3(
        sqrtf(m_m00 * m_m00 + m_m10 * m_m10 + m_m20 * m_m20),
        sqrtf(m_m01 * m_m01 + m_m11 * m_m11 + m_m21 * m_m21),
        sqrtf(m_m02 * m_m02 + m_m12 * m_m12 + m_m22 * m_m22)
    );
}

M3D::Matrix3 Matrix3::Transpose() const
{
    return Matrix3(
        m_m00,
        m_m10,
        m_m20,
        m_m01,
        m_m11,
        m_m21,
        m_m02,
        m_m12,
        m_m22
    );
}

M3D::Matrix3 Matrix3::Scaled( const Vector3& scale ) const
{
    return Matrix3(
        m_m00 * scale.m_x,
        m_m01 * scale.m_y,
        m_m02 * scale.m_z,
        m_m10 * scale.m_x,
        m_m11 * scale.m_y,
        m_m12 * scale.m_z,
        m_m20 * scale.m_x,
        m_m21 * scale.m_y,
        m_m22 * scale.m_z
    );
}

void Matrix3::BulkTranspose( float* dest, const float* src, unsigned count )
{
    for (unsigned i = 0; i < count; ++i)
    {
        dest[0] = src[0];
        dest[1] = src[3];
        dest[2] = src[6];
        dest[3] = src[1];
        dest[4] = src[4];
        dest[5] = src[7];
        dest[6] = src[2];
        dest[7] = src[5];
        dest[8] = src[8];

        dest += 9;
        src += 9;
    }
}

const float* Matrix3::Data() const
{
	return &m_m00;
}

bool Matrix3::Equals( const Matrix3& rhs ) const
{
	const float* leftData = Data();
	const float* rightData = rhs.Data();

	for (unsigned i = 0; i < 9; ++i)
	{
		if (!M3D::Equals(leftData[i], rightData[i]))
			return false;
	}

	return true;
}
/// Multiply a Vector3.
Vector3  Matrix3::operator * (const Vector3& rhs) const
{
    return Vector3(
        m_m00 * rhs.m_x + m_m01 * rhs.m_y + m_m02 * rhs.m_z,
        m_m10 * rhs.m_x + m_m11 * rhs.m_y + m_m12 * rhs.m_z,
        m_m20 * rhs.m_x + m_m21 * rhs.m_y + m_m22 * rhs.m_z
    );
}
Vector3  Matrix3::Multiply(const Vector3& rhs) const
{
	return Vector3(
		m_m00 * rhs.m_x + m_m01 * rhs.m_y + m_m02 * rhs.m_z,
		m_m10 * rhs.m_x + m_m11 * rhs.m_y + m_m12 * rhs.m_z,
		m_m20 * rhs.m_x + m_m21 * rhs.m_y + m_m22 * rhs.m_z
	);
}
/// Add a matrix.
Matrix3  Matrix3::operator + (const Matrix3& rhs) const
{
    return Matrix3(
        m_m00 + rhs.m_m00,
        m_m01 + rhs.m_m01,
        m_m02 + rhs.m_m02,
        m_m10 + rhs.m_m10,
        m_m11 + rhs.m_m11,
        m_m12 + rhs.m_m12,
        m_m20 + rhs.m_m20,
        m_m21 + rhs.m_m21,
        m_m22 + rhs.m_m22
    );
}
Matrix3  Matrix3::Add(const Matrix3& rhs) const
{
	return Matrix3(
		m_m00 + rhs.m_m00,
		m_m01 + rhs.m_m01,
		m_m02 + rhs.m_m02,
		m_m10 + rhs.m_m10,
		m_m11 + rhs.m_m11,
		m_m12 + rhs.m_m12,
		m_m20 + rhs.m_m20,
		m_m21 + rhs.m_m21,
		m_m22 + rhs.m_m22
	);
}
/// Subtract a matrix.
Matrix3  Matrix3::operator - (const Matrix3& rhs) const
{
    return Matrix3(
        m_m00 - rhs.m_m00,
        m_m01 - rhs.m_m01,
        m_m02 - rhs.m_m02,
        m_m10 - rhs.m_m10,
        m_m11 - rhs.m_m11,
        m_m12 - rhs.m_m12,
        m_m20 - rhs.m_m20,
        m_m21 - rhs.m_m21,
        m_m22 - rhs.m_m22
    );
}
Matrix3  Matrix3::Sub (const Matrix3& rhs) const
{
	return Matrix3(
		m_m00 - rhs.m_m00,
		m_m01 - rhs.m_m01,
		m_m02 - rhs.m_m02,
		m_m10 - rhs.m_m10,
		m_m11 - rhs.m_m11,
		m_m12 - rhs.m_m12,
		m_m20 - rhs.m_m20,
		m_m21 - rhs.m_m21,
		m_m22 - rhs.m_m22
	);
}
/// Multiply with a scalar.
Matrix3  Matrix3::operator * (float rhs) const
{
    return Matrix3(
        m_m00 * rhs,
        m_m01 * rhs,
        m_m02 * rhs,
        m_m10 * rhs,
        m_m11 * rhs,
        m_m12 * rhs,
        m_m20 * rhs,
        m_m21 * rhs,
        m_m22 * rhs
    );
}
Matrix3  Matrix3::Multiply(float rhs) const
{
	return Matrix3(
		m_m00 * rhs,
		m_m01 * rhs,
		m_m02 * rhs,
		m_m10 * rhs,
		m_m11 * rhs,
		m_m12 * rhs,
		m_m20 * rhs,
		m_m21 * rhs,
		m_m22 * rhs
	);
}
/// Multiply a matrix.
Matrix3  Matrix3::operator * (const Matrix3& rhs) const
{
    return Matrix3(
        m_m00 * rhs.m_m00 + m_m01 * rhs.m_m10 + m_m02 * rhs.m_m20,
        m_m00 * rhs.m_m01 + m_m01 * rhs.m_m11 + m_m02 * rhs.m_m21,
        m_m00 * rhs.m_m02 + m_m01 * rhs.m_m12 + m_m02 * rhs.m_m22,
        m_m10 * rhs.m_m00 + m_m11 * rhs.m_m10 + m_m12 * rhs.m_m20,
        m_m10 * rhs.m_m01 + m_m11 * rhs.m_m11 + m_m12 * rhs.m_m21,
        m_m10 * rhs.m_m02 + m_m11 * rhs.m_m12 + m_m12 * rhs.m_m22,
        m_m20 * rhs.m_m00 + m_m21 * rhs.m_m10 + m_m22 * rhs.m_m20,
        m_m20 * rhs.m_m01 + m_m21 * rhs.m_m11 + m_m22 * rhs.m_m21,
        m_m20 * rhs.m_m02 + m_m21 * rhs.m_m12 + m_m22 * rhs.m_m22
    );
}
Matrix3  Matrix3::Multiply(const Matrix3& rhs) const
{
	return Matrix3(
		m_m00 * rhs.m_m00 + m_m01 * rhs.m_m10 + m_m02 * rhs.m_m20,
		m_m00 * rhs.m_m01 + m_m01 * rhs.m_m11 + m_m02 * rhs.m_m21,
		m_m00 * rhs.m_m02 + m_m01 * rhs.m_m12 + m_m02 * rhs.m_m22,
		m_m10 * rhs.m_m00 + m_m11 * rhs.m_m10 + m_m12 * rhs.m_m20,
		m_m10 * rhs.m_m01 + m_m11 * rhs.m_m11 + m_m12 * rhs.m_m21,
		m_m10 * rhs.m_m02 + m_m11 * rhs.m_m12 + m_m12 * rhs.m_m22,
		m_m20 * rhs.m_m00 + m_m21 * rhs.m_m10 + m_m22 * rhs.m_m20,
		m_m20 * rhs.m_m01 + m_m21 * rhs.m_m11 + m_m22 * rhs.m_m21,
		m_m20 * rhs.m_m02 + m_m21 * rhs.m_m12 + m_m22 * rhs.m_m22
	);
}
}
