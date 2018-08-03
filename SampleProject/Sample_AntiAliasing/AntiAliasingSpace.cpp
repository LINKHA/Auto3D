#include "AntiAliasingSpace.h"
#include "Application.h"
#include "LightDirectional.h"
#include "Mesh.h"
#include "../FreeCamera.h"

AntiAliasingSpace::AntiAliasingSpace(Ambient* ambient)
	:MotionSpace(ambient)
{}
AntiAliasingSpace::~AntiAliasingSpace()
{}

void AntiAliasingSpace::Start()
{
	GameObject* cameraObj = new GameObject(_ambient);
	FreeCamera* camera = new FreeCamera(_ambient);
	camera->freeCamera->AllowMSAA(true,4);
	cameraObj->GetComponent(Transform).SetPosition(0.0f, 0.0f, 3.0f);
	cameraObj->AddComponent(camera);

	GameObject* lightObj = new GameObject(_ambient);
	Light* light = new LightDirectional(_ambient);
	lightObj->AddComponent(light);

	GameObject* meshObj = new GameObject(_ambient);
	Mesh* mesh = new Mesh(_ambient,"../Resource/object/base/Cube.FBX");
	meshObj->AddComponent(mesh);

}

void AntiAliasingSpace::Update()
{
}

int AntiAliasingSpace::Launch()
{
	return INSTANCE(Application).Run(_ambient);
}
AUTO_APPLICATION_MAIN(AntiAliasingSpace)