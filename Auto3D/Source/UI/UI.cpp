#include "UI.h"
#include "IO/FileSystem.h"
#include "Core/Modules/ModuleManager.h"

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
#include "Graphics/Graphics.h"

#include "Debug/Log.h"

namespace Auto3D
{

THashMap<FString, const char*> UIFont::Data = THashMap<FString, const char*>();
int UIFont::DefaultSize = 0;

UI::UI() :
	_window(nullptr)
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	//IO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
	//IO().ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsYellow();

	ImFontConfig config;
	config.FontDataOwnedByAtlas = false;
	config.MergeMode = false;
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
	ImGui_ImplSDL2_InitForVulkan(window->Handle());
	ImGui_ImplVulkan_InitInfo init_info = {};
	init_info.Instance = g_Instance;
	init_info.PhysicalDevice = g_PhysicalDevice;
	init_info.Device = g_Device;
	init_info.QueueFamily = g_QueueFamily;
	init_info.Queue = g_Queue;
	init_info.PipelineCache = g_PipelineCache;
	init_info.DescriptorPool = g_DescriptorPool;
	init_info.TAllocator = g_Allocator;
	init_info.MinImageCount = g_MinImageCount;
	init_info.ImageCount = wd->ImageCount;
	init_info.CheckVkResultFn = check_vk_result;
	ImGui_ImplVulkan_Init(&init_info, wd->RenderPass);
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

void UI::AddFont(Font* font, int pixels, FString fontname, UIFontLanguage::Data languageType)
{
	if (fontname == "Default")
		fontname = "Default" + UIFont::DefaultSize++;

	// Ignore if you have the same Font
	if (ImGui::GetFont(fontname.CString()))
		return;

	const ImWchar* laType;
	switch (languageType)
	{
	case Auto3D::UIFontLanguage::DEFAULT:
		laType = IO().Fonts->GetGlyphRangesDefault();
		break;
	case Auto3D::UIFontLanguage::CN:
		laType = IO().Fonts->GetGlyphRangesChineseSimplifiedCommon();
		break;
	case Auto3D::UIFontLanguage::CNF:
		laType = IO().Fonts->GetGlyphRangesChineseFull();
		break;
	case Auto3D::UIFontLanguage::JP:
		laType = IO().Fonts->GetGlyphRangesJapanese();
		break;
	case Auto3D::UIFontLanguage::KR:
		laType = IO().Fonts->GetGlyphRangesKorean();
		break;
	case Auto3D::UIFontLanguage::THA:
		laType = IO().Fonts->GetGlyphRangesThai();
		break;
	case Auto3D::UIFontLanguage::RUS:
		laType = IO().Fonts->GetGlyphRangesCyrillic();
		break;
	case Auto3D::UIFontLanguage::VIE:
		laType = IO().Fonts->GetGlyphRangesVietnamese();
		break;
	default:
		WarningString("Fail to add font,language type is non-standard,Automatic generation becomes default.");
		laType = IO().Fonts->GetGlyphRangesDefault();
		break;
	}

	ImFont* tFont = IO().Fonts->AddFontFromMemoryTTF(font->Data(), font->GetDataSize(), pixels, &ImFontConfig(), laType);
	ImGui::AddFont(UIFont::Data[fontname] = fontname.CString(), tFont);
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
}

namespace GUI
{
// Context creation and access
// Each context create its own ImFontAtlas by default. You may instance one yourself and pass it to CreateContext() to share a font atlas between imgui contexts.
// All those functions are not reliant on the current context.
Context* CreateContext(FontAtlas* shared_font_atlas)
{
	return ImGui::CreateContext(shared_font_atlas);
}
void DestroyContext(Context* ctx)
{
	ImGui::DestroyContext(ctx);
}
Context* GetCurrentContext()
{
	return ImGui::GetCurrentContext();
}
void SetCurrentContext(Context* ctx)
{
	ImGui::SetCurrentContext(ctx);
}
bool DebugCheckVersionAndDataLayout(const char* version_str, size_t sz_io, size_t sz_style, size_t sz_vec2, size_t sz_vec4, size_t sz_drawvert, size_t sz_drawidx)
{
	return ImGui::DebugCheckVersionAndDataLayout(version_str, sz_io, sz_style, sz_vec2, sz_vec4, sz_drawvert, sz_drawidx);
}

// Main
GUI::IO& GetIO()
{
	return ImGui::GetIO();
}
GUI::Style& GetStyle()
{
	return ImGui::GetStyle();
}
void NewFrame()
{
	ImGui::NewFrame();
}
void EndFrame()
{
	ImGui::EndFrame();
}
void Render()
{
	ImGui::Render();
}
DrawData* GetDrawData()
{
	return ImGui::GetDrawData();
}
// Windows
// - Begin() = push window to the stack and start appending to it. End() = pop window from the stack.
// - You may append multiple times to the same window during the same frame.
// - Passing 'bool* p_open != NULL' shows a window-closing widget in the upper-right corner of the window,
//   which clicking will set the boolean to false when clicked.
// - Begin() return false to indicate the window is collapsed or fully clipped, so you may early out and omit submitting
//   anything to the window. Always call a matching End() for each Begin() call, regardless of its return value!
//   [this is due to legacy reason and is inconsistent with most other functions such as BeginMenu/EndMenu, BeginPopup/EndPopup, etc.
//    where the EndXXX call should only be called if the corresponding BeginXXX function returned true.]
// - Note that the bottom of window stack always contains a window called "Debug".
bool Begin(const char* name, bool* p_open, WindowFlags flags)
{
	return ImGui::Begin(name, p_open, flags);
}

void End()
{
	ImGui::End();
}

// Child Windows
// - Use child windows to begin into a self-contained independent scrolling/clipping regions within a host window. Child windows can embed their own child.
// - For each independent axis of 'size': ==0.0f: use remaining host window size / >0.0f: fixed size / <0.0f: use remaining window size minus abs(size) / Each axis can use a different mode, e.g. Vector2F(0,400).
// - BeginChild() returns false to indicate the window is collapsed or fully clipped, so you may early out and omit submitting anything to the window.
//   Always call a matching EndChild() for each BeginChild() call, regardless of its return value [this is due to legacy reason and is inconsistent with most other functions such as BeginMenu/EndMenu, BeginPopup/EndPopup, etc. where the EndXXX call should only be called if the corresponding BeginXXX function returned true.]
bool BeginChild(const char* str_id, const Vector2F& size, bool border, WindowFlags flags)
{
	return ImGui::BeginChild(str_id, ToImVal(size), border, flags);
}

bool BeginChild(ID id, const Vector2F& size, bool border, WindowFlags flags)
{
	return ImGui::BeginChild(id, ToImVal(size), border, flags);
}
void EndChild()
{
	ImGui::EndChild();
}

bool IsWindowAppearing()
{
	return ImGui::IsWindowAppearing();
}

bool IsWindowCollapsed()
{
	return ImGui::IsWindowCollapsed();
}

bool IsWindowFocused(FocusedFlags flags)
{
	return ImGui::IsWindowFocused(flags);
}

bool IsWindowHovered(HoveredFlags flags)
{
	return ImGui::IsWindowHovered(flags);
}

DrawList* GetWindowDrawList()
{
	return ImGui::GetWindowDrawList();
}

Vector2F GetWindowPos()
{
	return ImToVal(ImGui::GetWindowPos());
}

Vector2F GetWindowSize()
{
	return ImToVal(ImGui::GetWindowSize());
}

float GetWindowWidth()
{
	return ImGui::GetWindowWidth();
}
float GetWindowHeight()
{
	return ImGui::GetWindowHeight();
}

// Prefer using SetNextXXX functions (before Begin) rather that SetXXX functions (after Begin).
void SetNextWindowPos(const Vector2F& pos, Cond cond, const Vector2F& pivot)
{
	ImGui::SetNextWindowPos(ToImVal(pos), cond, ToImVal(pivot));
}

void SetNextWindowSize(const Vector2F& size, Cond cond)
{
	ImGui::SetNextWindowSize(ToImVal(size), cond);
}

void SetNextWindowSizeConstraints(const Vector2F& size_min, const Vector2F& size_max, ImGuiSizeCallback custom_callback, void* custom_callback_data)
{
	ImGui::SetNextWindowSizeConstraints(ToImVal(size_min), ToImVal(size_max), custom_callback, custom_callback_data);
}

void SetNextWindowContentSize(const Vector2F& size)
{
	ImGui::SetNextWindowContentSize(ToImVal(size));
}

void SetNextWindowCollapsed(bool collapsed, Cond cond)
{
	ImGui::SetNextWindowCollapsed(collapsed, cond);
}

void SetNextWindowFocus()
{
	ImGui::SetNextWindowFocus();
}

void SetNextWindowBgAlpha(float alpha)
{
	ImGui::SetNextWindowBgAlpha(alpha);
}

void SetWindowPos(const Vector2F& pos, Cond cond)
{
	ImGui::SetWindowPos(ToImVal(pos), cond);
}

void SetWindowSize(const Vector2F& size, Cond cond)
{
	ImGui::SetWindowSize(ToImVal(size), cond);
}

void SetWindowCollapsed(bool collapsed, Cond cond)
{
	ImGui::SetWindowCollapsed(collapsed, cond);
}

void SetWindowFocus()
{
	ImGui::SetWindowFocus();
}

void SetWindowFontScale(float scale)
{
	ImGui::SetWindowFontScale(scale);
}

void SetWindowPos(const char* name, const Vector2F& pos, Cond cond)
{
	ImGui::SetWindowPos(name, ToImVal(pos), cond);
}

void SetWindowSize(const char* name, const Vector2F& size, Cond cond)
{
	ImGui::SetWindowSize(name, ToImVal(size), cond);
}
void SetWindowCollapsed(const char* name, bool collapsed, Cond cond)
{
	ImGui::SetWindowCollapsed(name, collapsed, cond);
}
void SetWindowFocus(const char* name)
{
	ImGui::SetWindowFocus(name);
}

// Content region
// - Those functions are bound to be redesigned soon (they are confusing, incomplete and return values in local window coordinates which increases confusion)
Vector2F GetContentRegionMax()
{
	return ImToVal(ImGui::GetContentRegionMax());
}
Vector2F GetContentRegionAvail()
{
	return ImToVal(ImGui::GetContentRegionAvail());
}
Vector2F GetWindowContentRegionMin()
{
	return ImToVal(ImGui::GetWindowContentRegionMin());
}
Vector2F GetWindowContentRegionMax()
{
	return ImToVal(ImGui::GetWindowContentRegionMax());
}
float GetWindowContentRegionWidth()
{
	return ImGui::GetWindowContentRegionWidth();
}

// Windows Scrolling
float GetScrollX()
{
	return ImGui::GetScrollX();
}
float GetScrollY()
{
	return ImGui::GetScrollY();
}
float GetScrollMaxX()
{
	return ImGui::GetScrollMaxX();
}
float GetScrollMaxY()
{
	return ImGui::GetScrollMaxY();
}
void SetScrollX(float scroll_x)
{
	ImGui::SetScrollX(scroll_x);
}
void SetScrollY(float scroll_y)
{
	ImGui::SetScrollY(scroll_y);
}
void SetScrollHereY(float center_y_ratio)
{
	ImGui::SetScrollHereY(center_y_ratio);
}
void SetScrollFromPosY(float local_y, float center_y_ratio)
{
	ImGui::SetScrollFromPosY(local_y, center_y_ratio);
}

// Parameters stacks (shared)
void PushFont(const FString& font)
{
	ImGui::PushFont(font.CString());
}
void PushFont(Font* font)
{
	ImGui::PushFont(font);
}
void PopFont()
{
	ImGui::PopFont();
}
void PushStyleColor(Col idx, unsigned col)
{
	ImGui::PushStyleColor(idx, col);
}
void PushStyleColor(Col idx, const Vector4F& col)
{
	ImGui::PushStyleColor(idx, ToImVal(col));
}
void PopStyleColor(int count)
{
	ImGui::PopStyleColor(count);
}
void PushStyleVar(StyleVar idx, float val)
{
	ImGui::PushStyleVar(idx, val);
}
void PushStyleVar(StyleVar idx, const Vector2F& val)
{
	ImGui::PushStyleVar(idx, ToImVal(val));
}
void PopStyleVar(int count)
{
	ImGui::PopStyleVar(count);
}
const Vector4F& GetStyleColorVec4(Col idx)
{
	return ImToVal(ImGui::GetStyleColorVec4(idx));
}
Font* GetFont()
{
	return ImGui::GetFont();
}
float GetFontSize() 
{
	return ImGui::GetFontSize();
}
Vector2F GetFontTexUvWhitePixel()
{
	return ImToVal(ImGui::GetFontTexUvWhitePixel());
}
unsigned GetColorU32(Col idx, float alpha_mul)
{
	return ImGui::GetColorU32(idx, alpha_mul);
}
unsigned GetColorU32(const Vector4F& col)
{
	return ImGui::GetColorU32(ToImVal(col));
}
unsigned GetColorU32(unsigned col) 
{
	return ImGui::GetColorU32(col);
}

// Parameters stacks (current window)
void PushItemWidth(float item_width)
{
	ImGui::PushItemWidth(item_width);
}
void PopItemWidth()
{
	ImGui::PopItemWidth();
}
void SetNextItemWidth(float item_width)
{
	ImGui::SetNextItemWidth(item_width);
}
float CalcItemWidth()
{
	return ImGui::CalcItemWidth();
}
void PushTextWrapPos(float wrap_local_pos_x)
{
	ImGui::PushTextWrapPos(wrap_local_pos_x);
}
void PopTextWrapPos()
{
	ImGui::PopTextWrapPos();
}
void PushAllowKeyboardFocus(bool allow_keyboard_focus)
{
	ImGui::PushAllowKeyboardFocus(allow_keyboard_focus);
}
void PopAllowKeyboardFocus()
{
	ImGui::PopAllowKeyboardFocus();
}
void PushButtonRepeat(bool repeat)
{
	ImGui::PushButtonRepeat(repeat);
}
void PopButtonRepeat()
{
	ImGui::PopButtonRepeat();
}

// Cursor / Layout
// - By "cursor" we mean the current output position.
// - The typical widget behavior is to output themselves at the current cursor position, then move the cursor one line down.
void Separator()
{
	ImGui::Separator();
}
void SameLine(float offset_from_start_x, float spacing)
{
	ImGui::SameLine(offset_from_start_x, spacing);
}
void NewLine()
{
	ImGui::NewLine();
}
void Spacing()
{
	ImGui::Spacing();
}
void Dummy(const Vector2F& size)
{
	ImGui::Spacing();
}
void Indent(float indent_w)
{
	ImGui::Indent(indent_w);
}
void Unindent(float indent_w)
{
	ImGui::Unindent(indent_w);
}
void BeginGroup()
{
	ImGui::BeginGroup();
}
void EndGroup()
{
	ImGui::EndGroup();
}
Vector2F GetCursorPos()
{
	return ImToVal(ImGui::GetCursorPos());
}
float GetCursorPosX()
{
	return ImGui::GetCursorPosX();
}
float GetCursorPosY()
{
	return ImGui::GetCursorPosY();
}
void SetCursorPos(const Vector2F& local_pos)
{
	ImGui::SetCursorPos(ToImVal(local_pos));
}
void SetCursorPosX(float local_x)
{
	ImGui::SetCursorPosX(local_x);
}
void SetCursorPosY(float local_y)
{
	ImGui::SetCursorPosY(local_y);
}
Vector2F GetCursorStartPos()
{
	return ImToVal(ImGui::GetCursorStartPos());
}
Vector2F GetCursorScreenPos()
{
	return ImToVal(ImGui::GetCursorScreenPos());
}

void SetCursorScreenPos(const Vector2F& pos)
{
	ImGui::SetCursorScreenPos(ToImVal(pos));
}
void AlignTextToFramePadding()
{
	ImGui::AlignTextToFramePadding();
}
float GetTextLineHeight()
{
	return ImGui::GetTextLineHeight();
}
float GetTextLineHeightWithSpacing()
{
	return ImGui::GetTextLineHeightWithSpacing();
}
float GetFrameHeight()
{
	return ImGui::GetFrameHeight();
}
float GetFrameHeightWithSpacing()
{
	return ImGui::GetFrameHeightWithSpacing();
}

// ID stack/scopes
// - Read the FAQ for more details about how ID are handled in dear imgui. If you are creating widgets in a loop you most
//   likely want to push a unique identifier (e.g. object pointer, loop index) to uniquely differentiate them.
// - The resulting ID are hashes of the entire stack.
// - You can also use the "Label##foobar" syntax within widget label to distinguish them from each others.
// - In this header file we use the "label"/"name" terminology to denote a string that will be displayed and used as an ID,
//   whereas "str_id" denote a string that is only used as an ID and not normally displayed.
void PushID(const char* str_id)
{
	ImGui::PushID(str_id);
}
void PushID(const char* str_id_begin, const char* str_id_end)
{
	ImGui::PushID(str_id_begin,str_id_end);
}
void PushID(const void* ptr_id)
{
	ImGui::PushID(ptr_id);
}
void PushID(int int_id)
{
	ImGui::PushID(int_id);
}
void PopID()
{
	ImGui::PopID();
}
ID       GetID(const char* str_id)
{
	return ImGui::GetID(str_id);
}
ID GetID(const char* str_id_begin, const char* str_id_end)
{
	return ImGui::GetID(str_id_begin,str_id_end);
}
ID GetID(const void* ptr_id)
{
	return ImGui::GetID(ptr_id);
}

// Widgets: Text
void TextUnformatted(const char* text, const char* text_end)
{
	ImGui::TextUnformatted(text,text_end);
}
void Text(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	TextV(fmt, args);
	va_end(args);
}
void TextV(const char* fmt, va_list args)
{
	ImGui::TextV(fmt,  args);
}
void TextColored(const Vector4F& col, const char* fmt, ...)
{

	va_list args;
	va_start(args, fmt);
	TextColoredV(col, fmt, args);
	va_end(args);
}
void TextColoredV(const Vector4F& col, const char* fmt, va_list args)
{
	ImGui::TextColoredV(ToImVal(col), fmt,  args);
}
void TextDisabled(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	TextDisabledV(fmt, args);
	va_end(args);
}
void TextDisabledV(const char* fmt, va_list args)
{
	ImGui::TextDisabledV(fmt,  args);
}
void TextWrapped(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	TextWrappedV(fmt, args);
	va_end(args);
}
void TextWrappedV(const char* fmt, va_list args)
{
	ImGui::TextWrappedV(fmt,  args);
}
void LabelText(const char* label, const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	LabelTextV(label, fmt, args);
	va_end(args);
}
void LabelTextV(const char* label, const char* fmt, va_list args)
{
	ImGui::LabelTextV(label,fmt,  args);
}
void BulletText(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	BulletTextV(fmt, args);
	va_end(args);
}
void BulletTextV(const char* fmt, va_list args)
{
	ImGui::BulletTextV(fmt,  args);
}

	// Widgets: Main
	// - Most widgets return true when the value has been changed or when pressed/selected
bool Button(const char* label, const Vector2F& size)
{
	return ImGui::Button(label, ToImVal(size));
}
bool SmallButton(const char* label)
{
	return ImGui::SmallButton( label);
}
bool InvisibleButton(const char* str_id, const Vector2F& size)
{
	return ImGui::InvisibleButton(str_id, ToImVal(size));
}
bool ArrowButton(const char* str_id, Dir dir)
{
	return ImGui::ArrowButton( str_id,  dir);
}
void Image(Texture* user_texture, const Vector2F& size, const Vector2F& uv0, const Vector2F& uv1, const Vector4F& tint_col, const Vector4F& border_col)
{
	ImGui::Image((void*)user_texture->GetGLTexture(), ToImVal(size), ToImVal(uv0), ToImVal(uv1), ToImVal(tint_col), ToImVal(border_col));
}
bool ImageButton(Texture* user_texture, const Vector2F& size, const Vector2F& uv0, const Vector2F& uv1, int frame_padding, const Vector4F& bg_col, const Vector4F& tint_col )
{
	return ImGui::ImageButton((void*)user_texture->GetGLTexture(), ToImVal(size), ToImVal(uv0), ToImVal(uv1),  frame_padding, ToImVal(bg_col), ToImVal(tint_col));
}
bool Checkbox(const char* label, bool* v)
{
	return ImGui::Checkbox( label,  v);
}
bool CheckboxFlags(const char* label, unsigned int* flags, unsigned int flags_value)
{
	return ImGui::CheckboxFlags(label,flags,flags_value);
}
bool RadioButton(const char* label, bool active)
{
	return ImGui::RadioButton(label, active);
}
bool RadioButton(const char* label, int* v, int v_button)
{
	return ImGui::RadioButton(label, v, v_button);
}
void ProgressBar(float fraction, const Vector2F& size_arg, const char* overlay)
{
	ImGui::ProgressBar(fraction, ToImVal(size_arg), overlay);
}
void Bullet()
{
	ImGui::Bullet();
}

// Widgets: Combo Box
// - The new BeginCombo()/EndCombo() api allows you to manage your contents and selection state however you want it, by creating e.g. Selectable() items.
// - The old Combo() api are helpers over BeginCombo()/EndCombo() which are kept available for convenience purpose.
bool BeginCombo(const char* label, const char* preview_value, ComboFlags flags)
{
	return ImGui::BeginCombo(label, preview_value, flags);
}
void EndCombo()
{
	ImGui::EndCombo();
}
bool Combo(const char* label, int* current_item, const char* const items[], int items_count, int popup_max_height_in_items1)
{
	return ImGui::Combo(label, current_item, items, items_count, popup_max_height_in_items1);
}
bool Combo(const char* label, int* current_item, const char* items_separated_by_zeros, int popup_max_height_in_items)
{
	return ImGui::Combo(label, current_item, items_separated_by_zeros, popup_max_height_in_items);
}
bool Combo(const char* label, int* current_item, bool(*items_getter)(void* data, int idx, const char** out_text), void* data, int items_count, int popup_max_height_in_items)
{
	return ImGui::Combo(label, current_item, items_getter, data, items_count, popup_max_height_in_items);
}

// Widgets: Drags
// - CTRL+Click on any drag box to turn them into an input box. Manually input values aren't clamped and can go off-bounds.
// - For all the Float2/Float3/Float4/Int2/Int3/Int4 versions of every functions, note that a 'float v[X]' function argument is the same as 'float* v', the array syntax is just a way to document the number of elements that are expected to be accessible. You can pass address of your first element out of a contiguous set, e.g. &myvector.x
// - Adjust format string to decorate the value with a prefix, a suffix, or adapt the editing and display precision e.g. "%.3f" -> 1.234; "%5.2f secs" -> 01.23 secs; "Biscuit: %.0f" -> Biscuit: 1; etc.
// - Speed are per-pixel of mouse movement (v_speed=0.2f: mouse needs to move by 5 pixels to increase value by 1). For gamepad/keyboard navigation, minimum speed is Max(v_speed, minimum_step_at_given_precision).
bool DragFloat(const char* label, float* v, float v_speed, float v_min, float v_max, const char* format, float power)
{
	return ImGui::DragFloat(label, v, v_speed, v_min, v_max, format, power);
}
bool DragFloat2(const char* label, float v[2], float v_speed, float v_min, float v_max, const char* format, float power)
{
	return ImGui::DragFloat2(label, v, v_speed, v_min, v_max, format, power);
}
bool DragFloat3(const char* label, float v[3], float v_speed, float v_min, float v_max, const char* format, float power)
{
	return ImGui::DragFloat3(label, v, v_speed, v_min, v_max, format, power);
}
bool DragFloat4(const char* label, float v[4], float v_speed, float v_min, float v_max, const char* format, float power)
{
	return ImGui::DragFloat4(label, v, v_speed, v_min, v_max, format, power);
}
bool DragFloatRange2(const char* label, float* v_current_min, float* v_current_max, float v_speed, float v_min, float v_max, const char* format, const char* format_max, float power)
{
	return ImGui::DragFloatRange2(label, v_current_min, v_current_max, v_speed, v_min, v_max, format, format_max, power);
}
bool DragInt(const char* label, int* v, float v_speed, int v_min, int v_max, const char* format)
{
	return ImGui::DragInt(label, v, v_speed, v_min, v_max, format);
}
bool DragInt2(const char* label, int v[2], float v_speed, int v_min, int v_max, const char* format)
{
	return ImGui::DragInt2(label, v, v_speed, v_min, v_max, format);
}
bool DragInt3(const char* label, int v[3], float v_speed, int v_min, int v_max, const char* format)
{
	return ImGui::DragInt3(label, v, v_speed, v_min, v_max, format);
}
bool DragInt4(const char* label, int v[4], float v_speed, int v_min, int v_max, const char* format)
{
	return ImGui::DragInt4(label, v, v_speed, v_min, v_max, format);
}
bool DragIntRange2(const char* label, int* v_current_min, int* v_current_max, float v_speed, int v_min, int v_max, const char* format, const char* format_max)
{
	return ImGui::DragIntRange2(label, v_current_min, v_current_max, v_speed, v_min, v_max, format, format_max);
}
bool DragScalar(const char* label, DataType data_type, void* v, float v_speed, const void* v_min, const void* v_max, const char* format, float power)
{
	return ImGui::DragScalar(label, data_type, v, v_speed, v_min, v_max, format, power);
}
bool DragScalarN(const char* label, DataType data_type, void* v, int components, float v_speed, const void* v_min, const void* v_max, const char* format, float power)
{
	return ImGui::DragScalarN(label, data_type, v, components, v_speed, v_min, v_max, format, power);
}

// Widgets: Sliders
// - CTRL+Click on any slider to turn them into an input box. Manually input values aren't clamped and can go off-bounds.
// - Adjust format string to decorate the value with a prefix, a suffix, or adapt the editing and display precision e.g. "%.3f" -> 1.234; "%5.2f secs" -> 01.23 secs; "Biscuit: %.0f" -> Biscuit: 1; etc.
bool SliderFloat(const char* label, float* v, float v_min, float v_max, const char* format, float power)
{
	return ImGui::SliderFloat(label, v, v_min, v_max, format, power);
}
bool SliderFloat2(const char* label, float v[2], float v_min, float v_max, const char* format, float power)
{
	return ImGui::SliderFloat2(label, v, v_min, v_max, format, power);
}
bool SliderFloat3(const char* label, float v[3], float v_min, float v_max, const char* format, float power)
{
	return ImGui::SliderFloat3(label, v, v_min, v_max, format, power);
}
bool SliderFloat4(const char* label, float v[4], float v_min, float v_max, const char* format, float power)
{
	return ImGui::SliderFloat4(label, v, v_min, v_max, format, power);
}
bool SliderAngle(const char* label, float* v_rad, float v_degrees_min, float v_degrees_max, const char* format)
{
	return ImGui::SliderAngle(label, v_rad, v_degrees_min, v_degrees_max, format);
}
bool SliderInt(const char* label, int* v, int v_min, int v_max, const char* format)
{
	return ImGui::SliderInt(label, v, v_min, v_max, format);
}
bool SliderInt2(const char* label, int v[2], int v_min, int v_max, const char* format)
{
	return ImGui::SliderInt2(label, v, v_min, v_max, format);
}
bool SliderInt3(const char* label, int v[3], int v_min, int v_max, const char* format)
{
	return ImGui::SliderInt3(label, v, v_min, v_max, format);
}
bool SliderInt4(const char* label, int v[4], int v_min, int v_max, const char* format)
{
	return ImGui::SliderInt4(label, v, v_min, v_max, format);
}
bool SliderScalar(const char* label, DataType data_type, void* v, const void* v_min, const void* v_max, const char* format, float power)
{
	return ImGui::SliderScalar(label, data_type, v, v_min, v_max, format, power);
}
bool SliderScalarN(const char* label, DataType data_type, void* v, int components, const void* v_min, const void* v_max, const char* format, float power)
{
	return ImGui::SliderScalarN(label, data_type, v, components, v_min, v_max, format, power);
}
bool VSliderFloat(const char* label, const Vector2F& size, float* v, float v_min, float v_max, const char* format, float power)
{
	return ImGui::VSliderFloat(label, ToImVal(size), v, v_min, v_max, format, power);
}
bool VSliderInt(const char* label, const Vector2F& size, int* v, int v_min, int v_max, const char* format)
{
	return ImGui::VSliderInt(label, ToImVal(size), v, v_min, v_max, format);
}
bool VSliderScalar(const char* label, const Vector2F& size, DataType data_type, void* v, const void* v_min, const void* v_max, const char* format, float power)
{
	return ImGui::VSliderScalar(label, ToImVal(size), data_type, v, v_min, v_max, format, power);
}

// Widgets: Input with Keyboard
// - If you want to use InputText() with a dynamic string type such as std::string or your own, see misc/cpp/imgui_stdlib.h
// - Most of the InputTextFlags flags are only useful for InputText() and not for InputFloatX, InputIntX, InputDouble etc.
bool InputText(const char* label, char* buf, size_t buf_size, InputTextFlags flags, ImGuiInputTextCallback callback, void* user_data)
{
	return ImGui::InputText(label, buf, buf_size, flags, callback, user_data);
}
bool InputTextMultiline(const char* label, char* buf, size_t buf_size, const Vector2F& size, InputTextFlags flags, ImGuiInputTextCallback callback, void* user_data)
{
	return ImGui::InputTextMultiline(label, buf, buf_size, ToImVal(size), flags, callback, user_data);
}
bool InputTextWithHint(const char* label, const char* hint, char* buf, size_t buf_size, InputTextFlags flags, ImGuiInputTextCallback callback, void* user_data)
{
	return ImGui::InputTextWithHint(label, hint, buf, buf_size, flags, callback, user_data);
}
bool InputFloat(const char* label, float* v, float step, float step_fast, const char* format, InputTextFlags flags)
{
	return ImGui::InputFloat(label, v, step, step_fast, format, flags);
}
bool InputFloat2(const char* label, float v[2], const char* format, InputTextFlags flags)
{
	return ImGui::InputFloat2(label, v, format, flags);
}
bool InputFloat3(const char* label, float v[3], const char* format, InputTextFlags flags)
{
	return ImGui::InputFloat3(label, v, format, flags);
}
bool InputFloat4(const char* label, float v[4], const char* format, InputTextFlags flags)
{
	return ImGui::InputFloat4(label, v, format, flags);
}
bool InputInt(const char* label, int* v, int step, int step_fast, InputTextFlags flags)
{
	return ImGui::InputInt(label, v, step, step_fast, flags);
}
bool InputInt2(const char* label, int v[2], InputTextFlags flags)
{
	return ImGui::InputInt2(label, v, flags);
}
bool InputInt3(const char* label, int v[3], InputTextFlags flags)
{
	return ImGui::InputInt3(label, v, flags);
}
bool InputInt4(const char* label, int v[4], InputTextFlags flags)
{
	return ImGui::InputInt4(label, v, flags);
}
bool InputDouble(const char* label, double* v, double step, double step_fast, const char* format, InputTextFlags flags)
{
	return ImGui::InputDouble(label, v, step, step_fast, format, flags);
}
bool InputScalar(const char* label, DataType data_type, void* v, const void* step, const void* step_fast, const char* format, InputTextFlags flags)
{
	return ImGui::InputScalar(label, data_type, v, step, step_fast, format, flags);
}
bool InputScalarN(const char* label, DataType data_type, void* v, int components, const void* step, const void* step_fast, const char* format, InputTextFlags flags)
{
	return ImGui::InputScalarN(label, data_type, v, components, step, step_fast, format, flags);
}

// Widgets: Color Editor/Picker (tip: the ColorEdit* functions have a little colored preview square that can be left-clicked to open a picker, and right-clicked to open an option menu.)
// - Note that in C++ a 'float v[X]' function argument is the _same_ as 'float* v', the array syntax is just a way to document the number of elements that are expected to be accessible.
// - You can pass the address of a first float element out of a contiguous structure, e.g. &myvector.x
bool ColorEdit3(const char* label, float col[3], ColorEditFlags flags)
{
	return ImGui::ColorEdit3(label, col, flags);
}
bool ColorEdit4(const char* label, float col[4], ColorEditFlags flags)
{
	return ImGui::ColorEdit4(label, col, flags);
}
bool ColorPicker3(const char* label, float col[3], ColorEditFlags flags)
{
	return ImGui::ColorPicker3(label, col, flags);
}
bool ColorPicker4(const char* label, float col[4], ColorEditFlags flags, const float* ref_col)
{
	return ImGui::ColorPicker4(label, col, flags, ref_col);
}
bool ColorButton(const char* desc_id, const Vector4F& col, ColorEditFlags flags, Vector2F size)
{
	return ImGui::ColorButton(desc_id, ToImVal(col), flags, ToImVal(size));
}
void SetColorEditOptions(ColorEditFlags flags)
{
	ImGui::SetColorEditOptions(flags);
}

// Widgets: Trees
// - TreeNode functions return true when the node is open, in which case you need to also call TreePop() when you are finished displaying the tree node contents.
bool TreeNode(const char* label)
{
	return ImGui::TreeNode(label);
}
bool TreeNode(const char* str_id, const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	bool is_open = TreeNodeExV(str_id, 0, fmt, args);
	va_end(args);
	return is_open;
}
bool TreeNode(const void* ptr_id, const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	bool is_open = TreeNodeExV(ptr_id, 0, fmt, args);
	va_end(args);
	return is_open;
}
bool TreeNodeV(const char* str_id, const char* fmt, va_list args)
{
	return ImGui::TreeNodeV(str_id, fmt, args);
}
bool TreeNodeV(const void* ptr_id, const char* fmt, va_list args)
{
	return TreeNodeV(ptr_id, fmt, args);
}
bool TreeNodeEx(const char* label, TreeNodeFlags flags)
{
	return ImGui::TreeNodeEx(label, flags);
}
bool TreeNodeEx(const char* str_id, TreeNodeFlags flags, const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	bool is_open = TreeNodeExV(str_id, flags, fmt, args);
	va_end(args);
	return is_open;
}
bool TreeNodeEx(const void* ptr_id, TreeNodeFlags flags, const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	bool is_open = TreeNodeExV(ptr_id, flags, fmt, args);
	va_end(args);
	return is_open;
}
bool TreeNodeExV(const char* str_id, TreeNodeFlags flags, const char* fmt, va_list args)
{
	return ImGui::TreeNodeExV(str_id, flags, fmt, args);
}
bool TreeNodeExV(const void* ptr_id, TreeNodeFlags flags, const char* fmt, va_list args)
{
	return ImGui::TreeNodeExV(ptr_id, flags, fmt, args);
}
void TreePush(const char* str_id)
{
	ImGui::TreePush(str_id);
}
void TreePush(const void* ptr_id)
{
	ImGui::TreePush(ptr_id);
}
void TreePop()
{
	ImGui::TreePop();
}
void TreeAdvanceToLabelPos()
{
	ImGui::TreeAdvanceToLabelPos();
}
float GetTreeNodeToLabelSpacing()
{
	return ImGui::GetTreeNodeToLabelSpacing();
}
bool CollapsingHeader(const char* label, TreeNodeFlags flags)
{
	return ImGui::CollapsingHeader(label, flags);
}
bool CollapsingHeader(const char* label, bool* p_open, TreeNodeFlags flags)
{
	return ImGui::CollapsingHeader(label, p_open, flags);
}
void SetNextItemOpen(bool is_open, Cond cond)
{
	ImGui::SetNextItemOpen(is_open, cond);
}

// Widgets: Selectables
// - A selectable highlights when hovered, and can display another color when selected.
// - Neighbors selectable extend their highlight bounds in order to leave no gap between them.
bool Selectable(const char* label, bool selected, SelectableFlags flags, const Vector2F& size)
{
	return ImGui::Selectable(label, selected, flags, ToImVal(size));
}
bool Selectable(const char* label, bool* p_selected, SelectableFlags flags, const Vector2F& size)
{
	return ImGui::Selectable(label, p_selected, flags, ToImVal(size));
}

// Widgets: TList Boxes
// - FIXME: To be consistent with all the newer API, ListBoxHeader/ListBoxFooter should in reality be called BeginListBox/EndListBox. Will rename them.
bool ListBox(const char* label, int* current_item, const char* const items[], int items_count, int height_in_items)
{
	return ImGui::ListBox(label, current_item, items, items_count, height_in_items);
}
bool ListBox(const char* label, int* current_item, bool (*items_getter)(void* data, int idx, const char** out_text), void* data, int items_count, int height_in_items)
{
	return ImGui::ListBox(label, current_item, items_getter, data, items_count, height_in_items);
}
bool ListBoxHeader(const char* label, const Vector2F& size)
{
	return ImGui::ListBoxHeader(label, ToImVal(size));
}
bool ListBoxHeader(const char* label, int items_count, int height_in_items)
{
	return ImGui::ListBoxHeader(label, items_count, height_in_items);
}
void ListBoxFooter() 
{
	ImGui::ListBoxFooter();
}

// Widgets: Data Plotting
void PlotLines(const char* label, const float* values, int values_count, int values_offset, const char* overlay_text, float scale_min, float scale_max, Vector2F graph_size, int stride)
{
	ImGui::PlotLines(label, values, values_count, values_offset, overlay_text, scale_min, scale_max, ToImVal(graph_size), stride);
} 
void PlotLines(const char* label, float(*values_getter)(void* data, int idx), void* data, int values_count, int values_offset, const char* overlay_text, float scale_min, float scale_max, Vector2F graph_size)
{
	ImGui::PlotLines(label, values_getter, data, values_count, values_offset, overlay_text, scale_min, scale_max, ToImVal(graph_size));
}
void PlotHistogram(const char* label, const float* values, int values_count, int values_offset, const char* overlay_text, float scale_min, float scale_max, Vector2F graph_size, int stride)
{
	ImGui::PlotHistogram(label, values, values_count, values_offset, overlay_text, scale_min, scale_max, ToImVal(graph_size), stride);
}
void PlotHistogram(const char* label, float(*values_getter)(void* data, int idx), void* data, int values_count, int values_offset, const char* overlay_text, float scale_min, float scale_max, Vector2F graph_size)
{
	ImGui::PlotHistogram(label, values_getter, data, values_count, values_offset, overlay_text, scale_min, scale_max, ToImVal(graph_size));
}

// Widgets: Value() Helpers.
// - Those are merely shortcut to calling Text() with a format string. Output single value in "name: value" format (tip: freely declare more in your code to handle your types. you can add functions to the ImGui namespace)
void Value(const char* prefix, bool b)
{
	ImGui::Value(prefix, b);
}
void Value(const char* prefix, int v)
{
	ImGui::Value(prefix, v);
}
void Value(const char* prefix, unsigned int v)
{
	ImGui::Value(prefix, v);
}
void Value(const char* prefix, float v, const char* float_format)
{
	ImGui::Value(prefix, v, float_format);
}

// Widgets: Menus
bool BeginMainMenuBar()
{
	return ImGui::BeginMainMenuBar();
}
void EndMainMenuBar()
{
	ImGui::EndMainMenuBar();
}
bool BeginMenuBar()
{
	return ImGui::BeginMenuBar();
}
void EndMenuBar()
{
	ImGui::EndMenuBar();
}
bool BeginMenu(const char* label, bool enabled)
{
	return ImGui::BeginMenu(label, enabled);
}
void EndMenu()
{
	ImGui::EndMenu();
}
bool MenuItem(const char* label, const char* shortcut, bool selected, bool enabled)
{
	return ImGui::MenuItem(label, shortcut, selected, enabled);
}
bool MenuItem(const char* label, const char* shortcut, bool* p_selected, bool enabled)
{
	return ImGui::MenuItem(label, shortcut, p_selected, enabled);
}

// Tooltips
void BeginTooltip()
{
	ImGui::BeginTooltip();
}
void EndTooltip()
{
	ImGui::EndTooltip();
}
void SetTooltip(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	SetTooltipV(fmt, args);
	va_end(args);
}
void SetTooltipV(const char* fmt, va_list args)
{
	ImGui::SetTooltipV(fmt, args);
}

// Popups, Modals
// The properties of popups windows are:
// - They block normal mouse hovering detection outside them. (*)
// - Unless modal, they can be closed by clicking anywhere outside them, or by pressing ESCAPE.
// - Their visibility state (~bool) is held internally by imgui instead of being held by the programmer as we are used to with regular Begin() calls.
//   User can manipulate the visibility state by calling OpenPopup().
// (*) One can use IsItemHovered(HoveredFlags_AllowWhenBlockedByPopup) to bypass it and detect hovering even when normally blocked by a popup.
// Those three properties are connected. The library needs to hold their visibility state because it can close popups at any time.
void OpenPopup(const char* str_id)
{
	ImGui::OpenPopup(str_id);
}
bool BeginPopup(const char* str_id, WindowFlags flags)
{
	return ImGui::BeginPopup(str_id, flags);
}
bool BeginPopupContextItem(const char* str_id, int mouse_button)
{
	return ImGui::BeginPopupContextItem(str_id, mouse_button);
}
bool BeginPopupContextWindow(const char* str_id, int mouse_button, bool also_over_items)
{
	return ImGui::BeginPopupContextWindow(str_id, mouse_button, also_over_items);
}
bool BeginPopupContextVoid(const char* str_id, int mouse_button)
{
	return 	ImGui::BeginPopupContextVoid(str_id, mouse_button);
}
bool BeginPopupModal(const char* name, bool* p_open, WindowFlags flags)
{
	return ImGui::BeginPopupModal(name, p_open, flags);
}
void EndPopup()
{
	ImGui::EndPopup();
}
bool OpenPopupOnItemClick(const char* str_id, int mouse_button)
{
	return ImGui::OpenPopupOnItemClick(str_id, mouse_button);
}
bool IsPopupOpen(const char* str_id)
{
	return ImGui::IsPopupOpen(str_id);
}
void CloseCurrentPopup()
{
	ImGui::CloseCurrentPopup();
}

// Columns
// - You can also use SameLine(pos_x) to mimic simplified columns.
// - The columns API is work-in-progress and rather lacking (columns are arguably the worst part of dear imgui at the moment!)
void Columns(int count, const char* id, bool border)
{
	ImGui::Columns(count, id, border);
}
void NextColumn()
{
	ImGui::NextColumn();
}
int  GetColumnIndex()
{
	return ImGui::GetColumnIndex();
}
float GetColumnWidth(int column_index)
{
	return ImGui::GetColumnWidth(column_index);
}
void SetColumnWidth(int column_index, float width)
{
	ImGui::SetColumnWidth(column_index, width);
}
float GetColumnOffset(int column_index)
{
	return ImGui::GetColumnOffset(column_index);
}
void SetColumnOffset(int column_index, float offset_x)
{
	ImGui::SetColumnOffset(column_index, offset_x);
}
int  GetColumnsCount()
{
	return ImGui::GetColumnsCount();
}

// Tab Bars, Tabs
// [BETA API] API may evolve!
bool BeginTabBar(const char* str_id, TabBarFlags flags)
{
	return 	ImGui::BeginTabBar(str_id, flags);
}
void EndTabBar()
{
	ImGui::EndTabBar();
}
bool BeginTabItem(const char* label, bool* p_open, ImGuiTabItemFlags flags)
{
	return ImGui::BeginTabItem(label, p_open, flags);
}
void EndTabItem()
{
	ImGui::EndTabItem();
}
void SetTabItemClosed(const char* tab_or_docked_window_label)
{
	ImGui::SetTabItemClosed(tab_or_docked_window_label);
}

// Logging/Capture
// - All text output from the interface can be captured into tty/file/clipboard. By default, tree nodes are automatically opened during logging.
void LogToTTY(int auto_open_depth)
{
	ImGui::LogToTTY(auto_open_depth);
}
void LogToFile(int auto_open_depth, const char* filename)
{
	ImGui::LogToFile(auto_open_depth, filename);
}
void LogToClipboard(int auto_open_depth)
{
	ImGui::LogToClipboard(auto_open_depth);
}
void LogFinish()
{
	ImGui::LogFinish();
}
void LogButtons()
{
	ImGui::LogButtons();
}
void LogText(const char* fmt, ...)
{
	ImGuiContext& g = *ImGui::GetCurrentContext();
	if (!g.LogEnabled)
		return;

	va_list args;
	va_start(args, fmt);
	if (g.LogFile)
		vfprintf(g.LogFile, fmt, args);
	else
		g.LogBuffer.appendfv(fmt, args);
	va_end(args);
}

// Drag and Drop
// [BETA API] API may evolve!
bool BeginDragDropSource(DragDropFlags flags)
{
	return ImGui::BeginDragDropSource(flags);
}
bool SetDragDropPayload(const char* type, const void* data, size_t sz, Cond cond)
{
	return ImGui::SetDragDropPayload(type, data, sz, cond);
}
void EndDragDropSource()
{
	ImGui::EndDragDropSource();
}
bool BeginDragDropTarget()
{
	return ImGui::BeginDragDropTarget();
}
const Payload* AcceptDragDropPayload(const char* type, DragDropFlags flags)
{
	return ImGui::AcceptDragDropPayload(type, flags);
}
void EndDragDropTarget()
{
	ImGui::EndDragDropTarget();
}
const Payload* GetDragDropPayload()
{
	return 	ImGui::GetDragDropPayload();
}

// Clipping
void PushClipRect(const Vector2F& clip_rect_min, const Vector2F& clip_rect_max, bool intersect_with_current_clip_rect)
{
	ImGui::PushClipRect(ToImVal(clip_rect_min), ToImVal(clip_rect_max), intersect_with_current_clip_rect);
}
void PopClipRect()
{
	ImGui::PopClipRect();
}

// Focus, Activation
// - Prefer using "SetItemDefaultFocus()" over "if (IsWindowAppearing()) SetScrollHereY()" when applicable to signify "this is the default item"
void SetItemDefaultFocus()
{
	ImGui::SetItemDefaultFocus();
}
void SetKeyboardFocusHere(int offset)
{
	ImGui::SetKeyboardFocusHere(offset);
}

// Item/Widgets Utilities
// - Most of the functions are referring to the last/previous item we submitted.
// - See Demo Window under "Widgets->Querying Status" for an interactive visualization of most of those functions.
bool IsItemHovered(HoveredFlags flags)
{
	return ImGui::IsItemHovered(flags);
}
bool IsItemActive()
{
	return ImGui::IsItemActive();
}
bool IsItemFocused()
{
	return ImGui::IsItemFocused();
}
bool IsItemClicked(int mouse_button)
{
	return ImGui::IsItemClicked(mouse_button);
}
bool IsItemVisible()
{
	return ImGui::IsItemVisible();
}
bool IsItemEdited()
{
	return ImGui::IsItemEdited();
}
bool IsItemActivated()
{
	return ImGui::IsItemActivated();
}
bool IsItemDeactivated()
{
	return ImGui::IsItemDeactivated();
}
bool IsItemDeactivatedAfterEdit()
{
	return ImGui::IsItemDeactivatedAfterEdit();
}
bool IsAnyItemHovered()
{
	return ImGui::IsAnyItemHovered();
}
bool IsAnyItemActive()
{
	return ImGui::IsAnyItemActive();
}
bool IsAnyItemFocused()
{
	return ImGui::IsAnyItemFocused();
}
Vector2F GetItemRectMin()
{
	return ImToVal(ImGui::GetItemRectMin());
}
Vector2F GetItemRectMax()
{
	return ImToVal(ImGui::GetItemRectMax());
}
Vector2F GetItemRectSize()
{
	return ImToVal(ImGui::GetItemRectSize());
}
void SetItemAllowOverlap()
{
	ImGui::SetItemAllowOverlap();
}

// Miscellaneous Utilities
bool IsRectVisible(const Vector2F& size)
{
	return ImGui::IsRectVisible(ToImVal(size));
}
bool IsRectVisible(const Vector2F& rect_min, const Vector2F& rect_max)
{
	return ImGui::IsRectVisible(ToImVal(rect_min), ToImVal(rect_max));
}
double GetTime()
{
	return ImGui::GetTime();
}
int  GetFrameCount()
{
	return ImGui::GetFrameCount();
}
DrawList* GetBackgroundDrawList()
{
	return ImGui::GetBackgroundDrawList();
}
DrawList* GetForegroundDrawList()
{
	return ImGui::GetForegroundDrawList();
}
DrawListSharedData* GetDrawListSharedData()
{
	return ImGui::GetDrawListSharedData();
}
const char* GetStyleColorName(Col idx)
{
	return ImGui::GetStyleColorName(idx);
}
void SetStateStorage(Storage* storage)
{
	ImGui::SetStateStorage(storage);
}
Storage* GetStateStorage()
{
	return ImGui::GetStateStorage();
}
Vector2F CalcTextSize(const char* text, const char* text_end, bool hide_text_after_double_hash, float wrap_width)
{
	return ImToVal(ImGui::CalcTextSize(text, text_end, hide_text_after_double_hash, wrap_width));
}
void CalcListClipping(int items_count, float items_height, int* out_items_display_start, int* out_items_display_end)
{
	ImGui::CalcListClipping(items_count, items_height, out_items_display_start, out_items_display_end);
}
bool BeginChildFrame(ID id, const Vector2F& size, WindowFlags flags)
{
	return ImGui::BeginChildFrame(id, ToImVal(size), flags);
}
void EndChildFrame()
{
	ImGui::EndChildFrame();
}

// Color Utilities
Vector4F ColorConvertU32ToFloat4(unsigned in)
{
	return ImToVal(ImGui::ColorConvertU32ToFloat4(in));
}
unsigned ColorConvertFloat4ToU32(const Vector4F& in)
{
	return ImGui::ColorConvertFloat4ToU32(ToImVal(in));
}
void ColorConvertRGBtoHSV(float r, float g, float b, float& out_h, float& out_s, float& out_v)
{
	ImGui::ColorConvertRGBtoHSV(r, g, b, out_h, out_s, out_v);
}
void ColorConvertHSVtoRGB(float h, float s, float v, float& out_r, float& out_g, float& out_b)
{
	ImGui::ColorConvertHSVtoRGB(h, s, v, out_r, out_g, out_b);
}

// Inputs Utilities
int  GetKeyIndex(Key imgui_key)
{
	return ImGui::GetKeyIndex(imgui_key);
}
bool IsKeyDown(int user_key_index)
{
	return ImGui::IsKeyDown(user_key_index);
}
bool IsKeyPressed(int user_key_index, bool repeat)
{
	return ImGui::IsKeyPressed(user_key_index, repeat);
}
bool IsKeyReleased(int user_key_index)
{
	return ImGui::IsKeyReleased(user_key_index);
}
int  GetKeyPressedAmount(int key_index, float repeat_delay, float rate)
{
	return ImGui::GetKeyPressedAmount(key_index, repeat_delay, rate);
}
bool IsMouseDown(int button)
{
	return ImGui::IsMouseDown(button);
}
bool IsAnyMouseDown()
{
	return ImGui::IsAnyMouseDown();
}
bool IsMouseClicked(int button, bool repeat)
{
	return ImGui::IsMouseClicked(button, repeat);
}
bool IsMouseDoubleClicked(int button)
{
	return ImGui::IsMouseDoubleClicked(button);
}
bool IsMouseReleased(int button)
{
	return ImGui::IsMouseReleased(button);
}
bool IsMouseDragging(int button, float lock_threshold)
{
	return ImGui::IsMouseDragging(button, lock_threshold);
}
bool IsMouseHoveringRect(const Vector2F& r_min, const Vector2F& r_max, bool clip)
{
	return ImGui::IsMouseHoveringRect(ToImVal(r_min), ToImVal(r_max), clip);
}
bool IsMousePosValid(const Vector2F* mouse_pos )
{
	return ImGui::IsMousePosValid(ToImVal(mouse_pos));
}
Vector2F GetMousePos()
{
	return ImToVal(ImGui::GetMousePos());
}
Vector2F GetMousePosOnOpeningCurrentPopup()
{
	return ImToVal(ImGui::GetMousePosOnOpeningCurrentPopup());
}
Vector2F GetMouseDragDelta(int button, float lock_threshold)
{
	return ImToVal(ImGui::GetMouseDragDelta(button, lock_threshold));
}
void ResetMouseDragDelta(int button)
{
	ImGui::ResetMouseDragDelta(button);
}
MouseCursor GetMouseCursor()
{
	return ImGui::GetMouseCursor();
}
void SetMouseCursor(MouseCursor type)
{
	ImGui::SetMouseCursor(type);
}
void CaptureKeyboardFromApp(bool want_capture_keyboard_value)
{
	ImGui::CaptureKeyboardFromApp(want_capture_keyboard_value);
}
void CaptureMouseFromApp(bool want_capture_mouse_value )
{
	ImGui::CaptureMouseFromApp(want_capture_mouse_value);
} 
const char* GetClipboardText()
{
	return ImGui::GetClipboardText();
}
void SetClipboardText(const char* text)
{
	ImGui::SetClipboardText(text);
}
void LoadIniSettingsFromDisk(const char* ini_filename)
{
	ImGui::LoadIniSettingsFromDisk(ini_filename);
}
void LoadIniSettingsFromMemory(const char* ini_data, size_t ini_size )
{
	ImGui::LoadIniSettingsFromMemory(ini_data, ini_size);
}
void SaveIniSettingsToDisk(const char* ini_filename)
{
	ImGui::SaveIniSettingsToDisk(ini_filename);
}
const char* SaveIniSettingsToMemory(size_t* out_ini_size)
{
	return ImGui::SaveIniSettingsToMemory(out_ini_size);
}
void SetAllocatorFunctions(void* (*alloc_func)(size_t sz, void* user_data), void (*free_func)(void* ptr, void* user_data), void* user_data)
{
	ImGui::SetAllocatorFunctions(alloc_func, free_func, user_data);
}
void* MemAlloc(size_t size) 
{
	return ImGui::MemAlloc(size);
}
void MemFree(void* ptr) 
{
	ImGui::MemFree(ptr);
}

}

}