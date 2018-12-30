#include "Level_0.h"
#include "Node.h"
#include "Light.h"
#include "MeshRenderer.h"
#include "../FreeCamera.h"
#include "ResourceSystem.h"


void Level_0::Start()
{
	auto cube = GetSubSystem<ResourceSystem>()->GetResource<Mesh>("object/base/Cube.3DS");

	GameNode cameraObj = CreateNode();
	auto camera = cameraObj->CreateComponent<Camera>();
	camera->SetViewRect(Rectf(0, 0, 0.5f, 0.5f));
	cameraObj->SetPosition(0.0f, 0.0f, 3.0f);
	//////////////////////////////////////////////////////////////////////////
	GameNode cameraObj2 = CreateNode();
	auto camera2 = cameraObj2->CreateComponent<Camera>();
	camera2->SetViewRect(Rectf(0.5f, 0, 0.5f, 0.5f));
	cameraObj2->SetPosition(0.0f, 0.0f, 3.0f);

	//////////////////////////////////////////////////////////////////////////
	GameNode freeCameraObj = CreateNode();
	SharedPtr<FreeCamera> freeCamera = MakeShared<FreeCamera>(_ambient);
	freeCameraObj->AddComponent(freeCamera);

	//////////////////////////////////////////////////////////////////////////
	GameNode lightObj = CreateNode();
	lightObj->SetPosition(2.0f, 0.0f, 0.0f);
	auto light = lightObj->CreateComponent<Light>();
	light->SetType(LightType::Point);
	//////////////////////////////////////////////////////////////////////////
	GameNode meshObj = CreateNode();
	auto meshRenderer = meshObj->CreateComponent<MeshRenderer>();
	meshRenderer->SetMesh(cube);
	meshObj->SetPosition(0.0f, 0.0f, -3.0f);

}
void Level_0::Update()
{

}
