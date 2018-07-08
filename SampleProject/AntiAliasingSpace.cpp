#include "AntiAliasingSpace.h"
#include "Application.h"
#include "Light.h"
#include "FreeCamera.h"
#include "Mesh.h"
AntiAliasingSpace::AntiAliasingSpace()
{}
AntiAliasingSpace::~AntiAliasingSpace()
{}

void AntiAliasingSpace::Start()
{
	GameObject * cameraObj = new GameObject();
	FreeCamera * camera = new FreeCamera();
	cameraObj->GetComponent(Transform).SetPosition(0.0f, 0.0f, 3.0f);
	cameraObj->AddComponent(camera);

	GameObject * lightObj = new GameObject();
	Light * light = new Light(Directional);
	lightObj->AddComponent(light);

	GameObject * meshObj = new GameObject();
	Mesh * mesh = new Mesh("Resource/object/base/Cube.FBX");
	meshObj->AddComponent(mesh);

}

void AntiAliasingSpace::Update()
{
}

int AntiAliasingSpace::Launch()
{
	return INSTANCE(Application).Run();
}
