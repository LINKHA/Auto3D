#pragma once
#include "MathBase.h"
#include "Vector2.h"

namespace Auto3D {

/// Three-dimensional vector.
template <typename _Ty> class Vector3
{
public:
	/// Construct a zero vector.
	Vector3() noexcept :
		_x(0),
		_y(0),
		_z(0)
	{
	}

	/// Copy-construct from another vector.
	Vector3(const Vector3& vector) noexcept = default;

	/// Construct from a two-dimensional vector and the Z coordinate.
	Vector3(const Vector2<_Ty>& vector, _Ty z) noexcept :
		_x(vector._x),
		_y(vector._y),
		_z(z)
	{
	}

	/// Construct from a two-dimensional vector (for Urho2D).
	explicit Vector3(const Vector2<_Ty>& vector) noexcept :
		_x(vector._x),
		_y(vector._y),
		_z(0.0f)
	{
	}


	/// Construct from coordinates.
	Vector3(_Ty x, _Ty y, _Ty z) noexcept :
		_x(x),
		_y(y),
		_z(z)
	{
	}

	/// Construct from two-dimensional coordinates (for Urho2D).
	Vector3(_Ty x, _Ty y) noexcept :
		_x(x),
		_y(y),
		_z(0.0f)
	{
	}

	/// Construct from a _Ty array.
	explicit Vector3(const _Ty* data) noexcept :
		_x(data[0]),
		_y(data[1]),
		_z(data[2])
	{
	}

	/// Assign from another vector.
	Vector3& operator =(const Vector3& rhs) noexcept = default;

	/// Test for equality with another vector without epsilon.
	bool operator ==(const Vector3& rhs) const { return _x == rhs._x && _y == rhs._y && _z == rhs._z; }

	/// Test for inequality with another vector without epsilon.
	bool operator !=(const Vector3& rhs) const { return _x != rhs._x || _y != rhs._y || _z != rhs._z; }

	/// Add a vector.
	Vector3 operator +(const Vector3& rhs) const { return Vector3(_x + rhs._x, _y + rhs._y, _z + rhs._z); }

	/// Return negation.
	Vector3 operator -() const { return Vector3(-_x, -_y, -_z); }

	/// Subtract a vector.
	Vector3 operator -(const Vector3& rhs) const { return Vector3(_x - rhs._x, _y - rhs._y, _z - rhs._z); }

	/// Multiply with a scalar.
	Vector3 operator *(_Ty rhs) const { return Vector3(_x * rhs, _y * rhs, _z * rhs); }

	/// Multiply with a vector.
	Vector3 operator *(const Vector3& rhs) const { return Vector3(_x * rhs._x, _y * rhs._y, _z * rhs._z); }

	/// Divide by a scalar.
	Vector3 operator /(_Ty rhs) const { return Vector3(_x / rhs, _y / rhs, _z / rhs); }

	/// Divide by a vector.
	Vector3 operator /(const Vector3& rhs) const { return Vector3(_x / rhs._x, _y / rhs._y, _z / rhs._z); }

	/// Add-assign a vector.
	Vector3& operator +=(const Vector3& rhs)
	{
		_x += rhs._x;
		_y += rhs._y;
		_z += rhs._z;
		return *this;
	}

	/// Subtract-assign a vector.
	Vector3& operator -=(const Vector3& rhs)
	{
		_x -= rhs._x;
		_y -= rhs._y;
		_z -= rhs._z;
		return *this;
	}

	/// Multiply-assign a scalar.
	Vector3& operator *=(_Ty rhs)
	{
		_x *= rhs;
		_y *= rhs;
		_z *= rhs;
		return *this;
	}

	/// Multiply-assign a vector.
	Vector3& operator *=(const Vector3& rhs)
	{
		_x *= rhs._x;
		_y *= rhs._y;
		_z *= rhs._z;
		return *this;
	}

	/// Divide-assign a scalar.
	Vector3& operator /=(_Ty rhs)
	{
		_Ty invRhs = 1.0f / rhs;
		_x *= invRhs;
		_y *= invRhs;
		_z *= invRhs;
		return *this;
	}

	/// Divide-assign a vector.
	Vector3& operator /=(const Vector3& rhs)
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

	/// Return length.
	_Ty Length() const { return sqrtf(_x * _x + _y * _y + _z * _z); }

	/// Return squared length.
	_Ty LengthSquared() const { return _x * _x + _y * _y + _z * _z; }

	/// Calculate dot product.
	_Ty DotProduct(const Vector3& rhs) const { return _x * rhs._x + _y * rhs._y + _z * rhs._z; }

	/// Calculate absolute dot product.
	_Ty AbsDotProduct(const Vector3& rhs) const
	{
		return Auto3D::Abs(_x * rhs._x) + Auto3D::Abs(_y * rhs._y) + Auto3D::Abs(_z * rhs._z);
	}

	/// Project direction vector onto axis.
	_Ty ProjectOntoAxis(const Vector3& axis) const { return DotProduct(axis.Normalized()); }

	/// Project position vector onto plane with given origin and normal.
	Vector3 ProjectOntoPlane(const Vector3& origin, const Vector3& normal) const
	{
		const Vector3 delta = *this - origin;
		return *this - normal.Normalized() * delta.ProjectOntoAxis(normal);
	}

	/// Project position vector onto line segment.
	Vector3 ProjectOntoLine(const Vector3& from, const Vector3& to, bool clamped = false) const
	{
		const Vector3 direction = to - from;
		const _Ty lengthSquared = direction.LengthSquared();
		_Ty factor = (*this - from).DotProduct(direction) / lengthSquared;

		if (clamped)
			factor = Clamp(factor, 0.0f, 1.0f);

		return from + direction * factor;
	}

	/// Calculate distance to another position vector.
	_Ty DistanceToPoint(const Vector3& point) const { return (*this - point).Length(); }

	/// Calculate distance to the plane with given origin and normal.
	_Ty DistanceToPlane(const Vector3& origin, const Vector3& normal) const { return (*this - origin).ProjectOntoAxis(normal); }

	/// Make vector orthogonal to the axis.
	Vector3 Orthogonalize(const Vector3& axis) const { return axis.CrossProduct(*this).CrossProduct(axis).Normalized(); }

	/// Calculate cross product.
	Vector3 CrossProduct(const Vector3& rhs) const
	{
		return Vector3(
			_y * rhs._z - _z * rhs._y,
			_z * rhs._x - _x * rhs._z,
			_x * rhs._y - _y * rhs._x
		);
	}

	/// Return absolute vector.
	Vector3 Abs() const { return Vector3(Auto3D::Abs(_x), Auto3D::Abs(_y), Auto3D::Abs(_z)); }

	/// Linear interpolation with another vector.
	Vector3 Lerp(const Vector3& rhs, _Ty t) const { return *this * (1.0f - t) + rhs * t; }

	/// Test for equality with another vector with epsilon.
	bool Equals(const Vector3& rhs) const
	{
		return Auto3D::Equals(_x, rhs._x) && Auto3D::Equals(_y, rhs._y) && Auto3D::Equals(_z, rhs._z);
	}

	/// Returns the angle between this vector and another vector in degrees.
	_Ty Angle(const Vector3& rhs) const { return Auto3D::Acos(DotProduct(rhs) / (Length() * rhs.Length())); }

	/// Return whether is NaN.
	bool IsNaN() const { return Auto3D::IsNaN(_x) || Auto3D::IsNaN(_y) || Auto3D::IsNaN(_z); }

	/// Return normalized to unit length.
	Vector3 Normalized() const
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

	/// Return hash value for HashSet & HashMap.
	unsigned ToHash() const
	{
		unsigned hash = 37;
		hash = 37 * hash + FloatToRawIntBits(_x);
		hash = 37 * hash + FloatToRawIntBits(_y);
		hash = 37 * hash + FloatToRawIntBits(_z);

		return hash;
	}

	/// X coordinate.
	_Ty _x;
	/// Y coordinate.
	_Ty _y;
	/// Z coordinate.
	_Ty _z;

	/// Zero vector.
	static const Vector3 ZERO;
	/// (-1,0,0) vector.
	static const Vector3 LEFT;
	/// (1,0,0) vector.
	static const Vector3 RIGHT;
	/// (0,1,0) vector.
	static const Vector3 UP;
	/// (0,-1,0) vector.
	static const Vector3 DOWN;
	/// (0,0,1) vector.
	static const Vector3 FORWARD;
	/// (0,0,-1) vector.
	static const Vector3 BACK;
	/// (1,1,1) vector.
	static const Vector3 ONE;
};

/// Multiply Vector3 with a scalar.
template <typename _Ty> Vector3<_Ty> operator *(_Ty lhs, const Vector3<_Ty>& rhs) { return rhs * lhs; }

/// Per-component linear interpolation between two 3-vectors.
template <typename _Ty> Vector3<_Ty> VectorLerp(const Vector3<_Ty>& lhs, const Vector3<_Ty>& rhs, const Vector3<_Ty>& t) { return lhs + (rhs - lhs) * t; }

/// Per-component min of two 3-vectors.
template <typename _Ty> Vector3<_Ty> VectorMin(const Vector3<_Ty>& lhs, const Vector3<_Ty>& rhs) { return Vector3<_Ty>(Min(lhs._x, rhs._x), Min(lhs._y, rhs._y), Min(lhs._z, rhs._z)); }

/// Per-component max of two 3-vectors.
template <typename _Ty> Vector3<_Ty> VectorMax(const Vector3<_Ty>& lhs, const Vector3<_Ty>& rhs) { return Vector3<_Ty>(Max(lhs._x, rhs._x), Max(lhs._y, rhs._y), Max(lhs._z, rhs._z)); }

/// Per-component floor of 3-vector.
template <typename _Ty> Vector3<_Ty> VectorFloor(const Vector3<_Ty>& vec) { return Vector3<_Ty>(Floor(vec._x), Floor(vec._y), Floor(vec._z)); }

/// Per-component round of 3-vector.
template <typename _Ty> Vector3<_Ty> VectorRound(const Vector3<_Ty>& vec) { return Vector3<_Ty>(Round(vec._x), Round(vec._y), Round(vec._z)); }

/// Per-component ceil of 3-vector.
template <typename _Ty> Vector3<_Ty> VectorCeil(const Vector3<_Ty>& vec) { return Vector3<_Ty>(Ceil(vec._x), Ceil(vec._y), Ceil(vec._z)); }

using Vector3F = Vector3<float>;

using Vector3I = Vector3<int>;

using Vector3C = Vector3<char>;

using Vector3D = Vector3<double>;

using Vector3U = Vector3<unsigned>;

}

