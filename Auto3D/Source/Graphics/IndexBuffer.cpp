#include "../Debug/Log.h"
#include "../Debug/Profiler.h"
#include "IndexBuffer.h"

#include "../Debug/DebugNew.h"

namespace Auto3D
{

bool IndexBuffer::Define(ResourceUsage usage, size_t numIndices, size_t indexSize, bool useShadowData, const void* data)
{
    PROFILE(DefineIndexBuffer);

    Release();

    if (!numIndices)
    {
        ErrorString("Can not define index buffer with no indices");
        return false;
    }
    if (usage == USAGE_RENDERTARGET)
    {
        ErrorString("Rendertarget usage is illegal for index buffers");
        return false;
    }
    if (usage == USAGE_IMMUTABLE && !data)
    {
        ErrorString("Immutable index buffer must define initial data");
        return false;
    }
    if (indexSize != sizeof(unsigned) && indexSize != sizeof(unsigned short))
    {
        ErrorString("Index buffer index size must be 2 or 4");
        return false;
    }

    _numIndices = numIndices;
    _indexSize = indexSize;
    _usage = usage;

    // If buffer is reinitialized with the same shadow data, no need to reallocate
    if (useShadowData && (!data || data != _shadowData.Get()))
    {
        _shadowData = new unsigned char[_numIndices * _indexSize];
        if (data)
            memcpy(_shadowData.Get(), data, _numIndices * _indexSize);
    }

    return Create(data);
}

}
