#include "SkyBox.h"
#include "../Resource/Image.h"
#include "../Graphics/Texture.h"
#include "../Resource/ResourceCache.h"
#include "../Debug/DebugNew.h"
#include "../Engine/ModuleManager.h"

#include "Model.h"

namespace Auto3D
{

SkyBox::SkyBox()
{
	auto cache = ModuleManager::Get().CacheModule();
	SetModel(cache->LoadResource<Model>("Box.mdl"));
	OnWorldBoundingBoxUpdate();
}

SkyBox::~SkyBox() = default;


void SkyBox::RegisterObject()
{
	RegisterFactory<SkyBox>();
}

void SkyBox::OnWorldBoundingBoxUpdate()
{
	// The skybox is supposed to be visible everywhere, so set a humongous bounding box
	_boundingBox.Define(-M_FLOAT_BIG, M_FLOAT_BIG);
}

}