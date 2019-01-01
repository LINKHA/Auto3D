#include "Physics2DSapce.h"
#include "Application.h"
#include "FileSystem.h"
#include "Level_0.h"
#include "ResourceSystem.h"

AUTO_APPLICATION_MAIN(Physics2DSapce)
void Physics2DSapce::Init()
{
	STRING ResourceDir = GetSubSystem<FileSystem>()->GetProgramDir() + "../../SampleProject/Resource/";
	GetSubSystem<ResourceSystem>()->AddResourceDir(ResourceDir);
	RegisterScene(MakeShared<Level_0>(_ambient, 0));
}
void Physics2DSapce::Destruct()
{
	RemoveScene(0);
}
