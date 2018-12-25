#include "DebugSapce.h"
#include "ResourceSystem.h"
#include "FileSystem.h"
#include "level_0.h"

DebugSapce::DebugSapce(Ambient* ambient)
	:MotionSpace(ambient)
{}
DebugSapce::~DebugSapce()
{
	RemoveScene(0);
}
void DebugSapce::Awake()
{
	STRING ResourceDir = GetSubSystem<FileSystem>()->GetProgramDir() + "../../SampleProject/Resource/";
	GetSubSystem<ResourceSystem>()->AddResourceDir(ResourceDir);
	RegisterScene(new Level_0(_ambient, 0));
}

void DebugSapce::Start()
{
}
void DebugSapce::Update()
{
}
AUTO_APPLICATION_MAIN(DebugSapce)