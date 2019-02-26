#include "Plane.h"

#include "../Debug/DebugNew.h"

namespace Auto3D
{

// Static initialization order can not be relied on, so do not use Vector3 constants
const Plane Plane::UP(Vector3(0.0f, 1.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f));

void Plane::Transform(const Matrix3& transform)
{
    Define(Matrix4(transform).Inverse().Transpose() * ToVector4());
}

void Plane::Transform(const Matrix3x4& transform)
{
    Define(transform.ToMatrix4().Inverse().Transpose() * ToVector4());
}

void Plane::Transform(const Matrix4& transform)
{
    Define(transform.Inverse().Transpose() * ToVector4());
}

Matrix3x4 Plane::ReflectionMatrix() const
{
    return Matrix3x4(
        -2.0f * _normal._x * _normal._x + 1.0f,
        -2.0f * _normal._x * _normal._y,
        -2.0f * _normal._x * _normal._z,
        -2.0f * _normal._x * _d,
        -2.0f * _normal._y * _normal._x ,
        -2.0f * _normal._y * _normal._y + 1.0f,
        -2.0f * _normal._y * _normal._z,
        -2.0f * _normal._y * _d,
        -2.0f * _normal._z * _normal._x,
        -2.0f * _normal._z * _normal._y,
        -2.0f * _normal._z * _normal._z + 1.0f,
        -2.0f * _normal._z * _d
    );
}

Plane Plane::Transformed(const Matrix3& transform) const
{
    return Plane(Matrix4(transform).Inverse().Transpose() * ToVector4());
}

Plane Plane::Transformed(const Matrix3x4& transform) const
{
    return Plane(transform.ToMatrix4().Inverse().Transpose() * ToVector4());
}

Plane Plane::Transformed(const Matrix4& transform) const
{
    return Plane(transform.Inverse().Transpose() * ToVector4());
}

}
