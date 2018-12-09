#pragma once
#include "Resource.h"

namespace Auto3D {

class Sprite : public Resource
{
	REGISTER_DERIVED_CLASS(Sprite, Resource);
	DECLARE_OBJECT_SERIALIZE(Sprite);
public:
	explicit Sprite(Ambient* ambient);
	/**
	* @brief : Register object factory
	*/
	static void RegisterObject(Ambient* ambient);

};

}