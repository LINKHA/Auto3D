#include "HDREquirectangularSpace.h"
#include "Application.h"
#include "LightDirectional.h"
#include "FreeCamera.h"
#include "HDREquirectangular.h"
HDREquirectangularSpace::HDREquirectangularSpace()
{}
HDREquirectangularSpace::~HDREquirectangularSpace()
{}

void HDREquirectangularSpace::Start()
{
	GameObject * cameraObj = new GameObject();
	FreeCamera * camera = new FreeCamera();
	cameraObj->GetComponent(Transform).SetPosition(0.0f, 0.0f, 3.0f);
	cameraObj->AddComponent(camera);

	GameObject * lightObj = new GameObject();
	Light * light = new LightDirectional();
	lightObj->AddComponent(light);

	GameObject * meshObj = new GameObject();
	HDREquirectangular * mesh = new HDREquirectangular();
	meshObj->AddComponent(mesh);

}

void HDREquirectangularSpace::Update()
{
}

int HDREquirectangularSpace::Launch()
{
	return INSTANCE(Application).Run();
}
