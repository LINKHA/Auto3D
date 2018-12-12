#pragma once
#include "Resource.h"

namespace Auto3D {

class Sound : public Resource
{
	REGISTER_OBJECT_CLASS(Sound, Resource)
public:
	explicit Sound(Ambient* ambient);

	static void RegisterObject(Ambient* ambient);
};

}