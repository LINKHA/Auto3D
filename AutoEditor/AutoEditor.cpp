#include "AutoEditor.h"
#include "StartPage.h"

#include <ThirdParty/Imgui/imgui.h>
#include <ThirdParty/Imgui/imgui_user/imgui_user.h>

void AutoEditor::Init()
{
	auto* graphics = Object::Subsystem<Graphics>();
	graphics->RenderWindow()->SetTitle("AutoEditor");
}

void AutoEditor::Start()
{
	
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