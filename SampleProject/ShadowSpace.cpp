#include "ShadowSpace.h"
#include "Application.h"
#include "Light.h"
#include "FreeCamera.h"
#include "Mesh.h"
#include "Shadow.h"
#include "MeshShadowTest.h"
#include "ShadowTest.h"
ShadowSpace::ShadowSpace()
{}
ShadowSpace::~ShadowSpace()
{}

void ShadowSpace::Start()
{
	GameObject * cameraObj = new GameObject();
	FreeCamera * camera = new FreeCamera();
	cameraObj->GetComponent(Transform).SetPosition(0.0f, 0.0f, 3.0f);
	cameraObj->AddComponent(camera);

	/*GameObject * lightObj = new GameObject();
	Light * light = new Light(Directional);
	lightObj->AddComponent(light);*/

	/*GameObject * meshObj = new GameObject();
	MeshShadowTest * mesh = new MeshShadowTest("Resource/object/base/Cube.FBX");
	mesh->GetMaterial().SetImage("Resource/texture/wood.jpg");
	meshObj->AddComponent(mesh);
	meshObj->GetComponent(Transform).SetPosition(0.0f, 2.0f, 0.0f);
	
	GameObject * meshObj2 = new GameObject();
	MeshShadowTest * mesh2 = new MeshShadowTest("Resource/object/base/Cube.FBX");
	mesh2->GetMaterial().SetImage("Resource/texture/wood.jpg");
	meshObj2->AddComponent(mesh2);
	meshObj2->GetComponent(Transform).SetScale(10.0f, 0.5f, 10.0f);

	GameObject * meshObj3 = new GameObject();
	MeshShadowTest * mesh3 = new MeshShadowTest("Resource/object/base/Cube.FBX");
	mesh3->GetMaterial().SetImage("Resource/texture/wood.jpg");
	meshObj3->AddComponent(mesh3);
	meshObj3->GetComponent(Transform).SetPosition(4.0f, 2.0f, 0.0f);*/

	GameObject * shadowObj = new GameObject();
	Shadow * shadow = new Shadow();
	shadowObj->AddComponent(shadow);
}

void ShadowSpace::Update()
{
}

int ShadowSpace::Launch()
{
	return INSTANCE(Application).Run();
}
