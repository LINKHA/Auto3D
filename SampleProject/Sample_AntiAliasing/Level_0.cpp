#include "Level_0.h"
#include "GameObject.h"
#include "LightDirectional.h"
#include "Mesh.h"
#include "../FreeCamera.h"
Level_0::Level_0(Ambient* ambient, int id)
	:LevelScene(ambient, id)
{}

void Level_0::Start()
{
	Node* cameraObj = new Node(_ambient, _id);
	FreeCamera* camera = new FreeCamera(_ambient, _id);
	camera->freeCamera->AllowOffScreen(true);
	camera->freeCamera->AllowMSAA(true, 4);
	cameraObj->GetComponent(Transform).SetPosition(0.0f, 0.0f, 3.0f);
	cameraObj->AddComponent(camera);


	Node* lightObj = new Node(_ambient, _id);
	Light* light = new LightDirectional(_ambient);
	light->direction.Set(0.0f, -0.5f, -0.5f);
	lightObj->AddComponent(light);

	Node* meshObj = new Node(_ambient, _id);
	Mesh* mesh = new Mesh(_ambient, "../Resource/object/base/Cube.3DS");
	meshObj->GetComponent(Transform).SetPosition(0.0f, 0.0f, -3.0f);
	meshObj->AddComponent(mesh);

}

void Level_0::Update()
{
}

