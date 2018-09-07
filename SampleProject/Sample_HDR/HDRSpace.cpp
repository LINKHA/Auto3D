#include "HDRSpace.h"
#include "Application.h"
#include "Level_0.h"

HDRSpace::HDRSpace(Ambient* ambient)
	:MotionSpace(ambient)
{}
HDRSpace::~HDRSpace()
{
	RemoveLevel(0);
}

void HDRSpace::Awake()
{
	RegisterLevel(new Level_0(_ambient, 0));
}

int HDRSpace::Launch()
{
	return INSTANCE(Application).Run(_ambient);
}
AUTO_APPLICATION_MAIN(HDRSpace)