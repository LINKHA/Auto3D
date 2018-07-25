#include "SSAOSpace.h"
#include "Application.h"
#include "LightDirectional.h"
#include "../FreeCamera.h"
#include "Mesh.h"
#include "SSAO.h"
SSAOSpace::SSAOSpace(Ambient* ambient)
	:MotionSpace(ambient)
{}
SSAOSpace::~SSAOSpace()
{}

void SSAOSpace::Start()
{
	GameObject * cameraObj = new GameObject();
	FreeCamera * camera = new FreeCamera(_ambient);
	cameraObj->GetComponent(Transform).SetPosition(0.0f, 0.0f, 3.0f);
	cameraObj->AddComponent(camera);

	GameObject * ssaoObj = new GameObject();
	SSAO * ssao = new SSAO();
	ssaoObj->AddComponent(ssao);

}

void SSAOSpace::Update()
{
}

int SSAOSpace::Launch()
{
	return INSTANCE(Application).Run();
}
AUTO_APPLICATION_MAIN(SSAOSpace)