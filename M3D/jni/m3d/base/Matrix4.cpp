#include "m3d/base/Matrix3x4.h"
#include "m3d/base/Matrix4.h"

#include <cstdio>

namespace M3D
{

const Matrix4 Matrix4::ZERO(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

const Matrix4 Matrix4::IDENTITY;

/// Construct an identity matrix.
Matrix4::Matrix4() :
		m_m00(1.0f), m_m01(0.0f), m_m02(0.0f), m_m03(0.0f), m_m10(0.0f), m_m11(1.0f), m_m12(
				0.0f), m_m13(0.0f), m_m20(0.0f), m_m21(0.0f), m_m22(1.0f), m_m23(
				0.0f), m_m30(0.0f), m_m31(0.0f), m_m32(0.0f), m_m33(1.0f)
{
}

/// Copy-construct from another matrix.
Matrix4::Matrix4(const Matrix4& matrix) :
		m_m00(matrix.m_m00), m_m01(matrix.m_m01), m_m02(matrix.m_m02), m_m03(
				matrix.m_m03), m_m10(matrix.m_m10), m_m11(matrix.m_m11), m_m12(
				matrix.m_m12), m_m13(matrix.m_m13), m_m20(matrix.m_m20), m_m21(
				matrix.m_m21), m_m22(matrix.m_m22), m_m23(matrix.m_m23), m_m30(
				matrix.m_m30), m_m31(matrix.m_m31), m_m32(matrix.m_m32), m_m33(
				matrix.m_m33)
{

}

/// Copy-cnstruct from a 3x3 matrix and set the extra elements to identity.
Matrix4::Matrix4(const Matrix3& matrix) :
		m_m00(matrix.m_m00), m_m01(matrix.m_m01), m_m02(matrix.m_m02), m_m03(0.0f), m_m10(
				matrix.m_m10), m_m11(matrix.m_m11), m_m12(matrix.m_m12), m_m13(0.0f), m_m20(
				matrix.m_m20), m_m21(matrix.m_m21), m_m22(matrix.m_m22), m_m23(0.0f), m_m30(
				0.0f), m_m31(0.0f), m_m32(0.0f), m_m33(1.0f)
{
}

// Construct from values.
Matrix4::Matrix4(float v00, float v01, float v02, float v03, float v10,
		float v11, float v12, float v13, float v20, float v21, float v22,
		float v23, float v30, float v31, float v32, float v33) :
		m_m00(v00), m_m01(v01), m_m02(v02), m_m03(v03), m_m10(v10), m_m11(v11), m_m12(
				v12), m_m13(v13), m_m20(v20), m_m21(v21), m_m22(v22), m_m23(v23), m_m30(
				v30), m_m31(v31), m_m32(v32), m_m33(v33)
{
}

/// Construct from a float array.
Matrix4::Matrix4(const float* data) :
		m_m00(data[0]), m_m01(data[1]), m_m02(data[2]), m_m03(data[3]), m_m10(
				data[4]), m_m11(data[5]), m_m12(data[6]), m_m13(data[7]), m_m20(
				data[8]), m_m21(data[9]), m_m22(data[10]), m_m23(data[11]), m_m30(
				data[12]), m_m31(data[13]), m_m32(data[14]), m_m33(data[15])
{
}

Matrix4::Matrix4(const Vector3& translation, const Quaternion& rotation,
		float scale)
{
	this->ToIdentity();
	SetRotation(rotation.RotationMatrix() * scale);
	SetTranslation(translation);
}

Matrix4::Matrix4(const Vector3& translation, const Quaternion& rotation,
		const Vector3& scale)
{
	this->ToIdentity();
	SetRotation(rotation.RotationMatrix().Scaled(scale));
	SetTranslation(translation);
}
/// Assign from another matrix.
Matrix4& Matrix4::operator =(const Matrix4& rhs)
{
	m_m00 = rhs.m_m00;
	m_m01 = rhs.m_m01;
	m_m02 = rhs.m_m02;
	m_m03 = rhs.m_m03;
	m_m10 = rhs.m_m10;
	m_m11 = rhs.m_m11;
	m_m12 = rhs.m_m12;
	m_m13 = rhs.m_m13;
	m_m20 = rhs.m_m20;
	m_m21 = rhs.m_m21;
	m_m22 = rhs.m_m22;
	m_m23 = rhs.m_m23;
	m_m30 = rhs.m_m30;
	m_m31 = rhs.m_m31;
	m_m32 = rhs.m_m32;
	m_m33 = rhs.m_m33;
	return *this;
}
/// Assign from another matrix.
void Matrix4::Set(const float* data)
{
	m_m00 = data[0];
	m_m01 = data[1];
	m_m02 = data[2];
	m_m03 = data[3];
	m_m10 = data[4];
	m_m11 = data[5];
	m_m12 = data[6];
	m_m13 = data[7];
	m_m20 = data[8];
	m_m21 = data[9];
	m_m22 = data[10];
	m_m23 = data[11];
	m_m30 = data[12];
	m_m31 = data[13];
	m_m32 = data[14];
	m_m33 = data[15];
}

/// Assign from a 3x3 matrix. Set the extra elements to identity.
Matrix4& Matrix4::operator =(const Matrix3& rhs)
{
	m_m00 = rhs.m_m00;
	m_m01 = rhs.m_m01;
	m_m02 = rhs.m_m02;
	m_m03 = 0.0f;
	m_m10 = rhs.m_m10;
	m_m11 = rhs.m_m11;
	m_m12 = rhs.m_m12;
	m_m13 = 0.0f;
	m_m20 = rhs.m_m20;
	m_m21 = rhs.m_m21;
	m_m22 = rhs.m_m22;
	m_m23 = 0.0f;
	m_m30 = 0.0f;
	m_m31 = 0.0f;
	m_m32 = 0.0f;
	m_m33 = 1.0f;
	return *this;
}

/// Test for equality with another matrix without epsilon.
bool Matrix4::operator ==(const Matrix4& rhs)const
{
	const float* leftData = Data();
	const float* rightData = rhs.Data();

	for (unsigned i = 0; i < 16; ++i)
	{
		if (leftData[i] != rightData[i])
			return false;
	}

	return true;
}

/// Test for inequality with another matrix without epsilon.
bool Matrix4::operator !=(const Matrix4& rhs) const
{
	return !(*this == rhs);
}

/// Multiply a Vector3 which is assumed to represent position.
Vector3 Matrix4::operator *(const Vector3& rhs)const
{
    float invW = 1.0f / (m_m30 * rhs.m_x + m_m31 * rhs.m_y + m_m32 * rhs.m_z + m_m33);

    return Vector3(
        (m_m00 * rhs.m_x + m_m01 * rhs.m_y + m_m02 * rhs.m_z + m_m03) * invW,
        (m_m10 * rhs.m_x + m_m11 * rhs.m_y + m_m12 * rhs.m_z + m_m13) * invW,
        (m_m20 * rhs.m_x + m_m21 * rhs.m_y + m_m22 * rhs.m_z + m_m23) * invW
    );
}
Vector3 Matrix4::Multiply(const Vector3& rhs)const
{
	float invW = 1.0f / (m_m30 * rhs.m_x + m_m31 * rhs.m_y + m_m32 * rhs.m_z + m_m33);

	return Vector3(
		(m_m00 * rhs.m_x + m_m01 * rhs.m_y + m_m02 * rhs.m_z + m_m03) * invW,
		(m_m10 * rhs.m_x + m_m11 * rhs.m_y + m_m12 * rhs.m_z + m_m13) * invW,
		(m_m20 * rhs.m_x + m_m21 * rhs.m_y + m_m22 * rhs.m_z + m_m23) * invW
	);
}
/// Multiply a Vector4.
Vector4 Matrix4::operator *(const Vector4& rhs)const
{
    return Vector4(
        m_m00 * rhs.m_x + m_m01 * rhs.m_y + m_m02 * rhs.m_z + m_m03 * rhs.m_w,
        m_m10 * rhs.m_x + m_m11 * rhs.m_y + m_m12 * rhs.m_z + m_m13 * rhs.m_w,
        m_m20 * rhs.m_x + m_m21 * rhs.m_y + m_m22 * rhs.m_z + m_m23 * rhs.m_w,
        m_m30 * rhs.m_x + m_m31 * rhs.m_y + m_m32 * rhs.m_z + m_m33 * rhs.m_w
    );
}
Vector4 Matrix4::Multiply(const Vector4& rhs)const
{
	return Vector4(
		m_m00 * rhs.m_x + m_m01 * rhs.m_y + m_m02 * rhs.m_z + m_m03 * rhs.m_w,
		m_m10 * rhs.m_x + m_m11 * rhs.m_y + m_m12 * rhs.m_z + m_m13 * rhs.m_w,
		m_m20 * rhs.m_x + m_m21 * rhs.m_y + m_m22 * rhs.m_z + m_m23 * rhs.m_w,
		m_m30 * rhs.m_x + m_m31 * rhs.m_y + m_m32 * rhs.m_z + m_m33 * rhs.m_w
	);
}
/// Add a matrix.
Matrix4 Matrix4::operator +(const Matrix4& rhs) const
{
    return Matrix4(
        m_m00 + rhs.m_m00,
        m_m01 + rhs.m_m01,
        m_m02 + rhs.m_m02,
        m_m03 + rhs.m_m03,
        m_m10 + rhs.m_m10,
        m_m11 + rhs.m_m11,
        m_m12 + rhs.m_m12,
        m_m13 + rhs.m_m13,
        m_m20 + rhs.m_m20,
        m_m21 + rhs.m_m21,
        m_m22 + rhs.m_m22,
        m_m23 + rhs.m_m23,
        m_m30 + rhs.m_m30,
        m_m31 + rhs.m_m31,
        m_m32 + rhs.m_m32,
        m_m33 + rhs.m_m33
    );
}
Matrix4 Matrix4::Add(const Matrix4& rhs) const
{
	return Matrix4(
		m_m00 + rhs.m_m00,
		m_m01 + rhs.m_m01,
		m_m02 + rhs.m_m02,
		m_m03 + rhs.m_m03,
		m_m10 + rhs.m_m10,
		m_m11 + rhs.m_m11,
		m_m12 + rhs.m_m12,
		m_m13 + rhs.m_m13,
		m_m20 + rhs.m_m20,
		m_m21 + rhs.m_m21,
		m_m22 + rhs.m_m22,
		m_m23 + rhs.m_m23,
		m_m30 + rhs.m_m30,
		m_m31 + rhs.m_m31,
		m_m32 + rhs.m_m32,
		m_m33 + rhs.m_m33
	);
}
/// Subtract a matrix.
Matrix4 Matrix4::operator -(const Matrix4& rhs) const
{
    return Matrix4(
        m_m00 - rhs.m_m00,
        m_m01 - rhs.m_m01,
        m_m02 - rhs.m_m02,
        m_m03 - rhs.m_m03,
        m_m10 - rhs.m_m10,
        m_m11 - rhs.m_m11,
        m_m12 - rhs.m_m12,
        m_m13 - rhs.m_m13,
        m_m20 - rhs.m_m20,
        m_m21 - rhs.m_m21,
        m_m22 - rhs.m_m22,
        m_m23 - rhs.m_m23,
        m_m30 - rhs.m_m30,
        m_m31 - rhs.m_m31,
        m_m32 - rhs.m_m32,
        m_m33 - rhs.m_m33
    );
}
Matrix4 Matrix4::Sub(const Matrix4& rhs) const
{
	return Matrix4(
		m_m00 - rhs.m_m00,
		m_m01 - rhs.m_m01,
		m_m02 - rhs.m_m02,
		m_m03 - rhs.m_m03,
		m_m10 - rhs.m_m10,
		m_m11 - rhs.m_m11,
		m_m12 - rhs.m_m12,
		m_m13 - rhs.m_m13,
		m_m20 - rhs.m_m20,
		m_m21 - rhs.m_m21,
		m_m22 - rhs.m_m22,
		m_m23 - rhs.m_m23,
		m_m30 - rhs.m_m30,
		m_m31 - rhs.m_m31,
		m_m32 - rhs.m_m32,
		m_m33 - rhs.m_m33
	);
}
/// Multiply with a scalar.
Matrix4 Matrix4::operator *(float rhs) const
{
    return Matrix4(
        m_m00 * rhs,
        m_m01 * rhs,
        m_m02 * rhs,
        m_m03 * rhs,
        m_m10 * rhs,
        m_m11 * rhs,
        m_m12 * rhs,
        m_m13 * rhs,
        m_m20 * rhs,
        m_m21 * rhs,
        m_m22 * rhs,
        m_m23 * rhs,
        m_m30 * rhs,
        m_m31 * rhs,
        m_m32 * rhs,
        m_m33 * rhs
    );
}
Matrix4 Matrix4::Multiply(float rhs) const
{
	return Matrix4(
		m_m00 * rhs,
		m_m01 * rhs,
		m_m02 * rhs,
		m_m03 * rhs,
		m_m10 * rhs,
		m_m11 * rhs,
		m_m12 * rhs,
		m_m13 * rhs,
		m_m20 * rhs,
		m_m21 * rhs,
		m_m22 * rhs,
		m_m23 * rhs,
		m_m30 * rhs,
		m_m31 * rhs,
		m_m32 * rhs,
		m_m33 * rhs
	);
}
//按列存储的矩阵，乘法的计算为列乘行,和按行存储的矩阵相反 very important by xuli
/// Multiply a matrix.
Matrix4 Matrix4::operator *(const Matrix4& rhs) const
{
    return Matrix4(
        m_m00 * rhs.m_m00 + m_m01 * rhs.m_m10 + m_m02 * rhs.m_m20 + m_m03 * rhs.m_m30,
        m_m00 * rhs.m_m01 + m_m01 * rhs.m_m11 + m_m02 * rhs.m_m21 + m_m03 * rhs.m_m31,
        m_m00 * rhs.m_m02 + m_m01 * rhs.m_m12 + m_m02 * rhs.m_m22 + m_m03 * rhs.m_m32,
        m_m00 * rhs.m_m03 + m_m01 * rhs.m_m13 + m_m02 * rhs.m_m23 + m_m03 * rhs.m_m33,
        m_m10 * rhs.m_m00 + m_m11 * rhs.m_m10 + m_m12 * rhs.m_m20 + m_m13 * rhs.m_m30,
        m_m10 * rhs.m_m01 + m_m11 * rhs.m_m11 + m_m12 * rhs.m_m21 + m_m13 * rhs.m_m31,
        m_m10 * rhs.m_m02 + m_m11 * rhs.m_m12 + m_m12 * rhs.m_m22 + m_m13 * rhs.m_m32,
        m_m10 * rhs.m_m03 + m_m11 * rhs.m_m13 + m_m12 * rhs.m_m23 + m_m13 * rhs.m_m33,
        m_m20 * rhs.m_m00 + m_m21 * rhs.m_m10 + m_m22 * rhs.m_m20 + m_m23 * rhs.m_m30,
        m_m20 * rhs.m_m01 + m_m21 * rhs.m_m11 + m_m22 * rhs.m_m21 + m_m23 * rhs.m_m31,
        m_m20 * rhs.m_m02 + m_m21 * rhs.m_m12 + m_m22 * rhs.m_m22 + m_m23 * rhs.m_m32,
        m_m20 * rhs.m_m03 + m_m21 * rhs.m_m13 + m_m22 * rhs.m_m23 + m_m23 * rhs.m_m33,
        m_m30 * rhs.m_m00 + m_m31 * rhs.m_m10 + m_m32 * rhs.m_m20 + m_m33 * rhs.m_m30,
        m_m30 * rhs.m_m01 + m_m31 * rhs.m_m11 + m_m32 * rhs.m_m21 + m_m33 * rhs.m_m31,
        m_m30 * rhs.m_m02 + m_m31 * rhs.m_m12 + m_m32 * rhs.m_m22 + m_m33 * rhs.m_m32,
        m_m30 * rhs.m_m03 + m_m31 * rhs.m_m13 + m_m32 * rhs.m_m23 + m_m33 * rhs.m_m33
    );
}
Matrix4 Matrix4::Multiply(const Matrix4& rhs) const
{
	return Matrix4(
		m_m00 * rhs.m_m00 + m_m01 * rhs.m_m10 + m_m02 * rhs.m_m20 + m_m03 * rhs.m_m30,
		m_m00 * rhs.m_m01 + m_m01 * rhs.m_m11 + m_m02 * rhs.m_m21 + m_m03 * rhs.m_m31,
		m_m00 * rhs.m_m02 + m_m01 * rhs.m_m12 + m_m02 * rhs.m_m22 + m_m03 * rhs.m_m32,
		m_m00 * rhs.m_m03 + m_m01 * rhs.m_m13 + m_m02 * rhs.m_m23 + m_m03 * rhs.m_m33,
		m_m10 * rhs.m_m00 + m_m11 * rhs.m_m10 + m_m12 * rhs.m_m20 + m_m13 * rhs.m_m30,
		m_m10 * rhs.m_m01 + m_m11 * rhs.m_m11 + m_m12 * rhs.m_m21 + m_m13 * rhs.m_m31,
		m_m10 * rhs.m_m02 + m_m11 * rhs.m_m12 + m_m12 * rhs.m_m22 + m_m13 * rhs.m_m32,
		m_m10 * rhs.m_m03 + m_m11 * rhs.m_m13 + m_m12 * rhs.m_m23 + m_m13 * rhs.m_m33,
		m_m20 * rhs.m_m00 + m_m21 * rhs.m_m10 + m_m22 * rhs.m_m20 + m_m23 * rhs.m_m30,
		m_m20 * rhs.m_m01 + m_m21 * rhs.m_m11 + m_m22 * rhs.m_m21 + m_m23 * rhs.m_m31,
		m_m20 * rhs.m_m02 + m_m21 * rhs.m_m12 + m_m22 * rhs.m_m22 + m_m23 * rhs.m_m32,
		m_m20 * rhs.m_m03 + m_m21 * rhs.m_m13 + m_m22 * rhs.m_m23 + m_m23 * rhs.m_m33,
		m_m30 * rhs.m_m00 + m_m31 * rhs.m_m10 + m_m32 * rhs.m_m20 + m_m33 * rhs.m_m30,
		m_m30 * rhs.m_m01 + m_m31 * rhs.m_m11 + m_m32 * rhs.m_m21 + m_m33 * rhs.m_m31,
		m_m30 * rhs.m_m02 + m_m31 * rhs.m_m12 + m_m32 * rhs.m_m22 + m_m33 * rhs.m_m32,
		m_m30 * rhs.m_m03 + m_m31 * rhs.m_m13 + m_m32 * rhs.m_m23 + m_m33 * rhs.m_m33
	);
}
Matrix4 Matrix4::operator * (const Matrix3x4& rhs) const
{
    return Matrix4(
        m_m00 * rhs.m_m00 + m_m01 * rhs.m_m10 + m_m02 * rhs.m_m20,
        m_m00 * rhs.m_m01 + m_m01 * rhs.m_m11 + m_m02 * rhs.m_m21,
        m_m00 * rhs.m_m02 + m_m01 * rhs.m_m12 + m_m02 * rhs.m_m22,
        m_m00 * rhs.m_m03 + m_m01 * rhs.m_m13 + m_m02 * rhs.m_m23 + m_m03,
        m_m10 * rhs.m_m00 + m_m11 * rhs.m_m10 + m_m12 * rhs.m_m20,
        m_m10 * rhs.m_m01 + m_m11 * rhs.m_m11 + m_m12 * rhs.m_m21,
        m_m10 * rhs.m_m02 + m_m11 * rhs.m_m12 + m_m12 * rhs.m_m22,
        m_m10 * rhs.m_m03 + m_m11 * rhs.m_m13 + m_m12 * rhs.m_m23 + m_m13,
        m_m20 * rhs.m_m00 + m_m21 * rhs.m_m10 + m_m22 * rhs.m_m20,
        m_m20 * rhs.m_m01 + m_m21 * rhs.m_m11 + m_m22 * rhs.m_m21,
        m_m20 * rhs.m_m02 + m_m21 * rhs.m_m12 + m_m22 * rhs.m_m22,
        m_m20 * rhs.m_m03 + m_m21 * rhs.m_m13 + m_m22 * rhs.m_m23 + m_m23,
        m_m30 * rhs.m_m00 + m_m31 * rhs.m_m10 + m_m32 * rhs.m_m20,
        m_m30 * rhs.m_m01 + m_m31 * rhs.m_m11 + m_m32 * rhs.m_m21,
        m_m30 * rhs.m_m02 + m_m31 * rhs.m_m12 + m_m32 * rhs.m_m22,
        m_m30 * rhs.m_m03 + m_m31 * rhs.m_m13 + m_m32 * rhs.m_m23 + m_m33
    );
}
Matrix4 Matrix4::Multiply(const Matrix3x4& rhs) const
{
	return Matrix4(
		m_m00 * rhs.m_m00 + m_m01 * rhs.m_m10 + m_m02 * rhs.m_m20,
		m_m00 * rhs.m_m01 + m_m01 * rhs.m_m11 + m_m02 * rhs.m_m21,
		m_m00 * rhs.m_m02 + m_m01 * rhs.m_m12 + m_m02 * rhs.m_m22,
		m_m00 * rhs.m_m03 + m_m01 * rhs.m_m13 + m_m02 * rhs.m_m23 + m_m03,
		m_m10 * rhs.m_m00 + m_m11 * rhs.m_m10 + m_m12 * rhs.m_m20,
		m_m10 * rhs.m_m01 + m_m11 * rhs.m_m11 + m_m12 * rhs.m_m21,
		m_m10 * rhs.m_m02 + m_m11 * rhs.m_m12 + m_m12 * rhs.m_m22,
		m_m10 * rhs.m_m03 + m_m11 * rhs.m_m13 + m_m12 * rhs.m_m23 + m_m13,
		m_m20 * rhs.m_m00 + m_m21 * rhs.m_m10 + m_m22 * rhs.m_m20,
		m_m20 * rhs.m_m01 + m_m21 * rhs.m_m11 + m_m22 * rhs.m_m21,
		m_m20 * rhs.m_m02 + m_m21 * rhs.m_m12 + m_m22 * rhs.m_m22,
		m_m20 * rhs.m_m03 + m_m21 * rhs.m_m13 + m_m22 * rhs.m_m23 + m_m23,
		m_m30 * rhs.m_m00 + m_m31 * rhs.m_m10 + m_m32 * rhs.m_m20,
		m_m30 * rhs.m_m01 + m_m31 * rhs.m_m11 + m_m32 * rhs.m_m21,
		m_m30 * rhs.m_m02 + m_m31 * rhs.m_m12 + m_m32 * rhs.m_m22,
		m_m30 * rhs.m_m03 + m_m31 * rhs.m_m13 + m_m32 * rhs.m_m23 + m_m33
	);
}

Matrix4 Matrix4::operator*(const Quaternion& rhs) const
{
	Matrix3 rotationMatrix = rhs.RotationMatrix();
	Matrix4 tempMatrix = Matrix4(rotationMatrix);
	return (*this) * tempMatrix;
}
Matrix4 Matrix4::Multiply(const Quaternion& rhs) const
{
	Matrix3 rotationMatrix = rhs.RotationMatrix();
	Matrix4 tempMatrix = Matrix4(rotationMatrix);
	return (*this) * tempMatrix;
}
/// Multiply with a 3x4 matrix.
// Matrix4 operator * (const Matrix3x4& rhs) const;

/// Set translation elements.
void Matrix4::SetTranslation(const Vector3& translation)
{
    m_m03 = translation.m_x;
    m_m13 = translation.m_y;
    m_m23 = translation.m_z;
}

/// Set rotation elements from a 3x3 matrix.
void Matrix4::SetRotation(const Matrix3& rotation)
{
    m_m00 = rotation.m_m00;
    m_m01 = rotation.m_m01;
    m_m02 = rotation.m_m02;
    m_m10 = rotation.m_m10;
    m_m11 = rotation.m_m11;
    m_m12 = rotation.m_m12;
    m_m20 = rotation.m_m20;
    m_m21 = rotation.m_m21;
    m_m22 = rotation.m_m22;
}

// Set scaling elements.
void Matrix4::SetScale(const Vector3& scale)
{
    m_m00 = scale.m_x;
    m_m11 = scale.m_y;
    m_m22 = scale.m_z;
}

// Set uniform scaling elements.
void Matrix4::SetScale(float scale)
{
    m_m00 = scale;
    m_m11 = scale;
    m_m22 = scale;
}

/// Return the combined rotation and scaling matrix.
Matrix3 Matrix4::ToMatrix3() const
{
    return Matrix3(
        m_m00,
        m_m01,
        m_m02,
        m_m10,
        m_m11,
        m_m12,
        m_m20,
        m_m21,
        m_m22
    );
}

/// Return the rotation matrix with scaling removed.
Matrix3 Matrix4::RotationMatrix() const
{
    Vector3 invScale(
        1.0f / sqrtf(m_m00 * m_m00 + m_m10 * m_m10 + m_m20 * m_m20),
        1.0f / sqrtf(m_m01 * m_m01 + m_m11 * m_m11 + m_m21 * m_m21),
        1.0f / sqrtf(m_m02 * m_m02 + m_m12 * m_m12 + m_m22 * m_m22)
    );

    return ToMatrix3().Scaled(invScale);
}

/// Return the translation part.
Vector3 Matrix4::Translation() const
{
    return Vector3(
        m_m03,
        m_m13,
        m_m23
    );
}

void Matrix4::MultiTranslate(const Vector3& mov)
{
	Matrix4 temp;
	temp.SetTranslation(mov);
	*this = (*this)*temp ;
}

void Matrix4::MultiRotatiton(const Quaternion& rotation)
{
	Matrix4 temp;
	temp.SetRotation(rotation.RotationMatrix());
	*this = temp*(*this) ;
}

void Matrix4::MultiScale(float scale)
{
	Matrix4 temp;
	temp.SetScale(scale);
	*this = temp*(*this) ;
}

void Matrix4::ToIdentity()
{
	*this = Matrix4::IDENTITY;
}

/// Return the rotation part.
Quaternion Matrix4::Rotation() const
{
	return Quaternion(RotationMatrix());
}

/// Return the scaling part
Vector3 Matrix4::Scale() const
{
    return Vector3(
        sqrtf(m_m00 * m_m00 + m_m10 * m_m10 + m_m20 * m_m20),
        sqrtf(m_m01 * m_m01 + m_m11 * m_m11 + m_m21 * m_m21),
        sqrtf(m_m02 * m_m02 + m_m12 * m_m12 + m_m22 * m_m22)
    );
}

/// Return transpose
Matrix4 Matrix4::Transpose() const
{
    return Matrix4(
        m_m00,
        m_m10,
        m_m20,
        m_m30,
        m_m01,
        m_m11,
        m_m21,
        m_m31,
        m_m02,
        m_m12,
        m_m22,
        m_m32,
        m_m03,
        m_m13,
        m_m23,
        m_m33
    );
}

void Matrix4::Transposed()
{
	*this = Matrix4(
		m_m00,
		m_m10,
		m_m20,
		m_m30,
		m_m01,
		m_m11,
		m_m21,
		m_m31,
		m_m02,
		m_m12,
		m_m22,
		m_m32,
		m_m03,
		m_m13,
		m_m23,
		m_m33
	);
}

void Matrix4::Ortho(float left, float right, float bottom, float top,
		float nearPlane, float farPlane)
{
	// Bail out if the projection volume is zero-sized.
	if (left == right || bottom == top || nearPlane == farPlane)
		return;

	// Construct the projection.
	float width = right - left;
	float invheight = top - bottom;
	float clip = farPlane - nearPlane;
	Matrix4 ort; // The "1" says to not load the identity.
	ort.m_m00 = 2.0f / width;
	ort.m_m10 = 0.0f;
	ort.m_m20 = 0.0f;
	ort.m_m30 = -(left + right) / width;
	ort.m_m01 = 0.0f;
	ort.m_m11 = 2.0f / invheight;
	ort.m_m21 = 0.0f;
	ort.m_m31 = -(top + bottom) / invheight;
	ort.m_m02 = 0.0f;
	ort.m_m12 = 0.0f;
	ort.m_m22 = -2.0f / clip;
	ort.m_m32 = -(nearPlane + farPlane) / clip;
	ort.m_m03 = 0.0f;
	ort.m_m13 = 0.0f;
	ort.m_m23 = 0.0f;
	ort.m_m33 = 1.0f;
	// Apply the projection.
	*this = *this * ort;
}

void Matrix4::LookAt(float eyeX, float eyeY, float eyeZ, float centerX,
		float centerY, float centerZ, float upX, float upY, float upZ)
{
	Vector3 forward;
	Vector3 side;
	Vector3 up;

	//float forward[3], side[3], up[3];
	//GLfloat m[4][4];
	Matrix4 m;

	forward = Vector3(centerX, centerY, centerZ) - Vector3(eyeX, eyeY, eyeZ);
	up = Vector3(upX, upY, upZ);

	forward.Normalize();

	side = forward.CrossProduct(up);
	side.Normalize();

	/* Recompute up as: up = side x forward */
	up = side.CrossProduct(forward);
	up.Normalize();

	m.m_m00 = side.m_x;
	m.m_m10 = side.m_y;
	m.m_m20 = side.m_z;

	m.m_m01 = up.m_x;
	m.m_m11 = up.m_y;
	m.m_m21 = up.m_z;

	m.m_m02 = -forward.m_x;
	m.m_m12 = -forward.m_y;
	m.m_m22 = -forward.m_z;

	//m.m03_ = -side.DotProduct(Vector3(eyeX,eyeY,eyeZ));
	//m.m13_ = -up.DotProduct(Vector3(eyeX,eyeY,eyeZ));
	//m.m23_ = -forward.DotProduct(Vector3(eyeX,eyeY,eyeZ));

	m.m_m03 =  eyeX;
	m.m_m13 =  eyeY;
	m.m_m23 =  eyeZ;

	*this =  m; //右乘
//	this->(Vector3(eyeX, eyeY, eyeZ));
}

/// Test for equality with another matrix with epsilon.
bool Matrix4::Equals(const Matrix4& rhs) const
{
	const float* leftData = Data();
	const float* rightData = rhs.Data();

	for (unsigned i = 0; i < 16; ++i)
	{
		if (!M3D::Equals(leftData[i], rightData[i]))
			return false;
	}

	return true;
}

/// Return float data
const float* Matrix4::Data() const
{
	return &m_m00;
}

/// Bulk transpose matrices.
void Matrix4::BulkTranspose(float* dest, const float* src,
		unsigned count)
{
    for (unsigned i = 0; i < count; ++i)
    {
        dest[0] = src[0];
        dest[1] = src[4];
        dest[2] = src[8];
        dest[3] = src[12];
        dest[4] = src[1];
        dest[5] = src[5];
        dest[6] = src[9];
        dest[7] = src[13];
        dest[8] = src[2];
        dest[9] = src[6];
        dest[10] = src[10];
        dest[11] = src[14];
        dest[12] = src[3];
        dest[13] = src[7];
        dest[14] = src[11];
        dest[15] = src[15];

        dest += 16;
        src += 16;
    }
}

void Matrix4::Decompose(Vector3& translation, Quaternion& rotation,
		Vector3& scale) const
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

Matrix4 Matrix4::Inverse() const
{
    float v0 = m_m20 * m_m31 - m_m21 * m_m30;
    float v1 = m_m20 * m_m32 - m_m22 * m_m30;
    float v2 = m_m20 * m_m33 - m_m23 * m_m30;
    float v3 = m_m21 * m_m32 - m_m22 * m_m31;
    float v4 = m_m21 * m_m33 - m_m23 * m_m31;
    float v5 = m_m22 * m_m33 - m_m23 * m_m32;

    float i00 = (v5 * m_m11 - v4 * m_m12 + v3 * m_m13);
    float i10 = -(v5 * m_m10 - v2 * m_m12 + v1 * m_m13);
    float i20 = (v4 * m_m10 - v2 * m_m11 + v0 * m_m13);
    float i30 = -(v3 * m_m10 - v1 * m_m11 + v0 * m_m12);

    float invDet = 1.0f / (i00 * m_m00 + i10 * m_m01 + i20 * m_m02 + i30 * m_m03);

    i00 *= invDet;
    i10 *= invDet;
    i20 *= invDet;
    i30 *= invDet;

    float i01 = -(v5 * m_m01 - v4 * m_m02 + v3 * m_m03) * invDet;
    float i11 = (v5 * m_m00 - v2 * m_m02 + v1 * m_m03) * invDet;
    float i21 = -(v4 * m_m00 - v2 * m_m01 + v0 * m_m03) * invDet;
    float i31 = (v3 * m_m00 - v1 * m_m01 + v0 * m_m02) * invDet;

    v0 = m_m10 * m_m31 - m_m11 * m_m30;
    v1 = m_m10 * m_m32 - m_m12 * m_m30;
    v2 = m_m10 * m_m33 - m_m13 * m_m30;
    v3 = m_m11 * m_m32 - m_m12 * m_m31;
    v4 = m_m11 * m_m33 - m_m13 * m_m31;
    v5 = m_m12 * m_m33 - m_m13 * m_m32;

    float i02 = (v5 * m_m01 - v4 * m_m02 + v3 * m_m03) * invDet;
    float i12 = -(v5 * m_m00 - v2 * m_m02 + v1 * m_m03) * invDet;
    float i22 = (v4 * m_m00 - v2 * m_m01 + v0 * m_m03) * invDet;
    float i32 = -(v3 * m_m00 - v1 * m_m01 + v0 * m_m02) * invDet;

    v0 = m_m21 * m_m10 - m_m20 * m_m11;
    v1 = m_m22 * m_m10 - m_m20 * m_m12;
    v2 = m_m23 * m_m10 - m_m20 * m_m13;
    v3 = m_m22 * m_m11 - m_m21 * m_m12;
    v4 = m_m23 * m_m11 - m_m21 * m_m13;
    v5 = m_m23 * m_m12 - m_m22 * m_m13;

    float i03 = -(v5 * m_m01 - v4 * m_m02 + v3 * m_m03) * invDet;
    float i13 = (v5 * m_m00 - v2 * m_m02 + v1 * m_m03) * invDet;
    float i23 = -(v4 * m_m00 - v2 * m_m01 + v0 * m_m03) * invDet;
    float i33 = (v3 * m_m00 - v1 * m_m01 + v0 * m_m02) * invDet;

    return Matrix4(
        i00, i01, i02, i03,
        i10, i11, i12, i13,
        i20, i21, i22, i23,
        i30, i31, i32, i33);
}

string Matrix4::Tostring() const
{
    char tempBuffer[MATRIX_CONVERSION_BUFFER_LENGTH];
    sprintf(tempBuffer, "%g %g %g %g %g %g %g %g %g %g %g %g %g %g %g %g", m_m00, m_m01, m_m02, m_m03, m_m10, m_m11, m_m12, m_m13, m_m20,
        m_m21, m_m22, m_m23, m_m30, m_m31, m_m32, m_m33);
    return string(tempBuffer);
}

}
