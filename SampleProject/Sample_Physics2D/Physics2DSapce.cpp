#include "Physics2DSapce.h"
#include "Application.h"
#include "LightDirectional.h"
#include "../FreeCamera.h"
#include "Mesh.h"
#include "Sprite.h"
#include "Input.h"
#include "Time.h"
#include "AudioRun.h"
Physics2DSapce::Physics2DSapce(Ambient* ambient)
	:MotionSpace(ambient)
{}
Physics2DSapce::~Physics2DSapce()
{}

void Physics2DSapce::Start()
{
	//GameObject* cameraObj = new GameObject(_ambient);
	//cameraObj->RegisterObjectFactory(_ambient);
	//Print(cameraObj->GetAttach());
	//Run();
	Run2();
}
void Physics2DSapce::Update()
{
}
int Physics2DSapce::Launch()
{
	return INSTANCE(Application).Run(_ambient);
}
AUTO_APPLICATION_MAIN(Physics2DSapce)