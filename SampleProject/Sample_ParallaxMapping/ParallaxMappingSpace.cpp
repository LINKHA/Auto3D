#include "ParallaxMappingSpace.h"
#include "Config.h"
#include "Level_0.h"
#include "ResourceSystem.h"
#include "FileSystem.h"

AUTO_APPLICATION_MAIN(ParallaxMappingSpace)


void ParallaxMappingSpace::Init()
{
	STRING ResourceDir = system_content_dictionary; 
	GetSubSystem<ResourceSystem>()->AddResourceDir(ResourceDir);
	GetSubSystem<Scene>()->RegisterScene(MakeShared<Level_0>(_ambient, 0));
}
void ParallaxMappingSpace::Start()
{
}
void ParallaxMappingSpace::Stop()
{
}