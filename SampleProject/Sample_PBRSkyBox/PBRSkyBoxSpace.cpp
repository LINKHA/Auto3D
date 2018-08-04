#include "PBRSkyBoxSpace.h"
#include "Application.h"
#include "LightDirectional.h"
#include "../FreeCamera.h"
#include "PBRMaterial.h"
#include "PBRTextureMaterial.h"
#include "HDRSkybox.h"
#include "Sprite.h"
PBRSkyBoxSpace::PBRSkyBoxSpace(Ambient* ambient)
	:MotionSpace(ambient)
{}
PBRSkyBoxSpace::~PBRSkyBoxSpace()
{}

void PBRSkyBoxSpace::Start()
{
	GameObject * cameraObj = new GameObject(_ambient);
	FreeCamera * camera = new FreeCamera(_ambient);
	cameraObj->GetComponent(Transform).SetPosition(0.0f, 0.0f, 3.0f);
	cameraObj->AddComponent(camera);

	GameObject * skyboxObj = new GameObject(_ambient);
	HDRSkyBox * skybox = new HDRSkyBox(_ambient);
	skyboxObj->AddComponent(skybox);


	GameObject * pbrObj = new GameObject(_ambient);
	//PBRMaterial * pbr = new PBRMaterial(_ambient);
	PBRTextureMaterial * pbr = new PBRTextureMaterial(_ambient);
	pbrObj->AddComponent(pbr);

}

void PBRSkyBoxSpace::Update()
{
	
}

int PBRSkyBoxSpace::Launch()
{
	return INSTANCE(Application).Run(_ambient);
}
AUTO_APPLICATION_MAIN(PBRSkyBoxSpace)