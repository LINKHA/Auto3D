#include "../../Debug/Log.h"
#include "../../Debug/Profiler.h"
#include "../../Math/Color.h"
#include "../../Math/Matrix3x3.h"
#include "../../Math/Matrix3x4.h"
#include "OGLConstantBuffer.h"
#include "OGLGraphics.h"

#include <glad.h>

#include "../../Debug/DebugNew.h"

namespace Auto3D
{

ConstantBuffer::ConstantBuffer() :
    _buffer(0),
    _byteSize(0),
    _usage(USAGE_DEFAULT),
    _dirty(false)
{
}

ConstantBuffer::~ConstantBuffer()
{
    Release();
}

void ConstantBuffer::Release()
{
    if (_graphics)
    {
        for (size_t i = 0; i < MAX_SHADER_STAGES; ++i)
        {
            for (size_t j = 0; j < MAX_CONSTANT_BUFFERS; ++j)
            {
                if (_graphics->GetConstantBuffer((ShaderStage)i, j) == this)
                    _graphics->SetConstantBuffer((ShaderStage)i, j, 0);
            }
        }
    }

    if (_buffer)
    {
        if (_graphics && _graphics->BoundUBO() == _buffer)
            _graphics->BindUBO(0);

        glDeleteBuffers(1, &_buffer);
        _buffer = 0;
    }
}

void ConstantBuffer::Recreate()
{
    if (_constants.Size())
    {
        // Make a copy of the current constants, as they are passed by reference and manipulated by Define()
        Vector<Constant> srcConstants = _constants;
        Define(_usage, srcConstants);
        Apply();
    }
}

bool ConstantBuffer::SetData(const void* data, bool copyToShadow)
{
    if (copyToShadow)
        memcpy(_shadowData.Get(), data, _byteSize);

    if (_usage == USAGE_IMMUTABLE)
    {
        if (!_buffer)
            return Create(data);
        else
        {
            ErrorString("Apply can only be called once on an immutable constant buffer");
            return false;
        }
    }

    if (_buffer)
    {
        _graphics->BindUBO(_buffer);
        glBufferData(GL_UNIFORM_BUFFER, _byteSize, data, _usage != USAGE_IMMUTABLE ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
    }

    _dirty = false;
    return true;
}

bool ConstantBuffer::Create(const void* data)
{
    _dirty = false;

    if (_graphics && _graphics->IsInitialized())
    {
        glGenBuffers(1, &_buffer);
        if (!_buffer)
        {
            ErrorString("Failed to create constant buffer");
            return false;
        }

        _graphics->BindUBO(_buffer);
        glBufferData(GL_UNIFORM_BUFFER, _byteSize, data, _usage != USAGE_IMMUTABLE ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
    }

    return true;
}

}
