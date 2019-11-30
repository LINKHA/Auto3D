#include "Frustum.h"
#include "Polyhedron.h"

#include "Debug/DebugNew.h"

namespace Auto3D
{

FPolyhedron::FPolyhedron()
{
}

FPolyhedron::FPolyhedron(const FPolyhedron& polyhedron) :
    _faces(polyhedron._faces)
{
}

FPolyhedron::FPolyhedron(const TVector<TVector<TVector3F> >& faces) :
    _faces(faces)
{
}

FPolyhedron::FPolyhedron(const TBoundingBoxF& box)
{
    Define(box);
}

FPolyhedron::FPolyhedron(const FFrustum& frustum)
{
    Define(frustum);
}

FPolyhedron::~FPolyhedron()
{
}

void FPolyhedron::Define(const TBoundingBoxF& box)
{
    TVector3F vertices[8];
    vertices[0] = box._min;
    vertices[1] = TVector3F(box._max._x, box._min._y, box._min._z);
    vertices[2] = TVector3F(box._min._x, box._max._y, box._min._z);
    vertices[3] = TVector3F(box._max._x, box._max._y, box._min._z);
    vertices[4] = TVector3F(box._min._x, box._min._y, box._max._z);
    vertices[5] = TVector3F(box._max._x, box._min._y, box._max._z);
    vertices[6] = TVector3F(box._min._x, box._max._y, box._max._z);
    vertices[7] = box._max;
    
    _faces.Resize(6);
    SetFace(0, vertices[3], vertices[7], vertices[5], vertices[1]);
    SetFace(1, vertices[6], vertices[2], vertices[0], vertices[4]);
    SetFace(2, vertices[6], vertices[7], vertices[3], vertices[2]);
    SetFace(3, vertices[1], vertices[5], vertices[4], vertices[0]);
    SetFace(4, vertices[7], vertices[6], vertices[4], vertices[5]);
    SetFace(5, vertices[2], vertices[3], vertices[1], vertices[0]);
}

void FPolyhedron::Define(const FFrustum& frustum)
{
    const TVector3F* vertices = frustum._vertices;
    
    _faces.Resize(6);
    SetFace(0, vertices[0], vertices[4], vertices[5], vertices[1]);
    SetFace(1, vertices[7], vertices[3], vertices[2], vertices[6]);
    SetFace(2, vertices[7], vertices[4], vertices[0], vertices[3]);
    SetFace(3, vertices[1], vertices[5], vertices[6], vertices[2]);
    SetFace(4, vertices[4], vertices[7], vertices[6], vertices[5]);
    SetFace(5, vertices[3], vertices[0], vertices[1], vertices[2]);
}

void FPolyhedron::AddFace(const TVector3F& v0, const TVector3F& v1, const TVector3F& v2)
{
    _faces.Resize(_faces.Size() + 1);
    TVector<TVector3F>& face = _faces[_faces.Size() - 1];
    face.Resize(3);
    face[0] = v0;
    face[1] = v1;
    face[2] = v2;
}

void FPolyhedron::AddFace(const TVector3F& v0, const TVector3F& v1, const TVector3F& v2, const TVector3F& v3)
{
    _faces.Resize(_faces.Size() + 1);
    TVector<TVector3F>& face = _faces[_faces.Size() - 1];
    face.Resize(4);
    face[0] = v0;
    face[1] = v1;
    face[2] = v2;
    face[3] = v3;
}

void FPolyhedron::AddFace(const TVector<TVector3F>& face)
{
    _faces.Push(face);
}

void FPolyhedron::Clip(const FPlane& plane, TVector<TVector3F>& clippedVertices, TVector<TVector3F>& outFace)
{
    clippedVertices.Clear();
    
    for (size_t i = 0; i < _faces.Size(); ++i)
    {
        TVector<TVector3F>& face = _faces[i];
        TVector3F lastVertex;
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
                    TVector3F clippedVertex = lastVertex + t * (face[j] - lastVertex);
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
                    TVector3F clippedVertex = lastVertex + t * (face[j] - lastVertex);
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
            TVector3F clippedVertex = lastVertex + t * (face[0] - lastVertex);
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
            const TVector3F& lastAdded = outFace.Back();
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

void FPolyhedron::Clip(const FPlane& plane)
{
    TVector<TVector3F> clippedVertices;
    TVector<TVector3F> outFace;
    
    Clip(plane, clippedVertices, outFace);
}

void FPolyhedron::Clip(const FFrustum& frustum)
{
    TVector<TVector3F> clippedVertices;
    TVector<TVector3F> outFace;
    
    for (size_t i = 0; i < NUM_FRUSTUM_PLANES; ++i)
        Clip(frustum._planes[i], clippedVertices, outFace);
}

void FPolyhedron::Clip(const TBoundingBoxF& box)
{
    TVector<TVector3F> clippedVertices;
    TVector<TVector3F> outFace;
    
    TVector3F vertices[8];
    vertices[0] = box._min;
    vertices[1] = TVector3F(box._max._x, box._min._y, box._min._z);
    vertices[2] = TVector3F(box._min._x, box._max._y, box._min._z);
    vertices[3] = TVector3F(box._max._x, box._max._y, box._min._z);
    vertices[4] = TVector3F(box._min._x, box._min._y, box._max._z);
    vertices[5] = TVector3F(box._max._x, box._min._y, box._max._z);
    vertices[6] = TVector3F(box._min._x, box._max._y, box._max._z);
    vertices[7] = box._max;
    
    Clip(FPlane(vertices[5], vertices[7], vertices[3]), clippedVertices, outFace);
    Clip(FPlane(vertices[0], vertices[2], vertices[6]), clippedVertices, outFace);
    Clip(FPlane(vertices[3], vertices[7], vertices[6]), clippedVertices, outFace);
    Clip(FPlane(vertices[4], vertices[5], vertices[1]), clippedVertices, outFace);
    Clip(FPlane(vertices[4], vertices[6], vertices[7]), clippedVertices, outFace);
    Clip(FPlane(vertices[1], vertices[3], vertices[2]), clippedVertices, outFace);
}

void FPolyhedron::Clear()
{
    _faces.Clear();
}

void FPolyhedron::Transform(const TMatrix3x3F& transform)
{
    for (size_t i = 0; i < _faces.Size(); ++i)
    {
        TVector<TVector3F>& face = _faces[i];
        for (size_t j = 0; j < face.Size(); ++j)
            face[j] = transform * face[j];
    }
}

void FPolyhedron::Transform(const TMatrix3x4F& transform)
{
    for (size_t i = 0; i < _faces.Size(); ++i)
    {
        TVector<TVector3F>& face = _faces[i];
        for (size_t j = 0; j < face.Size(); ++j)
            face[j] = transform * face[j];
    }
}

FPolyhedron FPolyhedron::Transformed(const TMatrix3x3F& transform) const
{
    FPolyhedron ret;
    ret._faces.Resize(_faces.Size());
    
    for (size_t i = 0; i < _faces.Size(); ++i)
    {
        const TVector<TVector3F>& face = _faces[i];
        TVector<TVector3F>& newFace = ret._faces[i];
        newFace.Resize(face.Size());
        
        for (size_t j = 0; j < face.Size(); ++j)
            newFace[j] = transform * face[j];
    }
    
    return ret;
}

FPolyhedron FPolyhedron::Transformed(const TMatrix3x4F& transform) const
{
    FPolyhedron ret;
    ret._faces.Resize(_faces.Size());
    
    for (size_t i = 0; i < _faces.Size(); ++i)
    {
        const TVector<TVector3F>& face = _faces[i];
        TVector<TVector3F>& newFace = ret._faces[i];
        newFace.Resize(face.Size());
        
        for (size_t j = 0; j < face.Size(); ++j)
            newFace[j] = transform * face[j];
    }
    
    return ret;
}

void FPolyhedron::SetFace(size_t index, const TVector3F& v0, const TVector3F& v1, const TVector3F& v2)
{
    TVector<TVector3F>& face = _faces[index];
    face.Resize(3);
    face[0] = v0;
    face[1] = v1;
    face[2] = v2;
}

void FPolyhedron::SetFace(size_t index, const TVector3F& v0, const TVector3F& v1, const TVector3F& v2, const TVector3F& v3)
{
    TVector<TVector3F>& face = _faces[index];
    face.Resize(4);
    face[0] = v0;
    face[1] = v1;
    face[2] = v2;
    face[3] = v3;
}

}
