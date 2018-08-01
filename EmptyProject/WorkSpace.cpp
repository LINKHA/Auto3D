#include "WorkSpace.h"
#include "Application.h"
#include "LightDirectional.h"
#include "FreeCamera.h"
#include "Mesh.h"
WorkSpace::WorkSpace(Ambient* ambient)
	:MotionSpace(ambient)
{}
WorkSpace::~WorkSpace()
{}

void WorkSpace::Start()
{
	GameObject * cameraObj = new GameObject(_ambient);
	FreeCamera * camera = new FreeCamera(_ambient);
	cameraObj->GetComponent(Transform).SetPosition(0.0f, 0.0f, 3.0f);
	cameraObj->AddComponent(camera);

	GameObject * lightObj = new GameObject(_ambient);
	Light * light = new LightDirectional(_ambient);
	lightObj->AddComponent(light);

	//GameObject * meshObj = new GameObject(_ambient);
	//Mesh * mesh = new Mesh(_ambient,"Resource/object/base/Cube.FBX");
	//meshObj->AddComponent(mesh);

}

void WorkSpace::Update()
{
}

int WorkSpace::Launch()
{
	return INSTANCE(Application).Run(_ambient);
}
AUTO_APPLICATION_MAIN(WorkSpace)