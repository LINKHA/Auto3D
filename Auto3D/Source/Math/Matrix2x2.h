#pragma once

#include "Vector2.h"

namespace Auto3D
{

/// 2x2 matrix for rotation and scaling.
template<typename _Ty> class AUTO_API Matrix2x2
{
public:
	/// Matrix values.
	_Ty _m00, _m01;
	_Ty _m10, _m11;
	/*/// Addresses of all members
	_Ty* _data[4] =
	{
		&_m00, &_m01,
		&_m10, &_m11
	}*/

	/// Construct an identity matrix.
	///|1,0|
	///|0,1|
	Matrix2x2() noexcept :
		_m00(1),
		_m01(0),
		_m10(0),
		_m11(1)
	{
	}

	/// Copy-construct from another matrix.
	Matrix2x2(const Matrix2x2& matrix) noexcept = default;

	/// Construct from values.
	Matrix2x2(_Ty v00, _Ty v01,
		_Ty v10, _Ty v11) noexcept :
		_m00(v00),
		_m01(v01),
		_m10(v10),
		_m11(v11)
	{
	}

	/// Construct from a _Ty array.
	explicit Matrix2x2(const _Ty* data) noexcept :
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
		const _Ty* leftData = Data();
		const _Ty* rightData = rhs.Data();

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
	Vector2<_Ty> operator *(const Vector2<_Ty>& rhs) const
	{
		return Vector2<_Ty>(
			_m00 * rhs._x + _m01 * rhs._y,
			_m10 * rhs._x + _m11 * rhs._y
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
	Matrix2x2 operator *(_Ty rhs) const
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
	void SetScale(const Vector2<_Ty>& scale)
	{
		_m00 = scale._x;
		_m11 = scale._y;
	}

	/// Set uniform scaling elements.
	void SetScale(_Ty scale)
	{
		_m00 = scale;
		_m11 = scale;
	}

	/// Return the scaling part.
	Vector2<_Ty> Scale() const
	{
		return Vector2<_Ty>(
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
	Matrix2x2 Scaled(const Vector2<_Ty>& scale) const
	{
		return Matrix2x2(
			_m00 * scale._x,
			_m01 * scale._y,
			_m10 * scale._x,
			_m11 * scale._y
		);
	}

	/// Test for equality with another matrix with epsilon.
	bool Equals(const Matrix2x2& rhs) const
	{
		const _Ty* leftData = Data();
		const _Ty* rightData = rhs.Data();

		for (unsigned i = 0; i != 4; ++i)
		{
			if (!Auto3D::Equals(leftData[i], rightData[i]))
				return false;
		}

		return true;
	}

	/// Return inverse.
	Matrix2x2 Inverse() const
	{
		float det = _m00 * _m11 -
			_m01 * _m10;

		float invDet = 1.0f / det;

		return Matrix2x2<_Ty>(
			_m11, -_m01,
			-_m10, _m00
			) * invDet;
	}


	/// Return _Ty data.
	const _Ty* Data() const { return &_m00; }

	/// Return as string.
	String ToString() const
	{
		char tempBuffer[CONVERSION_BUFFER_LENGTH];
		sprintf(tempBuffer, "%g %g %g %g", _m00, _m01, _m10, _m11);
		return String(tempBuffer);
	}

	/// Bulk transpose matrices.
	static void BulkTranspose(_Ty* dest, const _Ty* src, unsigned count)
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
template<typename _Ty> Matrix2x2<_Ty> operator *(_Ty lhs, const Matrix2x2<_Ty>& rhs) { return rhs * lhs; }

using Matrix2x2F = Matrix2x2<float>;

using Matrix2x2I = Matrix2x2<int>;

using Matrix2x2C = Matrix2x2<char>;

using Matrix2x2D = Matrix2x2<double>;

using Matrix2x2U = Matrix2x2<unsigned>;

}
