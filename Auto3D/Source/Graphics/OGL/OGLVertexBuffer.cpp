#include "Debug/Log.h"
#include "Debug/Profiler.h"
#include "OGLGraphics.h"
#include "OGLVertexBuffer.h"

#include <glad.h>

#include "Debug/DebugNew.h"

namespace Auto3D
{

VertexBuffer::VertexBuffer() :
    _buffer(0),
    _numVertices(0),
    _vertexSize(0),
    _elementHash(0),
    _usage(ResourceUsage::DEFAULT)
{
}

VertexBuffer::~VertexBuffer()
{
    Release();
}

void VertexBuffer::Release()
{
    if (_graphics)
    {
        for (size_t i = 0; i < MAX_VERTEX_STREAMS; ++i)
        {
            if (_graphics->GetVertexBuffer(i) == this)
                _graphics->SetVertexBuffer(i, 0);
        }
    }

    if (_buffer)
    {
        if (_graphics && _graphics->BoundVBO() == _buffer)
            _graphics->BindVBO(0);

        glDeleteBuffers(1, &_buffer);
        _buffer = 0;
    }
}

void VertexBuffer::Recreate()
{
    if (_numVertices)
    {
        // Also make a copy of the current vertex elements, as they are passed by reference and manipulated by Define()
        TVector<VertexElement> srcElements = _elements;
        Define(_usage, _numVertices, srcElements, !_shadowData.IsNull(), _shadowData.Get());
        SetDataLost(!_shadowData.IsNull());
    }
}

bool VertexBuffer::SetData(size_t firstVertex, size_t numVertices, const void* data)
{
    PROFILE(UpdateVertexBuffer);

    if (!data)
    {
        ErrorString("Null source data for updating vertex buffer");
        return false;
    }
    if (firstVertex + numVertices > _numVertices)
    {
        ErrorString("Out of bounds range for updating vertex buffer");
        return false;
    }
    if (_buffer && _usage == ResourceUsage::IMMUTABLE)
    {
        ErrorString("Can not update immutable vertex buffer");
        return false;
    }

    if (_shadowData)
        memcpy(_shadowData.Get() + firstVertex * _vertexSize, data, numVertices * _vertexSize);

    if (_buffer)
    {
        _graphics->BindVBO(_buffer);
        if (numVertices == _numVertices)
            glBufferData(GL_ARRAY_BUFFER, numVertices * _vertexSize, data, _usage == ResourceUsage::DYNAMIC ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
        else
            glBufferSubData(GL_ARRAY_BUFFER, firstVertex * _vertexSize, numVertices * _vertexSize, data);
    }

    return true;
}

bool VertexBuffer::Create(const void* data)
{
    if (_graphics && _graphics->IsInitialized())
    {
        glGenBuffers(1, &_buffer);
        if (!_buffer)
        {
            ErrorString("Failed to create vertex buffer");
            return false;
        }

        _graphics->BindVBO(_buffer);
        glBufferData(GL_ARRAY_BUFFER, _numVertices * _vertexSize, data, _usage == ResourceUsage::DYNAMIC ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
        LogStringF("Created vertex buffer numVertices %u vertexSize %u", (unsigned)_numVertices, (unsigned)_vertexSize);
    }

    return true;
}

}
