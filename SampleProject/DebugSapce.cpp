#include "DebugSapce.h"
#include "Application.h"
#include "LightDirectional.h"
#include "FreeCamera.h"
#include "debug_test.h"
#include "SampleSet.h"
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
	return INSTANCE(Application).Run();
}
#if uDebugSpace
AUTO_APPLICATION_MAIN(DebugSapce)
#endif 