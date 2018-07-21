#include "DebugSapce.h"
#include "Application.h"
#include "LightDirectional.h"
#include "FreeCamera.h"
#include "debug_test.h"
#include "SampleSet.h"
DebugSapce::DebugSapce()
{}
DebugSapce::~DebugSapce()
{}

void DebugSapce::Start()
{
	GameObject* obj = new GameObject();
	Component* debug = new debug_test();
	Print(debug->GetSharedPtrTypeStringVirtual());
	obj->AddComponent(debug);
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