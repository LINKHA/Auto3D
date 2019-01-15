#include "FrameBuffersSpace.h"
#include "Config.h"
#include "Level_0.h"
#include "FileSystem.h"
#include "ResourceSystem.h"

AUTO_APPLICATION_MAIN(FrameBuffersSpace)

void FrameBuffersSpace::Init()
{
	STRING ResourceDir = system_content_dictionary; 
	GetSubSystem<ResourceSystem>()->AddResourceDir(ResourceDir);
	GetSubSystem<Scene>()->RegisterScene(MakeShared<Level_0>(_ambient, 0));
}
void FrameBuffersSpace::Start()
{
}
void FrameBuffersSpace::Stop()
{
}
