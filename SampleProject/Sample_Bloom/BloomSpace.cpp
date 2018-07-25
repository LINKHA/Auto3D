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
	GameObject * cameraObj = new GameObject();
	FreeCamera * camera = new FreeCamera(_ambient);
	cameraObj->GetComponent(Transform).SetPosition(0.0f, 0.0f, 3.0f);
	cameraObj->AddComponent(camera);

	GameObject * bloomObj = new GameObject();
	Bloom * bloom = new Bloom();
	bloomObj->AddComponent(bloom);

}

void BloomSpace::Update()
{
}

int BloomSpace::Launch()
{
	return INSTANCE(Application).Run();
}
AUTO_APPLICATION_MAIN(BloomSpace)
