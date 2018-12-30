#include "Level_1.h"
#include "Node.h"
#include "Light.h"
#include "MeshRenderer.h"
#include "MeshShadowPoint.h"
#include "ResourceSystem.h"
#include "../FreeCamera.h"

void Level_1::ShadowPoint()
{
	auto sphere = GetSubSystem<ResourceSystem>()->GetResource<Mesh>("object/base/Sphere.3DS");

	GameNode lightObj = CreateNode();
	lightObj->SetPosition(0.0f, 0.0f, -0.0f);
	auto light = lightObj->CreateComponent<Light>();
	light->SetType(LightType::Point);
	auto lightMesh = lightObj->CreateComponent<MeshRenderer>();
	lightMesh->SetMesh(sphere);
	lightMesh->GetMaterial()->color = Color(1.0f, 1.0f, 1.0f);

	GameNode shadowMeshObj = CreateNode();
	shadowMeshObj->SetScale(10.0f);
	auto mesh = MakeShared<MeshShadowPoint>(_ambient, false);
	shadowMeshObj->AddComponent(mesh);

	GameNode shadowMeshObj1 = CreateNode();
	shadowMeshObj1->SetPosition(4.0f, -3.5f, 0.0f);
	shadowMeshObj1->SetScale(0.5f);
	auto mesh1 = MakeShared<MeshShadowPoint>(_ambient);
	shadowMeshObj1->AddComponent(mesh1);

	GameNode shadowMeshObj2 = CreateNode();
	shadowMeshObj2->SetPosition(2.0f, 3.0f, 1.0f);
	shadowMeshObj2->SetScale(0.75f);
	auto mesh2 = MakeShared<MeshShadowPoint>(_ambient);
	shadowMeshObj2->AddComponent(mesh2);

	GameNode shadowMeshObj3 = CreateNode();
	shadowMeshObj3->SetPosition(-3.0f, -1.0f, 0.0f);
	shadowMeshObj3->SetScale(0.5f);
	auto mesh3 = MakeShared<MeshShadowPoint>(_ambient);
	shadowMeshObj3->AddComponent(mesh3);

	GameNode shadowMeshObj4 = CreateNode();
	shadowMeshObj4->SetPosition(-1.5f, 1.0f, 1.5f);
	shadowMeshObj4->SetScale(0.5f);
	auto mesh4 = MakeShared<MeshShadowPoint>(_ambient);
	shadowMeshObj4->AddComponent(mesh4);

	GameNode shadowMeshObj5 = CreateNode();
	shadowMeshObj5->SetPosition(-1.5f, 2.0f, -3.0f);
	shadowMeshObj5->SetRotation(60.0f, Vector3(1.0f, 0.0f, 1.0f));
	shadowMeshObj5->SetScale(0.75f);
	auto mesh5 = MakeShared<MeshShadowPoint>(_ambient);
	shadowMeshObj5->AddComponent(mesh5);

}
void Level_1::Start()
{
	GameNode cameraObj = CreateNode();
	auto camera = MakeShared<FreeCamera>(_ambient);
	cameraObj->AddComponent(camera);
	cameraObj->SetPosition(0.0f, 0.0f, 3.0f);


	ShadowPoint();
}

void Level_1::Update()
{
}
