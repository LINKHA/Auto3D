#include "Input.h"
#include "SDL2/SDL.h"
AUTO_BEGIN
SINGLETON_INSTANCE(Input);
Input::Input()
{
}


Input::~Input()
{
}

void Input::HandleWindowEvents()
{
	SDL_Event _event;
	while (SDL_PollEvent(&_event))
	{
		if (_event.type == SDL_QUIT)
		{
		
		}
		if (_event.type == SDL_KEYDOWN)
		{
			if (_event.key.keysym.sym == SDLK_a)
			{
				Print(1);
			}
			if (_event.key.keysym.sym == SDLK_s)
			{

				Print(2);
			}

		}
	}
}

void Input::Update()
{
	SDL_Event evt;
	while (SDL_PollEvent(&evt))
		HandleSDLEvent(&evt);

}

void Input::HandleSDLEvent(void* sdlEvent)
{
	SDL_Event& evt = *static_cast<SDL_Event*>(sdlEvent);
	switch (evt.type)
	{
	case SDL_KEYDOWN:
		//SendEvent
		SetKey(evt.key.keysym.sym,true);
		break;
	case SDL_KEYUP:
		SetKey(evt.key.keysym.sym, false);
		//SendEvent
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