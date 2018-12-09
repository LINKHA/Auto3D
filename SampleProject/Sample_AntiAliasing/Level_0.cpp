#include "Level_0.h"
#include "Light.h"
#include "Mesh.h"
#include "../FreeCamera.h"

Level_0::Level_0(Ambient* ambient, int id)
	:LevelScene(ambient, id)
{}
void Level_0::Awake()
{
	SceneSuper::Awake();
}
void Level_0::Start()
{
	SceneSuper::Start();

	Node* cameraObj = CreateNode();
	FreeCamera* camera = new FreeCamera(_ambient, _sceneID);
	camera->freeCamera->AllowOffScreen(true);
	camera->freeCamera->AllowMSAA(true, 4);
	cameraObj->GetComponent<Transform>()->SetPosition(0.0f, 0.0f, 3.0f);
	cameraObj->AddComponent(camera);


	Node* lightObj = CreateNode();
	auto* light = lightObj->CreateComponent<Light>();
	light->SetType(LightType::Directional);
	light->direction.Set(0.0f, -0.5f, -0.5f);


	Node* meshObj = CreateNode();
	auto* mesh = meshObj->CreateComponent<Mesh>();
	mesh->SetModel("../Resource/object/base/Cube.3DS");
	meshObj->GetComponent<Transform>()->SetPosition(0.0f, 0.0f, -3.0f);
}

void Level_0::Update()
{
	SceneSuper::Update();
}