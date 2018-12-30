#include "Level_0.h"
#include "DeferredShading.h"
#include "Transform.h"
#include "../FreeCamera.h"

void Level_0::Start()
{
	GameNode cameraObj = CreateNode();
	SharedPtr<FreeCamera> freeCamera = MakeShared<FreeCamera>(_ambient);
	cameraObj->AddComponent(freeCamera);
	freeCamera->cameraNode->SetPosition(0.0f, 0.0f, 10.0f);


	GameNode deferredObj = CreateNode();
	auto deferred = MakeShared<DeferredShading>(_ambient);
	deferredObj->AddComponent(deferred);
}

void Level_0::Update()
{

}

