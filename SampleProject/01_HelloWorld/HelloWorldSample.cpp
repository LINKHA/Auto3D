#include "HelloWorldSample.h"

void HelloWorldSample::Init()
{
	Super::Init();
}
void HelloWorldSample::Start()
{
	Super::Start();
	auto* cache = Object::Module<ResourceCache>();

	Scene2D* scene2d = new Scene2D();
	Camera2D* camera = scene2d->CreateChild<Camera2D>();
	camera->SetOrthographic(true);
	camera->SetPosition(Vector3F(0.0f, 0.0f, -100.0f));
	camera->SetLayoutMaskName("UI");

	Sprite2D* bakcground = scene2d->CreateChild<Sprite2D>();
	bakcground->SetTexture(cache->LoadResource<Texture>("Texture/HelloWorld.png"));
	bakcground->SetPosition(Vector3F(0.0f, 0.0f, 10.1f));
	bakcground->SetScale(Vector3F(20.0f, 15.0f, 1.0f));

	Sprite2D* logo = scene2d->CreateChild<Sprite2D>();
	logo->SetTexture(cache->LoadResource<Texture>("Texture/Newlogo.png"));
	logo->SetPosition(Vector3F(0.0f, 0.0f, -0.1f));
	logo->SetScale(Vector3F(3.0f, 3.0f, 1.0f));
}
void HelloWorldSample::Update()
{
	Super::Update();
}

void HelloWorldSample::Stop()
{
	Super::Stop();
}
AUTO_APPLICATION_MAIN(HelloWorldSample)