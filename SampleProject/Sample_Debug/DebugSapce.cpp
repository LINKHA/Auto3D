#include "DebugSapce.h"
#include "Application.h"
#include "../FreeCamera.h"
#include "level_0.h"
DebugSapce::DebugSapce(Ambient* ambient)
	:MotionSpace(ambient)
{}
DebugSapce::~DebugSapce()
{
	RemoveLevel(0);
}
void DebugSapce::Awake()
{
	RegisterLevel(new Level_0(_ambient, 0));
}

void DebugSapce::Start()
{
}
void DebugSapce::Update()
{
}
int DebugSapce::Launch()
{
	return INSTANCE(Application).Run(_ambient);
}
AUTO_APPLICATION_MAIN(DebugSapce)