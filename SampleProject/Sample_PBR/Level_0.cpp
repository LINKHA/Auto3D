#include "Level_0.h"
#include "Node.h"
#include "Light.h"
#include "MeshPBR.h"
#include "MeshPBRTexture.h"
#include "MeshRenderer.h"
#include "ResourceSystem.h"
#include "../FreeCamera.h"

void Level_0::init()
{
	Node* cameraObj = CreateNode();
	FreeCamera* camera = new FreeCamera(_ambient);
	cameraObj->AddComponent(camera);
	camera->cameraNode->SetPosition(0.0f, 0.0f, 3.0f);

	Node* lightObj = CreateNode();
	lightObj->SetPosition(-10.0f, 10.0f, 10.0f);
	Light* light1 = lightObj->CreateComponent<Light>();
	light1->SetType(LightType::Point);
	light1->SetColor(300.0f, 300.0f, 300.0f);

	Node* lightObj2 = CreateNode();
	lightObj2->SetPosition(10.0f, 10.0f, 10.0f);
	Light* light2 = lightObj2->CreateComponent<Light>();
	light2->SetType(LightType::Point);
	light2->SetColor(300.0f, 300.0f, 300.0f);

	Node* lightObj3 = CreateNode();
	lightObj3->SetPosition(-10.0f, -10.0f, 10.0f);
	Light* light3 = lightObj3->CreateComponent<Light>();
	light3->SetType(LightType::Point);
	light3->SetColor(300.0f, 300.0f, 300.0f);

	Node* lightObj4 = CreateNode();
	lightObj4->SetPosition(10.0f, -10.0f, 10.0f);
	Light* light4 = lightObj4->CreateComponent<Light>();
	light4->SetType(LightType::Point);
	light4->SetColor(300.0f, 300.0f, 300.0f);

	Node* lightObj5 = CreateNode();
	lightObj5->SetPosition(3.0f, 10.0f, 10.0f);
	Light* light5 = lightObj5->CreateComponent<Light>();
	light5->SetType(LightType::Directional);
	light5->direction.Set(0.0f, -0.5f, -0.5f);

	Node* skyboxObj = CreateNode();
	HDRSkyBox* skybox = new HDRSkyBox(_ambient);
	skyboxObj->AddComponent(skybox);
}

void Level_0::back()
{
	Node* pbrWhiteObj = CreateNode();
	MeshPBR* pbrw = new MeshPBR(_ambient);
	pbrw->SetMetallic(1.0f);
	pbrw->SetRoughness(0.05f);
	pbrWhiteObj->AddComponent(pbrw);

	Node* pbrRedObj = CreateNode();
	pbrRedObj->SetPosition(2.0f, 0.0f, 0.0f);
	MeshPBR* pbrr = new MeshPBR(_ambient);
	pbrr->SetAlbedo(1.0f, 0.0f, 0.0f);
	pbrr->SetMetallic(1.0f);
	pbrr->SetRoughness(0.05f);
	pbrRedObj->AddComponent(pbrr);

	Node* pbrGreenObj = CreateNode();
	pbrGreenObj->SetPosition(4.0f, 0.0f, 0.0f);
	MeshPBR* pbrg = new MeshPBR(_ambient);
	pbrg->SetAlbedo(0.0f, 1.0f, 0.0f);
	pbrg->SetMetallic(1.0f);
	pbrg->SetRoughness(0.05f);
	pbrGreenObj->AddComponent(pbrg);

	Node* pbrBlueObj = CreateNode();
	pbrBlueObj->SetPosition(6.0f, 0.0f, 0.0f);
	MeshPBR* pbrb = new MeshPBR(_ambient);
	pbrb->SetAlbedo(0.0f, 0.0f, 1.0f);
	pbrb->SetMetallic(1.0f);
	pbrb->SetRoughness(0.05f);
	pbrBlueObj->AddComponent(pbrb);

	Node* pbrBlackObj = CreateNode();
	pbrBlackObj->SetPosition(8.0f, 0.0f, 0.0f);
	MeshPBR* pbrbla = new MeshPBR(_ambient);
	pbrbla->SetAlbedo(0.0f, 0.0f, 0.0f);
	pbrbla->SetMetallic(1.0f);
	pbrbla->SetRoughness(0.05f);
	pbrBlackObj->AddComponent(pbrbla);

	Node* pbrObj2 = CreateNode();
	MeshPBRTexture* pbrTexture = new MeshPBRTexture(_ambient);
	pbrObj2->AddComponent(pbrTexture);
	pbrObj2->SetPosition(-2.0f, 0.0f, 0.0f);
	for (int i = 0; i < 6; i++)
	{
		Node* pbrObj = CreateNode();
		pbrObj->SetPosition(-14.0f + i * 2.0f, 0.0f, 0.0f);
		MeshPBR* pbrmesh = new MeshPBR(_ambient);
		pbrmesh->SetAlbedo(1.0f, 1.0f, 1.0f);
		//pbrmesh->SetMetallic(1.0f - i * (float)1 / (float)18);
		pbrmesh->SetRoughness(0.05f);
		pbrObj->AddComponent(pbrmesh);
	}

	for (int i = 0; i < 6; i++)
	{
		Node* pbrObj = CreateNode();
		pbrObj->SetPosition(10.0f + i * 2.0f, 0.0f, 0.0f);
		MeshPBR* pbrmesh = new MeshPBR(_ambient);
		pbrmesh->SetAlbedo(1.0f, 1.0f, 1.0f);
		//pbrmesh->SetMetallic(1.0f - i * (float)1 / (float)18);
		pbrmesh->SetRoughness(0.05f);
		pbrObj->AddComponent(pbrmesh);
	}

	for (int i = 0; i < 18; i++)
	{
		Node* pbrObj = CreateNode();
		pbrObj->SetPosition(-14.0f + i * 2.0f, -2.0f, 2.0f);
		MeshPBR* pbrmesh = new MeshPBR(_ambient);
		pbrmesh->SetAlbedo(1.0f, 1.0f, 1.0f);
		pbrmesh->SetMetallic(1.0f - i * (float)1 / (float)18);
		pbrmesh->SetRoughness(0.05f);
		pbrObj->AddComponent(pbrmesh);
	}

	for (int i = 0; i < 18; i++)
	{
		Node* pbrObj = CreateNode();
		pbrObj->SetPosition(-14.0f + i * 2.0f, -4.0f, 4.0f);
		MeshPBR* pbrmesh = new MeshPBR(_ambient);
		pbrmesh->SetAlbedo(1.0f, 1.0f, 1.0f);
		pbrmesh->SetMetallic(1.0f);
		pbrmesh->SetRoughness(0.05f + i * (float)1 / (float)18);
		pbrObj->AddComponent(pbrmesh);
	}
	/// Storage rack cube
	Node* meshObj1 = CreateNode();
	meshObj1->SetPosition(3.0f, -2.0f, 0.0f);
	meshObj1->SetScale(12.0f, 2.0f, 2.0f);
	MeshRenderer* mesh1 = meshObj1->CreateComponent<MeshRenderer>();
	mesh1->GetMaterial()->SetImage("../Resource/texture/wood.jpg");

	Node* meshObj2 = CreateNode();
	meshObj2->SetPosition(3.0f, -4.0f, 2.0f);
	meshObj2->SetScale(12.0f, 2.0f, 2.0f);
	MeshRenderer* mesh2 = meshObj2->CreateComponent<MeshRenderer>();
	mesh2->GetMaterial()->SetImage("../Resource/texture/wood.jpg");

	Node* meshObj3 = CreateNode();
	meshObj3->SetPosition(3.0f, -6.0f, 4.0f);
	meshObj3->SetScale(12.0f, 2.0f, 2.0f);
	MeshRenderer* mesh3 = meshObj3->CreateComponent<MeshRenderer>();
	mesh3->GetMaterial()->SetImage("../Resource/texture/wood.jpg");

	Node* meshObj4 = CreateNode();
	meshObj4->SetPosition(3.0f, -6.0f, 2.0f);
	meshObj4->SetScale(12.0f, 2.0f, 2.0f);
	MeshRenderer* mesh4 = meshObj4->CreateComponent<MeshRenderer>();
	mesh4->GetMaterial()->SetImage("../Resource/texture/wood.jpg");

	Node* meshObj5 = CreateNode();
	meshObj5->SetPosition(3.0f, -4.0f, 0.0f);
	meshObj5->SetScale(12.0f, 2.0f, 2.0f);
	MeshRenderer* mesh5 = meshObj5->CreateComponent<MeshRenderer>();
	mesh5->GetMaterial()->SetImage("../Resource/texture/wood.jpg");

	Node* meshObj6 = CreateNode();
	meshObj6->SetPosition(3.0f, -6.0f, 0.0f);
	meshObj6->SetScale(12.0f, 2.0f, 2.0f);
	MeshRenderer* mesh6 = meshObj6->CreateComponent<MeshRenderer>();
	mesh6->GetMaterial()->SetImage("../Resource/texture/wood.jpg");


}
void Level_0::wood(float offset)
{
	/// Storage rack cube
	Node* meshObj1 = CreateNode();
	meshObj1->SetPosition(offset, -2.0f, 0.0f);
	meshObj1->SetScale(12.0f, 2.0f, 2.0f);
	MeshRenderer* mesh1 = meshObj1->CreateComponent<MeshRenderer>();
	mesh1->SetMesh(cube);
	mesh1->GetMaterial()->SetImage("../Resource/texture/wood.jpg");

	Node* meshObj2 = CreateNode();
	meshObj2->SetPosition(offset, -4.0f, 2.0f);
	meshObj2->SetScale(12.0f, 2.0f, 2.0f);
	MeshRenderer* mesh2 = meshObj2->CreateComponent<MeshRenderer>();
	mesh2->SetMesh(cube);
	mesh2->GetMaterial()->SetImage("../Resource/texture/wood.jpg");

	Node* meshObj3 = CreateNode();
	meshObj3->SetPosition(offset, -6.0f, 4.0f);
	meshObj3->SetScale(12.0f, 2.0f, 2.0f);
	MeshRenderer* mesh3 = meshObj3->CreateComponent<MeshRenderer>();
	mesh3->SetMesh(cube);
	mesh3->GetMaterial()->SetImage("../Resource/texture/wood.jpg");

	Node* meshObj4 = CreateNode();
	meshObj4->SetPosition(offset, -6.0f, 2.0f);
	meshObj4->SetScale(12.0f, 2.0f, 2.0f);
	MeshRenderer* mesh4 = meshObj4->CreateComponent<MeshRenderer>();
	mesh4->SetMesh(cube);
	mesh4->GetMaterial()->SetImage("../Resource/texture/wood.jpg");

	Node* meshObj5 = CreateNode();
	meshObj5->SetPosition(offset, -4.0f, 0.0f);
	meshObj5->SetScale(12.0f, 2.0f, 2.0f);
	MeshRenderer* mesh5 = meshObj5->CreateComponent<MeshRenderer>();
	mesh5->SetMesh(cube);
	mesh5->GetMaterial()->SetImage("../Resource/texture/wood.jpg");

	Node* meshObj6 = CreateNode();
	meshObj6->SetPosition(offset, -6.0f, 0.0f);
	meshObj6->SetScale(12.0f, 2.0f, 2.0f);
	MeshRenderer* mesh6 = meshObj6->CreateComponent<MeshRenderer>();
	mesh6->SetMesh(cube);
	mesh6->GetMaterial()->SetImage("../Resource/texture/wood.jpg");
}

void Level_0::demo()
{
	Node* cameraObj = CreateNode();
	FreeCamera* camera = new FreeCamera(_ambient);
	cameraObj->AddComponent(camera);
	camera->cameraNode->SetPosition(0.0f, 0.0f, 3.0f);


	Node* lightObj = CreateNode();
	lightObj->SetPosition(-10.0f, 10.0f, 10.0f);
	Light* light1 = lightObj->CreateComponent<Light>();
	light1->SetType(LightType::Point);
	light1->SetColor(300.0f, 300.0f, 300.0f);

	Node* lightObj2 = CreateNode();
	lightObj2->SetPosition(10.0f, 10.0f, 10.0f);
	Light* light2 = lightObj2->CreateComponent<Light>();
	light2->SetType(LightType::Point);
	light2->SetColor(300.0f, 300.0f, 300.0f);

	Node* lightObj3 = CreateNode();
	lightObj3->SetPosition(-10.0f, -10.0f, 10.0f);
	Light* light3 = lightObj3->CreateComponent<Light>();
	light3->SetType(LightType::Point);
	light3->SetColor(300.0f, 300.0f, 300.0f);

	Node* lightObj4 = CreateNode();
	lightObj4->SetPosition(10.0f, -10.0f, 10.0f);
	Light* light4 = lightObj4->CreateComponent<Light>();
	light4->SetType(LightType::Point);
	light4->SetColor(300.0f, 300.0f, 300.0f);

	Node* lightObj5 = CreateNode();
	lightObj5->SetPosition(3.0f, 10.0f, 10.0f);
	Light* light5 = lightObj5->CreateComponent<Light>();
	light5->SetType(LightType::Directional);
	light5->direction.Set(0.0f, -0.5f, -0.5f);

	Node* skyboxObj = CreateNode();
	HDRSkyBox * skybox = new HDRSkyBox(_ambient);
	skyboxObj->AddComponent(skybox);


	Node* pbrWhiteObj = CreateNode();
	MeshPBR* pbrw = new MeshPBR(_ambient);
	pbrw->SetMetallic(1.0f);
	pbrw->SetRoughness(0.05f);
	pbrWhiteObj->AddComponent(pbrw);

	Node* pbrRedObj = CreateNode();
	pbrRedObj->SetPosition(2.0f, 0.0f, 0.0f);
	MeshPBR* pbrr = new MeshPBR(_ambient);
	pbrr->SetAlbedo(1.0f, 0.0f, 0.0f);
	pbrr->SetMetallic(1.0f);
	pbrr->SetRoughness(0.05f);
	pbrRedObj->AddComponent(pbrr);

	Node* pbrGreenObj = CreateNode();
	pbrGreenObj->SetPosition(4.0f, 0.0f, 0.0f);
	MeshPBR* pbrg = new MeshPBR(_ambient);
	pbrg->SetAlbedo(0.0f, 1.0f, 0.0f);
	pbrg->SetMetallic(1.0f);
	pbrg->SetRoughness(0.05f);
	pbrGreenObj->AddComponent(pbrg);

	Node* pbrBlueObj = CreateNode();
	pbrBlueObj->SetPosition(6.0f, 0.0f, 0.0f);
	MeshPBR* pbrb = new MeshPBR(_ambient);
	pbrb->SetAlbedo(0.0f, 0.0f, 1.0f);
	pbrb->SetMetallic(1.0f);
	pbrb->SetRoughness(0.05f);
	pbrBlueObj->AddComponent(pbrb);

	Node* pbrBlackObj = CreateNode();
	pbrBlackObj->SetPosition(8.0f, 0.0f, 0.0f);
	MeshPBR* pbrbla = new MeshPBR(_ambient);
	pbrbla->SetAlbedo(0.0f, 0.0f, 0.0f);
	pbrbla->SetMetallic(1.0f);
	pbrbla->SetRoughness(0.05f);
	pbrBlackObj->AddComponent(pbrbla);

	Node* pbrObj2 = CreateNode();
	MeshPBRTexture* pbrTexture = new MeshPBRTexture(_ambient);
	pbrObj2->AddComponent(pbrTexture);
	pbrObj2->SetPosition(-2.0f, 0.0f, 0.0f);

	for (int i = 0; i < 6; i++)
	{
		Node* pbrObj = CreateNode();
		pbrObj->SetPosition(-2.0f + i * 2.0f, -2.0f, 2.0f);
		MeshPBR* pbrmesh = new MeshPBR(_ambient);
		pbrmesh->SetAlbedo(1.0f, 1.0f, 1.0f);
		pbrmesh->SetMetallic(1.0f - i * (float)1 / (float)6);
		pbrmesh->SetRoughness(0.05f);
		pbrObj->AddComponent(pbrmesh);
	}

	for (int i = 0; i < 6; i++)
	{
		Node* pbrObj = CreateNode();
		pbrObj->SetPosition(-2.0f + i * 2.0f, -4.0f, 4.0f);
		MeshPBR* pbrmesh = new MeshPBR(_ambient);
		pbrmesh->SetAlbedo(1.0f, 1.0f, 1.0f);
		pbrmesh->SetMetallic(1.0f);
		pbrmesh->SetRoughness(0.05f + i * (float)1 / (float)6);
		pbrObj->AddComponent(pbrmesh);
	}



	/// Storage rack cube
	Node* meshObj1 = CreateNode();
	meshObj1->SetPosition(3.0f, -2.0f, 0.0f);
	meshObj1->SetScale(12.0f, 2.0f, 2.0f);
	MeshRenderer* mesh1 = meshObj1->CreateComponent<MeshRenderer>();
	mesh1->SetMesh(cube);
	mesh1->GetMaterial()->SetImage("../Resource/texture/wood.jpg");

	Node* meshObj2 = CreateNode();
	meshObj2->SetPosition(3.0f, -4.0f, 2.0f);
	meshObj2->SetScale(12.0f, 2.0f, 2.0f);
	MeshRenderer* mesh2 = meshObj2->CreateComponent<MeshRenderer>();
	mesh2->SetMesh(cube);
	mesh2->GetMaterial()->SetImage("../Resource/texture/wood.jpg");

	Node* meshObj3 = CreateNode();
	meshObj3->SetPosition(3.0f, -6.0f, 4.0f);
	meshObj3->SetScale(12.0f, 2.0f, 2.0f);
	MeshRenderer* mesh3 = meshObj3->CreateComponent<MeshRenderer>();
	mesh3->SetMesh(cube);
	mesh3->GetMaterial()->SetImage("../Resource/texture/wood.jpg");
}

void Level_0::Start()
{
	cube = GetSubSystem<ResourceSystem>()->GetResource<Mesh>("object/base/Cube.3DS");
#if USE_DEMO
	demo();
#else
	init();
	back();
	wood(15.0f);
	wood(3.0f);
	wood(-9.0f);
#endif


	Node* floorObj = CreateNode();
	floorObj->SetPosition(3.0f, -8.0f, 4.0f);
	floorObj->SetScale(50.0f, 2.0f, 50.0f);
	MeshRenderer* mesh10 = floorObj->CreateComponent<MeshRenderer>();
	mesh10->SetMesh(cube);
	mesh10->GetMaterial()->SetImage("../Resource/texture/wood.jpg");
}

void Level_0::Update()
{

}

