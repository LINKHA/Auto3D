#include "SpriteSample.h"

void SpriteSample::Init()
{
	Super::Init();
}
void SpriteSample::Start()
{
	Super::Start();
	auto* cache = Object::Subsystem<ResourceCache>();
	auto flower = (cache->LoadResource<Texture>("flower.png"));
	SubscribeToEvent(Subsystem<Graphics>()->RenderWindow()->closeRequestEvent, &SpriteSample::HandleCloseRequest);

	for (int i = 0; i < flowerNum; i++)
	{
		Sprite2D* flower = scene2d->CreateChild<Sprite2D>();
		flower->SetTexture(cache->LoadResource<Texture>("flower.png"));
		flower->SetPosition(Vector3F(RandomSignedFloat()*10.0f, RandomSignedFloat()*10.0f, -0.1f));
		flower->SetScale(Vector3F(1.0f, 1.0f, 1.0f));
		sprites.Push(Pair<FlowerMSG, Sprite2D*>(FlowerMSG(Random() * 5, RandomSignedFloat()), flower));
	}
}
void SpriteSample::Update()
{
	Super::Update();
	auto* input = Object::Subsystem<Input>();
	auto* renderer = Object::Subsystem<Renderer>();
	auto* time = Object::Subsystem<Time>();

	float moveSpeed = input->IsKeyDown(KEY_LSHIFT) ? 50 : 10.0f;

	if (input->IsKeyDown(KEY_W))
		uiCamera->Translate(Vector3F::UP * time->GetDeltaTime() * moveSpeed);
	if (input->IsKeyDown(KEY_S))
		uiCamera->Translate(Vector3F::DOWN * time->GetDeltaTime()  * moveSpeed);
	if (input->IsKeyDown(KEY_A))
		uiCamera->Translate(Vector3F::LEFT * time->GetDeltaTime()  * moveSpeed);
	if (input->IsKeyDown(KEY_D))
		uiCamera->Translate(Vector3F::RIGHT * time->GetDeltaTime()  * moveSpeed);

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