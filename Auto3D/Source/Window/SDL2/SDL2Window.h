#pragma once

#include "../../Math/IntVector2.h"
#include "../../Object/Object.h"

struct SDL_Window;

namespace Auto3D
{

/// %Window resized event.
class AUTO_API WindowResizeEvent : public Event
{
public:
	/// New window size.
	IntVector2 size;
};

/// Operating system window, Win32 implementation.
class AUTO_API Window : public Object
{
	REGISTER_OBJECT_CLASS(Window, Object)

public:
	/// Construct and register subsystem. The window is not yet opened.
	Window();
	/// Destruct. Close window if open.
	~Window();

	/// Set window title.
	void SetTitle(const String& newTitle);
	/// Set window size. Open the window if not opened yet. Return true on success.
	bool SetSize(const IntVector2& size, bool fullscreen, bool resizable);
	/// Set window position.
	void SetPosition(const IntVector2& position);
	/// Set mouse cursor visible. Default is true. When hidden, the mouse cursor is confined to the window and kept centered; relative mouse motion can be read "endlessly" but absolute mouse position should not be used.
	void SetMouseVisible(bool enable);
	/// Move the mouse cursor to a window top-left relative position.
	void SetMousePosition(const IntVector2& position);
	/// Close the window.
	void Close();
	/// Minimize the window.
	void Minimize();
	/// Maximize the window.
	void Maximize();
	/// Restore window size.
	void Restore();
	/// Pump window messages from the operating system.
	void PumpMessages();

	/// Return window title.
	const String& Title() const { return title; }
	/// Return window client area size.
	const IntVector2& Size() const { return size; }
	/// Return window client area width.
	int Width() const { return size.x; }
	/// Return window client area height.
	int Height() const { return size.y; }
	/// Return window position.
	IntVector2 Position() const;
	/// Return last known mouse cursor position relative to window top-left.
	const IntVector2& MousePosition() const { return mousePosition; }
	/// Return whether window is open.
	bool IsOpen() const { return handle != nullptr; }
	/// Return whether is resizable.
	bool IsResizable() const { return resizable; }
	/// Return whether is fullscren.
	bool IsFullscreen() const { return fullscreen; }
	/// Return whether is currently minimized.
	bool IsMinimized() const { return minimized; }
	/// Return whether has input focus.
	bool HasFocus() const { return focus; }
	/// Return whether mouse cursor is visible.
	bool IsMouseVisible() const { return mouseVisible; }
	/// Return window handle. Can be cast to a HWND.
	SDL_Window* Handle() const { return handle; }

	/// Handle a window message. Return true if handled and should not be passed to the default window procedure.
	bool OnWindowMessage(void* sdlEvent);

	/// Close requested event.
	Event closeRequestEvent;
	/// Gained focus event.
	Event gainFocusEvent;
	/// Lost focus event.
	Event loseFocusEvent;
	/// Minimized event.
	Event minimizeEvent;
	/// Restored after minimization -event.
	Event restoreEvent;
	/// Size changed event.
	WindowResizeEvent resizeEvent;

	/// Window class name
	static String className;

private:
	/// Change display mode. If width and height are zero, will restore desktop resolution.
	void SetDisplayMode(int width, int height);
	/// Update mouse visibility and clipping region to the OS.
	void UpdateMouseVisible();
	/// Update mouse clipping region.
	void UpdateMouseClipping();
	/// Refresh the internally tracked mouse cursor position.
	void UpdateMousePosition();
	/// Verify window size from the window client rect.
	IntVector2 ClientRectSize() const;

	/// Window handle.
	SDL_Window* handle;
	/// Window title.
	String title;
	/// Current client area size.
	IntVector2 size;
	/// Last stored windowed mode position.
	IntVector2 savedPosition;
	/// Current mouse cursor position.
	IntVector2 mousePosition;
	///mouse wheel offset
	IntVector2 mouseWheelOffset;
	///mouse wheel move
	IntVector2 mouseMoveWheel;

	/// Window style flags.
	unsigned windowStyle;
	/// Current minimization state.
	bool minimized;
	/// Current focus state.
	bool focus;
	/// Resizable flag.
	bool resizable;
	/// Fullscreen flag.
	bool fullscreen;
	/// Performing window resize flag. Used internally to suppress resize events during it.
	bool inResize;
	/// Mouse visible flag as requested by the application.
	bool mouseVisible;
	/// Internal mouse visible flag. The mouse is automatically shown when the window is unfocused, while mouseVisible represents the application's desired state. Used to prevent multiple calls to OS mouse visibility functions, which utilize a counter.
	bool mouseVisibleInternal;
};

}
