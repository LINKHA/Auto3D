#include "PBRSample.h"

void PBRSample::Init()
{
	Super::Init();
	auto* graphics = GModuleManager::Get().GraphicsModule();
	graphics->RenderWindow()->SetTitle("PBR Sample");
}

void PBRSample::Start()
{
	Super::Start();
	auto* cache = GModuleManager::Get().CacheModule();
	auto* graphics = GModuleManager::Get().GraphicsModule();

	graphics->RenderWindow()->SetMouseLock(true);
	graphics->RenderWindow()->SetMouseHide(true);

	scene = new AScene();
	scene->CreateChild<AOctree>();
	camera = scene->CreateChild<ACamera>();
	camera->SetPosition(TVector3F(0.0f, 5.0f, -5.0f));
	camera->SetAmbientColor(FColor(0.5f, 0.5f, 0.5f));

	ASkyBox* skybox = scene->CreateChild<ASkyBox>();
	skybox->SetMaterial(cache->LoadResource<AMaterial>("HdrSkyBox.json"));
	skybox->SetupIBLMap();

	AStaticModel* teaPot = scene->CreateChild<AStaticModel>();
	teaPot->SetPosition(TVector3F(0.0f, 0.5f, 0.0f));
	teaPot->SetScale(10);
	teaPot->SetModel(cache->LoadResource<AModel>("Model/TeaPot.mdl"));
	teaPot->SetMaterial(cache->LoadResource<AIBLMaterial>("PBRTitanium.json"));

	AStaticModel* sphere = scene->CreateChild<AStaticModel>();
	sphere->SetPosition(TVector3F(6.0f, 1.5f, 0.0f));
	sphere->SetScale(2);
	sphere->SetCastShadows(true);
	sphere->SetModel(cache->LoadResource<AModel>("Model/Sphere.mdl"));
	sphere->SetMaterial(cache->LoadResource<AIBLMaterial>("PBRGold.json"));

	AStaticModel* plane = scene->CreateChild<AStaticModel>();
	plane->SetScale(TVector3F(100.0f, 1.0f, 100.0f));
	plane->SetCastShadows(true);
	plane->SetModel(cache->LoadResource<AModel>("Model/Box.mdl"));
	plane->SetMaterial(cache->LoadResource<AIBLMaterial>("PBRWood.json"));


	ALight* light = scene->CreateChild<ALight>();
	light->SetLightType(ELightType::POINT);
	light->SetCastShadows(true);
	light->SetColor(FColor(500.0f, 500.0f, 500.0f));
	light->SetRange(100.0f);
	light->SetPosition(TVector3F(-10.0f, 10.0f, 10.0f));

	ALight* light2 = scene->CreateChild<ALight>();
	light2->SetLightType(ELightType::POINT);
	light2->SetCastShadows(true);
	light2->SetColor(FColor(500.0f, 500.0f, 500.0f));
	light2->SetRange(100.0f);
	light2->SetPosition(TVector3F(10.0f, 10.0f, 10.0f));

	ALight* light3 = scene->CreateChild<ALight>();
	light3->SetLightType(ELightType::POINT);
	light3->SetCastShadows(true);
	light3->SetColor(FColor(500.0f, 500.0f, 500.0f));
	light3->SetRange(100.0f);
	light3->SetPosition(TVector3F(-10.0f, -10.0f, 10.0f));

	ALight* light4 = scene->CreateChild<ALight>();
	light4->SetLightType(ELightType::POINT);
	light4->SetCastShadows(true);
	light4->SetColor(FColor(500.0f, 500.0f, 500.0f));
	light4->SetRange(100.0f);
	light4->SetPosition(TVector3F(10.0f, -10.0f, 10.0f));
}

void PBRSample::UIDraw()
{

}

void PBRSample::Update()
{

	Super::Update();
	auto input = GModuleManager::Get().InputModule();
	auto graphics = GModuleManager::Get().GraphicsModule();
	auto renderer = GModuleManager::Get().RendererModule();
	auto time = GModuleManager::Get().TimeModule();

	pitch += input->GetMouseMove()._y * 0.25f;
	yaw += input->GetMouseMove()._x * 0.25f;
	pitch = Clamp(pitch, -90.0f, 90.0f);

	float moveSpeed = input->IsKeyDown(KEY_LSHIFT) ? 50 : 10.0f;

	camera->SetRotation(FQuaternion(pitch, yaw, 0.0f));
	if (input->IsKeyDown(KEY_W))
		camera->Translate(TVector3F::FORWARD * time->GetDeltaTime() * moveSpeed);
	if (input->IsKeyDown(KEY_S))
		camera->Translate(TVector3F::BACK * time->GetDeltaTime()  * moveSpeed);
	if (input->IsKeyDown(KEY_A))
		camera->Translate(TVector3F::LEFT * time->GetDeltaTime()  * moveSpeed);
	if (input->IsKeyDown(KEY_D))
		camera->Translate(TVector3F::RIGHT * time->GetDeltaTime()  * moveSpeed);
}

void PBRSample::Stop()
{
	Super::Stop();
}

AUTO_APPLICATION_MAIN(PBRSample)