#include "Level_0.h"
#include "Node.h"
#include "SSAO.h"
#include "Transform.h"
#include "../FreeCamera.h"

void Level_0::Start()
{
	GameObject* cameraObj = CreateNode();
	FreeCamera* camera = new FreeCamera(_ambient);
	cameraObj->AddComponent(camera);
	camera->cameraNode->SetPosition(0.0f, 0.0f, 3.0f);

	GameObject* ssaoObj = CreateNode();
	SSAO* ssao = new SSAO(_ambient);
	ssaoObj->AddComponent(ssao);

}

void Level_0::Update()
{
}

