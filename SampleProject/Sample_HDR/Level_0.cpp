#include "Level_0.h"
#include "Node.h"
#include "Light.h"
#include "MeshLight.h"
#include "../FreeCamera.h"

void Level_0::Start()
{
	Node* cameraObj = CreateNode();
	FreeCamera * freeCamera = new FreeCamera(_ambient);
	cameraObj->AddComponent(freeCamera);
	freeCamera->cameraNode->SetPosition(0.0f, 0.0f, 3.0f);
	freeCamera->camera->AllowHDR(true);

	Node* lightObj = CreateNode();
	Light* light = lightObj->CreateComponent<Light>();
	light->SetType(LightType::Point);
	light->SetColor(100.0f, 100.0f, 100.0f);
	lightObj->SetPosition(0.0f, 0.0f, 49.5f);

	for (int i = 0; i < 8; i++)
	{
		Node* lightObj2 = CreateNode();
		Light* light2 = lightObj2->CreateComponent<Light>();
		light2->SetType(LightType::Point);
		light2->SetColor(1.0f, 0.0f, 0.0f);
		lightObj2->SetPosition(0.0f, 2.0f, 3.0f + i * 5);
	}

	Node* meshObj = CreateNode();
	MeshLight* mesh = new MeshLight(_ambient);
	meshObj->SetPosition(0.0f, 0.0f, 25.0f);
	meshObj->SetScale(2.5f, 2.5f, 27.5f);
	meshObj->AddComponent(mesh);

}

void Level_0::Update()
{
}

