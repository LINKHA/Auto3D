#include "Physics2DSample.h"
void Physics2DSample::Init()
{
	Super::Init();
	auto* graphics = GModuleManager::Get().GraphicsModule();
	graphics->RenderWindow()->SetTitle("Physics2D Sample");
}

void Physics2DSample::Start()
{
	Super::Start();
	
	auto* cache = GModuleManager::Get().CacheModule();
	auto squareTexture = cache->LoadResource<ATexture>("Texture/Box.png");
	auto circleTexture = cache->LoadResource<ATexture>("Texture/Ball.png");

	APhysicsWorld2D* physicsWorld2d = scene2d->CreateChild<APhysicsWorld2D>();

	camera2d = scene2d->CreateChild<ACamera2D>();
	camera2d->SetOrthoSize(50);
	camera2d->SetOrthographic(true);
	camera2d->SetPosition(TVector3F(0.0f, 0.0f, -100.0f));
	camera2d->SetLayoutMaskName("UI");

	{
		ASprite2D* plane = scene2d->CreateChild<ASprite2D>();
		plane->SetTexture(squareTexture);
		plane->SetPosition(TVector2F(0.0f, -24.0f));
		plane->SetScale(TVector3F(24.0f, 1.0f));
		ARigidBody2D* planeRigidBody = plane->CreateChild<ARigidBody2D>();
		planeRigidBody->SetBodyType(EBodyType2D::STATIC);
		AColliderBox2D* planeColliderBox = plane->CreateChild<AColliderBox2D>();
		planeColliderBox->SetSize(TVector2F(24.0f, 1.0f));
		planeColliderBox->SetFriction(0.5f);

		//Wall right
		ASprite2D* wallRight = scene2d->CreateChild<ASprite2D>();
		wallRight->SetTexture(squareTexture);
		wallRight->SetPosition(TVector2F(23.0f, 0.0f));
		wallRight->SetScale(TVector3F(1.0f, 25.0f));
		ARigidBody2D* wallRightRigidBody = wallRight->CreateChild<ARigidBody2D>();
		wallRightRigidBody->SetBodyType(EBodyType2D::STATIC);
		AColliderBox2D* wallRightColliderBox = wallRight->CreateChild<AColliderBox2D>();
		wallRightColliderBox->SetSize(TVector2F(1.0f, 25.0f));
		wallRightColliderBox->SetFriction(0.5f);

		//Wall left
		ASprite2D* wallLeft = scene2d->CreateChild<ASprite2D>();
		wallLeft->SetTexture(squareTexture);
		wallLeft->SetPosition(TVector2F(-24.0f,0.0f));
		wallLeft->SetScale(TVector3F(1.0f, 25.0f));
		ARigidBody2D* wallLeftRigidBody = wallLeft->CreateChild<ARigidBody2D>();
		wallLeftRigidBody->SetBodyType(EBodyType2D::STATIC);
		AColliderBox2D* wallLeftColliderBox = wallLeft->CreateChild<AColliderBox2D>();
		wallLeftColliderBox->SetSize(TVector2F(1.0f, 25.0f));
		wallLeftColliderBox->SetFriction(0.5f);
	}

	{
		//Line left
		ASprite2D* lineLeft = scene2d->CreateChild<ASprite2D>();
		lineLeft->SetTexture(squareTexture);
		lineLeft->SetPosition(TVector2F(-15.0f, 0.0f));
		lineLeft->Rotate(FQuaternion(0.0f, 0.0f, 45.0f));
		lineLeft->SetScale(TVector3F(1.0f, 13.0f));
		ARigidBody2D* lineLeftRigidBody = lineLeft->CreateChild<ARigidBody2D>();
		lineLeftRigidBody->SetBodyType(EBodyType2D::STATIC);
		AColliderBox2D* lineLeftColliderBox = lineLeft->CreateChild<AColliderBox2D>();
		lineLeftColliderBox->SetSize(TVector2F(1.0f, 15.0f));
		lineLeftColliderBox->SetFriction(0.5f);

		//Line right
		ASprite2D* lineRight = scene2d->CreateChild<ASprite2D>();
		lineRight->SetTexture(squareTexture);
		lineRight->SetPosition(TVector2F(15.0f, 0.0f));
		lineRight->Rotate(FQuaternion(0.0f, 0.0f, -45.0f));
		lineRight->SetScale(TVector3F(1.0f, 13.0f));
		ARigidBody2D* lineRightRigidBody = lineRight->CreateChild<ARigidBody2D>();
		lineRightRigidBody->SetBodyType(EBodyType2D::STATIC);
		AColliderBox2D* lineRightColliderBox = lineRight->CreateChild<AColliderBox2D>();
		lineRightColliderBox->SetSize(TVector2F(1.0f, 15.0f));
		lineRightColliderBox->SetFriction(0.5f);
	}
		

	

	int layerCount = 30;
	for (int i = 0; i < layerCount; ++i)
	{
		if (i % 2 == 0)
		{

			ASprite2D* node = scene2d->CreateChild<ASprite2D>();
			node->SetPosition(TVector2F(-10.0f, 20.0f + i * 3.0f));
			node->SetScale(TVector2F(1.0f, 1.0f));
			node->SetTexture(squareTexture);
			ARigidBody2D* nodeRigidBody = node->CreateChild<ARigidBody2D>();
			nodeRigidBody->SetBodyType(EBodyType2D::DYNAMIC);

			AColliderBox2D* squareColliderBox = node->CreateChild<AColliderBox2D>();
			squareColliderBox->SetSize(TVector2F(1.0f, 1.0f));
			squareColliderBox->SetDensity(1.0f);
			squareColliderBox->SetFriction(0.5f);
			squareColliderBox->SetRestitution(0.1f);
			
		}
		else
		{
			ASprite2D* node = scene2d->CreateChild<ASprite2D>();
			node->SetPosition(TVector2F(10.0f, 20.0f + i * 3.0f));
			node->SetScale(TVector2F(1.0f, 1.0f));
			node->SetTexture(circleTexture);
			ARigidBody2D* nodeRigidBody = node->CreateChild<ARigidBody2D>();
			nodeRigidBody->SetBodyType(EBodyType2D::DYNAMIC);


			AColliderCircle2D* squareColliderBox = node->CreateChild<AColliderCircle2D>();
			squareColliderBox->SetRadius(1.0f);
			squareColliderBox->SetDensity(1.0f);
			squareColliderBox->SetFriction(0.5f);
			squareColliderBox->SetRestitution(0.1f);
			
		}
	}
}
void Physics2DSample::Update()
{
	Super::Update();

	auto input = GModuleManager::Get().InputModule();
	auto renderer = GModuleManager::Get().RendererModule();
	auto time = GModuleManager::Get().TimeModule();

	float moveSpeed = input->IsKeyDown(KEY_LSHIFT) ? 50 : 10.0f;

	if (input->IsKeyDown(KEY_W))
		camera2d->Translate(TVector3F::UP * time->GetDeltaTime() * moveSpeed);
	if (input->IsKeyDown(KEY_S))
		camera2d->Translate(TVector3F::DOWN * time->GetDeltaTime() * moveSpeed);
	if (input->IsKeyDown(KEY_A))
		camera2d->Translate(TVector3F::LEFT * time->GetDeltaTime() * moveSpeed);
	if (input->IsKeyDown(KEY_D))
		camera2d->Translate(TVector3F::RIGHT * time->GetDeltaTime() * moveSpeed);
}

void Physics2DSample::Stop()
{
	Super::Stop();
}

AUTO_APPLICATION_MAIN(Physics2DSample)