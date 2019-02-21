#pragma once
#include "MathBase.h"
#include "Quaternion.h"
#include "Vector4.h"

namespace Auto3D {

class Matrix3x4;

/// 4x4 matrix for arbitrary linear transforms including projection.
class Matrix4x4
{
public:
	/// Construct an identity matrix.
	Matrix4x4() noexcept
		:_m00(1.0f),
		_m01(0.0f),
		_m02(0.0f),
		_m03(0.0f),
		_m10(0.0f),
		_m11(1.0f),
		_m12(0.0f),
		_m13(0.0f),
		_m20(0.0f),
		_m21(0.0f),
		_m22(1.0f),
		_m23(0.0f),
		_m30(0.0f),
		_m31(0.0f),
		_m32(0.0f),
		_m33(1.0f)
	{
	}

	/// Copy-construct from another matrix.
	Matrix4x4(const Matrix4x4& matrix) noexcept
		:_m00(matrix._m00),
		_m01(matrix._m01),
		_m02(matrix._m02),
		_m03(matrix._m03),
		_m10(matrix._m10),
		_m11(matrix._m11),
		_m12(matrix._m12),
		_m13(matrix._m13),
		_m20(matrix._m20),
		_m21(matrix._m21),
		_m22(matrix._m22),
		_m23(matrix._m23),
		_m30(matrix._m30),
		_m31(matrix._m31),
		_m32(matrix._m32),
		_m33(matrix._m33)
	{
	}

	/// Copy-construct from a 3x3 matrix and set the extra elements to identity.
	explicit Matrix4x4(const Matrix3x3& matrix) noexcept :
		_m00(matrix._m00),
		_m01(matrix._m01),
		_m02(matrix._m02),
		_m03(0.0f),
		_m10(matrix._m10),
		_m11(matrix._m11),
		_m12(matrix._m12),
		_m13(0.0f),
		_m20(matrix._m20),
		_m21(matrix._m21),
		_m22(matrix._m22),
		_m23(0.0f),
		_m30(0.0f),
		_m31(0.0f),
		_m32(0.0f),
		_m33(1.0f)
	{
	}

	/// Construct from values.
	Matrix4x4(float v00, float v01, float v02, float v03,
		float v10, float v11, float v12, float v13,
		float v20, float v21, float v22, float v23,
		float v30, float v31, float v32, float v33) noexcept :
		_m00(v00),
		_m01(v01),
		_m02(v02),
		_m03(v03),
		_m10(v10),
		_m11(v11),
		_m12(v12),
		_m13(v13),
		_m20(v20),
		_m21(v21),
		_m22(v22),
		_m23(v23),
		_m30(v30),
		_m31(v31),
		_m32(v32),
		_m33(v33)
	{
	}
	/// Construct from a float array.
	explicit Matrix4x4(const float* data) noexcept
		:_m00(data[0]),
		_m01(data[1]),
		_m02(data[2]),
		_m03(data[3]),
		_m10(data[4]),
		_m11(data[5]),
		_m12(data[6]),
		_m13(data[7]),
		_m20(data[8]),
		_m21(data[9]),
		_m22(data[10]),
		_m23(data[11]),
		_m30(data[12]),
		_m31(data[13]),
		_m32(data[14]),
		_m33(data[15])
	{
	}

	/// Assign from another matrix.
	Matrix4x4& operator =(const Matrix4x4& rhs) noexcept
	{
		_m00 = rhs._m00;
		_m01 = rhs._m01;
		_m02 = rhs._m02;
		_m03 = rhs._m03;
		_m10 = rhs._m10;
		_m11 = rhs._m11;
		_m12 = rhs._m12;
		_m13 = rhs._m13;
		_m20 = rhs._m20;
		_m21 = rhs._m21;
		_m22 = rhs._m22;
		_m23 = rhs._m23;
		_m30 = rhs._m30;
		_m31 = rhs._m31;
		_m32 = rhs._m32;
		_m33 = rhs._m33;
		return *this;
	}

	/// Assign from a 3x3 matrix. Set the extra elements to identity.
	Matrix4x4& operator =(const Matrix3x3& rhs) noexcept
	{
		_m00 = rhs._m00;
		_m01 = rhs._m01;
		_m02 = rhs._m02;
		_m03 = 0.0f;
		_m10 = rhs._m10;
		_m11 = rhs._m11;
		_m12 = rhs._m12;
		_m13 = 0.0f;
		_m20 = rhs._m20;
		_m21 = rhs._m21;
		_m22 = rhs._m22;
		_m23 = 0.0f;
		_m30 = 0.0f;
		_m31 = 0.0f;
		_m32 = 0.0f;
		_m33 = 1.0f;
		return *this;
	}

	/// Test for equality with another matrix without epsilon.
	bool operator ==(const Matrix4x4& rhs) const
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
	bool operator !=(const Matrix4x4& rhs) const { return !(*this == rhs); }

	/// Multiply a Vector3 which is assumed to represent position.
	Vector3 operator *(const Vector3& rhs) const
	{
		float invW = 1.0f / (_m30 * rhs._x + _m31 * rhs._y + _m32 * rhs._z + _m33);

		return Vector3(
			(_m00 * rhs._x + _m01 * rhs._y + _m02 * rhs._z + _m03) * invW,
			(_m10 * rhs._x + _m11 * rhs._y + _m12 * rhs._z + _m13) * invW,
			(_m20 * rhs._x + _m21 * rhs._y + _m22 * rhs._z + _m23) * invW
		);
	}

	/// Multiply a Vector4.
	Vector4 operator *(const Vector4& rhs) const
	{
		return Vector4(
			_m00 * rhs._x + _m01 * rhs._y + _m02 * rhs._z + _m03 * rhs._w,
			_m10 * rhs._x + _m11 * rhs._y + _m12 * rhs._z + _m13 * rhs._w,
			_m20 * rhs._x + _m21 * rhs._y + _m22 * rhs._z + _m23 * rhs._w,
			_m30 * rhs._x + _m31 * rhs._y + _m32 * rhs._z + _m33 * rhs._w
		);
	}

	/// Add a matrix.
	Matrix4x4 operator +(const Matrix4x4& rhs) const
	{
		return Matrix4x4(
			_m00 + rhs._m00,
			_m01 + rhs._m01,
			_m02 + rhs._m02,
			_m03 + rhs._m03,
			_m10 + rhs._m10,
			_m11 + rhs._m11,
			_m12 + rhs._m12,
			_m13 + rhs._m13,
			_m20 + rhs._m20,
			_m21 + rhs._m21,
			_m22 + rhs._m22,
			_m23 + rhs._m23,
			_m30 + rhs._m30,
			_m31 + rhs._m31,
			_m32 + rhs._m32,
			_m33 + rhs._m33
		);
	}

	/// Subtract a matrix.
	Matrix4x4 operator -(const Matrix4x4& rhs) const
	{
		return Matrix4x4(
			_m00 - rhs._m00,
			_m01 - rhs._m01,
			_m02 - rhs._m02,
			_m03 - rhs._m03,
			_m10 - rhs._m10,
			_m11 - rhs._m11,
			_m12 - rhs._m12,
			_m13 - rhs._m13,
			_m20 - rhs._m20,
			_m21 - rhs._m21,
			_m22 - rhs._m22,
			_m23 - rhs._m23,
			_m30 - rhs._m30,
			_m31 - rhs._m31,
			_m32 - rhs._m32,
			_m33 - rhs._m33
		);
	}

	/// Multiply with a scalar.
	Matrix4x4 operator *(float rhs) const
	{
		return Matrix4x4(
			_m00 * rhs,
			_m01 * rhs,
			_m02 * rhs,
			_m03 * rhs,
			_m10 * rhs,
			_m11 * rhs,
			_m12 * rhs,
			_m13 * rhs,
			_m20 * rhs,
			_m21 * rhs,
			_m22 * rhs,
			_m23 * rhs,
			_m30 * rhs,
			_m31 * rhs,
			_m32 * rhs,
			_m33 * rhs
		);
	}

	/// Multiply a matrix.
	Matrix4x4 operator *(const Matrix4x4& rhs) const
	{
		return Matrix4x4(
			_m00 * rhs._m00 + _m01 * rhs._m10 + _m02 * rhs._m20 + _m03 * rhs._m30,
			_m00 * rhs._m01 + _m01 * rhs._m11 + _m02 * rhs._m21 + _m03 * rhs._m31,
			_m00 * rhs._m02 + _m01 * rhs._m12 + _m02 * rhs._m22 + _m03 * rhs._m32,
			_m00 * rhs._m03 + _m01 * rhs._m13 + _m02 * rhs._m23 + _m03 * rhs._m33,
			_m10 * rhs._m00 + _m11 * rhs._m10 + _m12 * rhs._m20 + _m13 * rhs._m30,
			_m10 * rhs._m01 + _m11 * rhs._m11 + _m12 * rhs._m21 + _m13 * rhs._m31,
			_m10 * rhs._m02 + _m11 * rhs._m12 + _m12 * rhs._m22 + _m13 * rhs._m32,
			_m10 * rhs._m03 + _m11 * rhs._m13 + _m12 * rhs._m23 + _m13 * rhs._m33,
			_m20 * rhs._m00 + _m21 * rhs._m10 + _m22 * rhs._m20 + _m23 * rhs._m30,
			_m20 * rhs._m01 + _m21 * rhs._m11 + _m22 * rhs._m21 + _m23 * rhs._m31,
			_m20 * rhs._m02 + _m21 * rhs._m12 + _m22 * rhs._m22 + _m23 * rhs._m32,
			_m20 * rhs._m03 + _m21 * rhs._m13 + _m22 * rhs._m23 + _m23 * rhs._m33,
			_m30 * rhs._m00 + _m31 * rhs._m10 + _m32 * rhs._m20 + _m33 * rhs._m30,
			_m30 * rhs._m01 + _m31 * rhs._m11 + _m32 * rhs._m21 + _m33 * rhs._m31,
			_m30 * rhs._m02 + _m31 * rhs._m12 + _m32 * rhs._m22 + _m33 * rhs._m32,
			_m30 * rhs._m03 + _m31 * rhs._m13 + _m32 * rhs._m23 + _m33 * rhs._m33
		);
	}

	/// Multiply with a 3x4 matrix.
	Matrix4x4 operator *(const Matrix3x4& rhs) const;

	/// Set translation elements.
	void SetTranslation(const Vector3& translation)
	{
		_m03 = translation._x;
		_m13 = translation._y;
		_m23 = translation._z;
	}

	/// Set rotation elements from a 3x3 matrix.
	void SetRotation(const Matrix3x3& rotation)
	{
		_m00 = rotation._m00;
		_m01 = rotation._m01;
		_m02 = rotation._m02;
		_m10 = rotation._m10;
		_m11 = rotation._m11;
		_m12 = rotation._m12;
		_m20 = rotation._m20;
		_m21 = rotation._m21;
		_m22 = rotation._m22;
	}

	/// Set scaling elements.
	void SetScale(const Vector3& scale)
	{
		_m00 = scale._x;
		_m11 = scale._y;
		_m22 = scale._z;
	}

	/// Set uniform scaling elements.
	void SetScale(float scale)
	{
		_m00 = scale;
		_m11 = scale;
		_m22 = scale;
	}

	/// Return the combined rotation and scaling matrix.
	Matrix3x3 ToMatrix3x3() const
	{
		return Matrix3x3(
			_m00,
			_m01,
			_m02,
			_m10,
			_m11,
			_m12,
			_m20,
			_m21,
			_m22
		);
	}

	/// Return the rotation matrix with scaling removed.
	Matrix3x3 RotationMatrix() const
	{
		Vector3 invScale(
			1.0f / sqrtf(_m00 * _m00 + _m10 * _m10 + _m20 * _m20),
			1.0f / sqrtf(_m01 * _m01 + _m11 * _m11 + _m21 * _m21),
			1.0f / sqrtf(_m02 * _m02 + _m12 * _m12 + _m22 * _m22)
		);

		return ToMatrix3x3().Scaled(invScale);
	}

	/// Return the translation part.
	Vector3 Translation() const
	{
		return Vector3(
			_m03,
			_m13,
			_m23
		);
	}


	/// Return the rotation part.
	Quaternion Rotation() const 
	{ 
		return Quaternion(RotationMatrix()); 
	}

	/// Return the scaling part.
	Vector3 Scale() const
	{
		return Vector3(
			sqrtf(_m00 * _m00 + _m10 * _m10 + _m20 * _m20),
			sqrtf(_m01 * _m01 + _m11 * _m11 + _m21 * _m21),
			sqrtf(_m02 * _m02 + _m12 * _m12 + _m22 * _m22)
		);
	}

	/// Return the scaling part with the sign. Reference rotation matrix is required to avoid ambiguity.
	Vector3 SignedScale(const Matrix3x3& rotation) const
	{
		return Vector3(
			rotation._m00 * _m00 + rotation._m10 * _m10 + rotation._m20 * _m20,
			rotation._m01 * _m01 + rotation._m11 * _m11 + rotation._m21 * _m21,
			rotation._m02 * _m02 + rotation._m12 * _m12 + rotation._m22 * _m22
		);
	}

	/// Return transposed.
	Matrix4x4 Transpose() const
	{
		return Matrix4x4(
			_m00,
			_m10,
			_m20,
			_m30,
			_m01,
			_m11,
			_m21,
			_m31,
			_m02,
			_m12,
			_m22,
			_m32,
			_m03,
			_m13,
			_m23,
			_m33
		);
	}

	/// Test for equality with another matrix with epsilon.
	bool Equals(const Matrix4x4& rhs) const
	{
		const float* leftData = Data();
		const float* rightData = rhs.Data();

		for (unsigned i = 0; i < 16; ++i)
		{
			if (!Auto3D::Equals(leftData[i], rightData[i]))
				return false;
		}

		return true;
	}

	/// Return decomposition to translation, rotation and scale.
	void Decompose(Vector3& translation, Quaternion& rotation, Vector3& scale) const;

	/// Return inverse.
	Matrix4x4 Inverse() const;

	/// Return float data.
	const float* Data() const { return &_m00; }

	/// Return matrix element.
	float Element(unsigned i, unsigned j) const { return Data()[i * 4 + j]; }

	/// Return matrix row.
	Vector4 Row(unsigned i) const { return Vector4(Element(i, 0), Element(i, 1), Element(i, 2), Element(i, 3)); }

	/// Return matrix column.
	Vector4 Column(unsigned j) const { return Vector4(Element(0, j), Element(1, j), Element(2, j), Element(3, j)); }

	/// Return as string.
	STRING ToString() const;

	float _m00;
	float _m01;
	float _m02;
	float _m03;
	float _m10;
	float _m11;
	float _m12;
	float _m13;
	float _m20;
	float _m21;
	float _m22;
	float _m23;
	float _m30;
	float _m31;
	float _m32;
	float _m33;

	/// Bulk transpose matrices.
	static void BulkTranspose(float* dest, const float* src, unsigned count)
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

	/// Zero matrix.
	static const Matrix4x4 ZERO;
	/// Identity matrix.
	static const Matrix4x4 IDENTITY;
};

/// Multiply a 4x4 matrix with a scalar.
inline Matrix4x4 operator *(float lhs, const Matrix4x4& rhs) { return rhs * lhs; }

}


