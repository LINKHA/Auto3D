#include "WorkSpace.h"
#include "Application.h"
#include "LightDirectional.h"
#include "FreeCamera.h"
#include "Mesh.h"
WorkSpace::WorkSpace()
{}
WorkSpace::~WorkSpace()
{}

void WorkSpace::Start()
{
	GameObject * cameraObj = new GameObject();
	FreeCamera * camera = new FreeCamera();
	cameraObj->GetComponent(Transform).SetPosition(0.0f, 0.0f, 3.0f);
	cameraObj->AddComponent(camera);

	GameObject * lightObj = new GameObject();
	Light * light = new LightDirectional();
	lightObj->AddComponent(light);

	GameObject * meshObj = new GameObject();
	Mesh * mesh = new Mesh("Resource/object/base/Cube.FBX");
	meshObj->AddComponent(mesh);

}

void WorkSpace::Update()
{
}

int WorkSpace::Launch()
{
	return INSTANCE(Application).Run();
}
