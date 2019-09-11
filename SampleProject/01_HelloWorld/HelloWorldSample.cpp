#include "HelloWorldSample.h"

void HelloWorldSample::Init()
{
	Super::Init();
}
void HelloWorldSample::Start()
{
	Super::Start();
	auto* cache = Object::Module<ResourceCache>();

	Sprite2D* bakcground = scene2d->CreateChild<Sprite2D>();
	bakcground->SetTexture(cache->LoadResource<Texture>("HelloWorld.png"));
	bakcground->SetScale(Vector3F(20.0f, 15.0f, 1.0f));

	Sprite2D* logo = scene2d->CreateChild<Sprite2D>();
	logo->SetTexture(cache->LoadResource<Texture>("Newlogo.png"));
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