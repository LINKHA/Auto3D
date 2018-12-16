#include "Level_0.h"
#include "GameObject.h"
#include "LightDirectional.h"
#include "Mesh.h"
#include "Bloom.h"
#include "../FreeCamera.h"

void Level_0::Start()
{
	GameObject* cameraObj = new GameObject(_ambient,_levelNumber);
	FreeCamera* camera = new FreeCamera(_ambient, _levelNumber);
	cameraObj->GetComponent(Transform).SetPosition(0.0f, 0.0f, 3.0f);
	cameraObj->AddComponent(camera);

	GameObject* bloomObj = new GameObject(_ambient, _levelNumber);
	Bloom* bloom = new Bloom(_ambient);
	bloomObj->AddComponent(bloom);

}

void Level_0::Update()
{
}

