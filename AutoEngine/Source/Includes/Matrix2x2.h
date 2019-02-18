#pragma once
#include "MathBase.h"
#include "Vector2.h"

namespace Auto3D
{

/// 2x2 matrix for rotation and scaling.
class Matrix2x2
{
public:
	/// Construct an identity matrix.
	Matrix2x2() noexcept :
		_m00(1.0f),
		_m01(0.0f),
		_m10(0.0f),
		_m11(1.0f)
	{
	}

	/// Copy-construct from another matrix.
	Matrix2x2(const Matrix2x2& matrix) noexcept = default;

	/// Construct from values.
	Matrix2x2(float v00, float v01,
		float v10, float v11) noexcept :
		_m00(v00),
		_m01(v01),
		_m10(v10),
		_m11(v11)
	{
	}

	/// Construct from a float array.
	explicit Matrix2x2(const float* data) noexcept :
		_m00(data[0]),
		_m01(data[1]),
		_m10(data[2]),
		_m11(data[3])
	{
	}

	/// Assign from another matrix.
	Matrix2x2& operator =(const Matrix2x2& rhs) noexcept = default;

	/// Test for equality with another matrix without epsilon.
	bool operator ==(const Matrix2x2& rhs) const
	{
		const float* leftData = Data();
		const float* rightData = rhs.Data();

		for (unsigned i = 0; i != 4; ++i)
		{
			if (leftData[i] != rightData[i])
				return false;
		}

		return true;
	}

	/// Test for inequality with another matrix without epsilon.
	bool operator !=(const Matrix2x2& rhs) const { return !(*this == rhs); }

	/// Multiply a Vector2.
	Vector2 operator *(const Vector2& rhs) const
	{
		return Vector2(
			_m00 * rhs.x + _m01 * rhs.y,
			_m10 * rhs.x + _m11 * rhs.y
		);
	}

	/// Add a matrix.
	Matrix2x2 operator +(const Matrix2x2& rhs) const
	{
		return Matrix2x2(
			_m00 + rhs._m00,
			_m01 + rhs._m01,
			_m10 + rhs._m10,
			_m11 + rhs._m11
		);
	}

	/// Subtract a matrix.
	Matrix2x2 operator -(const Matrix2x2& rhs) const
	{
		return Matrix2x2(
			_m00 - rhs._m00,
			_m01 - rhs._m01,
			_m10 - rhs._m10,
			_m11 - rhs._m11
		);
	}

	/// Multiply with a scalar.
	Matrix2x2 operator *(float rhs) const
	{
		return Matrix2x2(
			_m00 * rhs,
			_m01 * rhs,
			_m10 * rhs,
			_m11 * rhs
		);
	}

	/// Multiply a matrix.
	Matrix2x2 operator *(const Matrix2x2& rhs) const
	{
		return Matrix2x2(
			_m00 * rhs._m00 + _m01 * rhs._m10,
			_m00 * rhs._m01 + _m01 * rhs._m11,
			_m10 * rhs._m00 + _m11 * rhs._m10,
			_m10 * rhs._m01 + _m11 * rhs._m11
		);
	}

	/// Set scaling elements.
	void SetScale(const Vector2& scale)
	{
		_m00 = scale.x;
		_m11 = scale.y;
	}

	/// Set uniform scaling elements.
	void SetScale(float scale)
	{
		_m00 = scale;
		_m11 = scale;
	}

	/// Return the scaling part.
	Vector2 Scale() const
	{
		return Vector2(
			sqrtf(_m00 * _m00 + _m10 * _m10),
			sqrtf(_m01 * _m01 + _m11 * _m11)
		);
	}

	/// Return transpose.
	Matrix2x2 Transpose() const
	{
		return Matrix2x2(
			_m00,
			_m10,
			_m01,
			_m11
		);
	}

	/// Return scaled by a vector.
	Matrix2x2 Scaled(const Vector2& scale) const
	{
		return Matrix2x2(
			_m00 * scale.x,
			_m01 * scale.y,
			_m10 * scale.x,
			_m11 * scale.y
		);
	}

	/// Test for equality with another matrix with epsilon.
	bool Equals(const Matrix2x2& rhs) const
	{
		const float* leftData = Data();
		const float* rightData = rhs.Data();

		for (unsigned i = 0; i != 4; ++i)
		{
			if (!Auto3D::Equals(leftData[i], rightData[i]))
				return false;
		}

		return true;
	}

	/// Return inverse.
	Matrix2x2 Inverse() const;

	/// Return float data.
	const float* Data() const { return &_m00; }

	/// Return as string.
	STRING ToString() const;

	float _m00;
	float _m01;
	float _m10;
	float _m11;

	/// Bulk transpose matrices.
	static void BulkTranspose(float* dest, const float* src, unsigned count)
	{
		for (unsigned i = 0; i < count; ++i)
		{
			dest[0] = src[0];
			dest[1] = src[2];
			dest[2] = src[1];
			dest[3] = src[3];

			dest += 4;
			src += 4;
		}
	}

	/// Zero matrix.
	static const Matrix2x2 ZERO;
	/// Identity matrix.
	static const Matrix2x2 IDENTITY;
};

/// Multiply a 2x2 matrix with a scalar.
inline Matrix2x2 operator *(float lhs, const Matrix2x2& rhs) { return rhs * lhs; }

}
