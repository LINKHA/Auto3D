#include "SkyBox.h"
#include "../Resource/Image.h"
#include "../Graphics/Texture.h"
#include "../Resource/ResourceCache.h"
#include "../Engine/ModuleManager.h"
#include "../RegisteredBox/RegisteredBox.h"
#include "../Scene/Scene.h"
#include "Model.h"

#include "../Debug/DebugNew.h"

namespace Auto3D
{

SkyBox::SkyBox()
{
	auto cache = ModuleManager::Get().CacheModule();
	SetModel(cache->LoadResource<Model>("Model/Box.mdl"));
	OnWorldBoundingBoxUpdate();
	ModuleManager::Get().RegisteredBoxModule()->GetActiveScene()->SetSkyBox(this);
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