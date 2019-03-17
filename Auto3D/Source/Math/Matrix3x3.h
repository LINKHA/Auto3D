#pragma once

#include "Vector3.h"

namespace Auto3D
{

/// 3x3 matrix for rotation and scaling.
template<typename _Ty> class AUTO_API Matrix3x3
{
public:
	/// Matrix values.
	_Ty _m00, _m01, _m02;
	_Ty _m10, _m11, _m12;
	_Ty _m20, _m21, _m22;

	/// Construct an identity matrix.
	///|1,0,0|
	///|0,1,0|
	///|0,0,1|
	Matrix3x3() noexcept :
		_m00(1),
		_m01(0),
		_m02(0),
		_m10(0),
		_m11(1),
		_m12(0),
		_m20(0),
		_m21(0),
		_m22(1)
	{
	}

	/// Copy-construct.
	Matrix3x3(const Matrix3x3& matrix) :
		_m00(matrix._m00), _m01(matrix._m01), _m02(matrix._m02),
		_m10(matrix._m10), _m11(matrix._m11), _m12(matrix._m12),
		_m20(matrix._m20), _m21(matrix._m21), _m22(matrix._m22)
	{
	}

	/// Construct from values.
	Matrix3x3(_Ty v00, _Ty v01, _Ty v02,
		_Ty v10, _Ty v11, _Ty v12,
		_Ty v20, _Ty v21, _Ty v22) :
		_m00(v00), _m01(v01), _m02(v02),
		_m10(v10), _m11(v11), _m12(v12),
		_m20(v20), _m21(v21), _m22(v22)
	{
	}

	/// Construct from a _Ty array.
	Matrix3x3(const _Ty* _data) :
		_m00(_data[0]), _m01(_data[1]), _m02(_data[2]),
		_m10(_data[3]), _m11(_data[4]), _m12(_data[5]),
		_m20(_data[6]), _m21(_data[7]), _m22(_data[8])
	{
	}

	/// Construct by parsing a string.
	Matrix3x3(const String& str)
	{
		FromString(str);
	}
		
		/// Assign from another matrix.
	Matrix3x3& operator = (const Matrix3x3& rhs)
	{
		_m00 = rhs._m00; _m01 = rhs._m01; _m02 = rhs._m02;
		_m10 = rhs._m10; _m11 = rhs._m11; _m12 = rhs._m12;
		_m20 = rhs._m20; _m21 = rhs._m21; _m22 = rhs._m22;
		return *this;
	}

	/// Test for equality with another matrix without epsilon.
	bool operator == (const Matrix3x3& rhs) const
	{
		const _Ty* leftData = Data();
		const _Ty* rightData = rhs.Data();

		for (size_t i = 0; i < 9; ++i)
		{
			if (leftData[i] != rightData[i])
				return false;
		}

		return true;
	}

	/// Test for inequality with another matrix without epsilon.
	bool operator != (const Matrix3x3& rhs) const { return !(*this == rhs); }

	/// Multiply a Vector3
	Vector3<_Ty> operator * (const Vector3<_Ty>& rhs) const
	{
		return Vector3<_Ty>(
			_m00 * rhs._x + _m01 * rhs._y + _m02 * rhs._z,
			_m10 * rhs._x + _m11 * rhs._y + _m12 * rhs._z,
			_m20 * rhs._x + _m21 * rhs._y + _m22 * rhs._z
			);
	}

	/// Add a matrix.
	Matrix3x3 operator + (const Matrix3x3& rhs) const
	{
		return Matrix3x3(
			_m00 + rhs._m00, _m01 + rhs._m01, _m02 + rhs._m02,
			_m10 + rhs._m10, _m11 + rhs._m11, _m12 + rhs._m12,
			_m20 + rhs._m20, _m21 + rhs._m21, _m22 + rhs._m22
		);
	}

	/// Subtract a matrix.
	Matrix3x3 operator - (const Matrix3x3& rhs) const
	{
		return Matrix3x3(
			_m00 - rhs._m00, _m01 - rhs._m01, _m02 - rhs._m02,
			_m10 - rhs._m10, _m11 - rhs._m11, _m12 - rhs._m12,
			_m20 - rhs._m20, _m21 - rhs._m21, _m22 - rhs._m22
		);
	}

	/// Multiply with a scalar.
	Matrix3x3 operator * (_Ty rhs) const
	{
		return Matrix3x3(
			_m00 * rhs, _m01 * rhs, _m02 * rhs,
			_m10 * rhs, _m11 * rhs, _m12 * rhs,
			_m20 * rhs, _m21 * rhs, _m22 * rhs
		);
	}

	/// Multiply a matrix.
	Matrix3x3 operator * (const Matrix3x3& rhs) const
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
	void SetScale(const Vector3<_Ty>& scale)
	{
		_m00 = scale._x;
		_m11 = scale._y;
		_m22 = scale._z;
	}

	/// Set uniform scaling elements.
	void SetScale(_Ty scale)
	{
		_m00 = scale;
		_m11 = scale;
		_m22 = scale;
	}

	/// Parse from a string. Return true on success.
	bool FromString(const String& str)
	{
		return FromString(str.CString());
	}

	/// Parse from a C string. Return true on success.
	bool FromString(const char* str)
	{
		size_t elements = String::CountElements(str, ' ');
		if (elements < 9)
			return false;

		char* ptr = (char*)str;
		_m00 = (float)strtod(ptr, &ptr);
		_m01 = (float)strtod(ptr, &ptr);
		_m02 = (float)strtod(ptr, &ptr);
		_m10 = (float)strtod(ptr, &ptr);
		_m11 = (float)strtod(ptr, &ptr);
		_m12 = (float)strtod(ptr, &ptr);
		_m20 = (float)strtod(ptr, &ptr);
		_m21 = (float)strtod(ptr, &ptr);
		_m22 = (float)strtod(ptr, &ptr);

		return true;
	}

	/// Return the scaling part.
	Vector3<_Ty> Scale() const
	{
		return Vector3<_Ty>(
			sqrtf(_m00 * _m00 + _m10 * _m10 + _m20 * _m20),
			sqrtf(_m01 * _m01 + _m11 * _m11 + _m21 * _m21),
			sqrtf(_m02 * _m02 + _m12 * _m12 + _m22 * _m22)
			);
	}

	/// Return transpose.
	Matrix3x3 Transpose() const
	{
		return Matrix3x3(
			_m00, _m10, _m20,
			_m01, _m11, _m21,
			_m02, _m12, _m22
		);
	}

	/// Return scaled by a vector.
	Matrix3x3 Scaled(const Vector3<_Ty>& scale) const
	{
		return Matrix3x3(
			_m00 * scale._x, _m01 * scale._y, _m02 * scale._z,
			_m10 * scale._x, _m11 * scale._y, _m12 * scale._z,
			_m20 * scale._x, _m21 * scale._y, _m22 * scale._z
		);
	}

	/// Test for equality with another matrix with epsilon.
	bool Equals(const Matrix3x3& rhs) const
	{
		const _Ty* leftData = Data();
		const _Ty* rightData = rhs.Data();

		for (size_t i = 0; i < 9; ++i)
		{
			if (!Auto3D::Equals(leftData[i], rightData[i]))
				return false;
		}

		return true;
	}

	/// Return inverse.
	Matrix3x3 Inverse() const
	{
		float det = _m00 * _m11 * _m22 +
			_m10 * _m21 * _m02 +
			_m20 * _m01 * _m12 -
			_m20 * _m11 * _m02 -
			_m10 * _m01 * _m22 -
			_m00 * _m21 * _m12;

		float invDet = 1.0f / det;

		return Matrix3x3<_Ty>(
			(_m11 * _m22 - _m21 * _m12) * invDet,
			-(_m01 * _m22 - _m21 * _m02) * invDet,
			(_m01 * _m12 - _m11 * _m02) * invDet,
			-(_m10 * _m22 - _m20 * _m12) * invDet,
			(_m00 * _m22 - _m20 * _m02) * invDet,
			-(_m00 * _m12 - _m10 * _m02) * invDet,
			(_m10 * _m21 - _m20 * _m11) * invDet,
			-(_m00 * _m21 - _m20 * _m01) * invDet,
			(_m00 * _m11 - _m10 * _m01) * invDet
			);
	}

	/// Return _Ty data.
	const _Ty* Data() const { return &_m00; }
	/// Return as string.
	String ToString() const
	{
		char tempBuffer[CONVERSION_BUFFER_LENGTH];
		sprintf(tempBuffer, "%g %g %g %g %g %g %g %g %g", _m00, _m01, _m02, _m10, _m11, _m12, _m20, _m21, _m22);
		return String(tempBuffer);
	}

	/// Zero matrix.
	static const Matrix3x3 ZERO;
	/// Identity matrix.
	static const Matrix3x3 IDENTITY;
};

/// Construct by parsing a C string.
template<> Matrix3x3<char>::Matrix3x3(const char* str)
{
	FromString(str);
}

/// Multiply a 3x3 matrix with a scalar.
template<typename _Ty> Matrix3x3<_Ty> operator * (_Ty lhs, const Matrix3x3<_Ty>& rhs) { return rhs * lhs; }

using Matrix3x3F = Matrix3x3<float>;

using Matrix3x3I = Matrix3x3<int>;

using Matrix3x3C = Matrix3x3<char>;

using Matrix3x3D = Matrix3x3<double>;

using Matrix3x3U = Matrix3x3<unsigned>;

}
