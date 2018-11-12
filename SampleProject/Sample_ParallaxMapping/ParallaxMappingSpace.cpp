#include "ParallaxMappingSpace.h"
#include "Application.h"
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

int ParallaxMappingSpace::Launch()
{
	return INSTANCE(Application).Run(_ambient);
}
AUTO_APPLICATION_MAIN(ParallaxMappingSpace)