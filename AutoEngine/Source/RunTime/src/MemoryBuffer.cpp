#include "MemoryBuffer.h"

namespace Auto3D {


MemoryBuffer::MemoryBuffer(void* data, unsigned size) 
	: AbstractFile(size)
	, _buffer((unsigned char*)data)
	, _readOnly(false)
{
	if (!_buffer)
		_size = 0;
}

MemoryBuffer::MemoryBuffer(const void* data, unsigned size) 
	: AbstractFile(size)
	, _buffer((unsigned char*)data)
	, _readOnly(true)
{
	if (!_buffer)
		_size = 0;
}

MemoryBuffer::MemoryBuffer(VECTOR<unsigned char>& data)
	: AbstractFile(data.size())
	, _buffer(data.data())
	, _readOnly(false)
{
}

MemoryBuffer::MemoryBuffer(const VECTOR<unsigned char>& data) 
	: AbstractFile(data.size())
	, _buffer(const_cast<unsigned char*>(data.data()))
	, _readOnly(true)
{
}

unsigned MemoryBuffer::Read(void* dest, unsigned size)
{
	if (size + _position > _size)
		size = _size - _position;
	if (!size)
		return 0;

	unsigned char* srcPtr = &_buffer[_position];
	auto* destPtr = (unsigned char*)dest;
	_position += size;

	unsigned copySize = size;
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
	if (copySize & 1u)
		*destPtr = *srcPtr;

	return size;
}

unsigned MemoryBuffer::Seek(unsigned position)
{
	if (position > _size)
		position = _size;

	_position = position;
	return _position;
}

unsigned MemoryBuffer::Write(const void* data, unsigned size)
{
	if (size + _position > _size)
		size = _size - _position;
	if (!size)
		return 0;

	auto* srcPtr = (unsigned char*)data;
	unsigned char* destPtr = &_buffer[_position];
	_position += size;

	unsigned copySize = size;
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
	if (copySize & 1u)
		*destPtr = *srcPtr;

	return size;
}


}