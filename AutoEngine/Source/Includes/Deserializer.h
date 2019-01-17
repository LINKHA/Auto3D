#pragma once

#include "Auto.h"
 

namespace Auto3D {

/**
* Abstract stream for reading
*/
class Deserializer
{
public:
	/**
	* @brief : Construct with zero size
	*/
	Deserializer();
	/**
	* @brief : Construct with defined size
	*/
	explicit Deserializer(unsigned size);
	/**
	* @brief : Destruct
	*/
	virtual ~Deserializer();
	/**
	* @brief : Read bytes from the stream. Return number of bytes actually read
	*/
	virtual unsigned Read(void* dest, unsigned size) = 0;
	/**
	* @brief : Set position from the beginning of the stream. Return actual new position
	*/
	virtual unsigned Seek(unsigned position) = 0;
	/**
	* @brief : Return name of the stream
	*/
	virtual const STRING& GetName() const;
	/**
	* @brief : Return a checksum if applicable
	*/
	virtual unsigned GetChecksum();
	/**
	* @brief : Return whether the end of stream has been reached
	*/
	virtual bool IsEof() const { return _position >= _size; }
	/**
	* @brief : Return size
	*/
	unsigned GetSize() const { return _size; }
	/**
	* @brief : Read a 64-bit integer
	*/
	long long ReadInt64();
	/**
	* @brief : Read a 32-bit integer
	*/
	int ReadInt();
	/**
	* @brief : Read a 16-bit integer
	*/
	short ReadShort();
	/**
	* @brief : Read an 8-bit integer
	*/
	signed char ReadByte();
	/**
	* @brief : Read a 64-bit unsigned integer
	*/
	unsigned long long ReadUInt64();
	/**
	* @brief : Read a 32-bit unsigned integer
	*/
	unsigned ReadUInt();
	/**
	* @brief : Read a 16-bit unsigned integer
	*/
	unsigned short ReadUShort();
	/**
	* @brief : Read an 8-bit unsigned integer
	*/
	unsigned char ReadUByte();
	/**
	* @brief : Read a bool
	*/
	bool ReadBool();
	/**
	* @brief : Read a float
	*/
	float ReadFloat();
	/**
	* @brief : Read a double
	*/
	double ReadDouble();
	/**
	* @brief : Read a four-letter file ID
	*/
	STRING ReadFileID();
	/**
	* @brief : Read a text line
	*/
	STRING ReadLine();
protected:
	/// stream position
	unsigned _position;
	/// stream size
	unsigned _size;
};

}