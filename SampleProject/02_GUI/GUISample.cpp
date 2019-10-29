#include "GUISample.h"

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
	ui->AddFont(msyh26, 26, "Msyh_26", UIFontLanguage::CN);

	Font* msyh48 = cache->LoadResource<Font>("Font/msyh.ttc");
	ui->AddFont(msyh48, 48, "Msyh_48", UIFontLanguage::CN);

	Font* msyh14 = cache->LoadResource<Font>("Font/msyh.ttc");
	ui->AddFont(msyh14, 15, "Msyh_15", UIFontLanguage::CN);

	Font* msyh20 = cache->LoadResource<Font>("Font/msyh.ttc");
	ui->AddFont(msyh14, 20, "Msyh_20", UIFontLanguage::CN);
}

void GUISample::Update()
{
	Super::Update();
}

void GUISample::UIDraw()
{
	static bool simpleOverlayWindow = true;
	static bool logWindow = true;
	static bool layoutWindow = true;

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

	if (simpleOverlayWindow) ShowExampleAppSimpleOverlay(&simpleOverlayWindow);
	if (logWindow) ShowExampleAppLog(&logWindow);
	if (layoutWindow) ShowExampleAppLayout(&layoutWindow);
}

void GUISample::Stop()
{
	Super::Stop();
}

void GUISample::ShowExampleAppSimpleOverlay(bool* open)
{
	const float DISTANCE = 10.0f;
	static int corner = 0;
	GUI::IO& io = GUI::GetIO();
	if (corner != -1)
	{
		Vector2F window_pos = Vector2F((corner & 1) ? io.DisplaySize.x - DISTANCE : DISTANCE, (corner & 2) ? io.DisplaySize.y - DISTANCE : DISTANCE);
		Vector2F window_pos_pivot = Vector2F((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
		GUI::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
	}
	GUI::SetNextWindowBgAlpha(0.35f); // Transparent background
	if (GUI::Begin("Example: Simple overlay", open, (corner != -1 ? ImGuiWindowFlags_NoMove : 0) | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
	{
		GUI::PushFont("Msyh_15");
		
		if (GUI::IsMousePosValid())
			GUI::Text("Mouse Position:\n (%.1f,%.1f)", io.MousePos.x, io.MousePos.y);
		else
			GUI::Text("Mouse Position: <invalid>");
		if (GUI::BeginPopupContextWindow())
		{
			if (GUI::MenuItem("Custom", NULL, corner == -1)) corner = -1;
			if (GUI::MenuItem("Top-left", NULL, corner == 0)) corner = 0;
			if (GUI::MenuItem("Top-right", NULL, corner == 1)) corner = 1;
			if (GUI::MenuItem("Bottom-left", NULL, corner == 2)) corner = 2;
			if (GUI::MenuItem("Bottom-right", NULL, corner == 3)) corner = 3;
			if (open && GUI::MenuItem("Close")) *open = false;
			GUI::EndPopup();
		}
		GUI::Separator();
		static int count = 10;
		
		static float fps = 0; 
		static float currentFps = 0;
		if (!count--)
		{
			count = 5;
			currentFps = ModuleManager::Get().TimeModule()->GetFramesPerSecond();
		}

		if (Abs(fps - currentFps) > 0.2f)
		{
			fps = currentFps;
		}
		GUI::Text("FPS: %.1f", fps);
		
		GUI::PopFont();

	}
	GUI::End();
}

void GUISample::ShowExampleAppLog(bool* open)
{
	static UILog log;

	// For the demo: add a debug button _BEFORE_ the normal log window contents
	// We take advantage of a rarely used feature: multiple calls to Begin()/End() are appending to the _same_ window.
	// Most of the contents of the window will be added by the log.Draw() call.
	auto window = ModuleManager::Get().GraphicsModule()->RenderWindow();

	GUI::SetNextWindowPos(Vector2F(0, window->GetHeight() * 0.7f), ImGuiCond_Always);
	GUI::SetNextWindowSize(Vector2F(window->GetWidth(), window->GetHeight() * 0.3f), ImGuiCond_Always);

	GUI::WindowFlags windowFlags = 
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoBackground;

	GUI::PushFont("Msyh_20");

	GUI::Begin("Log", open, windowFlags);
	if (GUI::SmallButton("[Debug] Add 5 entries"))
	{
		static int counter = 0;
		for (int n = 0; n < 5; n++)
		{
			const char* categories[3] = { "info", "warn", "error" };
			const char* words[] = { "Bumfuzzled", "Cattywampus", "Snickersnee", "Abibliophobia", "Absquatulate", "Nincompoop", "Pauciloquent" };
			log.AddLog("[%05d] [%s] Hello, current time is %.1f, here's a word: '%s'\n",
				GUI::GetFrameCount(), categories[counter % IM_ARRAYSIZE(categories)], GUI::GetTime(), words[counter % IM_ARRAYSIZE(words)]);
			counter++;
		}
	}
	GUI::End();
	GUI::PopFont();
	// Actually call in the regular Log helper (which will Begin() into the same window as we just did)
	log.Draw("Log", open);
}

void GUISample::ShowExampleAppLayout(bool* open)
{
	GUI::SetNextWindowSize(Vector2F(500, 440), ImGuiCond_FirstUseEver);
	if (GUI::Begin("Example: Simple layout", open, ImGuiWindowFlags_MenuBar))
	{
		if (GUI::BeginMenuBar())
		{
			if (GUI::BeginMenu("File"))
			{
				if (GUI::MenuItem("Close")) *open = false;
				GUI::EndMenu();
			}
			GUI::EndMenuBar();
		}

		// left
		static int selected = 0;
		GUI::BeginChild("left pane", Vector2F(150, 0), true);
		for (int i = 0; i < 100; i++)
		{
			char label[128];
			sprintf(label, "MyObject %d", i);
			if (GUI::Selectable(label, selected == i))
				selected = i;
		}
		GUI::EndChild();
		GUI::SameLine();

		// right
		GUI::BeginGroup();
		GUI::BeginChild("item view", Vector2F(0, -GUI::GetFrameHeightWithSpacing())); // Leave room for 1 line below us
		GUI::Text("MyObject: %d", selected);
		GUI::Separator();
		if (GUI::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
		{
			if (GUI::BeginTabItem("Description"))
			{
				GUI::TextWrapped("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. ");
				GUI::EndTabItem();
			}
			if (GUI::BeginTabItem("Details"))
			{
				GUI::Text("ID: 0123456789");
				GUI::EndTabItem();
			}
			GUI::EndTabBar();
		}
		GUI::EndChild();
		if (GUI::Button("Revert")) {}
		GUI::SameLine();
		if (GUI::Button("Save")) {}
		GUI::EndGroup();
	}
	GUI::End();
}
AUTO_APPLICATION_MAIN(GUISample)