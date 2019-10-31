#include "PhysicsSample.h"

void PhysicsSample::Init()
{
	Super::Init();
	auto* graphics = Object::Module<Graphics>();
	graphics->RenderWindow()->SetTitle("Physics Sample");
}

void PhysicsSample::Start()
{
	Super::Start();
	auto* cache = Object::Module<ResourceCache>();
	auto* graphics = Object::Module<Graphics>();

	graphics->RenderWindow()->SetMouseLock(true);
	graphics->RenderWindow()->SetMouseHide(true);

	scene = new Scene();
	scene->SetupShadowMap(1, 4096);
	scene->CreateChild<Octree>();
	scene->CreateChild<PhysicsWorld>();
	camera = scene->CreateChild<Camera>();
	camera->SetPosition(Vector3F(0.0f, 10.0f, -60.0f));

	//Plane
	{
		StaticModel* plane = scene->CreateChild<StaticModel>();
		plane->SetModel(cache->LoadResource<Model>("Model/Box.mdl"));
		plane->SetPosition(Vector3F(0.0f, -20.0f, 0.0f));
		plane->SetScale(Vector3F(100.0f, 1.0f, 100.0f));
		plane->SetMaterial(cache->LoadResource<Material>("Stone.json"));
		RigidBody* planeRigidBody = plane->CreateChild<RigidBody>();
		planeRigidBody->SetMass(0.0f);
		ColliderBox* planeColliderBox = plane->CreateChild<ColliderBox>();
		planeColliderBox->SetSize(Vector3F(50.0f, 0.5f, 50.0f));
	}

	//The fence
	{
		//Right
		{
			StaticModel* fence = scene->CreateChild<StaticModel>();
			fence->SetModel(cache->LoadResource<Model>("Model/Box.mdl"));
			fence->SetPosition(Vector3F(50.0f, -20.0f, 0.0f));
			fence->SetScale(Vector3F(1.0f, 10.0f, 100.0f));
			fence->SetMaterial(cache->LoadResource<Material>("Stone.json"));
			RigidBody* fenceRigidBody = fence->CreateChild<RigidBody>();
			fenceRigidBody->SetMass(0.0f);
			ColliderBox* fenceColliderBox = fence->CreateChild<ColliderBox>();
			fenceColliderBox->SetSize(Vector3F(0.5f, 5.0f, 50.0f));
		}
		//Left
		{
			StaticModel* fence = scene->CreateChild<StaticModel>();
			fence->SetModel(cache->LoadResource<Model>("Model/Box.mdl"));
			fence->SetPosition(Vector3F(-50.0f, -20.0f, 0.0f));
			fence->SetScale(Vector3F(1.0f, 10.0f, 100.0f));
			fence->SetMaterial(cache->LoadResource<Material>("Stone.json"));
			RigidBody* fenceRigidBody = fence->CreateChild<RigidBody>();
			fenceRigidBody->SetMass(0.0f);
			ColliderBox* fenceColliderBox = fence->CreateChild<ColliderBox>();
			fenceColliderBox->SetSize(Vector3F(0.5f, 5.0f, 50.0f));
		}
		//Back
		{
			StaticModel* fence = scene->CreateChild<StaticModel>();
			fence->SetModel(cache->LoadResource<Model>("Model/Box.mdl"));
			fence->SetPosition(Vector3F(0.0f, -20.0f, 50.0f));
			fence->SetScale(Vector3F(100.0f, 10.0f, 1.0f));
			fence->SetMaterial(cache->LoadResource<Material>("Stone.json"));
			RigidBody* fenceRigidBody = fence->CreateChild<RigidBody>();
			fenceRigidBody->SetMass(0.0f);
			ColliderBox* fenceColliderBox = fence->CreateChild<ColliderBox>();
			fenceColliderBox->SetSize(Vector3F(50.0f, 5.0f, 0.5f));
		}

		//Front
		{
			StaticModel* fence = scene->CreateChild<StaticModel>();
			fence->SetModel(cache->LoadResource<Model>("Model/Box.mdl"));
			fence->SetPosition(Vector3F(0.0f, -20.0f, -50.0f));
			fence->SetScale(Vector3F(100.0f, 10.0f, 1.0f));
			fence->SetMaterial(cache->LoadResource<Material>("Stone.json"));
			RigidBody* fenceRigidBody = fence->CreateChild<RigidBody>();
			fenceRigidBody->SetMass(0.0f);
			ColliderBox* fenceColliderBox = fence->CreateChild<ColliderBox>();
			fenceColliderBox->SetSize(Vector3F(50.0f, 5.0f, 0.5f));
		}
	}

	for (int i = 0; i < 5; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			for (int k = 0; k < 5; ++k)
			{
				StaticModel* box = scene->CreateChild<StaticModel>();
				box->SetModel(cache->LoadResource<Model>("Model/Sphere.mdl"));
				//box->SetMaterial(cache->LoadResource<Material>("Stone.json"));
				box->SetPosition(Vector3F(i, 10.f + j * 2, k));
				box->SetScale(Vector3F(2.0f, 2.0f, 2.0f));
				box->SetCastShadows(true);
				RigidBody* boxRigidBody = box->CreateChild<RigidBody>();
				boxRigidBody->SetMass(1.0f);
				ColliderSphere* boxcolliderBox = box->CreateChild<ColliderSphere>();
				boxcolliderBox->SetSize(1.0f);
			}
		}
	}

	for (int i = 0; i < 5; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			for (int k = 0; k < 5; ++k)
			{
				StaticModel* box = scene->CreateChild<StaticModel>();
				box->SetModel(cache->LoadResource<Model>("Model/Box.mdl"));
				//box->SetMaterial(cache->LoadResource<Material>("Stone.json"));
				box->SetPosition(Vector3F(i, 11.f + j * 2, k));
				box->SetScale(Vector3F(2.0f, 2.0f, 2.0f));
				box->SetCastShadows(true);
				RigidBody* boxRigidBody = box->CreateChild<RigidBody>();
				boxRigidBody->SetMass(1.0f);
				ColliderBox* boxcolliderBox = box->CreateChild<ColliderBox>();
				boxcolliderBox->SetSize(Vector3F(1.0f, 1.0f, 1.0f));
			}
		}
	}


	Light* lightDir = scene->CreateChild<Light>();
	lightDir->SetLightType(LightType::DIRECTIONAL);
	lightDir->SetCastShadows(true);
	lightDir->SetColor(Color(1.0f, 1.0f, 1.0f));
	lightDir->SetDirection(Vector3F(-0.5f, -1.0f, -0.5f));
	lightDir->SetShadowMapSize(2048);
	
}
void PhysicsSample::Update()
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
		camera->Translate(Vector3F::BACK * time->GetDeltaTime()  * moveSpeed);
	if (input->IsKeyDown(KEY_A))
		camera->Translate(Vector3F::LEFT * time->GetDeltaTime()  * moveSpeed);
	if (input->IsKeyDown(KEY_D))
		camera->Translate(Vector3F::RIGHT * time->GetDeltaTime()  * moveSpeed);
}

void PhysicsSample::Stop()
{
	Super::Stop();
}

AUTO_APPLICATION_MAIN(PhysicsSample)