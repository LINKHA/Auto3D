#include "WaterSample.h"

void WaterSample::Init()
{
	//Super::Init();
	auto* graphics = Object::Module<Graphics>();
	graphics->RenderWindow()->SetTitle("Water Sample");
}

void WaterSample::Start()
{
	//Super::Start();
	auto* cache = Object::Module<ResourceCache>();
	auto* graphics = Object::Module<Graphics>();

	graphics->RenderWindow()->SetMouseLock(true);
	graphics->RenderWindow()->SetMouseHide(true);

	/*scene = new Scene();
	scene->CreateChild<Octree>();
	camera = scene->CreateChild<Camera>();
	camera->SetPosition(Vector3F(0.0f, 0.0f, 0.0f));
	camera->SetAmbientColor(Color(0.1f, 0.1f, 0.1f));

	SkyBox* skybox = scene->CreateChild<SkyBox>();
	skybox->SetMaterial(cache->LoadResource<Material>("WaterSkyBox.json"));*/

	//DynamicModel* water = scene->CreateChild<DynamicModel>();

	DynamicModel* water = new DynamicModel();
	water->AAA();

}

void WaterSample::Update()
{
	Super::Update();
	auto input = ModuleManager::Get().InputModule();
	auto graphics = ModuleManager::Get().GraphicsModule();
	auto renderer = ModuleManager::Get().RendererModule();
	auto time = ModuleManager::Get().TimeModule();

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

void WaterSample::Stop()
{
	Super::Stop();
}

AUTO_APPLICATION_MAIN(WaterSample)