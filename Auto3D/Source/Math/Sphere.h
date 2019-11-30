#pragma once

#include "Vector3.h"

namespace Auto3D
{

template<typename _Ty> class TBoundingBox;
class FPolyhedron;
class FFrustum;

/// %FSphere in three-dimensional space.
class AUTO_API FSphere
{
public:
    /// FSphere center.
    TVector3F _center;
    /// FSphere radius.
    float _radius;
    
    /// Construct as undefined (negative radius.)
    FSphere() :
        _center(TVector3F::ZERO),
        _radius(-M_INFINITY)
    {
    }
    
    /// Copy-construct.
    FSphere(const FSphere& sphere) :
        _center(sphere._center),
        _radius(sphere._radius)
    {
    }
    
    /// Construct from center and radius.
    FSphere(const TVector3F& center, float radius) :
        _center(center),
        _radius(radius)
    {
    }
    
    /// Construct from an array of vertices.
    FSphere(const TVector3F* vertices, size_t count)
    {
        Define(vertices, count);
    }
    
    /// Construct from a bounding box.
    FSphere(const TBoundingBoxF& box)
    {
        Define(box);
    }
    
    /// Construct from a frustum.
    FSphere(const FFrustum& frustum)
    {
        Define(frustum);
    }
    
    /// Construct from a polyhedron.
    FSphere(const FPolyhedron& poly)
    {
        Define(poly);
    }
    
    /// Assign from another sphere.
    FSphere& operator = (const FSphere& rhs)
    {
        _center = rhs._center;
        _radius = rhs._radius;
        return *this;
    }
    
    /// Test for equality with another sphere without epsilon.
    bool operator == (const FSphere& rhs) const { return _center == rhs._center && _radius == rhs._radius; }
    /// Test for inequality with another sphere without epsilon.
    bool operator != (const FSphere& rhs) const { return !(*this == rhs); }
    
    /// Define from another sphere.
    void Define(const FSphere& sphere)
    {
        _center = sphere._center;
        _radius = sphere._radius;
    }
    
    /// Define from center and radius.
    void Define(const TVector3F& center_, float radius_)
    {
        _center = center_;
        _radius = radius_;
    }
    
    /// Define from an array of vertices.
    void Define(const TVector3F* vertices, size_t count);
    /// Define from a bounding box.
    void Define(const TBoundingBoxF& box);
    /// Define from a frustum.
    void Define(const FFrustum& frustum);
    /// Define from a polyhedron.
    void Define(const FPolyhedron& poly);
    
    /// Merge a point.
    void Merge(const TVector3F& point)
    {
        // If undefined, set initial dimensions
        if (!IsDefined())
        {
            _center = point;
            _radius = 0.0f;
            return;
        }
        
        TVector3F offset = point - _center;
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
    void Merge(const TVector3F* vertices, size_t count);
    /// Merge a bounding box.
    void Merge(const TBoundingBoxF& box);
    /// Merge a frustum.
    void Merge(const FFrustum& frustum);
    /// Merge a polyhedron.
    void Merge(const FPolyhedron& poly);
    /// Merge a sphere.
    void Merge(const FSphere& sphere);
    
    /// Return whether has non-negative radius.
    bool IsDefined() const { return _radius >= 0.0f; }

    /// Test if a point is inside.
    Intersection IsInside(const TVector3F& point) const
    {
        float distSquared = (point - _center).LengthSquared();
        if (distSquared < _radius * _radius)
            return INSIDE;
        else
            return OUTSIDE;
    }
    
    /// Test if another sphere is inside, outside or intersects.
    Intersection IsInside(const FSphere& sphere) const
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
    Intersection IsInsideFast(const FSphere& sphere) const
    {
        float distSquared = (sphere._center - _center).LengthSquared();
        float combined = sphere._radius + _radius;
        
        if (distSquared >= combined * combined)
            return OUTSIDE;
        else
            return INSIDE;
    }
    
    /// Test if a bounding box is inside, outside or intersects.
    Intersection IsInside(const TBoundingBoxF& box) const;
    /// Test if a bounding box is (partially) inside or outside.
    Intersection IsInsideFast(const TBoundingBoxF& box) const;
    
    /// Return distance of a point to the surface, or 0 if inside.
    float Distance(const TVector3F& point) const { return Max((point - _center).Length() - _radius, 0.0f); }
};

}
