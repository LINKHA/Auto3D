#include "AutoEditor.h"
#include "SelectProjectWindow.h"

#include <ThirdParty/Imgui/imgui.h>
#include <ThirdParty/Imgui/imgui_user/imgui_user.h>

void DrawStartPage()
{

}

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
		
		bool state;
		ImGuiWindowFlags windowFlag = 0;
		windowFlag |= ImGuiWindowFlags_NoTitleBar;
		windowFlag |= ImGuiWindowFlags_NoResize;
		windowFlag |= ImGuiWindowFlags_NoMove;
		//windowFlag |= ImGuiWindowFlags_NoBackground;
		ImGui::Begin("SelectProjectWindow", &state, windowFlag);// Create a window called "Hello, world!" and append into it.

		ImGui::SetWindowPos(ImVec2(0, 0));
		ImGui::SetWindowSize(ImVec2(windowSize._x, windowSize._y));

		ImGui::PushFont("standard_big");

		ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoResize | ImGuiWindowFlags_HorizontalScrollbar |
			ImGuiWindowFlags_NoSavedSettings;

		ImGui::AlignTextToFramePadding();
		ImGui::TextUnformatted("RECENT PROJECTS");
		ImGui::Separator();
		ImGui::BeginGroup();
		{
			if (ImGui::BeginChild("projects_content",
				ImVec2(ImGui::GetContentRegionAvail().x * 0.7f, ImGui::GetContentRegionAvail().y),
				false, flags))
			{

				
			}
			ImGui::EndChild();
		}
		ImGui::EndGroup();

		ImGui::SameLine();

		ImGui::BeginGroup();
		{
			if (ImGui::Button("NEW PROJECT", ImVec2(ImGui::GetContentRegionAvailWidth(), 0.0f)))
			{
				
			}

			if (ImGui::Button("OPEN OTHER", ImVec2(ImGui::GetContentRegionAvailWidth(), 0.0f)))
			{
				
			}
		}
		ImGui::EndGroup();

		ImGui::PopFont();

		ImGui::End();
	}
	
}

void AutoEditor::Stop()
{
	
}

AUTO_APPLICATION_MAIN(AutoEditor)