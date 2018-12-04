#include "DeferredShadingSpace.h"
#include "Level_0.h"

DeferredShadingSpace::DeferredShadingSpace(Ambient* ambient)
	:MotionSpace(ambient)
{
}

DeferredShadingSpace::~DeferredShadingSpace()
{
	RemoveScene(0);
}

void DeferredShadingSpace::Awake()
{
	RegisterScene(new Level_0(_ambient, 0));
}
AUTO_APPLICATION_MAIN(DeferredShadingSpace)