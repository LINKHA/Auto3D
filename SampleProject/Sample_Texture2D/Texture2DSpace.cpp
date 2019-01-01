#include "Texture2DSpace.h"
#include "ResourceSystem.h"
#include "FileSystem.h"
#include "level_0.h"

AUTO_APPLICATION_MAIN(Texture2DSpace)

void Texture2DSpace::Init()
{
	STRING ResourceDir = GetSubSystem<FileSystem>()->GetProgramDir() + "../../SampleProject/Resource/";
	GetSubSystem<ResourceSystem>()->AddResourceDir(ResourceDir);
	RegisterScene(MakeShared<Level_0>(_ambient, 0));
}
void Texture2DSpace::Start()
{
}
void Texture2DSpace::Stop()
{
}