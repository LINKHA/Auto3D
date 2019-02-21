#pragma once
#include "MathBase.h"

namespace Auto3D {

/// Two-dimensional vector.
class Vector2
{
public:
	/// Construct a zero vector.
	Vector2() noexcept :
		_x(0.0f),
		_y(0.0f)
	{
	}

	/// Copy-construct from another vector.
	Vector2(const Vector2& vector) noexcept = default;

	/// Construct from coordinates.
	Vector2(float x, float y) noexcept :
		_x(x),
		_y(y)
	{
	}

	/// Construct from a float array.
	explicit Vector2(const float* data) noexcept :
		_x(data[0]),
		_y(data[1])
	{
	}

	/// Assign from another vector.
	Vector2& operator =(const Vector2& rhs) noexcept = default;

	/// Test for equality with another vector without epsilon.
	bool operator ==(const Vector2& rhs) const { return _x == rhs._x && _y == rhs._y; }

	/// Test for inequality with another vector without epsilon.
	bool operator !=(const Vector2& rhs) const { return _x != rhs._x || _y != rhs._y; }

	/// Add a vector.
	Vector2 operator +(const Vector2& rhs) const { return Vector2(_x + rhs._x, _y + rhs._y); }

	/// Return negation.
	Vector2 operator -() const { return Vector2(-_x, -_y); }

	/// Subtract a vector.
	Vector2 operator -(const Vector2& rhs) const { return Vector2(_x - rhs._x, _y - rhs._y); }

	/// Multiply with a scalar.
	Vector2 operator *(float rhs) const { return Vector2(_x * rhs, _y * rhs); }

	/// Multiply with a vector.
	Vector2 operator *(const Vector2& rhs) const { return Vector2(_x * rhs._x, _y * rhs._y); }

	/// Divide by a scalar.
	Vector2 operator /(float rhs) const { return Vector2(_x / rhs, _y / rhs); }

	/// Divide by a vector.
	Vector2 operator /(const Vector2& rhs) const { return Vector2(_x / rhs._x, _y / rhs._y); }

	/// Add-assign a vector.
	Vector2& operator +=(const Vector2& rhs)
	{
		_x += rhs._x;
		_y += rhs._y;
		return *this;
	}

	/// Subtract-assign a vector.
	Vector2& operator -=(const Vector2& rhs)
	{
		_x -= rhs._x;
		_y -= rhs._y;
		return *this;
	}

	/// Multiply-assign a scalar.
	Vector2& operator *=(float rhs)
	{
		_x *= rhs;
		_y *= rhs;
		return *this;
	}

	/// Multiply-assign a vector.
	Vector2& operator *=(const Vector2& rhs)
	{
		_x *= rhs._x;
		_y *= rhs._y;
		return *this;
	}

	/// Divide-assign a scalar.
	Vector2& operator /=(float rhs)
	{
		float invRhs = 1.0f / rhs;
		_x *= invRhs;
		_y *= invRhs;
		return *this;
	}

	/// Divide-assign a vector.
	Vector2& operator /=(const Vector2& rhs)
	{
		_x /= rhs._x;
		_y /= rhs._y;
		return *this;
	}

	/// Normalize to unit length.
	void Normalize()
	{
		float lenSquared = LengthSquared();
		if (!Auto3D::Equals(lenSquared, 1.0f) && lenSquared > 0.0f)
		{
			float invLen = 1.0f / sqrtf(lenSquared);
			_x *= invLen;
			_y *= invLen;
		}
	}

	/// Return length.
	float Length() const { return sqrtf(_x * _x + _y * _y); }

	/// Return squared length.
	float LengthSquared() const { return _x * _x + _y * _y; }

	/// Calculate dot product.
	float DotProduct(const Vector2& rhs) const { return _x * rhs._x + _y * rhs._y; }

	/// Calculate absolute dot product.
	float AbsDotProduct(const Vector2& rhs) const { return Auto3D::Abs(_x * rhs._x) + Auto3D::Abs(_y * rhs._y); }

	/// Project vector onto axis.
	float ProjectOntoAxis(const Vector2& axis) const { return DotProduct(axis.Normalized()); }

	/// Returns the angle between this vector and another vector in degrees.
	float Angle(const Vector2& rhs) const { return Auto3D::Acos(DotProduct(rhs) / (Length() * rhs.Length())); }

	/// Return absolute vector.
	Vector2 Abs() const { return Vector2(Auto3D::Acos(_x), Auto3D::Acos(_y)); }

	/// Linear interpolation with another vector.
	Vector2 Lerp(const Vector2& rhs, float t) const { return *this * (1.0f - t) + rhs * t; }

	/// Test for equality with another vector with epsilon.
	bool Equals(const Vector2& rhs) const { return Auto3D::Equals(_x, rhs._x) && Auto3D::Equals(_y, rhs._y); }

	/// Return whether is NaN.
	bool IsNaN() const { return Auto3D::IsNaN(_x) || Auto3D::IsNaN(_y); }

	/// Return normalized to unit length.
	Vector2 Normalized() const
	{
		float lenSquared = LengthSquared();
		if (!Auto3D::Equals(lenSquared, 1.0f) && lenSquared > 0.0f)
		{
			float invLen = 1.0f / sqrtf(lenSquared);
			return *this * invLen;
		}
		else
			return *this;
	}

	/// Return float data.
	const float* Data() const { return &_x; }

	/// Return as string.
	STRING ToString() const;

	/// X coordinate.
	float _x;
	/// Y coordinate.
	float _y;

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

/// Multiply Vector2 with a scalar
inline Vector2 operator *(float lhs, const Vector2& rhs) { return rhs * lhs; }

/// Per-component linear interpolation between two 2-vectors.
inline Vector2 VectorLerp(const Vector2& lhs, const Vector2& rhs, const Vector2& t) { return lhs + (rhs - lhs) * t; }

/// Per-component min of two 2-vectors.
inline Vector2 VectorMin(const Vector2& lhs, const Vector2& rhs) { return Vector2(Min(lhs._x, rhs._x), Min(lhs._y, rhs._y)); }

/// Per-component max of two 2-vectors.
inline Vector2 VectorMax(const Vector2& lhs, const Vector2& rhs) { return Vector2(Max(lhs._x, rhs._x), Max(lhs._y, rhs._y)); }

/// Per-component floor of 2-vector.
inline Vector2 VectorFloor(const Vector2& vec) { return Vector2(Floor(vec._x), Floor(vec._y)); }

/// Per-component round of 2-vector.
inline Vector2 VectorRound(const Vector2& vec) { return Vector2(Round(vec._x), Round(vec._y)); }

/// Per-component ceil of 2-vector.
inline Vector2 VectorCeil(const Vector2& vec) { return Vector2(Ceil(vec._x), Ceil(vec._y)); }

/// Return a random value from [0, 1) from 2-vector seed.
/// http://stackoverflow.com/questions/12964279/whats-the-origin-of-this-glsl-rand-one-liner
inline float StableRandom(const Vector2& seed) { return Fract(Sin(seed.DotProduct(Vector2(12.9898f, 78.233f)) * M_RADTODEG) * 43758.5453f); }

/// Return a random value from [0, 1) from scalar seed.
inline float StableRandom(float seed) { return StableRandom(Vector2(seed, seed)); }
}

