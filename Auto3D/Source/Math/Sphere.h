#pragma once

#include "Vector3.h"

namespace Auto3D
{

template<typename _Ty> class BoundingBox;
class Polyhedron;
class Frustum;

/// %Sphere in three-dimensional space.
class AUTO_API Sphere
{
public:
    /// Sphere center.
    Vector3F _center;
    /// Sphere radius.
    float _radius;
    
    /// Construct as undefined (negative radius.)
    Sphere() :
        _center(Vector3F::ZERO),
        _radius(-M_INFINITY)
    {
    }
    
    /// Copy-construct.
    Sphere(const Sphere& sphere) :
        _center(sphere._center),
        _radius(sphere._radius)
    {
    }
    
    /// Construct from center and radius.
    Sphere(const Vector3F& center, float radius) :
        _center(center),
        _radius(radius)
    {
    }
    
    /// Construct from an array of vertices.
    Sphere(const Vector3F* vertices, size_t count)
    {
        Define(vertices, count);
    }
    
    /// Construct from a bounding box.
    Sphere(const BoundingBoxF& box)
    {
        Define(box);
    }
    
    /// Construct from a frustum.
    Sphere(const Frustum& frustum)
    {
        Define(frustum);
    }
    
    /// Construct from a polyhedron.
    Sphere(const Polyhedron& poly)
    {
        Define(poly);
    }
    
    /// Assign from another sphere.
    Sphere& operator = (const Sphere& rhs)
    {
        _center = rhs._center;
        _radius = rhs._radius;
        return *this;
    }
    
    /// Test for equality with another sphere without epsilon.
    bool operator == (const Sphere& rhs) const { return _center == rhs._center && _radius == rhs._radius; }
    /// Test for inequality with another sphere without epsilon.
    bool operator != (const Sphere& rhs) const { return !(*this == rhs); }
    
    /// Define from another sphere.
    void Define(const Sphere& sphere)
    {
        _center = sphere._center;
        _radius = sphere._radius;
    }
    
    /// Define from center and radius.
    void Define(const Vector3F& center_, float radius_)
    {
        _center = center_;
        _radius = radius_;
    }
    
    /// Define from an array of vertices.
    void Define(const Vector3F* vertices, size_t count);
    /// Define from a bounding box.
    void Define(const BoundingBoxF& box);
    /// Define from a frustum.
    void Define(const Frustum& frustum);
    /// Define from a polyhedron.
    void Define(const Polyhedron& poly);
    
    /// Merge a point.
    void Merge(const Vector3F& point)
    {
        // If undefined, set initial dimensions
        if (!IsDefined())
        {
            _center = point;
            _radius = 0.0f;
            return;
        }
        
        Vector3F offset = point - _center;
        float dist = offset.Length();
        
        if (dist > _radius)
        {
            float half = (dist - _radius) * 0.5f;
            _radius += half;
            _center += (half / dist) * offset;
        }
    }
    
    /// Set as undefined to allow the next merge to set initial _size.
    void Undefine()
    {
        _radius = -M_INFINITY;
    }
    
    /// Merge an array of vertices.
    void Merge(const Vector3F* vertices, size_t count);
    /// Merge a bounding box.
    void Merge(const BoundingBoxF& box);
    /// Merge a frustum.
    void Merge(const Frustum& frustum);
    /// Merge a polyhedron.
    void Merge(const Polyhedron& poly);
    /// Merge a sphere.
    void Merge(const Sphere& sphere);
    
    /// Return whether has non-negative radius.
    bool IsDefined() const { return _radius >= 0.0f; }

    /// Test if a point is inside.
    Intersection IsInside(const Vector3F& point) const
    {
        float distSquared = (point - _center).LengthSquared();
        if (distSquared < _radius * _radius)
            return INSIDE;
        else
            return OUTSIDE;
    }
    
    /// Test if another sphere is inside, outside or intersects.
    Intersection IsInside(const Sphere& sphere) const
    {
        float dist = (sphere._center - _center).Length();
        if (dist >= sphere._radius + _radius)
            return OUTSIDE;
        else if (dist + sphere._radius < _radius)
            return INSIDE;
        else
            return INTERSECTS;
    }
    
    /// Test if another sphere is (partially) inside or outside.
    Intersection IsInsideFast(const Sphere& sphere) const
    {
        float distSquared = (sphere._center - _center).LengthSquared();
        float combined = sphere._radius + _radius;
        
        if (distSquared >= combined * combined)
            return OUTSIDE;
        else
            return INSIDE;
    }
    
    /// Test if a bounding box is inside, outside or intersects.
    Intersection IsInside(const BoundingBoxF& box) const;
    /// Test if a bounding box is (partially) inside or outside.
    Intersection IsInsideFast(const BoundingBoxF& box) const;
    
    /// Return distance of a point to the surface, or 0 if inside.
    float Distance(const Vector3F& point) const { return Max((point - _center).Length() - _radius, 0.0f); }
};

}
