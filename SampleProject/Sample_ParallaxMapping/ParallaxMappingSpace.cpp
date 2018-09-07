#include "ParallaxMappingSpace.h"
#include "Application.h"
#include "Level_0.h"

ParallaxMappingSpace::ParallaxMappingSpace(Ambient* ambient)
	:MotionSpace(ambient)
{}
ParallaxMappingSpace::~ParallaxMappingSpace()
{
	RemoveLevel(0);
}

void ParallaxMappingSpace::Awake()
{
	RegisterLevel(new Level_0(_ambient, 0));
}

int ParallaxMappingSpace::Launch()
{
	return INSTANCE(Application).Run(_ambient);
}
AUTO_APPLICATION_MAIN(ParallaxMappingSpace)