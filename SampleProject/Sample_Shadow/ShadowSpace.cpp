#include "ShadowSpace.h"
#include "ResourceSystem.h"
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
	GetSubSystem<ResourceSystem>()->AddResourceDir("../Resource/");
	//ShadowNormal
	//RegisterScene(new Level_0(_ambient, 0));
	//ShadowPoint
	RegisterScene(new Level_1(_ambient, 1));
}

AUTO_APPLICATION_MAIN(ShadowSpace)