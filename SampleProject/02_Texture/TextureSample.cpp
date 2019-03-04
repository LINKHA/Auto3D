#include "TextureSample.h"

void TextureSample::Init()
{

}
void TextureSample::Start()
{
	auto* cache = Object::Subsystem<ResourceCache>();
	auto texture = (cache->LoadResource<Texture>("HelloWorld.png"));
	SubscribeToEvent(Subsystem<Graphics>()->RenderWindow()->closeRequestEvent, &TextureSample::HandleCloseRequest);

	canvas = new Canvas();
	uiCamera = canvas->CreateChild<UICamera>();
	uiCamera->SetOrthographic(true);
	uiCamera->SetPosition(Vector3F(0.0f, 0.0f, -1.0f));
	Subsystem<RegisteredBox>()->RegisterCanvas(canvas, uiCamera);

	for (int i = 0; i < 1; i++)
	{
		Sprite* sprite = canvas->CreateChild<Sprite>();
		sprite->SetTexture(texture);
		sprite->SetScale(Vector3F(0.0f, 0.0f, -1.0f));
	}
}
void TextureSample::Update()
{
	auto* input = Object::Subsystem<Input>();
	auto* graphics = Object::Subsystem<Graphics>();
	auto* renderer = Object::Subsystem<Renderer>();
	auto* time = Object::Subsystem<Time>();

	pitch += input->GetMouseMove()._y * 0.25f;
	yaw += input->GetMouseMove()._x * 0.25f;
	pitch = Clamp(pitch, -90.0f, 90.0f);

	float moveSpeed = input->IsKeyDown(KEY_LSHIFT) ? 50 : 10.0f;

	uiCamera->SetRotation(Quaternion(pitch, yaw, 0.0f));
	if (input->IsKeyDown(KEY_W))
		uiCamera->Translate(Vector3F::FORWARD * time->GetDeltaTime() * moveSpeed);
	if (input->IsKeyDown(KEY_S))
		uiCamera->Translate(Vector3F::BACK * time->GetDeltaTime()  * moveSpeed);
	if (input->IsKeyDown(KEY_A))
		uiCamera->Translate(Vector3F::LEFT * time->GetDeltaTime()  * moveSpeed);
	if (input->IsKeyDown(KEY_D))
		uiCamera->Translate(Vector3F::RIGHT * time->GetDeltaTime()  * moveSpeed);

	// Update uiCamera aspect ratio based on window size
	uiCamera->SetAspectRatio((float)graphics->GetWidth() / (float)graphics->GetHeight());
}

void TextureSample::Stop()
{
}

AUTO_APPLICATION_MAIN(TextureSample)