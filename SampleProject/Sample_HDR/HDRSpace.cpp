#include "HDRSpace.h"
#include "Application.h"
#include "ResourceSystem.h"
#include "Level_0.h"

HDRSpace::HDRSpace(Ambient* ambient)
	:MotionSpace(ambient)
{}
HDRSpace::~HDRSpace()
{
	RemoveScene(0);
}

void HDRSpace::Awake()
{
	GetSubSystem<ResourceSystem>()->AddResourceDir("../Resource/");
	RegisterScene(new Level_0(_ambient, 0));
}

AUTO_APPLICATION_MAIN(HDRSpace)