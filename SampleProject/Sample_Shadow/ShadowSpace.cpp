#include "ShadowSpace.h"
#include "Application.h"
#include "Level_0.h"
#include "Level_1.h"


ShadowSpace::ShadowSpace(Ambient* ambient)
	:MotionSpace(ambient)
{}
ShadowSpace::~ShadowSpace()
{
}

void ShadowSpace::Awake()
{
	//ShadowNormal
	RegisterScene(new Level_0(_ambient, 0));
	//ShadowPoint
	//RegisterScene(new Level_1(_ambient, 1));
}

int ShadowSpace::Launch()
{
	return INSTANCE(Application).Run(_ambient);
}
AUTO_APPLICATION_MAIN(ShadowSpace)