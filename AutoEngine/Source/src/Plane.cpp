// For conditions of distribution and use, see copyright notice in License.txt

#include "Plane.h"

#include "NewDef.h"

namespace Auto3D
{

// Static initialization order can not be relied on, so do not use Vector3F constants
const Plane Plane::UP(Vector3F(0.0f, 1.0f, 0.0f), Vector3F(0.0f, 0.0f, 0.0f));

void Plane::Transform(const Matrix3x3F& transform)
{
	Define(Matrix4x4F(transform).Inverse().Transpose() * ToVector4());
}

void Plane::Transform(const Matrix3x4F& transform)
{
	Define(transform.ToMatrix4().Inverse().Transpose() * ToVector4());
}

void Plane::Transform(const Matrix4x4F& transform)
{
	Define(transform.Inverse().Transpose() * ToVector4());
}

Matrix3x4F Plane::ReflectionMatrix() const
{
	return Matrix3x4F(
		-2.0f * normal._x * normal._x + 1.0f,
		-2.0f * normal._x * normal._y,
		-2.0f * normal._x * normal._z,
		-2.0f * normal._x * d,
		-2.0f * normal._y * normal._x,
		-2.0f * normal._y * normal._y + 1.0f,
		-2.0f * normal._y * normal._z,
		-2.0f * normal._y * d,
		-2.0f * normal._z * normal._x,
		-2.0f * normal._z * normal._y,
		-2.0f * normal._z * normal._z + 1.0f,
		-2.0f * normal._z * d
	);
}

Plane Plane::Transformed(const Matrix3x3F& transform) const
{
	return Plane(Matrix4x4F(transform).Inverse().Transpose() * ToVector4());
}

Plane Plane::Transformed(const Matrix3x4F& transform) const
{
	return Plane(transform.ToMatrix4().Inverse().Transpose() * ToVector4());
}

Plane Plane::Transformed(const Matrix4x4F& transform) const
{
	return Plane(transform.Inverse().Transpose() * ToVector4());
}

}
