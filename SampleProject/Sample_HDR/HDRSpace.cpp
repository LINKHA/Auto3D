#include "HDRSpace.h"
#include "Application.h"
#include "Light.h"
#include "../FreeCamera.h"
#include "Mesh.h"
#include "LightDirectional.h"
#include "HDR.h"
HDRSpace::HDRSpace(Ambient* ambient)
	:MotionSpace(ambient)
{}
HDRSpace::~HDRSpace()
{}

void HDRSpace::Start()
{
	GameObject * cameraObj = new GameObject();
	FreeCamera * camera = new FreeCamera(_ambient);
	cameraObj->GetComponent(Transform).SetPosition(0.0f, 0.0f, 3.0f);
	cameraObj->AddComponent(camera);

	GameObject * hdrObj = new GameObject();
	HDR * hdr = new HDR();
	//ShadowPoint * shadow = new ShadowPoint();
	hdrObj->AddComponent(hdr);

}

void HDRSpace::Update()
{
}

int HDRSpace::Launch()
{
	return INSTANCE(Application).Run();
}
AUTO_APPLICATION_MAIN(HDRSpace)