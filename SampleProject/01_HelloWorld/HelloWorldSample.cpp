#include "HelloWorldSample.h"

void HelloWorldSample::Init()
{
	Super::Init();
}
void HelloWorldSample::Start()
{
	Super::Start();
	auto* cache = Object::Subsystem<ResourceCache>();
	
	SubscribeToEvent(Object::Subsystem<Graphics>()->RenderWindow()->closeRequestEvent, &HelloWorldSample::HandleCloseRequest);

	Sprite* bakcground = canvas->CreateChild<Sprite>();
	bakcground->SetTexture(cache->LoadResource<Texture>("HelloWorld.png"));
	bakcground->SetScale(Vector3F(20.0f, 15.0f, 1.0f));

	Sprite* logo = canvas->CreateChild<Sprite>();
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