#include "PBRSpace.h"
#include "Application.h"
#include "LightDirectional.h"
#include "FreeCamera.h"
#include "Mesh.h"
#include "PBR.h"
#include "PBRTexture.h"
#include "SampleSet.h"
PBRSpace::PBRSpace()
{}
PBRSpace::~PBRSpace()
{}

void PBRSpace::Start()
{
	GameObject * cameraObj = new GameObject();
	FreeCamera * camera = new FreeCamera();
	cameraObj->GetComponent(Transform).SetPosition(0.0f, 0.0f, 3.0f);
	cameraObj->AddComponent(camera);

	GameObject * pbrObj = new GameObject();
	PBR * pbr = new PBR();
	//PBRTexture * pbr = new PBRTexture();
	pbrObj->AddComponent(pbr);
}

void PBRSpace::Update()
{
}

int PBRSpace::Launch()
{
	return INSTANCE(Application).Run();
}
#if uPBRSpace
AUTO_APPLICATION_MAIN(PBRSpace)
#endif 