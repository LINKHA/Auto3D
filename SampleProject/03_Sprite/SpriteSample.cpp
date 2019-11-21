#include "SpriteSample.h"

void SpriteSample::Init()
{
	Super::Init();
	auto* graphics = ModuleManager::Get().GraphicsModule();
	graphics->RenderWindow()->SetTitle("Sprite Sample");
}
void SpriteSample::Start()
{
	Super::Start();

	camera2d = scene2d->CreateChild<Camera2D>();
	camera2d->SetOrthographic(true);
	camera2d->SetPosition(Vector3F(0.0f, 0.0f, -100.0f));
	camera2d->SetLayoutMaskName("UI");

	auto* cache = ModuleManager::Get().CacheModule();
	auto flowerTexture = cache->LoadResource<Texture>("Texture/flower.png");

	for (int i = 0; i < flowerNum; i++)
	{
		Sprite2D* flower = scene2d->CreateChild<Sprite2D>();
		flower->SetTexture(flowerTexture);
		flower->SetPosition(Vector3F(RandomSignedFloat()*10.0f, RandomSignedFloat()*10.0f, -0.1f));
		flower->SetScale(Vector3F(1.0f, 1.0f, 1.0f));
		sprites.Push(Pair<FlowerMSG, Sprite2D*>(FlowerMSG(Random() * 5, RandomSignedFloat()), flower));
	}
}
void SpriteSample::Update()
{
	Super::Update();
	auto input = ModuleManager::Get().InputModule();
	auto renderer = ModuleManager::Get().RendererModule();
	auto time = ModuleManager::Get().TimeModule();

	float moveSpeed = input->IsKeyDown(KEY_LSHIFT) ? 50 : 10.0f;

	if (input->IsKeyDown(KEY_W))
		camera2d->Translate(Vector3F::UP * time->GetDeltaTime() * moveSpeed);
	if (input->IsKeyDown(KEY_S))
		camera2d->Translate(Vector3F::DOWN * time->GetDeltaTime()  * moveSpeed);
	if (input->IsKeyDown(KEY_A))
		camera2d->Translate(Vector3F::LEFT * time->GetDeltaTime()  * moveSpeed);
	if (input->IsKeyDown(KEY_D))
		camera2d->Translate(Vector3F::RIGHT * time->GetDeltaTime()  * moveSpeed);

	float scaleAmount = (float)sin(time->GetCurTime());
	for (auto it = sprites.Begin(); it != sprites.End(); it++)
	{
		float speed = it->_first.speed;
		float rotateOffset = it->_first.rotateOffset;
		Sprite2D* speite = it->_second;
		speite->Translate(Vector3F::DOWN * time->GetDeltaTime() * speed, Transform2DSpace::WORLD);
		speite->Rotate(Quaternion(0.0f, 0.0f, rotateOffset));
		Vector3F oldPos = speite->GetPosition();
		if (oldPos._y <= -11.0f)
			speite->SetPosition(Vector3F(oldPos._x, 11.0f, oldPos._z));
	}
}

void SpriteSample::Stop()
{
	Super::Stop();
}

AUTO_APPLICATION_MAIN(SpriteSample)