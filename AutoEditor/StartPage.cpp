#include "StartPage.h"

void StartPage::DrawStartPage()
{
	auto window = GModuleManager::Get().GraphicsModule()->RenderWindow();
	TVector2I windowSize = window->GetSize();
	auto onCreateProject = [&](const FString& path)
	{
		auto fileSystem = GModuleManager::Get().FileSystemModule();
		fileSystem->CreateDir(path + "/Cache");
		fileSystem->CreateDir(path + "/Data");
		fileSystem->CreateDir(path + "/Meta");
		fileSystem->CreateDir(path + "/Settings");
	};
	auto onOpenProject = [&](const FString& path) {
		auto fileSystem = GModuleManager::Get().FileSystemModule();
	};

	GUI::SetNextWindowPos(TVector2F(0, 0), ImGuiCond_Always);
	GUI::SetNextWindowSize(TVector2F(windowSize._x, windowSize._y), ImGuiCond_Always);

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
			TVector2F(GUI::GetContentRegionAvail()._x * 0.7f, GUI::GetContentRegionAvail()._y),
			false, flags))
		{
			///Temp
			TVector<FString> rencentProjects;
			rencentProjects.Push("C:/Users/Administrator/Desktop/Test");
			rencentProjects.Push("C:/Users/Administrator/Desktop/Test2");

			for (auto it = rencentProjects.Begin(); it != rencentProjects.End(); ++it)
			{
				FString path = *it;
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
		if (GUI::Button("NEW PROJECT", TVector2F(ImGui::GetContentRegionAvailWidth(), 0.0f)))
		{
			FString path;
			if (PickFolderDialog("", path))
			{
				onCreateProject(path);
			}
		}

		if (GUI::Button("OPEN OTHER", TVector2F(ImGui::GetContentRegionAvailWidth(), 0.0f)))
		{
			FString path;
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