#include "SpriteSample.h"

void SpriteSample::Init()
{
	Super::Init();
	auto* graphics = GModuleManager::Get().GraphicsModule();
	graphics->RenderWindow()->SetTitle("Sprite Sample");
}
void SpriteSample::Start()
{
	Super::Start();

	camera2d = scene2d->CreateChild<ACamera2D>();
	camera2d->SetOrthographic(true);
	camera2d->SetPosition(TVector3F(0.0f, 0.0f, -100.0f));
	camera2d->SetLayoutMaskName("UI");

	auto* cache = GModuleManager::Get().CacheModule();
	auto flowerTexture = cache->LoadResource<ATexture>("Texture/flower.png");

	for (int i = 0; i < flowerNum; i++)
	{
		ASprite2D* flower = scene2d->CreateChild<ASprite2D>();
		flower->SetTexture(flowerTexture);
		flower->SetPosition(TVector3F(RandomSignedFloat()*10.0f, RandomSignedFloat()*10.0f, -0.1f));
		flower->SetScale(TVector3F(1.0f, 1.0f, 1.0f));
		sprites.Push(TPair<FlowerMSG, ASprite2D*>(FlowerMSG(Random() * 5, RandomSignedFloat()), flower));
	}
}
void SpriteSample::Update()
{
	Super::Update();
	auto input = GModuleManager::Get().InputModule();
	auto renderer = GModuleManager::Get().RendererModule();
	auto time = GModuleManager::Get().TimeModule();

	float moveSpeed = input->IsKeyDown(KEY_LSHIFT) ? 50 : 10.0f;

	if (input->IsKeyDown(KEY_W))
		camera2d->Translate(TVector3F::UP * time->GetDeltaTime() * moveSpeed);
	if (input->IsKeyDown(KEY_S))
		camera2d->Translate(TVector3F::DOWN * time->GetDeltaTime()  * moveSpeed);
	if (input->IsKeyDown(KEY_A))
		camera2d->Translate(TVector3F::LEFT * time->GetDeltaTime()  * moveSpeed);
	if (input->IsKeyDown(KEY_D))
		camera2d->Translate(TVector3F::RIGHT * time->GetDeltaTime()  * moveSpeed);

	float scaleAmount = (float)sin(time->GetCurTime());
	for (auto it = sprites.Begin(); it != sprites.End(); it++)
	{
		float speed = it->_first.speed;
		float rotateOffset = it->_first.rotateOffset;
		ASprite2D* speite = it->_second;
		speite->Translate(TVector3F::DOWN * time->GetDeltaTime() * speed, ETransform2DSpace::WORLD);
		speite->Rotate(FQuaternion(0.0f, 0.0f, rotateOffset));
		TVector3F oldPos = speite->GetPosition();
		if (oldPos._y <= -11.0f)
			speite->SetPosition(TVector3F(oldPos._x, 11.0f, oldPos._z));
	}
}

void SpriteSample::Stop()
{
	Super::Stop();
}

AUTO_APPLICATION_MAIN(SpriteSample)