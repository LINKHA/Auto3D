#include "MeshSample.h"

void MeshSample::Init()
{
	Super::Init();
	auto* graphics = Object::Subsystem<Graphics>();
	graphics->RenderWindow()->SetTitle("Mesh Sample");
}
void MeshSample::Start()
{ 
	Super::Start();
	auto* cache = Object::Subsystem<ResourceCache>();
	auto* graphics = Object::Subsystem<Graphics>();

	graphics->RenderWindow()->SetMouseLock(true);
	graphics->RenderWindow()->SetMouseHide(true);

	scene = new Scene();
	scene->CreateChild<Octree>();
	camera = scene->CreateChild<Camera>();
	camera->SetPosition(Vector3F(0.0f, 5.0f, -15.0f));
	camera->SetAmbientColor(Color(0.1f, 0.1f, 0.1f));
	

	StaticModel* plane = scene->CreateChild<StaticModel>();
	plane->SetScale(Vector3F(50.0f, 0.1f, 50.0f));
	plane->SetCastShadows(true);
	plane->SetModel(cache->LoadResource<Model>("Box.mdl"));
	plane->SetMaterial(cache->LoadResource<Material>("Stone.json"));

	StaticModel* teaPot = scene->CreateChild<StaticModel>();
	teaPot->SetPosition(Vector3F(0.0f, 0.0f, 0.0f));
	teaPot->SetScale(10.0f);
	teaPot->SetModel(cache->LoadResource<Model>("TeaPot.mdl"));
	teaPot->SetCastShadows(true);
	teaPot->SetLodBias(2.0f);

	for (int i = 0; i < 2; i++)
	{
		Light* light = scene->CreateChild<Light>();
		light->SetLightType(LightType::POINT);
		light->SetCastShadows(true);
		light->SetColor(Color(1.0f, 1.0f, 1.0f));
		light->SetFov(90.0f);
		light->SetRange(20.0f);
		light->SetPosition(Vector3F(i * 5.0f - 10.0f, 10.0f, 5.0f));
		light->SetDirection(Vector3F(0.0f, -1.0f, 0.0f));
		light->SetShadowMapSize(256);
	}
}
void MeshSample::Update()
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

void MeshSample::Stop()
{
	Super::Stop();
}

AUTO_APPLICATION_MAIN(MeshSample)
