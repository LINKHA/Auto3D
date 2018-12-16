#include "Level_0.h"
#include "GameObject.h"
#include "TextureParallax.h"
#include "../FreeCamera.h"

void Level_0::Start()
{
	GameObject* cameraObj = new GameObject(_ambient,_levelNumber);
	FreeCamera* camera = new FreeCamera(_ambient, _levelNumber);
	cameraObj->GetComponent(Transform).SetPosition(0.0f, 0.0f, 3.0f);
	cameraObj->AddComponent(camera);

	GameObject* normalObj = new GameObject(_ambient, _levelNumber);
	TextureParallax * normal = new TextureParallax(_ambient);
	normalObj->AddComponent(normal);

}

void Level_0::Update()
{
}


