#include "HDRSpace.h"
#include "Application.h"
#include "Light.h"
#include "../FreeCamera.h"
#include "Mesh.h"
#include "LightPoint.h"
#include "LightPoint.h"
#include "MeshLight.h"

HDRSpace::HDRSpace(Ambient* ambient)
	:MotionSpace(ambient)
{}
HDRSpace::~HDRSpace()
{}

void HDRSpace::Start()
{
	GameObject * cameraObj = new GameObject(_ambient);
	FreeCamera * camera = new FreeCamera(_ambient);
	camera->freeCamera->AllowHDR(true);
	cameraObj->GetComponent(Transform).SetPosition(0.0f, 0.0f, 3.0f);
	cameraObj->AddComponent(camera);


	GameObject* lightObj = new GameObject(_ambient);
	Light* light = new LightPoint(_ambient);
	light->SetColor(100.0f, 100.0f, 100.0f);
	lightObj->GetComponent(Transform).SetPosition(0.0f, 0.0f, 49.5f);
	lightObj->AddComponent(light);

	for (int i = 0; i < 8; i++)
	{
		GameObject* lightObj2 = new GameObject(_ambient);
		Light* light2 = new LightPoint(_ambient);
		light2->SetColor(1.0f, 0.0f, 0.0f);
		lightObj2->GetComponent(Transform).SetPosition(0.0f, 2.0f, 3.0f+i*5);
		lightObj2->AddComponent(light2);
	}
	



	GameObject* meshObj = new GameObject(_ambient);
	MeshLight* mesh = new MeshLight(_ambient);
	meshObj->GetComponent(Transform).SetPosition(0.0f, 0.0f, 25.0f);
	meshObj->GetComponent(Transform).SetScale(2.5f, 2.5f, 27.5f);
	meshObj->AddComponent(mesh);


}

void HDRSpace::Update()
{
}

int HDRSpace::Launch()
{
	return INSTANCE(Application).Run(_ambient);
}
AUTO_APPLICATION_MAIN(HDRSpace)