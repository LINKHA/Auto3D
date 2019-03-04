#include "MeshSample.h"

void MeshSample::Init()
{

}
void MeshSample::Start()
{

	auto* cache = Object::Subsystem<ResourceCache>();
	auto* graphics = Object::Subsystem<Graphics>();
	auto* renderer = Object::Subsystem<Renderer>();
	auto* input = Object::Subsystem<Input>();
	auto* profiler = Object::Subsystem<Profiler>();


	SubscribeToEvent(graphics->RenderWindow()->closeRequestEvent, &MeshSample::HandleCloseRequest);

	scene = new Scene();
	scene->CreateChild<Octree>();
	camera = scene->CreateChild<Camera>();
	camera->SetPosition(Vector3F(0.0f, 5.0f, -15.0f));
	camera->SetAmbientColor(Color(0.1f, 0.1f, 0.1f));
	// Register scene to scene system use to render
	Object::Subsystem<RegisteredBox>()->RegisterScene(scene, camera);

	for (int i = 0; i < 2; i++)
	{
		StaticModel* plane = scene->CreateChild<StaticModel>();
		plane->SetScale(Vector3F(10.0f, 0.1f, 10.0f));
		plane->SetModel(cache->LoadResource<Model>("Box.mdl"));
		plane->SetMaterial(cache->LoadResource<Material>("Stone.json"));
	}
	StaticModel* mushroom = scene->CreateChild<StaticModel>();
	mushroom->SetPosition(Vector3F(0.0f, 1.0f, 0.0f));
	mushroom->SetScale(1.5f);
	mushroom->SetModel(cache->LoadResource<Model>("Mushroom.mdl"));
	mushroom->SetMaterial(cache->LoadResource<Material>("Mushroom.json"));
	mushroom->SetCastShadows(true);
	mushroom->SetLodBias(2.0f);

	Light* light = scene->CreateChild<Light>();
	light->SetLightType(LightType::POINT);
	light->SetCastShadows(true);
	light->SetColor(Color(1.0f, 1.0f, 1.0f));
	light->SetFov(90.0f);
	light->SetRange(20.0f);
	light->SetPosition(Vector3F(0.0f, 7.0f, 0.0f));
	light->SetDirection(Vector3F(0.0f, -1.0f, 0.0f));
	light->SetShadowMapSize(256);

	canvas = new Canvas();
	uiCamera = canvas->CreateChild<UICamera>();
	uiCamera->SetOrthographic(true);
	uiCamera->SetPosition(Vector3F(0.0f, 0.0f, -100.0f));
	Subsystem<RegisteredBox>()->RegisterCanvas(canvas, uiCamera);

	CreateLogo();
}
void MeshSample::Update()
{
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
}

void MeshSample::CreateLogo()
{
	auto* cache = Object::Subsystem<ResourceCache>();
	Sprite* logoLong = canvas->CreateChild<Sprite>();
	logoLong->SetTexture(cache->LoadResource<Texture>("LogoLong.png"));
	logoLong->SetScale(Vector3F(3.0f, 0.8f, 1.0f));
	logoLong->SetPosition(Vector3F(7.0f, -9.2f, -0.1f));
}
AUTO_APPLICATION_MAIN(MeshSample)