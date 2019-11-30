#include "SkyboxSample.h"

void SkyboxSample::Init()
{
	Super::Init();
	auto* graphics = GModuleManager::Get().GraphicsModule();
	graphics->RenderWindow()->SetTitle("Skybox Sample");
}

void SkyboxSample::Start()
{
	Super::Start();
	auto* cache = GModuleManager::Get().CacheModule();
	auto* graphics = GModuleManager::Get().GraphicsModule();

	graphics->RenderWindow()->SetMouseLock(true);
	graphics->RenderWindow()->SetMouseHide(true);

	scene = AObject::Create<AScene>();
	scene->CreateChild<AOctree>();
	camera = scene->CreateChild<ACamera>();
	camera->SetPosition(TVector3F(0.0f, 0.0f, 0.0f));
	camera->SetAmbientColor(FColor(0.1f, 0.1f, 0.1f));

	ASkyBox* skybox = scene->CreateChild<ASkyBox>();
	skybox->SetMaterial(cache->LoadResource<AMaterial>("SkyBox.json"));
}
void SkyboxSample::Update()
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

void SkyboxSample::Stop()
{
	Super::Stop();
}

AUTO_APPLICATION_MAIN(SkyboxSample)