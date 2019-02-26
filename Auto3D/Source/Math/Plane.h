#pragma once

#include "Matrix3x4.h"

namespace Auto3D
{

/// Surface in three-dimensional space.
class AUTO_API Plane
{
public:
    /// Plane normal.
    Vector3F _normal;
    /// Plane absolute normal.
    Vector3F _absNormal;
    /// Plane constant.
    float _d;

    /// Construct undefined.
    Plane()
    {
    }
    
    /// Copy-construct.
    Plane(const Plane& plane) :
        _normal(plane._normal),
        _absNormal(plane._absNormal),
        _d(plane._d)
    {
    }
    
    /// Construct from 3 vertices.
    Plane(const Vector3F& v0, const Vector3F& v1, const Vector3F& v2)
    {
        Define(v0, v1, v2);
    }
    
    /// Construct from a normal vector and a point on the plane.
    Plane(const Vector3F& normal, const Vector3F& point)
    {
        Define(normal, point);
    }
    
    /// Construct from a 4-dimensional vector, where the w coordinate is the plane parameter.
    Plane(const Vector4F& plane)
    {
        Define(plane);
    }
    
    /// Define from 3 vertices.
    void Define(const Vector3F& v0, const Vector3F& v1, const Vector3F& v2)
    {
        Vector3F dist1 = v1 - v0;
        Vector3F dist2 = v2 - v0;
        
        Define(dist1.CrossProduct(dist2), v0);
    }

    /// Define from a normal vector and a point on the plane.
    void Define(const Vector3F& normal_, const Vector3F& point)
    {
        _normal = normal_.Normalized();
        _absNormal = _normal.Abs();
        _d = -_normal.DotProduct(point);
    }
    
    /// Define from a 4-dimensional vector, where the w coordinate is the plane parameter.
    void Define(const Vector4F& plane)
    {
        _normal = Vector3F(plane._x, plane._y, plane._z);
        _absNormal = _normal.Abs();
        _d = plane._w;
    }
    
    /// Transform with a 3x3 matrix.
    void Transform(const Matrix3& transform);
    /// Transform with a 3x4 matrix.
    void Transform(const Matrix3x4& transform);
    /// Transform with a 4x4 matrix.
    void Transform(const Matrix4x4F& transform);
    
    /// Project a point on the plane.
    Vector3F Project(const Vector3F& point) const { return point - _normal * (_normal.DotProduct(point) + _d); }
    /// Return signed distance to a point.
    float Distance(const Vector3F& point) const { return _normal.DotProduct(point) + _d; }
    /// Reflect a normalized direction vector.
    Vector3F Reflect(const Vector3F& direction) const { return direction - (2.0f * _normal.DotProduct(direction) * _normal); }
    /// Return a reflection matrix.
    Matrix3x4 ReflectionMatrix() const;
    /// Return transformed by a 3x3 matrix.
    Plane Transformed(const Matrix3& transform) const;
    /// Return transformed by a 3x4 matrix.
    Plane Transformed(const Matrix3x4& transform) const;
    /// Return transformed by a 4x4 matrix.
    Plane Transformed(const Matrix4x4F& transform) const;
    /// Return as a vector.
    Vector4F ToVector4() const { return Vector4F(_normal, _d); }
    
    /// Plane at origin with normal pointing up.
    static const Plane UP;
};

}
