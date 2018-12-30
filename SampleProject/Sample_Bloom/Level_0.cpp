#include "Level_0.h"
#include "Node.h"
#include "Light.h"
#include "Mesh.h"
#include "Bloom.h"
#include "../FreeCamera.h"

void Level_0::Start()
{
	GameNode cameraObj = CreateNode();
	SharedPtr<FreeCamera> freeCamera = MakeShared<FreeCamera>(_ambient);
	cameraObj->AddComponent(freeCamera);
	freeCamera->cameraNode->SetPosition(0.0f, 0.0f, 4.0f);


	GameNode bloomObj = CreateNode();
	auto bloom = MakeShared<Bloom>(_ambient);
	bloomObj->AddComponent(bloom);

}

void Level_0::Update()
{
}

