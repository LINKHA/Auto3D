#include "Level_0.h"
#include "Node.h"
#include "Light.h"
#include "MeshRenderer.h"
#include "SkyBox.h"
#include "SkyBoxReflectMesh.h"
#include "../FreeCamera.h"
#include "ResourceSystem.h"

void Level_0::Start()
{
	auto cube = GetSubSystem<ResourceSystem>()->GetResource<Mesh>("object/base/Cube.3DS");

	GameNode cameraObj = CreateNode();
	SharedPtr<FreeCamera> freeCamera = MakeShared<FreeCamera>(_ambient);
	cameraObj->AddComponent(freeCamera);
	freeCamera->cameraNode->SetPosition(0.0f, 0.0f, 3.0f);

	GameNode skyBoxObj = CreateNode();
	auto skybox = skyBoxObj->CreateComponent<SkyBox>();

	GameNode lightObj = CreateNode();
	auto light = lightObj->CreateComponent<Light>();

	GameNode meshObj = CreateNode();
	auto mesh = meshObj->CreateComponent<MeshRenderer>();
	mesh->SetMesh(cube);
	mesh->GetMaterial()->SetImage("../Resource/texture/wood.jpg");
	//mesh->GetMaterial()->color = Color(0.0f, 0.0f, 1.0f);

	GameNode meshObj2 = CreateNode();
	auto mesh2 = MakeShared<SkyBoxReflectMesh>(_ambient);
	meshObj2->SetPosition(1.0f, 0.0f, 0.0f);
	meshObj2->AddComponent(mesh2);

	GameNode meshObj3 = CreateNode();
	auto shader = GetSubSystem<ResourceSystem>()->GetResource<Shader>("shader/au_skybox_cube_refract.glsl");
	auto mesh3 = MakeShared<SkyBoxReflectMesh>(_ambient,shader);
	
	meshObj3->SetPosition(2.0f, 0.0f, 0.0f);
	meshObj3->AddComponent(mesh3);
}


void Level_0::Update()
{
}

