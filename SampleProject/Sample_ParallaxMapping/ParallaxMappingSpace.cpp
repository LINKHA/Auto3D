#include "ParallaxMappingSpace.h"
#include "Level_0.h"
#include "ResourceSystem.h"
#include "FileSystem.h"


ParallaxMappingSpace::ParallaxMappingSpace(Ambient* ambient)
	:MotionSpace(ambient)
{}
ParallaxMappingSpace::~ParallaxMappingSpace()
{
	RemoveScene(0);
}

void ParallaxMappingSpace::Awake()
{
	STRING ResourceDir = GetSubSystem<FileSystem>()->GetProgramDir() + "../../SampleProject/Resource/";
	GetSubSystem<ResourceSystem>()->AddResourceDir(ResourceDir);
	RegisterScene(new Level_0(_ambient, 0));
}

AUTO_APPLICATION_MAIN(ParallaxMappingSpace)