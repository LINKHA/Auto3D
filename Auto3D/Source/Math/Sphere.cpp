#include "../Container/String.h"
#include "BoundingBox.h"
#include "Frustum.h"
#include "Polyhedron.h"
#include "Sphere.h"

#include "../Debug/DebugNew.h"

namespace Auto3D
{

void Sphere::Define(const Vector3F* vertices, size_t count)
{
    Undefine();
    Merge(vertices, count);
}

void Sphere::Define(const BoundingBoxF& box)
{
    const Vector3F& min = box._min;
    const Vector3F& max = box._max;
    
    Undefine();
    Merge(min);
    Merge(Vector3F(max._x, min._y, min._z));
    Merge(Vector3F(min._x, max._y, min._z));
    Merge(Vector3F(max._x, max._y, min._z));
    Merge(Vector3F(min._x, min._y, max._z));
    Merge(Vector3F(max._x, min._y, max._z));
    Merge(Vector3F(min._x, max._y, max._z));
    Merge(max);
}

void Sphere::Define(const Frustum& frustum)
{
    Define(frustum._vertices, NUM_FRUSTUM_VERTICES);
}

void Sphere::Define(const Polyhedron& poly)
{
    Undefine();
    Merge(poly);
}

void Sphere::Merge(const Vector3F* vertices, size_t count)
{
    while (count--)
        Merge(*vertices++);
}

void Sphere::Merge(const BoundingBoxF& box)
{
    const Vector3F& min = box._min;
    const Vector3F& max = box._max;
    
    Merge(min);
    Merge(Vector3F(max._x, min._y, min._z));
    Merge(Vector3F(min._x, max._y, min._z));
    Merge(Vector3F(max._x, max._y, min._z));
    Merge(Vector3F(min._x, min._y, max._z));
    Merge(Vector3F(max._x, min._y, max._z));
    Merge(Vector3F(min._x, max._y, max._z));
    Merge(max);
}

void Sphere::Merge(const Frustum& frustum)
{
    const Vector3F* vertices = frustum._vertices;
    Merge(vertices, NUM_FRUSTUM_VERTICES);
}

void Sphere::Merge(const Polyhedron& poly)
{
    for (size_t i = 0; i < poly._faces.Size(); ++i)
    {
        const Vector<Vector3F>& face = poly._faces[i];
        if (!face.IsEmpty())
            Merge(&face[0], face.Size());
    }
}

void Sphere::Merge(const Sphere& sphere)
{
    // If undefined, set initial dimensions
    if (!IsDefined())
    {
        _center = sphere._center;
        _radius = sphere._radius;
        return;
    }
    
    Vector3F offset = sphere._center - _center;
    float dist = offset.Length();
    
    // If sphere fits inside, do nothing
    if (dist + sphere._radius < _radius)
        return;
    
    // If we fit inside the other sphere, become it
    if (dist + _radius < sphere._radius)
    {
        _center = sphere._center;
        _radius = sphere._radius;
    }
    else
    {
        Vector3F normalizedOffset = offset / dist;
        
        Vector3F min = _center - _radius * normalizedOffset;
        Vector3F max = sphere._center + sphere._radius * normalizedOffset;
        _center = (min + max) * 0.5f;
        _radius = (max - _center).Length();
    }
}

Intersection Sphere::IsInside(const BoundingBoxF& box) const
{
    float radiusSquared = _radius * _radius;
    float distSquared = 0;
    float temp;
    Vector3F min = box._min;
    Vector3F max = box._max;
    
    if (_center._x < min._x)
    {
        temp = _center._x - min._x;
        distSquared += temp * temp;
    }
    else if (_center._x > max._x)
    {
        temp = _center._x - max._x;
        distSquared += temp * temp;
    }
    if (_center._y < min._y)
    {
        temp = _center._y - min._y;
        distSquared += temp * temp;
    }
    else if (_center._y > max._y)
    {
        temp = _center._y - max._y;
        distSquared += temp * temp;
    }
    if (_center._z < min._z)
    {
        temp = _center._z - min._z;
        distSquared += temp * temp;
    }
    else if (_center._z > max._z)
    {
        temp = _center._z - max._z;
        distSquared += temp * temp;
    }
    
    if (distSquared >= radiusSquared)
        return OUTSIDE;
    
    min -= _center;
    max -= _center;
    
    Vector3F tempVec = min; // - - -
    if (tempVec.LengthSquared() >= radiusSquared)
        return INTERSECTS;
    tempVec._x = max._x; // + - -
    if (tempVec.LengthSquared() >= radiusSquared)
        return INTERSECTS;
    tempVec._y = max._y; // + + -
    if (tempVec.LengthSquared() >= radiusSquared)
        return INTERSECTS;
    tempVec._x = min._x; // - + -
    if (tempVec.LengthSquared() >= radiusSquared)
        return INTERSECTS;
    tempVec._z = max._z; // - + +
    if (tempVec.LengthSquared() >= radiusSquared)
        return INTERSECTS;
    tempVec._y = min._y; // - - +
    if (tempVec.LengthSquared() >= radiusSquared)
        return INTERSECTS;
    tempVec._x = max._x; // + - +
    if (tempVec.LengthSquared() >= radiusSquared)
        return INTERSECTS;
    tempVec._y = max._y; // + + +
    if (tempVec.LengthSquared() >= radiusSquared)
        return INTERSECTS;
    
    return INSIDE;
}

Intersection Sphere::IsInsideFast(const BoundingBoxF& box) const
{
    float radiusSquared = _radius * _radius;
    float distSquared = 0;
    float temp;
    Vector3F min = box._min;
    Vector3F max = box._max;
    
    if (_center._x < min._x)
    {
        temp = _center._x - min._x;
        distSquared += temp * temp;
    }
    else if (_center._x > max._x)
    {
        temp = _center._x - max._x;
        distSquared += temp * temp;
    }
    if (_center._y < min._y)
    {
        temp = _center._y - min._y;
        distSquared += temp * temp;
    }
    else if (_center._y > max._y)
    {
        temp = _center._y - max._y;
        distSquared += temp * temp;
    }
    if (_center._z < min._z)
    {
        temp = _center._z - min._z;
        distSquared += temp * temp;
    }
    else if (_center._z > max._z)
    {
        temp = _center._z - max._z;
        distSquared += temp * temp;
    }
    
    if (distSquared >= radiusSquared)
        return OUTSIDE;
    else
        return INSIDE;
}

}
