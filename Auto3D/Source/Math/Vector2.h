#pragma once

#include "Math.h"
#include "../AutoConfig.h"

#include "../Base/String.h"
#include <cstdio>
#include <cstdlib>

namespace Auto3D
{

class String;

/// Two-dimensional vector.
template<typename _Ty> class AUTO_API Vector2
{
public:
	/// X coordinate.
	_Ty _x;
	/// Y coordinate.
	_Ty _y;

	/// Construct a zero vector.
	Vector2() noexcept :
		_x(0),
		_y(0)
	{
	}

	/// Copy-construct.
	Vector2(const Vector2& vector) :
		_x(vector._x),
		_y(vector._y)
	{
	}

	/// Construct from coordinates.
	Vector2(_Ty x, _Ty y) :
		_x(x),
		_y(y)
	{
	}

	/// Construct from a _Ty array.
	Vector2(const _Ty* _data) :
		_x(_data[0]),
		_y(_data[1])
	{
	}

	/// Construct by parsing a string.
	Vector2(const String& str)
	{
		FromString(str);
	}

	/// Assign from another vector.
	Vector2& operator = (const Vector2& rhs)
	{
		_x = rhs._x;
		_y = rhs._y;
		return *this;
	}

	/// Add-assign a vector.
	Vector2& operator += (const Vector2& rhs)
	{
		_x += rhs._x;
		_y += rhs._y;
		return *this;
	}

	/// Subtract-assign a vector.
	Vector2& operator -= (const Vector2& rhs)
	{
		_x -= rhs._x;
		_y -= rhs._y;
		return *this;
	}

	/// Multiply-assign a scalar.
	Vector2& operator *= (_Ty rhs)
	{
		_x *= rhs;
		_y *= rhs;
		return *this;
	}

	/// Multiply-assign a vector.
	Vector2& operator *= (const Vector2& rhs)
	{
		_x *= rhs._x;
		_y *= rhs._y;
		return *this;
	}

	/// Divide-assign a scalar.
	Vector2& operator /= (_Ty rhs)
	{
		_Ty invRhs = 1.0f / rhs;
		_x *= invRhs;
		_y *= invRhs;
		return *this;
	}

	/// Divide-assign a vector.
	Vector2& operator /= (const Vector2& rhs)
	{
		_x /= rhs._x;
		_y /= rhs._y;
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
		}
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
		if (elements < 2)
			return false;

		char* ptr = (char*)str;
		_x = (_Ty)strtod(ptr, &ptr);
		_y = (_Ty)strtod(ptr, &ptr);

		return true;
	}

	/// Test for equality with another vector without epsilon.
	bool operator == (const Vector2& rhs) const { return _x == rhs._x && _y == rhs._y; }
	/// Test for inequality with another vector without epsilon.
	bool operator != (const Vector2& rhs) const { return !(*this == rhs); }
	/// Add a vector.
	Vector2 operator + (const Vector2& rhs) const { return Vector2(_x + rhs._x, _y + rhs._y); }
	/// Return negation.
	Vector2 operator - () const { return Vector2(-_x, -_y); }
	/// Subtract a vector.
	Vector2 operator - (const Vector2& rhs) const { return Vector2(_x - rhs._x, _y - rhs._y); }
	/// Multiply with a scalar.
	Vector2 operator * (_Ty rhs) const { return Vector2(_x * rhs, _y * rhs); }
	/// Multiply with a vector.
	Vector2 operator * (const Vector2& rhs) const { return Vector2(_x * rhs._x, _y * rhs._y); }
	/// Divide by a scalar.
	Vector2 operator / (_Ty rhs) const { return Vector2(_x / rhs, _y / rhs); }
	/// Divide by a vector.
	Vector2 operator / (const Vector2& rhs) const { return Vector2(_x / rhs._x, _y / rhs._y); }

	/// Return length.
	_Ty Length() const { return sqrtf(_x * _x + _y * _y); }
	/// Return squared length.
	_Ty LengthSquared() const { return _x * _x + _y * _y; }
	/// Calculate dot product.
	_Ty DotProduct(const Vector2& rhs) const { return _x * rhs._x + _y * rhs._y; }
	/// Calculate absolute dot product.
	_Ty AbsDotProduct(const Vector2& rhs) const { return Auto3D::Abs(_x * rhs._x) + Auto3D::Abs(_y * rhs._y); }
	/// Return absolute vector.
	Vector2 Abs() const { return Vector2(Auto3D::Abs(_x), Auto3D::Abs(_y)); }
	/// Linear interpolation with another vector.
	Vector2 Lerp(const Vector2& rhs, _Ty t) const { return *this * (1.0f - t) + rhs * t; }
	/// Test for equality with another vector with epsilon.
	bool Equals(const Vector2& rhs) const { return Auto3D::Equals(_x, rhs._x) && Auto3D::Equals(_y, rhs._y); }
	/// Return whether is NaN.
	bool IsNaN() const { return Auto3D::IsNaN(_x) || Auto3D::IsNaN(_y); }

	/// Return normalized to unit length.
	Vector2 Normalized() const
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
	String ToString() const
	{
		char tempBuffer[CONVERSION_BUFFER_LENGTH];
		sprintf(tempBuffer, "%g %g", _x, _y);
		return String(tempBuffer);
	}

	/// Zero vector.
	static const Vector2 ZERO;
	/// (-1,0) vector.
	static const Vector2 LEFT;
	/// (1,0) vector.
	static const Vector2 RIGHT;
	/// (0,1) vector.
	static const Vector2 UP;
	/// (0,-1) vector.
	static const Vector2 DOWN;
	/// (1,1) vector.
	static const Vector2 ONE;
};

/// Construct by parsing a C string.
Vector2<char>::Vector2<char>(const char* str)
{
	FromString(str);
}
	
/// Multiply Vector2 with a scalar
template<typename _Ty> Vector2<_Ty> operator * (_Ty lhs, const Vector2<_Ty>& rhs) { return rhs * lhs; }

using Vector2F = Vector2<float>;

using Vector2I = Vector2<int>;

using Vector2C = Vector2<char>;

using Vector2D = Vector2<double>;

using Vector2U = Vector2<unsigned>;

}
