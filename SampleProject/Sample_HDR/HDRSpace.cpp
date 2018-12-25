#include "HDRSpace.h"
#include "Application.h"
#include "ResourceSystem.h"
#include "FileSystem.h"
#include "Level_0.h"

HDRSpace::HDRSpace(Ambient* ambient)
	:MotionSpace(ambient)
{}
HDRSpace::~HDRSpace()
{
	RemoveScene(0);
}

void HDRSpace::Awake()
{
	STRING ResourceDir = GetSubSystem<FileSystem>()->GetProgramDir() + "../../SampleProject/Resource/";
	GetSubSystem<ResourceSystem>()->AddResourceDir(ResourceDir);
	RegisterScene(new Level_0(_ambient, 0));
}

AUTO_APPLICATION_MAIN(HDRSpace)