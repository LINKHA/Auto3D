#include "Level_0.h"
#include "DeferredShading.h"
#include "Transform.h"
#include "../FreeCamera.h"

void Level_0::Start()
{
	Node* cameraObj = CreateNode();
	FreeCamera* camera = new FreeCamera(_ambient);
	cameraObj->AddComponent(camera);
	camera->cameraNode->SetPosition(0.0f, 0.0f, 10.0f);


	Node* deferredObj = CreateNode();
	DeferredShading* deferred = new DeferredShading(_ambient);
	deferredObj->AddComponent(deferred);
}

void Level_0::Update()
{

}

