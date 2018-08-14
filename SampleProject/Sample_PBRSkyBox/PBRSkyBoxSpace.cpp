#include "PBRSkyBoxSpace.h"
#include "Application.h"
#include "LightPoint.h"
#include "../FreeCamera.h"
#include "PBRMaterial.h"
#include "PBRTextureMaterial.h"
#include "HDRSkybox.h"
#include "MeshPBR.h"
#include "MeshPBRTexture.h"

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

	GameObject * lightObj = new GameObject(_ambient);
	lightObj->GetComponent(Transform).SetPosition(-10.0f, 10.0f, 10.0f);
	Light* light1 = new LightPoint(_ambient);
	light1->SetColor(300.0f, 300.0f, 300.0f);
	lightObj->AddComponent(light1);

	GameObject * lightObj2 = new GameObject(_ambient);
	lightObj2->GetComponent(Transform).SetPosition(10.0f, 10.0f, 10.0f);
	Light* light2 = new LightPoint(_ambient);
	light2->SetColor(300.0f, 300.0f, 300.0f);
	lightObj2->AddComponent(light2);

	GameObject * lightObj3 = new GameObject(_ambient);
	lightObj3->GetComponent(Transform).SetPosition(-10.0f, -10.0f, 10.0f);
	Light* light3 = new LightPoint(_ambient);
	light3->SetColor(300.0f, 300.0f, 300.0f);
	lightObj3->AddComponent(light3);

	GameObject * lightObj4 = new GameObject(_ambient);
	lightObj4->GetComponent(Transform).SetPosition(10.0f, -10.0f, 10.0f);
	Light* light4 = new LightPoint(_ambient);
	light4->SetColor(300.0f, 300.0f, 300.0f);
	lightObj4->AddComponent(light4);



	GameObject * skyboxObj = new GameObject(_ambient);
	HDRSkyBox * skybox = new HDRSkyBox(_ambient);
	skyboxObj->AddComponent(skybox);


	//GameObject * pbrObj = new GameObject(_ambient);
	//PBRMaterial * pbr = new PBRMaterial(_ambient);
	////PBRTextureMaterial * pbr = new PBRTextureMaterial(_ambient);
	//pbrObj->AddComponent(pbr);


	GameObject * pbrObj = new GameObject(_ambient);
	MeshPBR * pbr = new MeshPBR(_ambient);
	pbr->SetMetallic(1.0f);
	pbr->SetRoughness(0.02f);
	pbrObj->AddComponent(pbr);


	GameObject * pbrObj2 = new GameObject(_ambient);
	MeshPBRTexture * pbrTexture = new MeshPBRTexture(_ambient);
	pbrObj2->AddComponent(pbrTexture);
	pbrObj2->GetComponent(Transform).SetPosition(-2.0f, 0.0f, 0.0f);

}

void PBRSkyBoxSpace::Update()
{
	
}

int PBRSkyBoxSpace::Launch()
{
	return INSTANCE(Application).Run(_ambient);
}
AUTO_APPLICATION_MAIN(PBRSkyBoxSpace)