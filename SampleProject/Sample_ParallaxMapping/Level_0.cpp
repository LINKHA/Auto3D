#include "Level_0.h"
#include "TextureParallax.h"
#include "../FreeCamera.h"

void Level_0::Start()
{
	GameNode cameraObj = CreateNode();
	SharedPtr<FreeCamera> freeCamera = MakeShared<FreeCamera>(_ambient);
	cameraObj->AddComponent(freeCamera);
	freeCamera->cameraNode->SetPosition(0.0f, 0.0f, 3.0f);

	GameNode normalObj = CreateNode();
	auto normal = MakeShared<TextureParallax>(_ambient);
	normalObj->AddComponent(normal);

}

void Level_0::Update()
{
}


