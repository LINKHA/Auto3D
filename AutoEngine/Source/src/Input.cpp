#include "Input.h"
#include "AutoSDL.h"
#include "Graphics.h"
#include "NewDef.h"

namespace Auto3D {
Input::Input(SharedPtr<Ambient> ambient)
	: Super(ambient)
	, _isMouseMove(true)
	, _isLockCursor(false)
	, _isHideCursor(false)
	, _isShowCursor(true)
{
}


Input::~Input()
{
}

void Input::Update()
{
	SDL_Event evt;
	while (SDL_PollEvent(&evt))
	{
		handleSDLEvent(&evt);
		hideCursorEvent(_isHideCursor);
		lockCursorEvent();
	}
		
}

void Input::handleSDLEvent(void* sdlEvent)
{
	SDL_Event& evt = *static_cast<SDL_Event*>(sdlEvent);
	switch (evt.type)
	{
	case SDL_KEYDOWN:
		SetKey(evt.key.keysym.sym, true); break;
	case SDL_KEYUP:
		SetKey(evt.key.keysym.sym, false); break;
	case SDL_MOUSEMOTION:
		int x, y;
		SDL_GetMouseState(&x, &y);
		_mousePosition._x = x;
		_mousePosition._y = y;
		_mouseMove._x = evt.motion.xrel;
		_mouseMove._y = -evt.motion.yrel;
		_isMouseMove = true;
		break;
	case SDL_MOUSEBUTTONDOWN:
		break;
	case SDL_MOUSEBUTTONUP:
		break;
	case SDL_MOUSEWHEEL:
		SetWheel(evt.wheel.y);
		break;
	case SDL_WINDOWEVENT:
	{
		switch (evt.window.event)
		{
		case SDL_WINDOWEVENT_MINIMIZED:
			_isMinimized = true;
			break;

		case SDL_WINDOWEVENT_MAXIMIZED:
		case SDL_WINDOWEVENT_RESTORED:
			_isMinimized = false;
			break;

		case SDL_WINDOWEVENT_RESIZED:
			//_graphics->OnWindowResized();
			break;
		case SDL_WINDOWEVENT_MOVED:
			//_graphics->OnWindowMoved();
			break;

		default: break;
		}
	}
	break;
	case SDL_QUIT:
		_isQuit = true;
		break;
	}
}

void Input::SetKey(int key, bool newState)
{
	if (newState)
	{
		_keysDown.insert(key);
		_keysPress.insert(key);
	}
	else
	{
		_keysPress.erase(key);
	}
}
void Input::SetWheel(int delta)
{
	if (delta)
	{
		_mouseWheelOffset = delta;
		_mouseMoveWheel += delta;
	}
}
void Input::EndFrame()
{
	_keysDown.clear();
	_mouseMove._x = 0;
	_mouseMove._y = 0;
	_isMouseMove = false;
	_mouseWheelOffset = 0;
}

bool Input::GetKeyDown(int key)
{
	return !(_keysDown.find(key) == _keysDown.end());
}
bool Input::GetKeyPress(int key)
{
	return !(_keysPress.find(key) == _keysPress.end());
}


void Input::LockCursor(int x, int y)
{
	_lockMousePosition._x = x;
	_lockMousePosition._y = y;
	_isLockCursor = true;
}
void Input::LockCursorInCenter()
{
	_lockMousePosition._x = GetSubSystem<Graphics>()->GetWindowRectInt().Width() / 2;
	_lockMousePosition._y = GetSubSystem<Graphics>()->GetWindowRectInt().Height() / 2;
	_isLockCursor = true;
}
void Input::HideMouseInWindow(bool enable)
{
	_isHideCursor = enable;
}
void Input::ShowCursor(bool enable)
{
	if (enable) 
		SDL_ShowCursor(SDL_ENABLE);
	else if (!enable)
		SDL_ShowCursor(SDL_DISABLE);
	else
		SDL_ShowCursor(SDL_QUERY);
	_isShowCursor = enable;
}

void Input::lockCursorEvent()
{
	if(_isLockCursor)
		SDL_WarpMouseInWindow(GetSubSystem<Graphics>()->GetGameWindow(), _lockMousePosition._x, _lockMousePosition._y);
}

void Input::hideCursorEvent(bool enable)
{
	if (enable)
		SDL_SetRelativeMouseMode(SDL_TRUE);
	else
		SDL_SetRelativeMouseMode(SDL_FALSE);
}

}