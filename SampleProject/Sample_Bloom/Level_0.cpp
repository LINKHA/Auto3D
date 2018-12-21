#include "Level_0.h"
#include "Node.h"
#include "Light.h"
#include "Mesh.h"
#include "Bloom.h"
#include "../FreeCamera.h"

void Level_0::Start()
{
	Node* cameraObj = CreateNode();
	FreeCamera* camera = new FreeCamera(_ambient, _sceneID);
	cameraObj->GetComponent<Transform>()->SetPosition(0.0f, 0.0f, 3.0f);
	cameraObj->AddComponent(camera);

	Node* bloomObj = CreateNode();
	Bloom* bloom = new Bloom(_ambient);
	bloomObj->AddComponent(bloom);

}

void Level_0::Update()
{
}

