#include "Level_0.h"
#include "Node.h"
#include "Light.h"
#include "MeshRenderer.h"
#include "SkyBox.h"
#include "SkyBoxReflectMesh.h"
#include "../FreeCamera.h"
#include "ResourceSystem.h"
#include "Configs.h"

void Level_0::Start()
{
	auto* cube = GetSubSystem<ResourceSystem>()->GetResource<Mesh>("object/base/Cube.3DS");

	Node* cameraObj = CreateNode();
	FreeCamera* camera = new FreeCamera(_ambient, _sceneID);
	cameraObj->SetPosition(0.0f, 0.0f, 3.0f);
	cameraObj->AddComponent(camera);


	Node* skyBoxObj = CreateNode();
	SkyBox* skybox = new SkyBox(_ambient);
	skyBoxObj->AddComponent(skybox);

	Node* lightObj = CreateNode();
	Light* light = lightObj->CreateComponent<Light>();

	Node* meshObj = CreateNode();
	MeshRenderer* mesh = meshObj->CreateComponent<MeshRenderer>();
	mesh->SetMesh(cube);
	mesh->GetMaterial()->SetImage("../Resource/texture/wood.jpg");
	//mesh->GetMaterial()->color = Color(0.0f, 0.0f, 1.0f);
	meshObj->AddComponent(mesh);

	Node* meshObj2 = CreateNode();
	SkyBoxReflectMesh * mesh2 = new SkyBoxReflectMesh(_ambient);
	meshObj2->SetPosition(1.0f, 0.0f, 0.0f);
	meshObj2->AddComponent(mesh2);

	Node* meshObj3 = CreateNode();
	SkyBoxReflectMesh * mesh3 = new SkyBoxReflectMesh(_ambient,
		_Shader(shader_path + "au_skybox_cube.auvs"
			, shader_path + "au_skybox_cube_refract.aufs"));
	meshObj3->SetPosition(2.0f, 0.0f, 0.0f);
	meshObj3->AddComponent(mesh3);
}


void Level_0::Update()
{
}

