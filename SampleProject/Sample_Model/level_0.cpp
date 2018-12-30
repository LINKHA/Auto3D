#include "Level_0.h"
#include "Node.h"
#include "Light.h"
#include "Mesh.h"
#include "../FreeCamera.h"
#include "SkyBox.h"
#include "ResourceSystem.h"
#include "MeshRenderer.h"


void Level_0::Start()
{
	Super::Start();

	GameNode cameraObj = CreateNode();
	SharedPtr<FreeCamera> freeCamera = MakeShared<FreeCamera>(_ambient);
	cameraObj->AddComponent(freeCamera);
	freeCamera->cameraNode->SetPosition(0.0f, 0.0f, 1.0f);
	

	GameNode lightObj = CreateNode();
	auto light = lightObj->CreateComponent<Light>();
	light->SetType(LightType::Directional);
	light->direction.Set(0.0f, -0.5f, -0.5f);

	auto mesh = GetSubSystem<ResourceSystem>()->GetResource<Mesh>("object/base/Cube.3DS");

	GameNode meshObj = CreateNode();
	auto meshRenderer = meshObj->CreateComponent<MeshRenderer>();
	meshRenderer->SetMesh(mesh);
	meshRenderer->GetMaterial()->SetImage("../Resource/texture/wood.jpg");
	//meshRenderer->GetMaterial()->color.Set(0.5f, 0.8f, 0.3f);
	meshObj->GetComponent<Transform>()->SetPosition(0.0f, 0.0f, -3.0f);
}

void Level_0::Update()
{
}


