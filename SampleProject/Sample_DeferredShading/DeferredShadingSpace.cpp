#include "DeferredShadingSpace.h"
#include "Application.h"
#include "Level_0.h"

DeferredShadingSpace::DeferredShadingSpace(Ambient* ambient)
	:MotionSpace(ambient)
{
}

DeferredShadingSpace::~DeferredShadingSpace()
{
	RemoveLevel(0);
}

void DeferredShadingSpace::Awake()
{
	RegisterLevel(new Level_0(_ambient, 0));
}

int DeferredShadingSpace::Launch()
{
	return INSTANCE(Application).Run(_ambient);
}
AUTO_APPLICATION_MAIN(DeferredShadingSpace)