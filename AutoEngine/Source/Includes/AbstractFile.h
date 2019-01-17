#pragma once
#include "Deserializer.h"
#include "Serializer.h"

namespace Auto3D {

/** 
* Abstract file 
*/
class AbstractFile : public Deserializer, public Serializer
{
public:
	/**
	* @brief : Construct
	*/
	AbstractFile() : Deserializer() {}
	/**
	* @brief : Construct
	*/
	explicit AbstractFile(unsigned int size) : Deserializer(size) {}
	/**
	* @brief : Destruct
	*/
	~AbstractFile() override = default;
};

}