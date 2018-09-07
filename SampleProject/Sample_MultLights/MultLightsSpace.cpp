#include "MultLightsSpace.h"
#include "Application.h"
#include "Level_0.h"

MultLightsSpace::MultLightsSpace(Ambient* ambient)
	:MotionSpace(ambient)
{}
MultLightsSpace::~MultLightsSpace()
{
	RemoveLevel(0);
}

void MultLightsSpace::Awake()
{
	RegisterLevel(new Level_0(_ambient, 0));
}

int MultLightsSpace::Launch()
{
	return INSTANCE(Application).Run(_ambient);
}
AUTO_APPLICATION_MAIN(MultLightsSpace)