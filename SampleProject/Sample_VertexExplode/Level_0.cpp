#include "Level_0.h"
#include "GameObject.h"
#include "LightDirectional.h"
#include "Mesh.h"
#include "../FreeCamera.h"
#include "Configs.h"

Level_0::Level_0(Ambient* ambient, int levelNumber)
	:LevelScene(ambient, levelNumber)
{}

void Level_0::Start()
{
	GameObject* cameraObj = new GameObject(_ambient,_levelNumber);
	FreeCamera* camera = new FreeCamera(_ambient, _levelNumber);
	cameraObj->GetComponent(Transform).SetPosition(0.0f, 0.0f, 3.0f);
	cameraObj->AddComponent(camera);

	GameObject* lightObj = new GameObject(_ambient, _levelNumber);
	Light* light = new LightDirectional(_ambient);
	lightObj->AddComponent(light);

	GameObject* meshObj = new GameObject(_ambient, _levelNumber);
	Shader shader(shader_path + "au_vertex_explode.auvs",
		shader_path + "au_vertex_explode.aufs",
		shader_path + "au_vertex_explode.augs");
	Mesh* mesh = new Mesh(_ambient, "../Resource/object/nanosuit/nanosuit.obj", shader);
	meshObj->AddComponent(mesh);

	GameObject* meshObj2 = new GameObject(_ambient, _levelNumber);
	Mesh* mesh2 = new Mesh(_ambient, "../Resource/object/nanosuit/nanosuit.obj");
	mesh2->GetMaterial()->isTexture = true;
	meshObj2->AddComponent(mesh2);
	meshObj2->GetComponent(Transform).SetPosition(10.0f, 0.0f, 0.0f);
}

void Level_0::Update()
{
}

