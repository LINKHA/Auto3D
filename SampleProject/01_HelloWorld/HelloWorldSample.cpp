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
	uiCamera->SetOrthographic(true);
	uiCamera->SetPosition(Vector3F(0.0f, 0.0f, -1.0f));
	Subsystem<RegisteredBox>()->RegisterCanvas(canvas, uiCamera);

	/*scene = new Scene();
	scene->CreateChild<Octree>();
	camera = scene->CreateChild<Camera>();
	Object::Subsystem<RegisteredBox>()->RegisterScene(scene, camera);*/

	Sprite* bakcground = canvas->CreateChild<Sprite>();
	bakcground->SetTexture(cache->LoadResource<Texture>("HelloWorld.png"));
	bakcground->SetScale(Vector3F(20.0f, 15.0f, 1.0f));

	Sprite* logo = canvas->CreateChild<Sprite>();
	logo->SetTexture(cache->LoadResource<Texture>("Newlogo.png"));
	logo->SetPosition(Vector3F(0.0f, 0.0f, -0.1f));
	logo->SetScale(Vector3F(3.0f, 3.0f, 1.0f));

	Sprite* logoLong = canvas->CreateChild<Sprite>();
	logoLong->SetTexture(cache->LoadResource<Texture>("logoLong.png"));
	logoLong->SetScale(Vector3F(3.0f, 0.8f, 1.0f));
	logoLong->SetPosition(Vector3F(7.0f, -9.0f, -0.1f));
}
void HelloWorldSample::Update()
{

}

void HelloWorldSample::Stop()
{
}

AUTO_APPLICATION_MAIN(HelloWorldSample)