#include "MassInstantiationSpace.h"
#include "Application.h"
#include "ResourceSystem.h"
#include "FileSystem.h"
#include "Level_0.h"

AUTO_APPLICATION_MAIN(MassInstantiationSpace)

void MassInstantiationSpace::Init()
{
	STRING ResourceDir = GetSubSystem<FileSystem>()->GetProgramDir() + "../../SampleProject/Resource/";
	GetSubSystem<ResourceSystem>()->AddResourceDir(ResourceDir);
	STRING ResourceDir2 = GetSubSystem<FileSystem>()->GetProgramDir() + "../../SampleProject/Sample_MassInstantiation/";
	GetSubSystem<ResourceSystem>()->AddResourceDir(ResourceDir2);
	GetSubSystem<Scene>()->RegisterScene(MakeShared<Level_0>(_ambient, 0));
}
void MassInstantiationSpace::Start()
{

}
void MassInstantiationSpace::Stop()
{
}
