#pragma once

#include "Math/Vector2.h"
#include "Core/Object.h"

namespace Auto3D
{

class AImage;

struct AUTO_API WindowModeDesc
{
	WindowModeDesc():
		_size(TRectI(0,0,1024, 768)),
		_multisample(1),
		_fullscreen(false),
		_resizable(false),
		_center(true),
		_borderless(false),
		_highDPI(false)
	{
	}
	/// Window position and size.
	TRectI _size;
	/// Multis sample num point;
	int _multisample;
	/// Window is full screen.
	bool _fullscreen;
	/// Window is resizable.
	bool _resizable;
	/// Window is center in screen.
	bool _center;
	/// Window is borderless.
	bool _borderless;
	/// Window is highDPI.
	bool _highDPI;

};
/// Window resized event.
class AUTO_API WindowResizeEvent : public FEvent
{
public:
	///New _window size.
	TVector2I _size;
};

/// Operating system _window, Win32 implementation.
class AUTO_API Window : public AObject
{
	REGISTER_OBJECT_CLASS(Window, AObject)

#ifdef AUTO_OPENGL
	friend class GLContext;
#endif

public:
	/// Construct and register subsystem. The _window is not yet opened.
	Window();
	/// Destruct. Close _window if open.
	~Window();
	/// Initializes the opengl context version and its support
	bool InitMsg();
	/// Delete game window and if OpenGL delete context
	void DestoryWindow();
	/// Set window title.
	void SetTitle(const FString& newTitle);
	/// Set window icon
	void SetIcon(AImage* icon);
	/// Set _window _size. Open the _window if not opened yet. Return true on success.
	bool SetSize(const TRectI& rect, int multisample = 1, bool fullscreen = false, bool resizable = false,bool center = true, bool borderless = false, bool highDPI = false);
	/// Set _window _position.
	void SetPosition(const TVector2I& position);
	/// Set mouse cursor visible. Default is true. When hidden, the mouse cursor is confined to the _window and kept centered; relative mouse motion can be read "endlessly" but absolute mouse _position should not be used.
	void SetMouseHide(bool enable);
	/// Set mouse cusor lock in window
	void SetMouseLock(bool enable);
	/// Move the mouse cursor to a _window top-left relative _position.
	void SetMousePosition(const TVector2I& position);
	/// Set multi sample point num
	void SetMultisample(unsigned multi) { _multisample = multi; }
	/// Create window icon
	void CreateWindowIcon();
	/// Close the _window.
	void Close();
	/// Raises window if it was minimized.
	void Raise();
	/// Minimize the window.
	void Minimize();
	/// Maximize the window.
	void Maximize();
	/// Restore window size.
	void Restore();
	/// Pump _window messages from the operating system.
	void PumpMessages();

	/// Return window title.
	const FString& GetTitle() const { return _title; }
	/// Return window rect
	const TRectI& GetRect() const { return _rect; }
	/// Return _window client area _size.
	const TVector2I GetSize() const { return TVector2I(_rect.Width(), _rect.Height()); }
	/// Return _window client area width.
	int GetWidth() const { return _rect.Width(); }
	/// Return _window client area height.
	int GetHeight() const { return _rect.Height(); }
	/// Return _window _position.
	const TVector2I GetPosition() const;
	/// Return last known mouse cursor _position relative to _window top-left.
	const TVector2I& GetMousePosition() const { return _mousePosition; }
	/// Return multisample num.
	int GetMultisample() { return _multisample; }
	/// Return close flags
	bool IsCloseing() const { return _close; }
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
	void* Handle() const { return _handle; }
	/// Handle a window message. Return true if handled and should not be passed to the default _window procedure.
	bool OnWindowMessage(unsigned msg, unsigned wParam, unsigned lParam);
	/// Return window mode desc.
	WindowModeDesc& ModeDesc() { return _windowModeDesc; }
	/// Close requested event.
	FEvent _closeRequestEvent;
	/// Gained focus event.
	FEvent _gainFocusEvent;
	/// Lost _focus event.
	FEvent _loseFocusEvent;
	/// Maximized event.
	FEvent _maximizeEvent;
	/// Minimized event.
	FEvent _minimizeEvent;
	/// Restored after minimization event.
	FEvent _restoreEvent;
	/// Size changed event.
	WindowResizeEvent _resizeEvent;

	/// Window class name
	static FString className;
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
	TVector2I ClientRectSize() const;

	/// Window handle.
	void* _handle;
	/// Window icon image.
	TWeakPtr<AImage> _icon;
	/// Window _title.
	FString _title;
	/// Window rect
	TRectI _rect;
	/// Last stored windowed mode position.
	TVector2I _savedPosition;
	/// Current mouse cursor position.
	TVector2I _mousePosition;
	/// Mouse wheel offset
	TVector2I _mouseWheelOffset;
	/// Mouse wheel move
	TVector2I _mouseMoveWheel;

	/// Window close flag
	bool _close;
	/// Window style flags.
	unsigned _windowStyle;
	/// window multi sample num
	unsigned _multisample;
	/// Current minimization state.
	bool _minimized;
	/// Current _focus state.
	bool _focus;
	/// Resizable flag.
	bool _resizable;
	/// Fullscreen flag.
	bool _fullscreen;
	/// Window borderless.
	bool _borderless;
	/// Support high DPI.
	bool _highDPI;
	/// Performing _window resize flag. Used internally to suppress resize events during it.
	bool _inResize;
	/// Mouse visible flag as requested by the application.
	bool _mouseHide;
	/// Mouse lock in window flag
	bool _mouseVisible;
	/// Internal mouse visible flag. The mouse is automatically shown when the _window is unfocused, while mouseVisible represents the application's desired state. Used to prevent multiple calls to OS mouse visibility functions, which utilize a counter.
	bool _mouseVisibleInternal;
	/// Store the window mode information, and then create the mode.
	WindowModeDesc _windowModeDesc;
};

}
