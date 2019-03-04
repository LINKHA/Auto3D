#include "HelloWorldSample.h"

void HelloWorldSample::Init()
{

}
void HelloWorldSample::Start()
{
	auto* cache = Object::Subsystem<ResourceCache>();
	
	SubscribeToEvent(Object::Subsystem<Graphics>()->RenderWindow()->closeRequestEvent, &HelloWorldSample::HandleCloseRequest);
	
	canvas = new Canvas();
	uiCamera = canvas->CreateChild<UICamera>();
	uiCamera->SetOrthographic(true);
	uiCamera->SetPosition(Vector3F(0.0f, 0.0f, -100.0f));
	Subsystem<RegisteredBox>()->RegisterCanvas(canvas, uiCamera);

	
	Sprite* bakcground = canvas->CreateChild<Sprite>();
	bakcground->SetTexture(cache->LoadResource<Texture>("HelloWorld.png"));
	bakcground->SetScale(Vector3F(20.0f, 15.0f, 1.0f));

	Sprite* logo = canvas->CreateChild<Sprite>();
	logo->SetTexture(cache->LoadResource<Texture>("Newlogo.png"));
	logo->SetPosition(Vector3F(0.0f, 0.0f, -0.1f));
	logo->SetScale(Vector3F(3.0f, 3.0f, 1.0f));

	CreateLogo();
}
void HelloWorldSample::Update()
{

}

void HelloWorldSample::Stop()
{
}
void HelloWorldSample::CreateLogo()
{
	auto* cache = Object::Subsystem<ResourceCache>();
	Sprite* logoLong = canvas->CreateChild<Sprite>();
	logoLong->SetTexture(cache->LoadResource<Texture>("logoLong.png"));
	logoLong->SetScale(Vector3F(3.0f, 0.8f, 1.0f));
	logoLong->SetPosition(Vector3F(7.0f, -9.2f, -0.1f));
}
AUTO_APPLICATION_MAIN(HelloWorldSample)