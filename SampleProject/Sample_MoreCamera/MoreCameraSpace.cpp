#include "MoreCameraSpace.h"
#include "Level_0.h"
#include "FileSystem.h"
#include "ResourceSystem.h"


AUTO_APPLICATION_MAIN(MoreCameraSpace)

void MoreCameraSpace::Init()
{
	STRING ResourceDir = GetSubSystem<FileSystem>()->GetProgramDir() + "../../SampleProject/Resource/";
	GetSubSystem<ResourceSystem>()->AddResourceDir(ResourceDir);
	RegisterScene(new Level_0(_ambient, 0));
}
void MoreCameraSpace::Destruct()
{
	RemoveScene(0);
}