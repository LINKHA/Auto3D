#include "Level_0.h"
#include "Light.h"
#include "Mesh.h"
#include "../FreeCamera.h"
#include "Configs.h"

void Level_0::Start()
{
	GameObject* cameraObj = CreateNode();
	FreeCamera* camera = new FreeCamera(_ambient, _sceneID);
	cameraObj->GetComponent<Transform>()->SetPosition(0.0f, 0.0f, 3.0f);
	cameraObj->AddComponent(camera);

	GameObject* lightObj = CreateNode();
	auto* light = lightObj->CreateComponent<Light>();
	light->SetType(LightType::kDirectional);

	lightObj->AddComponent(light);

	GameObject* meshObj = CreateNode();
	Shader shader(shader_path + "au_vertex_explode.auvs",
		shader_path + "au_vertex_explode.aufs",
		shader_path + "au_vertex_explode.augs");
	Mesh* mesh = new Mesh(_ambient, "../Resource/object/nanosuit/nanosuit.obj", shader);
	meshObj->AddComponent(mesh);

	GameObject* meshObj2 = CreateNode();
	Mesh* mesh2 = new Mesh(_ambient, "../Resource/object/nanosuit/nanosuit.obj");
	mesh2->GetMaterial()->isTexture = true;
	meshObj2->AddComponent(mesh2);
	meshObj2->GetComponent<Transform>()->SetPosition(10.0f, 0.0f, 0.0f);
}

void Level_0::Update()
{
}

