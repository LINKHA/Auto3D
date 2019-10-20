#include "Physics2DSample.h"

b2Body* body;

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
	
	scene2d->CreateChild<PhysicsWorld2D>();

	Sprite2D* plane = scene2d->CreateChild<Sprite2D>();
	plane->SetTexture(squareTexture);
	plane->SetPosition(Vector2F(0.0f, -25.0f));
	plane->SetScale(Vector3F(10.0f, 1.0f));
	RigidBody2D* planeRigidBody = plane->CreateChild<RigidBody2D>();
	planeRigidBody->SetBodyType(BodyType2D::STATIC);
	ColliderBox2D* planeColliderBox = plane->CreateChild<ColliderBox2D>();
	planeColliderBox->SetSize(Vector2F(50.0f, 1.0f));
	body = planeRigidBody->GetBody();

	for (int i = 0; i < 10; ++i)
	{
		Sprite2D* square = scene2d->CreateChild<Sprite2D>();
		square->SetTexture(squareTexture);
		square->SetPosition(Vector2F(0.0f+i/2, 25.0f + i));
		square->SetScale(Vector3F(1.0f, 1.0f));
		RigidBody2D* squareRigidBody = square->CreateChild<RigidBody2D>();
		squareRigidBody->SetBodyType(BodyType2D::DYNAMIC);
		ColliderBox2D* squareColliderBox = square->CreateChild<ColliderBox2D>();
		squareColliderBox->SetSize(Vector2F(1.0f, 1.0f));
	}
	

}
void Physics2DSample::Update()
{
	Super::Update();

	//b2Vec2 position = body->GetPosition();

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