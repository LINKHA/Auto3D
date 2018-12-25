#include "PBRSpace.h"
#include "Level_0.h"
#include "ResourceSystem.h"
#include "FileSystem.h"

PBRSpace::PBRSpace(Ambient* ambient)
	:MotionSpace(ambient)
{}
PBRSpace::~PBRSpace()
{
	RemoveScene(0);
}

void PBRSpace::Awake()
{
	STRING ResourceDir = GetSubSystem<FileSystem>()->GetProgramDir() + "../../SampleProject/Resource/";
	GetSubSystem<ResourceSystem>()->AddResourceDir(ResourceDir);
	RegisterScene(new Level_0(_ambient, 0));
}

AUTO_APPLICATION_MAIN(PBRSpace)