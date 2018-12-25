#include "DepthTestSpace.h"
#include "Level_0.h"
#include "FileSystem.h"
#include "ResourceSystem.h"
DepthTestSpace::DepthTestSpace(Ambient* ambient)
	:MotionSpace(ambient)
{}
DepthTestSpace::~DepthTestSpace()
{
	RemoveScene(0);
}
void DepthTestSpace::Awake()
{
	STRING ResourceDir = GetSubSystem<FileSystem>()->GetProgramDir() + "../../SampleProject/Resource/";
	GetSubSystem<ResourceSystem>()->AddResourceDir(ResourceDir);
	RegisterScene(new Level_0(_ambient, 0));
}

AUTO_APPLICATION_MAIN(DepthTestSpace)