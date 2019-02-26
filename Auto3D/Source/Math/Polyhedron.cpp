#include "Frustum.h"
#include "Polyhedron.h"

#include "../Debug/DebugNew.h"

namespace Auto3D
{

Polyhedron::Polyhedron()
{
}

Polyhedron::Polyhedron(const Polyhedron& polyhedron) :
    _faces(polyhedron._faces)
{
}

Polyhedron::Polyhedron(const Vector<Vector<Vector3F> >& faces) :
    _faces(faces)
{
}

Polyhedron::Polyhedron(const BoundingBox& box)
{
    Define(box);
}

Polyhedron::Polyhedron(const Frustum& frustum)
{
    Define(frustum);
}

Polyhedron::~Polyhedron()
{
}

void Polyhedron::Define(const BoundingBox& box)
{
    Vector3F vertices[8];
    vertices[0] = box._min;
    vertices[1] = Vector3F(box._max._x, box._min._y, box._min._z);
    vertices[2] = Vector3F(box._min._x, box._max._y, box._min._z);
    vertices[3] = Vector3F(box._max._x, box._max._y, box._min._z);
    vertices[4] = Vector3F(box._min._x, box._min._y, box._max._z);
    vertices[5] = Vector3F(box._max._x, box._min._y, box._max._z);
    vertices[6] = Vector3F(box._min._x, box._max._y, box._max._z);
    vertices[7] = box._max;
    
    _faces.Resize(6);
    SetFace(0, vertices[3], vertices[7], vertices[5], vertices[1]);
    SetFace(1, vertices[6], vertices[2], vertices[0], vertices[4]);
    SetFace(2, vertices[6], vertices[7], vertices[3], vertices[2]);
    SetFace(3, vertices[1], vertices[5], vertices[4], vertices[0]);
    SetFace(4, vertices[7], vertices[6], vertices[4], vertices[5]);
    SetFace(5, vertices[2], vertices[3], vertices[1], vertices[0]);
}

void Polyhedron::Define(const Frustum& frustum)
{
    const Vector3F* vertices = frustum._vertices;
    
    _faces.Resize(6);
    SetFace(0, vertices[0], vertices[4], vertices[5], vertices[1]);
    SetFace(1, vertices[7], vertices[3], vertices[2], vertices[6]);
    SetFace(2, vertices[7], vertices[4], vertices[0], vertices[3]);
    SetFace(3, vertices[1], vertices[5], vertices[6], vertices[2]);
    SetFace(4, vertices[4], vertices[7], vertices[6], vertices[5]);
    SetFace(5, vertices[3], vertices[0], vertices[1], vertices[2]);
}

void Polyhedron::AddFace(const Vector3F& v0, const Vector3F& v1, const Vector3F& v2)
{
    _faces.Resize(_faces.Size() + 1);
    Vector<Vector3F>& face = _faces[_faces.Size() - 1];
    face.Resize(3);
    face[0] = v0;
    face[1] = v1;
    face[2] = v2;
}

void Polyhedron::AddFace(const Vector3F& v0, const Vector3F& v1, const Vector3F& v2, const Vector3F& v3)
{
    _faces.Resize(_faces.Size() + 1);
    Vector<Vector3F>& face = _faces[_faces.Size() - 1];
    face.Resize(4);
    face[0] = v0;
    face[1] = v1;
    face[2] = v2;
    face[3] = v3;
}

void Polyhedron::AddFace(const Vector<Vector3F>& face)
{
    _faces.Push(face);
}

void Polyhedron::Clip(const Plane& plane, Vector<Vector3F>& clippedVertices, Vector<Vector3F>& outFace)
{
    clippedVertices.Clear();
    
    for (size_t i = 0; i < _faces.Size(); ++i)
    {
        Vector<Vector3F>& face = _faces[i];
        Vector3F lastVertex;
        float lastDistance = 0.0f;
        
        outFace.Clear();
        
        for (size_t j = 0; j < face.Size(); ++j)
        {
            float distance = plane.Distance(face[j]);
            if (distance >= 0.0f)
            {
                if (lastDistance < 0.0f)
                {
                    float t = lastDistance / (lastDistance - distance);
                    Vector3F clippedVertex = lastVertex + t * (face[j] - lastVertex);
                    outFace.Push(clippedVertex);
                    clippedVertices.Push(clippedVertex);
                }
                
                outFace.Push(face[j]);
            }
            else
            {
                if (lastDistance >= 0.0f && j != 0)
                {
                    float t = lastDistance / (lastDistance - distance);
                    Vector3F clippedVertex = lastVertex + t * (face[j] - lastVertex);
                    outFace.Push(clippedVertex);
                    clippedVertices.Push(clippedVertex);
                }
            }
            
            lastVertex = face[j];
            lastDistance = distance;
        }
        
        // Recheck the distances of the last and first vertices and add the final clipped vertex if applicable
        float distance = plane.Distance(face[0]);
        if ((lastDistance < 0.0f && distance >= 0.0f) || (lastDistance >= 0.0f && distance < 0.0f))
        {
            float t = lastDistance / (lastDistance - distance);
            Vector3F clippedVertex = lastVertex + t * (face[0] - lastVertex);
            outFace.Push(clippedVertex);
            clippedVertices.Push(clippedVertex);
        }
        
        // Do not keep faces which are less than triangles
        if (outFace.Size() < 3)
            outFace.Clear();
        
        face = outFace;
    }
    
    // Remove empty faces
    for (size_t i = _faces.Size() - 1; i < _faces.Size(); --i)
    {
        if (_faces[i].IsEmpty())
            _faces.Erase(i);
    }
    
    // Create a new face from the clipped vertices. First remove duplicates
    for (size_t i = 0; i < clippedVertices.Size(); ++i)
    {
        for (size_t j = clippedVertices.Size() - 1; j > i; --j)
        {
            if (clippedVertices[j].Equals(clippedVertices[i]))
                clippedVertices.Erase(j);
        }
    }
    
    if (clippedVertices.Size() > 3)
    {
        outFace.Clear();
        
        // Start with the first vertex
        outFace.Push(clippedVertices.Front());
        clippedVertices.Erase(0);
        
        while (!clippedVertices.IsEmpty())
        {
            // Then add the vertex which is closest to the last added
            const Vector3F& lastAdded = outFace.Back();
            float bestDistance = M_INFINITY;
            size_t bestIndex = 0;
            
            for (size_t i = 0; i < clippedVertices.Size(); ++i)
            {
                float distance = (clippedVertices[i] - lastAdded).LengthSquared();
                if (distance < bestDistance)
                {
                    bestDistance = distance;
                    bestIndex = i;
                }
            }
            
            outFace.Push(clippedVertices[bestIndex]);
            clippedVertices.Erase(bestIndex);
        }
        
        _faces.Push(outFace);
    }
}

void Polyhedron::Clip(const Plane& plane)
{
    Vector<Vector3F> clippedVertices;
    Vector<Vector3F> outFace;
    
    Clip(plane, clippedVertices, outFace);
}

void Polyhedron::Clip(const Frustum& frustum)
{
    Vector<Vector3F> clippedVertices;
    Vector<Vector3F> outFace;
    
    for (size_t i = 0; i < NUM_FRUSTUM_PLANES; ++i)
        Clip(frustum._planes[i], clippedVertices, outFace);
}

void Polyhedron::Clip(const BoundingBox& box)
{
    Vector<Vector3F> clippedVertices;
    Vector<Vector3F> outFace;
    
    Vector3F vertices[8];
    vertices[0] = box._min;
    vertices[1] = Vector3F(box._max._x, box._min._y, box._min._z);
    vertices[2] = Vector3F(box._min._x, box._max._y, box._min._z);
    vertices[3] = Vector3F(box._max._x, box._max._y, box._min._z);
    vertices[4] = Vector3F(box._min._x, box._min._y, box._max._z);
    vertices[5] = Vector3F(box._max._x, box._min._y, box._max._z);
    vertices[6] = Vector3F(box._min._x, box._max._y, box._max._z);
    vertices[7] = box._max;
    
    Clip(Plane(vertices[5], vertices[7], vertices[3]), clippedVertices, outFace);
    Clip(Plane(vertices[0], vertices[2], vertices[6]), clippedVertices, outFace);
    Clip(Plane(vertices[3], vertices[7], vertices[6]), clippedVertices, outFace);
    Clip(Plane(vertices[4], vertices[5], vertices[1]), clippedVertices, outFace);
    Clip(Plane(vertices[4], vertices[6], vertices[7]), clippedVertices, outFace);
    Clip(Plane(vertices[1], vertices[3], vertices[2]), clippedVertices, outFace);
}

void Polyhedron::Clear()
{
    _faces.Clear();
}

void Polyhedron::Transform(const Matrix3& transform)
{
    for (size_t i = 0; i < _faces.Size(); ++i)
    {
        Vector<Vector3F>& face = _faces[i];
        for (size_t j = 0; j < face.Size(); ++j)
            face[j] = transform * face[j];
    }
}

void Polyhedron::Transform(const Matrix3x4& transform)
{
    for (size_t i = 0; i < _faces.Size(); ++i)
    {
        Vector<Vector3F>& face = _faces[i];
        for (size_t j = 0; j < face.Size(); ++j)
            face[j] = transform * face[j];
    }
}

Polyhedron Polyhedron::Transformed(const Matrix3& transform) const
{
    Polyhedron ret;
    ret._faces.Resize(_faces.Size());
    
    for (size_t i = 0; i < _faces.Size(); ++i)
    {
        const Vector<Vector3F>& face = _faces[i];
        Vector<Vector3F>& newFace = ret._faces[i];
        newFace.Resize(face.Size());
        
        for (size_t j = 0; j < face.Size(); ++j)
            newFace[j] = transform * face[j];
    }
    
    return ret;
}

Polyhedron Polyhedron::Transformed(const Matrix3x4& transform) const
{
    Polyhedron ret;
    ret._faces.Resize(_faces.Size());
    
    for (size_t i = 0; i < _faces.Size(); ++i)
    {
        const Vector<Vector3F>& face = _faces[i];
        Vector<Vector3F>& newFace = ret._faces[i];
        newFace.Resize(face.Size());
        
        for (size_t j = 0; j < face.Size(); ++j)
            newFace[j] = transform * face[j];
    }
    
    return ret;
}

void Polyhedron::SetFace(size_t index, const Vector3F& v0, const Vector3F& v1, const Vector3F& v2)
{
    Vector<Vector3F>& face = _faces[index];
    face.Resize(3);
    face[0] = v0;
    face[1] = v1;
    face[2] = v2;
}

void Polyhedron::SetFace(size_t index, const Vector3F& v0, const Vector3F& v1, const Vector3F& v2, const Vector3F& v3)
{
    Vector<Vector3F>& face = _faces[index];
    face.Resize(4);
    face[0] = v0;
    face[1] = v1;
    face[2] = v2;
    face[3] = v3;
}

}
