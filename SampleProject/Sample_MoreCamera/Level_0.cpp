#include "Level_0.h"
#include "GameObject.h"
#include "LightPoint.h"
#include "Mesh.h"
#include "../FreeCamera.h"

void Level_0::Start()
{
	Camera* camera = new Camera(_ambient);
	camera->SetViewRect(Rectf(0, 0, 0.5f, 0.5f));
	Node* cameraObj = new Node(_ambient,_id);
	cameraObj->GetComponent(Transform).SetPosition(0.0f, 0.0f, 3.0f);
	cameraObj->AddComponent(camera);
	//////////////////////////////////////////////////////////////////////////
	Camera* camera2 = new Camera(_ambient);
	camera2->SetViewRect(Rectf(0.5f, 0.0f, 0.5f, 0.5f));
	Node* cameraObj2 = new Node(_ambient, _id);
	cameraObj2->GetComponent(Transform).SetPosition(0.0f, 0.0f, 3.0f);
	cameraObj2->AddComponent(camera2);

	//////////////////////////////////////////////////////////////////////////
	Node* freeCameraObj = new Node(_ambient, _id);
	FreeCamera* freeCamera = new FreeCamera(_ambient, _id);
	freeCameraObj->AddComponent(freeCamera);

	//////////////////////////////////////////////////////////////////////////
	Node* lightObj = new Node(_ambient, _id);
	lightObj->GetComponent(Transform).SetPosition(2.0f, 0.0f, 0.0f);
	Light* light = new LightPoint(_ambient);
	lightObj->AddComponent(light);
	//////////////////////////////////////////////////////////////////////////
	Mesh* mesh = new Mesh(_ambient, "../Resource/object/base/Cube.3DS");
	Node* meshObj = new Node(_ambient, _id);
	meshObj->GetComponent(Transform).SetPosition(0.0f, 0.0f, -3.0f);
	meshObj->AddComponent(mesh);
}
void Level_0::Update()
{

}
