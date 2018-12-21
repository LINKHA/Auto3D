#include "Level_0.h"
#include "TextureParallax.h"
#include "../FreeCamera.h"

void Level_0::Start()
{
	Node* cameraObj = CreateNode();
	FreeCamera* camera = new FreeCamera(_ambient, _sceneID);
	cameraObj->SetPosition(0.0f, 0.0f, 3.0f);
	cameraObj->AddComponent(camera);

	Node* normalObj = CreateNode();
	TextureParallax* normal = new TextureParallax(_ambient);
	normalObj->AddComponent(normal);

}

void Level_0::Update()
{
}


