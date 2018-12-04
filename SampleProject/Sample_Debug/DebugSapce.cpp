#include "DebugSapce.h"
#include "../FreeCamera.h"
#include "ResourceSystem.h"
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
	GetSubSystem<ResourceSystem>()->AddResourceDir("../Resource/");
	RegisterScene(new Level_0(_ambient, 0));
}

void DebugSapce::Start()
{
}
void DebugSapce::Update()
{
}
AUTO_APPLICATION_MAIN(DebugSapce)