#pragma once

#include "Vector3.h"

namespace Auto3D
{

/// Four-dimensional vector.
template<typename _Ty> class AUTO_API Vector4
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
	Vector4() noexcept :
		_x(0),
		_y(0),
		_z(0),
		_w(0)
	{
	}

	/// Copy-construct.
	Vector4(const Vector4& vector) :
		_x(vector._x),
		_y(vector._y),
		_z(vector._z),
		_w(vector._w)
	{
	}

	/// Construct from a 3-dimensional vector and the W coordinate.
	Vector4(const Vector3<_Ty>& vector, float w) :
		_x(vector._x),
		_y(vector._y),
		_z(vector._z),
		_w(w)
	{
	}

	/// Construct from coordinates.
	Vector4(float x, float y, float z, float w) :
		_x(x),
		_y(y),
		_z(z),
		_w(w)
	{
	}

	/// Construct from a float array.
	Vector4(const float* _data) :
		_x(_data[0]),
		_y(_data[1]),
		_z(_data[2]),
		_w(_data[3])
	{
	}

	/// Assign from another vector.
	Vector4& operator = (const Vector4& rhs)
	{
		_x = rhs._x;
		_y = rhs._y;
		_z = rhs._z;
		_w = rhs._w;
		return *this;
	}

	/// Construct by parsing a string.
	Vector4(const FString& str)
	{
		FromString(str);
	}

	/// Construct by parsing a C string.
	Vector4(const char* str)
	{
		FromString(str);
	}

	/// Test for equality with another vector without epsilon.
	bool operator == (const Vector4& rhs) const { return _x == rhs._x && _y == rhs._y && _z == rhs._z && _w == rhs._w; }
	/// Test for inequality with another vector without epsilon.
	bool operator != (const Vector4& rhs) const { return !(*this == rhs); }
	/// Add a vector.
	Vector4 operator + (const Vector4& rhs) const { return Vector4(_x + rhs._x, _y + rhs._y, _z + rhs._z, _w + rhs._w); }
	/// Return negation.
	Vector4 operator - () const { return Vector4(-_x, -_y, -_z, -_w); }
	/// Subtract a vector.
	Vector4 operator - (const Vector4& rhs) const { return Vector4(_x - rhs._x, _y - rhs._y, _z - rhs._z, _w - rhs._w); }
	/// Multiply with a scalar.
	Vector4 operator * (float rhs) const { return Vector4(_x * rhs, _y * rhs, _z * rhs, _w * rhs); }
	/// Multiply with a vector.
	Vector4 operator * (const Vector4& rhs) const { return Vector4(_x * rhs._x, _y * rhs._y, _z * rhs._z, _w * rhs._w); }
	/// Divide by a scalar.
	Vector4 operator / (float rhs) const { return Vector4(_x / rhs, _y / rhs, _z / rhs, _w / rhs); }
	/// Divide by a vector.
	Vector4 operator / (const Vector4& rhs) const { return Vector4(_x / rhs._x, _y / rhs._y, _z / rhs._z, _w / rhs._w); }

	/// Add-assign a vector.
	Vector4& operator += (const Vector4& rhs)
	{
		_x += rhs._x;
		_y += rhs._y;
		_z += rhs._z;
		_w += rhs._w;
		return *this;
	}

	/// Subtract-assign a vector.
	Vector4& operator -= (const Vector4& rhs)
	{
		_x -= rhs._x;
		_y -= rhs._y;
		_z -= rhs._z;
		_w -= rhs._w;
		return *this;
	}

	/// Multiply-assign a scalar.
	Vector4& operator *= (float rhs)
	{
		_x *= rhs;
		_y *= rhs;
		_z *= rhs;
		_w *= rhs;
		return *this;
	}

	/// Multiply-assign a vector.
	Vector4& operator *= (const Vector4& rhs)
	{
		_x *= rhs._x;
		_y *= rhs._y;
		_z *= rhs._z;
		_w *= rhs._w;
		return *this;
	}

	/// Divide-assign a scalar.
	Vector4& operator /= (float rhs)
	{
		float invRhs = 1.0f / rhs;
		_x *= invRhs;
		_y *= invRhs;
		_z *= invRhs;
		_w *= invRhs;
		return *this;
	}

	/// Divide-assign a vector.
	Vector4& operator /= (const Vector4& rhs)
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
	float DotProduct(const Vector4& rhs) const { return _x * rhs._x + _y * rhs._y + _z * rhs._z + _w * rhs._w; }
	/// Calculate absolute dot product.
	float AbsDotProduct(const Vector4& rhs) const { return Auto3D::Abs(_x * rhs._x) + Auto3D::Abs(_y * rhs._y) + Auto3D::Abs(_z * rhs._z) + Auto3D::Abs(_w * rhs._w); }
	/// Return absolute vector.
	Vector4 Abs() const { return Vector4(Auto3D::Abs(_x), Auto3D::Abs(_y), Auto3D::Abs(_z), Auto3D::Abs(_w)); }
	/// Linear interpolation with another vector.
	Vector4 Lerp(const Vector4& rhs, float t) const { return *this * (1.0f - t) + rhs * t; }
	/// Test for equality with another vector with epsilon.
	bool Equals(const Vector4& rhs) const { return Auto3D::Equals(_x, rhs._x) && Auto3D::Equals(_y, rhs._y) && Auto3D::Equals(_z, rhs._z) && Auto3D::Equals(_w, rhs._w); }
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
	static const Vector4 ZERO;
	/// (1,1,1) vector.
	static const Vector4 ONE;
};

/// Multiply Vector4 with a scalar.
template <typename _Ty> Vector4<_Ty> operator * (float lhs, const Vector4<_Ty>& rhs) { return rhs * lhs; }

using Vector4F = Vector4<float>;

using Vector4I = Vector4<int>;

using Vector4C = Vector4<char>;

using Vector4D = Vector4<double>;

using Vector4U = Vector4<unsigned>;

}
