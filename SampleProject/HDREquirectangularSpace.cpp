#include "HDREquirectangularSpace.h"
#include "Application.h"
#include "LightDirectional.h"
#include "FreeCamera.h"
#include "PBRMaterial.h"
#include "PBRTextureMaterial.h"
#include "HDRSkybox.h"

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


	GameObject * skyboxObj = new GameObject();
	HDRSkyBox * skybox = new HDRSkyBox();
	skyboxObj->AddComponent(skybox);


	GameObject * pbrObj = new GameObject();
	//PBRMaterial * pbr = new PBRMaterial();
	PBRTextureMaterial * pbr = new PBRTextureMaterial();
	pbrObj->AddComponent(pbr);

}

void HDREquirectangularSpace::Update()
{
	
}

int HDREquirectangularSpace::Launch()
{
	return INSTANCE(Application).Run();
}
