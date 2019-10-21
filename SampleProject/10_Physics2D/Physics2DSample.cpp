#include "Physics2DSample.h"

b2Body* body_;

void Physics2DSample::Init()
{
	Super::Init();
	auto* graphics = Object::Module<Graphics>();
	graphics->RenderWindow()->SetTitle("Physics2D Sample");
}

void Physics2DSample::Start()
{
	Super::Start();
	uiCamera->SetOrthoSize(50);

	auto* cache = Object::Module<ResourceCache>();
	auto squareTexture = cache->LoadResource<Texture>("Texture/Square.png");
	
	PhysicsWorld2D* physicsWorld2d = scene2d->CreateChild<PhysicsWorld2D>();

	//Sprite2D* plane = scene2d->CreateChild<Sprite2D>();
	//plane->SetTexture(squareTexture);
	//plane->SetPosition(Vector2F(0.0f, -25.0f));
	//plane->SetScale(Vector3F(10.0f, 1.0f));
	//RigidBody2D* planeRigidBody = plane->CreateChild<RigidBody2D>();
	////planeRigidBody->SetBodyType(BodyType2D::STATIC);
	//ColliderBox2D* planeColliderBox = plane->CreateChild<ColliderBox2D>();
	//planeColliderBox->SetSize(Vector2F(0.32f,0.32f));
	//planeColliderBox->SetFriction(0.5f);


	//for (int i = 0; i < 1; ++i)
	//{
	//	Sprite2D* square = scene2d->CreateChild<Sprite2D>();
	//	square->SetTexture(squareTexture);
	//	square->SetPosition(Vector2F(0.0f+0.1*i, 25.0f + i));
	//	square->SetRotation2D(40.0f);
	//	square->SetScale(Vector3F(1.0f, 1.0f));
	//	RigidBody2D* squareRigidBody = square->CreateChild<RigidBody2D>();
	//	squareRigidBody->SetBodyType(BodyType2D::DYNAMIC);
	//	ColliderBox2D* squareColliderBox = square->CreateChild<ColliderBox2D>();
	//	squareColliderBox->SetSize(Vector2F(1.0f, 1.0f));
	//	squareColliderBox->SetDensity(1.0f);
	//	squareColliderBox->SetFriction(0.5f);
	//	squareColliderBox->SetRestitution(0.1f);

	//	body_ = squareRigidBody->GetBody();
	//}



	{
		float32 a = 0.5f;
		b2BodyDef bd;
		bd.position.y = -a;
		b2Body* ground = physicsWorld2d->GetWorld()->CreateBody(&bd);

		int32 N = 200;
		int32 M = 10;
		b2Vec2 position;
		position.y = 0.0f;
		for (int32 j = 0; j < M; ++j)
		{
			position.x = -N * a;
			for (int32 i = 0; i < N; ++i)
			{
				b2PolygonShape shape;
				shape.SetAsBox(a, a, position, 0.0f);
				ground->CreateFixture(&shape, 0.0f);
				position.x += 2.0f * a;
			}
			position.y -= 2.0f * a;
		}
	}

	{
		b2PolygonShape shape;
		shape.SetAsBox(0.5f, 0.5f);

		b2BodyDef bd;
		bd.type = b2_dynamicBody;
		bd.position = b2Vec2(7.0f, 1.75f);
		bd.angle = 0.698131621;

		b2Body* body = physicsWorld2d->GetWorld()->CreateBody(&bd);
		body->CreateFixture(&shape, 5.0f);

		body_ = body;
	}



	

}
void Physics2DSample::Update()
{
	Super::Update();

	b2Vec2 position = body_->GetPosition();
	float angle = body_->GetAngle();

	auto input = ModuleManager::Get().InputModule();
	auto renderer = ModuleManager::Get().RendererModule();
	auto time = ModuleManager::Get().TimeModule();

	float moveSpeed = input->IsKeyDown(KEY_LSHIFT) ? 50 : 10.0f;

	if (input->IsKeyDown(KEY_W))
		uiCamera->Translate(Vector3F::UP * time->GetDeltaTime() * moveSpeed);
	if (input->IsKeyDown(KEY_S))
		uiCamera->Translate(Vector3F::DOWN * time->GetDeltaTime() * moveSpeed);
	if (input->IsKeyDown(KEY_A))
		uiCamera->Translate(Vector3F::LEFT * time->GetDeltaTime() * moveSpeed);
	if (input->IsKeyDown(KEY_D))
		uiCamera->Translate(Vector3F::RIGHT * time->GetDeltaTime() * moveSpeed);
}

void Physics2DSample::Stop()
{
	Super::Stop();
}

AUTO_APPLICATION_MAIN(Physics2DSample)