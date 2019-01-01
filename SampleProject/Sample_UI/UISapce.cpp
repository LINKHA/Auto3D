#include "UISapce.h"
#include "ResourceSystem.h"
#include "FileSystem.h"
#include "level_0.h"

AUTO_APPLICATION_MAIN(UISapce)

void UISapce::Init()
{
	STRING ResourceDir = GetSubSystem<FileSystem>()->GetProgramDir() + "../../SampleProject/Resource/";
	GetSubSystem<ResourceSystem>()->AddResourceDir(ResourceDir);
	RegisterScene(MakeShared<Level_0>(_ambient, 0));
}
void UISapce::Start()
{
}
void UISapce::Stop()
{
}