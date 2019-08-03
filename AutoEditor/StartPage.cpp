#include "StartPage.h"
#include <ThirdParty/Imgui/imgui.h>
#include <ThirdParty/Imgui/imgui_user/imgui_user.h>

void StartPage::DrawStartPage()
{
	auto* window = Subsystem<Graphics>()->RenderWindow();
	Vector2I windowSize = window->GetSize();
	auto onCreateProject = [&](const String& path)
	{
		auto* fileSystem = Subsystem<FileSystem>();
		fileSystem->CreateDir(path + "/Cache");
		fileSystem->CreateDir(path + "/Data");
		fileSystem->CreateDir(path + "/Meta");
		fileSystem->CreateDir(path + "/Settings");
	};
	auto onOpenProject = [&](const String& path) {
		auto* fileSystem = Subsystem<FileSystem>();
	};

	bool state;
	ImGuiWindowFlags windowFlag = 0;
	windowFlag |= ImGuiWindowFlags_NoTitleBar;
	windowFlag |= ImGuiWindowFlags_NoResize;
	//windowFlag |= ImGuiWindowFlags_NoMove;
	windowFlag |= ImGuiWindowFlags_NoBackground;
	ImGui::Begin("SelectProjectWindow", &state, windowFlag);// Create a window called "Hello, world!" and append into it.

	ImGui::SetWindowPos(ImVec2(0, 0));
	ImGui::SetWindowSize(ImVec2(windowSize._x, windowSize._y));

	ImGui::PushFont(UIFont::standard_big);

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
			///Temp
			Vector<String> rencentProjects;
			rencentProjects.Push("C:/Users/Administrator/Desktop/Test");
			rencentProjects.Push("C:/Users/Administrator/Desktop/Test2");

			for (auto it = rencentProjects.Begin(); it != rencentProjects.End(); ++it)
			{
				String path = *it;
				if (ImGui::Selectable(path.CString()))
				{
					onOpenProject(path);
				}
			}
		}
		ImGui::EndChild();
	}
	ImGui::EndGroup();

	ImGui::SameLine();

	ImGui::BeginGroup();
	{
		if (ImGui::Button("NEW PROJECT", ImVec2(ImGui::GetContentRegionAvailWidth(), 0.0f)))
		{
			String path;
			if (PickFolderDialog("", path))
			{
				onCreateProject(path);
			}
		}

		if (ImGui::Button("OPEN OTHER", ImVec2(ImGui::GetContentRegionAvailWidth(), 0.0f)))
		{
			String path;
			if (PickFolderDialog("", path))
			{
				onOpenProject(path);
			}
		}
	}
	ImGui::EndGroup();

	ImGui::PopFont();

	ImGui::End();
}