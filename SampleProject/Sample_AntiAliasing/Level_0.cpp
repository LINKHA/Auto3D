#include "Level_0.h"
#include "Light.h"
#include "MeshRenderer.h"
#include "ResourceSystem.h"
#include "Graphics.h"
#include "../FreeCamera.h"


void Level_0::Awake()
{
	Super::Awake();
}
void Level_0::Start()
{
	Super::Start();

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

	auto* mesh = GetSubSystem<ResourceSystem>()->GetResource<Mesh>("object/base/Cube.3DS");

	Node* meshObj = CreateNode();
	auto* meshRenderer = meshObj->CreateComponent<MeshRenderer>();
	meshRenderer->SetMesh(mesh);
	meshObj->GetComponent<Transform>()->SetPosition(0.0f, 0.0f, -3.0f);

}

void Level_0::Update()
{
	Super::Update();
}