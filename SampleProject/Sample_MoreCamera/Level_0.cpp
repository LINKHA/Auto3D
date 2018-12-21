#include "Level_0.h"
#include "Node.h"
#include "Light.h"
#include "MeshRenderer.h"
#include "../FreeCamera.h"
#include "ResourceSystem.h"


void Level_0::Start()
{
	auto* cube = GetSubSystem<ResourceSystem>()->GetResource<Mesh>("object/base/Cube.3DS");

	Node* cameraObj = CreateNode();
	Camera* camera = cameraObj->CreateComponent<Camera>();
	camera->SetViewRect(Rectf(0, 0, 0.5f, 0.5f));
	cameraObj->SetPosition(0.0f, 0.0f, 3.0f);
	//////////////////////////////////////////////////////////////////////////
	Node* cameraObj2 = CreateNode();
	Camera* camera2 = cameraObj2->CreateComponent<Camera>();
	camera2->SetViewRect(Rectf(0.5f, 0, 0.5f, 0.5f));
	cameraObj2->SetPosition(0.0f, 0.0f, 3.0f);

	//////////////////////////////////////////////////////////////////////////
	Node* freeCameraObj = CreateNode();
	FreeCamera* freeCamera = new FreeCamera(_ambient, _sceneID);
	freeCameraObj->AddComponent(freeCamera);

	//////////////////////////////////////////////////////////////////////////
	Node* lightObj = CreateNode();
	lightObj->SetPosition(2.0f, 0.0f, 0.0f);
	Light* light = lightObj->CreateComponent<Light>();
	light->SetType(LightType::Point);
	//////////////////////////////////////////////////////////////////////////
	Node* meshObj = CreateNode();
	auto* meshRenderer = meshObj->CreateComponent<MeshRenderer>();
	meshRenderer->SetMesh(cube);
	meshObj->SetPosition(0.0f, 0.0f, -3.0f);

}
void Level_0::Update()
{

}
