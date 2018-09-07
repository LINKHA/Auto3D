#include "Level_0.h"
#include "GameObject.h"
#include "LightDirectional.h"
#include "Mesh.h"
#include "SkyBox.h"
#include "SkyBoxReflectMesh.h"
#include "Configs.h"
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

	GameObject* skyBoxObj = new GameObject(_ambient, _levelNumber);
	SkyBox* skybox = new SkyBox(_ambient);
	skyBoxObj->AddComponent(skybox);

	GameObject* lightObj = new GameObject(_ambient, _levelNumber);
	Light * light = new LightDirectional(_ambient);
	lightObj->AddComponent(light);

	GameObject* meshObj = new GameObject(_ambient, _levelNumber);
	Mesh * mesh = new Mesh(_ambient);
	mesh->GetMaterial()->SetImage("../Resource/texture/wood.jpg");
	//mesh->GetMaterial()->color = Color(0.0f, 0.0f, 1.0f);
	meshObj->AddComponent(mesh);

	GameObject* meshObj2 = new GameObject(_ambient, _levelNumber);
	SkyBoxReflectMesh * mesh2 = new SkyBoxReflectMesh(_ambient);
	meshObj2->GetComponent(Transform).SetPosition(1.0f, 0.0f, 0.0f);
	meshObj2->AddComponent(mesh2);

	GameObject* meshObj3 = new GameObject(_ambient, _levelNumber);
	SkyBoxReflectMesh * mesh3 = new SkyBoxReflectMesh(_ambient,
		Shader(shader_path + "au_skybox_cube.auvs"
			, shader_path + "au_skybox_cube_refract.aufs"));
	meshObj3->GetComponent(Transform).SetPosition(2.0f, 0.0f, 0.0f);
	meshObj3->AddComponent(mesh3);
}


void Level_0::Update()
{
}

