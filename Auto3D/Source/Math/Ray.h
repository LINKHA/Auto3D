#pragma once

#include "Vector3.h"
#include "Matrix3x4.h"

namespace Auto3D
{

template<typename _Ty> class TBoundingBox;
class FFrustum;
class FPlane;
class FSphere;

/// Infinite straight line in three-dimensional space.
class AUTO_API FRay
{
public:
    /// FRay origin.
    TVector3F _origin;
    /// FRay direction.
    TVector3F _direction;
    
    /// Construct undefined ray.
    FRay()
    {
    }
    
    /// Copy-construct.
    FRay(const FRay& ray) :
        _origin(ray._origin),
        _direction(ray._direction)
    {
    }
    
    /// Construct from origin and direction. The direction will be normalized.
    FRay(const TVector3F& origin, const TVector3F& direction)
    {
        Define(origin, direction);
    }
    
    /// Assign from another ray.
    FRay& operator = (const FRay& rhs)
    {
        _origin = rhs._origin;
        _direction = rhs._direction;
        return *this;
    }
    
    /// Check for equality with another ray without epsilon.
    bool operator == (const FRay& rhs) const { return _origin == rhs._origin && _direction == rhs._direction; }
    /// Check for inequality with another ray without epsilon.
    bool operator != (const FRay& rhs) const { return !(*this == rhs); }
    
    /// Define from origin and direction. The direction will be normalized.
    void Define(const TVector3F& origin, const TVector3F& direction)
    {
        _origin = origin;
        _direction = direction.Normalized();
    }
    
    /// Project a point on the ray.
    TVector3F Project(const TVector3F& point) const
    {
        TVector3F offset = point - _origin;
        return _origin + offset.DotProduct(_direction) * _direction;
    }

    /// Return distance of a point from the ray.
    float Distance(const TVector3F& point) const
    {
        TVector3F projected = Project(point);
        return (point - projected).Length();
    }

    /// Test for equality with another ray with epsilon.
    bool Equals(const FRay& ray) const { return _origin.Equals(ray._origin) && _direction.Equals(ray._direction); }
    
    /// Return closest point to another ray.
    TVector3F ClosestPoint(const FRay& ray) const;
    /// Return hit distance to a plane, or infinity if no hit.
    float HitDistance(const FPlane& plane) const;
    /// Return hit distance to a bounding box, or infinity if no hit.
    float HitDistance(const TBoundingBox<float>& box) const;
    /// Return hit distance to a frustum, or infinity if no hit. If solidInside parameter is true (default) rays originating from inside return zero distance, otherwise the distance to the closest plane.
    float HitDistance(const FFrustum& frustum, bool solidInside = true) const;
    /// Return hit distance to a sphere, or infinity if no hit.
    float HitDistance(const FSphere& sphere) const;
    /// Return hit distance to a triangle, or infinity if no hit.
    float HitDistance(const TVector3F& v0, const TVector3F& v1, const TVector3F& v2) const;
    /// Return hit distance to a triangle and out normal, or infinity if no hit.
    float HitDistance(const TVector3F& v0, const TVector3F& v1, const TVector3F& v2, TVector3F* outNormal) const;
    /// Return hit distance to non-indexed geometry data, or infinity if no hit. Optionally return normal.
    float HitDistance(const void* vertexData, size_t vertexSize, size_t vertexStart, size_t vertexCount, TVector3F* outNormal = 0) const;
    /// Return hit distance to indexed geometry data, or infinity if no hit.
    float HitDistance(const void* vertexData, size_t vertexSize, const void* indexData, size_t indexSize, size_t indexStart, size_t indexCount, TVector3F* outNormal = 0) const;
    /// Return whether ray is inside non-indexed geometry.
    bool InsideGeometry(const void* vertexData, size_t vertexSize, size_t vertexStart, size_t vertexCount) const;
    /// Return whether ray is inside indexed geometry.
    bool InsideGeometry(const void* vertexData, size_t vertexSize, const void* indexData, size_t indexSize, size_t indexStart, size_t indexCount) const;
    /// Return transformed by a 3x4 matrix. This may result in a non-normalized direction.
    FRay Transformed(const TMatrix3x4F& transform) const;
};

}
