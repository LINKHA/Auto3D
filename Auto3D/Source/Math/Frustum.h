#pragma once

#include "BoundingBox.h"
#include "Matrix3x4.h"
#include "Plane.h"
#include "Sphere.h"

namespace Auto3D
{

/// Frustum planes.
enum FrustumPlane
{
    PLANE_NEAR = 0,
    PLANE_LEFT,
    PLANE_RIGHT,
    PLANE_UP,
    PLANE_DOWN,
    PLANE_FAR,
};

static const size_t NUM_FRUSTUM_PLANES = 6;
static const size_t NUM_FRUSTUM_VERTICES = 8;

/// Convex constructed of 6 planes.
class AUTO_API Frustum
{
public:
    /// Frustum planes.
    Plane _planes[NUM_FRUSTUM_PLANES];
    /// Frustum vertices.
    Vector3 _vertices[NUM_FRUSTUM_VERTICES];
    
    /// Construct a degenerate frustum with all points at origin.
    Frustum();
    /// Copy-construct.
    Frustum(const Frustum& frustum);
    
    /// Assign from another frustum.
    Frustum& operator = (const Frustum& rhs);
    
    /// Define with projection parameters and a transform matrix.
    void Define(float fov, float aspectRatio, float zoom, float nearZ, float farZ, const Matrix3x4& transform = Matrix3x4::IDENTITY);
    /// Define with near and far dimension vectors and a transform matrix.
    void Define(const Vector3& near, const Vector3& far, const Matrix3x4& transform = Matrix3x4::IDENTITY);
    /// Define with a bounding box and a transform matrix.
    void Define(const BoundingBox& box, const Matrix3x4& transform = Matrix3x4::IDENTITY);
    /// Define with orthographic projection parameters and a transform matrix.
    void DefineOrtho(float orthoSize, float aspectRatio, float zoom, float nearZ, float farZ, const Matrix3x4& transform = Matrix3x4::IDENTITY);
    /// Transform by a 3x3 matrix.
    void Transform(const Matrix3& transform);
    /// Transform by a 3x4 matrix.
    void Transform(const Matrix3x4& transform);
    
    /// Test if a point is inside or outside.
    Intersection IsInside(const Vector3& point) const
    {
        for (size_t i = 0; i < NUM_FRUSTUM_PLANES; ++i)
        {
            if (_planes[i].Distance(point) < 0.0f)
                return OUTSIDE;
        }
        
        return INSIDE;
    }
    
    /// Test if a sphere is inside, outside or intersects.
    Intersection IsInside(const Sphere& sphere) const
    {
        bool allInside = true;
        for (size_t i = 0; i < NUM_FRUSTUM_PLANES; ++i)
        {
            float dist = _planes[i].Distance(sphere._center);
            if (dist < -sphere._radius)
                return OUTSIDE;
            else if (dist < sphere._radius)
                allInside = false;
        }
        
        return allInside ? INSIDE : INTERSECTS;
    }
    
    /// Test if a sphere if (partially) inside or outside.
    Intersection IsInsideFast(const Sphere& sphere) const
    {
        for (size_t i = 0; i < NUM_FRUSTUM_PLANES; ++i)
        {
            if (_planes[i].Distance(sphere._center) < -sphere._radius)
                return OUTSIDE;
        }
        
        return INSIDE;
    }
    
    /// Test if a bounding box is inside, outside or intersects.
    Intersection IsInside(const BoundingBox& box) const
    {
        Vector3 center = box.Center();
        Vector3 edge = center - box._min;
        bool allInside = true;
        
        for (size_t i = 0; i < NUM_FRUSTUM_PLANES; ++i)
        {
            const Plane& plane = _planes[i];
            float dist = plane._normal.DotProduct(center) + plane._d;
            float absDist = plane._absNormal.DotProduct(edge);
            
            if (dist < -absDist)
                return OUTSIDE;
            else if (dist < absDist)
                allInside = false;
        }
        
        return allInside ? INSIDE : INTERSECTS;
    }
    
    /// Test if a bounding box is (partially) inside or outside.
    Intersection IsInsideFast(const BoundingBox& box) const
    {
        Vector3 center = box.Center();
        Vector3 edge = center - box._min;
        
        for (size_t i = 0; i < NUM_FRUSTUM_PLANES; ++i)
        {
            const Plane& plane = _planes[i];
            float dist = plane._normal.DotProduct(center) + plane._d;
            float absDist = plane._absNormal.DotProduct(edge);
            
            if (dist < -absDist)
                return OUTSIDE;
        }
        
        return INSIDE;
    }
    
    /// Return distance of a point to the frustum, or 0 if inside.
    float Distance(const Vector3& point) const
    {
        float distance = 0.0f;
        for (size_t i = 0; i < NUM_FRUSTUM_PLANES; ++i)
            distance = Max(-_planes[i].Distance(point), distance);
        
        return distance;
    }
    
    /// Return transformed by a 3x3 matrix.
    Frustum Transformed(const Matrix3& transform) const;
    /// Return transformed by a 3x4 matrix.
    Frustum Transformed(const Matrix3x4& transform) const;
    /// Return projected by a 4x4 projection matrix.
    Rect Projected(const Matrix4& transform) const;
    
    /// Update the planes. Called internally.
    void UpdatePlanes();
};

}
