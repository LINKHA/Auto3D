#include "../Base/WString.h"
#include "../Debug/Log.h"
#include "../Math/Math.h"
#include "../Resource/Image.h"

#include "Input.h"
#include "Window.h"

#include <SDL.h>
#include <SDL_stdinc.h>
#include <SDL_scancode.h>
#include <SDL_syswm.h>

#include "../Debug/DebugNew.h"

namespace Auto3D
{

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
	_inResize(false),
	_mouseHide(false),
	_mouseLock(false),
	_mouseVisibleInternal(true)
{
	RegisterSubsystem(this);

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		ErrorString("Couldn't initialize SDL");
	atexit(SDL_Quit);
	SDL_GL_LoadLibrary(NULL);

	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	// Also request a depth buffer
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

}

Window::~Window()
{
	Close();
	RemoveSubsystem(this);
}
void Window::SetTitle(const String& newTitle)
{
	_title = newTitle;
}

void Window::SetIcon(Image* icon)
{
	_icon = icon;
	if (_handle)
		CreateWindowIcon();
}

bool Window::SetSize(const RectI& rect, int multisample, bool fullscreen, bool resizable, bool center, bool borderless, bool highDPI)
{
	_rect = rect;
	_multisample = multisample;
	/// Set MSAA
	if (_multisample != 1)
	{
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, _multisample);
	}

	// Init window position
	SDL_Rect windowRect;
	SDL_GetDisplayBounds(0, &windowRect);
	if (fullscreen)
	{
		_rect.Left() = 0;
		_rect.Top() = 0;
		_rect.Right() = windowRect.w;
		_rect.Bottom() = windowRect.h;
	}
	else
	{
		if (center)
		{
			int w = _rect.Width();
			int h = _rect.Height();
			_rect.Left() = windowRect.w / 2 - w / 2;
			_rect.Top() = windowRect.h / 2 - h / 2;
			_rect.Right() = windowRect.w / 2 + w / 2;
			_rect.Bottom() = windowRect.h / 2 + h / 2;

			int a1 = _rect.Left();
			int a2 = _rect.Top();
			int a3 = _rect.Right();
			int a4 = _rect.Bottom();
			int i = 0;
		}
	}
	// Create the _window
	unsigned flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
	if (fullscreen)
		flags |= SDL_WINDOW_FULLSCREEN;
	if (resizable)
		flags |= SDL_WINDOW_RESIZABLE;
	if (borderless)
		flags |= SDL_WINDOW_BORDERLESS;
	if (highDPI)
		flags |= SDL_WINDOW_ALLOW_HIGHDPI;

	// The _position _size will be reset later
	_handle = SDL_CreateWindow(
		_title.CString(),
		_rect.Left(), _rect.Top(),
		_rect.Width(), _rect.Height(), flags
	);

	if (_handle == NULL)
		ErrorString("Couldn't set video mode");


	return true;
}

void Window::SetPosition(const Vector2I& position)
{
	if (_handle)
	{

		SDL_SetWindowPosition(_handle, position._x, position._y);
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
	if (enable != _mouseLock)
	{
		_mouseLock = enable;
		SDL_SetWindowGrab(_handle, (SDL_bool)enable);
	}
}

void Window::SetMousePosition(const Vector2I& position)
{
	if (_handle)
	{
		_mousePosition = position;
		SDL_WarpMouseInWindow(_handle, position._x, position._y);
	}
}
void Window::CreateWindowIcon()
{
	if (_icon)
	{
		SDL_Surface* surface = _icon->GetSDLSurface();
		if (surface)
		{
			SDL_SetWindowIcon(_handle, surface);
			SDL_FreeSurface(surface);
		}
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

	SDL_RaiseWindow(_handle);
}

void Window::Minimize()
{
	if (!_handle)
		return;

	SDL_MinimizeWindow(_handle);
}

void Window::Maximize()
{
	if (!_handle)
		return;

	SDL_MaximizeWindow(_handle);
}

void Window::Restore()
{
	
}

void Window::PumpMessages()
{
	if (!_handle)
		return;

	SDL_Event evt;
	while (SDL_PollEvent(&evt))
	{
		OnWindowMessage(&evt);
	}
}

const Vector2I Window::GetPosition() const
{
	return Vector2I(_rect.Left(),_rect.Top());
}

bool Window::OnWindowMessage(void* sdlEvent)
{
	auto* input = Subsystem<Input>();

	SDL_Event& evt = *static_cast<SDL_Event*>(sdlEvent);
	switch (evt.type)
	{
	case SDL_KEYDOWN:
		if (input)
			input->OnKey(evt.key.keysym.sym, (evt.key.keysym.sym >> 16) & 0xff, true);
		break;
	case SDL_KEYUP:
		if (input)
			input->OnKey(evt.key.keysym.sym, (evt.key.keysym.sym >> 16) & 0xff, false);
		break;
	case SDL_MOUSEMOTION:
		int x, y;
		SDL_GetMouseState(&x, &y);
		_mousePosition._x = x;
		_mousePosition._y = y;
		if (input)
			input->OnMouseMove(_mousePosition, Vector2I(evt.motion.xrel, evt.motion.yrel));
		break;
	case SDL_MOUSEBUTTONDOWN:
		break;
	case SDL_MOUSEBUTTONUP:
		break;
	case SDL_MOUSEWHEEL:
		if (input)
			input->OnMouseWheel(Vector2I(evt.wheel.x, evt.wheel.y));
		break;
	case SDL_WINDOWEVENT:
	{
		switch (evt.window.event)
		{
		case SDL_WINDOWEVENT_MINIMIZED:
			_minimized = true;
			break;
		case SDL_WINDOWEVENT_MAXIMIZED:
		case SDL_WINDOWEVENT_RESTORED:
			_minimized = false;
			break;

		case SDL_WINDOWEVENT_RESIZED:
			int newWidth, newHeight;
			SDL_GetWindowSize(_handle, &newWidth, &newHeight);
			SendEvent(resizeEvent);
			if (Vector2I(newWidth, newHeight) != GetSize()) 
			{
				_rect.Right() = _rect.Left() + newWidth;
				_rect.Bottom() = _rect.Top() + newHeight;
				resizeEvent._size = Vector2I(newWidth, newHeight);
				SendEvent(resizeEvent);
			}
			break;
		case SDL_WINDOWEVENT_MOVED:
			break;

		default: break;
		}
	}
	break;
	case SDL_QUIT:
		Close();
		break;
	}
	//Handles whether the mouse is visible
	SDL_SetRelativeMouseMode((SDL_bool)_mouseHide);

	return true;
}

}