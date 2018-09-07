#include "Level_0.h"
#include "GameObject.h"
#include "LightPoint.h"
#include "Mesh.h"
#include "../FreeCamera.h"

Level_0::Level_0(Ambient* ambient, int levelNumber)
	:LevelScene(ambient, levelNumber)
{}


void Level_0::Start()
{
	Camera* camera = new Camera(_ambient);
	camera->SetViewRect(Rectf(0, 0, 0.5f, 0.5f));
	GameObject* cameraObj = new GameObject(_ambient,_levelNumber);
	cameraObj->GetComponent(Transform).SetPosition(0.0f, 0.0f, 3.0f);
	cameraObj->AddComponent(camera);
	//////////////////////////////////////////////////////////////////////////
	Camera* camera2 = new Camera(_ambient);
	camera2->SetViewRect(Rectf(0.5f, 0.0f, 0.5f, 0.5f));
	GameObject* cameraObj2 = new GameObject(_ambient, _levelNumber);
	cameraObj2->GetComponent(Transform).SetPosition(0.0f, 0.0f, 3.0f);
	cameraObj2->AddComponent(camera2);

	//////////////////////////////////////////////////////////////////////////
	GameObject* freeCameraObj = new GameObject(_ambient, _levelNumber);
	FreeCamera* freeCamera = new FreeCamera(_ambient, _levelNumber);
	freeCameraObj->AddComponent(freeCamera);

	//////////////////////////////////////////////////////////////////////////
	GameObject* lightObj = new GameObject(_ambient, _levelNumber);
	lightObj->GetComponent(Transform).SetPosition(2.0f, 0.0f, 0.0f);
	Light* light = new LightPoint(_ambient);
	lightObj->AddComponent(light);
	//////////////////////////////////////////////////////////////////////////
	Mesh* mesh = new Mesh(_ambient, "../Resource/object/base/Cube.3DS");
	GameObject* meshObj = new GameObject(_ambient, _levelNumber);
	meshObj->GetComponent(Transform).SetPosition(0.0f, 0.0f, -3.0f);
	meshObj->AddComponent(mesh);
}
void Level_0::Update()
{

}
