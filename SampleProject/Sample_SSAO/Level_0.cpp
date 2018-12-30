#include "Level_0.h"
#include "Node.h"
#include "SSAO.h"
#include "Transform.h"
#include "../FreeCamera.h"

void Level_0::Start()
{
	GameNode cameraObj = CreateNode();
	SharedPtr<FreeCamera> freeCamera = MakeShared<FreeCamera>(_ambient);
	cameraObj->AddComponent(freeCamera);
	freeCamera->cameraNode->SetPosition(0.0f, 0.0f, 3.0f);

	GameNode ssaoObj = CreateNode();
	auto ssao = MakeShared<SSAO>(_ambient);
	ssaoObj->AddComponent(ssao);

}

void Level_0::Update()
{
}

