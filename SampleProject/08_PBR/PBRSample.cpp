#include "PBRSample.h"

void PBRSample::Init()
{
	Super::Init();
	auto* graphics = Object::Subsystem<Graphics>();
	graphics->RenderWindow()->SetTitle("PBR Sample");
}

void PBRSample::Start()
{
	Super::Start();
	auto* cache = Object::Subsystem<ResourceCache>();
	auto* graphics = Object::Subsystem<Graphics>();

	graphics->RenderWindow()->SetMouseLock(true);
	graphics->RenderWindow()->SetMouseHide(true);

	scene = new Scene();
	scene->CreateChild<Octree>();
	camera = scene->CreateChild<Camera>();
	camera->SetPosition(Vector3F(0.0f, 10.0f, -20.0f));
	camera->SetAmbientColor(Color(0.1f, 0.1f, 0.1f));


	StaticModel* plane = scene->CreateChild<StaticModel>();
	plane->SetScale(Vector3F(10.0f, 10.0f, 10.0f));
	plane->SetCastShadows(true);
	plane->SetModel(cache->LoadResource<Model>("Sphere.mdl"));
	plane->SetMaterial(cache->LoadResource<Material>("PBRNoTexture.json"));

	
	Light* light = scene->CreateChild<Light>();
	light->SetLightType(LightType::POINT);
	light->SetCastShadows(true);
	light->SetColor(Color(500.0f, 500.0f, 500.0f));
	//light->SetColor(Color(100.0f, 100.0f, 100.0f));
	light->SetRange(100.0f);
	light->SetPosition(Vector3F(-10.0f, 10.0f, -10.0f));

	Light* light2 = scene->CreateChild<Light>();
	light2->SetLightType(LightType::POINT);
	light2->SetCastShadows(true);
	light2->SetColor(Color(500.0f, 500.0f, 500.0f));
	//light2->SetColor(Color(5.0f, 5.0f, 5.0f));
	light2->SetRange(100.0f);
	light2->SetPosition(Vector3F(10.0f, 10.0f, -10.0f));

	Light* light3 = scene->CreateChild<Light>();
	light3->SetLightType(LightType::POINT);
	light3->SetCastShadows(true);
	light3->SetColor(Color(500.0f, 500.0f, 500.0f));
	//light3->SetColor(Color(5.0f, 5.0f, 5.0f));
	light3->SetRange(100.0f);
	light3->SetPosition(Vector3F(-10.0f, -10.0f, -10.0f));

	Light* light4 = scene->CreateChild<Light>();
	light4->SetLightType(LightType::POINT);
	light4->SetCastShadows(true);
	light4->SetColor(Color(500.0f, 500.0f, 500.0f));
	//light4->SetColor(Color(5.0f, 5.0f, 5.0f));
	light4->SetRange(100.0f);
	light4->SetPosition(Vector3F(10.0f, -10.0f, -10.0f));
	
}
void PBRSample::Update()
{
	Super::Update();
	auto* input = Object::Subsystem<Input>();
	auto* graphics = Object::Subsystem<Graphics>();
	auto* renderer = Object::Subsystem<Renderer>();
	auto* time = Object::Subsystem<Time>();

	pitch += input->GetMouseMove()._y * 0.25f;
	yaw += input->GetMouseMove()._x * 0.25f;
	pitch = Clamp(pitch, -90.0f, 90.0f);

	float moveSpeed = input->IsKeyDown(KEY_LSHIFT) ? 50 : 10.0f;

	camera->SetRotation(Quaternion(pitch, yaw, 0.0f));
	if (input->IsKeyDown(KEY_W))
		camera->Translate(Vector3F::FORWARD * time->GetDeltaTime() * moveSpeed);
	if (input->IsKeyDown(KEY_S))
		camera->Translate(Vector3F::BACK * time->GetDeltaTime()  * moveSpeed);
	if (input->IsKeyDown(KEY_A))
		camera->Translate(Vector3F::LEFT * time->GetDeltaTime()  * moveSpeed);
	if (input->IsKeyDown(KEY_D))
		camera->Translate(Vector3F::RIGHT * time->GetDeltaTime()  * moveSpeed);
}

void PBRSample::Stop()
{
	Super::Stop();
}

AUTO_APPLICATION_MAIN(PBRSample)