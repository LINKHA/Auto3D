#include "HDREquirectangularSpace.h"
#include "Application.h"
#include "LightDirectional.h"
#include "FreeCamera.h"
#include "HDREquirectangular.h"
HDREquirectangularSpace::HDREquirectangularSpace()
{}
HDREquirectangularSpace::~HDREquirectangularSpace()
{}

void HDREquirectangularSpace::Start()
{
	GameObject * cameraObj = new GameObject();
	FreeCamera * camera = new FreeCamera();
	cameraObj->GetComponent(Transform).SetPosition(0.0f, 0.0f, 3.0f);
	cameraObj->AddComponent(camera);

	GameObject * hdrObj = new GameObject();
	HDREquirectangular * hdr = new HDREquirectangular();
	hdrObj->AddComponent(hdr);

}

void HDREquirectangularSpace::Update()
{
}

int HDREquirectangularSpace::Launch()
{
	return INSTANCE(Application).Run();
}
