#pragma once
#include "MathBase.h"
#include "Vector2.h"

namespace Auto3D {

/// Three-dimensional vector.
class Vector3
{
public:
	/// Construct a zero vector.
	Vector3() noexcept :
		x(0.0f),
		y(0.0f),
		z(0.0f)
	{
	}

	/// Copy-construct from another vector.
	Vector3(const Vector3& vector) noexcept = default;

	/// Construct from a two-dimensional vector and the Z coordinate.
	Vector3(const Vector2& vector, float z) noexcept :
		x(vector.x),
		y(vector.y),
		z(z)
	{
	}

	/// Construct from a two-dimensional vector (for Urho2D).
	explicit Vector3(const Vector2& vector) noexcept :
		x(vector.x),
		y(vector.y),
		z(0.0f)
	{
	}


	/// Construct from coordinates.
	Vector3(float x, float y, float z) noexcept :
		x(x),
		y(y),
		z(z)
	{
	}

	/// Construct from two-dimensional coordinates (for Urho2D).
	Vector3(float x, float y) noexcept :
		x(x),
		y(y),
		z(0.0f)
	{
	}

	/// Construct from a float array.
	explicit Vector3(const float* data) noexcept :
		x(data[0]),
		y(data[1]),
		z(data[2])
	{
	}

	/// Assign from another vector.
	Vector3& operator =(const Vector3& rhs) noexcept = default;

	/// Test for equality with another vector without epsilon.
	bool operator ==(const Vector3& rhs) const { return x == rhs.x && y == rhs.y && z == rhs.z; }

	/// Test for inequality with another vector without epsilon.
	bool operator !=(const Vector3& rhs) const { return x != rhs.x || y != rhs.y || z != rhs.z; }

	/// Add a vector.
	Vector3 operator +(const Vector3& rhs) const { return Vector3(x + rhs.x, y + rhs.y, z + rhs.z); }

	/// Return negation.
	Vector3 operator -() const { return Vector3(-x, -y, -z); }

	/// Subtract a vector.
	Vector3 operator -(const Vector3& rhs) const { return Vector3(x - rhs.x, y - rhs.y, z - rhs.z); }

	/// Multiply with a scalar.
	Vector3 operator *(float rhs) const { return Vector3(x * rhs, y * rhs, z * rhs); }

	/// Multiply with a vector.
	Vector3 operator *(const Vector3& rhs) const { return Vector3(x * rhs.x, y * rhs.y, z * rhs.z); }

	/// Divide by a scalar.
	Vector3 operator /(float rhs) const { return Vector3(x / rhs, y / rhs, z / rhs); }

	/// Divide by a vector.
	Vector3 operator /(const Vector3& rhs) const { return Vector3(x / rhs.x, y / rhs.y, z / rhs.z); }

	/// Add-assign a vector.
	Vector3& operator +=(const Vector3& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}

	/// Subtract-assign a vector.
	Vector3& operator -=(const Vector3& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}

	/// Multiply-assign a scalar.
	Vector3& operator *=(float rhs)
	{
		x *= rhs;
		y *= rhs;
		z *= rhs;
		return *this;
	}

	/// Multiply-assign a vector.
	Vector3& operator *=(const Vector3& rhs)
	{
		x *= rhs.x;
		y *= rhs.y;
		z *= rhs.z;
		return *this;
	}

	/// Divide-assign a scalar.
	Vector3& operator /=(float rhs)
	{
		float invRhs = 1.0f / rhs;
		x *= invRhs;
		y *= invRhs;
		z *= invRhs;
		return *this;
	}

	/// Divide-assign a vector.
	Vector3& operator /=(const Vector3& rhs)
	{
		x /= rhs.x;
		y /= rhs.y;
		z /= rhs.z;
		return *this;
	}

	/// Normalize to unit length.
	void Normalize()
	{
		float lenSquared = LengthSquared();
		if (!Auto3D::Equals(lenSquared, 1.0f) && lenSquared > 0.0f)
		{
			float invLen = 1.0f / sqrtf(lenSquared);
			x *= invLen;
			y *= invLen;
			z *= invLen;
		}
	}

	/// Return length.
	float Length() const { return sqrtf(x * x + y * y + z * z); }

	/// Return squared length.
	float LengthSquared() const { return x * x + y * y + z * z; }

	/// Calculate dot product.
	float DotProduct(const Vector3& rhs) const { return x * rhs.x + y * rhs.y + z * rhs.z; }

	/// Calculate absolute dot product.
	float AbsDotProduct(const Vector3& rhs) const
	{
		return Auto3D::Abs(x * rhs.x) + Auto3D::Abs(y * rhs.y) + Auto3D::Abs(z * rhs.z);
	}

	/// Project direction vector onto axis.
	float ProjectOntoAxis(const Vector3& axis) const { return DotProduct(axis.Normalized()); }

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
		const float lengthSquared = direction.LengthSquared();
		float factor = (*this - from).DotProduct(direction) / lengthSquared;

		if (clamped)
			factor = Clamp(factor, 0.0f, 1.0f);

		return from + direction * factor;
	}

	/// Calculate distance to another position vector.
	float DistanceToPoint(const Vector3& point) const { return (*this - point).Length(); }

	/// Calculate distance to the plane with given origin and normal.
	float DistanceToPlane(const Vector3& origin, const Vector3& normal) const { return (*this - origin).ProjectOntoAxis(normal); }

	/// Make vector orthogonal to the axis.
	Vector3 Orthogonalize(const Vector3& axis) const { return axis.CrossProduct(*this).CrossProduct(axis).Normalized(); }

	/// Calculate cross product.
	Vector3 CrossProduct(const Vector3& rhs) const
	{
		return Vector3(
			y * rhs.z - z * rhs.y,
			z * rhs.x - x * rhs.z,
			x * rhs.y - y * rhs.x
		);
	}

	/// Return absolute vector.
	Vector3 Abs() const { return Vector3(Auto3D::Abs(x), Auto3D::Abs(y), Auto3D::Abs(z)); }

	/// Linear interpolation with another vector.
	Vector3 Lerp(const Vector3& rhs, float t) const { return *this * (1.0f - t) + rhs * t; }

	/// Test for equality with another vector with epsilon.
	bool Equals(const Vector3& rhs) const
	{
		return Auto3D::Equals(x, rhs.x) && Auto3D::Equals(y, rhs.y) && Auto3D::Equals(z, rhs.z);
	}

	/// Returns the angle between this vector and another vector in degrees.
	float Angle(const Vector3& rhs) const { return Auto3D::Acos(DotProduct(rhs) / (Length() * rhs.Length())); }

	/// Return whether is NaN.
	bool IsNaN() const { return Auto3D::IsNaN(x) || Auto3D::IsNaN(y) || Auto3D::IsNaN(z); }

	/// Return normalized to unit length.
	Vector3 Normalized() const
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
	const float* Data() const { return &x; }

	/// Return as string.
	STRING ToString() const;

	/// Return hash value for HashSet & HashMap.
	unsigned ToHash() const
	{
		unsigned hash = 37;
		hash = 37 * hash + FloatToRawIntBits(x);
		hash = 37 * hash + FloatToRawIntBits(y);
		hash = 37 * hash + FloatToRawIntBits(z);

		return hash;
	}

	/// X coordinate.
	float x;
	/// Y coordinate.
	float y;
	/// Z coordinate.
	float z;

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
inline Vector3 operator *(float lhs, const Vector3& rhs) { return rhs * lhs; }

/// Per-component linear interpolation between two 3-vectors.
inline Vector3 VectorLerp(const Vector3& lhs, const Vector3& rhs, const Vector3& t) { return lhs + (rhs - lhs) * t; }

/// Per-component min of two 3-vectors.
inline Vector3 VectorMin(const Vector3& lhs, const Vector3& rhs) { return Vector3(Min(lhs.x, rhs.x), Min(lhs.y, rhs.y), Min(lhs.z, rhs.z)); }

/// Per-component max of two 3-vectors.
inline Vector3 VectorMax(const Vector3& lhs, const Vector3& rhs) { return Vector3(Max(lhs.x, rhs.x), Max(lhs.y, rhs.y), Max(lhs.z, rhs.z)); }

/// Per-component floor of 3-vector.
inline Vector3 VectorFloor(const Vector3& vec) { return Vector3(Floor(vec.x), Floor(vec.y), Floor(vec.z)); }

/// Per-component round of 3-vector.
inline Vector3 VectorRound(const Vector3& vec) { return Vector3(Round(vec.x), Round(vec.y), Round(vec.z)); }

/// Per-component ceil of 3-vector.
inline Vector3 VectorCeil(const Vector3& vec) { return Vector3(Ceil(vec.x), Ceil(vec.y), Ceil(vec.z)); }

}

