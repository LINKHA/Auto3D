#include "ParallaxMappingSpace.h"
#include "Level_0.h"

ParallaxMappingSpace::ParallaxMappingSpace(Ambient* ambient)
	:MotionSpace(ambient)
{}
ParallaxMappingSpace::~ParallaxMappingSpace()
{
	RemoveScene(0);
}

void ParallaxMappingSpace::Awake()
{
	RegisterScene(new Level_0(_ambient, 0));
}

AUTO_APPLICATION_MAIN(ParallaxMappingSpace)