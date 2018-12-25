#include "Physics3DSpace.h"
#include "Application.h"
#include "ResourceSystem.h"
#include "FileSystem.h"
#include "Level_0.h"

Physics3DSpace::Physics3DSpace(Ambient* ambient)
	:MotionSpace(ambient)
{}
Physics3DSpace::~Physics3DSpace()
{
	RemoveScene(0);
}
void Physics3DSpace::Awake()
{
	STRING ResourceDir = GetSubSystem<FileSystem>()->GetProgramDir() + "../../SampleProject/Resource/";
	GetSubSystem<ResourceSystem>()->AddResourceDir(ResourceDir);
	RegisterScene(new Level_0(_ambient, 0));
}

AUTO_APPLICATION_MAIN(Physics3DSpace)