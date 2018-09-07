/**
 *@file
 *@brief
 *@author	XL
 *@date		2015-2-26
 *@version	1.0
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 */
#ifndef M3D_MATRIX4_H_
#define M3D_MATRIX4_H_

#include "m3d/base/Quaternion.h"
#include "m3d/base/Vector4.h"

namespace M3D
{
class Matrix3x4;
/// 4x4 matrix for arbitrary linear transforms including projection.
class M3D_API Matrix4
{
public:
	/// Construct an identity matrix.
	Matrix4();

	/** 
	 * @brief  Copy-construct from another matrix.
	 */
	Matrix4(const Matrix4& matrix);

	/** 
	 * @brief  Copy-cnstruct from a 3x3 matrix and set the extra elements to identity.
	 */
	Matrix4(const Matrix3& matrix);

	/** 
	 * @brief  Construct from translation, rotation and uniform scale.
	 */
	Matrix4(const Vector3& translation, const Quaternion& rotation,
			float scale);
	/** 
	 * @brief  Construct from translation, rotation and nonuniform scale.
	 */
	Matrix4(const Vector3& translation, const Quaternion& rotation,
			const Vector3& scale);
	// Construct from values.
	Matrix4(float v00, float v01, float v02, float v03, float v10, float v11,
			float v12, float v13, float v20, float v21, float v22, float v23,
			float v30, float v31, float v32, float v33);

	/** 
	 * @brief  Construct from a float array.
	 */
	Matrix4(const float* data);

	/** 
	 * @brief  Assign from another matrix.
	 */
	Matrix4& operator =(const Matrix4& rhs);
	/** 
	 * @brief  Assign from another matrix.
	 */
	void Set(const float* data);

	/** 
	 * @brief  Assign from a 3x3 matrix. Set the extra elements to identity.
	 */
	Matrix4& operator =(const Matrix3& rhs);

	/** 
	 * @brief  Test for equality with another matrix without epsilon.
	 */
	bool operator ==(const Matrix4& rhs) const;

	/** 
	 * @brief  Test for inequality with another matrix without epsilon.
	 */
	bool operator !=(const Matrix4& rhs) const;

	/** 
	 * @brief  Multiply a Vector3 which is assumed to represent position.
	 */
	Vector3 operator *(const Vector3& rhs) const;
	Vector3 Multiply(const Vector3& rhs) const;
	/** 
	 * @brief  Multiply a Vector4.
	 */
	Vector4 operator *(const Vector4& rhs) const;
	Vector4 Multiply(const Vector4& rhs) const;
	/** 
	 * @brief  Add a matrix.
	 */
	Matrix4 operator +(const Matrix4& rhs) const;
	Matrix4 Add(const Matrix4& rhs) const;
	/** 
	 * @brief  Subtract a matrix.
	 */
	Matrix4 operator -(const Matrix4& rhs) const;
	Matrix4 Sub(const Matrix4& rhs) const;

	/** 
	 * @brief  Multiply with a scalar.
	 */
	Matrix4 operator *(float rhs) const;
	Matrix4 Multiply(float rhs) const;

	//按列存储的矩阵，乘法的计算为列乘行,和按行存储的矩阵相反 very important by xuli
	/** 
	 * @brief  Multiply a matrix.
	 */
	Matrix4 operator *(const Matrix4& rhs) const;
	Matrix4 Multiply(const Matrix4& rhs) const;

	/**
	 * @brief  Multiply with a 3x4 matrix.
	 */
	Matrix4 operator *(const Matrix3x4& rhs) const;
	Matrix4 Multiply(const Matrix3x4& rhs) const;

	Matrix4 operator*(const Quaternion& rhs) const;
	Matrix4 Multiply(const Quaternion& rhs) const;

	/** 
	 * @brief  Multiply with a 3x4 matrix.
	 */
	// Matrix4 operator * (const Matrix3x4& rhs) const;
	/** 
	 * @brief  Set translation elements.
	 */
	void SetTranslation(const Vector3& translation);

	/** 
	 * @brief  Set rotation elements from a 3x3 matrix.
	 */
	void SetRotation(const Matrix3& rotation);

	/** 
	 * @brief  Set scaling elements.
	 */
	void SetScale(const Vector3& scale);

	/** 
	 * @brief  Set uniform scaling elements.
	 */
	void SetScale(float scale);

	/** 
	 * @brief  Return the combined rotation and scaling matrix.
	 */
	Matrix3 ToMatrix3() const;

	/** 
	 * @brief  Return the rotation matrix with scaling removed.
	 */
	Matrix3 RotationMatrix() const;

	/** 
	 * @brief  Return the translation part.
	 */
	Vector3 Translation() const;
	/**
	 * @brief
	 */
	void MultiTranslate(const Vector3& mov);
	/**
	 * @brief
	 */
	void MultiRotatiton(const Quaternion& rotation);
	/**
	 * @brief
	 */
	void MultiScale(float scale);
	/**
	 * @brief
	 */
	void ToIdentity();

	/** 
	 * @brief  Return the rotation part.
	 */
	Quaternion Rotation() const;

	/** 
	 * @brief  Return the scaling part
	 */
	Vector3 Scale() const;

	/** 
	 * @brief  Return transpose
	 */
	Matrix4 Transpose() const;
	
	void Transposed();

	void Ortho(float left, float right, float bottom, float top,
			float nearPlane, float farPlane);
	void LookAt(float eyeX, float eyeY, float eyeZ, float centerX,
			float centerY, float centerZ, float upX, float upY, float upZ);

	/** 
	 * @brief  Test for equality with another matrix with epsilon.
	 */
	bool Equals(const Matrix4& rhs) const;

	/** 
	 * @brief  Return decomposition to translation, rotation and scale.
	 */
	void Decompose(Vector3& translation, Quaternion& rotation,
			Vector3& scale) const;
	/** 
	 * @brief  Return inverse.
	 */
	Matrix4 Inverse() const;

	/** 
	 * @brief  Return float data
	 */
	const float* Data() const;
	/** 
	 * @brief  Return as string.
	 */
	string Tostring() const;
	M3D_ADDRESSTYPE Address()
	{
		//获取地址的真实方式
		return TAddress(*this);
	}	
	
	void FillByAddress(M3D_ADDRESSTYPE memoryAddress)
	{
		*this = *TFromAddress<Matrix4>(memoryAddress);
	}

	float m_m00; //!< 数据
	float m_m01;
	float m_m02;
	float m_m03;
	float m_m10;
	float m_m11;
	float m_m12;
	float m_m13;
	float m_m20;
	float m_m21;
	float m_m22;
	float m_m23;
	float m_m30;
	float m_m31;
	float m_m32;
	float m_m33;

	/** 
	 * @brief  Bulk transpose matrices.
	 */
	static void BulkTranspose(float* dest, const float* src, unsigned count);

	/** 
	 * @brief  Zero matrix.
	 */
	static const Matrix4 ZERO;
	/** 
	 * @brief  Identity matrix.
	 */
	static const Matrix4 IDENTITY;
};

/** 
 * @brief  Multiply a 4x4 matrix with a scalar 
 */
inline Matrix4 operator *(float lhs, const Matrix4& rhs)
{
	return rhs * lhs;
}

}
#endif //MATRIX4_H
