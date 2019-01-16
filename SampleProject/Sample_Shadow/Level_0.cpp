#include "Level_0.h"
#include "Node.h"
#include "Light.h"
#include "MeshRenderer.h"
#include "MeshShadow.h"
#include "ResourceSystem.h"
#include "../FreeCamera.h"

void Level_0::ShadowNormal()
{
	auto sphere = GetSubSystem<ResourceSystem>()->GetResource<Mesh>("object/base/Sphere.3DS");

	GameNode lightObj = CreateNode();
	lightObj->SetPosition(-2.0f, 4.0f, -5.0f);
	auto light = lightObj->CreateComponent<Light>();
	light->SetType(LightType::Directional);
	auto lightMesh = lightObj->CreateComponent<MeshRenderer>();
	lightMesh->GetMaterial()->color = Color(1.0f, 1.0f, 1.0f);
	lightMesh->SetMesh(sphere);

	GameNode shadowMeshObj = CreateNode();
	shadowMeshObj->SetPosition(0.0f, -1.0f, 0.0f);
	shadowMeshObj->SetScale(10.0f, 0.5f, 10.0f);
	auto mesh = MakeShared<MeshShadow>(_ambient);
	shadowMeshObj->AddComponent(mesh);


	GameNode shadowMeshObj1 = CreateNode();
	shadowMeshObj1->SetPosition(2.0f, 0.0f, 1.0f);
	shadowMeshObj1->SetScale(0.5f);
	auto mesh1 = MakeShared<MeshShadow>(_ambient);
	shadowMeshObj1->AddComponent(mesh1);

	GameNode shadowMeshObj2 = CreateNode();
	shadowMeshObj2->SetPosition(-1.0f, 0.0f, 2.0f);
	shadowMeshObj2->SetRotation(60.0f, Vector3(1.0f, 0.0f, 1.0f));
	shadowMeshObj2->SetScale(0.25f);
	auto mesh2 = MakeShared<MeshShadow>(_ambient);
	shadowMeshObj2->AddComponent(mesh2);

	GameNode shadowMeshObj3 = CreateNode();
	shadowMeshObj3->SetPosition(0.0f, 1.5f, 0.0);
	shadowMeshObj3->SetScale(0.5f);
	auto mesh3 = MakeShared<MeshShadow>(_ambient);
	shadowMeshObj3->AddComponent(mesh3);

	GameNode shadowMeshObj4 = CreateNode();
	shadowMeshObj4->SetPosition(2.0f, 2.0f, 1.0f);
	shadowMeshObj4->SetScale(0.5f);
	auto mesh4= MakeShared<MeshShadow>(_ambient);
	shadowMeshObj4->AddComponent(mesh4);
}

void Level_0::Start()
{
	GameNode cameraObj = CreateNode();
	auto camera = MakeShared<FreeCamera>(_ambient);
	cameraObj->AddComponent(camera);

	cameraObj->SetPosition(0.0f, 0.0f, 3.0f);

	ShadowNormal();

}

void Level_0::Update()
{

}
