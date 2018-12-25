#include "SSAOSpace.h"
#include "Application.h"
#include "ResourceSystem.h"
#include "FileSystem.h"
#include "Level_0.h"


SSAOSpace::SSAOSpace(Ambient* ambient)
	:MotionSpace(ambient)
{}
SSAOSpace::~SSAOSpace()
{
	RemoveScene(0);
}

void SSAOSpace::Awake()
{
	STRING ResourceDir = GetSubSystem<FileSystem>()->GetProgramDir() + "../../SampleProject/Resource/";
	GetSubSystem<ResourceSystem>()->AddResourceDir(ResourceDir);
	RegisterScene(new Level_0(_ambient, 0));
}
AUTO_APPLICATION_MAIN(SSAOSpace)