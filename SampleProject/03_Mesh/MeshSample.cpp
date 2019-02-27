#include "MeshSample.h"

void MeshSample::Init()
{

}
void MeshSample::Start()
{

	auto* cache = Object::GetSubsystem<ResourceCache>();
	auto* graphics = Object::GetSubsystem<Graphics>();
	auto* renderer = Object::GetSubsystem<Renderer>();
	auto* input = Object::GetSubsystem<Input>();
	auto* profiler = Object::GetSubsystem<Profiler>();


	SubscribeToEvent(graphics->RenderWindow()->closeRequestEvent, &MeshSample::HandleCloseRequest);

	scene = new Scene();
	scene->CreateChild<Octree>();
	camera = scene->CreateChild<Camera>();
	camera->SetPosition(Vector3F(0.0f, 5.0f, -10.0f));
	camera->SetAmbientColor(Color(0.1f, 0.1f, 0.1f));
	// Register scene to scene system use to render
	Object::GetSubsystem<SceneSystem>()->RegisterScene(scene, camera);


		StaticModel* plane = scene->CreateChild<StaticModel>();
		plane->SetScale(Vector3F(10.0f, 0.1f, 10.0f));
		plane->SetModel(cache->LoadResource<Model>("Box.mdl"));
		plane->SetMaterial(cache->LoadResource<Material>("Stone.json"));


		StaticModel* mushroom = scene->CreateChild<StaticModel>();
		mushroom->SetPosition(Vector3F(0.0f, 1.0f,0.0f));
		mushroom->SetScale(1.5f);
		mushroom->SetModel(cache->LoadResource<Model>("Mushroom.mdl"));
		mushroom->SetMaterial(cache->LoadResource<Material>("Mushroom.json"));
		mushroom->SetCastShadows(true);
		mushroom->SetLodBias(2.0f);

		Light* light = scene->CreateChild<Light>();
		light->SetLightType(LIGHT_POINT);
		light->SetCastShadows(true);
		light->SetColor(Color(1.0f,1.0f,1.0f));
		light->SetFov(90.0f);
		light->SetRange(20.0f);
		light->SetPosition(Vector3F(0.0f, 7.0f, 0.0f));
		light->SetDirection(Vector3F(0.0f, -1.0f, 0.0f));
		light->SetShadowMapSize(256);

}
void MeshSample::Update()
{
	auto* input = Object::GetSubsystem<Input>();
	auto* graphics = Object::GetSubsystem<Graphics>();
	auto* renderer = Object::GetSubsystem<Renderer>();
	auto* time = Object::GetSubsystem<Time>();

	pitch += input->MouseMove()._y * 0.25f;
	yaw += input->MouseMove()._x * 0.25f;
	pitch = Clamp(pitch, -90.0f, 90.0f);

	float moveSpeed = input->IsKeyDown(KEY_LSHIFT) ? 200.0f : 50.0f;

	camera->SetRotation(Quaternion(pitch, yaw, 0.0f));
	if (input->IsKeyDown(KEY_W))
		camera->Translate(Vector3F::FORWARD * time->GetDeltaTime() * moveSpeed);
	if (input->IsKeyDown(KEY_S))
		camera->Translate(Vector3F::BACK * time->GetDeltaTime()  * moveSpeed);
	if (input->IsKeyDown(KEY_A))
		camera->Translate(Vector3F::LEFT * time->GetDeltaTime()  * moveSpeed);
	if (input->IsKeyDown(KEY_D))
		camera->Translate(Vector3F::RIGHT * time->GetDeltaTime()  * moveSpeed);

	// Update camera aspect ratio based on window size
	camera->SetAspectRatio((float)graphics->Width() / (float)graphics->Height());

}

void MeshSample::Stop()
{
}


AUTO_APPLICATION_MAIN(MeshSample)