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


	GameObject * pbrWhiteObj = new GameObject(_ambient);
	MeshPBR * pbrw = new MeshPBR(_ambient);
	pbrw->SetMetallic(1.0f);
	pbrw->SetRoughness(0.05f);
	pbrWhiteObj->AddComponent(pbrw);

	GameObject * pbrRedObj = new GameObject(_ambient);
	pbrRedObj->GetComponent(Transform).SetPosition(2.0f, 0.0f, 0.0f);
	MeshPBR * pbrr = new MeshPBR(_ambient);
	pbrr->SetAlbedo(1.0f, 0.0f, 0.0f);
	pbrr->SetMetallic(1.0f);
	pbrr->SetRoughness(0.05f);
	pbrRedObj->AddComponent(pbrr);

	GameObject * pbrGreenObj = new GameObject(_ambient);
	pbrGreenObj->GetComponent(Transform).SetPosition(4.0f, 0.0f, 0.0f);
	MeshPBR * pbrg = new MeshPBR(_ambient);
	pbrg->SetAlbedo(0.0f, 1.0f, 0.0f);
	pbrg->SetMetallic(1.0f);
	pbrg->SetRoughness(0.05f);
	pbrGreenObj->AddComponent(pbrg);

	GameObject * pbrBlueObj = new GameObject(_ambient);
	pbrBlueObj->GetComponent(Transform).SetPosition(6.0f, 0.0f, 0.0f);
	MeshPBR * pbrb = new MeshPBR(_ambient);
	pbrb->SetAlbedo(0.0f, 0.0f, 1.0f);
	pbrb->SetMetallic(1.0f);
	pbrb->SetRoughness(0.05f);
	pbrBlueObj->AddComponent(pbrb);

	GameObject * pbrBlackObj = new GameObject(_ambient);
	pbrBlackObj->GetComponent(Transform).SetPosition(8.0f, 0.0f, 0.0f);
	MeshPBR * pbrbla = new MeshPBR(_ambient);
	pbrbla->SetAlbedo(0.0f, 0.0f, 0.0f);
	pbrbla->SetMetallic(1.0f);
	pbrbla->SetRoughness(0.05f);
	pbrBlackObj->AddComponent(pbrbla);

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