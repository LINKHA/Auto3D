#include "DebugSapce.h"
#include "Application.h"
#include "LightDirectional.h"
#include "../FreeCamera.h"
#include "Mesh.h"
#include "Sprite.h"
#include "Input.h"
#include "Time.h"
DebugSapce::DebugSapce(Ambient* ambient)
	:MotionSpace(ambient)
{}
DebugSapce::~DebugSapce()
{}

void DebugSapce::Start()
{
	GameObject* cameraObj = new GameObject(_ambient);
	FreeCamera* camera = new FreeCamera(_ambient);

	//camera->freeCamera->SetSortMode(kSortOrthographic);
	cameraObj->GetComponent(Transform).SetPosition(0.0f, 0.0f, 3.0f);
	cameraObj->AddComponent(camera);

}
void DebugSapce::Update()
{
}
int DebugSapce::Launch()
{
	return INSTANCE(Application).Run(_ambient);
}
AUTO_APPLICATION_MAIN(DebugSapce)