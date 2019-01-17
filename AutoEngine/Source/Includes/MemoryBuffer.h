#pragma once

#include "AbstractFile.h"

namespace Auto3D {
/** 
* Memory area that can be read and written to as a stream
*/
class MemoryBuffer : public AbstractFile
{
public:
	/**
	* @brief : Construct with a pointer and size
	*/
	MemoryBuffer(void* data, unsigned size);
	/**
	* @brief : Construct as read-only with a pointer and size
	*/
	MemoryBuffer(const void* data, unsigned size);
	/**
	* @brief : Construct from a vector, which must not go out of scope before MemoryBuffer
	*/
	explicit MemoryBuffer(VECTOR<unsigned char>& data);
	/**
	* @brief : Construct from a read-only vector, which must not go out of scope before MemoryBuffer
	*/
	explicit MemoryBuffer(const VECTOR<unsigned char>& data);
	/**
	* @brief : Read bytes from the memory area. Return number of bytes actually read
	*/
	unsigned Read(void* dest, unsigned size) override;
	/**
	* @brief : Set position from the beginning of the memory area. Return actual new position
	*/
	unsigned Seek(unsigned position) override;
	/**
	* @brief : Write bytes to the memory area
	*/
	unsigned Write(const void* data, unsigned size) override;
	/**
	* @brief : Return memory area
	*/
	unsigned char* GetData() { return _buffer; } 
	/**
	* @brief : Return whether buffer is read-only
	*/
	bool IsReadOnly() { return _readOnly; }
private:
	/// pointer to the memory area
	unsigned char* _buffer;
	/// read-only flag
	bool _readOnly;
};

}