#include "MassInstantiationSpace.h"
#include "Application.h"
#include "ResourceSystem.h"
#include "FileSystem.h"
#include "Level_0.h"

MassInstantiationSpace::MassInstantiationSpace(Ambient* ambient)
	:MotionSpace(ambient)
{
}
MassInstantiationSpace::~MassInstantiationSpace()
{
	RemoveScene(0);
}

void MassInstantiationSpace::Awake()
{
	STRING ResourceDir = GetSubSystem<FileSystem>()->GetProgramDir() + "../../SampleProject/Resource/";
	GetSubSystem<ResourceSystem>()->AddResourceDir(ResourceDir);
	STRING ResourceDir2 = GetSubSystem<FileSystem>()->GetProgramDir() + "../../SampleProject/Sample_MassInstantiation/";
	GetSubSystem<ResourceSystem>()->AddResourceDir(ResourceDir2);
	RegisterScene(new Level_0(_ambient, 0));
}

AUTO_APPLICATION_MAIN(MassInstantiationSpace)