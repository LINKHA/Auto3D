#include "SkyBox.h"
#include "../Resource/Image.h"
#include "../Graphics/Texture.h"
#include "../Resource/ResourceCache.h"
#include "../Debug/DebugNew.h"

#include "Model.h"

namespace Auto3D
{

SkyBox::SkyBox()
{
	auto* cache = Object::Subsystem<ResourceCache>();
	SetModel(cache->LoadResource<Model>("Box.mdl"));
}

SkyBox::~SkyBox()
{

}

void SkyBox::RegisterObject()
{
	RegisterFactory<SkyBox>();
}

}