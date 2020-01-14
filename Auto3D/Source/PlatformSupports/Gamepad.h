#pragma once
#include "AutoConfig.h"
#include "PlatformSupports/PlatformDef.h"

#include <SDL.h>
#include <SDL_syswm.h>

#include <stdint.h>

namespace Auto3D
{
struct FGamepad
{
	FGamepad();

	void Create(const SDL_JoyDeviceEvent& _jev);

	void Create(const SDL_ControllerDeviceEvent& _cev);

	void Update(EventQueue& _eventQueue, WindowHandle _handle, GamepadHandle _gamepad, GamepadAxis::Enum _axis, int32_t _value);

	void Destroy();

	bool Filter(GamepadAxis::Enum _axis, int32_t* _value);

	int32_t _value[GamepadAxis::Count];
	int32_t _deadzone[GamepadAxis::Count];

	SDL_Joystick*       _joystick;
	SDL_GameController* _controller;
	//		SDL_Haptic*         m_haptic;
	SDL_JoystickID      _jid;

	

};

}