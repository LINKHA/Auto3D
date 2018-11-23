#pragma once
#include "Auto.h"
#include "GameManager.h"

namespace Auto3D {

class ResourceCache : public GlobalGameManager
{
	REGISTER_DERIVED_CLASS(ResourceCache, GlobalGameManager);
	DECLARE_OBJECT_SERIALIZE(ResourceCache);
public:
	ResourceCache();
	

};

}