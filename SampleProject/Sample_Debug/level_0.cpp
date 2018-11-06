#include "Level_0.h"
#include "GameObject.h"
#include "LightDirectional.h"
#include "Mesh.h"
#include "../FreeCamera.h"
#include "SkyBox.h"

Level_0::Level_0(Ambient* ambient, int levelNumber)
	:LevelScene(ambient, levelNumber)
{}

void Level_0::Start()
{
	GameObject* cameraObj = new GameObject(_ambient, _levelNumber);
	FreeCamera* camera = new FreeCamera(_ambient, _levelNumber);
	SkyBox* skybox = new SkyBox(_ambient);
	cameraObj->GetComponent(Transform).SetPosition(0.0f, 0.0f, 3.0f);
	cameraObj->AddComponent(camera);
	cameraObj->AddComponent(skybox);

	GameObject* lightObj = new GameObject(_ambient, _levelNumber);
	Light* light = new LightDirectional(_ambient);
	light->direction.Set(0.0f, -0.5f, -0.5f);
	lightObj->AddComponent(light);

	GameObject* meshObj = new GameObject(_ambient, _levelNumber);
	Mesh* mesh = new Mesh(_ambient, "../Resource/object/base/Cube.3DS");
	meshObj->GetComponent(Transform).SetPosition(0.0f, 0.0f, -3.0f);
	meshObj->AddComponent(mesh);

}

void Level_0::Update()
{
}


