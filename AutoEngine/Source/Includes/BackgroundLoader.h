#pragma once
#include "Object.h"
#include "Resource.h"
namespace Auto3D {

class BackgroundLoader : public Object
{
	REGISTER_OBJECT_CLASS(BackgroundLoader, Object)
public:
	explicit BackgroundLoader(SharedPtr<Ambient> ambient);
};

}