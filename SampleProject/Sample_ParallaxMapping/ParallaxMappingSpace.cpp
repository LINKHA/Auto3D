#include "ParallaxMappingSpace.h"
#include "Level_0.h"
#include "ResourceSystem.h"
#include "FileSystem.h"

AUTO_APPLICATION_MAIN(ParallaxMappingSpace)


void ParallaxMappingSpace::Init()
{
	STRING ResourceDir = GetSubSystem<FileSystem>()->GetProgramDir() + "../../SampleProject/Resource/";
	GetSubSystem<ResourceSystem>()->AddResourceDir(ResourceDir);
	RegisterScene(MakeShared<Level_0>(_ambient, 0));
}
void ParallaxMappingSpace::Start()
{
}
void ParallaxMappingSpace::Stop()
{
}