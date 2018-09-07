#include "Level_0.h"
#include "GameObject.h"
#include "LightDirectional.h"
#include "Mesh.h"
#include "../FreeCamera.h"

Level_0::Level_0(Ambient* ambient, int levelNumber)
	:LevelScene(ambient, levelNumber)
{}

void Level_0::Start()
{
	GameObject* lightObj = new GameObject(_ambient,_levelNumber);
	lightObj->GetComponent(Transform).SetPosition(2.0f, 5.0f, 0.0f);
	Light* light = new LightDirectional(_ambient);
	lightObj->AddComponent(light);
	/////////////////////////////////////////////////////////////////////////////////////////////
	GameObject* camObj = new GameObject(_ambient, _levelNumber);
	FreeCamera* cam = new FreeCamera(_ambient, _levelNumber);
	camObj->AddComponent(cam);
	/////////////////////////////////////////////////////////////////////////////////////////////
	GameObject* obj1 = new GameObject(_ambient, _levelNumber);
	Mesh* plane = new Mesh(_ambient);
	plane->GetMaterial()->color.Set(0.5f, 0.5f, 0.5f);
	obj1->AddComponent(plane);
	obj1->GetComponent(Transform).SetScale(10.0f, 0.1f, 10.0f);
	/////////////////////////////////////////////////////////////////////////////////////////////
	GameObject * obj2 = new GameObject(_ambient, _levelNumber);
	Mesh* box = new Mesh(_ambient);
	box->EnableDepth(false);
	obj2->AddComponent(box);
	obj2->GetComponent(Transform).SetPosition(1.0f, 0.5f, 3.0f);
	/////////////////////////////////////////////////////////////////////////////////////////////
	GameObject * obj3 = new GameObject(_ambient, _levelNumber);
	Mesh* box2 = new Mesh(_ambient);
	box2->EnableDepth(false);
	obj3->AddComponent(box2);
	obj3->GetComponent(Transform).SetPosition(3.0f, 0.5f, 2.0f);
}

void Level_0::Update()
{
}


