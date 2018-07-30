#include "BloomSpace.h"
#include "Application.h"
#include "LightDirectional.h"
#include "Mesh.h"
#include "Bloom.h"
#include "../FreeCamera.h"
BloomSpace::BloomSpace(Ambient* ambient)
	:MotionSpace(ambient)
{}
BloomSpace::~BloomSpace()
{}

void BloomSpace::Start()
{
	GameObject * cameraObj = new GameObject(_ambient);
	FreeCamera * camera = new FreeCamera(_ambient);
	cameraObj->GetComponent(Transform).SetPosition(0.0f, 0.0f, 3.0f);
	cameraObj->AddComponent(camera);

	GameObject * bloomObj = new GameObject(_ambient);
	Bloom * bloom = new Bloom(_ambient);
	bloomObj->AddComponent(bloom);

}

void BloomSpace::Update()
{
}

int BloomSpace::Launch()
{
	return INSTANCE(Application).Run(_ambient);
}
AUTO_APPLICATION_MAIN(BloomSpace)
