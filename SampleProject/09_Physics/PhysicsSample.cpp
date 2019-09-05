#include "PhysicsSample.h"

PhysicsWorld* physics;

void PhysicsSample::Init()
{
	Super::Init();
	auto* graphics = Object::Subsystem<Graphics>();
	graphics->RenderWindow()->SetTitle("Physics Sample");
}

void PhysicsSample::Start()
{
	Super::Start();
	auto* cache = Object::Subsystem<ResourceCache>();
	auto* graphics = Object::Subsystem<Graphics>();

	graphics->RenderWindow()->SetMouseLock(true);
	graphics->RenderWindow()->SetMouseHide(true);

	scene = new Scene();
	scene->CreateChild<Octree>();
	physics = scene->CreateChild<PhysicsWorld>();
	camera = scene->CreateChild<Camera>();
	camera->SetPosition(Vector3F(0.0f, 10.0f, -60.0f));

	StaticModel* plane = scene->CreateChild<StaticModel>();
	plane->SetModel(cache->LoadResource<Model>("Box.mdl"));
	plane->SetPosition(Vector3F(0.0f, -25.0f, 0.0f));
	plane->SetScale(Vector3F(50.0f, 50.0f, 50.0f));
	plane->SetMaterial(cache->LoadResource<Material>("Stone.json"));
	RigidBody* planeRigidBody = plane->CreateChild<RigidBody>();
	planeRigidBody->SetMass(0.0f);
	ColliderBox* planeColliderBox = plane->CreateChild<ColliderBox>();
	planeColliderBox->SetSize(Vector3F(50.0f, 50.0f, 50.0f));
	//colliderBox->SetRigidBody(0.0f, Vector3F(0, 0, 0));
	

	StaticModel* box1 = scene->CreateChild<StaticModel>();
	box1->SetModel(cache->LoadResource<Model>("Box.mdl"));
	box1->SetPosition(Vector3F(0.0f, 50.0f, 0.0f));
	box1->SetScale(Vector3F(1.0f, 1.0f, 1.0f));
	RigidBody* box1RigidBody = box1->CreateChild<RigidBody>();
	box1RigidBody->SetMass(1.0f);
	ColliderBox* box1colliderBox = box1->CreateChild<ColliderBox>();
	box1colliderBox->SetSize(Vector3F(1.0f, 1.0f, 1.0f));
	//box1colliderBox->SetRigidBody(1.0f, Vector3F(0, 0, 0));
	
}
void PhysicsSample::Update()
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

	physics->Update();
}

void PhysicsSample::Stop()
{
	Super::Stop();
}

AUTO_APPLICATION_MAIN(PhysicsSample)