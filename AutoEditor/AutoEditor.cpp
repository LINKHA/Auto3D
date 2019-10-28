#include "AutoEditor.h"
#include "StartPage.h"

void AutoEditor::Init()
{
	auto graphics = ModuleManager::Get().GraphicsModule();
	graphics->RenderWindow()->SetTitle("AutoEditor");


}

void AutoEditor::Start()
{
	auto cache = ModuleManager::Get().CacheModule();
	auto ui = ModuleManager::Get().UiModule();

	Font* msyh24 = cache->LoadResource<Font>("Font/msyh.ttc");
	ui->AddFont(msyh24, 24, "Msyh_24", UIFontLanguage::CN);

	Font* msyh48 = cache->LoadResource<Font>("Font/msyh.ttc");
	ui->AddFont(msyh48, 36, "Msyh_36", UIFontLanguage::CN);
}
void AutoEditor::Update()
{
	if (!mainFlag)
	{
		StartPage::DrawStartPage();
	}
	else
	{
		DrawMainPage();
	}
}

void AutoEditor::Stop()
{
	
}

void AutoEditor::DrawMainPage()
{

}

AUTO_APPLICATION_MAIN(AutoEditor)