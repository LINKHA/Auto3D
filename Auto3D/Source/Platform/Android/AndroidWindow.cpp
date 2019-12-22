#include "AutoConfig.h"
#ifdef AUTO_SDL
#include "Container/WString.h"
#include "Debug/Log.h"
#include "Math/AutoMath.h"
#include "Engine/Components/Image.h"
#include "Graphics/Graphics.h"
#include "UI/UI.h"
#include "Core/Modules/ModuleManager.h"
#include "Event/EventManager.h"

#include "AndroidInput.h"
#include "AndroidWindow.h"

#include <SDL.h>
#include <SDL_stdinc.h>
#include <SDL_scancode.h>
#include <SDL_syswm.h>

#include "../../Debug/DebugNew.h"

namespace Auto3D
{

AWindow::AWindow() :
	_handle(nullptr),
	_title("Auto3D Window"),
	_savedPosition(TVector2I(M_MIN_INT, M_MIN_INT)),
	_mousePosition(TVector2I::ZERO),
	_mouseWheelOffset(TVector2I::ZERO),
	_mouseMoveWheel(TVector2I::ZERO),
	_rect(TRectI::ZERO),
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
}

AWindow::~AWindow()
{
	// Really destroy the game form
	DestoryWindow();
}

bool AWindow::InitMsg()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
	{
		ErrorString(SDL_GetError());
		return false;
	}
	atexit(SDL_Quit);
	SDL_GL_LoadLibrary(NULL);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);

	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

#if defined(AUTO_OPENGL)
	auto graphics = GModuleManager::Get().GraphicsModule();

#	ifndef AUTO_OPENGL_ES
	if (graphics->GetGraphicsApiVersion() == EGraphicsVersion::OPENGL_4_3)
	{
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	}
	else if (graphics->GetGraphicsApiVersion() == EGraphicsVersion::OPENGL_3_3)
	{
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	}
	else
	{
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	}
#	else
	if (graphics->GetGraphicsApiVersion() == EGraphicsVersion::OPENGL_ES_3_0)
	{
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	}
	else
	{
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	}
#	endif

	
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
#endif

	return true;
}

void AWindow::SetTitle(const FString& newTitle)
{
	_title = newTitle;
}

void AWindow::DestoryWindow()
{
	if (_handle)
	{
		SDL_DestroyWindow(_handle);
		_handle = nullptr;
		SDL_Quit();
	}
	else
		ErrorString("Destroy window operation failed and exiting program");
}

void AWindow::SetIcon(AImage* icon)
{
	if(icon)
		_icon = icon;
	if (_handle && _icon)
		CreateWindowIcon();
}

bool AWindow::SetSize(const TRectI& rect, int multisample, bool fullscreen, bool resizable, bool center, bool borderless, bool highDPI)
{
	_inResize = true;
	_fullscreen = fullscreen;
	_resizable = resizable;
	_borderless = borderless;
	_highDPI = highDPI;
	
	_rect = rect;
	_multisample = multisample;
	/// Set MSAA
	if (_multisample > 1)
	{
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, _multisample);
	}
	else
	{
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 0);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 0);
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
		}
	}
	// Create the _window
	unsigned windowStyle =  SDL_WINDOW_SHOWN;
	if (fullscreen)
		windowStyle |= SDL_WINDOW_FULLSCREEN;
	if (resizable)
		windowStyle |= SDL_WINDOW_RESIZABLE;
	if (borderless)
		windowStyle |= SDL_WINDOW_BORDERLESS;
	if (highDPI)
		windowStyle |= SDL_WINDOW_ALLOW_HIGHDPI;
#if defined(AUTO_OPENGL)
	windowStyle |= SDL_WINDOW_OPENGL;
#elif defined(AUTO_VULKAN)
	windowStyle |= SDL_WINDOW_VULKAN;
#endif

	// The _position _size will be reset later
	_handle = SDL_CreateWindow(
		_title.CString(),
		_rect.Left(), _rect.Top(),
		_rect.Width(), _rect.Height(), windowStyle
	);

	if (_handle == NULL)
		ErrorString("Couldn't set video mode");

	_inResize = false;
	
	return true;
}

void AWindow::SetPosition(const TVector2I& position)
{
	if (_handle)
	{
		SDL_SetWindowPosition(_handle, position._x, position._y);
	}
}

void AWindow::SetMouseHide(bool enable)
{
	if (enable != _mouseHide)
	{
		_mouseHide = enable;
	}
}

void AWindow::SetMouseLock(bool enable)
{
	if (enable != _mouseLock)
	{
		_mouseLock = enable;
		SDL_SetWindowGrab(_handle, (SDL_bool)enable);
	}
}

void AWindow::SetMousePosition(const TVector2I& position)
{
	if (_handle)
	{
		_mousePosition = position;
		SDL_WarpMouseInWindow(_handle, position._x, position._y);
	}
}
void AWindow::CreateWindowIcon()
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

void AWindow::Close()
{
	_close = true;
}

void AWindow::Raise()
{
	if (!_handle)
		return;

	SDL_RaiseWindow(_handle);
}

void AWindow::Minimize()
{
	if (!_handle)
		return;

	SDL_MinimizeWindow(_handle);
}

void AWindow::Maximize()
{
	if (!_handle)
		return;

	SDL_MaximizeWindow(_handle);
}

void AWindow::Restore()
{
	if (!_handle)
		return;

	SDL_RestoreWindow(_handle);
}

void AWindow::PumpMessages()
{
	if (!_handle)
		return;

	SDL_Event evt;
	while (SDL_PollEvent(&evt))
	{
		auto ui = GModuleManager::Get().UiModule();

		ui->ProcessEvent(&evt);
		OnWindowMessage(&evt);
	}
}

const TVector2I AWindow::GetPosition() const
{
	return TVector2I(_rect.Left(),_rect.Top());
}

bool AWindow::OnWindowMessage(void* sdlEvent)
{
	auto input = GModuleManager::Get().InputModule();

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
			input->OnMouseMove(_mousePosition,TVector2I(evt.motion.xrel, evt.motion.yrel));
		break;
	case SDL_MOUSEBUTTONDOWN:
		break;
	case SDL_MOUSEBUTTONUP:
		break;
	case SDL_MOUSEWHEEL:
		if (input)
			input->OnMouseWheel(TVector2I(evt.wheel.x, evt.wheel.y));
		break;
	case SDL_WINDOWEVENT:
	{
		switch (evt.window.event)
		{
		case SDL_WINDOWEVENT_MINIMIZED:
			GEventManager::Get().SendEvent(this, _minimizeEvent);
			_minimized = true;
			break;
		case SDL_WINDOWEVENT_MAXIMIZED:
			GEventManager::Get().SendEvent(this, _maximizeEvent);
		case SDL_WINDOWEVENT_RESTORED:
			_minimized = false;
			break;

		case SDL_WINDOWEVENT_RESIZED:
			int newWidth, newHeight;
			SDL_GetWindowSize(_handle, &newWidth, &newHeight);
			if (TVector2I(newWidth, newHeight) != GetSize()) 
			{
				_rect.Right() = _rect.Left() + newWidth;
				_rect.Bottom() = _rect.Top() + newHeight;
				_resizeEvent._size = TVector2I(newWidth, newHeight);
				GEventManager::Get().SendEvent(this, _resizeEvent);
			}
			break;
		case SDL_WINDOWEVENT_MOVED:
			break;

		default: break;
		}
	}
	break;
	case SDL_QUIT:
		GEventManager::Get().SendEvent(this, _closeRequestEvent);
		Close();
		break;
	}
	//Handles whether the mouse is visible
	SDL_SetRelativeMouseMode((SDL_bool)_mouseHide);

	return true;
}

}
#endif