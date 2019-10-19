#include "Physics2DSample.h"

PhysicsWorld* physics;

void Physics2DSample::Init()
{
	Super::Init();
	auto* graphics = Object::Module<Graphics>();
	graphics->RenderWindow()->SetTitle("Physics2D Sample");
}

void Physics2DSample::Start()
{
	Super::Start();
	auto* cache = Object::Module<ResourceCache>();
	auto squareTexture = cache->LoadResource<Texture>("Texture/Square.png");
	
	scene2d->CreateChild<PhysicsWorld2D>();

	Sprite2D* ground = scene2d->CreateChild<Sprite2D>();
	ground->SetTexture(squareTexture);
	ground->SetPosition(Vector2F(0.0f, -50.0f));
	ground->SetScale(Vector3F(1.0f, 50.0f));
	ground->CreateChild()


	Sprite2D* square = scene2d->CreateChild<Sprite2D>();
	square->SetTexture(squareTexture);
	square->SetPosition(Vector2F(0.0f, 0.0f));
	square->SetScale(Vector3F(1.0f, 1.0f));
}
void Physics2DSample::Update()
{
	Super::Update();
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