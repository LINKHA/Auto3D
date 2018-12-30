#include "Level_0.h"
#include "Node.h"
#include "Light.h"
#include "MeshLight.h"
#include "../FreeCamera.h"

void Level_0::Start()
{
	GameNode cameraObj = CreateNode();
	SharedPtr<FreeCamera> freeCamera = MakeShared<FreeCamera>(_ambient);
	cameraObj->AddComponent(freeCamera);
	freeCamera->cameraNode->SetPosition(0.0f, 0.0f, 3.0f);
	freeCamera->camera->AllowHDR(true);

	GameNode lightObj = CreateNode();
	auto light = lightObj->CreateComponent<Light>();
	light->SetType(LightType::Point);
	light->SetColor(100.0f, 100.0f, 100.0f);
	lightObj->SetPosition(0.0f, 0.0f, 49.5f);

	for (int i = 0; i < 8; i++)
	{
		GameNode lightObj2 = CreateNode();
		auto light2 = lightObj2->CreateComponent<Light>();
		light2->SetType(LightType::Point);
		light2->SetColor(1.0f, 0.0f, 0.0f);
		lightObj2->SetPosition(0.0f, 2.0f, 3.0f + i * 5);
	}

	GameNode meshObj = CreateNode();
	auto mesh = MakeShared<MeshLight>(_ambient);
	meshObj->SetPosition(0.0f, 0.0f, 25.0f);
	meshObj->SetScale(2.5f, 2.5f, 27.5f);
	meshObj->AddComponent(mesh);

}

void Level_0::Update()
{
}

