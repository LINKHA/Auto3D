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

	for (int i = 0; i < 3; i++)
	{
		Sprite* logo = canvas->CreateChild<Sprite>();
		logo->SetTexture(cache->LoadResource<Texture>("Star.png"));
		float s = RandomSignedFloat();
		logo->SetPosition(Vector3F(RandomSignedFloat()*10.0f, RandomSignedFloat()*10.0f, -0.1f));
		logo->SetScale(Vector3F(1.0f, 1.0f, 1.0f));
	}

}
void SpriteSample::Update()
{
	Super::Update();
	auto* input = Object::Subsystem<Input>();
	auto* graphics = Object::Subsystem<Graphics>();
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
}

void SpriteSample::Stop()
{
	Super::Stop();
}

AUTO_APPLICATION_MAIN(SpriteSample)