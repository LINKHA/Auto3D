#include "Level_0.h"
#include "GameObject.h"
#include "LightDirectional.h"
#include "LightPoint.h"
#include "LightSpot.h"
#include "Mesh.h"
#include "../FreeCamera.h"


void Level_0::Start()
{
	GameObject* camObj = new GameObject(_ambient,_levelNumber);
	FreeCamera* freeCamera = new FreeCamera(_ambient, _levelNumber);
	camObj->GetComponent(Transform).SetPosition(0.0f, 0.0f, 3.0f);
	camObj->AddComponent(freeCamera);
	//////////////////////////////////////////////////////////////////////////
	GameObject* lightObj = new GameObject(_ambient, _levelNumber);
	lightObj->GetComponent(Transform).SetPosition(0.0f, 0.0f, -3.0f);
	Light* light = new LightPoint(_ambient);
	lightObj->AddComponent(light);
	//////////////////////////////////////////////////////////////////////////
	GameObject* lightObj2 = new GameObject(_ambient, _levelNumber);
	lightObj2->GetComponent(Transform).SetPosition(0.0f, 0.0f, 3.0f);
	Light* light2 = new LightSpot(_ambient);
	light2->direction.Set(0.0f, 0.0f, -1.0f);
	lightObj2->AddComponent(light2);
	//////////////////////////////////////////////////////////////////////////
	GameObject* lightObj3 = new GameObject(_ambient, _levelNumber);
	Light* light3 = new LightDirectional(_ambient);
	light3->ambient.Set(0.1f, 0.1f, 0.1f);
	light3->direction.Set(0.0f, -1.0f, 0.0f);
	lightObj3->AddComponent(light3);
	//////////////////////////////////////////////////////////////////////////
	GameObject* meshObj = new GameObject(_ambient, _levelNumber);
	Mesh* mesh = new Mesh(_ambient);
	mesh->GetMaterial()->SetImage("../Resource/texture/wood.jpg");
	mesh->GetMaterial()->color.Set(0.5f, 0.8f, 0.3f);
	meshObj->AddComponent(mesh);
	//////////////////////////////////////////////////////////////////////////
	GameObject* meshObj2 = new GameObject(_ambient, _levelNumber);
	Mesh* mesh2 = new Mesh(_ambient);
	mesh2->GetMaterial()->SetImage("../Resource/texture/wood.jpg");
	mesh2->GetMaterial()->color.Set(0.5f, 0.8f, 0.3f);
	meshObj2->GetComponent(Transform).SetPosition(1.0f, 0.0f, 0.0f);
	meshObj2->AddComponent(mesh2);
	//////////////////////////////////////////////////////////////////////////
	GameObject* meshObj3 = new GameObject(_ambient, _levelNumber);
	Mesh* mesh3 = new Mesh(_ambient);
	mesh3->GetMaterial()->SetImage("../Resource/texture/wood.jpg");
	mesh3->GetMaterial()->color.Set(0.5f, 0.8f, 0.3f);
	meshObj3->GetComponent(Transform).SetPosition(-1.0f, 0.0f, 0.0f);
	meshObj3->AddComponent(mesh3);
}

void Level_0::Update()
{

}

