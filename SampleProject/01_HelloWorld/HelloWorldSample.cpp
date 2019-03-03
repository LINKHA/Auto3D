#include "HelloWorldSample.h"

void HelloWorldSample::Init()
{

}
void HelloWorldSample::Start()
{
	auto* cache = Object::Subsystem<ResourceCache>();
	auto* graphics = Object::Subsystem<Graphics>();
	SubscribeToEvent(graphics->RenderWindow()->closeRequestEvent, &HelloWorldSample::HandleCloseRequest);
	
	canvas = new Canvas();
	uiCamera = canvas->CreateChild<UICamera>();
	uiCamera->SetPosition(Vector3F(0.0f, 0.0f, 0.0f));
	Subsystem<RegisteredBox>()->RegisterCanvas(canvas, uiCamera);

	scene = new Scene();
	scene->CreateChild<Octree>();
	camera = scene->CreateChild<Camera>();
	Object::Subsystem<RegisteredBox>()->RegisterScene(scene, camera);

	Sprite* bakcground = canvas->CreateChild<Sprite>();
	bakcground->SetTexture(cache->LoadResource<Texture>("HelloWorld.png"));

}
void HelloWorldSample::Update()
{
}

void HelloWorldSample::Stop()
{
}

AUTO_APPLICATION_MAIN(HelloWorldSample)