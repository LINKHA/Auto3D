#include "StencilTestSpace.h"
#include "Application.h"
#include "Mesh.h"
#include "FreeCamera.h"
#include "LightDirectional.h"
StencilTestSpace::StencilTestSpace()
{
}
StencilTestSpace::~StencilTestSpace()
{
}

void StencilTestSpace::Start()
{
	GameObject * lightObj = new GameObject();
	lightObj->GetComponent(Transform).SetPosition(2.0f, 5.0f, 0.0f);
	Light * light = new LightDirectional();
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
	Mesh * box1 = new Mesh("Resource/object/base/Cube.FBX");
	box1->EnableStencil(true);
	box1->StencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	box1->StencilFunc(GL_ALWAYS, 1, 0xFF);
	box1->StencilMask(0xFF);
	box1->GetMaterial().color.Set(1.0f, 0.0f, 0.5f);
	obj2->AddComponent(box1);
	obj2->GetComponent(Transform).SetPosition(1.0f, 0.5f, 3.0f);
	/////////////////////////////////////////////////////////////////////////////////////////////
	GameObject * obj3 = new GameObject();
	Mesh * box2 = new Mesh("Resource/object/base/Cube.FBX");
	box2->EnableStencil(true);
	box2->StencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	box2->StencilFunc(GL_NOTEQUAL, 1, 0xFF);
	box2->StencilMask(0x00);
	obj3->AddComponent(box2);
	obj3->GetComponent(Transform).SetScale(1.1f, 1.1f, 1.1f);
	obj3->GetComponent(Transform).SetPosition(1.0f, 0.5f, 3.0f);
}
void StencilTestSpace::Update()
{

}
int StencilTestSpace::Launch()
{
	return INSTANCE(Application).Run();
}
