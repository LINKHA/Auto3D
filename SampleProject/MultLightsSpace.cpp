#include "MultLightsSpace.h"
#include "BaseSpace.h"
#include "TimeManager.h"
#include "RenderManager.h"
#include "GameObjectManager.h"
#include "Camera.h"
#include "Light.h"
#include "FreeCamera.h"
#include "LightDirectional.h"
#include "LightSpot.h"
#include "LightPoint.h"
#include "SampleSet.h"


Light* mu_light;
GameObject* mu_lightObj;

Light* mu_light2;
GameObject* mu_lightObj2;

Light* mu_light3;
GameObject* mu_lightObj3;

GameObject* mu_camObj;

MultLightsSpace::MultLightsSpace()
{}
MultLightsSpace::~MultLightsSpace()
{}

void MultLightsSpace::Start()
{
	mu_camObj = new GameObject();
	FreeCamera * freeCamera = new FreeCamera();
	mu_camObj->GetComponent(Transform).SetPosition(0.0f, 0.0f, 3.0f);
	mu_camObj->AddComponent(freeCamera);
	//////////////////////////////////////////////////////////////////////////
	mu_lightObj = new GameObject();
	mu_lightObj->GetComponent(Transform).SetPosition(2.0f, 0.0f, 0.0f);
	mu_light = new LightPoint();
	mu_lightObj->AddComponent(mu_light);
	//////////////////////////////////////////////////////////////////////////
	mu_lightObj2 = new GameObject();
	mu_lightObj2->GetComponent(Transform).SetPosition(-2.0f, 0.0f, -0.5f);
	mu_light2 = new LightSpot();
	mu_light2->direction.Set(1.0f, 0.0f, 0.0f);
	mu_lightObj2->AddComponent(mu_light2);
	//////////////////////////////////////////////////////////////////////////
	mu_lightObj3 = new GameObject();
	mu_light3 = new LightDirectional();
	mu_light->ambient.Set(0.1f, 0.1f, 0.1f);
	mu_light3->direction.Set(0.0f, -1.0f, 0.0f);
	mu_lightObj3->AddComponent(mu_light3);
	//////////////////////////////////////////////////////////////////////////
	Mesh * mesh = new Mesh("Resource/object/base/Cube.FBX");
	mesh->GetMaterial().SetImage("Resource/texture/wood.jpg");
	mesh->GetMaterial().color.Set(0.5f, 0.8f, 0.3f);
	GameObject * meshObj = new GameObject();
	meshObj->GetComponent(Transform).SetPosition(0.0f, 0.0f, -1.0f);
	meshObj->AddComponent(mesh);
	//////////////////////////////////////////////////////////////////////////
	Mesh * mesh2 = new Mesh("Resource/object/base/Cone.FBX");
	mesh2->GetMaterial().color.Set(0.5f, 0.8f, 0.3f);
	GameObject * meshObj2 = new GameObject();
	meshObj2->GetComponent(Transform).SetPosition(0.0f, 0.0f, 1.0f);
	meshObj2->AddComponent(mesh2);
	//////////////////////////////////////////////////////////////////////////
	Mesh * mesh3 = new Mesh("Resource/object/base/Cone.FBX");
	mesh3->GetMaterial().color.Set(0.5f, 0.8f, 0.3f);
	GameObject * meshObj3 = new GameObject();
	meshObj3->GetComponent(Transform).SetPosition(0.0f, 0.0f, 2.0f);
	meshObj3->AddComponent(mesh3);
}

void MultLightsSpace::Update()
{
	
}

int MultLightsSpace::Launch()
{
	return INSTANCE(Application).Run();
}
#if uMultLightsSpace
AUTO_APPLICATION_MAIN(MultLightsSpace)
#endif