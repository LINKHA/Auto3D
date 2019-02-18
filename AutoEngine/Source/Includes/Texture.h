#pragma once
#include "ResourceWithMetaData.h"
#include "GPUObject.h"

namespace Auto3D {

static const int MAX_TEXTURE_QUALITY_LEVELS = 3;

class Texture : public ResourceWithMetaData, public GPUObject
{
	REGISTER_OBJECT_ABSTRACT_CLASS(Texture, ResourceWithMetaData)
public:
	explicit Texture(SharedPtr<Ambient> ambient);
	
};

}