#include "Level_0.h"
#include "Light.h"
#include "MeshRenderer.h"
#include "ResourceSystem.h"
#include "../FreeCamera.h"


void Level_0::Awake()
{
	Super::Awake();
}
void Level_0::Start()
{
	Super::Start();

	auto mesh = GetSubSystem<ResourceSystem>()->GetResource<Mesh>("object/base/Cube.3DS");

	GameNode cameraObj = CreateNode();
	SharedPtr<FreeCamera> freeCamera = MakeShared<FreeCamera>(_ambient);
	cameraObj->AddComponent(freeCamera);
	freeCamera->camera->AllowOffScreen(true);
	freeCamera->camera->AllowMSAA(true, 4);

	GameNode lightObj = CreateNode();
	auto light = lightObj->CreateComponent<Light>();
	light->SetType(LightType::Directional);
	light->direction.Set(0.0f, -0.5f, -0.5f);

	GameNode meshObj = CreateNode();
	auto meshRenderer = meshObj->CreateComponent<MeshRenderer>();
	meshRenderer->SetMesh(mesh);
	meshObj->GetComponent<Transform>()->SetPosition(0.0f, 0.0f, -3.0f);

}

void Level_0::Update()
{
	Super::Update();
}