#include "DebugSapce.h"
#include "Application.h"
#include "LightDirectional.h"
#include "../FreeCamera.h"
#include "Mesh.h"
#include "Sprite.h"
#include "Input.h"
#include "Time.h"
DebugSapce::DebugSapce(Ambient* ambient)
	:MotionSpace(ambient)
{}
DebugSapce::~DebugSapce()
{}

void DebugSapce::Start()
{
	//GameObject* cameraObj = new GameObject(_ambient);
	//cameraObj->RegisterObjectFactory(_ambient);
	//Print(cameraObj->GetAttach());
}
void DebugSapce::Update()
{
}
int DebugSapce::Launch()
{
	return INSTANCE(Application).Run(_ambient);
}
AUTO_APPLICATION_MAIN(DebugSapce)