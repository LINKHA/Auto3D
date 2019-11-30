#pragma once

#include "Vector3.h"

namespace Auto3D
{

/// Four-dimensional vector.
template<typename _Ty> class AUTO_API TVector4
{
public:
	/// X coordinate.
	float _x;
	/// Y coordinate.
	float _y;
	/// Z coordinate.
	float _z;
	/// W coordinate.
	float _w;

	/// Construct a zero vector.
	TVector4() noexcept :
		_x(0),
		_y(0),
		_z(0),
		_w(0)
	{
	}

	/// Copy-construct.
	TVector4(const TVector4& vector) :
		_x(vector._x),
		_y(vector._y),
		_z(vector._z),
		_w(vector._w)
	{
	}

	/// Construct from a 3-dimensional vector and the W coordinate.
	TVector4(const TVector3<_Ty>& vector, float w) :
		_x(vector._x),
		_y(vector._y),
		_z(vector._z),
		_w(w)
	{
	}

	/// Construct from coordinates.
	TVector4(float x, float y, float z, float w) :
		_x(x),
		_y(y),
		_z(z),
		_w(w)
	{
	}

	/// Construct from a float array.
	TVector4(const float* _data) :
		_x(_data[0]),
		_y(_data[1]),
		_z(_data[2]),
		_w(_data[3])
	{
	}

	/// Assign from another vector.
	TVector4& operator = (const TVector4& rhs)
	{
		_x = rhs._x;
		_y = rhs._y;
		_z = rhs._z;
		_w = rhs._w;
		return *this;
	}

	/// Construct by parsing a string.
	TVector4(const FString& str)
	{
		FromString(str);
	}

	/// Construct by parsing a C string.
	TVector4(const char* str)
	{
		FromString(str);
	}

	/// Test for equality with another vector without epsilon.
	bool operator == (const TVector4& rhs) const { return _x == rhs._x && _y == rhs._y && _z == rhs._z && _w == rhs._w; }
	/// Test for inequality with another vector without epsilon.
	bool operator != (const TVector4& rhs) const { return !(*this == rhs); }
	/// Add a vector.
	TVector4 operator + (const TVector4& rhs) const { return TVector4(_x + rhs._x, _y + rhs._y, _z + rhs._z, _w + rhs._w); }
	/// Return negation.
	TVector4 operator - () const { return TVector4(-_x, -_y, -_z, -_w); }
	/// Subtract a vector.
	TVector4 operator - (const TVector4& rhs) const { return TVector4(_x - rhs._x, _y - rhs._y, _z - rhs._z, _w - rhs._w); }
	/// Multiply with a scalar.
	TVector4 operator * (float rhs) const { return TVector4(_x * rhs, _y * rhs, _z * rhs, _w * rhs); }
	/// Multiply with a vector.
	TVector4 operator * (const TVector4& rhs) const { return TVector4(_x * rhs._x, _y * rhs._y, _z * rhs._z, _w * rhs._w); }
	/// Divide by a scalar.
	TVector4 operator / (float rhs) const { return TVector4(_x / rhs, _y / rhs, _z / rhs, _w / rhs); }
	/// Divide by a vector.
	TVector4 operator / (const TVector4& rhs) const { return TVector4(_x / rhs._x, _y / rhs._y, _z / rhs._z, _w / rhs._w); }

	/// Add-assign a vector.
	TVector4& operator += (const TVector4& rhs)
	{
		_x += rhs._x;
		_y += rhs._y;
		_z += rhs._z;
		_w += rhs._w;
		return *this;
	}

	/// Subtract-assign a vector.
	TVector4& operator -= (const TVector4& rhs)
	{
		_x -= rhs._x;
		_y -= rhs._y;
		_z -= rhs._z;
		_w -= rhs._w;
		return *this;
	}

	/// Multiply-assign a scalar.
	TVector4& operator *= (float rhs)
	{
		_x *= rhs;
		_y *= rhs;
		_z *= rhs;
		_w *= rhs;
		return *this;
	}

	/// Multiply-assign a vector.
	TVector4& operator *= (const TVector4& rhs)
	{
		_x *= rhs._x;
		_y *= rhs._y;
		_z *= rhs._z;
		_w *= rhs._w;
		return *this;
	}

	/// Divide-assign a scalar.
	TVector4& operator /= (float rhs)
	{
		float invRhs = 1.0f / rhs;
		_x *= invRhs;
		_y *= invRhs;
		_z *= invRhs;
		_w *= invRhs;
		return *this;
	}

	/// Divide-assign a vector.
	TVector4& operator /= (const TVector4& rhs)
	{
		_x /= rhs._x;
		_y /= rhs._y;
		_z /= rhs._z;
		_w /= rhs._w;
		return *this;
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
		if (elements < 4)
			return false;

		char* ptr = (char*)str;
		_x = (float)strtod(ptr, &ptr);
		_y = (float)strtod(ptr, &ptr);
		_z = (float)strtod(ptr, &ptr);
		_w = (float)strtod(ptr, &ptr);

		return true;
	}

	/// Calculate dot product.
	float DotProduct(const TVector4& rhs) const { return _x * rhs._x + _y * rhs._y + _z * rhs._z + _w * rhs._w; }
	/// Calculate absolute dot product.
	float AbsDotProduct(const TVector4& rhs) const { return Auto3D::Abs(_x * rhs._x) + Auto3D::Abs(_y * rhs._y) + Auto3D::Abs(_z * rhs._z) + Auto3D::Abs(_w * rhs._w); }
	/// Return absolute vector.
	TVector4 Abs() const { return TVector4(Auto3D::Abs(_x), Auto3D::Abs(_y), Auto3D::Abs(_z), Auto3D::Abs(_w)); }
	/// Linear interpolation with another vector.
	TVector4 Lerp(const TVector4& rhs, float t) const { return *this * (1.0f - t) + rhs * t; }
	/// Test for equality with another vector with epsilon.
	bool Equals(const TVector4& rhs) const { return Auto3D::Equals(_x, rhs._x) && Auto3D::Equals(_y, rhs._y) && Auto3D::Equals(_z, rhs._z) && Auto3D::Equals(_w, rhs._w); }
	/// Return whether is NaN.
	bool IsNaN() const { return Auto3D::IsNaN(_x) || Auto3D::IsNaN(_y) || Auto3D::IsNaN(_z) || Auto3D::IsNaN(_w); }

	/// Return float data.
	const float* Data() const { return &_x; }
	/// Return as string.
	FString ToString() const
	{
		char tempBuffer[CONVERSION_BUFFER_LENGTH];
		sprintf(tempBuffer, "%g %g %g %g", _x, _y, _z, _w);
		return FString(tempBuffer);
	}

	/// Zero vector.
	static const TVector4 ZERO;
	/// (1,1,1) vector.
	static const TVector4 ONE;
};

/// Multiply TVector4 with a scalar.
template <typename _Ty> TVector4<_Ty> operator * (float lhs, const TVector4<_Ty>& rhs) { return rhs * lhs; }

using TVector4F = TVector4<float>;

using TVector4I = TVector4<int>;

using TVector4C = TVector4<char>;

using TVector4D = TVector4<double>;

using TVector4U = TVector4<unsigned>;

}
