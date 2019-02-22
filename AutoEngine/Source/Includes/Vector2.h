#pragma once
#include "MathBase.h"

namespace Auto3D {

/// Two-dimensional vector.
template <typename _Ty> class Vector2
{
public:
	/// Construct a zero vector.
	Vector2() noexcept :
		_x(0),
		_y(0)
	{
	}

	/// Copy-construct from another vector.
	Vector2(const Vector2& vector) noexcept = default;

	/// Construct from coordinates.
	Vector2(_Ty x, _Ty y) noexcept :
		_x(x),
		_y(y)
	{
	}

	/// Construct from a _Ty array.
	explicit Vector2(const _Ty* data) noexcept :
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
	Vector2 operator *(_Ty rhs) const { return Vector2(_x * rhs, _y * rhs); }

	/// Multiply with a vector.
	Vector2 operator *(const Vector2& rhs) const { return Vector2(_x * rhs._x, _y * rhs._y); }

	/// Divide by a scalar.
	Vector2 operator /(_Ty rhs) const { return Vector2(_x / rhs, _y / rhs); }

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
	Vector2& operator *=(_Ty rhs)
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
	Vector2& operator /=(_Ty rhs)
	{
		_Ty invRhs = 1.0f / rhs;
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
		_Ty lenSquared = LengthSquared();
		if (!Auto3D::Equals(lenSquared, 1.0f) && lenSquared > 0.0f)
		{
			_Ty invLen = 1.0f / sqrtf(lenSquared);
			_x *= invLen;
			_y *= invLen;
		}
	}

	/// Return length.
	_Ty Length() const { return sqrtf(_x * _x + _y * _y); }

	/// Return squared length.
	_Ty LengthSquared() const { return _x * _x + _y * _y; }

	/// Calculate dot product.
	_Ty DotProduct(const Vector2& rhs) const { return _x * rhs._x + _y * rhs._y; }

	/// Calculate absolute dot product.
	_Ty AbsDotProduct(const Vector2& rhs) const { return Auto3D::Abs(_x * rhs._x) + Auto3D::Abs(_y * rhs._y); }

	/// Project vector onto axis.
	_Ty ProjectOntoAxis(const Vector2& axis) const { return DotProduct(axis.Normalized()); }

	/// Returns the angle between this vector and another vector in degrees.
	_Ty Angle(const Vector2& rhs) const { return Auto3D::Acos(DotProduct(rhs) / (Length() * rhs.Length())); }

	/// Return absolute vector.
	Vector2 Abs() const { return Vector2(Auto3D::Acos(_x), Auto3D::Acos(_y)); }

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
	STRING ToString() const;

	/// X coordinate.
	_Ty _x;
	/// Y coordinate.
	_Ty _y;

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
template <typename _Ty> Vector2<_Ty> operator *(_Ty lhs, const Vector2<_Ty>& rhs) { return rhs * lhs; }

/// Per-component linear interpolation between two 2-vectors.
template <typename _Ty> Vector2<_Ty> VectorLerp(const Vector2<_Ty>& lhs, const Vector2<_Ty>& rhs, const Vector2<_Ty>& t) { return lhs + (rhs - lhs) * t; }

/// Per-component min of two 2-vectors.
template <typename _Ty> Vector2<_Ty> VectorMin(const Vector2<_Ty>& lhs, const Vector2<_Ty>& rhs) { return Vector2(Min(lhs._x, rhs._x), Min(lhs._y, rhs._y)); }

/// Per-component max of two 2-vectors.
template <typename _Ty> Vector2<_Ty> VectorMax(const Vector2<_Ty>& lhs, const Vector2<_Ty>& rhs) { return Vector2(Max(lhs._x, rhs._x), Max(lhs._y, rhs._y)); }

/// Per-component floor of 2-vector.
template <typename _Ty> Vector2<_Ty> VectorFloor(const Vector2<_Ty>& vec) { return Vector2<_Ty>(Floor(vec._x), Floor(vec._y)); }

/// Per-component round of 2-vector.
template <typename _Ty> Vector2<_Ty> VectorRound(const Vector2<_Ty>& vec) { return Vector2<_Ty>(Round(vec._x), Round(vec._y)); }

/// Per-component ceil of 2-vector.
template <typename _Ty> Vector2<_Ty> VectorCeil(const Vector2<_Ty>& vec) { return Vector2<_Ty>(Ceil(vec._x), Ceil(vec._y)); }

/// Return a random value from [0, 1) from 2-vector seed.
/// http://stackoverflow.com/questions/12964279/whats-the-origin-of-this-glsl-rand-one-liner
template <typename _Ty> _Ty StableRandom(const Vector2<_Ty>& seed) { return Fract(Sin(seed.DotProduct(Vector2<_Ty>(12.9898f, 78.233f)) * M_RADTODEG) * 43758.5453f); }

/// Return a random value from [0, 1) from scalar seed.
template <typename _Ty> _Ty StableRandom(_Ty seed) { return StableRandom(Vector2<_Ty>(seed, seed)); }

using Vector2F = Vector2<float>;

using Vector2I = Vector2<int>;

using Vector2C = Vector2<char>;

using Vector2D = Vector2<double>;

using Vector2U = Vector2<unsigned>;

}

