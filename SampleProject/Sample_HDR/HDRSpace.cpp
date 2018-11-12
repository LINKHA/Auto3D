#include "HDRSpace.h"
#include "Application.h"
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
	RegisterScene(new Level_0(_ambient, 0));
}

int HDRSpace::Launch()
{
	return INSTANCE(Application).Run(_ambient);
}
AUTO_APPLICATION_MAIN(HDRSpace)