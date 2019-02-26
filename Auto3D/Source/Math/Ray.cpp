#include "BoundingBox.h"
#include "Frustum.h"
#include "Plane.h"
#include "Ray.h"

#include "../Debug/DebugNew.h"

namespace Auto3D
{

Vector3F Ray::ClosestPoint(const Ray& ray) const
{
    Vector3F p13 = _origin - ray._origin;
    Vector3F p43 = ray._direction;
    Vector3F p21 = _direction;
    
    float d1343 = p13.DotProduct(p43);
    float d4321 = p43.DotProduct(p21);
    float d1321 = p13.DotProduct(p21);
    float d4343 = p43.DotProduct(p43);
    float d2121 = p21.DotProduct(p21);
    
    float d = d2121 * d4343 - d4321 * d4321;
    if (Abs(d) < M_EPSILON)
        return _origin;
    float n = d1343 * d4321 - d1321 * d4343;
    float a = n / d;
    
    return _origin + a * _direction;
}

float Ray::HitDistance(const Plane& plane) const
{
    float d = plane._normal.DotProduct(_direction);
    if (Abs(d) >= M_EPSILON)
    {
        float t = -(plane._normal.DotProduct(_origin) + plane._d) / d;
        if (t >= 0.0f)
            return t;
        else
            return M_INFINITY;
    }
    else
        return M_INFINITY;
}

float Ray::HitDistance(const BoundingBox& box) const
{
    // Check for ray origin being inside the box
    if (box.IsInside(_origin))
        return 0.0f;
    
    float dist = M_INFINITY;
    
    // Check for intersecting in the X-direction
    if (_origin._x < box._min._x && _direction._x > 0.0f)
    {
        float x = (box._min._x - _origin._x) / _direction._x;
        if (x < dist)
        {
            Vector3F point = _origin + x * _direction;
            if (point._y >= box._min._y && point._y <= box._max._y && point._z >= box._min._z && point._z <= box._max._z)
                dist = x;
        }
    }
    if (_origin._x > box._max._x && _direction._x < 0.0f)
    {
        float x = (box._max._x - _origin._x) / _direction._x;
        if (x < dist)
        {
            Vector3F point = _origin + x * _direction;
            if (point._y >= box._min._y && point._y <= box._max._y && point._z >= box._min._z && point._z <= box._max._z)
                dist = x;
        }
    }
    // Check for intersecting in the Y-direction
    if (_origin._y < box._min._y && _direction._y > 0.0f)
    {
        float x = (box._min._y - _origin._y) / _direction._y;
        if (x < dist)
        {
            Vector3F point = _origin + x * _direction;
            if (point._x >= box._min._x && point._x <= box._max._x && point._z >= box._min._z && point._z <= box._max._z)
                dist = x;
        }
    }
    if (_origin._y > box._max._y && _direction._y < 0.0f)
    {
        float x = (box._max._y - _origin._y) / _direction._y;
        if (x < dist)
        {
            Vector3F point = _origin + x * _direction;
            if (point._x >= box._min._x && point._x <= box._max._x && point._z >= box._min._z && point._z <= box._max._z)
                dist = x;
        }
    }
    // Check for intersecting in the Z-direction
    if (_origin._z < box._min._z && _direction._z > 0.0f)
    {
        float x = (box._min._z - _origin._z) / _direction._z;
        if (x < dist)
        {
            Vector3F point = _origin + x * _direction;
            if (point._x >= box._min._x && point._x <= box._max._x && point._y >= box._min._y && point._y <= box._max._y)
                dist = x;
        }
    }
    if (_origin._z > box._max._z && _direction._z < 0.0f)
    {
        float x = (box._max._z - _origin._z) / _direction._z;
        if (x < dist)
        {
            Vector3F point = _origin + x * _direction;
            if (point._x >= box._min._x && point._x <= box._max._x && point._y >= box._min._y && point._y <= box._max._y)
                dist = x;
        }
    }
    
    return dist;
}

float Ray::HitDistance(const Frustum& frustum, bool solidInside) const
{
    float maxOutside = 0.0f;
    float minInside = M_INFINITY;
    bool allInside = true;
    
    for (size_t i = 0; i < NUM_FRUSTUM_PLANES; ++i)
    {
        const Plane& plane = frustum._planes[i];
        float distance = HitDistance(frustum._planes[i]);
        
        if (plane.Distance(_origin) < 0.0f)
        {
            maxOutside = Max(maxOutside, distance);
            allInside = false;
        }
        else
            minInside = Min(minInside, distance);
    }
    
    if (allInside)
        return solidInside ? 0.0f : minInside;
    else if (maxOutside <= minInside)
        return maxOutside;
    else
        return M_INFINITY;
}

float Ray::HitDistance(const Sphere& sphere) const
{
    Vector3F centeredOrigin = _origin - sphere._center;
    float squaredRadius = sphere._radius * sphere._radius;
    
    // Check if ray originates inside the sphere
    if (centeredOrigin.LengthSquared() <= squaredRadius)
        return 0.0f;
    
    // Calculate intersection by quadratic equation
    float a = _direction.DotProduct(_direction);
    float b = 2.0f * centeredOrigin.DotProduct(_direction);
    float c = centeredOrigin.DotProduct(centeredOrigin) - squaredRadius;
    float d = b * b - 4.0f * a * c;
    
    // No solution
    if (d < 0.0f)
        return M_INFINITY;
    
    // Get the nearer solution
    float dSqrt = sqrtf(d);
    float dist = (-b - dSqrt) / (2.0f * a);
    if (dist >= 0.0f)
        return dist;
    else
        return (-b + dSqrt) / (2.0f * a);
}

float Ray::HitDistance(const Vector3F& v0, const Vector3F& v1, const Vector3F& v2) const
{
    return HitDistance(v0, v1, v2, 0);
}

float Ray::HitDistance(const Vector3F& v0, const Vector3F& v1, const Vector3F& v2, Vector3F* outNormal) const
{
    // Based on Fast, Minimum Storage Ray/Triangle Intersection by M�ller & Trumbore
    // http://www.graphics.cornell.edu/pubs/1997/MT97.pdf
    // Calculate edge vectors
    Vector3F edge1(v1 - v0);
    Vector3F edge2(v2 - v0);
    
    // Calculate determinant & check backfacing
    Vector3F p(_direction.CrossProduct(edge2));
    float det = edge1.DotProduct(p);
    if (det >= M_EPSILON)
    {
        // Calculate u & v parameters and test
        Vector3F t(_origin - v0);
        float u = t.DotProduct(p);
        if (u >= 0.0f && u <= det)
        {
            Vector3F q(t.CrossProduct(edge1));
            float v = _direction.DotProduct(q);
            if (v >= 0.0f && u + v <= det)
            {
                float distance = edge2.DotProduct(q) / det;
                if (distance >= 0.0f)
                {
                    // There is an intersection, so calculate distance & optional normal
                    if (outNormal)
                        *outNormal = edge1.CrossProduct(edge2);
                    
                    return distance;
                }
            }
        }
    }
    
    return M_INFINITY;
}

float Ray::HitDistance(const void* vertexData, size_t vertexSize, size_t vertexStart, size_t vertexCount, Vector3F* outNormal) const
{
    float nearest = M_INFINITY;
    const unsigned char* vertices = ((const unsigned char*)vertexData) + vertexStart * vertexSize;
    size_t index = 0;
    
    while (index + 2 < vertexCount)
    {
        const Vector3F& v0 = *((const Vector3F*)(&vertices[index * vertexSize]));
        const Vector3F& v1 = *((const Vector3F*)(&vertices[(index + 1) * vertexSize]));
        const Vector3F& v2 = *((const Vector3F*)(&vertices[(index + 2) * vertexSize]));
        nearest = Min(nearest, HitDistance(v0, v1, v2, outNormal));
        index += 3;
    }
    
    return nearest;
}

float Ray::HitDistance(const void* vertexData, size_t vertexSize, const void* indexData, size_t indexSize,
    size_t indexStart, size_t indexCount, Vector3F* outNormal) const
{
    float nearest = M_INFINITY;
    const unsigned char* vertices = (const unsigned char*)vertexData;
    
    // 16-bit indices
    if (indexSize == sizeof(unsigned short))
    {
        const unsigned short* indices = ((const unsigned short*)indexData) + indexStart;
        const unsigned short* indicesEnd = indices + indexCount;
        
        while (indices < indicesEnd)
        {
            const Vector3F& v0 = *((const Vector3F*)(&vertices[indices[0] * vertexSize]));
            const Vector3F& v1 = *((const Vector3F*)(&vertices[indices[1] * vertexSize]));
            const Vector3F& v2 = *((const Vector3F*)(&vertices[indices[2] * vertexSize]));
            nearest = Min(nearest, HitDistance(v0, v1, v2, outNormal));
            indices += 3;
        }
    }
    // 32-bit indices
    else
    {
        const size_t* indices = ((const size_t*)indexData) + indexStart;
        const size_t* indicesEnd = indices + indexCount;
        
        while (indices < indicesEnd)
        {
            const Vector3F& v0 = *((const Vector3F*)(&vertices[indices[0] * vertexSize]));
            const Vector3F& v1 = *((const Vector3F*)(&vertices[indices[1] * vertexSize]));
            const Vector3F& v2 = *((const Vector3F*)(&vertices[indices[2] * vertexSize]));
            nearest = Min(nearest, HitDistance(v0, v1, v2, outNormal));
            indices += 3;
        }
    }
    
    return nearest;
}

bool Ray::InsideGeometry(const void* vertexData, size_t vertexSize, size_t vertexStart, size_t vertexCount) const
{
    float currentFrontFace = M_INFINITY;
    float currentBackFace = M_INFINITY;
    const unsigned char* vertices = ((const unsigned char*)vertexData) + vertexStart * vertexSize;
    size_t index = 0;
    
    while (index + 2 < vertexCount)
    {
        const Vector3F& v0 = *((const Vector3F*)(&vertices[index * vertexSize]));
        const Vector3F& v1 = *((const Vector3F*)(&vertices[(index + 1) * vertexSize]));
        const Vector3F& v2 = *((const Vector3F*)(&vertices[(index + 2) * vertexSize]));
        float frontFaceDistance = HitDistance(v0, v1, v2);
        float backFaceDistance = HitDistance(v2, v1, v0);
        currentFrontFace = Min(frontFaceDistance > 0.0f ? frontFaceDistance : M_INFINITY, currentFrontFace);
        // A backwards face is just a regular one, with the vertices in the opposite order. This essentially checks backfaces by
        // checking reversed frontfaces
        currentBackFace = Min(backFaceDistance > 0.0f ? backFaceDistance : M_INFINITY, currentBackFace);
        index += 3;
    }
    
    // If the closest face is a backface, that means that the ray originates from the inside of the geometry
    // NOTE: there may be cases where both are equal, as in, no collision to either. This is prevented in the most likely case
    // (ray doesnt hit either) by this conditional
    if (currentFrontFace != M_INFINITY || currentBackFace != M_INFINITY)
        return currentBackFace < currentFrontFace;
    
    // It is still possible for two triangles to be equally distant from the triangle, however, this is extremely unlikely.
    // As such, it is safe to assume they are not
    return false;
}

bool Ray::InsideGeometry(const void* vertexData, size_t vertexSize, const void* indexData, size_t indexSize,
    size_t indexStart, size_t indexCount) const
{
    float currentFrontFace = M_INFINITY;
    float currentBackFace = M_INFINITY;
    const unsigned char* vertices = (const unsigned char*)vertexData;
    
    // 16-bit indices
    if (indexSize == sizeof(unsigned short))
    {
        const unsigned short* indices = ((const unsigned short*)indexData) + indexStart;
        const unsigned short* indicesEnd = indices + indexCount;
        
        while (indices < indicesEnd)
        {
            const Vector3F& v0 = *((const Vector3F*)(&vertices[indices[0] * vertexSize]));
            const Vector3F& v1 = *((const Vector3F*)(&vertices[indices[1] * vertexSize]));
            const Vector3F& v2 = *((const Vector3F*)(&vertices[indices[2] * vertexSize]));
            float frontFaceDistance = HitDistance(v0, v1, v2);
            float backFaceDistance = HitDistance(v2, v1, v0);
            currentFrontFace = Min(frontFaceDistance > 0.0f ? frontFaceDistance : M_INFINITY, currentFrontFace);
            // A backwards face is just a regular one, with the vertices in the opposite order. This essentially checks backfaces by
            // checking reversed frontfaces
            currentBackFace = Min(backFaceDistance > 0.0f ? backFaceDistance : M_INFINITY, currentBackFace);
            indices += 3;
        }
    }
    // 32-bit indices
    else
    {
        const size_t* indices = ((const size_t*)indexData) + indexStart;
        const size_t* indicesEnd = indices + indexCount;
        
        while (indices < indicesEnd)
        {
            const Vector3F& v0 = *((const Vector3F*)(&vertices[indices[0] * vertexSize]));
            const Vector3F& v1 = *((const Vector3F*)(&vertices[indices[1] * vertexSize]));
            const Vector3F& v2 = *((const Vector3F*)(&vertices[indices[2] * vertexSize]));
            float frontFaceDistance = HitDistance(v0, v1, v2);
            float backFaceDistance = HitDistance(v2, v1, v0);
            currentFrontFace = Min(frontFaceDistance > 0.0f ? frontFaceDistance : M_INFINITY, currentFrontFace);
            // A backwards face is just a regular one, with the vertices in the opposite order. This essentially checks backfaces by
            // checking reversed frontfaces
            currentBackFace = Min(backFaceDistance > 0.0f ? backFaceDistance : M_INFINITY, currentBackFace);
            indices += 3; 
        }
    }
    
    // If the closest face is a backface, that means that the ray originates from the inside of the geometry
    // NOTE: there may be cases where both are equal, as in, no collision to either. This is prevented in the most likely case
    // (ray doesnt hit either) by this conditional
    if (currentFrontFace != M_INFINITY || currentBackFace != M_INFINITY)
        return currentBackFace < currentFrontFace;
    
    // It is still possible for two triangles to be equally distant from the triangle, however, this is extremely unlikely.
    // As such, it is safe to assume they are not
    return false;
}

Ray Ray::Transformed(const Matrix3x4& transform) const
{
    Ray ret;
    ret._origin = transform * _origin;
    ret._direction = transform * Vector4F(_direction, 0.0f);
    return ret;
}

}
