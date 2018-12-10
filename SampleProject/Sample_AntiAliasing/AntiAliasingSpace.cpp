#include "AntiAliasingSpace.h"
#include "ResourceSystem.h"
#include "FileSystem.h"
#include "Level_0.h"

AntiAliasingSpace::AntiAliasingSpace(Ambient* ambient)
	:MotionSpace(ambient)
{}
AntiAliasingSpace::~AntiAliasingSpace()
{
	RemoveScene(0);
}

void AntiAliasingSpace::Awake()
{
	STRING ResourceDir = GetSubSystem<FileSystem>()->GetProgramDir() + "../../SampleProject/Resource/";
	GetSubSystem<ResourceSystem>()->AddResourceDir(ResourceDir);
	RegisterScene(new Level_0(_ambient, 0));
}

AUTO_APPLICATION_MAIN(AntiAliasingSpace)