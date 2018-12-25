#include "TimeSpace.h"
#include "Level_0.h"
#include "ResourceSystem.h"
#include "FileSystem.h"

TimeSpace::TimeSpace(Ambient* ambient)
	:MotionSpace(ambient)
{}
TimeSpace::~TimeSpace()
{
	RemoveScene(0);
}
void TimeSpace::Awake()
{
	STRING ResourceDir = GetSubSystem<FileSystem>()->GetProgramDir() + "../../SampleProject/Resource/";
	GetSubSystem<ResourceSystem>()->AddResourceDir(ResourceDir);
	RegisterScene(new Level_0(_ambient, 0));
}
AUTO_APPLICATION_MAIN(TimeSpace)