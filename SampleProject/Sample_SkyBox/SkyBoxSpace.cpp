#include "SkyBoxSpace.h"
#include "Level_0.h"

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
	RegisterScene(new Level_0(_ambient, 0));
}

AUTO_APPLICATION_MAIN(SkyBoxSpace)
