#include "MultLightsSpace.h"
#include "Level_0.h"
#include "ResourceSystem.h"

MultLightsSpace::MultLightsSpace(Ambient* ambient)
	:MotionSpace(ambient)
{}
MultLightsSpace::~MultLightsSpace()
{
	RemoveScene(0);
}

void MultLightsSpace::Awake()
{
	GetSubSystem<ResourceSystem>()->AddResourceDir("../Resource/");
	RegisterScene(new Level_0(_ambient, 0));
}

AUTO_APPLICATION_MAIN(MultLightsSpace)