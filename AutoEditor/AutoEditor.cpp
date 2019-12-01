#include "AutoEditor.h"
#include "StartPage.h"

void AutoEditor::Init()
{
	auto graphics = GModuleManager::Get().GraphicsModule();
	graphics->RenderWindow()->SetTitle("AutoEditor");


}

void AutoEditor::Start()
{
	auto cache = GModuleManager::Get().CacheModule();
	auto ui = GModuleManager::Get().UiModule();

	AFont* msyh24 = cache->LoadResource<AFont>("Font/msyh.ttc");
	ui->AddFont(msyh24, 24, "Msyh_24", EUIFontLanguage::CN);

	AFont* msyh48 = cache->LoadResource<AFont>("Font/msyh.ttc");
	ui->AddFont(msyh48, 36, "Msyh_36", EUIFontLanguage::CN);
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