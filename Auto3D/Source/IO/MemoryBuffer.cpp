#include "Container/Vector.h"
#include "MemoryBuffer.h"

#include <cstring>

#include "Debug/DebugNew.h"

namespace Auto3D
{

FMemoryBuffer::FMemoryBuffer(void* data, size_t numBytes) :
    FStream(data ? numBytes : 0),
    _buffer((unsigned char*)data),
    _readOnly(false)
{
    SetName("Memory");
}

FMemoryBuffer::FMemoryBuffer(const void* data, size_t numBytes) :
    FStream(data ? numBytes : 0),
    _buffer((unsigned char*)data),
    _readOnly(true)
{
    SetName("Memory");
}

FMemoryBuffer::FMemoryBuffer(TVector<unsigned char>& data) :
    FStream(data.Size()),
    _buffer(data.Begin()._ptr),
    _readOnly(false)
{
}

FMemoryBuffer::FMemoryBuffer(const TVector<unsigned char>& data) :
    FStream(data.Size()),
    _buffer(data.Begin()._ptr),
    _readOnly(true)
{
}

size_t FMemoryBuffer::Read(void* dest, size_t numBytes)
{
    if (numBytes + _position > _size)
        numBytes = _size - _position;
    if (!numBytes)
        return 0;
    
    unsigned char* srcPtr = &_buffer[_position];
    unsigned char* destPtr = (unsigned char*)dest;
    _position += numBytes;
    
    size_t copySize = numBytes;
    while (copySize >= sizeof(unsigned))
    {
        *((unsigned*)destPtr) = *((unsigned*)srcPtr);
        srcPtr += sizeof(unsigned);
        destPtr += sizeof(unsigned);
        copySize -= sizeof(unsigned);
    }
    if (copySize & sizeof(unsigned short))
    {
        *((unsigned short*)destPtr) = *((unsigned short*)srcPtr);
        srcPtr += sizeof(unsigned short);
        destPtr += sizeof(unsigned short);
    }
    if (copySize & 1)
        *destPtr = *srcPtr;
    
    return numBytes;
}

size_t FMemoryBuffer::Seek(size_t newPosition)
{
    if (newPosition > _size)
        newPosition = _size;
    
    _position = newPosition;
    return _position;
}

size_t FMemoryBuffer::Write(const void* _data, size_t numBytes)
{
    if (numBytes + _position > _size)
        numBytes = _size - _position;
    if (!numBytes || _readOnly)
        return 0;
    
    unsigned char* srcPtr = (unsigned char*)_data;
    unsigned char* destPtr = &_buffer[_position];
    _position += numBytes;
    
    size_t copySize = numBytes;
    while (copySize >= sizeof(unsigned))
    {
        *((unsigned*)destPtr) = *((unsigned*)srcPtr);
        srcPtr += sizeof(unsigned);
        destPtr += sizeof(unsigned);
        copySize -= sizeof(unsigned);
    }
    if (copySize & sizeof(unsigned short))
    {
        *((unsigned short*)destPtr) = *((unsigned short*)srcPtr);
        srcPtr += sizeof(unsigned short);
        destPtr += sizeof(unsigned short);
    }
    if (copySize & 1)
        *destPtr = *srcPtr;
    
    return numBytes;
}

bool FMemoryBuffer::IsReadable() const
{
    return _buffer != nullptr;
}

bool FMemoryBuffer::IsWritable() const
{
    return _buffer && !_readOnly;
}

}
