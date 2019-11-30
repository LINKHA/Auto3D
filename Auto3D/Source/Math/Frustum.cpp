#include "Frustum.h"

#include "Debug/DebugNew.h"

namespace Auto3D
{

inline TVector3F ClipEdgeZ(const TVector3F& v0, const TVector3F& v1, float clipZ)
{
    return TVector3F(
        v1._x + (v0._x - v1._x) * ((clipZ - v1._z) / (v0._z - v1._z)),
        v1._y + (v0._y - v1._y) * ((clipZ - v1._z) / (v0._z - v1._z)),
        clipZ
    );
}

void ProjectAndMergeEdge(TVector3F v0, TVector3F v1, TRectF& rect, const TMatrix4x4F& projection)
{
    // Check if both vertices behind near plane
    if (v0._z < M_EPSILON && v1._z < M_EPSILON)
        return;
    
    // Check if need to clip one of the vertices
    if (v1._z < M_EPSILON)
        v1 = ClipEdgeZ(v1, v0, M_EPSILON);
    else if (v0._z < M_EPSILON)
        v0 = ClipEdgeZ(v0, v1, M_EPSILON);
    
    // Project, perspective divide and merge
    TVector3F tV0(projection * v0);
    TVector3F tV1(projection * v1);
    rect.Merge(TVector2F(tV0._x, tV0._y));
    rect.Merge(TVector2F(tV1._x, tV1._y));
}

FFrustum::FFrustum()
{
    for (size_t i = 0; i < NUM_FRUSTUM_VERTICES; ++i)
        _vertices[i] = TVector3F::ZERO;

    UpdatePlanes();
}

FFrustum::FFrustum(const FFrustum& frustum)
{
    *this = frustum;
}

FFrustum& FFrustum::operator = (const FFrustum& rhs)
{
    for (size_t i = 0; i < NUM_FRUSTUM_PLANES; ++i)
        _planes[i] = rhs._planes[i];
    for (size_t i = 0; i < NUM_FRUSTUM_VERTICES; ++i)
        _vertices[i] = rhs._vertices[i];
    
    return *this;
}

void FFrustum::Define(float fov, float aspectRatio, float zoom, float nearZ, float farZ, const TMatrix3x4F& transform)
{
    nearZ = Max(nearZ, 0.0f);
    farZ = Max(farZ, nearZ);
    float halfViewSize = tanf(fov * M_DEGTORAD_2) / zoom;
    TVector3F near, far;
    
    near._z = nearZ;
    near._y = near._z * halfViewSize;
    near._x = near._y * aspectRatio;
    far._z = farZ;
    far._y = far._z * halfViewSize;
    far._x = far._y * aspectRatio;
    
    Define(near, far, transform);
}

void FFrustum::Define(const TVector3F& near, const TVector3F& far, const TMatrix3x4F& transform)
{
    _vertices[0] = transform * near;
    _vertices[1] = transform * TVector3F(near._x, -near._y, near._z);
    _vertices[2] = transform * TVector3F(-near._x, -near._y, near._z);
    _vertices[3] = transform * TVector3F(-near._x, near._y, near._z);
    _vertices[4] = transform * far;
    _vertices[5] = transform * TVector3F(far._x, -far._y, far._z);
    _vertices[6] = transform * TVector3F(-far._x, -far._y, far._z);
    _vertices[7] = transform * TVector3F(-far._x, far._y, far._z);
    
    UpdatePlanes();
}

void FFrustum::Define(const TBoundingBoxF& box, const TMatrix3x4F& transform)
{
    _vertices[0] = transform * TVector3F(box._max._x, box._max._y, box._min._z);
    _vertices[1] = transform * TVector3F(box._max._x, box._min._y, box._min._z);
    _vertices[2] = transform * TVector3F(box._min._x, box._min._y, box._min._z);
    _vertices[3] = transform * TVector3F(box._min._x, box._max._y, box._min._z);
    _vertices[4] = transform * TVector3F(box._max._x, box._max._y, box._max._z);
    _vertices[5] = transform * TVector3F(box._max._x, box._min._y, box._max._z);
    _vertices[6] = transform * TVector3F(box._min._x, box._min._y, box._max._z);
    _vertices[7] = transform * TVector3F(box._min._x, box._max._y, box._max._z);
    
    UpdatePlanes();
}

void FFrustum::DefineOrtho(float orthoSize, float aspectRatio, float zoom, float nearZ, float farZ, const TMatrix3x4F& transform)
{
    nearZ = Max(nearZ, 0.0f);
    farZ = Max(farZ, nearZ);
    float halfViewSize = orthoSize * 0.5f / zoom;
    TVector3F near, far;
    
    near._z = nearZ;
    far._z = farZ;
    far._y = near._y = halfViewSize;
    far._x = near._x = near._y * aspectRatio;
    
    Define(near, far, transform);
}

void FFrustum::Transform(const TMatrix3x3F& transform)
{
    for (size_t i = 0; i < NUM_FRUSTUM_VERTICES; ++i)
        _vertices[i] = transform * _vertices[i];
    
    UpdatePlanes();
}

void FFrustum::Transform(const TMatrix3x4F& transform)
{
    for (size_t i = 0; i < NUM_FRUSTUM_VERTICES; ++i)
        _vertices[i] = transform * _vertices[i];
    
    UpdatePlanes();
}

FFrustum FFrustum::Transformed(const TMatrix3x3F& transform) const
{
    FFrustum transformed;
    for (size_t i = 0; i < NUM_FRUSTUM_VERTICES; ++i)
        transformed._vertices[i] = transform * _vertices[i];
    
    transformed.UpdatePlanes();
    return transformed;
}

FFrustum FFrustum::Transformed(const TMatrix3x4F& transform) const
{
    FFrustum transformed;
    for (size_t i = 0; i < NUM_FRUSTUM_VERTICES; ++i)
        transformed._vertices[i] = transform * _vertices[i];
    
    transformed.UpdatePlanes();
    return transformed;
}

TRectF FFrustum::Projected(const TMatrix4x4F& projection) const
{
    TRectF rect;
    
    ProjectAndMergeEdge(_vertices[0], _vertices[4], rect, projection);
    ProjectAndMergeEdge(_vertices[1], _vertices[5], rect, projection);
    ProjectAndMergeEdge(_vertices[2], _vertices[6], rect, projection);
    ProjectAndMergeEdge(_vertices[3], _vertices[7], rect, projection);
    ProjectAndMergeEdge(_vertices[4], _vertices[5], rect, projection);
    ProjectAndMergeEdge(_vertices[5], _vertices[6], rect, projection);
    ProjectAndMergeEdge(_vertices[6], _vertices[7], rect, projection);
    ProjectAndMergeEdge(_vertices[7], _vertices[4], rect, projection);
    
    return rect;
}

void FFrustum::UpdatePlanes()
{
    _planes[EFrustumPlane::NEAR].Define(_vertices[2], _vertices[1], _vertices[0]);
    _planes[EFrustumPlane::LEFT].Define(_vertices[3], _vertices[7], _vertices[6]);
    _planes[EFrustumPlane::RIGHT].Define(_vertices[1], _vertices[5], _vertices[4]);
    _planes[EFrustumPlane::UP].Define(_vertices[0], _vertices[4], _vertices[7]);
    _planes[EFrustumPlane::DOWN].Define(_vertices[6], _vertices[5], _vertices[1]);
    _planes[EFrustumPlane::FAR].Define(_vertices[5], _vertices[6], _vertices[7]);

    // Check if we ended up with inverted planes (reflected transform) and flip in that case
    if (_planes[EFrustumPlane::NEAR].Distance(_vertices[5]) < 0.0f)
    {
        for (size_t i = 0; i < NUM_FRUSTUM_PLANES; ++i)
        {
            _planes[i]._normal = -_planes[i]._normal;
            _planes[i]._d = -_planes[i]._d;
        }
    }
}

}
