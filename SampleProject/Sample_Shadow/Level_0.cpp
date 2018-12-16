#include "Level_0.h"
#include "GameObject.h"
#include "LightDirectional.h"
#include "Mesh.h"
#include "MeshShadow.h"

void Level_0::ShadowNormal()
{
	GameObject* lightObj = new GameObject(_ambient, _levelNumber);
	lightObj->GetComponent(Transform).SetPosition(-2.0f, 4.0f, -5.0f);
	Light* light = new LightDirectional(_ambient);
	lightObj->AddComponent(light);
	Mesh* lightMesh = new Mesh(_ambient, "../Resource/object/base/Sphere.3DS");
	lightMesh->GetMaterial()->color = Color(1.0f, 1.0f, 1.0f);
	lightObj->AddComponent(lightMesh);


	GameObject* shadowMeshObj = new GameObject(_ambient, _levelNumber);
	shadowMeshObj->GetComponent(Transform).SetPosition(0.0f, -1.0f, 0.0f);
	shadowMeshObj->GetComponent(Transform).SetScale(10.0f, 0.5f, 10.0f);
	MeshShadow* mesh = new MeshShadow(_ambient);
	shadowMeshObj->AddComponent(mesh);


	GameObject* shadowMeshObj1 = new GameObject(_ambient, _levelNumber);
	shadowMeshObj1->GetComponent(Transform).SetPosition(2.0f, 0.0f, 1.0f);
	shadowMeshObj1->GetComponent(Transform).SetScale(0.5f);
	MeshShadow* mesh1 = new MeshShadow(_ambient);
	shadowMeshObj1->AddComponent(mesh1);

	GameObject* shadowMeshObj2 = new GameObject(_ambient, _levelNumber);
	shadowMeshObj2->GetComponent(Transform).SetPosition(-1.0f, 0.0f, 2.0f);
	shadowMeshObj2->GetComponent(Transform).SetRotation(60.0f, Vector3(1.0f, 0.0f, 1.0f));
	shadowMeshObj2->GetComponent(Transform).SetScale(0.25f);
	MeshShadow* mesh2 = new MeshShadow(_ambient);
	shadowMeshObj2->AddComponent(mesh2);

	GameObject* shadowMeshObj3 = new GameObject(_ambient, _levelNumber);
	shadowMeshObj3->GetComponent(Transform).SetPosition(0.0f, 1.5f, 0.0);
	shadowMeshObj3->GetComponent(Transform).SetScale(0.5f);
	MeshShadow* mesh3 = new MeshShadow(_ambient);
	shadowMeshObj3->AddComponent(mesh3);

}

void Level_0::Start()
{
	GameObject* cameraObj = new GameObject(_ambient, _levelNumber);
	FreeCamera* camera = new FreeCamera(_ambient, _levelNumber);
	cameraObj->GetComponent(Transform).SetPosition(0.0f, 0.0f, 3.0f);
	cameraObj->AddComponent(camera);

	ShadowNormal();

}

void Level_0::Update()
{

}
