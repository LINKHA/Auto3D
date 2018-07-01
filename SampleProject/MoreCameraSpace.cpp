#include "MoreCameraSpace.h"
#include "Texture2D.h"
#include "GameObject.h"
#include "FreeCamera.h"
#include "TimeManager.h"
#include "Mesh.h"
#include "FreeCamera.h"
#include "BaseLight.h"
MoreCameraSpace::MoreCameraSpace()
{
}
MoreCameraSpace::~MoreCameraSpace()
{
}

void MoreCameraSpace::Start()
{
	Camera * camera = new Camera(Vector3(0.3f, 0.0f, 3.0f));
	camera->ViewRect = Rectf(0, 0, 0.5f, 0.5f);
	GameObject * cameraObj = new GameObject();
	cameraObj->GetComponent(Transform).SetPosition(0.0f, 0.0f, 3.0f);
	cameraObj->AddComponent(camera);
	//////////////////////////////////////////////////////////////////////////
	Camera * camera2 = new Camera(Vector3(0.0f, -1.0f, 3.0f));
	camera2->ViewRect = Rectf(0.5f, 0.0f, 0.5f, 0.5f);
	GameObject * cameraObj2 = new GameObject();
	cameraObj2->GetComponent(Transform).SetPosition(0.0f, 0.0f, 3.0f);
	cameraObj2->AddComponent(camera2);
	//////////////////////////////////////////////////////////////////////////
	GameObject * freeCameraObj = new GameObject();
	FreeCamera * freeCamera = new FreeCamera();
	freeCameraObj->AddComponent(freeCamera);

	//////////////////////////////////////////////////////////////////////////
	GameObject * lightObj = new GameObject();
	lightObj->GetComponent(Transform).SetPosition(2.0f, 0.0f, 0.0f);
	Light * light = new Light(Point);
	lightObj->AddComponent(light);
	//////////////////////////////////////////////////////////////////////////
	Mesh* mesh = new Mesh("Resource/object/base/Cube.FBX");
	GameObject * meshObj = new GameObject();
	meshObj->AddComponent(mesh);
}
void MoreCameraSpace::Update()
{

}
int MoreCameraSpace::Launch()
{
	return INSTANCE(Application).Run();
}