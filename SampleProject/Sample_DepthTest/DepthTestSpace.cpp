#include "DepthTestSpace.h"
#include "Level_0.h"
#include "FileSystem.h"
#include "ResourceSystem.h"

AUTO_APPLICATION_MAIN(DepthTestSpace)

void DepthTestSpace::Init()
{
	STRING ResourceDir = GetSubSystem<FileSystem>()->GetProgramDir() + "../../SampleProject/Resource/";
	GetSubSystem<ResourceSystem>()->AddResourceDir(ResourceDir);
	RegisterScene(new Level_0(_ambient, 0));
}
void DepthTestSpace::Destruct()
{
	RemoveScene(0);
}