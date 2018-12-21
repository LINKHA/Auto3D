#include "SkyBoxSpace.h"
#include "Level_0.h"
#include "ResourceSystem.h"

SkyBoxSpace::SkyBoxSpace(Ambient* ambient)
	:MotionSpace(ambient)
{
}

SkyBoxSpace::~SkyBoxSpace()
{
	RemoveScene(0);
}

void SkyBoxSpace::Awake()
{
	GetSubSystem<ResourceSystem>()->AddResourceDir("../Resource/");
	RegisterScene(new Level_0(_ambient, 0));
}

AUTO_APPLICATION_MAIN(SkyBoxSpace)
