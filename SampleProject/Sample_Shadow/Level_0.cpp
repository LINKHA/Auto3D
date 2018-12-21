#include "Level_0.h"
#include "Node.h"
#include "Light.h"
#include "MeshRenderer.h"
#include "MeshShadow.h"
#include "ResourceSystem.h"


void Level_0::ShadowNormal()
{
	auto* sphere = GetSubSystem<ResourceSystem>()->GetResource<Mesh>("object/base/Sphere.3DS");

	Node* lightObj = CreateNode();
	lightObj->SetPosition(-2.0f, 4.0f, -5.0f);
	Light* light = lightObj->CreateComponent<Light>();
	light->SetType(LightType::Directional);
	MeshRenderer* lightMesh = lightObj->CreateComponent<MeshRenderer>();
	lightMesh->GetMaterial()->color = Color(1.0f, 1.0f, 1.0f);


	Node* shadowMeshObj = CreateNode();
	shadowMeshObj->SetPosition(0.0f, -1.0f, 0.0f);
	shadowMeshObj->SetScale(10.0f, 0.5f, 10.0f);
	MeshShadow* mesh = new MeshShadow(_ambient);
	shadowMeshObj->AddComponent(mesh);


	Node* shadowMeshObj1 = CreateNode();
	shadowMeshObj1->SetPosition(2.0f, 0.0f, 1.0f);
	shadowMeshObj1->SetScale(0.5f);
	MeshShadow* mesh1 = new MeshShadow(_ambient);
	shadowMeshObj1->AddComponent(mesh1);

	Node* shadowMeshObj2 = CreateNode();
	shadowMeshObj2->SetPosition(-1.0f, 0.0f, 2.0f);
	shadowMeshObj2->SetRotation(60.0f, Vector3(1.0f, 0.0f, 1.0f));
	shadowMeshObj2->SetScale(0.25f);
	MeshShadow* mesh2 = new MeshShadow(_ambient);
	shadowMeshObj2->AddComponent(mesh2);

	Node* shadowMeshObj3 = CreateNode();
	shadowMeshObj3->SetPosition(0.0f, 1.5f, 0.0);
	shadowMeshObj3->SetScale(0.5f);
	MeshShadow* mesh3 = new MeshShadow(_ambient);
	shadowMeshObj3->AddComponent(mesh3);

}

void Level_0::Start()
{
	Node* cameraObj = CreateNode();
	FreeCamera* camera = new FreeCamera(_ambient, _sceneID);
	cameraObj->SetPosition(0.0f, 0.0f, 3.0f);
	cameraObj->AddComponent(camera);

	ShadowNormal();

}

void Level_0::Update()
{

}
