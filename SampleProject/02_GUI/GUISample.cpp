#include "GUISample.h"

#include <imgui.h>
void GUISample::Init()
{
	Super::Init();
	auto* graphics = Object::Module<Graphics>();
	graphics->RenderWindow()->SetTitle("GUI Sample");
}

void GUISample::Start()
{
	Super::Start();
	ResourceCache* cache = ModuleManager::Get().CacheModule();
	UI* ui = ModuleManager::Get().UiModule();

	Font* msyh26 = cache->LoadResource<Font>("Font/msyh.ttc");
	ui->AddFont(msyh26, 26, "Myth_26", UIFontLanguage::CN);

	Font* msyh48 = cache->LoadResource<Font>("Font/msyh.ttc");
	ui->AddFont(msyh48, 48, "Msyh_48", UIFontLanguage::CN);
}

void GUISample::Update()
{
	Super::Update();
}

void GUISample::UIDraw()
{
	static float f = 0.0f;
	static int counter = 0;
	auto* cache = Object::Module<ResourceCache>();
	auto flowerTexture = cache->LoadResource<Texture>("Texture/flower.png");

	GUI::Begin("Hello, world!");

	GUI::Text(u8"ÖÐÎÄ²âÊÔ");
	GUI::Image(flowerTexture, Vector2F(flowerTexture->GetWidth(), flowerTexture->GetHeight())); GUI::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

	if (GUI::Button("Button")) 
		counter++;
	GUI::SameLine();

	GUI::PushFont("Msyh_48");
	GUI::Text("counter = %d", counter);
	GUI::PopFont();

	GUI::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	GUI::End();
}

void GUISample::Stop()
{
	Super::Stop();
}

AUTO_APPLICATION_MAIN(GUISample)