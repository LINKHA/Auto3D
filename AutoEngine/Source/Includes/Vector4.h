#pragma once
#include "MathBase.h"
#include "Vector3.h"

namespace Auto3D {

/// Four-dimensional vector.
class Vector4
{
public:
	/// Construct a zero vector.
	Vector4() noexcept :
		_x(0.0f),
		_y(0.0f),
		_z(0.0f),
		_w(0.0f)
	{
	}

	/// Copy-construct from another vector.
	Vector4(const Vector4& vector) noexcept = default;

	/// Construct from a 3-dimensional vector and the W coordinate.
	Vector4(const Vector3& vector, float w) noexcept :
		_x(vector._x),
		_y(vector._y),
		_z(vector._z),
		_w(w)
	{
	}

	/// Construct from coordinates.
	Vector4(float x, float y, float z, float w) noexcept :
		_x(x),
		_y(y),
		_z(z),
		_w(w)
	{
	}

	/// Construct from a float array.
	explicit Vector4(const float* data) noexcept :
		_x(data[0]),
		_y(data[1]),
		_z(data[2]),
		_w(data[3])
	{
	}

	/// Assign from another vector.
	Vector4& operator =(const Vector4& rhs) noexcept = default;

	/// Test for equality with another vector without epsilon.
	bool operator ==(const Vector4& rhs) const { return _x == rhs._x && _y == rhs._y && _z == rhs._z && _w == rhs._w; }

	/// Test for inequality with another vector without epsilon.
	bool operator !=(const Vector4& rhs) const { return _x != rhs._x || _y != rhs._y || _z != rhs._z || _w != rhs._w; }

	/// Add a vector.
	Vector4 operator +(const Vector4& rhs) const { return Vector4(_x + rhs._x, _y + rhs._y, _z + rhs._z, _w + rhs._w); }

	/// Return negation.
	Vector4 operator -() const { return Vector4(-_x, -_y, -_z, -_w); }

	/// Subtract a vector.
	Vector4 operator -(const Vector4& rhs) const { return Vector4(_x - rhs._x, _y - rhs._y, _z - rhs._z, _w - rhs._w); }

	/// Multiply with a scalar.
	Vector4 operator *(float rhs) const { return Vector4(_x * rhs, _y * rhs, _z * rhs, _w * rhs); }

	/// Multiply with a vector.
	Vector4 operator *(const Vector4& rhs) const { return Vector4(_x * rhs._x, _y * rhs._y, _z * rhs._z, _w * rhs._w); }

	/// Divide by a scalar.
	Vector4 operator /(float rhs) const { return Vector4(_x / rhs, _y / rhs, _z / rhs, _w / rhs); }

	/// Divide by a vector.
	Vector4 operator /(const Vector4& rhs) const { return Vector4(_x / rhs._x, _y / rhs._y, _z / rhs._z, _w / rhs._w); }

	/// Add-assign a vector.
	Vector4& operator +=(const Vector4& rhs)
	{
		_x += rhs._x;
		_y += rhs._y;
		_z += rhs._z;
		_w += rhs._w;
		return *this;
	}

	/// Subtract-assign a vector.
	Vector4& operator -=(const Vector4& rhs)
	{
		_x -= rhs._x;
		_y -= rhs._y;
		_z -= rhs._z;
		_w -= rhs._w;
		return *this;
	}

	/// Multiply-assign a scalar.
	Vector4& operator *=(float rhs)
	{
		_x *= rhs;
		_y *= rhs;
		_z *= rhs;
		_w *= rhs;
		return *this;
	}

	/// Multiply-assign a vector.
	Vector4& operator *=(const Vector4& rhs)
	{
		_x *= rhs._x;
		_y *= rhs._y;
		_z *= rhs._z;
		_w *= rhs._w;
		return *this;
	}

	/// Divide-assign a scalar.
	Vector4& operator /=(float rhs)
	{
		float invRhs = 1.0f / rhs;
		_x *= invRhs;
		_y *= invRhs;
		_z *= invRhs;
		_w *= invRhs;
		return *this;
	}

	/// Divide-assign a vector.
	Vector4& operator /=(const Vector4& rhs)
	{
		_x /= rhs._x;
		_y /= rhs._y;
		_z /= rhs._z;
		_w /= rhs._w;
		return *this;
	}

	/// Return const value by index.
	float operator[](unsigned index) const { return (&_x)[index]; }

	/// Return mutable value by index.
	float& operator[](unsigned index) { return (&_x)[index]; }

	/// Calculate dot product.
	float DotProduct(const Vector4& rhs) const { return _x * rhs._x + _y * rhs._y + _z * rhs._z + _w * rhs._w; }

	/// Calculate absolute dot product.
	float AbsDotProduct(const Vector4& rhs) const
	{
		return Auto3D::Abs(_x * rhs._x) + Auto3D::Abs(_y * rhs._y) + Auto3D::Abs(_z * rhs._z) + Auto3D::Abs(_w * rhs._w);
	}

	/// Project vector onto axis.
	float ProjectOntoAxis(const Vector3& axis) const { return DotProduct(Vector4(axis.Normalized(), 0.0f)); }

	/// Return absolute vector.
	Vector4 Abs() const { return Vector4(Auto3D::Abs(_x), Auto3D::Abs(_y), Auto3D::Abs(_z), Auto3D::Abs(_w)); }

	/// Linear interpolation with another vector.
	Vector4 Lerp(const Vector4& rhs, float t) const { return *this * (1.0f - t) + rhs * t; }

	/// Test for equality with another vector with epsilon.
	bool Equals(const Vector4& rhs) const
	{
		return Auto3D::Equals(_x, rhs._x) && Auto3D::Equals(_y, rhs._y) && Auto3D::Equals(_z, rhs._z) && Auto3D::Equals(_w, rhs._w);
	}

	/// Return whether is NaN.
	bool IsNaN() const { return Auto3D::IsNaN(_x) || Auto3D::IsNaN(_y) || Auto3D::IsNaN(_z) || Auto3D::IsNaN(_w); }

	/// Return float data.
	const float* Data() const { return &_x; }

	/// Return as string.
	STRING ToString() const;

	/// Return hash value for HashSet & HashMap.
	unsigned ToHash() const
	{
		unsigned hash = 37;
		hash = 37 * hash + FloatToRawIntBits(_x);
		hash = 37 * hash + FloatToRawIntBits(_y);
		hash = 37 * hash + FloatToRawIntBits(_z);
		hash = 37 * hash + FloatToRawIntBits(_w);

		return hash;
	}

	/// X coordinate.
	float _x;
	/// Y coordinate.
	float _y;
	/// Z coordinate.
	float _z;
	/// W coordinate.
	float _w;

	/// Zero vector.
	static const Vector4 ZERO;
	/// (1,1,1) vector.
	static const Vector4 ONE;
};

/// Multiply Vector4 with a scalar.
inline Vector4 operator *(float lhs, const Vector4& rhs) { return rhs * lhs; }

/// Per-component linear interpolation between two 4-vectors.
inline Vector4 VectorLerp(const Vector4& lhs, const Vector4& rhs, const Vector4& t) { return lhs + (rhs - lhs) * t; }

/// Per-component min of two 4-vectors.
inline Vector4 VectorMin(const Vector4& lhs, const Vector4& rhs) { return Vector4(Min(lhs._x, rhs._x), Min(lhs._y, rhs._y), Min(lhs._z, rhs._z), Min(lhs._w, rhs._w)); }

/// Per-component max of two 4-vectors.
inline Vector4 VectorMax(const Vector4& lhs, const Vector4& rhs) { return Vector4(Max(lhs._x, rhs._x), Max(lhs._y, rhs._y), Max(lhs._z, rhs._z), Max(lhs._w, rhs._w)); }

/// Per-component floor of 4-vector.
inline Vector4 VectorFloor(const Vector4& vec) { return Vector4(Floor(vec._x), Floor(vec._y), Floor(vec._z), Floor(vec._w)); }

/// Per-component round of 4-vector.
inline Vector4 VectorRound(const Vector4& vec) { return Vector4(Round(vec._x), Round(vec._y), Round(vec._z), Round(vec._w)); }

/// Per-component ceil of 4-vector.
inline Vector4 VectorCeil(const Vector4& vec) { return Vector4(Ceil(vec._x), Ceil(vec._y), Ceil(vec._z), Ceil(vec._w)); }

}
