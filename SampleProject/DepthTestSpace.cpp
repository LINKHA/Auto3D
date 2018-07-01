#include "DepthTestSpace.h"
#include "Application.h"
#include "Mesh.h"
#include "FreeCamera.h"
#include "BaseLight.h"
#include "DepthSet.h"
DepthTestSpace::DepthTestSpace()
{}
DepthTestSpace::~DepthTestSpace()
{}
void DepthTestSpace::Awake()
{
	//INSTANCE(DepthSet).SetMode(DEPTH_ALWAYS);
}
void DepthTestSpace::Start()
{
	GameObject * lightObj = new GameObject();
	lightObj->GetComponent(Transform).SetPosition(2.0f, 5.0f, 0.0f);
	Light * light = new Light(Directional);
	lightObj->AddComponent(light);
	/////////////////////////////////////////////////////////////////////////////////////////////
	GameObject * camObj = new GameObject();
	FreeCamera * cam = new FreeCamera();
	camObj->AddComponent(cam);
	/////////////////////////////////////////////////////////////////////////////////////////////
	GameObject * obj1 = new GameObject();
	Mesh * plane = new Mesh("Resource/object/base/Cube.FBX");
	plane->GetMaterial().color.Set(0.5f, 0.5f, 0.5f);
	obj1->AddComponent(plane);
	obj1->GetComponent(Transform).SetScale(10.0f, 0.1f, 10.0f);
	/////////////////////////////////////////////////////////////////////////////////////////////
	GameObject * obj2 = new GameObject();
	Mesh * box = new Mesh("Resource/object/base/Cube.FBX");
	obj2->AddComponent(box);
	obj2->GetComponent(Transform).SetPosition(1.0f, 0.5f, 3.0f);
	/////////////////////////////////////////////////////////////////////////////////////////////
	GameObject * obj3 = new GameObject();
	Mesh * box2 = new Mesh("Resource/object/base/Cube.FBX");
	obj3->AddComponent(box2);
	obj3->GetComponent(Transform).SetPosition(3.0f, 0.5f, 2.0f);
}

void DepthTestSpace::Update()
{
}

int DepthTestSpace::Launch()
{
	return INSTANCE(Application).Run();
}
