#include "PBRSpace.h"
#include "Level_0.h"

PBRSpace::PBRSpace(Ambient* ambient)
	:MotionSpace(ambient)
{}
PBRSpace::~PBRSpace()
{
	RemoveScene(0);
}

void PBRSpace::Awake()
{
	RegisterScene(new Level_0(_ambient, 0));
}

AUTO_APPLICATION_MAIN(PBRSpace)