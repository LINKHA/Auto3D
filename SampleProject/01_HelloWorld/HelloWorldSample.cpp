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
	bakcground->SetScale(Vector3F(15.0f, 15.0f, 1.0f));
}
void HelloWorldSample::Update()
{
	auto* graphics = Object::Subsystem<Graphics>();

	// Update uiCamera aspect ratio based on window size
	uiCamera->SetAspectRatio((float)graphics->GetWidth() / (float)graphics->GetHeight());
}

void HelloWorldSample::Stop()
{
}

AUTO_APPLICATION_MAIN(HelloWorldSample)