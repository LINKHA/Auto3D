#pragma once

#include "Auto.h"
 
/**
* Abstract stream for reading
*/
namespace Auto3D {

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
protected:
	/// stream position
	unsigned _position;
	/// stream size
	unsigned _size;
};

}