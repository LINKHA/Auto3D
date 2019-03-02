#include "TextureSample.h"

void TextureSample::Init()
{

}
void TextureSample::Start()
{
	auto* cache = Object::Subsystem<ResourceCache>();

	SubscribeToEvent(Subsystem<Graphics>()->RenderWindow()->closeRequestEvent, &TextureSample::HandleCloseRequest);
	
	//scene = new Scene();
	//scene->CreateChild<Octree>();
	//camera = scene->CreateChild<Camera>();
	//Subsystem<RegisteredBox>()->RegisterScene(scene, camera);

	canvas = new Canvas();
	uiCamera = canvas->CreateChild<UICamera>();
	uiCamera->SetPosition(Vector3F(0.0f, 0.0f, 0.0f));
	Subsystem<RegisteredBox>()->RegisterCanvas(canvas, uiCamera);

	Sprite* sprite = canvas->CreateChild<Sprite>();
	sprite->SetTexture(cache->LoadResource<Texture>("Test.png"));
	
}
void TextureSample::Update()
{

}

void TextureSample::Stop()
{
}

AUTO_APPLICATION_MAIN(TextureSample)