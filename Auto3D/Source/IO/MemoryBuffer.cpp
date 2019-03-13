#include "../Base/Vector.h"
#include "MemoryBuffer.h"

#include <cstring>

#include "../Debug/DebugNew.h"

namespace Auto3D
{

MemoryBuffer::MemoryBuffer(void* _data, size_t numBytes) :
    Stream(_data ? numBytes : 0),
    _buffer((unsigned char*)_data),
    _readOnly(false)
{
    SetName("Memory");
}

MemoryBuffer::MemoryBuffer(const void* _data, size_t numBytes) :
    Stream(_data ? numBytes : 0),
    _buffer((unsigned char*)_data),
    _readOnly(true)
{
    SetName("Memory");
}

MemoryBuffer::MemoryBuffer(Vector<unsigned char>& data) :
    Stream(data.Size()),
    _buffer(data.Begin()._ptr),
    _readOnly(false)
{
}

MemoryBuffer::MemoryBuffer(const Vector<unsigned char>& data) :
    Stream(data.Size()),
    _buffer(data.Begin()._ptr),
    _readOnly(true)
{
}

size_t MemoryBuffer::Read(void* dest, size_t numBytes)
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

size_t MemoryBuffer::Seek(size_t newPosition)
{
    if (newPosition > _size)
        newPosition = _size;
    
    _position = newPosition;
    return _position;
}

size_t MemoryBuffer::Write(const void* _data, size_t numBytes)
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

bool MemoryBuffer::IsReadable() const
{
    return _buffer != nullptr;
}

bool MemoryBuffer::IsWritable() const
{
    return _buffer && !_readOnly;
}

}
