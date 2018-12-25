#include "SkyBoxSpace.h"
#include "Level_0.h"
#include "ResourceSystem.h"
#include "FileSystem.h"

SkyBoxSpace::SkyBoxSpace(Ambient* ambient)
	:MotionSpace(ambient)
{
}

SkyBoxSpace::~SkyBoxSpace()
{
	RemoveScene(0);
}

void SkyBoxSpace::Awake()
{
	STRING ResourceDir = GetSubSystem<FileSystem>()->GetProgramDir() + "../../SampleProject/Resource/";
	GetSubSystem<ResourceSystem>()->AddResourceDir(ResourceDir);
	RegisterScene(new Level_0(_ambient, 0));
}

AUTO_APPLICATION_MAIN(SkyBoxSpace)
