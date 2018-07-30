#include "DebugSapce.h"
#include "Application.h"
#include "LightDirectional.h"
#include "debug_test.h"
#include "../FreeCamera.h"
DebugSapce::DebugSapce(Ambient* ambient)
	:MotionSpace(ambient)
{}
DebugSapce::~DebugSapce()
{}

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