#include "ShadowSpace.h"
#include "Application.h"
#include "LightDirectional.h"
#include "LightPoint.h"
#include "../FreeCamera.h"
#include "Mesh.h"
#include "MeshShadowPoint.h"

#include "Shadow.h"
#include "ShadowPoint.h"
#include "ShadowTest.h"

ShadowSpace::ShadowSpace(Ambient* ambient)
	:MotionSpace(ambient)
{}
ShadowSpace::~ShadowSpace()
{}
void ShadowSpace::ShadowNormal()
{
	GameObject * lightObj = new GameObject(_ambient);
	lightObj->GetComponent(Transform).SetPosition(-2.0f, 4.0f, -1.0f);
	Light* light = new LightDirectional(_ambient);
	lightObj->AddComponent(light);

	GameObject* shadowMeshObj = new GameObject(_ambient);
	MeshShadow* mesh = new MeshShadow(_ambient, 0);
	shadowMeshObj->AddComponent(mesh);

	GameObject* shadowMeshObj1 = new GameObject(_ambient);
	MeshShadow* mesh1 = new MeshShadow(_ambient, 1);
	shadowMeshObj1->AddComponent(mesh1);

	GameObject* shadowMeshObj2 = new GameObject(_ambient);
	MeshShadow* mesh2 = new MeshShadow(_ambient, 2);
	shadowMeshObj2->AddComponent(mesh2);

	GameObject* shadowMeshObj3 = new GameObject(_ambient);
	MeshShadow* mesh3 = new MeshShadow(_ambient, 3);
	shadowMeshObj3->AddComponent(mesh3);
}
void ShadowSpace::ShadowPoint()
{
	GameObject * lightObj = new GameObject(_ambient);
	lightObj->GetComponent(Transform).SetPosition(0.0f, 0.0f, -0.0f);
	Light* light = new LightPoint(_ambient);
	lightObj->AddComponent(light);

	GameObject* shadowMeshObj = new GameObject(_ambient);
	MeshShadowPoint* mesh = new MeshShadowPoint(_ambient, 0);
	shadowMeshObj->AddComponent(mesh);

	GameObject* shadowMeshObj1 = new GameObject(_ambient);
	MeshShadowPoint* mesh1 = new MeshShadowPoint(_ambient, 1);
	shadowMeshObj1->AddComponent(mesh1);

	GameObject* shadowMeshObj2 = new GameObject(_ambient);
	MeshShadowPoint* mesh2 = new MeshShadowPoint(_ambient, 2);
	shadowMeshObj2->AddComponent(mesh2);

	GameObject* shadowMeshObj3 = new GameObject(_ambient);
	MeshShadowPoint* mesh3 = new MeshShadowPoint(_ambient, 3);
	shadowMeshObj3->AddComponent(mesh3);

	GameObject* shadowMeshObj4 = new GameObject(_ambient);
	MeshShadowPoint* mesh4 = new MeshShadowPoint(_ambient, 4);
	shadowMeshObj4->AddComponent(mesh4);

	GameObject* shadowMeshObj5 = new GameObject(_ambient);
	MeshShadowPoint* mesh5 = new MeshShadowPoint(_ambient, 5);
	shadowMeshObj5->AddComponent(mesh5);

}
void ShadowSpace::Start()
{
	GameObject * cameraObj = new GameObject(_ambient);
	FreeCamera * camera = new FreeCamera(_ambient);
	cameraObj->GetComponent(Transform).SetPosition(0.0f, 0.0f, 3.0f);
	cameraObj->AddComponent(camera);

	//ShadowNormal();
	ShadowPoint();

}

void ShadowSpace::Update()
{
}

int ShadowSpace::Launch()
{
	return INSTANCE(Application).Run(_ambient);
}
AUTO_APPLICATION_MAIN(ShadowSpace)