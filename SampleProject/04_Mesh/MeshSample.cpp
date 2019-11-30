#include "MeshSample.h"

void MeshSample::Init()
{
	Super::Init();
	auto* graphics = GModuleManager::Get().GraphicsModule();
	graphics->RenderWindow()->SetTitle("Mesh Sample");
}
void MeshSample::Start()
{
	Super::Start();
	auto* cache = GModuleManager::Get().CacheModule();
	auto* graphics = GModuleManager::Get().GraphicsModule();

	graphics->RenderWindow()->SetMouseLock(true);
	graphics->RenderWindow()->SetMouseHide(true);

	scene = new Scene();
	scene->SetupShadowMap(3, 4096);
	scene->CreateChild<Octree>();
	camera = scene->CreateChild<ACamera>();
	camera->SetPosition(Vector3F(0.0f, 5.0f, -15.0f));
	camera->SetAmbientColor(Color(0.1f, 0.1f, 0.1f));


	AStaticModel* plane = scene->CreateChild<AStaticModel>();
	plane->SetScale(Vector3F(50.0f, 0.1f, 50.0f));
	plane->SetCastShadows(true);
	plane->SetModel(cache->LoadResource<AModel>("Model/Box.mdl"));
	plane->SetMaterial(cache->LoadResource<AMaterial>("Stone.json"));

	AStaticModel* teaPot = scene->CreateChild<AStaticModel>();
	teaPot->SetPosition(Vector3F(0.0f, 0.0f, 0.0f));
	teaPot->SetScale(10.0f);
	teaPot->SetModel(cache->LoadResource<AModel>("Model/TeaPot.mdl"));
	teaPot->SetCastShadows(true);

	//Light directional point and spot
	{
		ALight* lightDir = scene->CreateChild<ALight>();
		lightDir->SetLightType(ELightType::DIRECTIONAL);
		lightDir->SetCastShadows(true);
		lightDir->SetColor(Color(1.0f, 0.0f, 0.0f));
		lightDir->SetDirection(Vector3F(0.0f, -1.0f, 0.5f));
		lightDir->SetShadowMapSize(2048);

		ALight* lightPoint = scene->CreateChild<ALight>();
		lightPoint->SetLightType(ELightType::POINT);
		lightPoint->SetCastShadows(true);
		lightPoint->SetColor(Color(0.0f, 1.0f, 0.0f));
		lightPoint->SetRange(20.0f);
		lightPoint->SetPosition(Vector3F(-10.0f, 10.0f, 0.0f));
		lightPoint->SetShadowMapSize(2048);

		ALight* lightSpot = scene->CreateChild<ALight>();
		lightSpot->SetLightType(ELightType::SPOT);
		lightSpot->SetCastShadows(true);
		lightSpot->SetColor(Color(1.0f, 1.0f, 1.0f));
		lightSpot->SetPosition(Vector3F(10.0f, 10.0f, 0.0f));
		lightSpot->SetDirection(Vector3F(-0.5f, -1.0f, 0.0f));
		lightSpot->SetFov(90.0f);
		lightSpot->SetRange(20.0f);
		lightSpot->SetShadowMapSize(2048);
	}
	

}
void MeshSample::Update()
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

	camera->SetRotation(Quaternion(pitch, yaw, 0.0f));
	if (input->IsKeyDown(KEY_W))
		camera->Translate(Vector3F::FORWARD * time->GetDeltaTime() * moveSpeed);
	if (input->IsKeyDown(KEY_S))
		camera->Translate(Vector3F::BACK * time->GetDeltaTime() * moveSpeed);
	if (input->IsKeyDown(KEY_A))
		camera->Translate(Vector3F::LEFT * time->GetDeltaTime() * moveSpeed);
	if (input->IsKeyDown(KEY_D))
		camera->Translate(Vector3F::RIGHT * time->GetDeltaTime() * moveSpeed);
}

void MeshSample::Stop()
{
	Super::Stop();
}

AUTO_APPLICATION_MAIN(MeshSample)