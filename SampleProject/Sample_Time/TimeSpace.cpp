#include "TimeSpace.h"
#include "Level_0.h"
#include "ResourceSystem.h"
#include "FileSystem.h"

AUTO_APPLICATION_MAIN(TimeSpace)

void TimeSpace::Init()
{
	STRING ResourceDir = GetSubSystem<FileSystem>()->GetProgramDir() + "../../SampleProject/Resource/";
	GetSubSystem<ResourceSystem>()->AddResourceDir(ResourceDir);
	GetSubSystem<Scene>()->RegisterScene(MakeShared<Level_0>(_ambient, 0));
}
void TimeSpace::Start()
{
}
void TimeSpace::Stop()
{
}
