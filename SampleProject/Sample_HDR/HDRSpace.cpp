#include "HDRSpace.h"
#include "Application.h"
#include "Light.h"
#include "../FreeCamera.h"
#include "Mesh.h"
#include "LightDirectional.h"
#include "HDRs.h"
HDRSpace::HDRSpace(Ambient* ambient)
	:MotionSpace(ambient)
{}
HDRSpace::~HDRSpace()
{}

void HDRSpace::Start()
{
	GameObject * cameraObj = new GameObject(_ambient);
	FreeCamera * camera = new FreeCamera(_ambient);
	camera->freeCamera->AllowHDR(true);
	//camera->freeCamera->AllowMSAA(true);
	cameraObj->GetComponent(Transform).SetPosition(0.0f, 0.0f, 3.0f);
	cameraObj->AddComponent(camera);

	GameObject * hdrObj = new GameObject(_ambient);
	HDRs * hdrs = new HDRs(_ambient);
	hdrObj->AddComponent(hdrs);

}

void HDRSpace::Update()
{
}

int HDRSpace::Launch()
{
	return INSTANCE(Application).Run(_ambient);
}
AUTO_APPLICATION_MAIN(HDRSpace)