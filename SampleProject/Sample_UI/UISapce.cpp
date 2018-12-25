#include "UISapce.h"
#include "ResourceSystem.h"
#include "FileSystem.h"
#include "level_0.h"
UISapce::UISapce(Ambient* ambient)
	:MotionSpace(ambient)
{}
UISapce::~UISapce()
{
	RemoveScene(0);
}
void UISapce::Awake()
{
	STRING ResourceDir = GetSubSystem<FileSystem>()->GetProgramDir() + "../../SampleProject/Resource/";
	GetSubSystem<ResourceSystem>()->AddResourceDir(ResourceDir);
	RegisterScene(new Level_0(_ambient, 0));
}


AUTO_APPLICATION_MAIN(UISapce)