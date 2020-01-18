#include "VectorBuffer.h"

#include <cstring>

#include "Debug/DebugNew.h"

namespace Auto3D
{

FVectorBuffer::FVectorBuffer()
{
    SetName("Vector");
}

FVectorBuffer::FVectorBuffer(const TVector<unsigned char>& _data)
{
    SetData(_data);
    SetName("Vector");
}

FVectorBuffer::FVectorBuffer(const void* _data, size_t numBytes)
{
    SetData(_data, numBytes);
    SetName("Vector");
}

FVectorBuffer::FVectorBuffer(FStream& source, size_t numBytes)
{
    SetData(source, numBytes);
    SetName("Vector");
}

size_t FVectorBuffer::Read(void* dest, size_t numBytes)
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
    
    return _size;
}

size_t FVectorBuffer::Seek(size_t newPosition)
{
    if (newPosition > _size)
        newPosition = _size;
    
    _position = newPosition;
    return _position;
}

size_t FVectorBuffer::Write(const void* _data, size_t numBytes)
{
    if (!numBytes)
        return 0;
    
    // Expand the buffer if necessary
    if (numBytes + _position > _size)
    {
        _size = _position + numBytes;
        _buffer.Resize(_size);
    }
    
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

bool FVectorBuffer::IsReadable() const
{
    return true;
}

bool FVectorBuffer::IsWritable() const
{
    return true;
}

void FVectorBuffer::SetData(const TVector<unsigned char>& _data)
{
    _buffer = _data;
    _position = 0;
    _size = _data.Size();
}

void FVectorBuffer::SetData(const void* _data, size_t numBytes)
{
    if (!_data)
        numBytes = 0;
    
    _buffer.Resize(numBytes);
    if (numBytes)
        memcpy(&_buffer[0], _data, numBytes);
    
    _position = 0;
    _size = numBytes;
}

void FVectorBuffer::SetData(FStream& source, size_t numBytes)
{
    _buffer.Resize(numBytes);
    size_t actualSize = source.Read(&_buffer[0], numBytes);
    if (actualSize != numBytes)
        _buffer.Resize(actualSize);
    
    _position = 0;
    _size = actualSize;
}

void FVectorBuffer::Clear()
{
    _buffer.Clear();
    _position = 0;
    _size = 0;
}

void FVectorBuffer::Resize(size_t newSize)
{
    _buffer.Resize(newSize);
    _size = newSize;
    if (_position > _size)
        _position = _size;
}

}
