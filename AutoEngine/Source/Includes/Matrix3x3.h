#pragma once
#include "MathBase.h"
#include "Vector3.h"

namespace Auto3D {

/// 3x3 matrix for rotation and scaling.
class Matrix3x3
{
public:
	/// Construct an identity matrix.
	Matrix3x3() noexcept :
		_m00(1.0f),
		_m01(0.0f),
		_m02(0.0f),
		_m10(0.0f),
		_m11(1.0f),
		_m12(0.0f),
		_m20(0.0f),
		_m21(0.0f),
		_m22(1.0f)
	{
	}

	/// Copy-construct from another matrix.
	Matrix3x3(const Matrix3x3& matrix) noexcept = default;

	/// Construct from values.
	Matrix3x3(float v00, float v01, float v02,
		float v10, float v11, float v12,
		float v20, float v21, float v22) noexcept :
		_m00(v00),
		_m01(v01),
		_m02(v02),
		_m10(v10),
		_m11(v11),
		_m12(v12),
		_m20(v20),
		_m21(v21),
		_m22(v22)
	{
	}

	/// Construct from a float array.
	explicit Matrix3x3(const float* data) noexcept :
		_m00(data[0]),
		_m01(data[1]),
		_m02(data[2]),
		_m10(data[3]),
		_m11(data[4]),
		_m12(data[5]),
		_m20(data[6]),
		_m21(data[7]),
		_m22(data[8])
	{
	}

	/// Assign from another matrix.
	Matrix3x3& operator =(const Matrix3x3& rhs) noexcept = default;

	/// Test for equality with another matrix without epsilon.
	bool operator ==(const Matrix3x3& rhs) const
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

	/// Test for inequality with another matrix without epsilon.
	bool operator !=(const Matrix3x3& rhs) const { return !(*this == rhs); }

	/// Multiply a Vector3.
	Vector3 operator *(const Vector3& rhs) const
	{
		return Vector3(
			_m00 * rhs.x + _m01 * rhs.y + _m02 * rhs.z,
			_m10 * rhs.x + _m11 * rhs.y + _m12 * rhs.z,
			_m20 * rhs.x + _m21 * rhs.y + _m22 * rhs.z
		);
	}

	/// Add a matrix.
	Matrix3x3 operator +(const Matrix3x3& rhs) const
	{
		return Matrix3x3(
			_m00 + rhs._m00,
			_m01 + rhs._m01,
			_m02 + rhs._m02,
			_m10 + rhs._m10,
			_m11 + rhs._m11,
			_m12 + rhs._m12,
			_m20 + rhs._m20,
			_m21 + rhs._m21,
			_m22 + rhs._m22
		);
	}

	/// Subtract a matrix.
	Matrix3x3 operator -(const Matrix3x3& rhs) const
	{
		return Matrix3x3(
			_m00 - rhs._m00,
			_m01 - rhs._m01,
			_m02 - rhs._m02,
			_m10 - rhs._m10,
			_m11 - rhs._m11,
			_m12 - rhs._m12,
			_m20 - rhs._m20,
			_m21 - rhs._m21,
			_m22 - rhs._m22
		);
	}

	/// Multiply with a scalar.
	Matrix3x3 operator *(float rhs) const
	{
		return Matrix3x3(
			_m00 * rhs,
			_m01 * rhs,
			_m02 * rhs,
			_m10 * rhs,
			_m11 * rhs,
			_m12 * rhs,
			_m20 * rhs,
			_m21 * rhs,
			_m22 * rhs
		);
	}

	/// Multiply a matrix.
	Matrix3x3 operator *(const Matrix3x3& rhs) const
	{
		return Matrix3x3(
			_m00 * rhs._m00 + _m01 * rhs._m10 + _m02 * rhs._m20,
			_m00 * rhs._m01 + _m01 * rhs._m11 + _m02 * rhs._m21,
			_m00 * rhs._m02 + _m01 * rhs._m12 + _m02 * rhs._m22,
			_m10 * rhs._m00 + _m11 * rhs._m10 + _m12 * rhs._m20,
			_m10 * rhs._m01 + _m11 * rhs._m11 + _m12 * rhs._m21,
			_m10 * rhs._m02 + _m11 * rhs._m12 + _m12 * rhs._m22,
			_m20 * rhs._m00 + _m21 * rhs._m10 + _m22 * rhs._m20,
			_m20 * rhs._m01 + _m21 * rhs._m11 + _m22 * rhs._m21,
			_m20 * rhs._m02 + _m21 * rhs._m12 + _m22 * rhs._m22
		);
	}

	/// Set scaling elements.
	void SetScale(const Vector3& scale)
	{
		_m00 = scale.x;
		_m11 = scale.y;
		_m22 = scale.z;
	}

	/// Set uniform scaling elements.
	void SetScale(float scale)
	{
		_m00 = scale;
		_m11 = scale;
		_m22 = scale;
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
	Matrix3x3 Transpose() const
	{
		return Matrix3x3(
			_m00,
			_m10,
			_m20,
			_m01,
			_m11,
			_m21,
			_m02,
			_m12,
			_m22
		);
	}

	/// Return scaled by a vector.
	Matrix3x3 Scaled(const Vector3& scale) const
	{
		return Matrix3x3(
			_m00 * scale.x,
			_m01 * scale.y,
			_m02 * scale.z,
			_m10 * scale.x,
			_m11 * scale.y,
			_m12 * scale.z,
			_m20 * scale.x,
			_m21 * scale.y,
			_m22 * scale.z
		);
	}

	/// Test for equality with another matrix with epsilon.
	bool Equals(const Matrix3x3& rhs) const
	{
		const float* leftData = Data();
		const float* rightData = rhs.Data();

		for (unsigned i = 0; i < 9; ++i)
		{
			if (!Auto3D::Equals(leftData[i], rightData[i]))
				return false;
		}

		return true;
	}

	/// Return inverse.
	Matrix3x3 Inverse() const;

	/// Return float data.
	const float* Data() const { return &_m00; }

	/// Return matrix element.
	float Element(unsigned i, unsigned j) const { return Data()[i * 3 + j]; }

	/// Return matrix row.
	Vector3 Row(unsigned i) const { return Vector3(Element(i, 0), Element(i, 1), Element(i, 2)); }

	/// Return matrix column.
	Vector3 Column(unsigned j) const { return Vector3(Element(0, j), Element(1, j), Element(2, j)); }

	/// Return as string.
	STRING ToString() const;

	float _m00;
	float _m01;
	float _m02;
	float _m10;
	float _m11;
	float _m12;
	float _m20;
	float _m21;
	float _m22;

	/// Bulk transpose matrices.
	static void BulkTranspose(float* dest, const float* src, unsigned count)
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

	/// Zero matrix.
	static const Matrix3x3 ZERO;
	/// Identity matrix.
	static const Matrix3x3 IDENTITY;
};

/// Multiply a 3x3 matrix with a scalar.
inline Matrix3x3 operator *(float lhs, const Matrix3x3& rhs) { return rhs * lhs; }


}

