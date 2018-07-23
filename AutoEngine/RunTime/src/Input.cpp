#include "Input.h"
#include "SDL2/SDL.h"
#include "GLWindow.h"
AUTO_BEGIN
SINGLETON_INSTANCE(Input);
Input::Input()
	:_isMouseMove(false)
{
}


Input::~Input()
{
}

void Input::HandleWindowEvents()
{


}

void Input::Update()
{
	SDL_Event evt;
	while (SDL_PollEvent(&evt))
	{
		HandleSDLEvent(&evt);
		int MidX = 1280 / 2;   //middle of the screen
		int MidY = 720 / 2;
		int tmpx, tmpy;
		SDL_GetMouseState(&tmpx, &tmpy); //get the current position of the cursor
		//camYaw += mousevel * (MidX - tmpx);   //get the rotation, for example, if the mouse current position is 315, than 5*0.2, this is for Y
		//camPitch += mousevel * (MidY - tmpy); //this is for X
		//lockCamera();
		//SDL_WarpMouse(MidX, MidY);       //move back the cursor to the center of the screen
		SDL_WarpMouseInWindow(INSTANCE(GLWindow).GetGLWindow(), MidX, MidY);

	}
		
}

void Input::HandleSDLEvent(void* sdlEvent)
{
	SDL_Event& evt = *static_cast<SDL_Event*>(sdlEvent);
	switch (evt.type)
	{
	case SDL_KEYDOWN:
		SetKey(evt.key.keysym.sym,true); break;
	case SDL_KEYUP:
		SetKey(evt.key.keysym.sym, false); break;
	case SDL_MOUSEMOTION:
		int x, y;
		SDL_GetMouseState(&x, &y);
		_mousePosition.x = x;
		_mousePosition.y = y;
		_mouseMove.x = evt.motion.xrel;
		_mouseMove.y = -evt.motion.yrel;
		_isMouseMove = true;
	case SDL_MOUSEBUTTONDOWN:

	case SDL_MOUSEBUTTONUP:

	case SDL_MOUSEWHEEL:

	case SDL_QUIT:
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
void Input::EndFrame()
{
	_keysDown.clear();
	_mouseMove.x = 0;
	_mouseMove.y = 0;
	_isMouseMove = false;
}

bool Input::GetKeyDown(int key)
{
	return !(_keysDown.find(key) == _keysDown.end());
}
bool Input::GetKeyPress(int key)
{
	return !(_keysPress.find(key) == _keysPress.end());
}
AUTO_END