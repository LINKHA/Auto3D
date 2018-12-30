#include "Level_0.h"
#include "Node.h"
#include "Light.h"
#include "MeshRenderer.h"
#include "ResourceSystem.h"
#include "../FreeCamera.h"

void Level_0::Start()
{
	auto mesh = GetSubSystem<ResourceSystem>()->GetResource<Mesh>("object/base/Cube.3DS");
	
	GameNode camObj = CreateNode();
	SharedPtr<FreeCamera> freeCamera = MakeShared<FreeCamera>(_ambient);
	camObj->AddComponent(freeCamera);
	freeCamera->cameraNode->SetPosition(0.0f, 1.0f, 10.0f);
	/////////////////////////////////////////////////////////////////////////////////////////////
	GameNode lightObj = CreateNode();
	lightObj->SetPosition(2.0f, 5.0f, 0.0f);
	auto light = lightObj->CreateComponent<Light>();
	light->SetType(LightType::Directional);
	/////////////////////////////////////////////////////////////////////////////////////////////
	GameNode obj1 = CreateNode();
	auto plane = obj1->CreateComponent<MeshRenderer>();
	plane->SetMesh(mesh);
	plane->GetMaterial()->color.Set(0.5f, 0.5f, 0.5f);
	obj1->SetScale(10.0f, 0.1f, 10.0f);
	/////////////////////////////////////////////////////////////////////////////////////////////
	GameNode obj2 = CreateNode();
	auto box = obj2->CreateComponent<MeshRenderer>();
	box->SetMesh(mesh);
	box->EnableDepth(false);
	obj2->SetPosition(1.0f, 0.5f, 3.0f);
	/////////////////////////////////////////////////////////////////////////////////////////////
	GameNode obj3 = CreateNode();
	auto box2 = obj3->CreateComponent<MeshRenderer>();
	box2->SetMesh(mesh);
	box2->EnableDepth(false);
	obj3->SetPosition(3.0f, 0.5f, 2.0f);
	/////////////////////////////////////////////////////////////////////////////////////////////
	GameNode obj4 = CreateNode();
	auto box3 = obj4->CreateComponent<MeshRenderer>();
	box3->SetMesh(mesh);
	box3->EnableDepth(false);
	obj4->SetPosition(-2.0f, 0.5f, 1.0f);
}

void Level_0::Update()
{
}


