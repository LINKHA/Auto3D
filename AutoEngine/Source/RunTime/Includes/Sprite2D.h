#pragma once
#include "Resource.h"

namespace Auto3D {

class Sprite2D : public Resource
{
	REGISTER_DERIVED_CLASS(Sprite2D, Resource);
	DECLARE_OBJECT_SERIALIZE(Sprite2D);
public:
	explicit Sprite2D(Ambient* ambient);
	/**
	* @brief : Register object factory
	*/
	static void RegisterObject(Ambient* ambient);
};

}