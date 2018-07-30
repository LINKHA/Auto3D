#include "PBRSpace.h"
#include "Application.h"
#include "LightDirectional.h"
#include "../FreeCamera.h"
#include "Mesh.h"
#include "PBR.h"
#include "PBRTexture.h"
PBRSpace::PBRSpace(Ambient* ambient)
	:MotionSpace(ambient)
{}
PBRSpace::~PBRSpace()
{}

void PBRSpace::Start()
{
	GameObject * cameraObj = new GameObject(_ambient);
	FreeCamera * camera = new FreeCamera(_ambient);
	cameraObj->GetComponent(Transform).SetPosition(0.0f, 0.0f, 3.0f);
	cameraObj->AddComponent(camera);

	GameObject * pbrObj = new GameObject(_ambient);
	//PBR * pbr = new PBR(_ambient);
	PBRTexture * pbr = new PBRTexture(_ambient);
	pbrObj->AddComponent(pbr);
}

void PBRSpace::Update()
{
}

int PBRSpace::Launch()
{
	return INSTANCE(Application).Run(_ambient);
}
AUTO_APPLICATION_MAIN(PBRSpace)