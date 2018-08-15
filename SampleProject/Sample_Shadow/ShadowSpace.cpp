#include "ShadowSpace.h"
#include "Application.h"
#include "LightDirectional.h"
#include "LightPoint.h"
#include "../FreeCamera.h"
#include "Mesh.h"
#include "MeshShadow.h"
#include "MeshShadowPoint.h"
#include "Math/Vector3.h"



ShadowSpace::ShadowSpace(Ambient* ambient)
	:MotionSpace(ambient)
{}
ShadowSpace::~ShadowSpace()
{}
void ShadowSpace::ShadowNormal()
{
	GameObject * lightObj = new GameObject(_ambient);
	lightObj->GetComponent(Transform).SetPosition(-2.0f, 4.0f, -5.0f);
	Light* light = new LightDirectional(_ambient);
	lightObj->AddComponent(light);
	Mesh* lightMesh = new Mesh(_ambient,"../Resource/object/base/Sphere.3DS");
	lightMesh->GetMaterial().color = Color(1.0f, 1.0f, 1.0f);
	lightObj->AddComponent(lightMesh);


	GameObject* shadowMeshObj = new GameObject(_ambient);
	shadowMeshObj->GetComponent(Transform).SetPosition(0.0f, -1.0f, 0.0f);
	shadowMeshObj->GetComponent(Transform).SetScale(10.0f, 0.5f, 10.0f);
	MeshShadow* mesh = new MeshShadow(_ambient);
	shadowMeshObj->AddComponent(mesh);


	GameObject* shadowMeshObj1 = new GameObject(_ambient);
	shadowMeshObj1->GetComponent(Transform).SetPosition(2.0f, 0.0f, 1.0f);
	shadowMeshObj1->GetComponent(Transform).SetScale(0.5f);
	MeshShadow* mesh1 = new MeshShadow(_ambient);
	shadowMeshObj1->AddComponent(mesh1);

	GameObject* shadowMeshObj2 = new GameObject(_ambient);
	shadowMeshObj2->GetComponent(Transform).SetPosition(-1.0f, 0.0f, 2.0f);
	shadowMeshObj2->GetComponent(Transform).SetRotation(60.0f, Vector3(1.0f, 0.0f, 1.0f));
	shadowMeshObj2->GetComponent(Transform).SetScale(0.25f);
	MeshShadow* mesh2 = new MeshShadow(_ambient);
	shadowMeshObj2->AddComponent(mesh2);

	GameObject* shadowMeshObj3 = new GameObject(_ambient);
	shadowMeshObj3->GetComponent(Transform).SetPosition(0.0f, 1.5f, 0.0);
	shadowMeshObj3->GetComponent(Transform).SetScale(0.5f);
	MeshShadow* mesh3 = new MeshShadow(_ambient);
	shadowMeshObj3->AddComponent(mesh3);

}
void ShadowSpace::ShadowPoint()
{
	GameObject * lightObj = new GameObject(_ambient);
	lightObj->GetComponent(Transform).SetPosition(0.0f, 0.0f, -0.0f);
	Light* light = new LightPoint(_ambient);
	lightObj->AddComponent(light);
	Mesh* lightMesh = new Mesh(_ambient, "../Resource/object/base/Sphere.3DS");
	lightMesh->GetMaterial().color = Color(1.0f, 1.0f, 1.0f);
	lightObj->AddComponent(lightMesh);

	GameObject* shadowMeshObj = new GameObject(_ambient);
	shadowMeshObj->GetComponent(Transform).SetScale(10.0f);
	MeshShadowPoint* mesh = new MeshShadowPoint(_ambient, false);
	shadowMeshObj->AddComponent(mesh);

	GameObject* shadowMeshObj1 = new GameObject(_ambient);
	shadowMeshObj1->GetComponent(Transform).SetPosition(4.0f, -3.5f, 0.0f);
	shadowMeshObj1->GetComponent(Transform).SetScale(0.5f);
	MeshShadowPoint* mesh1 = new MeshShadowPoint(_ambient);
	shadowMeshObj1->AddComponent(mesh1);

	GameObject* shadowMeshObj2 = new GameObject(_ambient);
	shadowMeshObj2->GetComponent(Transform).SetPosition(2.0f, 3.0f, 1.0f);
	shadowMeshObj2->GetComponent(Transform).SetScale(0.75f);
	MeshShadowPoint* mesh2 = new MeshShadowPoint(_ambient);
	shadowMeshObj2->AddComponent(mesh2);

	GameObject* shadowMeshObj3 = new GameObject(_ambient);
	shadowMeshObj3->GetComponent(Transform).SetPosition(-3.0f, -1.0f, 0.0f);
	shadowMeshObj3->GetComponent(Transform).SetScale(0.5f);
	MeshShadowPoint* mesh3 = new MeshShadowPoint(_ambient);
	shadowMeshObj3->AddComponent(mesh3);

	GameObject* shadowMeshObj4 = new GameObject(_ambient);
	shadowMeshObj4->GetComponent(Transform).SetPosition(-1.5f, 1.0f, 1.5f);
	shadowMeshObj4->GetComponent(Transform).SetScale(0.5f);
	MeshShadowPoint* mesh4 = new MeshShadowPoint(_ambient);
	shadowMeshObj4->AddComponent(mesh4);

	GameObject* shadowMeshObj5 = new GameObject(_ambient);
	shadowMeshObj5->GetComponent(Transform).SetPosition(-1.5f, 2.0f, -3.0f);
	shadowMeshObj5->GetComponent(Transform).SetRotation(60.0f, Vector3(1.0f, 0.0f, 1.0f));
	shadowMeshObj5->GetComponent(Transform).SetScale(0.75f);
	MeshShadowPoint* mesh5 = new MeshShadowPoint(_ambient);
	shadowMeshObj5->AddComponent(mesh5);

}
void ShadowSpace::Start()
{
	GameObject * cameraObj = new GameObject(_ambient);
	FreeCamera * camera = new FreeCamera(_ambient);
	cameraObj->GetComponent(Transform).SetPosition(0.0f, 0.0f, 3.0f);
	cameraObj->AddComponent(camera);

	ShadowNormal();
	//ShadowPoint();

}

void ShadowSpace::Update()
{
}

int ShadowSpace::Launch()
{
	return INSTANCE(Application).Run(_ambient);
}
AUTO_APPLICATION_MAIN(ShadowSpace)