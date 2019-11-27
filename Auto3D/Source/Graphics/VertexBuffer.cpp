#include "Debug/Log.h"
#include "Debug/Profiler.h"
#include "VertexBuffer.h"

#include "Debug/DebugNew.h"

namespace Auto3D
{

bool VertexBuffer::Define(ResourceUsage::Type usage, size_t numVertices, const Vector<VertexElement>& elements, bool useShadowData, const void* _data)
{
    if (!numVertices || !elements.Size())
    {
        ErrorString("Can not define vertex buffer with no vertices or no elements");
        return false;
    }

    return Define(usage, numVertices, elements.Size(), &elements[0], useShadowData, _data);
}

bool VertexBuffer::Define(ResourceUsage::Type usage, size_t numVertices, size_t numElements, const VertexElement* elements, bool useShadowData, const void* _data)
{
    PROFILE(DefineVertexBuffer);

    if (!numVertices || !numElements || !elements)
    {
        ErrorString("Can not define vertex buffer with no vertices or no elements");
        return false;
    }
    if (usage == ResourceUsage::RENDERTARGET)
    {
        ErrorString("Rendertarget usage is illegal for vertex buffers");
        return false;
    }
    if (usage == ResourceUsage::IMMUTABLE && !_data)
    {
        ErrorString("Immutable vertex buffer must define initial data");
        return false;
    }

    for (size_t i = 0; i < numElements; ++i)
    {
        if (elements[i]._type >= ElementType::MATRIX3X4)
        {
            ErrorString("Matrix elements are not supported in vertex buffers");
            return false;
        }
    }

    Release();

    _numVertices = numVertices;
    _usage = usage;

    // Determine offset of elements and the vertex _size & element hash
    _vertexSize = 0;
    _elementHash = 0;
    _elements.Resize(numElements);
    for (size_t i = 0; i < numElements; ++i)
    {
        _elements[i] = elements[i];
        _elements[i]._offset = _vertexSize;
        _vertexSize += elementSizes[_elements[i]._type];
        _elementHash |= ElementHash(i, _elements[i]._semantic);
    }

    // If buffer is reinitialized with the same shadow data, no need to reallocate
    if (useShadowData && (!_data || _data != _shadowData.Get()))
    {
        _shadowData = new unsigned char[_numVertices * _vertexSize];
        if (_data)
            memcpy(_shadowData.Get(), _data, _numVertices * _vertexSize);
    }

    return Create(_data);
}

}
