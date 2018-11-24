#pragma once
#include "Resource.h"

namespace Auto3D {

class Sound : public Resource
{
	REGISTER_DERIVED_CLASS(Sound, Resource);
	DECLARE_OBJECT_SERIALIZE(Sound);
public:
	explicit Sound(Ambient* ambient);

	static void RegisterObject(Ambient* ambient);
};

}