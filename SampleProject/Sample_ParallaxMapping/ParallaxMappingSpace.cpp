#include "ParallaxMappingSpace.h"
#include "Level_0.h"
#include "ResourceSystem.h"


ParallaxMappingSpace::ParallaxMappingSpace(Ambient* ambient)
	:MotionSpace(ambient)
{}
ParallaxMappingSpace::~ParallaxMappingSpace()
{
	RemoveScene(0);
}

void ParallaxMappingSpace::Awake()
{
	GetSubSystem<ResourceSystem>()->AddResourceDir("../Resource/");
	RegisterScene(new Level_0(_ambient, 0));
}

AUTO_APPLICATION_MAIN(ParallaxMappingSpace)