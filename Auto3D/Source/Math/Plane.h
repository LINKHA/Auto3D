#pragma once

#include "Matrix3x4.h"

namespace Auto3D
{

/// Surface in three-dimensional space.
class AUTO_API FPlane
{
public:
    /// FPlane normal.
    TVector3F _normal;
    /// FPlane absolute normal.
    TVector3F _absNormal;
    /// FPlane constant.
    float _d;

    /// Construct undefined.
    FPlane()
    {
    }
    
    /// Copy-construct.
    FPlane(const FPlane& plane) :
        _normal(plane._normal),
        _absNormal(plane._absNormal),
        _d(plane._d)
    {
    }
    
    /// Construct from 3 vertices.
    FPlane(const TVector3F& v0, const TVector3F& v1, const TVector3F& v2)
    {
        Define(v0, v1, v2);
    }
    
    /// Construct from a normal vector and a point on the plane.
    FPlane(const TVector3F& normal, const TVector3F& point)
    {
        Define(normal, point);
    }
    
    /// Construct from a 4-dimensional vector, where the w coordinate is the plane parameter.
    FPlane(const TVector4F& plane)
    {
        Define(plane);
    }
    
    /// Define from 3 vertices.
    void Define(const TVector3F& v0, const TVector3F& v1, const TVector3F& v2)
    {
        TVector3F dist1 = v1 - v0;
        TVector3F dist2 = v2 - v0;
        
        Define(dist1.CrossProduct(dist2), v0);
    }

    /// Define from a normal vector and a point on the plane.
    void Define(const TVector3F& normal, const TVector3F& point)
    {
        _normal = normal.Normalized();
        _absNormal = _normal.Abs();
        _d = -_normal.DotProduct(point);
    }
    
    /// Define from a 4-dimensional vector, where the w coordinate is the plane parameter.
    void Define(const TVector4F& plane)
    {
        _normal = TVector3F(plane._x, plane._y, plane._z);
        _absNormal = _normal.Abs();
        _d = plane._w;
    }
    
    /// Transform with a 3x3 matrix.
    void Transform(const TMatrix3x3F& transform);
    /// Transform with a 3x4 matrix.
    void Transform(const TMatrix3x4F& transform);
    /// Transform with a 4x4 matrix.
    void Transform(const TMatrix4x4F& transform);
    
    /// Project a point on the plane.
    TVector3F Project(const TVector3F& point) const { return point - _normal * (_normal.DotProduct(point) + _d); }
    /// Return signed distance to a point.
    float Distance(const TVector3F& point) const { return _normal.DotProduct(point) + _d; }
    /// Reflect a normalized direction vector.
    TVector3F Reflect(const TVector3F& direction) const { return direction - (2.0f * _normal.DotProduct(direction) * _normal); }
    /// Return a reflection matrix.
    TMatrix3x4F ReflectionMatrix() const;
    /// Return transformed by a 3x3 matrix.
    FPlane Transformed(const TMatrix3x3F& transform) const;
    /// Return transformed by a 3x4 matrix.
    FPlane Transformed(const TMatrix3x4F& transform) const;
    /// Return transformed by a 4x4 matrix.
    FPlane Transformed(const TMatrix4x4F& transform) const;
    /// Return as a vector.
    TVector4F ToVector4() const { return TVector4F(_normal, _d); }
    
    /// FPlane at origin with normal pointing up.
    static const FPlane UP;
};

}
