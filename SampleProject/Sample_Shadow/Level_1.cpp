#include "Level_1.h"
#include "Node.h"
#include "Light.h"
#include "MeshRenderer.h"
#include "MeshShadowPoint.h"
#include "ResourceSystem.h"

void Level_1::ShadowPoint()
{
	auto* sphere = GetSubSystem<ResourceSystem>()->GetResource<Mesh>("object/base/Sphere.3DS");

	Node* lightObj = CreateNode();
	lightObj->SetPosition(0.0f, 0.0f, -0.0f);
	Light* light = lightObj->CreateComponent<Light>();
	light->SetType(LightType::Point);
	MeshRenderer* lightMesh = lightObj->CreateComponent<MeshRenderer>();
	lightMesh->SetMesh(sphere);
	lightMesh->GetMaterial()->color = Color(1.0f, 1.0f, 1.0f);

	Node* shadowMeshObj = CreateNode();
	shadowMeshObj->SetScale(10.0f);
	MeshShadowPoint* mesh = new MeshShadowPoint(_ambient, false);
	shadowMeshObj->AddComponent(mesh);

	Node* shadowMeshObj1 = CreateNode();
	shadowMeshObj1->SetPosition(4.0f, -3.5f, 0.0f);
	shadowMeshObj1->SetScale(0.5f);
	MeshShadowPoint* mesh1 = new MeshShadowPoint(_ambient);
	shadowMeshObj1->AddComponent(mesh1);

	Node* shadowMeshObj2 = CreateNode();
	shadowMeshObj2->SetPosition(2.0f, 3.0f, 1.0f);
	shadowMeshObj2->SetScale(0.75f);
	MeshShadowPoint* mesh2 = new MeshShadowPoint(_ambient);
	shadowMeshObj2->AddComponent(mesh2);

	Node* shadowMeshObj3 = CreateNode();
	shadowMeshObj3->SetPosition(-3.0f, -1.0f, 0.0f);
	shadowMeshObj3->SetScale(0.5f);
	MeshShadowPoint* mesh3 = new MeshShadowPoint(_ambient);
	shadowMeshObj3->AddComponent(mesh3);

	Node* shadowMeshObj4 = CreateNode();
	shadowMeshObj4->SetPosition(-1.5f, 1.0f, 1.5f);
	shadowMeshObj4->SetScale(0.5f);
	MeshShadowPoint* mesh4 = new MeshShadowPoint(_ambient);
	shadowMeshObj4->AddComponent(mesh4);

	Node* shadowMeshObj5 = CreateNode();
	shadowMeshObj5->SetPosition(-1.5f, 2.0f, -3.0f);
	shadowMeshObj5->SetRotation(60.0f, Vector3(1.0f, 0.0f, 1.0f));
	shadowMeshObj5->SetScale(0.75f);
	MeshShadowPoint* mesh5 = new MeshShadowPoint(_ambient);
	shadowMeshObj5->AddComponent(mesh5);

}
void Level_1::Start()
{
	Node* cameraObj = CreateNode();
	FreeCamera* camera = new FreeCamera(_ambient, _sceneID);
	cameraObj->SetPosition(0.0f, 0.0f, 3.0f);
	cameraObj->AddComponent(camera);

	ShadowPoint();
}

void Level_1::Update()
{
}
