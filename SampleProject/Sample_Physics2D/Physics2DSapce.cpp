#include "Physics2DSapce.h"
#include "Application.h"
#include "FileSystem.h"
#include "Level_0.h"
#include "ResourceSystem.h"

Physics2DSapce::Physics2DSapce(Ambient* ambient)
	:MotionSpace(ambient)
{}
Physics2DSapce::~Physics2DSapce()
{
	RemoveScene(0);
}

void Physics2DSapce::Awake()
{
	STRING ResourceDir = GetSubSystem<FileSystem>()->GetProgramDir() + "../../SampleProject/Resource/";
	GetSubSystem<ResourceSystem>()->AddResourceDir(ResourceDir);
	RegisterScene(new Level_0(_ambient, 0));
}

AUTO_APPLICATION_MAIN(Physics2DSapce)