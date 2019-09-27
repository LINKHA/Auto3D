#include "UI.h"
#include "Canvas.h"
#include "Text.h"
#include "Button.h"
#include "Slider.h"
#include "ColorEdit.h"
#include "CheckBox.h"
#include "../Engine/ModuleManager.h"

#include <imgui.h>
#include <imgui_user/imgui_user.h>


#if defined(_WIN32) || defined(_WIN64)
#	include "../Adapter/imgui_impl_windows.h"
#else
#	include "../Adapter/imgui_impl_sdl.h"
#endif


#if defined(AUTO_OPENGL)
#	include "../Adapter/OGL/imgui_impl_opengl3.h"
#elif defined(AUTO_VULKAN)
#	include "../Adapter/VK/imgui_impl_vulkan.h"
#endif
#include "../Graphics/Graphics.h"

#include "../Debug/Log.h"

#pragma region TTF
#	include "tff.FontDefault.h"
#pragma endregion


namespace Auto3D
{

UI::UI() :
	_window(nullptr)
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsYellow();


	ImFontConfig config;
	config.FontDataOwnedByAtlas = false;
	config.MergeMode = false;

	ImGui::AddFont(UIFont::default, io.Fonts->AddFontDefault(&config));
	ImGui::AddFont(UIFont::standard,
		io.Fonts->AddFontFromMemoryTTF(reinterpret_cast<void*>(std::intptr_t(sFontDefault)),
			sizeof(sFontDefault), 20, &config));

	config.MergeMode = false;
	config.PixelSnapH = false;
	ImGui::AddFont(UIFont::standard_big,
		io.Fonts->AddFontFromMemoryTTF(reinterpret_cast<void*>(std::intptr_t(sFontDefault)),
			sizeof(sFontDefault), 50, &config));
	RegisterModule(this);
}

UI::~UI()
{
#if defined(_WIN32) || defined(_WIN64)
	// Cleanup
#	if defined(AUTO_OPENGL)
	ImGui_ImplOpenGL3_Shutdown();
#	endif
#if defined(_WIN32) || defined(_WIN64)
	ImGui_ImplWin32_Shutdown();
#else
	ImGui_ImplSDL2_Shutdown();
#endif
	ImGui::DestroyContext();

	RemoveModule(this);
#endif
}

#ifdef AUTO_OPENGL
bool UI::SetMode(Window* window, GraphicsContext* context)
#else
bool UI::SetMode(Window* window)
#endif
{
#if defined(_WIN32) || defined(_WIN64)
	if (!window)
		return false;

	_window = window;


	const char* glslVersion;
	GraphicsSLVersion::Type slVersion = ModuleManager::Get().GraphicsModule()->GetGraphicsSLVersion();
	if (slVersion == GraphicsSLVersion::GLSL_430)
		glslVersion = "#version 430";
	else if (slVersion == GraphicsSLVersion::GLSL_330)
		glslVersion = "#version 330";
	else if (slVersion == GraphicsSLVersion::GLSL_150)
		glslVersion = "#version 150";
	else if (slVersion == GraphicsSLVersion::GLSL_450)
		glslVersion = "#version 450";
	else if (slVersion == GraphicsSLVersion::GLSL_ES_300)
		glslVersion = "#version 300 es";
	else if (slVersion == GraphicsSLVersion::GLSL_ES_100)
		glslVersion = "#version 100";
	else
		glslVersion = NULL;
	// Setup Platform/Renderer bindings
#	if defined(AUTO_OPENGL)
#		if defined(_WIN32) || defined(_WIN64)
	ImGui_ImplWin32_Init(window->Handle());
#else	
	ImGui_ImplSDL2_InitForOpenGL(window->Handle(), context->Context());
#endif
	ImGui_ImplOpenGL3_Init(glslVersion);
#	elif defined(AUTO_VULKAN)
	/*ImGui_ImplSDL2_InitForVulkan(window->Handle());
	ImGui_ImplVulkan_InitInfo init_info = {};
	init_info.Instance = g_Instance;
	init_info.PhysicalDevice = g_PhysicalDevice;
	init_info.Device = g_Device;
	init_info.QueueFamily = g_QueueFamily;
	init_info.Queue = g_Queue;
	init_info.PipelineCache = g_PipelineCache;
	init_info.DescriptorPool = g_DescriptorPool;
	init_info.Allocator = g_Allocator;
	init_info.MinImageCount = g_MinImageCount;
	init_info.ImageCount = wd->ImageCount;
	init_info.CheckVkResultFn = check_vk_result;
	ImGui_ImplVulkan_Init(&init_info, wd->RenderPass);*/
#	endif
#endif
	return true;
}

bool UI::BeginUI()
{
#if defined(_WIN32) || defined(_WIN64)
	if (!_window)
	{
		ErrorString("Fail update new frame from ui,may bo window not create.");
		return false;
	}
	// Start the Dear ImGui frame
#	if defined(AUTO_OPENGL)
	ImGui_ImplOpenGL3_NewFrame();
#	endif

#	if defined(_WIN32) || defined(_WIN64)
	ImGui_ImplWin32_NewFrame();
#	else
	ImGui_ImplSDL2_NewFrame(_window->Handle());
#	endif

	ImGui::NewFrame();
#endif
	
	return true;
}

void UI::Present()
{
	ImGui::Render();
#	if defined(AUTO_OPENGL)
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#	endif
}

void UI::Render(Canvas* canvas)
{
	if (!canvas)
	{
		ErrorString("Fail render canvas,maybe canvas fail create");
		return;
	}

	const char* title = canvas->GetTitle().CString();

	if (canvas->GetCloseButtonEnable())
	{
		bool state;
		ImGui::Begin(title, &state);
		canvas->SetCloseState(state);
	}
	else
		ImGui::Begin(title);

	const Vector<SharedPtr<UINode> >& nodes = canvas->Children();
	for (auto it = nodes.Begin(); it != nodes.End(); ++it)
	{
		UINode* node = *it;
		if (node->IsEnabled())
			node->DefineNode();
	}

	ImGui::End();
}

void UI::ProcessEvent(const SDL_Event* event)
{
#if defined(_WIN32) || defined(_WIN64)
	//ImguiWin
#else
	ImGui_ImplSDL2_ProcessEvent(event);
#endif
}

void RegisterUILibrary()
{
	static bool registered = false;
	if (registered)
		return;
	registered = true;

	Canvas::RegisterObject();
	Text::RegisterObject();
	Button::RegisterObject();
	Slider::RegisterObject();
	ColorEdit::RegisterObject();
	CheckBox::RegisterObject();
}

}