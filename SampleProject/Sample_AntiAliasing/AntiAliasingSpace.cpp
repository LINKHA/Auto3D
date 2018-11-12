#include "AntiAliasingSpace.h"
#include "Application.h"
#include "Level_0.h"

AntiAliasingSpace::AntiAliasingSpace(Ambient* ambient)
	:MotionSpace(ambient)
{}
AntiAliasingSpace::~AntiAliasingSpace()
{
	RemoveScene(0);
}

void AntiAliasingSpace::Awake()
{
	RegisterScene(new Level_0(_ambient, 0));
}

int AntiAliasingSpace::Launch()
{
	return INSTANCE(Application).Run(_ambient);
}
AUTO_APPLICATION_MAIN(AntiAliasingSpace)