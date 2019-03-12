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
	auto graphics = Subsystem<Graphics>();
	graphics->RenderWindow()->SetMouseLock(true);
	graphics->RenderWindow()->SetMouseHide(true);

	auto resourceCache = Subsystem<ResourceCache>();
	auto image = resourceCache->LoadResource<Image>("Mt-Washington-Gold-Room_Ref.hdr");


	//Sprite* bakcground = canvas->CreateChild<Sprite>();
	//bakcground->SetTexture(texture);

	scene = new Scene();
	scene->CreateChild<Octree>();
	camera = scene->CreateChild<Camera>();
	//camera->SetPosition(Vector3F(0.0f, 5.0f, -15.0f));
	camera->SetAmbientColor(Color(0.1f, 0.1f, 0.1f));
	camera->SetFov(120.0f);
	
	// Register scene to scene system use to render
	Object::Subsystem<RegisteredBox>()->RegisterScene(scene, camera);

	StaticModel* plane = scene->CreateChild<StaticModel>();
	plane->SetScale(Vector3F(50.0f, 0.1f, 50.0f));
	plane->SetModel(resourceCache->LoadResource<Model>("Box.mdl"));
	plane->SetMaterial(resourceCache->LoadResource<Material>("Stone.json"));


	SkyBox* skybox = camera->CreateSkyBox(image);

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

	float moveSpeed = input->IsKeyDown(KEY_LSHIFT) ? 10.0f : 5.0f;

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