#include "../Base/WString.h"
#include "../Debug/Log.h"
#include "../Math/Math.h"
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
	handle(nullptr),
	title("Auto3D Window"),
	size(IntVector2::ZERO),
	savedPosition(IntVector2(M_MIN_INT, M_MIN_INT)),
	mousePosition(IntVector2::ZERO),
	mouseWheelOffset(IntVector2::ZERO),
	mouseMoveWheel(IntVector2::ZERO),
	windowStyle(0),
	minimized(false),
	focus(false),
	resizable(false),
	fullscreen(false),
	inResize(false),
	mouseHide(false),
	mouseLock(false),
	mouseVisibleInternal(true)
{
	RegisterSubsystem(this);

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		LOGERROR("Couldn't initialize SDL");
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
	title = newTitle;
}

bool Window::SetSize(const IntVector2& size_, bool fullscreen_, bool resizable_, bool borderless, bool highDPI)
{
	size = size_;
	// Create the window
	unsigned flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
	if (fullscreen_)
		flags |= SDL_WINDOW_FULLSCREEN;
	if (resizable_)
		flags |= SDL_WINDOW_RESIZABLE;
	if (borderless)
		flags |= SDL_WINDOW_BORDERLESS;


	// The position size will be reset later
	handle = SDL_CreateWindow(
		title.CString(),
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		size_.x, size_.y, flags
	);

	if (handle == NULL)
		LOGERROR("Couldn't set video mode");

	return true;
}

void Window::SetPosition(const IntVector2& position)
{

}

void Window::SetMouseHide(bool enable)
{
	if (enable != mouseHide)
	{
		mouseHide = enable;
		
	}
}

void Window::SetMouseLock(bool enable)
{
	if (enable != mouseLock)
	{
		mouseLock = enable;
		SDL_SetWindowGrab(handle, (SDL_bool)enable);
	}
}

void Window::SetMousePosition(const IntVector2& position)
{

}

void Window::Close()
{

}

void Window::Minimize()
{

}

void Window::Maximize()
{

}

void Window::Restore()
{

}

void Window::PumpMessages()
{
	if (!handle)
		return;

	SDL_Event evt;
	while (SDL_PollEvent(&evt))
	{
		OnWindowMessage(&evt);
		
	}
}


IntVector2 Window::Position() const
{
	return IntVector2();
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
		mousePosition.x = x;
		mousePosition.y = y;
		if (input)
			input->OnMouseMove(mousePosition, IntVector2(evt.motion.xrel, evt.motion.yrel));
		//_isMouseMove = true;
		break;
	case SDL_MOUSEBUTTONDOWN:
		break;
	case SDL_MOUSEBUTTONUP:
		break;
	case SDL_MOUSEWHEEL:
		if (input)
			input->OnMouseWheel(IntVector2(evt.wheel.x, evt.wheel.y));
		break;
	case SDL_WINDOWEVENT:
	{
		switch (evt.window.event)
		{
		case SDL_WINDOWEVENT_MINIMIZED:
			//_isMinimized = true;
			break;

		case SDL_WINDOWEVENT_MAXIMIZED:
		case SDL_WINDOWEVENT_RESTORED:
			//_isMinimized = false;
			break;

		case SDL_WINDOWEVENT_RESIZED:
			int x, y;
			SDL_GetWindowSize(handle, &x, &y);
			SendEvent(resizeEvent);
			if (IntVector2(x, y) != size) 
			{
				size = IntVector2(x, y);
				resizeEvent.size = IntVector2(x, y);
				SendEvent(resizeEvent);
			}
			break;
		case SDL_WINDOWEVENT_MOVED:
			//_graphics->OnWindowMoved();
			break;

		default: break;
		}
	}
	break;
	case SDL_QUIT:
		return false;
		break;
	}
	//Handles whether the mouse is visible
	SDL_SetRelativeMouseMode((SDL_bool)mouseHide);

	return true;
}

}