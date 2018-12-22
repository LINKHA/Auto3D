#pragma once

#include "Resource.h"

namespace Auto3D {

class Font : public Resource
{
	REGISTER_OBJECT_CLASS(Font, Resource)
public:
	explicit Font(Ambient* ambient);
	
};

}