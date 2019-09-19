#include "../../Container/WString.h"
#include "../../Debug/Log.h"
#include "../../Math/Math.h"
#include "../../Resource/Image.h"
#include "../../Graphics/Graphics.h"
#include "../../UI/UI.h"
#include "../../Engine/ModuleManager.h"

#include "WinInput.h"
#include "WinWindow.h"

#include <Windows.h>

#include "../../Debug/DebugNew.h"


namespace Auto3D
{

// Handle missing touch input definitions (MinGW)
#if WINVER < 0x0601
#define TWF_FINETOUCH 1
#define TWF_WANTPALM 2
#define TOUCHEVENTF_MOVE 0x1
#define TOUCHEVENTF_DOWN 0x2
#define TOUCHEVENTF_UP 0x4
#define WM_TOUCH 0x240

	DECLARE_HANDLE(HTOUCHINPUT);

	/// \cond PRIVATE
	typedef struct {
		LONG x;
		LONG y;
		HANDLE hSource;
		DWORD dwID;
		DWORD dwFlags;
		DWORD dwMask;
		DWORD dwTime;
		ULONG_PTR dwExtraInfo;
		DWORD cxContact;
		DWORD cyContact;
	} TOUCHINPUT, * PTOUCHINPUT;
	/// \endcond
#endif

static BOOL(WINAPI* setProcessDpiAware)() = nullptr;
static BOOL(WINAPI* registerTouchWindow)(HWND, ULONG) = nullptr;
static BOOL(WINAPI* getTouchInputInfo)(HTOUCHINPUT, UINT, PTOUCHINPUT, int) = nullptr;
static BOOL(WINAPI* closeTouchInputHandle)(HTOUCHINPUT) = nullptr;
static bool functionsInitialized = false;

static LRESULT CALLBACK WndProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);


String Window::className("Auto3DWindow");

Window::Window() :
	_handle(nullptr),
	_title("Auto3D Window"),
	_savedPosition(Vector2I(M_MIN_INT, M_MIN_INT)),
	_mousePosition(Vector2I::ZERO),
	_mouseWheelOffset(Vector2I::ZERO),
	_mouseMoveWheel(Vector2I::ZERO),
	_rect(RectI::ZERO),
	_close(false),
	_windowStyle(0),
	_multisample(1),
	_minimized(false),
	_focus(false),
	_resizable(false),
	_fullscreen(false),
	_borderless(false),
	_highDPI(false),
	_inResize(false),
	_mouseHide(false),
	_mouseLock(false),
	_mouseVisibleInternal(true)
{
	RegisterModule(this);
}

Window::~Window()
{
	// Really destroy the game form
	DestoryWindow();

	RemoveModule(this);
}

bool Window::InitMsg()
{
	if (!functionsInitialized)
	{
		HMODULE userDll = GetModuleHandle("user32.dll");
		setProcessDpiAware = (BOOL(WINAPI*)())(void*)GetProcAddress(userDll, "SetProcessDPIAware");
		registerTouchWindow = (BOOL(WINAPI*)(HWND, ULONG))(void*)GetProcAddress(userDll, "RegisterTouchWindow");
		getTouchInputInfo = (BOOL(WINAPI*)(HTOUCHINPUT, UINT, PTOUCHINPUT, int))(void*)GetProcAddress(userDll, "GetTouchInputInfo");
		closeTouchInputHandle = (BOOL(WINAPI*)(HTOUCHINPUT))(void*)GetProcAddress(userDll, "CloseTouchInputHandle");

		// Cancel automatic DPI scaling
		if (setProcessDpiAware)
			setProcessDpiAware();

		functionsInitialized = true;
	}
	return true;
}

void Window::SetTitle(const String& newTitle)
{
	_title = newTitle;
	if (_handle)
		SetWindowTextW((HWND)_handle, WString(_title).CString());
}

void Window::DestoryWindow()
{
	if (_handle)
	{
		// Return to desktop resolution if was fullscreen, else save last windowed mode position
		if (_fullscreen)
			SetDisplayMode(0, 0);
		else
			_savedPosition = GetPosition();

		DestroyWindow((HWND)_handle);
		_handle = nullptr;
	}
	else
		ErrorString("Destroy window operation failed and exiting program");
}

void Window::SetIcon(Image* icon)
{
	if(icon)
		_icon = icon;
	if (_handle && _icon)
		CreateWindowIcon();
}

bool Window::SetSize(const RectI& rect, int multisample, bool fullscreen, bool resizable, bool center, bool borderless, bool highDPI)
{
	_inResize = true;
	_fullscreen = fullscreen;
	_resizable = resizable;
	_borderless = borderless;
	_highDPI = highDPI;

	_rect = rect;

	Clamp(multisample, 1, 16);
	_multisample = multisample;
	Vector2I size = Vector2I(rect.Width(), rect.Height());
	Vector2I position = Vector2I(rect.Left(), rect.Top());

	unsigned windowStyle;

	if (!_fullscreen)
	{
		windowStyle = WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX;
		if (_resizable)
			windowStyle |= WS_THICKFRAME | WS_MAXIMIZEBOX;
		

		// Return to desktop resolution if was fullscreen
		if (fullscreen)
			SetDisplayMode(0, 0);
	}
	else
	{
		// When switching to fullscreen, save last windowed mode position
		if (!fullscreen)
			_savedPosition = GetPosition();

		windowStyle = WS_POPUP | WS_VISIBLE;
		position = Vector2I::ZERO;
		/// \todo Handle failure to set mode
		SetDisplayMode(size._x, size._y);
	}

	RECT winRect = { 0, 0, size._x, size._y };
	AdjustWindowRect(&winRect, windowStyle, false);

	if (!_handle)
	{
		WNDCLASS wc;
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.lpfnWndProc = WndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = GetModuleHandle(0);
		wc.hIcon = LoadIcon(0, IDI_APPLICATION);
		wc.hCursor = LoadCursor(0, IDC_ARROW);
		wc.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
		wc.lpszMenuName = nullptr;
		wc.lpszClassName = className.CString();

		RegisterClass(&wc);

		_handle = CreateWindowW(WString(className).CString(), WString(_title).CString(), windowStyle, position._x, position._y,
			winRect.right - winRect.left, winRect.bottom - winRect.top, 0, 0, GetModuleHandle(0), nullptr);
		if (!_handle)
		{
			ErrorString("Failed to create window");
			_inResize = false;
			return false;
		}

		// Enable touch input if available
		if (registerTouchWindow)
			registerTouchWindow((HWND)_handle, TWF_FINETOUCH | TWF_WANTPALM);

		_minimized = false;
		_focus = false;

		SetWindowLongPtr((HWND)_handle, GWLP_USERDATA, (LONG_PTR)this);
		ShowWindow((HWND)_handle, SW_SHOW);
	}
	else
	{
		SetWindowLong((HWND)_handle, GWL_STYLE, windowStyle);

		if (!_fullscreen && (_savedPosition._x == M_MIN_INT || _savedPosition._y == M_MIN_INT))
		{
			WINDOWPLACEMENT placement;
			placement.length = sizeof(placement);
			GetWindowPlacement((HWND)_handle, &placement);
			position = Vector2I(placement.rcNormalPosition.left, placement.rcNormalPosition.top);
		}

		SetWindowPos((HWND)_handle, NULL, position._x, position._y, winRect.right - winRect.left, winRect.bottom - winRect.top, SWP_NOZORDER);
		ShowWindow((HWND)_handle, SW_SHOW);
	}

	fullscreen = _fullscreen;
	resizable = _resizable;
	_inResize = false;

	Vector2I newSize = ClientRectSize();
	if (newSize != Vector2I(_rect.Width(), _rect.Height()))
	{
		_rect.Right() = _rect.Left() + size._x;
		_rect.Bottom() = _rect.Top() + size._y;

		_resizeEvent._size = newSize;
		SendEvent(_resizeEvent);
	}

	UpdateMouseVisible();
	UpdateMousePosition();

	return true;
}

void Window::SetPosition(const Vector2I& position)
{
	if (_handle)
	{
		SetWindowPos((HWND)_handle, NULL, position._x, position._y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	}
}

void Window::SetMouseHide(bool enable)
{
	if (enable != _mouseHide)
	{
		_mouseHide = enable;
	}
}

void Window::SetMouseLock(bool enable)
{
	enable = !enable;
	if (enable != _mouseLock)
	{
		_mouseLock = enable;
		UpdateMouseVisible();
	}
}

void Window::SetMousePosition(const Vector2I& position)
{
	if (_handle)
	{
		_mousePosition = position;
		POINT screenPosition;
		screenPosition.x = position._x;
		screenPosition.y = position._y;
		ClientToScreen((HWND)_handle, &screenPosition);
		SetCursorPos(screenPosition.x, screenPosition.y);
	}
}
void Window::CreateWindowIcon()
{
	if (_icon)
	{

		/*SDL_Surface* surface = _icon->GetSDLSurface();
		if (surface)
		{
			SDL_SetWindowIcon(_handle, surface);
			SDL_FreeSurface(surface);
		}*/
	}
}

void Window::Close()
{
	_close = true;
}

void Window::Raise()
{
	if (!_handle)
		return;

	//SDL_RaiseWindow(_handle);
}

void Window::Minimize()
{
	if (!_handle)
		return;

	ShowWindow((HWND)_handle, SW_MINIMIZE);
}

void Window::Maximize()
{
	if (!_handle)
		return;

	ShowWindow((HWND)_handle, SW_MAXIMIZE);
}

void Window::Restore()
{
	if (!_handle)
		return;

	ShowWindow((HWND)_handle, SW_RESTORE);
}

void Window::PumpMessages()
{
	if (!_handle)
		return;

	MSG msg;

	while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
	{
		//auto ui = ModuleManager::Get()._ui;

		//ui->ProcessEvent(&evt);

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

const Vector2I Window::GetPosition() const
{
	return Vector2I(_rect.Left(),_rect.Top());
}

bool Window::OnWindowMessage(unsigned msg, unsigned wParam, unsigned lParam)
{
	Input* input = ModuleManager::Get()._input;
	bool handled = false;

	// Skip emulated mouse events that are caused by touch
	bool emulatedMouse = (GetMessageExtraInfo() & 0xffffff00) == 0xff515700;

	switch (msg)
	{
	case WM_DESTROY:
		_handle = nullptr;
		break;

	case WM_CLOSE:
		SendEvent(_closeRequestEvent);
		handled = true;
		Close();
		break;

	case WM_ACTIVATE:
	{
		bool newFocus = LOWORD(wParam) != WA_INACTIVE;
		if (newFocus != _focus)
		{
			_focus = newFocus;
			if (_focus)
			{
				SendEvent(_gainFocusEvent);
				if (input)
					input->OnGainFocus();
				if (_minimized)
					Restore();

				// If fullscreen, automatically restore mouse focus
				if (_fullscreen)
					UpdateMouseVisible();
			}
			else
			{
				SendEvent(_loseFocusEvent);
				if (input)
					input->OnLoseFocus();

				// If fullscreen, minimize on focus loss
				if (_fullscreen)
					ShowWindow((HWND)_handle, SW_MINIMIZE);

				// Stop mouse cursor hiding & clipping
				UpdateMouseVisible();
			}
		}
	}
	break;

	case WM_SIZE:
	{
		bool newMinimized = (wParam == SIZE_MINIMIZED);
		if (newMinimized != _minimized)
		{
			_minimized = newMinimized;
			if (_minimized)
			{
				// If is fullscreen, restore desktop resolution
				if (_fullscreen)
					SetDisplayMode(0, 0);

				SendEvent(_minimizeEvent);
			}
			else
			{
				// If should be fullscreen, restore mode now
				if (_fullscreen)
					SetDisplayMode(_rect.Width(), _rect.Height());

				SendEvent(_restoreEvent);
			}
		}

		if (!_minimized && !_inResize)
		{
			Vector2I newSize = ClientRectSize();
			if (newSize != Vector2I(_rect.Width(),_rect.Height()))
			{
				_rect.Right() = _rect.Left() + newSize._x;
				_rect.Bottom() = _rect.Top() + newSize._y;

				_resizeEvent._size = newSize;
				SendEvent(_resizeEvent);
			}
		}

		// If mouse is currently hidden, update the clip region
		if (!_mouseVisibleInternal)
			UpdateMouseClipping();
	}
	break;

	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		if (input)
			input->OnKey(wParam, (lParam >> 16) & 0xff, true);
		handled = (msg == WM_KEYDOWN);
		break;

	case WM_KEYUP:
	case WM_SYSKEYUP:
		if (input)
			input->OnKey(wParam, (lParam >> 16) & 0xff, false);
		handled = (msg == WM_KEYUP);
		break;

	case WM_CHAR:
		if (input)
			input->OnChar(wParam);
		handled = true;
		break;

	case WM_MOUSEMOVE:
		if (input && !emulatedMouse)
		{
			Vector2I newPosition;
			newPosition._x = (int)(short)LOWORD(lParam);
			newPosition._y = (int)(short)HIWORD(lParam);

			// Do not transmit mouse move when mouse should be hidden, but is not due to no input focus
			if (_mouseVisibleInternal == _mouseLock)
			{
				Vector2I delta = newPosition - _mousePosition;
				input->OnMouseMove(newPosition, delta);
				// Recenter in hidden mouse cursor mode to allow endless relative motion
				if (!_mouseVisibleInternal && delta != Vector2I::ZERO)
					SetMousePosition(Vector2I(_rect.Width(), _rect.Height()) / 2);
				else
					_mousePosition = newPosition;
			}
			else
				_mousePosition = newPosition;
		}
		handled = true;
		break;

	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
		if (input && !emulatedMouse)
		{
			unsigned button = (msg == WM_LBUTTONDOWN) ? MOUSEB_LEFT : (msg == WM_MBUTTONDOWN) ? MOUSEB_MIDDLE : MOUSEB_RIGHT;
			input->OnMouseButton(button, true);
			// Make sure we track the button release even if mouse moves outside the window
			SetCapture((HWND)_handle);
			// Re-establish mouse cursor hiding & clipping
			if (!_mouseLock && _mouseVisibleInternal)
				UpdateMouseVisible();
		}
		handled = true;
		break;

	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
		if (input && !emulatedMouse)
		{
			unsigned button = (msg == WM_LBUTTONUP) ? MOUSEB_LEFT : (msg == WM_MBUTTONUP) ? MOUSEB_MIDDLE : MOUSEB_RIGHT;
			input->OnMouseButton(button, false);
			// End capture when there are no more mouse buttons held down
			if (!input->GetMouseButtons())
				ReleaseCapture();
		}
		handled = true;
		break;

	case WM_TOUCH:
		if (input && LOWORD(wParam))
		{
			Vector<TOUCHINPUT> touches(LOWORD(wParam));
			if (getTouchInputInfo((HTOUCHINPUT)lParam, (unsigned)touches.Size(), &touches[0], sizeof(TOUCHINPUT)))
			{
				for (auto it = touches.Begin(); it != touches.End(); ++it)
				{
					// Translate touch points inside window
					POINT point;
					point.x = it->x / 100;
					point.y = it->y / 100;
					ScreenToClient((HWND)_handle, &point);
					Vector2I position(point.x, point.y);

					if (it->dwFlags & (TOUCHEVENTF_DOWN || TOUCHEVENTF_UP))
						input->OnTouch(it->dwID, true, position, 1.0f);
					else if (it->dwFlags & TOUCHEVENTF_UP)
						input->OnTouch(it->dwID, false, position, 1.0f);

					// Mouse cursor will move to the position of the touch on next move, so move beforehand
					// to prevent erratic jumps in hidden mouse mode
					if (!_mouseVisibleInternal)
						_mousePosition = position;
				}
			}
		}

		closeTouchInputHandle((HTOUCHINPUT)lParam);
		handled = true;
		break;

	case WM_SYSCOMMAND:
		// Prevent system bell sound from Alt key combinations
		if ((wParam & 0xff00) == SC_KEYMENU)
			handled = true;
		break;
	}

	return handled;
}

void Window::SetDisplayMode(int width, int height)
{
	if (width && height)
	{
		DEVMODE screenMode;
		screenMode.dmSize = sizeof screenMode;
		screenMode.dmPelsWidth = width;
		screenMode.dmPelsHeight = height;
		screenMode.dmBitsPerPel = 32;
		screenMode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
		ChangeDisplaySettings(&screenMode, CDS_FULLSCREEN);
	}
	else
		ChangeDisplaySettings(nullptr, CDS_FULLSCREEN);
}

void Window::UpdateMouseVisible()
{
	if (!_handle)
		return;

	// When the window is unfocused, mouse should never be hidden
	bool newMouseVisible = HasFocus() ? _mouseLock : true;
	if (newMouseVisible != _mouseVisibleInternal)
	{
		ShowCursor(newMouseVisible ? TRUE : FALSE);
		_mouseVisibleInternal = newMouseVisible;
	}

	UpdateMouseClipping();
}

void Window::UpdateMouseClipping()
{
	if (_mouseVisibleInternal)
		ClipCursor(nullptr);
	else
	{
		RECT mouseRect;
		POINT point;
		Vector2I windowSize = Vector2I(_rect.Width(), _rect.Height());

		point.x = point.y = 0;
		ClientToScreen((HWND)_handle, &point);
		mouseRect.left = point.x;
		mouseRect.top = point.y;
		mouseRect.right = point.x + windowSize._x;
		mouseRect.bottom = point.y + windowSize._y;
		ClipCursor(&mouseRect);
	}
}

Vector2I Window::ClientRectSize() const
{
	if (_handle)
	{
		RECT rect;
		GetClientRect((HWND)_handle, &rect);
		return Vector2I(rect.right, rect.bottom);
	}
	else
		return Vector2I::ZERO;
}

void Window::UpdateMousePosition()
{
	POINT screenPosition;
	GetCursorPos(&screenPosition);
	ScreenToClient((HWND)_handle, &screenPosition);
	_mousePosition._x = screenPosition.x;
	_mousePosition._y = screenPosition.y;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Window* window = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	bool handled = false;
	// When the window is just opening and has not assigned the userdata yet, let the default procedure handle the messages
	if (window)
		handled = window->OnWindowMessage(msg, (unsigned)wParam, (unsigned)lParam);
	return handled ? 0 : DefWindowProc(hwnd, msg, wParam, lParam);
}

}