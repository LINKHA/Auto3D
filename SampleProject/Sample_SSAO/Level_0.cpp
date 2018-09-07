#include "Level_0.h"
#include "GameObject.h"
#include "SSAO.h"
#include "Transform.h"
#include "../FreeCamera.h"

Level_0::Level_0(Ambient* ambient, int levelNumber)
	:LevelScene(ambient, levelNumber)
{}

void Level_0::Start()
{
	GameObject* cameraObj = new GameObject(_ambient,_levelNumber);
	FreeCamera* camera = new FreeCamera(_ambient, _levelNumber);
	cameraObj->GetComponent(Transform).SetPosition(0.0f, 0.0f, 3.0f);
	cameraObj->AddComponent(camera);

	GameObject* ssaoObj = new GameObject(_ambient, _levelNumber);
	SSAO* ssao = new SSAO(_ambient);
	ssaoObj->AddComponent(ssao);

}

void Level_0::Update()
{
}

