#include "PhysicsSample.h"

void PhysicsSample::Init()
{
	Super::Init();
	auto* graphics = GModuleManager::Get().GraphicsModule();
	graphics->RenderWindow()->SetTitle("Physics Sample");
}

void PhysicsSample::Start()
{
	Super::Start();
	auto* cache = GModuleManager::Get().CacheModule();
	auto* graphics = GModuleManager::Get().GraphicsModule();

	graphics->RenderWindow()->SetMouseLock(true);
	graphics->RenderWindow()->SetMouseHide(true);

	scene = AObject::Create<AScene>();
	scene->SetupShadowMap(1, 4096);
	scene->CreateChild<AOctree>();
	scene->CreateChild<APhysicsWorld>();
	camera = scene->CreateChild<ACamera>();
	camera->SetPosition(TVector3F(0.0f, 10.0f, -60.0f));

	//Plane
	{
		AStaticModel* plane = scene->CreateChild<AStaticModel>();
		plane->SetModel(cache->LoadResource<AModel>("Model/Box.mdl"));
		plane->SetPosition(TVector3F(0.0f, -20.0f, 0.0f));
		plane->SetScale(TVector3F(100.0f, 1.0f, 100.0f));
		plane->SetMaterial(cache->LoadResource<AMaterial>("Stone.json"));
		ARigidBody* planeRigidBody = plane->CreateChild<ARigidBody>();
		planeRigidBody->SetMass(0.0f);
		AColliderBox* planeColliderBox = plane->CreateChild<AColliderBox>();
		planeColliderBox->SetSize(TVector3F(50.0f, 0.5f, 50.0f));
	}

	//The fence
	{
		//Right
		{
			AStaticModel* fence = scene->CreateChild<AStaticModel>();
			fence->SetModel(cache->LoadResource<AModel>("Model/Box.mdl"));
			fence->SetPosition(TVector3F(50.0f, -20.0f, 0.0f));
			fence->SetScale(TVector3F(1.0f, 10.0f, 100.0f));
			fence->SetMaterial(cache->LoadResource<AMaterial>("Stone.json"));
			ARigidBody* fenceRigidBody = fence->CreateChild<ARigidBody>();
			fenceRigidBody->SetMass(0.0f);
			AColliderBox* fenceColliderBox = fence->CreateChild<AColliderBox>();
			fenceColliderBox->SetSize(TVector3F(0.5f, 5.0f, 50.0f));
		}
		//Left
		{
			AStaticModel* fence = scene->CreateChild<AStaticModel>();
			fence->SetModel(cache->LoadResource<AModel>("Model/Box.mdl"));
			fence->SetPosition(TVector3F(-50.0f, -20.0f, 0.0f));
			fence->SetScale(TVector3F(1.0f, 10.0f, 100.0f));
			fence->SetMaterial(cache->LoadResource<AMaterial>("Stone.json"));
			ARigidBody* fenceRigidBody = fence->CreateChild<ARigidBody>();
			fenceRigidBody->SetMass(0.0f);
			AColliderBox* fenceColliderBox = fence->CreateChild<AColliderBox>();
			fenceColliderBox->SetSize(TVector3F(0.5f, 5.0f, 50.0f));
		}
		//Back
		{
			AStaticModel* fence = scene->CreateChild<AStaticModel>();
			fence->SetModel(cache->LoadResource<AModel>("Model/Box.mdl"));
			fence->SetPosition(TVector3F(0.0f, -20.0f, 50.0f));
			fence->SetScale(TVector3F(100.0f, 10.0f, 1.0f));
			fence->SetMaterial(cache->LoadResource<AMaterial>("Stone.json"));
			ARigidBody* fenceRigidBody = fence->CreateChild<ARigidBody>();
			fenceRigidBody->SetMass(0.0f);
			AColliderBox* fenceColliderBox = fence->CreateChild<AColliderBox>();
			fenceColliderBox->SetSize(TVector3F(50.0f, 5.0f, 0.5f));
		}

		//Front
		{
			AStaticModel* fence = scene->CreateChild<AStaticModel>();
			fence->SetModel(cache->LoadResource<AModel>("Model/Box.mdl"));
			fence->SetPosition(TVector3F(0.0f, -20.0f, -50.0f));
			fence->SetScale(TVector3F(100.0f, 10.0f, 1.0f));
			fence->SetMaterial(cache->LoadResource<AMaterial>("Stone.json"));
			ARigidBody* fenceRigidBody = fence->CreateChild<ARigidBody>();
			fenceRigidBody->SetMass(0.0f);
			AColliderBox* fenceColliderBox = fence->CreateChild<AColliderBox>();
			fenceColliderBox->SetSize(TVector3F(50.0f, 5.0f, 0.5f));
		}
	}

	for (int i = 0; i < 5; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			for (int k = 0; k < 5; ++k)
			{
				AStaticModel* box = scene->CreateChild<AStaticModel>();
				box->SetModel(cache->LoadResource<AModel>("Model/Sphere.mdl"));
				//box->SetMaterial(cache->LoadResource<Material>("Stone.json"));
				box->SetPosition(TVector3F(i, 10.f + j * 2, k));
				box->SetScale(TVector3F(2.0f, 2.0f, 2.0f));
				box->SetCastShadows(true);
				ARigidBody* boxRigidBody = box->CreateChild<ARigidBody>();
				boxRigidBody->SetMass(1.0f);
				AColliderSphere* boxcolliderBox = box->CreateChild<AColliderSphere>();
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
				AStaticModel* box = scene->CreateChild<AStaticModel>();
				box->SetModel(cache->LoadResource<AModel>("Model/Box.mdl"));
				//box->SetMaterial(cache->LoadResource<Material>("Stone.json"));
				box->SetPosition(TVector3F(i, 11.f + j * 2, k));
				box->SetScale(TVector3F(2.0f, 2.0f, 2.0f));
				box->SetCastShadows(true);
				ARigidBody* boxRigidBody = box->CreateChild<ARigidBody>();
				boxRigidBody->SetMass(1.0f);
				AColliderBox* boxcolliderBox = box->CreateChild<AColliderBox>();
				boxcolliderBox->SetSize(TVector3F(1.0f, 1.0f, 1.0f));
			}
		}
	}


	ALight* lightDir = scene->CreateChild<ALight>();
	lightDir->SetLightType(ELightType::DIRECTIONAL);
	lightDir->SetCastShadows(true);
	lightDir->SetColor(FColor(1.0f, 1.0f, 1.0f));
	lightDir->SetDirection(TVector3F(-0.5f, -1.0f, -0.5f));
	lightDir->SetShadowMapSize(2048);
	
}
void PhysicsSample::Update()
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

void PhysicsSample::Stop()
{
	Super::Stop();
}

AUTO_APPLICATION_MAIN(PhysicsSample)