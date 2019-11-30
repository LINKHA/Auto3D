#pragma once

#include "BoundingBox.h"
#include "Matrix3x4.h"
#include "Plane.h"
#include "Sphere.h"

namespace Auto3D
{

/// FFrustum planes.
namespace EFrustumPlane
{
	enum Type
	{
		NEAR = 0,
		LEFT,
		RIGHT,
		UP,
		DOWN,
		FAR,
	};
};


static const size_t NUM_FRUSTUM_PLANES = 6;
static const size_t NUM_FRUSTUM_VERTICES = 8;

/// Convex constructed of 6 planes.
class AUTO_API FFrustum
{
public:
    /// FFrustum planes.
    FPlane _planes[NUM_FRUSTUM_PLANES];
    /// FFrustum vertices.
    TVector3F _vertices[NUM_FRUSTUM_VERTICES];
    
    /// Construct a degenerate frustum with all points at origin.
    FFrustum();
    /// Copy-construct.
    FFrustum(const FFrustum& frustum);
    
    /// Assign from another frustum.
    FFrustum& operator = (const FFrustum& rhs);
    
    /// Define with projection parameters and a transform matrix.
    void Define(float fov, float aspectRatio, float zoom, float nearZ, float farZ, const TMatrix3x4F& transform = TMatrix3x4F::IDENTITY);
    /// Define with near and far dimension vectors and a transform matrix.
    void Define(const TVector3F& near, const TVector3F& far, const TMatrix3x4F& transform = TMatrix3x4F::IDENTITY);
    /// Define with a bounding box and a transform matrix.
    void Define(const TBoundingBoxF& box, const TMatrix3x4F& transform = TMatrix3x4F::IDENTITY);
    /// Define with orthographic projection parameters and a transform matrix.
    void DefineOrtho(float orthoSize, float aspectRatio, float zoom, float nearZ, float farZ, const TMatrix3x4F& transform = TMatrix3x4F::IDENTITY);
    /// Transform by a 3x3 matrix.
    void Transform(const TMatrix3x3F& transform);
    /// Transform by a 3x4 matrix.
    void Transform(const TMatrix3x4F& transform);
    
    /// Test if a point is inside or outside.
	EIntersection::Type IsInside(const TVector3F& point) const
    {
        for (size_t i = 0; i < NUM_FRUSTUM_PLANES; ++i)
        {
            if (_planes[i].Distance(point) < 0.0f)
                return EIntersection::OUTSIDE;
        }
        
        return EIntersection::INSIDE;
    }
    
    /// Test if a sphere is inside, outside or intersects.
	EIntersection::Type IsInside(const FSphere& sphere) const
    {
        bool allInside = true;
        for (size_t i = 0; i < NUM_FRUSTUM_PLANES; ++i)
        {
            float dist = _planes[i].Distance(sphere._center);
            if (dist < -sphere._radius)
                return EIntersection::OUTSIDE;
            else if (dist < sphere._radius)
                allInside = false;
        }
        
        return allInside ? EIntersection::INSIDE : EIntersection::INTERSECTS;
    }
    
    /// Test if a sphere if (partially) inside or outside.
	EIntersection::Type IsInsideFast(const FSphere& sphere) const
    {
        for (size_t i = 0; i < NUM_FRUSTUM_PLANES; ++i)
        {
            if (_planes[i].Distance(sphere._center) < -sphere._radius)
                return EIntersection::OUTSIDE;
        }
        
        return EIntersection::INSIDE;
    }
    
    /// Test if a bounding box is inside, outside or intersects.
	EIntersection::Type IsInside(const TBoundingBoxF& box) const
    {
        TVector3F center = box.Center();
        TVector3F edge = center - box._min;
        bool allInside = true;
        
        for (size_t i = 0; i < NUM_FRUSTUM_PLANES; ++i)
        {
            const FPlane& plane = _planes[i];
            float dist = plane._normal.DotProduct(center) + plane._d;
            float absDist = plane._absNormal.DotProduct(edge);
            
            if (dist < -absDist)
                return EIntersection::OUTSIDE;
            else if (dist < absDist)
                allInside = false;
        }
        
        return allInside ? EIntersection::INSIDE : EIntersection::INTERSECTS;
    }
    
    /// Test if a bounding box is (partially) inside or outside.
	EIntersection::Type IsInsideFast(const TBoundingBoxF& box) const
    {
        TVector3F center = box.Center();
        TVector3F edge = center - box._min;
        
        for (size_t i = 0; i < NUM_FRUSTUM_PLANES; ++i)
        {
            const FPlane& plane = _planes[i];
            float dist = plane._normal.DotProduct(center) + plane._d;
            float absDist = plane._absNormal.DotProduct(edge);
            
            if (dist < -absDist)
                return EIntersection::OUTSIDE;
        }
        
        return EIntersection::INSIDE;
    }
    
    /// Return distance of a point to the frustum, or 0 if inside.
    float Distance(const TVector3F& point) const
    {
        float distance = 0.0f;
        for (size_t i = 0; i < NUM_FRUSTUM_PLANES; ++i)
            distance = Max(-_planes[i].Distance(point), distance);
        
        return distance;
    }
    
    /// Return transformed by a 3x3 matrix.
    FFrustum Transformed(const TMatrix3x3F& transform) const;
    /// Return transformed by a 3x4 matrix.
    FFrustum Transformed(const TMatrix3x4F& transform) const;
    /// Return projected by a 4x4 projection matrix.
    TRectF Projected(const TMatrix4x4F& transform) const;
    
    /// Update the planes. Called internally.
    void UpdatePlanes();
};

}
