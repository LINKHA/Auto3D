#include "DepthTestSpace.h"
#include "Level_0.h"

DepthTestSpace::DepthTestSpace(Ambient* ambient)
	:MotionSpace(ambient)
{}
DepthTestSpace::~DepthTestSpace()
{
	RemoveScene(0);
}
void DepthTestSpace::Awake()
{
	RegisterScene(new Level_0(_ambient, 0));
}

AUTO_APPLICATION_MAIN(DepthTestSpace)