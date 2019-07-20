#include "AutoEditor.h"
#include "SelectProjectWindow.h"

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
	auto* window = Subsystem<Graphics>()->RenderWindow();
	Vector2I windowSize = window->GetSize();

	{
		ImGui::PushFont("standard_big");
		bool state;
		ImGuiWindowFlags windowFlag = 0;
		windowFlag |= ImGuiWindowFlags_NoTitleBar;
		windowFlag |= ImGuiWindowFlags_NoResize;
		windowFlag |= ImGuiWindowFlags_NoMove;
		//windowFlag |= ImGuiWindowFlags_NoBackground;
		ImGui::Begin("SelectProjectWindow", &state, windowFlag);// Create a window called "Hello, world!" and append into it.
		ImGui::SetWindowPos(ImVec2(0, 0));
		ImGui::SetWindowSize(ImVec2(windowSize._x, windowSize._y));

		if (ImGui::Button("Project Create",ImVec2(372.0f,200.0f)))
		{
		}
		//ImGui::SameLine();
			
		if (ImGui::Button("Project Select"))
		{
		}
		//ImGui::SameLine();
		ImGui::PopFont();

		ImGui::End();
	}
	
}

void AutoEditor::Stop()
{
	
}

AUTO_APPLICATION_MAIN(AutoEditor)