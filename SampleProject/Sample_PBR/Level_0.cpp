#include "Level_0.h"
#include "GameObject.h"
#include "LightPoint.h"
#include "LightDirectional.h"
#include "MeshPBR.h"
#include "MeshPBRTexture.h"
#include "Mesh.h"
#include "../FreeCamera.h"


Level_0::Level_0(Ambient* ambient, int levelNumber)
	:LevelScene(ambient, levelNumber)
{}

void Level_0::Start()
{
	GameObject* cameraObj = new GameObject(_ambient,_levelNumber);
	FreeCamera* camera = new FreeCamera(_ambient, _levelNumber);
	cameraObj->GetComponent(Transform).SetPosition(0.0f, 0.0f, 3.0f);
	cameraObj->AddComponent(camera);

	GameObject* lightObj = new GameObject(_ambient, _levelNumber);
	lightObj->GetComponent(Transform).SetPosition(-10.0f, 10.0f, 10.0f);
	Light* light1 = new LightPoint(_ambient);
	light1->SetColor(300.0f, 300.0f, 300.0f);
	lightObj->AddComponent(light1);

	GameObject* lightObj2 = new GameObject(_ambient, _levelNumber);
	lightObj2->GetComponent(Transform).SetPosition(10.0f, 10.0f, 10.0f);
	Light* light2 = new LightPoint(_ambient);
	light2->SetColor(300.0f, 300.0f, 300.0f);
	lightObj2->AddComponent(light2);

	GameObject* lightObj3 = new GameObject(_ambient, _levelNumber);
	lightObj3->GetComponent(Transform).SetPosition(-10.0f, -10.0f, 10.0f);
	Light* light3 = new LightPoint(_ambient);
	light3->SetColor(300.0f, 300.0f, 300.0f);
	lightObj3->AddComponent(light3);

	GameObject* lightObj4 = new GameObject(_ambient, _levelNumber);
	lightObj4->GetComponent(Transform).SetPosition(10.0f, -10.0f, 10.0f);
	Light* light4 = new LightPoint(_ambient);
	light4->SetColor(300.0f, 300.0f, 300.0f);
	lightObj4->AddComponent(light4);

	GameObject* lightObj5 = new GameObject(_ambient, _levelNumber);
	lightObj5->GetComponent(Transform).SetPosition(3.0f, 10.0f, 10.0f);
	Light* light5 = new LightDirectional(_ambient);
	light5->direction.Set(0.0f, -0.5f, -0.5f);
	lightObj5->AddComponent(light5);

	GameObject* skyboxObj = new GameObject(_ambient, _levelNumber);
	HDRSkyBox * skybox = new HDRSkyBox(_ambient);
	skyboxObj->AddComponent(skybox);


	GameObject* pbrWhiteObj = new GameObject(_ambient, _levelNumber);
	MeshPBR* pbrw = new MeshPBR(_ambient);
	pbrw->SetMetallic(1.0f);
	pbrw->SetRoughness(0.05f);
	pbrWhiteObj->AddComponent(pbrw);

	GameObject* pbrRedObj = new GameObject(_ambient, _levelNumber);
	pbrRedObj->GetComponent(Transform).SetPosition(2.0f, 0.0f, 0.0f);
	MeshPBR* pbrr = new MeshPBR(_ambient);
	pbrr->SetAlbedo(1.0f, 0.0f, 0.0f);
	pbrr->SetMetallic(1.0f);
	pbrr->SetRoughness(0.05f);
	pbrRedObj->AddComponent(pbrr);

	GameObject* pbrGreenObj = new GameObject(_ambient, _levelNumber);
	pbrGreenObj->GetComponent(Transform).SetPosition(4.0f, 0.0f, 0.0f);
	MeshPBR* pbrg = new MeshPBR(_ambient);
	pbrg->SetAlbedo(0.0f, 1.0f, 0.0f);
	pbrg->SetMetallic(1.0f);
	pbrg->SetRoughness(0.05f);
	pbrGreenObj->AddComponent(pbrg);

	GameObject* pbrBlueObj = new GameObject(_ambient, _levelNumber);
	pbrBlueObj->GetComponent(Transform).SetPosition(6.0f, 0.0f, 0.0f);
	MeshPBR* pbrb = new MeshPBR(_ambient);
	pbrb->SetAlbedo(0.0f, 0.0f, 1.0f);
	pbrb->SetMetallic(1.0f);
	pbrb->SetRoughness(0.05f);
	pbrBlueObj->AddComponent(pbrb);

	GameObject* pbrBlackObj = new GameObject(_ambient, _levelNumber);
	pbrBlackObj->GetComponent(Transform).SetPosition(8.0f, 0.0f, 0.0f);
	MeshPBR* pbrbla = new MeshPBR(_ambient);
	pbrbla->SetAlbedo(0.0f, 0.0f, 0.0f);
	pbrbla->SetMetallic(1.0f);
	pbrbla->SetRoughness(0.05f);
	pbrBlackObj->AddComponent(pbrbla);

	GameObject* pbrObj2 = new GameObject(_ambient, _levelNumber);
	MeshPBRTexture* pbrTexture = new MeshPBRTexture(_ambient);
	pbrObj2->AddComponent(pbrTexture);
	pbrObj2->GetComponent(Transform).SetPosition(-2.0f, 0.0f, 0.0f);

	for (int i = 0; i<6; i++)
	{
		GameObject* pbrObj = new GameObject(_ambient, _levelNumber);
		pbrObj->GetComponent(Transform).SetPosition(-2.0f + i * 2.0f, -2.0f, 2.0f);
		MeshPBR* pbrmesh = new MeshPBR(_ambient);
		pbrmesh->SetAlbedo(1.0f, 1.0f, 1.0f);
		pbrmesh->SetMetallic(1.0f - i * (float)1 / (float)6);
		pbrmesh->SetRoughness(0.05f);
		pbrObj->AddComponent(pbrmesh);
	}

	for (int i = 0; i < 6; i++)
	{
		GameObject* pbrObj = new GameObject(_ambient, _levelNumber);
		pbrObj->GetComponent(Transform).SetPosition(-2.0f + i * 2.0f, -4.0f, 4.0f);
		MeshPBR* pbrmesh = new MeshPBR(_ambient);
		pbrmesh->SetAlbedo(1.0f, 1.0f, 1.0f);
		pbrmesh->SetMetallic(1.0f);
		pbrmesh->SetRoughness(0.05f + i * (float)1 / (float)6);
		pbrObj->AddComponent(pbrmesh);
	}



	/// Storage rack cube
	GameObject* meshObj1 = new GameObject(_ambient, _levelNumber);
	meshObj1->GetComponent(Transform).SetPosition(3.0f, -2.0f, 0.0f);
	meshObj1->GetComponent(Transform).SetScale(12.0f, 2.0f, 2.0f);

	Mesh* mesh1 = new Mesh(_ambient);
	mesh1->GetMaterial()->SetImage("../Resource/texture/wood.jpg");
	meshObj1->AddComponent(mesh1);

	GameObject* meshObj2 = new GameObject(_ambient, _levelNumber);
	meshObj2->GetComponent(Transform).SetPosition(3.0f, -4.0f, 2.0f);
	meshObj2->GetComponent(Transform).SetScale(12.0f, 2.0f, 2.0f);

	Mesh* mesh2 = new Mesh(_ambient);
	mesh2->GetMaterial()->SetImage("../Resource/texture/wood.jpg");
	meshObj2->AddComponent(mesh2);

	GameObject* meshObj3 = new GameObject(_ambient, _levelNumber);
	meshObj3->GetComponent(Transform).SetPosition(3.0f, -6.0f, 4.0f);
	meshObj3->GetComponent(Transform).SetScale(12.0f, 2.0f, 2.0f);
	Mesh* mesh3 = new Mesh(_ambient);
	mesh3->GetMaterial()->SetImage("../Resource/texture/wood.jpg");
	meshObj3->AddComponent(mesh3);


	GameObject* floorObj = new GameObject(_ambient, _levelNumber);
	floorObj->GetComponent(Transform).SetPosition(3.0f, -8.0f, 4.0f);
	floorObj->GetComponent(Transform).SetScale(50.0f, 2.0f, 50.0f);
	Mesh* mesh4 = new Mesh(_ambient);
	mesh4->GetMaterial()->SetImage("../Resource/texture/wood.jpg");
	floorObj->AddComponent(mesh4);
}

void Level_0::Update()
{

}

