#include "Level_0.h"
#include "Node.h"
#include "Light.h"
#include "MeshRenderer.h"
#include "ResourceSystem.h"
#include "../FreeCamera.h"

void Level_0::Start()
{
	auto* mesh = GetSubSystem<ResourceSystem>()->GetResource<Mesh>("object/base/Cube.3DS");

	Node* lightObj = CreateNode();
	lightObj->GetComponent<Transform>()->SetPosition(2.0f, 5.0f, 0.0f);
	Light* light = lightObj->CreateComponent<Light>();
	light->SetType(LightType::Directional);
	/////////////////////////////////////////////////////////////////////////////////////////////
	Node* camObj = CreateNode();
	FreeCamera* cam = new FreeCamera(_ambient, _sceneID);
	camObj->AddComponent(cam);
	/////////////////////////////////////////////////////////////////////////////////////////////
	Node* obj1 = CreateNode();
	MeshRenderer* plane = obj1->CreateComponent<MeshRenderer>();
	plane->SetMesh(mesh);
	plane->GetMaterial()->color.Set(0.5f, 0.5f, 0.5f);
	obj1->SetScale(10.0f, 0.1f, 10.0f);
	/////////////////////////////////////////////////////////////////////////////////////////////
	Node* obj2 = CreateNode();
	MeshRenderer* box = obj2->CreateComponent<MeshRenderer>();
	box->SetMesh(mesh);
	box->EnableDepth(false);
	obj2->SetPosition(1.0f, 0.5f, 3.0f);
	/////////////////////////////////////////////////////////////////////////////////////////////
	Node* obj3 = CreateNode();
	MeshRenderer* box2 = obj3->CreateComponent<MeshRenderer>();
	box2->SetMesh(mesh);
	box2->EnableDepth(false);
	obj3->SetPosition(3.0f, 0.5f, 2.0f);
}

void Level_0::Update()
{
}


