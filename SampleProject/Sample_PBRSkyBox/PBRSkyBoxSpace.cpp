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
	GameObject * cameraObj = new GameObject();
	FreeCamera * camera = new FreeCamera(_ambient);
	cameraObj->GetComponent(Transform).SetPosition(0.0f, 0.0f, 3.0f);
	cameraObj->AddComponent(camera);

	

	GameObject * skyboxObj = new GameObject();
	HDRSkyBox * skybox = new HDRSkyBox();
	skyboxObj->AddComponent(skybox);


	GameObject * pbrObj = new GameObject();
	PBRMaterial * pbr = new PBRMaterial();
	//PBRTextureMaterial * pbr = new PBRTextureMaterial();
	pbrObj->AddComponent(pbr);

}

void PBRSkyBoxSpace::Update()
{
	
}

int PBRSkyBoxSpace::Launch()
{
	return INSTANCE(Application).Run();
}
AUTO_APPLICATION_MAIN(PBRSkyBoxSpace)