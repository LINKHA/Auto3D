#pragma once

#include "../Math/Vector2.h"
#include "../Object/Object.h"

struct SDL_Window;

namespace Auto3D
{

/// %Window resized _event.
class AUTO_API WindowResizeEvent : public Event
{
public:
	/// New _window _size.
	Vector2I _size;
};

/// Operating system _window, Win32 implementation.
class AUTO_API Window : public Object
{
	REGISTER_OBJECT_CLASS(Window, Object)

public:
	/// Construct and register subsystem. The _window is not yet opened.
	Window();
	/// Destruct. Close _window if open.
	~Window();

	/// Set _window _title.
	void SetTitle(const String& newTitle);
	/// Set _window _size. Open the _window if not opened yet. Return true on success.
	bool SetSize(const Vector2I& size, bool fullscreen = false, bool resizable = false, bool borderless = false, bool highDPI = false);
	/// Set _window _position.
	void SetPosition(const Vector2I& position);
	/// Set mouse cursor visible. Default is true. When hidden, the mouse cursor is confined to the _window and kept centered; relative mouse motion can be read "endlessly" but absolute mouse _position should not be used.
	void SetMouseHide(bool enable);

	void SetMouseLock(bool enable);
	/// Move the mouse cursor to a _window top-left relative _position.
	void SetMousePosition(const Vector2I& position);
	/// Close the _window.
	void Close();
	/// Minimize the _window.
	void Minimize();
	/// Maximize the _window.
	void Maximize();
	/// Restore _window _size.
	void Restore();
	/// Pump _window messages from the operating system.
	void PumpMessages();

	/// Return _window _title.
	const String& Title() const { return _title; }
	/// Return _window client area _size.
	const Vector2I& Size() const { return _size; }
	/// Return _window client area width.
	int Width() const { return _size._x; }
	/// Return _window client area height.
	int Height() const { return _size._y; }
	/// Return _window _position.
	Vector2I Position() const;
	/// Return last known mouse cursor _position relative to _window top-left.
	const Vector2I& MousePosition() const { return _mousePosition; }
	/// Return whether _window is open.
	bool IsOpen() const { return _handle != nullptr; }
	/// Return whether is _resizable.
	bool IsResizable() const { return _resizable; }
	/// Return whether is fullscren.
	bool IsFullscreen() const { return _fullscreen; }
	/// Return whether is currently _minimized.
	bool IsMinimized() const { return _minimized; }
	/// Return whether has input _focus.
	bool HasFocus() const { return _focus; }
	/// Return whether mouse cursor is visible.
	bool IsMouseHide() const { return _mouseHide; }
	/// Return _window _handle. Can be cast to a HWND.
	SDL_Window* Handle() const { return _handle; }

	/// Handle a _window message. Return true if handled and should not be passed to the default _window procedure.
	bool OnWindowMessage(void* sdlEvent);

	/// Close requested _event.
	Event closeRequestEvent;
	/// Gained _focus _event.
	Event gainFocusEvent;
	/// Lost _focus _event.
	Event loseFocusEvent;
	/// Minimized _event.
	Event minimizeEvent;
	/// Restored after minimization -_event.
	Event restoreEvent;
	/// Size changed _event.
	WindowResizeEvent resizeEvent;

	/// Window class name
	static String className;

private:
	/// Change display mode. If width and height are zero, will restore desktop resolution.
	void SetDisplayMode(int width, int height);

	/// Verify _window _size from the _window client rect.
	/// Window _handle.
	SDL_Window* _handle;
	/// Window _title.
	String _title;
	/// Current client area _size.
	Vector2I _size;
	/// Last stored windowed mode _position.
	Vector2I _savedPosition;
	/// Current mouse cursor _position.
	Vector2I _mousePosition;
	///mouse wheel offset
	Vector2I _mouseWheelOffset;
	///mouse wheel move
	Vector2I _mouseMoveWheel;

	/// Window style flags.
	unsigned _windowStyle;
	/// Current minimization state.
	bool _minimized;
	/// Current _focus state.
	bool _focus;
	/// Resizable flag.
	bool _resizable;
	/// Fullscreen flag.
	bool _fullscreen;
	/// Performing _window resize flag. Used internally to suppress resize events during it.
	bool _inResize;
	/// Mouse visible flag as requested by the application.
	bool _mouseHide;

	bool _mouseLock;
	/// Internal mouse visible flag. The mouse is automatically shown when the _window is unfocused, while mouseVisible represents the application's desired state. Used to prevent multiple calls to OS mouse visibility functions, which utilize a counter.
	bool _mouseVisibleInternal;
};

}
