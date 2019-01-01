#include "ShadowSpace.h"
#include "ResourceSystem.h"
#include "FileSystem.h"
#include "Level_0.h"

AUTO_APPLICATION_MAIN(ShadowSpace)

void ShadowSpace::Init()
{
	STRING ResourceDir = GetSubSystem<FileSystem>()->GetProgramDir() + "../../SampleProject/Resource/";
	GetSubSystem<ResourceSystem>()->AddResourceDir(ResourceDir);
	RegisterScene(MakeShared<Level_0>(_ambient, 0));
}
void ShadowSpace::Start()
{
}
void ShadowSpace::Stop()
{
}
