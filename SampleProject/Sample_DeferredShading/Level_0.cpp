#include "Level_0.h"
#include "DeferredShading.h"
#include "Transform.h"
#include "../FreeCamera.h"

void Level_0::Start()
{
	Node* cameraObj = CreateNode();
	FreeCamera* camera = new FreeCamera(_ambient, _sceneID);
	cameraObj->GetComponent<Transform>()->SetPosition(0.0f, 0.0f, 3.0f);
	cameraObj->AddComponent(camera);


	Node* deferredObj = CreateNode();
	DeferredShading* deferred = new DeferredShading(_ambient);
	deferredObj->AddComponent(deferred);
}

void Level_0::Update()
{

}

