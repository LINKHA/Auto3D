#include "MultLightsSpace.h"
#include "BaseSpace.h"
#include "TimeManager.h"
#include "RenderManager.h"
#include "GameObjectManager.h"
#include "Camera.h"
#include "BaseLight.h"
#include "FreeCamera.h"


Mesh* mesh;
GameObject* meshObj;

Light* light;
GameObject* lightObj;

Light* light2;
GameObject* lightObj2;

Light* light3;
GameObject* lightObj3;

GameObject* camObj;

MultLightsSpace::MultLightsSpace()
{}
MultLightsSpace::~MultLightsSpace()
{}

void MultLightsSpace::Start()
{
	camObj = new GameObject();
	FreeCamera * freeCamera = new FreeCamera();
	camObj->GetComponent(Transform).SetPosition(0.0f, 0.0f, 3.0f);
	camObj->AddComponent(freeCamera);
	//////////////////////////////////////////////////////////////////////////
	lightObj = new GameObject();
	lightObj->GetComponent(Transform).SetPosition(2.0f, 0.0f, 0.0f);
	light = new Light(Point);
	lightObj->AddComponent(light);
	//////////////////////////////////////////////////////////////////////////
	lightObj2 = new GameObject();
	lightObj2->GetComponent(Transform).SetPosition(-2.0f, 0.0f, -0.5f);
	light2 = new Light(Spot);
	light2->direction.Set(1.0f, 0.0f, 0.0f);
	lightObj2->AddComponent(light2);
	//////////////////////////////////////////////////////////////////////////
	lightObj3 = new GameObject();
	light3 = new Light(Directional);
	light->ambient.Set(0.1f, 0.1f, 0.1f);
	light3->direction.Set(0.0f, -1.0f, 0.0f);
	lightObj3->AddComponent(light3);
	//////////////////////////////////////////////////////////////////////////
	mesh = new Mesh("Resource/object/base/Cube.FBX");
	//mesh = new Mesh();
	mesh->GetMaterial().color.Set(0.5f, 0.8f, 0.3f);

	meshObj = new GameObject();
	meshObj->AddComponent(mesh);
	//////////////////////////////////////////////////////////////////////////
}

void MultLightsSpace::Update()
{
	meshObj->GetComponent(Transform).SetPosition(0.0f, 0.0f, -1.0f);
	//meshObj->GetComponent(Transform).UpdateTransform();
}

int MultLightsSpace::Launch()
{
	return INSTANCE(Application).Run();
}
