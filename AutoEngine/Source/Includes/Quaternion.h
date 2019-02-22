#pragma once
#include "MathBase.h"
#include "Matrix3x3.h"

namespace Auto3D {


/// Rotation represented as a four-dimensional normalized vector.
template<typename _Ty> class Quaternion
{
public:
	/// Construct an identity quaternion.
	Quaternion() noexcept
		:_w(1.0f),
		_x(0.0f),
		_y(0.0f),
		_z(0.0f)
	{
	}

	/// Copy-construct from another quaternion.
	Quaternion(const Quaternion& quat) noexcept
		:_w(quat._w),
		_x(quat._x),
		_y(quat._y),
		_z(quat._z)
	{
	}

	/// Construct from values.
	Quaternion(_Ty w, _Ty x, _Ty y, _Ty z) noexcept
		:_w(w),
		_x(x),
		_y(y),
		_z(z)
	{
	}

	/// Construct from a _Ty array.
	explicit Quaternion(const _Ty* data) noexcept
		:_w(data[0]),
		_x(data[1]),
		_y(data[2]),
		_z(data[3])
	{
	}

	/// Construct from an angle (in degrees) and axis.
	Quaternion(_Ty angle, const Vector3<_Ty>& axis) noexcept
	{
		FromAngleAxis(angle, axis);
	}

	/// Construct from an angle (in degrees, for Urho2D).
	explicit Quaternion(_Ty angle) noexcept
	{
		FromAngleAxis(angle, Vector3<_Ty>::FORWARD);
	}

	/// Construct from Euler angles (in degrees.)
	Quaternion(_Ty x, _Ty y, _Ty z) noexcept
	{
		FromEulerAngles(x, y, z);
	}

	/// Construct from the rotation difference between two direction vectors.
	Quaternion(const Vector3<_Ty>& start, const Vector3<_Ty>& end) noexcept
	{
		FromRotationTo(start, end);
	}

	/// Construct from orthonormal axes.
	Quaternion(const Vector3<_Ty>& xAxis, const Vector3<_Ty>& yAxis, const Vector3<_Ty>& zAxis) noexcept
	{
		FromAxes(xAxis, yAxis, zAxis);
	}

	/// Construct from a rotation matrix.
	explicit Quaternion(const Matrix3x3<_Ty>& matrix) noexcept
	{
		FromRotationMatrix(matrix);
	}


	/// Assign from another quaternion.
	Quaternion& operator =(const Quaternion& rhs) noexcept
	{
		_w = rhs._w;
		_x = rhs._x;
		_y = rhs._y;
		_z = rhs._z;
		return *this;
	}

	/// Add-assign a quaternion.
	Quaternion& operator +=(const Quaternion& rhs)
	{
		_w += rhs._w;
		_x += rhs._x;
		_y += rhs._y;
		_z += rhs._z;
		return *this;
	}

	/// Multiply-assign a scalar.
	Quaternion& operator *=(_Ty rhs)
	{
		_w *= rhs;
		_x *= rhs;
		_y *= rhs;
		_z *= rhs;
		return *this;
	}

	/// Test for equality with another quaternion without epsilon.
	bool operator ==(const Quaternion& rhs) const
	{
		return _w == rhs._w && _x == rhs._x && _y == rhs._y && _z == rhs._z;
	}

	/// Test for inequality with another quaternion without epsilon.
	bool operator !=(const Quaternion& rhs) const { return !(*this == rhs); }

	/// Multiply with a scalar.
	Quaternion operator *(_Ty rhs) const
	{
		return Quaternion(_w * rhs, _x * rhs, _y * rhs, _z * rhs);
	}

	/// Return negation.
	Quaternion operator -() const
	{
		return Quaternion(-_w, -_x, -_y, -_z);
	}

	/// Add a quaternion.
	Quaternion operator +(const Quaternion& rhs) const
	{
		return Quaternion(_w + rhs._w, _x + rhs._x, _y + rhs._y, _z + rhs._z);
	}

	/// Subtract a quaternion.
	Quaternion operator -(const Quaternion& rhs) const
	{
		return Quaternion(_w - rhs._w, _x - rhs._x, _y - rhs._y, _z - rhs._z);
	}

	/// Multiply a quaternion.
	Quaternion operator *(const Quaternion& rhs) const
	{
		return Quaternion(
			_w * rhs._w - _x * rhs._x - _y * rhs._y - _z * rhs._z,
			_w * rhs._x + _x * rhs._w + _y * rhs._z - _z * rhs._y,
			_w * rhs._y + _y * rhs._w + _z * rhs._x - _x * rhs._z,
			_w * rhs._z + _z * rhs._w + _x * rhs._y - _y * rhs._x
		);
	}

	/// Multiply a Vector3<_Ty>.
	Vector3<_Ty> operator *(const Vector3<_Ty>& rhs) const
	{
		Vector3<_Ty> qVec(_x, _y, _z);
		Vector3<_Ty> cross1(qVec.CrossProduct(rhs));
		Vector3<_Ty> cross2(qVec.CrossProduct(cross1));

		return rhs + 2.0f * (cross1 * _w + cross2);
	}

	/// Define from an angle (in degrees) and axis.
	void FromAngleAxis(_Ty angle, const Vector3<_Ty>& axis);
	/// Define from Euler angles (in degrees.)
	void FromEulerAngles(_Ty x, _Ty y, _Ty z);
	/// Define from the rotation difference between two direction vectors.
	void FromRotationTo(const Vector3<_Ty>& start, const Vector3<_Ty>& end);
	/// Define from orthonormal axes.
	void FromAxes(const Vector3<_Ty>& xAxis, const Vector3<_Ty>& yAxis, const Vector3<_Ty>& zAxis);
	/// Define from a rotation matrix.
	void FromRotationMatrix(const Matrix3x3<_Ty>& matrix);
	/// Define from a direction to look in and an up direction. Return true if successful, or false if would result in a NaN, in which case the current value remains.
	bool FromLookRotation(const Vector3<_Ty>& direction, const Vector3<_Ty>& up = Vector3<_Ty>::UP);

	/// Normalize to unit length.
	void Normalize()
	{
		_Ty lenSquared = LengthSquared();
		if (!Auto3D::Equals(lenSquared, 1.0f) && lenSquared > 0.0f)
		{
			_Ty invLen = 1.0f / sqrtf(lenSquared);
			_w *= invLen;
			_x *= invLen;
			_y *= invLen;
			_z *= invLen;
		}
	}

	/// Return normalized to unit length.
	Quaternion Normalized() const
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

	/// Return inverse.
	Quaternion Inverse() const
	{
		_Ty lenSquared = LengthSquared();
		if (lenSquared == 1.0f)
			return Conjugate();
		else if (lenSquared >= M_EPSILON)
			return Conjugate() * (1.0f / lenSquared);
		else
			return IDENTITY;
	}

	/// Return squared length.
	_Ty LengthSquared() const
	{
		return _w * _w + _x * _x + _y * _y + _z * _z;
	}

	/// Calculate dot product.
	_Ty DotProduct(const Quaternion& rhs) const
	{
		return _w * rhs._w + _x * rhs._x + _y * rhs._y + _z * rhs._z;
	}

	/// Test for equality with another quaternion with epsilon.
	bool Equals(const Quaternion& rhs) const
	{
		return Auto3D::Equals(_w, rhs._w) && Auto3D::Equals(_x, rhs._x) && Auto3D::Equals(_y, rhs._y) && Auto3D::Equals(_z, rhs._z);
	}

	/// Return whether is NaN.
	bool IsNaN() const { return Auto3D::IsNaN(_w) || Auto3D::IsNaN(_x) || Auto3D::IsNaN(_y) || Auto3D::IsNaN(_z); }

	/// Return conjugate.
	Quaternion Conjugate() const
	{
		return Quaternion(_w, -_x, -_y, -_z);
	}

	/// Return Euler angles in degrees.
	Vector3<_Ty> EulerAngles() const;
	/// Return yaw angle in degrees.
	_Ty YawAngle() const;
	/// Return pitch angle in degrees.
	_Ty PitchAngle() const;
	/// Return roll angle in degrees.
	_Ty RollAngle() const;
	/// Return rotation axis.
	Vector3<_Ty> Axis() const;
	/// Return rotation angle.
	_Ty Angle() const;
	/// Return the rotation matrix that corresponds to this quaternion.
	Matrix3x3<_Ty> RotationMatrix() const;
	/// Spherical interpolation with another quaternion.
	Quaternion Slerp(const Quaternion& rhs, _Ty t) const;
	/// Normalized linear interpolation with another quaternion.
	Quaternion Nlerp(const Quaternion& rhs, _Ty t, bool shortestPath = false) const;

	/// Return _Ty data.
	const _Ty* Data() const { return &_w; }

	/// Return as string.
	STRING ToString() const;

	/// W coordinate.
	_Ty _w;
	/// X coordinate.
	_Ty _x;
	/// Y coordinate.
	_Ty _y;
	/// Z coordinate.
	_Ty _z;

	/// Identity quaternion.
	static const Quaternion IDENTITY;
};

using QuaternionF= Quaternion<float>;

using QuaternionI = Quaternion<int>;

using QuaternionC = Quaternion<char>;

using QuaternionD = Quaternion<double>;

using QuaternionU = Quaternion<unsigned>;

}


