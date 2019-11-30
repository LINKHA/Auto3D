#pragma once

#include "Vector3.h"
#include "Container/Vector.h"

namespace Auto3D
{

template<typename _Ty> class TBoundingBox;
template<typename _Ty> class TMatrix3x3;
template<typename _Ty> class TMatrix4x4;
class FFrustum;
class FPlane;

/// A convex volume built from polygon faces.
class AUTO_API FPolyhedron
{
public:
    /// Polygon faces.
    TVector<TVector<TVector3F> > _faces;
    
    /// Construct empty.
    FPolyhedron();
    /// Copy-construct.
    FPolyhedron(const FPolyhedron& polyhedron);
    /// Construct from a list of faces.
    FPolyhedron(const TVector<TVector<TVector3F> >& faces);
    /// Construct from a bounding box.
    FPolyhedron(const TBoundingBoxF& box);
    /// Construct from a frustum.
    FPolyhedron(const FFrustum& frustum);
    /// Destruct.
    ~FPolyhedron();
    
    /// Define from a bounding box.
    void Define(const TBoundingBoxF& box);
    /// Define from a frustum.
    void Define(const FFrustum& frustum);
    /// Add a triangle face.
    void AddFace(const TVector3F& v0, const TVector3F& v1, const TVector3F& v2);
    /// Add a quadrilateral face.
    void AddFace(const TVector3F& v0, const TVector3F& v1, const TVector3F& v2, const TVector3F& v3);
    /// Add an arbitrary face.
    void AddFace(const TVector<TVector3F>& face);
    /// Clip with a plane using supplied work vectors. When clipping with several planes in a succession these can be the same to avoid repeated dynamic memory allocation.
    void Clip(const FPlane& plane, TVector<TVector3F>& outFace, TVector<TVector3F>& clippedVertices);
    /// Clip with a plane.
    void Clip(const FPlane& plane);
    /// Clip with a bounding box.
    void Clip(const TBoundingBoxF& box);
    /// Clip with a frustum.
    void Clip(const FFrustum& box);
    /// Clear all faces.
    void Clear();
    /// Transform with a 3x3 matrix.
    void Transform(const TMatrix3x3F& transform);
    /// Transform with a 3x4 matrix.
    void Transform(const TMatrix3x4F& transform);
    
    /// Return transformed with a 3x3 matrix.
    FPolyhedron Transformed(const TMatrix3x3F& transform) const;
    /// Return transformed with a 3x4 matrix.
    FPolyhedron Transformed(const TMatrix3x4F& transform) const;
    /// Return whether has no faces.
    bool IsEmpty() const { return _faces.IsEmpty(); }
    
private:
    /// Set a triangle face by index.
    void SetFace(size_t index, const TVector3F& v0, const TVector3F& v1, const TVector3F& v2);
    /// Set a quadrilateral face by index.
    void SetFace(size_t index, const TVector3F& v0, const TVector3F& v1, const TVector3F& v2, const TVector3F& v3);
};

}
