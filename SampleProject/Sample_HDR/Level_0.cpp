#include "Level_0.h"
#include "GameObject.h"
#include "LightPoint.h"
#include "MeshLight.h"
#include "../FreeCamera.h"

void Level_0::Start()
{
	Node * cameraObj = new Node(_ambient,_id);
	FreeCamera * camera = new FreeCamera(_ambient, _id);
	camera->freeCamera->AllowHDR(true);
	cameraObj->GetComponent(Transform).SetPosition(0.0f, 0.0f, 3.0f);
	cameraObj->AddComponent(camera);


	Node* lightObj = new Node(_ambient, _id);
	Light* light = new LightPoint(_ambient);
	light->SetColor(100.0f, 100.0f, 100.0f);
	lightObj->GetComponent(Transform).SetPosition(0.0f, 0.0f, 49.5f);
	lightObj->AddComponent(light);

	for (int i = 0; i < 8; i++)
	{
		Node* lightObj2 = new Node(_ambient, _id);
		Light* light2 = new LightPoint(_ambient);
		light2->SetColor(1.0f, 0.0f, 0.0f);
		lightObj2->GetComponent(Transform).SetPosition(0.0f, 2.0f, 3.0f + i * 5);
		lightObj2->AddComponent(light2);
	}

	Node* meshObj = new Node(_ambient, _id);
	MeshLight* mesh = new MeshLight(_ambient);
	meshObj->GetComponent(Transform).SetPosition(0.0f, 0.0f, 25.0f);
	meshObj->GetComponent(Transform).SetScale(2.5f, 2.5f, 27.5f);
	meshObj->AddComponent(mesh);

}

void Level_0::Update()
{
}

