#pragma once

#include "Vector2.h"

namespace Auto3D
{

/// Three-dimensional vector.
template<typename _Ty> class AUTO_API TVector3
{
public:
	/// X coordinate.
	_Ty _x;
	/// Y coordinate.
	_Ty _y;
	/// Z coordinate.
	_Ty _z;

	/// Construct a zero vector.
	TVector3() noexcept :
		_x(0),
		_y(0),
		_z(0)
	{
	}

	/// Copy-construct.
	TVector3(const TVector3& vector) :
		_x(vector._x),
		_y(vector._y),
		_z(vector._z)
	{
	}

	/// Construct from a two-dimensional vector and the Z coordinate.
	TVector3(const TVector2<_Ty>& vector, _Ty z) :
		_x(vector._x),
		_y(vector._y),
		_z(z)
	{
	}

	/// Construct from a two-dimensional vector, with Z coordinate left zero.
	TVector3(const TVector2<_Ty>& vector) :
		_x(vector._x),
		_y(vector._y),
		_z(0.0f)
	{
	}

	/// Construct from coordinates.
	TVector3(_Ty x, _Ty y, _Ty z) :
		_x(x),
		_y(y),
		_z(z)
	{
	}

	/// Construct from two-dimensional coordinates, with Z coordinate left zero.
	TVector3(_Ty x, _Ty y) :
		_x(x),
		_y(y),
		_z(0.0f)
	{
	}

	/// Construct from a _Ty array.
	TVector3(const _Ty* _data) :
		_x(_data[0]),
		_y(_data[1]),
		_z(_data[2])
	{
	}

	/// Construct by parsing a string.
	TVector3(const FString& str)
	{
		FromString(str);
	}

	/// Assign from another vector.
	TVector3& operator = (const TVector3& rhs)
	{
		_x = rhs._x;
		_y = rhs._y;
		_z = rhs._z;
		return *this;
	}

	/// Test for equality with another vector without epsilon.
	bool operator == (const TVector3& rhs) const { return _x == rhs._x && _y == rhs._y && _z == rhs._z; }
	/// Test for inequality with another vector without epsilon.
	bool operator != (const TVector3& rhs) const { return !(*this == rhs); }
	/// Add a vector.
	TVector3 operator + (const TVector3& rhs) const { return TVector3(_x + rhs._x, _y + rhs._y, _z + rhs._z); }
	/// Return negation.
	TVector3 operator - () const { return TVector3(-_x, -_y, -_z); }
	/// Subtract a vector.
	TVector3 operator - (const TVector3& rhs) const { return TVector3(_x - rhs._x, _y - rhs._y, _z - rhs._z); }
	/// Multiply with a scalar.
	TVector3 operator * (_Ty rhs) const { return TVector3(_x * rhs, _y * rhs, _z * rhs); }
	/// Multiply with a vector.
	TVector3 operator * (const TVector3& rhs) const { return TVector3(_x * rhs._x, _y * rhs._y, _z * rhs._z); }
	/// Divide by a scalar.
	TVector3 operator / (_Ty rhs) const { return TVector3(_x / rhs, _y / rhs, _z / rhs); }
	/// Divide by a vector.
	TVector3 operator / (const TVector3& rhs) const { return TVector3(_x / rhs._x, _y / rhs._y, _z / rhs._z); }

	/// Add-assign a vector.
	TVector3& operator += (const TVector3& rhs)
	{
		_x += rhs._x;
		_y += rhs._y;
		_z += rhs._z;
		return *this;
	}

	/// Subtract-assign a vector.
	TVector3& operator -= (const TVector3& rhs)
	{
		_x -= rhs._x;
		_y -= rhs._y;
		_z -= rhs._z;
		return *this;
	}

	/// Multiply-assign a scalar.
	TVector3& operator *= (_Ty rhs)
	{
		_x *= rhs;
		_y *= rhs;
		_z *= rhs;
		return *this;
	}

	/// Multiply-assign a vector.
	TVector3& operator *= (const TVector3& rhs)
	{
		_x *= rhs._x;
		_y *= rhs._y;
		_z *= rhs._z;
		return *this;
	}

	/// Divide-assign a scalar.
	TVector3& operator /= (_Ty rhs)
	{
		_Ty invRhs = 1.0f / rhs;
		_x *= invRhs;
		_y *= invRhs;
		_z *= invRhs;
		return *this;
	}

	/// Divide-assign a vector.
	TVector3& operator /= (const TVector3& rhs)
	{
		_x /= rhs._x;
		_y /= rhs._y;
		_z /= rhs._z;
		return *this;
	}

	/// Normalize to unit length.
	void Normalize()
	{
		_Ty lenSquared = LengthSquared();
		if (!Auto3D::Equals(lenSquared, 1.0f) && lenSquared > 0.0f)
		{
			_Ty invLen = 1.0f / sqrtf(lenSquared);
			_x *= invLen;
			_y *= invLen;
			_z *= invLen;
		}
	}

	/// Parse from a string. Return true on success.
	bool FromString(const FString& str)
	{
		return FromString(str.CString());
	}

	/// Parse from a C string. Return true on success.
	bool FromString(const char* str)
	{
		size_t elements = FString::CountElements(str, ' ');
		if (elements < 3)
			return false;

		char* ptr = (char*)str;
		_x = (_Ty)strtod(ptr, &ptr);
		_y = (_Ty)strtod(ptr, &ptr);
		_z = (_Ty)strtod(ptr, &ptr);

		return true;
	}

	/// Return length.
	_Ty Length() const { return sqrtf(_x * _x + _y * _y + _z * _z); }
	/// Return squared length.
	_Ty LengthSquared() const { return _x * _x + _y * _y + _z * _z; }
	/// Calculate dot product.
	_Ty DotProduct(const TVector3& rhs) const { return _x * rhs._x + _y * rhs._y + _z * rhs._z; }
	/// Calculate absolute dot product.
	_Ty AbsDotProduct(const TVector3& rhs) const { return Auto3D::Abs(_x * rhs._x) + Auto3D::Abs(_y * rhs._y) + Auto3D::Abs(_z * rhs._z); }

	/// Calculate cross product.
	TVector3 CrossProduct(const TVector3& rhs) const
	{
		return TVector3(
			_y * rhs._z - _z * rhs._y,
			_z * rhs._x - _x * rhs._z,
			_x * rhs._y - _y * rhs._x
		);
	}

	/// Return absolute vector.
	TVector3 Abs() const { return TVector3(Auto3D::Abs(_x), Auto3D::Abs(_y), Auto3D::Abs(_z)); }
	/// Linear interpolation with another vector.
	TVector3 Lerp(const TVector3& rhs, _Ty t) const { return *this * (1.0f - t) + rhs * t; }
	/// Test for equality with another vector with epsilon.
	bool Equals(const TVector3& rhs) const { return Auto3D::Equals(_x, rhs._x) && Auto3D::Equals(_y, rhs._y) && Auto3D::Equals(_z, rhs._z); }
	/// Return the angle between this vector and another vector in degrees.
	_Ty Angle(const TVector3& rhs) const { return Auto3D::Acos(DotProduct(rhs) / (Length() * rhs.Length())); }
	/// Return whether is NaN.
	bool IsNaN() const { return Auto3D::IsNaN(_x) || Auto3D::IsNaN(_y) || Auto3D::IsNaN(_z); }

	/// Return normalized to unit length.
	TVector3 Normalized() const
	{
		_Ty lenSquared = LengthSquared();
		if (!Auto3D::Equals(lenSquared, 1.0f) && lenSquared > 0.0f)
		{
			_Ty invLen = 1.0f / sqrtf(lenSquared);
			return *this * invLen;
		}
		else
			return *this;
	}

	/// Return _Ty data.
	const _Ty* Data() const { return &_x; }
	/// Return as string.
	FString ToString() const
	{
		char tempBuffer[CONVERSION_BUFFER_LENGTH];
		sprintf(tempBuffer, "%g %g %g", _x, _y, _z);
		return FString(tempBuffer);
	}


	/// Zero vector.
	static const TVector3 ZERO;
	/// (-1,0,0) vector.
	static const TVector3 LEFT;
	/// (1,0,0) vector.
	static const TVector3 RIGHT;
	/// (0,1,0) vector.
	static const TVector3 UP;
	/// (0,-1,0) vector.
	static const TVector3 DOWN;
	/// (0,0,1) vector.
	static const TVector3 FORWARD;
	/// (0,0,-1) vector.
	static const TVector3 BACK;
	/// (1,1,1) vector.
	static const TVector3 ONE;
};

/// Construct by parsing a C string.
TVector3<char>::TVector3(const char* str)
{
	FromString(str);
}
	
/// Multiply TVector3 with a scalar.
template <typename _Ty> TVector3<_Ty> operator * (_Ty lhs, const TVector3<_Ty>& rhs) { return rhs * lhs; }

using TVector3F = TVector3<float>;

using TVector3I = TVector3<int>;

using TVector3C = TVector3<char>;

using TVector3D = TVector3<double>;

using TVector3U = TVector3<unsigned>;

template<typename _Ty> TVector3<_Ty> Cross(const TVector3<_Ty>& x, const TVector3<_Ty>& y)
{
	return TVector3<_Ty>(
		x._y * y._z - y._y * x._z,
		x._z * y._x - y._z * x._x,
		x._x * y._y - y._x * x._y);
}
/// Peform the dot product on two vectors.
template<typename _Ty> inline _Ty Dot(const TVector3<_Ty>& a, const TVector3<_Ty>& b) { TVector3<_Ty>tmp(a*b); return tmp._x + tmp._y+ tmp._z; }

}
