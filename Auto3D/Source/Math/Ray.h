#pragma once

#include "Vector3.h"
#include "Matrix3x4.h"

namespace Auto3D
{

class BoundingBox;
class Frustum;
class Plane;
class Sphere;

/// Infinite straight line in three-dimensional space.
class AUTO_API Ray
{
public:
    /// Ray origin.
    Vector3 _origin;
    /// Ray direction.
    Vector3 _direction;
    
    /// Construct undefined ray.
    Ray()
    {
    }
    
    /// Copy-construct.
    Ray(const Ray& ray) :
        _origin(ray._origin),
        _direction(ray._direction)
    {
    }
    
    /// Construct from origin and direction. The direction will be normalized.
    Ray(const Vector3& origin, const Vector3& direction)
    {
        Define(origin, direction);
    }
    
    /// Assign from another ray.
    Ray& operator = (const Ray& rhs)
    {
        _origin = rhs._origin;
        _direction = rhs._direction;
        return *this;
    }
    
    /// Check for equality with another ray without epsilon.
    bool operator == (const Ray& rhs) const { return _origin == rhs._origin && _direction == rhs._direction; }
    /// Check for inequality with another ray without epsilon.
    bool operator != (const Ray& rhs) const { return !(*this == rhs); }
    
    /// Define from origin and direction. The direction will be normalized.
    void Define(const Vector3& origin, const Vector3& direction)
    {
        _origin = origin;
        _direction = direction.Normalized();
    }
    
    /// Project a point on the ray.
    Vector3 Project(const Vector3& point) const
    {
        Vector3 offset = point - _origin;
        return _origin + offset.DotProduct(_direction) * _direction;
    }

    /// Return distance of a point from the ray.
    float Distance(const Vector3& point) const
    {
        Vector3 projected = Project(point);
        return (point - projected).Length();
    }

    /// Test for equality with another ray with epsilon.
    bool Equals(const Ray& ray) const { return _origin.Equals(ray._origin) && _direction.Equals(ray._direction); }
    
    /// Return closest point to another ray.
    Vector3 ClosestPoint(const Ray& ray) const;
    /// Return hit distance to a plane, or infinity if no hit.
    float HitDistance(const Plane& plane) const;
    /// Return hit distance to a bounding box, or infinity if no hit.
    float HitDistance(const BoundingBox& box) const;
    /// Return hit distance to a frustum, or infinity if no hit. If solidInside parameter is true (default) rays originating from inside return zero distance, otherwise the distance to the closest plane.
    float HitDistance(const Frustum& frustum, bool solidInside = true) const;
    /// Return hit distance to a sphere, or infinity if no hit.
    float HitDistance(const Sphere& sphere) const;
    /// Return hit distance to a triangle, or infinity if no hit.
    float HitDistance(const Vector3& v0, const Vector3& v1, const Vector3& v2) const;
    /// Return hit distance to a triangle and out normal, or infinity if no hit.
    float HitDistance(const Vector3& v0, const Vector3& v1, const Vector3& v2, Vector3* outNormal) const;
    /// Return hit distance to non-indexed geometry data, or infinity if no hit. Optionally return normal.
    float HitDistance(const void* vertexData, size_t vertexSize, size_t vertexStart, size_t vertexCount, Vector3* outNormal = 0) const;
    /// Return hit distance to indexed geometry data, or infinity if no hit.
    float HitDistance(const void* vertexData, size_t vertexSize, const void* indexData, size_t indexSize, size_t indexStart, size_t indexCount, Vector3* outNormal = 0) const;
    /// Return whether ray is inside non-indexed geometry.
    bool InsideGeometry(const void* vertexData, size_t vertexSize, size_t vertexStart, size_t vertexCount) const;
    /// Return whether ray is inside indexed geometry.
    bool InsideGeometry(const void* vertexData, size_t vertexSize, const void* indexData, size_t indexSize, size_t indexStart, size_t indexCount) const;
    /// Return transformed by a 3x4 matrix. This may result in a non-normalized direction.
    Ray Transformed(const Matrix3x4& transform) const;
};

}
