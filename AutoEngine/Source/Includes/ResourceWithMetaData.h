#pragma once
#include "Resource.h"

namespace Auto3D {

class ResourceWithMetaData : public Resource
{
	REGISTER_OBJECT_ABSTRACT_CLASS(ResourceWithMetaData, Resource)
public:
	/// Construct
	explicit ResourceWithMetaData(SharedPtr<Ambient> ambient) : Super(ambient) {}

};



}