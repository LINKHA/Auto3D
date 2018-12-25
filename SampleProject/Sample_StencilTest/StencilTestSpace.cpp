#include "StencilTestSpace.h"
#include "ResourceSystem.h"
#include "FileSystem.h"
#include "Level_0.h"


StencilTestSpace::StencilTestSpace(Ambient* ambient)
	:MotionSpace(ambient)
{
}
StencilTestSpace::~StencilTestSpace()
{
	RemoveScene(0);
}

void StencilTestSpace::Awake()
{
	STRING ResourceDir = GetSubSystem<FileSystem>()->GetProgramDir() + "../../SampleProject/Resource/";
	GetSubSystem<ResourceSystem>()->AddResourceDir(ResourceDir);
	RegisterScene(new Level_0(_ambient, 0));
}

AUTO_APPLICATION_MAIN(StencilTestSpace)