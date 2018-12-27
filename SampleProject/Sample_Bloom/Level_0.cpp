#include "Level_0.h"
#include "Node.h"
#include "Light.h"
#include "Mesh.h"
#include "Bloom.h"
#include "../FreeCamera.h"

void Level_0::Start()
{
	Node* cameraObj = CreateNode();
	FreeCamera* camera = new FreeCamera(_ambient);
	cameraObj->AddComponent(camera);
	camera->cameraNode->SetPosition(0.0f, 0.0f, 4.0f);


	Node* bloomObj = CreateNode();
	Bloom* bloom = new Bloom(_ambient);
	bloomObj->AddComponent(bloom);

}

void Level_0::Update()
{
}

