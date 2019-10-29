#include "StartPage.h"

void StartPage::DrawStartPage()
{
	auto window = ModuleManager::Get().GraphicsModule()->RenderWindow();
	Vector2I windowSize = window->GetSize();
	auto onCreateProject = [&](const String& path)
	{
		auto fileSystem = ModuleManager::Get().FileSystemModule();
		fileSystem->CreateDir(path + "/Cache");
		fileSystem->CreateDir(path + "/Data");
		fileSystem->CreateDir(path + "/Meta");
		fileSystem->CreateDir(path + "/Settings");
	};
	auto onOpenProject = [&](const String& path) {
		auto fileSystem = ModuleManager::Get().FileSystemModule();
	};

	GUI::SetNextWindowPos(Vector2F(0, 0), ImGuiCond_Always);
	GUI::SetNextWindowSize(Vector2F(windowSize._x, windowSize._y), ImGuiCond_Always);

	bool state;
	GUI::WindowFlags windowFlag = 0;
	windowFlag |= ImGuiWindowFlags_NoTitleBar;
	windowFlag |= ImGuiWindowFlags_NoResize;
	windowFlag |= ImGuiWindowFlags_NoMove;
	windowFlag |= ImGuiWindowFlags_NoBackground;
	GUI::Begin("SelectProjectWindow", &state, windowFlag);// Create a window called "Hello, world!" and append into it.

	//GUI::SetWindowPos(Vector2F(0, 0));
	//GUI::SetWindowSize(Vector2F(windowSize._x, windowSize._y));

	GUI::PushFont("Msyh_36");

	GUI::WindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_HorizontalScrollbar |
		ImGuiWindowFlags_NoSavedSettings;

	GUI::AlignTextToFramePadding();
	GUI::TextUnformatted("RECENT PROJECTS");
	GUI::Separator();
	GUI::BeginGroup();
	{
		if (GUI::BeginChild("projects_content",
			Vector2F(GUI::GetContentRegionAvail()._x * 0.7f, GUI::GetContentRegionAvail()._y),
			false, flags))
		{
			///Temp
			Vector<String> rencentProjects;
			rencentProjects.Push("C:/Users/Administrator/Desktop/Test");
			rencentProjects.Push("C:/Users/Administrator/Desktop/Test2");

			for (auto it = rencentProjects.Begin(); it != rencentProjects.End(); ++it)
			{
				String path = *it;
				if (GUI::Selectable(path.CString()))
				{
					onOpenProject(path);
				}
			}
		}
		GUI::EndChild();
	}
	GUI::EndGroup();

	GUI::SameLine();

	GUI::BeginGroup();
	{
		if (GUI::Button("NEW PROJECT", Vector2F(ImGui::GetContentRegionAvailWidth(), 0.0f)))
		{
			String path;
			if (PickFolderDialog("", path))
			{
				onCreateProject(path);
			}
		}

		if (GUI::Button("OPEN OTHER", Vector2F(ImGui::GetContentRegionAvailWidth(), 0.0f)))
		{
			String path;
			if (PickFolderDialog("", path))
			{
				onOpenProject(path);
			}
		}
	}
	GUI::EndGroup();

	GUI::PopFont();

	GUI::End();
}