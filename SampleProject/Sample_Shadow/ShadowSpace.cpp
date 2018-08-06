#include "ShadowSpace.h"
#include "Application.h"
#include "LightDirectional.h"
#include "../FreeCamera.h"
#include "Mesh.h"
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

	GameObject* shadowMeshObj = new GameObject();
	MeshShadow* mesh = new MeshShadow(_ambient, 0);
	shadowMeshObj->AddComponent(mesh);

	GameObject* shadowMeshObj1 = new GameObject();
	MeshShadow* mesh1 = new MeshShadow(_ambient, 1);
	shadowMeshObj1->AddComponent(mesh1);

	GameObject* shadowMeshObj2 = new GameObject();
	MeshShadow* mesh2 = new MeshShadow(_ambient, 2);
	shadowMeshObj2->AddComponent(mesh2);

	GameObject* shadowMeshObj3 = new GameObject();
	MeshShadow* mesh3 = new MeshShadow(_ambient, 3);
	shadowMeshObj3->AddComponent(mesh3);
}
void ShadowSpace::ShadowPoint()
{

}
void ShadowSpace::Start()
{
	GameObject * cameraObj = new GameObject(_ambient);
	FreeCamera * camera = new FreeCamera(_ambient);
	cameraObj->GetComponent(Transform).SetPosition(0.0f, 0.0f, 3.0f);
	cameraObj->AddComponent(camera);

	GameObject * lightObj = new GameObject(_ambient);
	lightObj->GetComponent(Transform).SetPosition(-2.0f, 4.0f, -1.0f);
	Light* light = new LightDirectional(_ambient);
	lightObj->AddComponent(light);

	ShadowNormal();

}

void ShadowSpace::Update()
{
}

int ShadowSpace::Launch()
{
	return INSTANCE(Application).Run(_ambient);
}
AUTO_APPLICATION_MAIN(ShadowSpace)