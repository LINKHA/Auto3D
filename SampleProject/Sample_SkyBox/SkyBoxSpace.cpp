#include "SkyBoxSpace.h"
#include "Application.h"
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
int SkyBoxSpace::Launch()
{
	return INSTANCE(Application).Run(_ambient);
}
AUTO_APPLICATION_MAIN(SkyBoxSpace)
