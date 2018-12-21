#include "MoreCameraSpace.h"
#include "Level_0.h"
#include "ResourceSystem.h"

MoreCameraSpace::MoreCameraSpace(Ambient* ambient)
	:MotionSpace(ambient)
{
}
MoreCameraSpace::~MoreCameraSpace()
{
	RemoveScene(0);
}

void MoreCameraSpace::Awake()
{
	GetSubSystem<ResourceSystem>()->AddResourceDir("../Resource/");
	RegisterScene(new Level_0(_ambient, 0));
}

AUTO_APPLICATION_MAIN(MoreCameraSpace)