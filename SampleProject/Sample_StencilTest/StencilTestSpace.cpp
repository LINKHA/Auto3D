#include "StencilTestSpace.h"
#include "Application.h"
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
int StencilTestSpace::Launch()
{
	return INSTANCE(Application).Run(_ambient);
}
AUTO_APPLICATION_MAIN(StencilTestSpace)