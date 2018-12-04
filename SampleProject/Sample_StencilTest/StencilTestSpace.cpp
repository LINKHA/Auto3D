#include "StencilTestSpace.h"
#include "Level_0.h"


StencilTestSpace::StencilTestSpace(Ambient* ambient)
	:MotionSpace(ambient)
{
}
StencilTestSpace::~StencilTestSpace()
{
	RemoveScene(0);
}

void StencilTestSpace::Awake()
{
	RegisterScene(new Level_0(_ambient, 0));
}

AUTO_APPLICATION_MAIN(StencilTestSpace)