#pragma once
#include "Auto.h"

namespace Auto3D {

/**
* Abstract stream for writing
*/
class Serializer
{
public:
	virtual ~Serializer();
	/**
	* @brief : Write bytes to the stream. Return number of bytes actually written
	*/
	virtual unsigned Write(const void* data, unsigned size) = 0;
};

}