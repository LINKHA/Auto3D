#pragma once
#include "Resource.h"

namespace Auto3D {

class Sprite : public Resource
{
	REGISTER_OBJECT_CLASS(Sprite, Resource)
public:
	explicit Sprite(Ambient* ambient);
	/**
	* @brief : Register object factory
	*/
	static void RegisterObject(Ambient* ambient);

};

}