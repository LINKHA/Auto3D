#include "SpriteSample.h"

void SpriteSample::Init()
{

}
void SpriteSample::Start()
{
	auto* cache = Object::Subsystem<ResourceCache>();
	auto texture = (cache->LoadResource<Texture>("HelloWorld.png"));
	SubscribeToEvent(Subsystem<Graphics>()->RenderWindow()->closeRequestEvent, &SpriteSample::HandleCloseRequest);
	scene = new Scene();
	camera = scene->CreateChild<Camera>();
	Subsystem<RegisteredBox>()->RegisterScene(scene, camera);

	canvas = new Canvas();
	uiCamera = canvas->CreateChild<UICamera>();
	uiCamera->SetOrthographic(true);
	uiCamera->SetPosition(Vector3F(0.0f, 0.0f, -1.0f));
	Subsystem<RegisteredBox>()->RegisterCanvas(canvas, uiCamera);

	for (int i = 0; i < 1; i++)
	{
		Sprite* sprite = canvas->CreateChild<Sprite>();
		sprite->SetTexture(texture);
		sprite->SetScale(Vector3F(10.0f, 10.0f, 4.0f));
	}
	
	Sprite* bakcground = canvas->CreateChild<Sprite>();
	bakcground->SetTexture(cache->LoadResource<Texture>("HelloWorld.png"));
	bakcground->SetScale(Vector3F(20.0f, 15.0f, 1.0f));

	Sprite* logo = canvas->CreateChild<Sprite>();
	logo->SetTexture(cache->LoadResource<Texture>("Newlogo.png"));
	logo->SetPosition(Vector3F(0.0f, 0.0f, -0.1f));
	logo->SetScale(Vector3F(3.0f, 3.0f, 1.0f));


	CreateLogo();
}
void SpriteSample::Update()
{
	auto* input = Object::Subsystem<Input>();
	auto* graphics = Object::Subsystem<Graphics>();
	auto* renderer = Object::Subsystem<Renderer>();
	auto* time = Object::Subsystem<Time>();

	//pitch += input->GetMouseMove()._y * 0.25f;
	//yaw += input->GetMouseMove()._x * 0.25f;
	//pitch = Clamp(pitch, -90.0f, 90.0f);

	float moveSpeed = input->IsKeyDown(KEY_LSHIFT) ? 50 : 10.0f;

	uiCamera->SetRotation(Quaternion(pitch, yaw, 0.0f));
	if (input->IsKeyDown(KEY_W))
		uiCamera->Translate(Vector3F::UP * time->GetDeltaTime() * moveSpeed);
	if (input->IsKeyDown(KEY_S))
		uiCamera->Translate(Vector3F::DOWN * time->GetDeltaTime()  * moveSpeed);
	if (input->IsKeyDown(KEY_A))
		uiCamera->Translate(Vector3F::LEFT * time->GetDeltaTime()  * moveSpeed);
	if (input->IsKeyDown(KEY_D))
		uiCamera->Translate(Vector3F::RIGHT * time->GetDeltaTime()  * moveSpeed);

}

void SpriteSample::Stop()
{
}

void SpriteSample::CreateLogo()
{
	auto* cache = Object::Subsystem<ResourceCache>();
	Sprite* logoLong = canvas->CreateChild<Sprite>();
	logoLong->SetTexture(cache->LoadResource<Texture>("logoLong.png"));
	logoLong->SetScale(Vector3F(3.0f, 0.8f, 1.0f));
	logoLong->SetPosition(Vector3F(7.0f, -9.2f, -0.1f));
}

AUTO_APPLICATION_MAIN(SpriteSample)