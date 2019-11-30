#include "Plane.h"

#include "Debug/DebugNew.h"

namespace Auto3D
{

// Static initialization order can not be relied on, so do not use TVector3 constants
const FPlane FPlane::UP(TVector3F(0.0f, 1.0f, 0.0f), TVector3F(0.0f, 0.0f, 0.0f));

void FPlane::Transform(const TMatrix3x3F& transform)
{
    Define(TMatrix4x4F(transform).Inverse().Transpose() * ToVector4());
}

void FPlane::Transform(const TMatrix3x4F& transform)
{
    Define(transform.ToMatrix4().Inverse().Transpose() * ToVector4());
}

void FPlane::Transform(const TMatrix4x4F& transform)
{
    Define(transform.Inverse().Transpose() * ToVector4());
}

TMatrix3x4F FPlane::ReflectionMatrix() const
{
    return TMatrix3x4F(
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

FPlane FPlane::Transformed(const TMatrix3x3F& transform) const
{
    return FPlane(TMatrix4x4F(transform).Inverse().Transpose() * ToVector4());
}

FPlane FPlane::Transformed(const TMatrix3x4F& transform) const
{
    return FPlane(transform.ToMatrix4().Inverse().Transpose() * ToVector4());
}

FPlane FPlane::Transformed(const TMatrix4x4F& transform) const
{
    return FPlane(transform.Inverse().Transpose() * ToVector4());
}

}
