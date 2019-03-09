#include "../../Debug/Log.h"
#include "../../Debug/Profiler.h"
#include "OGLGraphics.h"
#include "OGLIndexBuffer.h"

#include <glad.h>

#include "../../Debug/DebugNew.h"

namespace Auto3D
{

IndexBuffer::IndexBuffer() :
    _buffer(0),
    _numIndices(0),
    _indexSize(0),
    _usage(ResourceUsage::DEFAULT)
{
}

IndexBuffer::~IndexBuffer()
{
    Release();
}

void IndexBuffer::Release()
{
    if (_graphics && _graphics->GetIndexBuffer() == this)
        _graphics->SetIndexBuffer(nullptr);

    if (_buffer)
    {
        glDeleteBuffers(1, &_buffer);
        _buffer = 0;
    }
}

void IndexBuffer::Recreate()
{
    if (_numIndices)
    {
        Define(_usage, _numIndices, _indexSize, !_shadowData.IsNull(), _shadowData.Get());
        SetDataLost(!_shadowData.IsNull());
    }
}

bool IndexBuffer::SetData(size_t firstIndex, size_t numIndices, const void* _data)
{
    PROFILE(UpdateIndexBuffer);

    if (!_data)
    {
        ErrorString("Null source data for updating index buffer");
        return false;
    }
    if (firstIndex + numIndices > _numIndices)
    {
        ErrorString("Out of bounds range for updating index buffer");
        return false;
    }
    if (_buffer && _usage == ResourceUsage::IMMUTABLE)
    {
        ErrorString("Can not update immutable index buffer");
        return false;
    }

    if (_shadowData)
        memcpy(_shadowData.Get() + firstIndex * _indexSize, _data, numIndices * _indexSize);

    if (_buffer)
    {
        _graphics->SetIndexBuffer(this);
        if (numIndices == _numIndices)
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * _indexSize, _data, _usage == ResourceUsage::DYNAMIC ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
        else
            glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, firstIndex * _indexSize, numIndices * _indexSize, _data);
    }

    return true;
}

bool IndexBuffer::Create(const void* _data)
{
    if (_graphics && _graphics->IsInitialized())
    {
        glGenBuffers(1, &_buffer);
        if (!_buffer)
        {
            ErrorString("Failed to create index buffer");
            return false;
        }

        _graphics->SetIndexBuffer(this);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, _numIndices * _indexSize, _data, _usage == ResourceUsage::DYNAMIC ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
        LogStringF("Created index buffer numIndices %u indexSize %u", (unsigned)_numIndices, (unsigned)_indexSize);
    }

    return true;
}

}
