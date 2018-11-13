#include "Level_0.h"
#include "GameObject.h"
#include "LightDirectional.h"
#include "Mesh.h"
#include "../FreeCamera.h"
#include "LogAssert.h"

Level_0::Level_0(Ambient* ambient, int id)
	:LevelScene(ambient, id)
{}

void Level_0::Start()
{
	GameObject* cameraObj = CreateNode();
	FreeCamera* camera = new FreeCamera(_ambient, _sceneID);
	camera->freeCamera->AllowOffScreen(true);
	camera->freeCamera->AllowMSAA(true, 4);
	cameraObj->GetComponent(Transform).SetPosition(0.0f, 0.0f, 3.0f);
	cameraObj->AddComponent(camera);


	GameObject* lightObj = CreateNode();
	//auto* light = lightObj->CreateComponent<LightDirectional>();
	//Print(light->GetClassString());
	auto* light = new LightDirectional(_ambient);
	lightObj->AddComponent(light);
	light->direction.Set(0.0f, -0.5f, -0.5f);



	GameObject* meshObj = CreateNode();
	Mesh* mesh = new Mesh(_ambient, "../Resource/object/base/Cube.3DS");
	meshObj->GetComponent(Transform).SetPosition(0.0f, 0.0f, -3.0f);
	meshObj->AddComponent(mesh);

}

void Level_0::Update()
{
}

