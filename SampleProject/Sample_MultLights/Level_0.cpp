#include "Level_0.h"
#include "Node.h"
#include "Light.h"
#include "Mesh.h"
#include "MeshRenderer.h"
#include "ResourceSystem.h"
#include "Image.h"
#include "../FreeCamera.h"


void Level_0::Start()
{
	auto* wood = GetSubSystem<ResourceSystem>()->GetResource<Image>("texture/wood.jpg");
	auto* cube = GetSubSystem<ResourceSystem>()->GetResource<Mesh>("object/base/Cube.3DS");

	Node* camObj = CreateNode();
	FreeCamera* freeCamera = new FreeCamera(_ambient);
	camObj->AddComponent(freeCamera);
	freeCamera->cameraNode->SetPosition(0.0f, 0.0f, 3.0f);
	//////////////////////////////////////////////////////////////////////////
	Node* lightObj = CreateNode();
	lightObj->SetPosition(0.0f, 0.0f, -3.0f);
	Light* light = lightObj->CreateComponent<Light>();
	light->SetType(LightType::Point);
	//////////////////////////////////////////////////////////////////////////
	Node* lightObj2 = CreateNode();
	lightObj2->SetPosition(0.0f, 0.0f, 3.0f);
	Light* light2 = lightObj2->CreateComponent<Light>();
	light2->SetType(LightType::Spot);
	light2->direction.Set(0.0f, 0.0f, -1.0f);
	//////////////////////////////////////////////////////////////////////////
	Node* lightObj3 = CreateNode();
	Light* light3 = lightObj3->CreateComponent<Light>();
	light3->SetType(LightType::Directional);
	light3->ambient.Set(0.1f, 0.1f, 0.1f);
	light3->direction.Set(0.0f, -1.0f, 0.0f);
	//////////////////////////////////////////////////////////////////////////
	Node* meshObj = CreateNode();
	meshObj->SetScale(2.0f, 2.0f, 2.0f);
	auto* mesh = meshObj->CreateComponent<MeshRenderer>();
	mesh->SetMesh(cube);
	mesh->GetMaterial()->SetImage("../Resource/texture/wood.jpg");
	mesh->GetMaterial()->color.Set(0.5f, 0.8f, 0.3f);
}

void Level_0::Update()
{

}

