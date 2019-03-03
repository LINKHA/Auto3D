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
	uiCamera->SetPosition(Vector3F(0.0f, 0.0f, 0.0f));
	Subsystem<RegisteredBox>()->RegisterCanvas(canvas, uiCamera);

	for (int i = 0; i < 2; i++)
	{
		Sprite* sprite = canvas->CreateChild<Sprite>();
		sprite->SetTexture(texture);
	}
}
void TextureSample::Update()
{

}

void TextureSample::Stop()
{
}

AUTO_APPLICATION_MAIN(TextureSample)