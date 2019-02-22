#include "Quaternion.h"

namespace Auto3D {

const QuaternionF QuaternionF::IDENTITY;

const QuaternionI QuaternionI::IDENTITY;

const QuaternionD QuaternionD::IDENTITY;

const QuaternionC QuaternionC::IDENTITY;

template<typename _Ty> void Quaternion<_Ty>::FromAngleAxis(_Ty angle, const Vector3<_Ty>& axis)
{
	Vector3 normAxis = axis.Normalized();
	angle *= M_DEGTORAD_2;
	_Ty sinAngle = sinf(angle);
	_Ty cosAngle = cosf(angle);

	_w = cosAngle;
	_x = normAxis._x * sinAngle;
	_y = normAxis._y * sinAngle;
	_z = normAxis._z * sinAngle;
}

template<typename _Ty> void Quaternion<_Ty>::FromEulerAngles(_Ty x, _Ty y, _Ty z)
{
	// Order of rotations: Z first, then X, then Y (mimics typical FPS camera with gimbal lock at top/bottom)
	x *= M_DEGTORAD_2;
	y *= M_DEGTORAD_2;
	z *= M_DEGTORAD_2;
	_Ty sinX = sinf(x);
	_Ty cosX = cosf(x);
	_Ty sinY = sinf(y);
	_Ty cosY = cosf(y);
	_Ty sinZ = sinf(z);
	_Ty cosZ = cosf(z);

	_w = cosY * cosX * cosZ + sinY * sinX * sinZ;
	_x = cosY * sinX * cosZ + sinY * cosX * sinZ;
	_y = sinY * cosX * cosZ - cosY * sinX * sinZ;
	_z = cosY * cosX * sinZ - sinY * sinX * cosZ;
}

template<typename _Ty> void Quaternion<_Ty>::FromRotationTo(const Vector3<_Ty>& start, const Vector3<_Ty>& end)
{
	Vector3 normStart = start.Normalized();
	Vector3 normEnd = end.Normalized();
	_Ty d = normStart.DotProduct(normEnd);

	if (d > -1.0f + M_EPSILON)
	{
		Vector3 c = normStart.CrossProduct(normEnd);
		_Ty s = sqrtf((1.0f + d) * 2.0f);
		_Ty invS = 1.0f / s;

		_x = c._x * invS;
		_y = c._y * invS;
		_z = c._z * invS;
		_w = 0.5f * s;
	}
	else
	{
		Vector3 axis = Vector3::RIGHT.CrossProduct(normStart);
		if (axis.Length() < M_EPSILON)
			axis = Vector3::UP.CrossProduct(normStart);

		FromAngleAxis(180.f, axis);
	}
}

template<typename _Ty> void Quaternion<_Ty>::FromAxes(const Vector3<_Ty>& xAxis, const Vector3<_Ty>& yAxis, const Vector3<_Ty>& zAxis)
{
	Matrix3x3 matrix(
		xAxis._x, yAxis._x, zAxis._x,
		xAxis._y, yAxis._y, zAxis._y,
		xAxis._z, yAxis._z, zAxis._z
	);

	FromRotationMatrix(matrix);
}

template<typename _Ty> void Quaternion<_Ty>::FromRotationMatrix(const Matrix3x3<_Ty>& matrix)
{
	_Ty t = matrix._m00 + matrix._m11 + matrix._m22;

	if (t > 0.0f)
	{
		_Ty invS = 0.5f / sqrtf(1.0f + t);

		_x = (matrix._m21 - matrix._m12) * invS;
		_y = (matrix._m02 - matrix._m20) * invS;
		_z = (matrix._m10 - matrix._m01) * invS;
		_w = 0.25f / invS;
	}
	else
	{
		if (matrix._m00 > matrix._m11 && matrix._m00 > matrix._m22)
		{
			_Ty invS = 0.5f / sqrtf(1.0f + matrix._m00 - matrix._m11 - matrix._m22);

			_x = 0.25f / invS;
			_y = (matrix._m01 + matrix._m10) * invS;
			_z = (matrix._m20 + matrix._m02) * invS;
			_w = (matrix._m21 - matrix._m12) * invS;
		}
		else if (matrix._m11 > matrix._m22)
		{
			_Ty invS = 0.5f / sqrtf(1.0f + matrix._m11 - matrix._m00 - matrix._m22);

			_x = (matrix._m01 + matrix._m10) * invS;
			_y = 0.25f / invS;
			_z = (matrix._m12 + matrix._m21) * invS;
			_w = (matrix._m02 - matrix._m20) * invS;
		}
		else
		{
			_Ty invS = 0.5f / sqrtf(1.0f + matrix._m22 - matrix._m00 - matrix._m11);

			_x = (matrix._m02 + matrix._m20) * invS;
			_y = (matrix._m12 + matrix._m21) * invS;
			_z = 0.25f / invS;
			_w = (matrix._m10 - matrix._m01) * invS;
		}
	}
}

template<typename _Ty> bool Quaternion<_Ty>::FromLookRotation(const Vector3<_Ty>& direction, const Vector3<_Ty>& up)
{
	Quaternion<_Ty> ret;
	Vector3<_Ty> forward = direction.Normalized();

	Vector3<_Ty> v = forward.CrossProduct(up);
	// If direction & up are parallel and crossproduct becomes zero, use FromRotationTo() fallback
	if (v.LengthSquared() >= M_EPSILON)
	{
		v.Normalize();
		Vector3<_Ty> up = v.CrossProduct(forward);
		Vector3<_Ty> right = up.CrossProduct(forward);
		ret.FromAxes(right, up, forward);
	}
	else
		ret.FromRotationTo(Vector3::FORWARD, forward);

	if (!ret.IsNaN())
	{
		(*this) = ret;
		return true;
	}
	else
		return false;
}

template<typename _Ty> Vector3<_Ty> Quaternion<_Ty>::EulerAngles() const
{
	// Derivation from http://www.geometrictools.com/Documentation/EulerAngles.pdf
	// Order of rotations: Z first, then X, then Y
	_Ty check = 2.0f * (-_y * _z + _w * _x);

	if (check < -0.995f)
	{
		return Vector3<_Ty>(
			-90.0f,
			0.0f,
			-atan2f(2.0f * (_x * _z - _w * _y), 1.0f - 2.0f * (_y * _y + _z * _z)) * M_RADTODEG
		);
	}
	else if (check > 0.995f)
	{
		return Vector3<_Ty>(
			90.0f,
			0.0f,
			atan2f(2.0f * (_x * _z - _w * _y), 1.0f - 2.0f * (_y * _y + _z * _z)) * M_RADTODEG
		);
	}
	else
	{
		return Vector3<_Ty>(
			asinf(check) * M_RADTODEG,
			atan2f(2.0f * (_x * _z + _w * _y), 1.0f - 2.0f * (_x * _x + _y * _y)) * M_RADTODEG,
			atan2f(2.0f * (_x * _y + _w * _z), 1.0f - 2.0f * (_x * _x + _z * _z)) * M_RADTODEG
		);
	}
}

template<typename _Ty> _Ty Quaternion<_Ty>::YawAngle() const
{
	return EulerAngles()._y;
}

template<typename _Ty> _Ty Quaternion<_Ty>::PitchAngle() const
{
	return EulerAngles()._x;
}

template<typename _Ty> _Ty Quaternion<_Ty>::RollAngle() const
{
	return EulerAngles()._z;
}

template<typename _Ty> Auto3D::Vector3<_Ty> Quaternion<_Ty>::Axis() const
{
	return Vector3(_x, _y, _z) / sqrt(1. - _w * _w);
}

template<typename _Ty> _Ty Quaternion<_Ty>::Angle() const
{
	return 2 * Acos(_w);
}

template<typename _Ty> Matrix3x3<_Ty> Quaternion<_Ty>::RotationMatrix() const
{
	return Matrix3x3(
		1.0f - 2.0f * _y * _y - 2.0f * _z * _z,
		2.0f * _x * _y - 2.0f * _w * _z,
		2.0f * _x * _z + 2.0f * _w * _y,
		2.0f * _x * _y + 2.0f * _w * _z,
		1.0f - 2.0f * _x * _x - 2.0f * _z * _z,
		2.0f * _y * _z - 2.0f * _w * _x,
		2.0f * _x * _z - 2.0f * _w * _y,
		2.0f * _y * _z + 2.0f * _w * _x,
		1.0f - 2.0f * _x * _x - 2.0f * _y * _y
	);
}

template<typename _Ty> Quaternion<_Ty> Quaternion<_Ty>::Slerp(const Quaternion<_Ty>& rhs, _Ty t) const
{
// Favor accuracy for native code builds
	_Ty cosAngle = DotProduct(rhs);
	_Ty sign = 1.0f;
	// Enable shortest path rotation
	if (cosAngle < 0.0f)
	{
		cosAngle = -cosAngle;
		sign = -1.0f;
	}

	_Ty angle = acosf(cosAngle);
	_Ty sinAngle = sinf(angle);
	_Ty t1, t2;

	if (sinAngle > 0.001f)
	{
		_Ty invSinAngle = 1.0f / sinAngle;
		t1 = sinf((1.0f - t) * angle) * invSinAngle;
		t2 = sinf(t * angle) * invSinAngle;
	}
	else
	{
		t1 = 1.0f - t;
		t2 = t;
	}

	return *this * t1 + (rhs * sign) * t2;
}

template<typename _Ty> Quaternion<_Ty> Quaternion<_Ty>::Nlerp(const Quaternion<_Ty>& rhs, _Ty t, bool shortestPath) const
{
	Quaternion<_Ty> result;
	_Ty fCos = DotProduct(rhs);
	if (fCos < 0.0f && shortestPath)
		result = (*this) + (((-rhs) - (*this)) * t);
	else
		result = (*this) + ((rhs - (*this)) * t);
	result.Normalize();
	return result;
}

template<typename _Ty> STRING Quaternion<_Ty>::ToString() const
{
	char tempBuffer[KhSTL::CONVERSION_BUFFER_LENGTH];
	sprintf(tempBuffer, "%g %g %g %g", _w, _x, _y, _z);
	return STRING(tempBuffer);
}


}