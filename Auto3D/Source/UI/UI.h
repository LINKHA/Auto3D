#pragma once
#include "../Base/GameManager.h"
#include "../Platform/Window.h"
#include "../Platform/Context.h"
#include "../Graphics/Texture.h"
#include "../Resource/Font.h"
#include "UIUtils.h"

#include <imgui.h>
#include <imgui_internal.h>
using SDL_Event =  union SDL_Event;

namespace Auto3D
{

namespace UIStyleColors
{
	const static Color Text = Color(1.00f, 1.00f, 1.00f, 1.00f);
}
struct UIFont
{
	static HashMap<String, const char*> Data;
	static int DefaultSize;
};
namespace UIFontLanguage
{
	enum Data
	{
		DEFAULT = 0,
		CN,
		CNF,
		JP,
		KR,
		THA,
		RUS,
		VIE
	};
}

//namespace UIFont
//{
//	const static char* default = "default";
//	const static char* standard = "standard";
//	const static char* standard_big = "standard_big";
//}

class AUTO_API UI : public BaseModule
{
	REGISTER_OBJECT_CLASS(UI, BaseModule)
public:
	/// Construct.
	UI();
	/// Destruct.
	~UI();

	/// Init UI state
#ifdef AUTO_OPENGL
	bool SetMode(Window* window, GraphicsContext* context);
#else
	bool SetMode(Window* window);
#endif
	/// New frame to draw ui
	bool BeginUI();
	/// Draw data to screen
	void Present();

	/// Add font from font.
	void AddFont(Font* font, int pixels = 24, String fontname = "Default", UIFontLanguage::Data languageType = UIFontLanguage::DEFAULT);

	/// Get gui IO.
	ImGuiIO& IO() { return ImGui::GetIO(); }
	/// Process event to windows
	void ProcessEvent(const SDL_Event* event);
private:
	/// Window this member is assigned in SetMode, make sure that SetMode calls this member later
	WeakPtr<Window> _window;
};

/// Register UI related object factories and attributes.
AUTO_API void RegisterUILibrary();

namespace GUI
{

using WindowFlags = ImGuiWindowFlags;
using FocusedFlags = ImGuiFocusedFlags;
using HoveredFlags = ImGuiHoveredFlags;
using Style = ImGuiStyle;
using Cond = ImGuiCond;
using ID = ImGuiID;
using Font = ImFont;
using DrawList = ImDrawList;
using DrawListSharedData = ImDrawListSharedData;
using MouseCursor = ImGuiMouseCursor;
using Storage = ImGuiStorage;
using Col = ImGuiCol;

using DragDropFlags = ImGuiDragDropFlags;
using Payload = ImGuiPayload;
using TabBarFlags = ImGuiTabBarFlags;
using SelectableFlags = ImGuiSelectableFlags;
using TreeNodeFlags = ImGuiTreeNodeFlags;
using InputTextFlags = ImGuiInputTextFlags;
using DataType = ImGuiDataType;
using ComboFlags = ImGuiComboFlags;
using StyleVar = ImGuiStyleVar;
using Dir = ImGuiDir;
using Key = ImGuiKey;
using ColorEditFlags = ImGuiColorEditFlags;


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
AUTO_API bool          Begin(const char* name, bool* p_open = NULL, WindowFlags flags = 0);
AUTO_API void          End();

// Child Windows
// - Use child windows to begin into a self-contained independent scrolling/clipping regions within a host window. Child windows can embed their own child.
// - For each independent axis of 'size': ==0.0f: use remaining host window size / >0.0f: fixed size / <0.0f: use remaining window size minus abs(size) / Each axis can use a different mode, e.g. Vector2F(0,400).
// - BeginChild() returns false to indicate the window is collapsed or fully clipped, so you may early out and omit submitting anything to the window.
//   Always call a matching EndChild() for each BeginChild() call, regardless of its return value [this is due to legacy reason and is inconsistent with most other functions such as BeginMenu/EndMenu, BeginPopup/EndPopup, etc. where the EndXXX call should only be called if the corresponding BeginXXX function returned true.]
AUTO_API bool          BeginChild(const char* str_id, const Vector2F& size = Vector2F(0, 0), bool border = false, WindowFlags flags = 0);
AUTO_API bool          BeginChild(ID id, const Vector2F& size = Vector2F(0, 0), bool border = false, WindowFlags flags = 0);
AUTO_API void          EndChild();

// Windows Utilities
// - "current window" = the window we are appending into while inside a Begin()/End() block. "next window" = next window we will Begin() into.
AUTO_API bool          IsWindowAppearing();
AUTO_API bool          IsWindowCollapsed();
AUTO_API bool          IsWindowFocused(FocusedFlags flags = 0);    // is current window focused? or its root/child, depending on flags. see flags for options.
AUTO_API bool          IsWindowHovered(HoveredFlags flags = 0);	   // is current window hovered (and typically: not blocked by a popup/modal)? see flags for options. NB: If you are trying to check whether your mouse should be dispatched to imgui or to your app, you should use the 'io.WantCaptureMouse' boolean for that! Please read the FAQ!
AUTO_API DrawList*	   GetWindowDrawList();                        // get draw list associated to the current window, to append your own drawing primitives
AUTO_API Vector2F      GetWindowPos();                             // get current window position in screen space (useful if you want to do your own drawing via the DrawList API)
AUTO_API Vector2F      GetWindowSize();                            // get current window size
AUTO_API float         GetWindowWidth();                           // get current window width (shortcut for GetWindowSize().x)
AUTO_API float         GetWindowHeight();                          // get current window height (shortcut for GetWindowSize().y)

// Prefer using SetNextXXX functions (before Begin) rather that SetXXX functions (after Begin).
AUTO_API void          SetNextWindowPos(const Vector2F& pos, Cond cond = 0, const Vector2F& pivot = Vector2F(0, 0)); // set next window position. call before Begin(). use pivot=(0.5f,0.5f) to center on given point, etc.
AUTO_API void          SetNextWindowSize(const Vector2F& size, Cond cond = 0);                  // set next window size. set axis to 0.0f to force an auto-fit on this axis. call before Begin()
AUTO_API void          SetNextWindowSizeConstraints(const Vector2F& size_min, const Vector2F& size_max, ImGuiSizeCallback custom_callback = NULL, void* custom_callback_data = NULL); // set next window size limits. use -1,-1 on either X/Y axis to preserve the current size. Sizes will be rounded down. Use callback to apply non-trivial programmatic constraints.
AUTO_API void          SetNextWindowContentSize(const Vector2F& size);                          // set next window content size (~ enforce the range of scrollbars). not including window decorations (title bar, menu bar, etc.). set an axis to 0.0f to leave it automatic. call before Begin()
AUTO_API void          SetNextWindowCollapsed(bool collapsed, Cond cond = 0);					// set next window collapsed state. call before Begin()
AUTO_API void          SetNextWindowFocus();                                                    // set next window to be focused / front-most. call before Begin()
AUTO_API void          SetNextWindowBgAlpha(float alpha);                                       // set next window background color alpha. helper to easily modify Col_WindowBg/ChildBg/PopupBg. you may also use WindowFlags_NoBackground.
AUTO_API void          SetWindowPos(const Vector2F& pos, Cond cond = 0);                        // (not recommended) set current window position - call within Begin()/End(). prefer using SetNextWindowPos(), as this may incur tearing and side-effects.
AUTO_API void          SetWindowSize(const Vector2F& size, Cond cond = 0);                      // (not recommended) set current window size - call within Begin()/End(). set to Vector2F(0,0) to force an auto-fit. prefer using SetNextWindowSize(), as this may incur tearing and minor side-effects.
AUTO_API void          SetWindowCollapsed(bool collapsed, Cond cond = 0);						// (not recommended) set current window collapsed state. prefer using SetNextWindowCollapsed().
AUTO_API void          SetWindowFocus();                                                        // (not recommended) set current window to be focused / front-most. prefer using SetNextWindowFocus().
AUTO_API void          SetWindowFontScale(float scale);                                         // set font scale. Adjust IO.FontGlobalScale if you want to scale all windows
AUTO_API void          SetWindowPos(const char* name, const Vector2F& pos, Cond cond = 0);      // set named window position.
AUTO_API void          SetWindowSize(const char* name, const Vector2F& size, Cond cond = 0);    // set named window size. set axis to 0.0f to force an auto-fit on this axis.
AUTO_API void          SetWindowCollapsed(const char* name, bool collapsed, Cond cond = 0);		// set named window collapsed state
AUTO_API void          SetWindowFocus(const char* name);                                        // set named window to be focused / front-most. use NULL to remove focus.

// Content region
// - Those functions are bound to be redesigned soon (they are confusing, incomplete and return values in local window coordinates which increases confusion)
AUTO_API Vector2F      GetContentRegionMax();                                          // current content boundaries (typically window boundaries including scrolling, or current column boundaries), in windows coordinates
AUTO_API Vector2F      GetContentRegionAvail();                                        // == GetContentRegionMax() - GetCursorPos()
AUTO_API Vector2F      GetWindowContentRegionMin();                                    // content boundaries min (roughly (0,0)-Scroll), in window coordinates
AUTO_API Vector2F      GetWindowContentRegionMax();                                    // content boundaries max (roughly (0,0)+Size-Scroll) where Size can be override with SetNextWindowContentSize(), in window coordinates
AUTO_API float         GetWindowContentRegionWidth();                                  //

// Windows Scrolling
AUTO_API float         GetScrollX();                                                   // get scrolling amount [0..GetScrollMaxX()]
AUTO_API float         GetScrollY();                                                   // get scrolling amount [0..GetScrollMaxY()]
AUTO_API float         GetScrollMaxX();                                                // get maximum scrolling amount ~~ ContentSize.X - WindowSize.X
AUTO_API float         GetScrollMaxY();                                                // get maximum scrolling amount ~~ ContentSize.Y - WindowSize.Y
AUTO_API void          SetScrollX(float scroll_x);                                     // set scrolling amount [0..GetScrollMaxX()]
AUTO_API void          SetScrollY(float scroll_y);                                     // set scrolling amount [0..GetScrollMaxY()]
AUTO_API void          SetScrollHereY(float center_y_ratio = 0.5f);                    // adjust scrolling amount to make current cursor position visible. center_y_ratio=0.0: top, 0.5: center, 1.0: bottom. When using to make a "default/current item" visible, consider using SetItemDefaultFocus() instead.
AUTO_API void          SetScrollFromPosY(float local_y, float center_y_ratio = 0.5f);  // adjust scrolling amount to make given position visible. Generally GetCursorStartPos() + offset to compute a valid position.

// Parameters stacks (shared)
AUTO_API void		   PushFont(const String& font);
AUTO_API void          PushFont(Font* font);                                           // use NULL as a shortcut to push default font
AUTO_API void          PopFont();
AUTO_API void          PushStyleColor(Col idx, unsigned col);
AUTO_API void          PushStyleColor(Col idx, const Vector4F& col);
AUTO_API void          PopStyleColor(int count = 1);
AUTO_API void          PushStyleVar(StyleVar idx, float val);
AUTO_API void          PushStyleVar(StyleVar idx, const Vector2F& val);
AUTO_API void          PopStyleVar(int count = 1);
AUTO_API const		   Vector4F& GetStyleColorVec4(Col idx);                           // retrieve style color as stored in ImGuiStyle structure. use to feed back into PushStyleColor(), otherwise use GetColorU32() to get style color with style alpha baked in.
AUTO_API Font*		   GetFont();													   // get current font
AUTO_API float         GetFontSize();                                                  // get current font size (= height in pixels) of current font with current scale applied
AUTO_API Vector2F      GetFontTexUvWhitePixel();                                       // get UV coordinate for a while pixel, useful to draw custom shapes via the DrawList API
AUTO_API unsigned      GetColorU32(Col idx, float alpha_mul = 1.0f);				   // retrieve given style color with style alpha applied and optional extra alpha multiplier
AUTO_API unsigned      GetColorU32(const Vector4F& col);                               // retrieve given color with style alpha applied
AUTO_API unsigned      GetColorU32(unsigned col);                                      // retrieve given color with style alpha applied

// Parameters stacks (current window)
AUTO_API void          PushItemWidth(float item_width);                                // set width of items for common large "item+label" widgets. >0.0f: width in pixels, <0.0f align xx pixels to the right of window (so -1.0f always align width to the right side). 0.0f = default to ~2/3 of windows width, 
AUTO_API void          PopItemWidth();
AUTO_API void          SetNextItemWidth(float item_width);                             // set width of the _next_ common large "item+label" widget. >0.0f: width in pixels, <0.0f align xx pixels to the right of window (so -1.0f always align width to the right side)
AUTO_API float         CalcItemWidth();                                                // width of item given pushed settings and current cursor position
AUTO_API void          PushTextWrapPos(float wrap_local_pos_x = 0.0f);                 // word-wrapping for Text*() commands. < 0.0f: no wrapping; 0.0f: wrap to end of window (or column); > 0.0f: wrap at 'wrap_pos_x' position in window local space
AUTO_API void          PopTextWrapPos();
AUTO_API void          PushAllowKeyboardFocus(bool allow_keyboard_focus);              // allow focusing using TAB/Shift-TAB, enabled by default but you can disable it for certain widgets
AUTO_API void          PopAllowKeyboardFocus();
AUTO_API void          PushButtonRepeat(bool repeat);                                  // in 'repeat' mode, Button*() functions return repeated true in a typematic manner (using io.KeyRepeatDelay/io.KeyRepeatRate setting). Note that you can call IsItemActive() after any Button() to tell if the button is held in the current frame.
AUTO_API void          PopButtonRepeat();

// Cursor / Layout
// - By "cursor" we mean the current output position.
// - The typical widget behavior is to output themselves at the current cursor position, then move the cursor one line down.
AUTO_API void          Separator();                                                    // separator, generally horizontal. inside a menu bar or in horizontal layout mode, this becomes a vertical separator.
AUTO_API void          SameLine(float offset_from_start_x = 0.0f, float spacing = -1.0f);  // call between widgets or groups to layout them horizontally. X position given in window coordinates.
AUTO_API void          NewLine();                                                      // undo a SameLine() or force a new line when in an horizontal-layout context.
AUTO_API void          Spacing();                                                      // add vertical spacing.
AUTO_API void          Dummy(const Vector2F& size);                                    // add a dummy item of given size. unlike InvisibleButton(), Dummy() won't take the mouse click or be navigable into.
AUTO_API void          Indent(float indent_w = 0.0f);                                  // move content position toward the right, by style.IndentSpacing or indent_w if != 0
AUTO_API void          Unindent(float indent_w = 0.0f);                                // move content position back to the left, by style.IndentSpacing or indent_w if != 0
AUTO_API void          BeginGroup();                                                   // lock horizontal starting position
AUTO_API void          EndGroup();                                                     // unlock horizontal starting position + capture the whole group bounding box into one "item" (so you can use IsItemHovered() or layout primitives such as SameLine() on whole group, etc.)
AUTO_API Vector2F      GetCursorPos();                                                 // cursor position in window coordinates (relative to window position)
AUTO_API float         GetCursorPosX();                                                //   (some functions are using window-relative coordinates, such as: GetCursorPos, GetCursorStartPos, GetContentRegionMax, GetWindowContentRegion* etc.
AUTO_API float         GetCursorPosY();                                                //    other functions such as GetCursorScreenPos or everything in DrawList::
AUTO_API void          SetCursorPos(const Vector2F& local_pos);                        //    are using the main, absolute coordinate system.
AUTO_API void          SetCursorPosX(float local_x);                                   //    GetWindowPos() + GetCursorPos() == GetCursorScreenPos() etc.)
AUTO_API void          SetCursorPosY(float local_y);                                   //
AUTO_API Vector2F      GetCursorStartPos();                                            // initial cursor position in window coordinates
AUTO_API Vector2F      GetCursorScreenPos();                                           // cursor position in absolute screen coordinates [0..io.DisplaySize] (useful to work with DrawList API)
AUTO_API void          SetCursorScreenPos(const Vector2F& pos);                        // cursor position in absolute screen coordinates [0..io.DisplaySize]
AUTO_API void          AlignTextToFramePadding();                                      // vertically align upcoming text baseline to FramePadding.y so that it will align properly to regularly framed items (call if you have text on a line before a framed item)
AUTO_API float         GetTextLineHeight();                                            // ~ FontSize
AUTO_API float         GetTextLineHeightWithSpacing();                                 // ~ FontSize + style.ItemSpacing.y (distance in pixels between 2 consecutive lines of text)
AUTO_API float         GetFrameHeight();                                               // ~ FontSize + style.FramePadding.y * 2
AUTO_API float         GetFrameHeightWithSpacing();                                    // ~ FontSize + style.FramePadding.y * 2 + style.ItemSpacing.y (distance in pixels between 2 consecutive lines of framed widgets)

// ID stack/scopes
// - Read the FAQ for more details about how ID are handled in dear imgui. If you are creating widgets in a loop you most
//   likely want to push a unique identifier (e.g. object pointer, loop index) to uniquely differentiate them.
// - The resulting ID are hashes of the entire stack.
// - You can also use the "Label##foobar" syntax within widget label to distinguish them from each others.
// - In this header file we use the "label"/"name" terminology to denote a string that will be displayed and used as an ID,
//   whereas "str_id" denote a string that is only used as an ID and not normally displayed.
AUTO_API void          PushID(const char* str_id);                                     // push string into the ID stack (will hash string).
AUTO_API void          PushID(const char* str_id_begin, const char* str_id_end);       // push string into the ID stack (will hash string).
AUTO_API void          PushID(const void* ptr_id);                                     // push pointer into the ID stack (will hash pointer).
AUTO_API void          PushID(int int_id);                                             // push integer into the ID stack (will hash integer).
AUTO_API void          PopID();                                                        // pop from the ID stack.
AUTO_API ID			   GetID(const char* str_id);                                      // calculate unique ID (hash of whole ID stack + given parameter). e.g. if you want to query into Storage yourself
AUTO_API ID			   GetID(const char* str_id_begin, const char* str_id_end);
AUTO_API ID			   GetID(const void* ptr_id);

// Widgets: Text
AUTO_API void          TextUnformatted(const char* text, const char* text_end = NULL);                // raw text without formatting. Roughly equivalent to Text("%s", text) but: A) doesn't require null terminated string if 'text_end' is specified, B) it's faster, no memory copy is done, no buffer size limits, recommended for long chunks of text.
AUTO_API void          Text(const char* fmt, ...)                                      IM_FMTARGS(1); // simple formatted text
AUTO_API void          TextV(const char* fmt, va_list args)                            IM_FMTLIST(1);
AUTO_API void          TextColored(const Vector4F& col, const char* fmt, ...)            IM_FMTARGS(2); // shortcut for PushStyleColor(Col_Text, col); Text(fmt, ...); PopStyleColor();
AUTO_API void          TextColoredV(const Vector4F& col, const char* fmt, va_list args)  IM_FMTLIST(2);
AUTO_API void          TextDisabled(const char* fmt, ...)                              IM_FMTARGS(1); // shortcut for PushStyleColor(Col_Text, style.Colors[Col_TextDisabled]); Text(fmt, ...); PopStyleColor();
AUTO_API void          TextDisabledV(const char* fmt, va_list args)                    IM_FMTLIST(1);
AUTO_API void          TextWrapped(const char* fmt, ...)                               IM_FMTARGS(1); // shortcut for PushTextWrapPos(0.0f); Text(fmt, ...); PopTextWrapPos();. Note that this won't work on an auto-resizing window if there's no other widgets to extend the window width, yoy may need to set a size using SetNextWindowSize().
AUTO_API void          TextWrappedV(const char* fmt, va_list args)                     IM_FMTLIST(1);
AUTO_API void          LabelText(const char* label, const char* fmt, ...)              IM_FMTARGS(2); // display text+label aligned the same way as value+label widgets
AUTO_API void          LabelTextV(const char* label, const char* fmt, va_list args)    IM_FMTLIST(2);
AUTO_API void          BulletText(const char* fmt, ...)                                IM_FMTARGS(1); // shortcut for Bullet()+Text()
AUTO_API void          BulletTextV(const char* fmt, va_list args)                      IM_FMTLIST(1);

// Widgets: Main
// - Most widgets return true when the value has been changed or when pressed/selected
AUTO_API bool          Button(const char* label, const Vector2F& size = Vector2F(0, 0));    // button
AUTO_API bool          SmallButton(const char* label);                                 // button with FramePadding=(0,0) to easily embed within text
AUTO_API bool          InvisibleButton(const char* str_id, const Vector2F& size);        // button behavior without the visuals, frequently useful to build custom behaviors using the public api (along with IsItemActive, IsItemHovered, etc.)
AUTO_API bool          ArrowButton(const char* str_id, Dir dir);                  // square button with an arrow shape
AUTO_API void          Image(Texture* user_texture, const Vector2F& size, const Vector2F& uv0 = Vector2F(0, 0), const Vector2F& uv1 = Vector2F(1, 1), const Vector4F& tint_col = Vector4F(1, 1, 1, 1), const Vector4F& border_col = Vector4F(0, 0, 0, 0));
AUTO_API bool          ImageButton(Texture* user_texture, const Vector2F& size, const Vector2F& uv0 = Vector2F(0, 0), const Vector2F& uv1 = Vector2F(1, 1), int frame_padding = -1, const Vector4F& bg_col = Vector4F(0, 0, 0, 0), const Vector4F& tint_col = Vector4F(1, 1, 1, 1));    // <0 frame_padding uses default frame padding settings. 0 for no padding
AUTO_API bool          Checkbox(const char* label, bool* v);
AUTO_API bool          CheckboxFlags(const char* label, unsigned int* flags, unsigned int flags_value);
AUTO_API bool          RadioButton(const char* label, bool active);                    // use with e.g. if (RadioButton("one", my_value==1)) { my_value = 1; }
AUTO_API bool          RadioButton(const char* label, int* v, int v_button);           // shortcut to handle the above pattern when value is an integer
AUTO_API void          ProgressBar(float fraction, const Vector2F& size_arg = Vector2F(-1, 0), const char* overlay = NULL);
AUTO_API void          Bullet();                                                       // draw a small circle and keep the cursor on the same line. advance cursor x position by GetTreeNodeToLabelSpacing(), same distance that TreeNode() uses

// Widgets: Combo Box
// - The new BeginCombo()/EndCombo() api allows you to manage your contents and selection state however you want it, by creating e.g. Selectable() items.
// - The old Combo() api are helpers over BeginCombo()/EndCombo() which are kept available for convenience purpose.
AUTO_API bool          BeginCombo(const char* label, const char* preview_value, ComboFlags flags = 0);
AUTO_API void          EndCombo(); // only call EndCombo() if BeginCombo() returns true!
AUTO_API bool          Combo(const char* label, int* current_item, const char* const items[], int items_count, int popup_max_height_in_items = -1);
AUTO_API bool          Combo(const char* label, int* current_item, const char* items_separated_by_zeros, int popup_max_height_in_items = -1);      // Separate items with \0 within a string, end item-list with \0\0. e.g. "One\0Two\0Three\0"
AUTO_API bool          Combo(const char* label, int* current_item, bool(*items_getter)(void* data, int idx, const char** out_text), void* data, int items_count, int popup_max_height_in_items = -1);

// Widgets: Drags
// - CTRL+Click on any drag box to turn them into an input box. Manually input values aren't clamped and can go off-bounds.
// - For all the Float2/Float3/Float4/Int2/Int3/Int4 versions of every functions, note that a 'float v[X]' function argument is the same as 'float* v', the array syntax is just a way to document the number of elements that are expected to be accessible. You can pass address of your first element out of a contiguous set, e.g. &myvector.x
// - Adjust format string to decorate the value with a prefix, a suffix, or adapt the editing and display precision e.g. "%.3f" -> 1.234; "%5.2f secs" -> 01.23 secs; "Biscuit: %.0f" -> Biscuit: 1; etc.
// - Speed are per-pixel of mouse movement (v_speed=0.2f: mouse needs to move by 5 pixels to increase value by 1). For gamepad/keyboard navigation, minimum speed is Max(v_speed, minimum_step_at_given_precision).
AUTO_API bool          DragFloat(const char* label, float* v, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", float power = 1.0f);     // If v_min >= v_max we have no bound
AUTO_API bool          DragFloat2(const char* label, float v[2], float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", float power = 1.0f);
AUTO_API bool          DragFloat3(const char* label, float v[3], float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", float power = 1.0f);
AUTO_API bool          DragFloat4(const char* label, float v[4], float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", float power = 1.0f);
AUTO_API bool          DragFloatRange2(const char* label, float* v_current_min, float* v_current_max, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", const char* format_max = NULL, float power = 1.0f);
AUTO_API bool          DragInt(const char* label, int* v, float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* format = "%d");                                       // If v_min >= v_max we have no bound
AUTO_API bool          DragInt2(const char* label, int v[2], float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* format = "%d");
AUTO_API bool          DragInt3(const char* label, int v[3], float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* format = "%d");
AUTO_API bool          DragInt4(const char* label, int v[4], float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* format = "%d");
AUTO_API bool          DragIntRange2(const char* label, int* v_current_min, int* v_current_max, float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* format = "%d", const char* format_max = NULL);
AUTO_API bool          DragScalar(const char* label, DataType data_type, void* v, float v_speed, const void* v_min = NULL, const void* v_max = NULL, const char* format = NULL, float power = 1.0f);
AUTO_API bool          DragScalarN(const char* label, DataType data_type, void* v, int components, float v_speed, const void* v_min = NULL, const void* v_max = NULL, const char* format = NULL, float power = 1.0f);

// Widgets: Sliders
// - CTRL+Click on any slider to turn them into an input box. Manually input values aren't clamped and can go off-bounds.
// - Adjust format string to decorate the value with a prefix, a suffix, or adapt the editing and display precision e.g. "%.3f" -> 1.234; "%5.2f secs" -> 01.23 secs; "Biscuit: %.0f" -> Biscuit: 1; etc.
AUTO_API bool          SliderFloat(const char* label, float* v, float v_min, float v_max, const char* format = "%.3f", float power = 1.0f);     // adjust format to decorate the value with a prefix or a suffix for in-slider labels or unit display. Use power!=1.0 for power curve sliders
AUTO_API bool          SliderFloat2(const char* label, float v[2], float v_min, float v_max, const char* format = "%.3f", float power = 1.0f);
AUTO_API bool          SliderFloat3(const char* label, float v[3], float v_min, float v_max, const char* format = "%.3f", float power = 1.0f);
AUTO_API bool          SliderFloat4(const char* label, float v[4], float v_min, float v_max, const char* format = "%.3f", float power = 1.0f);
AUTO_API bool          SliderAngle(const char* label, float* v_rad, float v_degrees_min = -360.0f, float v_degrees_max = +360.0f, const char* format = "%.0f deg");
AUTO_API bool          SliderInt(const char* label, int* v, int v_min, int v_max, const char* format = "%d");
AUTO_API bool          SliderInt2(const char* label, int v[2], int v_min, int v_max, const char* format = "%d");
AUTO_API bool          SliderInt3(const char* label, int v[3], int v_min, int v_max, const char* format = "%d");
AUTO_API bool          SliderInt4(const char* label, int v[4], int v_min, int v_max, const char* format = "%d");
AUTO_API bool          SliderScalar(const char* label, DataType data_type, void* v, const void* v_min, const void* v_max, const char* format = NULL, float power = 1.0f);
AUTO_API bool          SliderScalarN(const char* label, DataType data_type, void* v, int components, const void* v_min, const void* v_max, const char* format = NULL, float power = 1.0f);
AUTO_API bool          VSliderFloat(const char* label, const Vector2F& size, float* v, float v_min, float v_max, const char* format = "%.3f", float power = 1.0f);
AUTO_API bool          VSliderInt(const char* label, const Vector2F& size, int* v, int v_min, int v_max, const char* format = "%d");
AUTO_API bool          VSliderScalar(const char* label, const Vector2F& size, DataType data_type, void* v, const void* v_min, const void* v_max, const char* format = NULL, float power = 1.0f);

// Widgets: Input with Keyboard
// - If you want to use InputText() with a dynamic string type such as std::string or your own, see misc/cpp/imgui_stdlib.h
// - Most of the InputTextFlags flags are only useful for InputText() and not for InputFloatX, InputIntX, InputDouble etc.
AUTO_API bool          InputText(const char* label, char* buf, size_t buf_size, InputTextFlags flags = 0, ImGuiInputTextCallback callback = NULL, void* user_data = NULL);
AUTO_API bool          InputTextMultiline(const char* label, char* buf, size_t buf_size, const Vector2F& size = Vector2F(0, 0), InputTextFlags flags = 0, ImGuiInputTextCallback callback = NULL, void* user_data = NULL);
AUTO_API bool          InputTextWithHint(const char* label, const char* hint, char* buf, size_t buf_size, InputTextFlags flags = 0, ImGuiInputTextCallback callback = NULL, void* user_data = NULL);
AUTO_API bool          InputFloat(const char* label, float* v, float step = 0.0f, float step_fast = 0.0f, const char* format = "%.3f", InputTextFlags flags = 0);
AUTO_API bool          InputFloat2(const char* label, float v[2], const char* format = "%.3f", InputTextFlags flags = 0);
AUTO_API bool          InputFloat3(const char* label, float v[3], const char* format = "%.3f", InputTextFlags flags = 0);
AUTO_API bool          InputFloat4(const char* label, float v[4], const char* format = "%.3f", InputTextFlags flags = 0);
AUTO_API bool          InputInt(const char* label, int* v, int step = 1, int step_fast = 100, InputTextFlags flags = 0);
AUTO_API bool          InputInt2(const char* label, int v[2], InputTextFlags flags = 0);
AUTO_API bool          InputInt3(const char* label, int v[3], InputTextFlags flags = 0);
AUTO_API bool          InputInt4(const char* label, int v[4], InputTextFlags flags = 0);
AUTO_API bool          InputDouble(const char* label, double* v, double step = 0.0, double step_fast = 0.0, const char* format = "%.6f", InputTextFlags flags = 0);
AUTO_API bool          InputScalar(const char* label, DataType data_type, void* v, const void* step = NULL, const void* step_fast = NULL, const char* format = NULL, InputTextFlags flags = 0);
AUTO_API bool          InputScalarN(const char* label, DataType data_type, void* v, int components, const void* step = NULL, const void* step_fast = NULL, const char* format = NULL, InputTextFlags flags = 0);

// Widgets: Color Editor/Picker (tip: the ColorEdit* functions have a little colored preview square that can be left-clicked to open a picker, and right-clicked to open an option menu.)
// - Note that in C++ a 'float v[X]' function argument is the _same_ as 'float* v', the array syntax is just a way to document the number of elements that are expected to be accessible.
// - You can pass the address of a first float element out of a contiguous structure, e.g. &myvector.x
AUTO_API bool          ColorEdit3(const char* label, float col[3], ColorEditFlags flags = 0);
AUTO_API bool          ColorEdit4(const char* label, float col[4], ColorEditFlags flags = 0);
AUTO_API bool          ColorPicker3(const char* label, float col[3], ColorEditFlags flags = 0);
AUTO_API bool          ColorPicker4(const char* label, float col[4], ColorEditFlags flags = 0, const float* ref_col = NULL);
AUTO_API bool          ColorButton(const char* desc_id, const Vector4F& col, ColorEditFlags flags = 0, Vector2F size = Vector2F(0, 0));  // display a colored square/button, hover for details, return true when pressed.
AUTO_API void          SetColorEditOptions(ColorEditFlags flags);                     // initialize current options (generally on application startup) if you want to select a default format, picker type, etc. User will be able to change many settings, unless you pass the _NoOptions flag to your calls.

// Widgets: Trees
// - TreeNode functions return true when the node is open, in which case you need to also call TreePop() when you are finished displaying the tree node contents.
AUTO_API bool          TreeNode(const char* label);
AUTO_API bool          TreeNode(const char* str_id, const char* fmt, ...) IM_FMTARGS(2);   // helper variation to easily decorelate the id from the displayed string. Read the FAQ about why and how to use ID. to align arbitrary text at the same level as a TreeNode() you can use Bullet().
AUTO_API bool          TreeNode(const void* ptr_id, const char* fmt, ...) IM_FMTARGS(2);   // "
AUTO_API bool          TreeNodeV(const char* str_id, const char* fmt, va_list args) IM_FMTLIST(2);
AUTO_API bool          TreeNodeV(const void* ptr_id, const char* fmt, va_list args) IM_FMTLIST(2);
AUTO_API bool          TreeNodeEx(const char* label, TreeNodeFlags flags = 0);
AUTO_API bool          TreeNodeEx(const char* str_id, TreeNodeFlags flags, const char* fmt, ...) IM_FMTARGS(3);
AUTO_API bool          TreeNodeEx(const void* ptr_id, TreeNodeFlags flags, const char* fmt, ...) IM_FMTARGS(3);
AUTO_API bool          TreeNodeExV(const char* str_id, TreeNodeFlags flags, const char* fmt, va_list args) IM_FMTLIST(3);
AUTO_API bool          TreeNodeExV(const void* ptr_id, TreeNodeFlags flags, const char* fmt, va_list args) IM_FMTLIST(3);
AUTO_API void          TreePush(const char* str_id);                                       // ~ Indent()+PushId(). Already called by TreeNode() when returning true, but you can call TreePush/TreePop yourself if desired.
AUTO_API void          TreePush(const void* ptr_id = NULL);                                // "
AUTO_API void          TreePop();                                                          // ~ Unindent()+PopId()
AUTO_API void          TreeAdvanceToLabelPos();                                            // advance cursor x position by GetTreeNodeToLabelSpacing()
AUTO_API float         GetTreeNodeToLabelSpacing();                                        // horizontal distance preceding label when using TreeNode*() or Bullet() == (g.FontSize + style.FramePadding.x*2) for a regular unframed TreeNode
AUTO_API bool          CollapsingHeader(const char* label, TreeNodeFlags flags = 0);       // if returning 'true' the header is open. doesn't indent nor push on ID stack. user doesn't have to call TreePop().
AUTO_API bool          CollapsingHeader(const char* label, bool* p_open, TreeNodeFlags flags = 0); // when 'p_open' isn't NULL, display an additional small close button on upper right of the header
AUTO_API void          SetNextItemOpen(bool is_open, Cond cond = 0);					   // set next TreeNode/CollapsingHeader open state.

// Widgets: Selectables
// - A selectable highlights when hovered, and can display another color when selected.
// - Neighbors selectable extend their highlight bounds in order to leave no gap between them.
AUTO_API bool          Selectable(const char* label, bool selected = false, SelectableFlags flags = 0, const Vector2F& size = Vector2F(0, 0));  // "bool selected" carry the selection state (read-only). Selectable() is clicked is returns true so you can modify your selection state. size.x==0.0: use remaining width, size.x>0.0: specify width. size.y==0.0: use label height, size.y>0.0: specify height
AUTO_API bool          Selectable(const char* label, bool* p_selected, SelectableFlags flags = 0, const Vector2F& size = Vector2F(0, 0));       // "bool* p_selected" point to the selection state (read-write), as a convenient helper.

// Widgets: List Boxes
// - FIXME: To be consistent with all the newer API, ListBoxHeader/ListBoxFooter should in reality be called BeginListBox/EndListBox. Will rename them.
AUTO_API bool          ListBox(const char* label, int* current_item, const char* const items[], int items_count, int height_in_items = -1);
AUTO_API bool          ListBox(const char* label, int* current_item, bool (*items_getter)(void* data, int idx, const char** out_text), void* data, int items_count, int height_in_items = -1);
AUTO_API bool          ListBoxHeader(const char* label, const Vector2F& size = Vector2F(0, 0)); // use if you want to reimplement ListBox() will custom data or interactions. if the function return true, you can output elements then call ListBoxFooter() afterwards.
AUTO_API bool          ListBoxHeader(const char* label, int items_count, int height_in_items = -1); // "
AUTO_API void          ListBoxFooter();                                                    // terminate the scrolling region. only call ListBoxFooter() if ListBoxHeader() returned true!

// Widgets: Data Plotting
AUTO_API void          PlotLines(const char* label, const float* values, int values_count, int values_offset = 0, const char* overlay_text = NULL, float scale_min = FLT_MAX, float scale_max = FLT_MAX, Vector2F graph_size = Vector2F(0, 0), int stride = sizeof(float));
AUTO_API void          PlotLines(const char* label, float(*values_getter)(void* data, int idx), void* data, int values_count, int values_offset = 0, const char* overlay_text = NULL, float scale_min = FLT_MAX, float scale_max = FLT_MAX, Vector2F graph_size = Vector2F(0, 0));
AUTO_API void          PlotHistogram(const char* label, const float* values, int values_count, int values_offset = 0, const char* overlay_text = NULL, float scale_min = FLT_MAX, float scale_max = FLT_MAX, Vector2F graph_size = Vector2F(0, 0), int stride = sizeof(float));
AUTO_API void          PlotHistogram(const char* label, float(*values_getter)(void* data, int idx), void* data, int values_count, int values_offset = 0, const char* overlay_text = NULL, float scale_min = FLT_MAX, float scale_max = FLT_MAX, Vector2F graph_size = Vector2F(0, 0));

// Widgets: Value() Helpers.
// - Those are merely shortcut to calling Text() with a format string. Output single value in "name: value" format (tip: freely declare more in your code to handle your types. you can add functions to the ImGui namespace)
AUTO_API void          Value(const char* prefix, bool b);
AUTO_API void          Value(const char* prefix, int v);
AUTO_API void          Value(const char* prefix, unsigned int v);
AUTO_API void          Value(const char* prefix, float v, const char* float_format = NULL);

// Widgets: Menus
AUTO_API bool          BeginMainMenuBar();                                                 // create and append to a full screen menu-bar.
AUTO_API void          EndMainMenuBar();                                                   // only call EndMainMenuBar() if BeginMainMenuBar() returns true!
AUTO_API bool          BeginMenuBar();                                                     // append to menu-bar of current window (requires WindowFlags_MenuBar flag set on parent window).
AUTO_API void          EndMenuBar();                                                       // only call EndMenuBar() if BeginMenuBar() returns true!
AUTO_API bool          BeginMenu(const char* label, bool enabled = true);                  // create a sub-menu entry. only call EndMenu() if this returns true!
AUTO_API void          EndMenu();                                                          // only call EndMenu() if BeginMenu() returns true!
AUTO_API bool          MenuItem(const char* label, const char* shortcut = NULL, bool selected = false, bool enabled = true);  // return true when activated. shortcuts are displayed for convenience but not processed by ImGui at the moment
AUTO_API bool          MenuItem(const char* label, const char* shortcut, bool* p_selected, bool enabled = true);              // return true when activated + toggle (*p_selected) if p_selected != NULL

// Tooltips
AUTO_API void          BeginTooltip();                                                     // begin/append a tooltip window. to create full-featured tooltip (with any kind of items).
AUTO_API void          EndTooltip();
AUTO_API void          SetTooltip(const char* fmt, ...) IM_FMTARGS(1);                     // set a text-only tooltip, typically use with ImGui::IsItemHovered(). override any previous call to SetTooltip().
AUTO_API void          SetTooltipV(const char* fmt, va_list args) IM_FMTLIST(1);

// Popups, Modals
// The properties of popups windows are:
// - They block normal mouse hovering detection outside them. (*)
// - Unless modal, they can be closed by clicking anywhere outside them, or by pressing ESCAPE.
// - Their visibility state (~bool) is held internally by imgui instead of being held by the programmer as we are used to with regular Begin() calls.
//   User can manipulate the visibility state by calling OpenPopup().
// (*) One can use IsItemHovered(HoveredFlags_AllowWhenBlockedByPopup) to bypass it and detect hovering even when normally blocked by a popup.
// Those three properties are connected. The library needs to hold their visibility state because it can close popups at any time.
AUTO_API void          OpenPopup(const char* str_id);                                      // call to mark popup as open (don't call every frame!). popups are closed when user click outside, or if CloseCurrentPopup() is called within a BeginPopup()/EndPopup() block. By default, Selectable()/MenuItem() are calling CloseCurrentPopup(). Popup identifiers are relative to the current ID-stack (so OpenPopup and BeginPopup needs to be at the same level).
AUTO_API bool          BeginPopup(const char* str_id, WindowFlags flags = 0);                                                  // return true if the popup is open, and you can start outputting to it. only call EndPopup() if BeginPopup() returns true!
AUTO_API bool          BeginPopupContextItem(const char* str_id = NULL, int mouse_button = 1);                                 // helper to open and begin popup when clicked on last item. if you can pass a NULL str_id only if the previous item had an id. If you want to use that on a non-interactive item such as Text() you need to pass in an explicit ID here. read comments in .cpp!
AUTO_API bool          BeginPopupContextWindow(const char* str_id = NULL, int mouse_button = 1, bool also_over_items = true);  // helper to open and begin popup when clicked on current window.
AUTO_API bool          BeginPopupContextVoid(const char* str_id = NULL, int mouse_button = 1);                                 // helper to open and begin popup when clicked in void (where there are no imgui windows).
AUTO_API bool          BeginPopupModal(const char* name, bool* p_open = NULL, WindowFlags flags = 0);						   // modal dialog (regular window with title bar, block interactions behind the modal window, can't close the modal window by clicking outside)
AUTO_API void          EndPopup();                                                                                             // only call EndPopup() if BeginPopupXXX() returns true!
AUTO_API bool          OpenPopupOnItemClick(const char* str_id = NULL, int mouse_button = 1);                                  // helper to open popup when clicked on last item (note: actually triggers on the mouse _released_ event to be consistent with popup behaviors). return true when just opened.
AUTO_API bool          IsPopupOpen(const char* str_id);                                    // return true if the popup is open at the current begin-ed level of the popup stack.
AUTO_API void          CloseCurrentPopup();                                                // close the popup we have begin-ed into. clicking on a MenuItem or Selectable automatically close the current popup.

// Columns
// - You can also use SameLine(pos_x) to mimic simplified columns.
// - The columns API is work-in-progress and rather lacking (columns are arguably the worst part of dear imgui at the moment!)
AUTO_API void          Columns(int count = 1, const char* id = NULL, bool border = true);
AUTO_API void          NextColumn();                                                       // next column, defaults to current row or next row if the current row is finished
AUTO_API int           GetColumnIndex();                                                   // get current column index
AUTO_API float         GetColumnWidth(int column_index = -1);                              // get column width (in pixels). pass -1 to use current column
AUTO_API void          SetColumnWidth(int column_index, float width);                      // set column width (in pixels). pass -1 to use current column
AUTO_API float         GetColumnOffset(int column_index = -1);                             // get position of column line (in pixels, from the left side of the contents region). pass -1 to use current column, otherwise 0..GetColumnsCount() inclusive. column 0 is typically 0.0f
AUTO_API void          SetColumnOffset(int column_index, float offset_x);                  // set position of column line (in pixels, from the left side of the contents region). pass -1 to use current column
AUTO_API int           GetColumnsCount();

// Tab Bars, Tabs
// [BETA API] API may evolve!
AUTO_API bool          BeginTabBar(const char* str_id, TabBarFlags flags = 0);			   // create and append into a TabBar
AUTO_API void          EndTabBar();                                                        // only call EndTabBar() if BeginTabBar() returns true!
AUTO_API bool          BeginTabItem(const char* label, bool* p_open = NULL, ImGuiTabItemFlags flags = 0);// create a Tab. Returns true if the Tab is selected.
AUTO_API void          EndTabItem();                                                       // only call EndTabItem() if BeginTabItem() returns true!
AUTO_API void          SetTabItemClosed(const char* tab_or_docked_window_label);           // notify TabBar or Docking system of a closed tab/window ahead (useful to reduce visual flicker on reorderable tab bars). For tab-bar: call after BeginTabBar() and before Tab submissions. Otherwise call with a window name.

// Logging/Capture
// - All text output from the interface can be captured into tty/file/clipboard. By default, tree nodes are automatically opened during logging.
AUTO_API void          LogToTTY(int auto_open_depth = -1);                                 // start logging to tty (stdout)
AUTO_API void          LogToFile(int auto_open_depth = -1, const char* filename = NULL);   // start logging to file
AUTO_API void          LogToClipboard(int auto_open_depth = -1);                           // start logging to OS clipboard
AUTO_API void          LogFinish();                                                        // stop logging (close file, etc.)
AUTO_API void          LogButtons();                                                       // helper to display buttons for logging to tty/file/clipboard
AUTO_API void          LogText(const char* fmt, ...) IM_FMTARGS(1);                        // pass text data straight to log (without being displayed)

// Drag and Drop
// [BETA API] API may evolve!
AUTO_API bool          BeginDragDropSource(DragDropFlags flags = 0);                        // call when the current item is active. If this return true, you can call SetDragDropPayload() + EndDragDropSource()
AUTO_API bool          SetDragDropPayload(const char* type, const void* data, size_t sz, Cond cond = 0);  // type is a user defined string of maximum 32 characters. Strings starting with '_' are reserved for dear imgui internal types. Data is copied and held by imgui.
AUTO_API void          EndDragDropSource();                                                 // only call EndDragDropSource() if BeginDragDropSource() returns true!
AUTO_API bool          BeginDragDropTarget();                                               // call after submitting an item that may receive a payload. If this returns true, you can call AcceptDragDropPayload() + EndDragDropTarget()
AUTO_API const Payload* AcceptDragDropPayload(const char* type, DragDropFlags flags = 0);   // accept contents of a given type. If DragDropFlags_AcceptBeforeDelivery is set you can peek into the payload before the mouse button is released.
AUTO_API void          EndDragDropTarget();                                                 // only call EndDragDropTarget() if BeginDragDropTarget() returns true!
AUTO_API const Payload* GetDragDropPayload();                                               // peek directly into the current payload from anywhere. may return NULL. use Payload::IsDataType() to test for the payload type.

// Clipping
AUTO_API void          PushClipRect(const Vector2F& clip_rect_min, const Vector2F& clip_rect_max, bool intersect_with_current_clip_rect);
AUTO_API void          PopClipRect();

// Focus, Activation
// - Prefer using "SetItemDefaultFocus()" over "if (IsWindowAppearing()) SetScrollHereY()" when applicable to signify "this is the default item"
AUTO_API void          SetItemDefaultFocus();                                              // make last item the default focused item of a window.
AUTO_API void          SetKeyboardFocusHere(int offset = 0);                               // focus keyboard on the next widget. Use positive 'offset' to access sub components of a multiple component widget. Use -1 to access previous widget.

// Item/Widgets Utilities
// - Most of the functions are referring to the last/previous item we submitted.
// - See Demo Window under "Widgets->Querying Status" for an interactive visualization of most of those functions.
AUTO_API bool          IsItemHovered(HoveredFlags flags = 0);							   // is the last item hovered? (and usable, aka not blocked by a popup, etc.). See HoveredFlags for more options.
AUTO_API bool          IsItemActive();                                                     // is the last item active? (e.g. button being held, text field being edited. This will continuously return true while holding mouse button on an item. Items that don't interact will always return false)
AUTO_API bool          IsItemFocused();                                                    // is the last item focused for keyboard/gamepad navigation?
AUTO_API bool          IsItemClicked(int mouse_button = 0);                                // is the last item clicked? (e.g. button/node just clicked on) == IsMouseClicked(mouse_button) && IsItemHovered()
AUTO_API bool          IsItemVisible();                                                    // is the last item visible? (items may be out of sight because of clipping/scrolling)
AUTO_API bool          IsItemEdited();                                                     // did the last item modify its underlying value this frame? or was pressed? This is generally the same as the "bool" return value of many widgets.
AUTO_API bool          IsItemActivated();                                                  // was the last item just made active (item was previously inactive).
AUTO_API bool          IsItemDeactivated();                                                // was the last item just made inactive (item was previously active). Useful for Undo/Redo patterns with widgets that requires continuous editing.
AUTO_API bool          IsItemDeactivatedAfterEdit();                                       // was the last item just made inactive and made a value change when it was active? (e.g. Slider/Drag moved). Useful for Undo/Redo patterns with widgets that requires continuous editing. Note that you may get false positives (some widgets such as Combo()/ListBox()/Selectable() will return true even when clicking an already selected item).
AUTO_API bool          IsAnyItemHovered();                                                 // is any item hovered?
AUTO_API bool          IsAnyItemActive();                                                  // is any item active?
AUTO_API bool          IsAnyItemFocused();                                                 // is any item focused?
AUTO_API Vector2F      GetItemRectMin();                                                   // get upper-left bounding rectangle of the last item (screen space)
AUTO_API Vector2F      GetItemRectMax();                                                   // get lower-right bounding rectangle of the last item (screen space)
AUTO_API Vector2F      GetItemRectSize();                                                  // get size of last item
AUTO_API void          SetItemAllowOverlap();                                              // allow last item to be overlapped by a subsequent item. sometimes useful with invisible buttons, selectables, etc. to catch unused area.

// Miscellaneous Utilities
AUTO_API bool          IsRectVisible(const Vector2F& size);                                // test if rectangle (of given size, starting from cursor position) is visible / not clipped.
AUTO_API bool          IsRectVisible(const Vector2F& rect_min, const Vector2F& rect_max);  // test if rectangle (in screen space) is visible / not clipped. to perform coarse clipping on user's side.
AUTO_API double        GetTime();                                                          // get global imgui time. incremented by io.DeltaTime every frame.
AUTO_API int           GetFrameCount();                                                    // get global imgui frame count. incremented by 1 every frame.
AUTO_API DrawList*	   GetBackgroundDrawList();                                            // this draw list will be the first rendering one. Useful to quickly draw shapes/text behind dear imgui contents.
AUTO_API DrawList*	   GetForegroundDrawList();                                            // this draw list will be the last rendered one. Useful to quickly draw shapes/text over dear imgui contents.
AUTO_API DrawListSharedData* GetDrawListSharedData();									   // you may use this when creating your own DrawList instances.
AUTO_API const char*   GetStyleColorName(Col idx);										   // get a string corresponding to the enum value (for display, saving, etc.).
AUTO_API void          SetStateStorage(Storage* storage);								   // replace current window storage with our own (if you want to manipulate it yourself, typically clear subsection of it)
AUTO_API Storage*	   GetStateStorage();
AUTO_API Vector2F      CalcTextSize(const char* text, const char* text_end = NULL, bool hide_text_after_double_hash = false, float wrap_width = -1.0f);
AUTO_API void          CalcListClipping(int items_count, float items_height, int* out_items_display_start, int* out_items_display_end);    // calculate coarse clipping for large list of evenly sized items. Prefer using the ImGuiListClipper higher-level helper if you can.
AUTO_API bool          BeginChildFrame(ID id, const Vector2F& size, WindowFlags flags = 0); // helper to create a child window / scrolling region that looks like a normal widget frame
AUTO_API void          EndChildFrame();                                                    // always call EndChildFrame() regardless of BeginChildFrame() return values (which indicates a collapsed/clipped window)

// Color Utilities
AUTO_API Vector4F      ColorConvertU32ToFloat4(unsigned in);
AUTO_API unsigned      ColorConvertFloat4ToU32(const Vector4F& in);
AUTO_API void          ColorConvertRGBtoHSV(float r, float g, float b, float& out_h, float& out_s, float& out_v);
AUTO_API void          ColorConvertHSVtoRGB(float h, float s, float v, float& out_r, float& out_g, float& out_b);

// Inputs Utilities
AUTO_API int           GetKeyIndex(Key imgui_key);                                    // map Key_* values into user's key index. == io.KeyMap[key]
AUTO_API bool          IsKeyDown(int user_key_index);                                      // is key being held. == io.KeysDown[user_key_index]. note that imgui doesn't know the semantic of each entry of io.KeysDown[]. Use your own indices/enums according to how your backend/engine stored them into io.KeysDown[]!
AUTO_API bool          IsKeyPressed(int user_key_index, bool repeat = true);               // was key pressed (went from !Down to Down). if repeat=true, uses io.KeyRepeatDelay / KeyRepeatRate
AUTO_API bool          IsKeyReleased(int user_key_index);                                  // was key released (went from Down to !Down)..
AUTO_API int           GetKeyPressedAmount(int key_index, float repeat_delay, float rate); // uses provided repeat rate/delay. return a count, most often 0 or 1 but might be >1 if RepeatRate is small enough that DeltaTime > RepeatRate
AUTO_API bool          IsMouseDown(int button);                                            // is mouse button held (0=left, 1=right, 2=middle)
AUTO_API bool          IsAnyMouseDown();                                                   // is any mouse button held
AUTO_API bool          IsMouseClicked(int button, bool repeat = false);                    // did mouse button clicked (went from !Down to Down) (0=left, 1=right, 2=middle)
AUTO_API bool          IsMouseDoubleClicked(int button);                                   // did mouse button double-clicked. a double-click returns false in IsMouseClicked(). uses io.MouseDoubleClickTime.
AUTO_API bool          IsMouseReleased(int button);                                        // did mouse button released (went from Down to !Down)
AUTO_API bool          IsMouseDragging(int button = 0, float lock_threshold = -1.0f);      // is mouse dragging. if lock_threshold < -1.0f uses io.MouseDraggingThreshold
AUTO_API bool          IsMouseHoveringRect(const Vector2F& r_min, const Vector2F& r_max, bool clip = true);  // is mouse hovering given bounding rect (in screen space). clipped by current clipping settings, but disregarding of other consideration of focus/window ordering/popup-block.
AUTO_API bool          IsMousePosValid(const Vector2F* mouse_pos = NULL);                  // by convention we use (-FLT_MAX,-FLT_MAX) to denote that there is no mouse
AUTO_API Vector2F      GetMousePos();                                                      // shortcut to ImGui::GetIO().MousePos provided by user, to be consistent with other calls
AUTO_API Vector2F      GetMousePosOnOpeningCurrentPopup();                                 // retrieve backup of mouse position at the time of opening popup we have BeginPopup() into
AUTO_API Vector2F      GetMouseDragDelta(int button = 0, float lock_threshold = -1.0f);    // return the delta from the initial clicking position while the mouse button is pressed or was just released. This is locked and return 0.0f until the mouse moves past a distance threshold at least once. If lock_threshold < -1.0f uses io.MouseDraggingThreshold.
AUTO_API void          ResetMouseDragDelta(int button = 0);                                //
AUTO_API MouseCursor   GetMouseCursor();                                                   // get desired cursor type, reset in ImGui::NewFrame(), this is updated during the frame. valid before Render(). If you use software rendering by setting io.MouseDrawCursor ImGui will render those for you
AUTO_API void          SetMouseCursor(MouseCursor type);								   // set desired cursor type
AUTO_API void          CaptureKeyboardFromApp(bool want_capture_keyboard_value = true);    // attention: misleading name! manually override io.WantCaptureKeyboard flag next frame (said flag is entirely left for your application to handle). e.g. force capture keyboard when your widget is being hovered. This is equivalent to setting "io.WantCaptureKeyboard = want_capture_keyboard_value"; after the next NewFrame() call.
AUTO_API void          CaptureMouseFromApp(bool want_capture_mouse_value = true);          // attention: misleading name! manually override io.WantCaptureMouse flag next frame (said flag is entirely left for your application to handle). This is equivalent to setting "io.WantCaptureMouse = want_capture_mouse_value;" after the next NewFrame() call.

// Clipboard Utilities (also see the LogToClipboard() function to capture or output text data to the clipboard)
AUTO_API const char*   GetClipboardText();
AUTO_API void          SetClipboardText(const char* text);

// Settings/.Ini Utilities
// - The disk functions are automatically called if io.IniFilename != NULL (default is "imgui.ini").
// - Set io.IniFilename to NULL to load/save manually. Read io.WantSaveIniSettings description about handling .ini saving manually.
AUTO_API void          LoadIniSettingsFromDisk(const char* ini_filename);                  // call after CreateContext() and before the first call to NewFrame(). NewFrame() automatically calls LoadIniSettingsFromDisk(io.IniFilename).
AUTO_API void          LoadIniSettingsFromMemory(const char* ini_data, size_t ini_size = 0); // call after CreateContext() and before the first call to NewFrame() to provide .ini data from your own data source.
AUTO_API void          SaveIniSettingsToDisk(const char* ini_filename);                    // this is automatically called (if io.IniFilename is not empty) a few seconds after any modification that should be reflected in the .ini file (and also by DestroyContext).
AUTO_API const char*   SaveIniSettingsToMemory(size_t* out_ini_size = NULL);               // return a zero-terminated string with the .ini data which you can save by your own mean. call when io.WantSaveIniSettings is set, then save data by your own mean and clear io.WantSaveIniSettings.

// Memory Allocators
// - All those functions are not reliant on the current context.
// - If you reload the contents of imgui.cpp at runtime, you may need to call SetCurrentContext() + SetAllocatorFunctions() again because we use global storage for those.
AUTO_API void          SetAllocatorFunctions(void* (*alloc_func)(size_t sz, void* user_data), void (*free_func)(void* ptr, void* user_data), void* user_data = NULL);
AUTO_API void*		   MemAlloc(size_t size);
AUTO_API void          MemFree(void* ptr);

}

}