#include "UISample.h"

void UISample::Init()
{
	auto* graphics = Object::Subsystem<Graphics>();
	graphics->RenderWindow()->SetTitle("UI Sample");

}
void UISample::Start()
{
	
	CreateLogo();
}
void UISample::Update()
{
	
}

void UISample::Stop()
{
}
void UISample::CreateLogo()
{
	auto* cache = Object::Subsystem<ResourceCache>();
	Sprite* logoLong = canvas->CreateChild<Sprite>();
	logoLong->SetTexture(cache->LoadResource<Texture>("LogoLong.png"));
	logoLong->SetScale(Vector3F(3.0f, 0.8f, 1.0f));
	logoLong->SetPosition(Vector3F(7.0f, -9.2f, -0.1f));
}
AUTO_APPLICATION_MAIN(UISample)